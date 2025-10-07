/**
 * @file command_handler.c
 * @brief Command handler implementation
 * @author TeamSpeak Systems GmbH
 * @version 1.2.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command_handler.h"
#include "core/plugin_main.h"
#include "python/python_engine.h"
#include "utils/logging.h"
#include "utils/string_utils.h"

typedef enum {
    CMD_NONE = 0,
    CMD_HELP,
    CMD_STATUS,
    CMD_INFO,
    CMD_PYTHON
} CommandType;

static CommandType parse_command(const char* command, char** param1, char** param2)
{
    char buf[COMMAND_BUFSIZE];
    char* token;
    char* context = NULL;
    CommandType cmd = CMD_NONE;
    int tokenIndex = 0;

    safe_strcpy(buf, COMMAND_BUFSIZE, command);

#ifdef _WIN32
    token = strtok_s(buf, " ", &context);
#else
    token = strtok(buf, " ");
#endif

    while (token != NULL) {
        if (tokenIndex == 0) {
            if (strcmp(token, "help") == 0) {
                cmd = CMD_HELP;
            } else if (strcmp(token, "status") == 0) {
                cmd = CMD_STATUS;
            } else if (strcmp(token, "info") == 0) {
                cmd = CMD_INFO;
            } else if (strcmp(token, "python") == 0 || strcmp(token, "py") == 0) {
                cmd = CMD_PYTHON;
            }
        } else if (tokenIndex == 1 && param1 != NULL) {
            *param1 = token;
        } else if (tokenIndex == 2 && param2 != NULL) {
            *param2 = token;
        }

#ifdef _WIN32
        token = strtok_s(NULL, " ", &context);
#else
        token = strtok(NULL, " ");
#endif
        tokenIndex++;
    }

    return cmd;
}

static int handle_help_command(uint64 serverConnectionHandlerID)
{
    struct TS3Functions* ts3Functions = get_ts3_functions();
    
    (void)serverConnectionHandlerID; /* May be used in future */
    
    log_info("Available commands:");
    log_info("  /tspy help           - Show this help message");
    log_info("  /tspy status         - Show plugin status");
    log_info("  /tspy info           - Show plugin information");
    log_info("  /tspy python status  - Show Python engine status");
    log_info("  /tspy python load <script> - Load a Python script");
    log_info("  /tspy python reload  - Reload all Python scripts");

    if (ts3Functions != NULL && ts3Functions->printMessageToCurrentTab != NULL) {
        ts3Functions->printMessageToCurrentTab("TsPy Plugin Commands:");
        ts3Functions->printMessageToCurrentTab("  /tspy help           - Show this help message");
        ts3Functions->printMessageToCurrentTab("  /tspy status         - Show plugin status");
        ts3Functions->printMessageToCurrentTab("  /tspy info           - Show plugin information");
        ts3Functions->printMessageToCurrentTab("  /tspy python status  - Show Python engine status");
        ts3Functions->printMessageToCurrentTab("  /tspy python load <script> - Load a Python script");
        ts3Functions->printMessageToCurrentTab("  /tspy python reload  - Reload all Python scripts");
    }

    return 0;
}

static int handle_status_command(uint64 serverConnectionHandlerID)
{
    struct TS3Functions* ts3Functions = get_ts3_functions();
    char message[256];
    
    (void)serverConnectionHandlerID; /* May be used in future */

    snprintf(message, sizeof(message), "TsPy Plugin Status: Active");
    log_info("%s", message);

    if (ts3Functions != NULL && ts3Functions->printMessageToCurrentTab != NULL) {
        ts3Functions->printMessageToCurrentTab(message);
        
        /* Show Python status */
        if (python_engine_is_initialized()) {
            ts3Functions->printMessageToCurrentTab("Python Engine: Initialized");
        } else {
            ts3Functions->printMessageToCurrentTab("Python Engine: Not initialized");
        }
    }

    return 0;
}

static int handle_info_command(uint64 serverConnectionHandlerID)
{
    struct TS3Functions* ts3Functions = get_ts3_functions();
    char message[256];
    
    (void)serverConnectionHandlerID; /* May be used in future */

    snprintf(message, sizeof(message), "TsPy Plugin v%s by %s", PLUGIN_VERSION, PLUGIN_AUTHOR);
    log_info("%s", message);

    if (ts3Functions != NULL && ts3Functions->printMessageToCurrentTab != NULL) {
        ts3Functions->printMessageToCurrentTab(message);
    }

    return 0;
}

