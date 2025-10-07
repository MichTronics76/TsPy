/**
 * @file python_events.c
 * @brief Python event dispatcher implementation
 * @author TsPy Team
 * @version 1.4.0
 * 
 * Dispatches TeamSpeak events to Python event handlers
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

#include "python_events.h"
#include "python_engine.h"
#include "utils/logging.h"
#include <string.h>

/* Helper function to call a Python function if it exists */
static int call_python_handler(const char* func_name, PyObject* args)
{
    PyObject* main_dict;
    PyObject* func;
    PyObject* result;
    
    if (!python_engine_is_initialized()) {
        if (args) Py_DECREF(args);
        return 1;
    }
    
    /* Get __main__ module dictionary */
    main_dict = PyModule_GetDict(PyImport_AddModule("__main__"));
    if (main_dict == NULL) {
        log_error("Failed to get __main__ dictionary");
        if (args) Py_DECREF(args);
        return 1;
    }
    
    /* Look up the function */
    func = PyDict_GetItemString(main_dict, func_name);
    if (func == NULL || !PyCallable_Check(func)) {
        /* Function not defined - this is OK, not all scripts implement all handlers */
        if (args) Py_DECREF(args);
        return 0;
    }
    
    /* Call the function */
    result = PyObject_CallObject(func, args);
    if (args) Py_DECREF(args);
    
    if (result == NULL) {
        /* Python exception occurred */
        PyObject *ptype, *pvalue, *ptraceback;
        PyErr_Fetch(&ptype, &pvalue, &ptraceback);
        
        if (pvalue != NULL) {
            PyObject* str_obj = PyObject_Str(pvalue);
            if (str_obj != NULL) {
                const char* err_msg = PyUnicode_AsUTF8(str_obj);
                if (err_msg != NULL) {
                    log_error("Python error in %s: %s", func_name, err_msg);
                }
                Py_DECREF(str_obj);
            }
        }
        
        /* Clean up */
        Py_XDECREF(ptype);
        Py_XDECREF(pvalue);
        Py_XDECREF(ptraceback);
        
        return 1;
    }
    
    Py_DECREF(result);
    return 0;
}

int python_events_init(void)
{
    log_info("Python event dispatcher initialized");
    return 0;
}

void python_events_shutdown(void)
{
    log_debug("Python event dispatcher shutdown");
}

void python_event_on_connect_status_changed(uint64 serverConnectionHandlerID, int newStatus, unsigned int errorNumber)
{
    PyObject* args;
    
    /* STATUS_DISCONNECTED = 0, STATUS_CONNECTING = 1, STATUS_CONNECTED = 2, STATUS_CONNECTION_ESTABLISHING = 3 */
    
    /* Call on_connect for STATUS_CONNECTED (2) */
    if (newStatus == 2 && errorNumber == 0) {
        args = Py_BuildValue("(K)", serverConnectionHandlerID);
        if (args != NULL) {
            call_python_handler("on_connect", args);
        }
    }
    /* Call on_disconnect for STATUS_DISCONNECTED (0) */
    else if (newStatus == 0) {
        args = Py_BuildValue("(K)", serverConnectionHandlerID);
        if (args != NULL) {
            call_python_handler("on_disconnect", args);
        }
    }
}

void python_event_on_client_move(uint64 serverConnectionHandlerID, anyID clientID, 
                                  uint64 oldChannelID, uint64 newChannelID, 
                                  int visibility, const char* moveMessage)
{
    PyObject* args;
    
    (void)visibility;    /* Unused for now */
    (void)moveMessage;   /* Unused for now */
    
    /* Build arguments: (server_id, client_id, old_channel, new_channel) */
    args = Py_BuildValue("(KhKK)", serverConnectionHandlerID, clientID, oldChannelID, newChannelID);
    if (args != NULL) {
        call_python_handler("on_client_move", args);
    }
}

void python_event_on_text_message(uint64 serverConnectionHandlerID, anyID targetMode,
                                   anyID toID, anyID fromID, const char* fromName,
                                   const char* fromUniqueIdentifier, const char* message)
{
    PyObject* args;
    
    /* Build arguments: (server_id, target_mode, to_id, from_id, from_name, from_uid, message) */
    args = Py_BuildValue("(Khhhsss)", 
                         serverConnectionHandlerID, 
                         targetMode, 
                         toID, 
                         fromID, 
                         fromName ? fromName : "", 
                         fromUniqueIdentifier ? fromUniqueIdentifier : "",
                         message ? message : "");
    
    if (args != NULL) {
        call_python_handler("on_text_message", args);
    }
}

void python_event_on_talk_status_change(uint64 serverConnectionHandlerID, int status,
                                         int isReceivedWhisper, anyID clientID)
{
    PyObject* args;
    
    (void)isReceivedWhisper; /* Unused for now */
    
    /* Build arguments: (server_id, status, client_id) */
    args = Py_BuildValue("(Kih)", serverConnectionHandlerID, status, clientID);
    
    if (args != NULL) {
        call_python_handler("on_talk_status_change", args);
    }
}
