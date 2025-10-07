/**
 * @file python_api.c
 * @brief Python API module implementation
 * @author TsPy Team
 * @version 1.3.0
 * 
 * Provides TeamSpeak API functions to Python scripts
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

#include "python_api.h"
#include "core/plugin_main.h"
#include "utils/logging.h"

/* Python API functions */

static PyObject* py_ts_print_message(PyObject* self, PyObject* args)
{
    uint64 serverConnectionHandlerID;
    const char* message;
    int targetMode = 0; /* 0 = PluginMessageTarget_CURRENT_CHANNEL */
    struct TS3Functions* ts3Functions = get_ts3_functions();

    (void)self; /* Unused parameter */

    if (!PyArg_ParseTuple(args, "Ks|i", &serverConnectionHandlerID, &message, &targetMode)) {
        return NULL;
    }

    if (ts3Functions != NULL && ts3Functions->printMessageToCurrentTab != NULL) {
        ts3Functions->printMessageToCurrentTab(message);
    }

    Py_RETURN_NONE;
}

static PyObject* py_ts_get_client_id(PyObject* self, PyObject* args)
{
    uint64 serverConnectionHandlerID;
    anyID clientID;
    struct TS3Functions* ts3Functions = get_ts3_functions();

    (void)self; /* Unused parameter */

    if (!PyArg_ParseTuple(args, "K", &serverConnectionHandlerID)) {
        return NULL;
    }

    if (ts3Functions != NULL && ts3Functions->getClientID != NULL) {
        if (ts3Functions->getClientID(serverConnectionHandlerID, &clientID) == ERROR_ok) {
            return PyLong_FromLong(clientID);
        }
    }

    Py_RETURN_NONE;
}

static PyObject* py_ts_get_client_name(PyObject* self, PyObject* args)
{
    uint64 serverConnectionHandlerID;
    anyID clientID;
    char* name = NULL;
    PyObject* result = NULL;
    struct TS3Functions* ts3Functions = get_ts3_functions();

    (void)self; /* Unused parameter */

    if (!PyArg_ParseTuple(args, "Kh", &serverConnectionHandlerID, &clientID)) {
        return NULL;
    }

    if (ts3Functions != NULL && ts3Functions->getClientVariableAsString != NULL) {
        if (ts3Functions->getClientVariableAsString(serverConnectionHandlerID, clientID, CLIENT_NICKNAME, &name) == ERROR_ok) {
            result = PyUnicode_FromString(name ? name : "");
            if (name != NULL) {
                ts3Functions->freeMemory(name);
            }
            return result;
        }
    }

    Py_RETURN_NONE;
}

static PyObject* py_ts_send_channel_message(PyObject* self, PyObject* args)
{
    uint64 serverConnectionHandlerID;
    const char* message;
    uint64 targetChannelID = 0; /* 0 = current channel */
    const char* returnCode = NULL;
    unsigned int result;
    struct TS3Functions* ts3Functions = get_ts3_functions();

    (void)self; /* Unused parameter */

    if (!PyArg_ParseTuple(args, "Ks", &serverConnectionHandlerID, &message)) {
        return NULL;
    }

    if (ts3Functions != NULL && ts3Functions->requestSendChannelTextMsg != NULL) {
        result = ts3Functions->requestSendChannelTextMsg(serverConnectionHandlerID, message, targetChannelID, returnCode);
        if (result != ERROR_ok) {
            log_warning("Failed to send channel message: error=%u", result);
        } else {
            log_debug("Channel message sent successfully");
        }
    } else {
        log_error("requestSendChannelTextMsg function not available");
    }

    Py_RETURN_NONE;
}

static PyObject* py_ts_send_server_message(PyObject* self, PyObject* args)
{
    uint64 serverConnectionHandlerID;
    const char* message;
    const char* returnCode = NULL;
    struct TS3Functions* ts3Functions = get_ts3_functions();

    (void)self; /* Unused parameter */

    if (!PyArg_ParseTuple(args, "Ks", &serverConnectionHandlerID, &message)) {
        return NULL;
    }

    if (ts3Functions != NULL && ts3Functions->requestSendServerTextMsg != NULL) {
        ts3Functions->requestSendServerTextMsg(serverConnectionHandlerID, message, returnCode);
    }

    Py_RETURN_NONE;
}

static PyObject* py_ts_log(PyObject* self, PyObject* args)
{
    const char* message;
    int level = 0; /* INFO */

    (void)self; /* Unused parameter */

    if (!PyArg_ParseTuple(args, "s|i", &message, &level)) {
        return NULL;
    }

    switch (level) {
        case 0: log_info("[PY] %s", message); break;
        case 1: log_warning("[PY] %s", message); break;
        case 2: log_error("[PY] %s", message); break;
        default: log_debug("[PY] %s", message); break;
    }

    Py_RETURN_NONE;
}

