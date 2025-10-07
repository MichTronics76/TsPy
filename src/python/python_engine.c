/**
 * @file python_engine.c
 * @brief Python scripting engine implementation
 * @author TsPy Team
 * @version 1.3.0
 */

/* Undefine _DEBUG to use release Python library */
#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif

#define PY_SSIZE_T_CLEAN

#if defined(WIN32) || defined(__WIN32__) || defined(_WIN32)
#include <Windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "python_engine.h"
#include "python_api.h"
#include "utils/logging.h"
#include "utils/string_utils.h"

/* Python engine state */
static int g_python_initialized = 0;
static char g_python_error[512] = {0};
static char g_scripts_path[512] = {0};
static PyObject* g_main_module = NULL;
static PyObject* g_main_dict = NULL;

/* Forward declarations */
static void set_python_error(const char* msg);
static void clear_python_error(void);

int python_engine_init(const char* plugin_path)
{
    PyConfig config;
    PyStatus status;
    
    if (g_python_initialized) {
        log_warning("Python engine already initialized");
        return 0;
    }

    log_info("Initializing Python engine (safe mode)...");
    clear_python_error();

    /* IMPORTANT: Register ts3api module BEFORE initializing Python */
    log_info("Registering Python API module...");
    if (python_api_init() != 0) {
        set_python_error("Failed to register Python API module");
        log_error("Failed to register Python API module");
        return 1;
    }

    /* Initialize Python configuration - use simplest config that works */
    PyConfig_InitPythonConfig(&config);
    
    /* Disable signal handlers to prevent interference with TeamSpeak */
    config.install_signal_handlers = 0;
    
    /* Use system Python - don't try to isolate or set custom paths
     * isolated=1 causes "failed to get the Python codec" error */
    
    /* Initialize Python with minimal configuration */
    status = Py_InitializeFromConfig(&config);
    PyConfig_Clear(&config);
    
    if (PyStatus_Exception(status)) {
        set_python_error("Failed to initialize Python interpreter");
        log_error("Python initialization failed: %s", status.err_msg);
        return 1;
    }
    
    if (!Py_IsInitialized()) {
        set_python_error("Python interpreter not initialized");
        log_error("Python interpreter not initialized");
        return 1;
    }

    /* Set up scripts path */
#ifdef _WIN32
    _snprintf_s(g_scripts_path, sizeof(g_scripts_path), _TRUNCATE, "%s\\scripts", plugin_path);
#else
    snprintf(g_scripts_path, sizeof(g_scripts_path), "%s/scripts", plugin_path);
#endif
    
    /* Add scripts directory to Python path */
    PyObject* sys_path = PySys_GetObject("path");
    if (sys_path != NULL) {
        PyObject* path_str = PyUnicode_FromString(g_scripts_path);
        if (path_str != NULL) {
            PyList_Append(sys_path, path_str);
            Py_DECREF(path_str);
            log_info("Added scripts path: %s", g_scripts_path);
        }
    }

    /* Get main module and dictionary */
    g_main_module = PyImport_AddModule("__main__");
    if (g_main_module == NULL) {
        set_python_error("Failed to get __main__ module");
        log_error("Failed to get __main__ module");
        Py_Finalize();
        return 1;
    }
    Py_INCREF(g_main_module);

    g_main_dict = PyModule_GetDict(g_main_module);
    if (g_main_dict == NULL) {
        set_python_error("Failed to get __main__ dictionary");
        log_error("Failed to get __main__ dictionary");
        Py_DECREF(g_main_module);
        Py_Finalize();
        return 1;
    }
    Py_INCREF(g_main_dict);

    g_python_initialized = 1;
    log_info("Python engine initialized successfully");
    log_info("Scripts path: %s", g_scripts_path);

    return 0;
}

void python_engine_shutdown(void)
{
    if (!g_python_initialized) {
        return;
    }

    log_info("Shutting down Python engine...");

    /* Cleanup Python API */
    python_api_shutdown();

    /* Release references */
    if (g_main_dict != NULL) {
        Py_DECREF(g_main_dict);
        g_main_dict = NULL;
    }

    if (g_main_module != NULL) {
        Py_DECREF(g_main_module);
        g_main_module = NULL;
    }

    /* Finalize Python */
    if (Py_IsInitialized()) {
        Py_Finalize();
    }

    g_python_initialized = 0;
    log_info("Python engine shut down");
}

int python_engine_is_initialized(void)
{
    return g_python_initialized;
}

const char* python_engine_get_scripts_path(void)
{
    return g_scripts_path;
}

int python_engine_load_script(const char* script_path)
{
    FILE* fp;
    PyObject* result;

    if (!g_python_initialized) {
        set_python_error("Python engine not initialized");
        return 1;
    }

    log_info("Loading Python script: %s", script_path);
    clear_python_error();

    /* Open and execute the script */
    fp = fopen(script_path, "r");
    if (fp == NULL) {
        set_python_error("Failed to open script file");
        log_error("Failed to open script: %s", script_path);
        return 1;
    }

    result = PyRun_File(fp, script_path, Py_file_input, g_main_dict, g_main_dict);
    fclose(fp);

    if (result == NULL) {
        /* Python error occurred - capture the error details */
        PyObject *ptype, *pvalue, *ptraceback;
        PyErr_Fetch(&ptype, &pvalue, &ptraceback);
        
        if (pvalue != NULL) {
            PyObject* str_obj = PyObject_Str(pvalue);
            if (str_obj != NULL) {
                const char* err_msg = PyUnicode_AsUTF8(str_obj);
                if (err_msg != NULL) {
                    set_python_error(err_msg);
                    log_error("Python error in %s: %s", script_path, err_msg);
                }
                Py_DECREF(str_obj);
            }
        }
        
        /* Also print to stderr for debugging */
        if (ptype != NULL && pvalue != NULL) {
            PyErr_Restore(ptype, pvalue, ptraceback);
            PyErr_Print();
        } else {
            set_python_error("Script execution failed (unknown error)");
            log_error("Failed to execute script: %s", script_path);
            Py_XDECREF(ptype);
            Py_XDECREF(pvalue);
            Py_XDECREF(ptraceback);
        }
        return 1;
    }

    Py_DECREF(result);
    log_info("Script loaded successfully: %s", script_path);

    return 0;
}

