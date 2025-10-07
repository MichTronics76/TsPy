/**
 * @file plugin_interface.c
 * @brief TeamSpeak 3 plugin interface implementation
 * @author TeamSpeak Systems GmbH
 * @version 1.3.0
 * 
 * Copyright (c) TeamSpeak Systems GmbH
 */

#if defined(WIN32) || defined(__WIN32__) || defined(_WIN32)
#include <Windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "plugin_interface.h"
#include "plugin_main.h"
#include "plugin_config.h"
#include "commands/command_handler.h"
#include "ui/menu_handler.h"
#include "ui/hotkey_handler.h"
#include "python/python_engine.h"
#include "python/python_events.h"
#include "utils/logging.h"
#include "utils/string_utils.h"

/* ========================================================================
 * Required Plugin Functions
 * ======================================================================== */

const char* ts3plugin_name(void)
{
#ifdef _WIN32
    static char* result = NULL;
    if (!result) {
        const wchar_t* name = L"TsPy Plugin";
        if (wchar_to_utf8(name, &result) == -1) {
            result = PLUGIN_NAME;
        }
    }
    return result;
#else
    return PLUGIN_NAME;
#endif
}

const char* ts3plugin_version(void)
{
    return PLUGIN_VERSION;
}

int ts3plugin_apiVersion(void)
{
    return PLUGIN_API_VERSION;
}

const char* ts3plugin_author(void)
{
    return PLUGIN_AUTHOR;
}

const char* ts3plugin_description(void)
{
    return PLUGIN_DESCRIPTION;
}

void ts3plugin_setFunctionPointers(const struct TS3Functions funcs)
{
    set_ts3_functions(funcs);
}

int ts3plugin_init(void)
{
    char appPath[PATH_BUFSIZE];
    char resourcesPath[PATH_BUFSIZE];
    char configPath[PATH_BUFSIZE];
    char pluginPath[PATH_BUFSIZE];
    
    struct TS3Functions* ts3Functions = get_ts3_functions();

    log_info("Initializing plugin...");

    /* Query paths from client */
    ts3Functions->getAppPath(appPath, PATH_BUFSIZE);
    ts3Functions->getResourcesPath(resourcesPath, PATH_BUFSIZE);
    ts3Functions->getConfigPath(configPath, PATH_BUFSIZE);
    ts3Functions->getPluginPath(pluginPath, PATH_BUFSIZE, get_plugin_id());

    log_info("App path: %s", appPath);
    log_info("Resources path: %s", resourcesPath);
    log_info("Config path: %s", configPath);
    log_info("Plugin path: %s", pluginPath);

    /* Initialize plugin configuration */
    if (!init_plugin_config(configPath)) {
        log_error("Failed to initialize plugin configuration");
        return 1;
    }

    /* Initialize Python engine with safer configuration */
    log_info("Attempting to initialize Python engine (safe mode)...");
    
    if (python_engine_init(pluginPath) == 0) {
        log_info("Python engine initialized successfully");
        log_info("Scripts can be loaded from scripts/ directory");
        
        /* Initialize Python event dispatcher */
        if (python_events_init() == 0) {
            log_info("Python event dispatcher initialized");
        } else {
            log_warning("Failed to initialize Python event dispatcher");
        }
        
        /* Try to load default initialization script */
        char default_script[PATH_BUFSIZE];
#ifdef _WIN32
        _snprintf_s(default_script, sizeof(default_script), _TRUNCATE, "%s\\scripts\\tspy_init.py", pluginPath);
#else
        snprintf(default_script, sizeof(default_script), "%s/scripts/tspy_init.py", pluginPath);
#endif
        
        log_info("Looking for auto-load script: %s", default_script);
        
        /* Load the default script if it exists (non-fatal if missing) */
        if (python_engine_load_script(default_script) == 0) {
            log_info("Auto-loaded: tspy_init.py");
        } else {
            log_info("tspy_init.py not found or failed to load (this is OK)");
            log_info("Use /tspy python load <script> to load scripts manually");
        }
    } else {
        log_warning("Python engine initialization failed: %s", 
                    python_engine_get_error() ? python_engine_get_error() : "Unknown error");
        log_warning("Plugin will continue without Python support");
        log_warning("Core features (commands, events) will still work");
    }

    log_info("Plugin initialized successfully");
    return 0;
}

void ts3plugin_shutdown(void)
{
    log_info("Shutting down plugin...");
    
    /* Shutdown Python engine if initialized */
    if (python_engine_is_initialized()) {
        log_info("Shutting down Python engine...");
        python_events_shutdown();
        python_engine_shutdown();
    }
    
    cleanup_plugin_config();
    
    /* Cleanup plugin resources */
    if (get_plugin_id() != NULL) {
        /* Plugin ID will be freed by plugin_main cleanup */
    }
    
    log_info("Plugin shutdown complete");
}

/* ========================================================================
 * Optional Plugin Functions
 * ======================================================================== */