static int handle_python_command(uint64 serverConnectionHandlerID, const char* subcommand, const char* param)
{
    struct TS3Functions* ts3Functions = get_ts3_functions();
    char message[512];
    char script_path[512];
    
    if (!python_engine_is_initialized()) {
        snprintf(message, sizeof(message), "Python engine is not initialized");
        log_error("%s", message);
        
        if (ts3Functions != NULL && ts3Functions->printMessageToCurrentTab != NULL) {
            ts3Functions->printMessageToCurrentTab(message);
            ts3Functions->printMessageToCurrentTab("Python support may be disabled - check plugin logs");
        }
        return 1;
    }
    
    if (subcommand == NULL) {
        snprintf(message, sizeof(message), "Usage: /tspy python <status|load|reload>");
        log_warning("%s", message);
        
        if (ts3Functions != NULL && ts3Functions->printMessageToCurrentTab != NULL) {
            ts3Functions->printMessageToCurrentTab(message);
        }
        return 1;
    }
    
    /* Handle python status */
    if (strcmp(subcommand, "status") == 0) {
        snprintf(message, sizeof(message), "Python Engine: Running");
        log_info("%s", message);
        
        if (ts3Functions != NULL && ts3Functions->printMessageToCurrentTab != NULL) {
            ts3Functions->printMessageToCurrentTab(message);
            
            const char* error = python_engine_get_error();
            if (error && strlen(error) > 0) {
                snprintf(message, sizeof(message), "Last error: %s", error);
                ts3Functions->printMessageToCurrentTab(message);
            } else {
                ts3Functions->printMessageToCurrentTab("No errors");
            }
        }
        return 0;
    }
    
    /* Handle python load <script> */
    if (strcmp(subcommand, "load") == 0) {
        if (param == NULL || strlen(param) == 0) {
            snprintf(message, sizeof(message), "Usage: /tspy python load <script_name>");
            log_warning("%s", message);
            
            if (ts3Functions != NULL && ts3Functions->printMessageToCurrentTab != NULL) {
                ts3Functions->printMessageToCurrentTab(message);
                ts3Functions->printMessageToCurrentTab("Example: /tspy python load auto_greeter.py");
            }
            return 1;
        }
        
        /* Build full script path - assume it's in the scripts directory */
        const char* scripts_path = python_engine_get_scripts_path();
        if (scripts_path == NULL || strlen(scripts_path) == 0) {
            snprintf(message, sizeof(message), "Scripts directory not available");
            log_error("%s", message);
            
            if (ts3Functions != NULL && ts3Functions->printMessageToCurrentTab != NULL) {
                ts3Functions->printMessageToCurrentTab(message);
            }
            return 1;
        }
        
        /* Add .py extension if not present */
        char script_name[256];
        if (strstr(param, ".py") == NULL) {
            snprintf(script_name, sizeof(script_name), "%s.py", param);
        } else {
            safe_strcpy(script_name, sizeof(script_name), param);
        }
        
        /* Build full path: scripts_path/script_name */
#ifdef _WIN32
        snprintf(script_path, sizeof(script_path), "%s\\%s", scripts_path, script_name);
#else
        snprintf(script_path, sizeof(script_path), "%s/%s", scripts_path, script_name);
#endif
        
        snprintf(message, sizeof(message), "Loading Python script: %s", script_name);
        log_info("%s", message);
        
        if (ts3Functions != NULL && ts3Functions->printMessageToCurrentTab != NULL) {
            ts3Functions->printMessageToCurrentTab(message);
        }
        
        if (python_engine_load_script(script_path) == 0) {
            snprintf(message, sizeof(message), "Successfully loaded: %s", script_name);
            log_info("%s", message);
            
            if (ts3Functions != NULL && ts3Functions->printMessageToCurrentTab != NULL) {
                ts3Functions->printMessageToCurrentTab(message);
            }
            return 0;
        } else {
            const char* error = python_engine_get_error();
            snprintf(message, sizeof(message), "Failed to load %s: %s", 
                     script_name, error ? error : "Unknown error");
            log_error("%s", message);
            
            if (ts3Functions != NULL && ts3Functions->printMessageToCurrentTab != NULL) {
                ts3Functions->printMessageToCurrentTab(message);
            }
            return 1;
        }
    }
    
    /* Handle python reload */
    if (strcmp(subcommand, "reload") == 0) {
        snprintf(message, sizeof(message), "Reloading all Python scripts...");
        log_info("%s", message);
        
        if (ts3Functions != NULL && ts3Functions->printMessageToCurrentTab != NULL) {
            ts3Functions->printMessageToCurrentTab(message);
        }
        
        if (python_engine_reload_scripts() == 0) {
            snprintf(message, sizeof(message), "Successfully reloaded all scripts");
            log_info("%s", message);
            
            if (ts3Functions != NULL && ts3Functions->printMessageToCurrentTab != NULL) {
                ts3Functions->printMessageToCurrentTab(message);
            }
            return 0;
        } else {
            const char* error = python_engine_get_error();
            snprintf(message, sizeof(message), "Failed to reload scripts: %s", 
                     error ? error : "Unknown error");
            log_error("%s", message);
            
            if (ts3Functions != NULL && ts3Functions->printMessageToCurrentTab != NULL) {
                ts3Functions->printMessageToCurrentTab(message);
            }
            return 1;
        }
    }
    
    /* Unknown subcommand */
    snprintf(message, sizeof(message), "Unknown python command: %s", subcommand);
    log_warning("%s", message);
    
    if (ts3Functions != NULL && ts3Functions->printMessageToCurrentTab != NULL) {
        ts3Functions->printMessageToCurrentTab(message);
        ts3Functions->printMessageToCurrentTab("Available: status, load, reload");
    }
    
    return 1;
}

int process_command(uint64 serverConnectionHandlerID, const char* command)
{
    char* param1 = NULL;
    char* param2 = NULL;
    CommandType cmd;

    if (command == NULL) {
        return 1;
    }

    log_debug("Processing command: %s", command);

    cmd = parse_command(command, &param1, &param2);

    switch (cmd) {
        case CMD_HELP:
            return handle_help_command(serverConnectionHandlerID);
        case CMD_STATUS:
            return handle_status_command(serverConnectionHandlerID);
        case CMD_INFO:
            return handle_info_command(serverConnectionHandlerID);
        case CMD_PYTHON:
            return handle_python_command(serverConnectionHandlerID, param1, param2);
        case CMD_NONE:
        default:
            log_warning("Unknown command: %s", command);
            return 1;
    }
}