int python_engine_reload_scripts(void)
{
    char default_script[512];
    
    if (!g_python_initialized) {
        set_python_error("Python engine not initialized");
        return 1;
    }

    log_info("Reloading tspy_init.py...");
    
    /* Reload the default init script */
#ifdef _WIN32
    _snprintf_s(default_script, sizeof(default_script), _TRUNCATE, "%s\\tspy_init.py", g_scripts_path);
#else
    snprintf(default_script, sizeof(default_script), "%s/tspy_init.py", g_scripts_path);
#endif
    
    if (python_engine_load_script(default_script) == 0) {
        log_info("Successfully reloaded tspy_init.py");
        return 0;
    } else {
        log_warning("Failed to reload tspy_init.py");
        return 1;
    }
}

int python_engine_execute(const char* code)
{
    PyObject* result;

    if (!g_python_initialized) {
        set_python_error("Python engine not initialized");
        return 1;
    }

    if (code == NULL || *code == '\0') {
        set_python_error("Empty code string");
        return 1;
    }

    log_debug("Executing Python code: %s", code);
    clear_python_error();

    result = PyRun_String(code, Py_file_input, g_main_dict, g_main_dict);

    if (result == NULL) {
        PyErr_Print();
        set_python_error("Code execution failed (check console for details)");
        return 1;
    }

    Py_DECREF(result);
    return 0;
}

int python_engine_call_function(const char* function_name, const char* format, ...)
{
    PyObject* func;
    PyObject* args = NULL;
    PyObject* result;
    va_list va;
    int ret = 0;

    if (!g_python_initialized) {
        set_python_error("Python engine not initialized");
        return 1;
    }

    if (function_name == NULL || *function_name == '\0') {
        set_python_error("Invalid function name");
        return 1;
    }

    clear_python_error();

    /* Get the function object */
    func = PyDict_GetItemString(g_main_dict, function_name);
    if (func == NULL || !PyCallable_Check(func)) {
        /* Function not found or not callable - this is not always an error */
        log_debug("Python function not found or not callable: %s", function_name);
        return 0; /* Return success - script may not implement this function */
    }

    /* Build arguments tuple */
    if (format != NULL && *format != '\0') {
        va_start(va, format);
        args = Py_VaBuildValue(format, va);
        va_end(va);

        if (args == NULL) {
            set_python_error("Failed to build arguments");
            PyErr_Print();
            return 1;
        }
    }

    /* Call the function */
    result = PyObject_CallObject(func, args);
    
    if (args != NULL) {
        Py_DECREF(args);
    }

    if (result == NULL) {
        set_python_error("Function call failed");
        PyErr_Print();
        ret = 1;
    } else {
        Py_DECREF(result);
        ret = 0;
    }

    return ret;
}

/* Event notification functions */
int python_engine_on_connect(uint64 serverConnectionHandlerID)
{
    return python_engine_call_function("on_connect", "K", (unsigned long long)serverConnectionHandlerID);
}

int python_engine_on_disconnect(uint64 serverConnectionHandlerID)
{
    return python_engine_call_function("on_disconnect", "K", (unsigned long long)serverConnectionHandlerID);
}

int python_engine_on_client_move(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID)
{
    return python_engine_call_function("on_client_move", "KhKK", 
        (unsigned long long)serverConnectionHandlerID,
        (short)clientID,
        (unsigned long long)oldChannelID,
        (unsigned long long)newChannelID);
}

int python_engine_on_text_message(uint64 serverConnectionHandlerID, anyID targetMode, anyID toID, anyID fromID, 
                                   const char* fromName, const char* fromUniqueIdentifier, const char* message)
{
    return python_engine_call_function("on_text_message", "Khhhhsss",
        (unsigned long long)serverConnectionHandlerID,
        (short)targetMode,
        (short)toID,
        (short)fromID,
        fromName ? fromName : "",
        fromUniqueIdentifier ? fromUniqueIdentifier : "",
        message ? message : "");
}

int python_engine_on_command(uint64 serverConnectionHandlerID, const char* command, const char* args)
{
    return python_engine_call_function("on_command", "Kss",
        (unsigned long long)serverConnectionHandlerID,
        command ? command : "",
        args ? args : "");
}

const char* python_engine_get_error(void)
{
    return g_python_error[0] != '\0' ? g_python_error : NULL;
}

/* Internal helper functions */
static void set_python_error(const char* msg)
{
    if (msg != NULL) {
#ifdef _WIN32
        strncpy_s(g_python_error, sizeof(g_python_error), msg, _TRUNCATE);
#else
        strncpy(g_python_error, msg, sizeof(g_python_error) - 1);
        g_python_error[sizeof(g_python_error) - 1] = '\0';
#endif
    }
}

static void clear_python_error(void)
{
    g_python_error[0] = '\0';
}