int ts3plugin_offersConfigure(void)
{
    return PLUGIN_OFFERS_NO_CONFIGURE;
}

void ts3plugin_configure(void* handle, void* qParentWidget)
{
    (void)handle;
    (void)qParentWidget;
    log_info("Configure called (not implemented)");
}

void ts3plugin_registerPluginID(const char* id)
{
    set_plugin_id(id);
}

const char* ts3plugin_commandKeyword(void)
{
    return "tspy";
}

int ts3plugin_processCommand(uint64 serverConnectionHandlerID, const char* command)
{
    return process_command(serverConnectionHandlerID, command);
}

void ts3plugin_currentServerConnectionChanged(uint64 serverConnectionHandlerID)
{
    log_info("Server connection changed: %llu", (unsigned long long)serverConnectionHandlerID);
}

const char* ts3plugin_infoTitle(void)
{
    return "TsPy Plugin Info";
}

void ts3plugin_infoData(uint64 serverConnectionHandlerID, uint64 id, enum PluginItemType type, char** data)
{
    (void)serverConnectionHandlerID;
    (void)id;
    (void)type;
    *data = NULL;
}

void ts3plugin_freeMemory(void* data)
{
    if (data != NULL) {
        free(data);
    }
}

int ts3plugin_requestAutoload(void)
{
    return 0; /* Do not autoload by default */
}

void ts3plugin_initMenus(struct PluginMenuItem*** menuItems, char** menuIcon)
{
    init_menu_items(menuItems, menuIcon);
}

void ts3plugin_initHotkeys(struct PluginHotkey*** hotkeys)
{
    init_hotkeys(hotkeys);
}

/* ========================================================================
 * TeamSpeak Event Callbacks
 * ======================================================================== */

void ts3plugin_onServerErrorEvent(uint64 serverConnectionHandlerID, const char* errorMessage, 
                                   unsigned int error, const char* returnCode, const char* extraMessage)
{
    (void)returnCode;
    (void)extraMessage;
    
    log_info("EVENT CALLBACK: Server error: server=%llu, error=%u, msg=%s", 
             (unsigned long long)serverConnectionHandlerID, error, 
             errorMessage ? errorMessage : "none");
}

void ts3plugin_onConnectStatusChangeEvent(uint64 serverConnectionHandlerID, int newStatus, unsigned int errorNumber)
{
    log_info("EVENT CALLBACK: Connect status changed: server=%llu, status=%d, error=%u", 
              (unsigned long long)serverConnectionHandlerID, newStatus, errorNumber);
    
    /* Dispatch to Python event handlers */
    python_event_on_connect_status_changed(serverConnectionHandlerID, newStatus, errorNumber);
}

void ts3plugin_onClientMoveEvent(uint64 serverConnectionHandlerID, anyID clientID, 
                                  uint64 oldChannelID, uint64 newChannelID, 
                                  int visibility, const char* moveMessage)
{
    log_info("EVENT CALLBACK: Client move: server=%llu, client=%d, old=%llu, new=%llu", 
              (unsigned long long)serverConnectionHandlerID, clientID, 
              (unsigned long long)oldChannelID, (unsigned long long)newChannelID);
    
    /* Dispatch to Python event handlers */
    python_event_on_client_move(serverConnectionHandlerID, clientID, oldChannelID, 
                                newChannelID, visibility, moveMessage);
}

void ts3plugin_onTextMessageEvent(uint64 serverConnectionHandlerID, anyID targetMode,
                                   anyID toID, anyID fromID, const char* fromName,
                                   const char* fromUniqueIdentifier, const char* message, 
                                   int ffIgnored)
{
    (void)ffIgnored; /* Unused */
    
    log_info("EVENT CALLBACK: Text message: server=%llu, from=%s, message=%s", 
              (unsigned long long)serverConnectionHandlerID, 
              fromName ? fromName : "unknown", 
              message ? message : "");
    
    /* Dispatch to Python event handlers */
    python_event_on_text_message(serverConnectionHandlerID, targetMode, toID, fromID, 
                                 fromName, fromUniqueIdentifier, message);
}

void ts3plugin_onTalkStatusChangeEvent(uint64 serverConnectionHandlerID, int status, 
                                        int isReceivedWhisper, anyID clientID)
{
    (void)isReceivedWhisper;
    
    log_info("EVENT CALLBACK: Talk status: server=%llu, client=%d, status=%d", 
             (unsigned long long)serverConnectionHandlerID, clientID, status);
    
    /* Dispatch to Python event handlers */
    python_event_on_talk_status_change(serverConnectionHandlerID, status, isReceivedWhisper, clientID);
}

void ts3plugin_onNewChannelEvent(uint64 serverConnectionHandlerID, uint64 channelID, uint64 channelParentID)
{
    log_info("EVENT CALLBACK: New channel: server=%llu, channel=%llu, parent=%llu",
             (unsigned long long)serverConnectionHandlerID,
             (unsigned long long)channelID,
             (unsigned long long)channelParentID);
}
