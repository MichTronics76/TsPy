/**
 * @file python_engine.h
 * @brief Python scripting engine interface
 * @author TsPy Team
 * @version 1.3.0
 * 
 * Provides Python embedding capabilities for TeamSpeak plugin scripting
 */

#ifndef PYTHON_ENGINE_H
#define PYTHON_ENGINE_H

#include <stdint.h>
#include "teamspeak/public_definitions.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the Python engine
 * @param plugin_path Path to the plugin directory
 * @return 0 on success, non-zero on failure
 */
int python_engine_init(const char* plugin_path);

/**
 * @brief Shutdown the Python engine
 */
void python_engine_shutdown(void);

/**
 * @brief Check if Python engine is initialized
 * @return 1 if initialized, 0 otherwise
 */
int python_engine_is_initialized(void);

/**
 * @brief Get the scripts directory path
 * @return Path to scripts directory
 */
const char* python_engine_get_scripts_path(void);

/**
 * @brief Load and execute a Python script
 * @param script_path Path to the Python script
 * @return 0 on success, non-zero on failure
 */
int python_engine_load_script(const char* script_path);

/**
 * @brief Reload all Python scripts
 * @return 0 on success, non-zero on failure
 */
int python_engine_reload_scripts(void);

/**
 * @brief Execute Python code string
 * @param code Python code to execute
 * @return 0 on success, non-zero on failure
 */
int python_engine_execute(const char* code);

/**
 * @brief Call a Python function by name
 * @param function_name Name of the function to call
 * @param format Format string for arguments (like PyArg_ParseTuple)
 * @param ... Variable arguments based on format
 * @return 0 on success, non-zero on failure
 */
int python_engine_call_function(const char* function_name, const char* format, ...);

/* Event notifications to Python scripts */
int python_engine_on_connect(uint64 serverConnectionHandlerID);
int python_engine_on_disconnect(uint64 serverConnectionHandlerID);
int python_engine_on_client_move(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID);
int python_engine_on_text_message(uint64 serverConnectionHandlerID, anyID targetMode, anyID toID, anyID fromID, const char* fromName, const char* fromUniqueIdentifier, const char* message);
int python_engine_on_command(uint64 serverConnectionHandlerID, const char* command, const char* args);

/**
 * @brief Get last Python error message
 * @return Error message string, or NULL if no error
 */
const char* python_engine_get_error(void);

#ifdef __cplusplus
}
#endif

#endif /* PYTHON_ENGINE_H */