/* Audio functions */
static PyObject* py_ts_get_audio_level(PyObject* self, PyObject* args)
{
    uint64 serverConnectionHandlerID;
    const char* ident = "decibel_last_period";
    float result = 0.0f;
    struct TS3Functions* ts3Functions = get_ts3_functions();

    (void)self; /* Unused parameter */

    if (!PyArg_ParseTuple(args, "K|s", &serverConnectionHandlerID, &ident)) {
        return NULL;
    }

    if (ts3Functions != NULL && ts3Functions->getPreProcessorInfoValueFloat != NULL) {
        if (ts3Functions->getPreProcessorInfoValueFloat(serverConnectionHandlerID, ident, &result) == ERROR_ok) {
            return PyFloat_FromDouble((double)result);
        }
    }

    Py_RETURN_NONE;
}

static PyObject* py_ts_start_recording(PyObject* self, PyObject* args)
{
    uint64 serverConnectionHandlerID;
    unsigned int result;
    struct TS3Functions* ts3Functions = get_ts3_functions();

    (void)self; /* Unused parameter */

    if (!PyArg_ParseTuple(args, "K", &serverConnectionHandlerID)) {
        return NULL;
    }

    if (ts3Functions != NULL && ts3Functions->startVoiceRecording != NULL) {
        result = ts3Functions->startVoiceRecording(serverConnectionHandlerID);
        if (result == ERROR_ok) {
            log_info("Voice recording started");
            Py_RETURN_TRUE;
        } else {
            log_warning("Failed to start voice recording: error=%u", result);
            Py_RETURN_FALSE;
        }
    }

    Py_RETURN_FALSE;
}

static PyObject* py_ts_stop_recording(PyObject* self, PyObject* args)
{
    uint64 serverConnectionHandlerID;
    unsigned int result;
    struct TS3Functions* ts3Functions = get_ts3_functions();

    (void)self; /* Unused parameter */

    if (!PyArg_ParseTuple(args, "K", &serverConnectionHandlerID)) {
        return NULL;
    }

    if (ts3Functions != NULL && ts3Functions->stopVoiceRecording != NULL) {
        result = ts3Functions->stopVoiceRecording(serverConnectionHandlerID);
        if (result == ERROR_ok) {
            log_info("Voice recording stopped");
            Py_RETURN_TRUE;
        } else {
            log_warning("Failed to stop voice recording: error=%u", result);
            Py_RETURN_FALSE;
        }
    }

    Py_RETURN_FALSE;
}

/* Method definitions */
static PyMethodDef TsApiMethods[] = {
    {"print_message", py_ts_print_message, METH_VARARGS, 
     "Print a message to TeamSpeak (serverConnectionHandlerID, message, [targetMode])"},
    
    {"get_client_id", py_ts_get_client_id, METH_VARARGS,
     "Get own client ID (serverConnectionHandlerID)"},
    
    {"get_client_name", py_ts_get_client_name, METH_VARARGS,
     "Get client name by ID (serverConnectionHandlerID, clientID)"},
    
    {"send_channel_message", py_ts_send_channel_message, METH_VARARGS,
     "Send message to current channel (serverConnectionHandlerID, message)"},
    
    {"send_server_message", py_ts_send_server_message, METH_VARARGS,
     "Send message to server (serverConnectionHandlerID, message)"},
    
    {"log", py_ts_log, METH_VARARGS,
     "Write to plugin log (message, [level: 0=info, 1=warn, 2=error])"},
    
    {"get_audio_level", py_ts_get_audio_level, METH_VARARGS,
     "Get audio input level (serverConnectionHandlerID, [ident='decibel_last_period'])"},
    
    {"start_recording", py_ts_start_recording, METH_VARARGS,
     "Start voice recording (serverConnectionHandlerID)"},
    
    {"stop_recording", py_ts_stop_recording, METH_VARARGS,
     "Stop voice recording (serverConnectionHandlerID)"},
    
    {NULL, NULL, 0, NULL}
};

/* Module definition */
static struct PyModuleDef TsApiModule = {
    PyModuleDef_HEAD_INIT,
    "ts3api",
    "TeamSpeak 3 API for Python scripts",
    -1,
    TsApiMethods
};

/* Module initialization */
static PyObject* PyInit_ts3api(void)
{
    return PyModule_Create(&TsApiModule);
}

int python_api_init(void)
{
    /* Add the module to Python */
    if (PyImport_AppendInittab("ts3api", &PyInit_ts3api) == -1) {
        log_error("Failed to add ts3api module to Python");
        return 1;
    }

    log_info("Python API module initialized");
    return 0;
}

void python_api_shutdown(void)
{
    /* Nothing to clean up currently */
    log_debug("Python API module shutdown");
}
