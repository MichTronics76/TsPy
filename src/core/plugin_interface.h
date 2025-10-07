/**
 * @file plugin_interface.h
 * @brief TeamSpeak 3 plugin interface declarations
 * @author TeamSpeak Systems GmbH
 * @version 1.2.0
 * 
 * Copyright (c) TeamSpeak Systems GmbH
 */

#ifndef PLUGIN_INTERFACE_H
#define PLUGIN_INTERFACE_H

#include "teamspeak/public_definitions.h"
#include "ts3_functions.h"
#include "plugin_definitions.h"

#if defined(WIN32) || defined(__WIN32__) || defined(_WIN32)
#define PLUGINS_EXPORTDLL __declspec(dllexport)
#else
#define PLUGINS_EXPORTDLL __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Required functions */
PLUGINS_EXPORTDLL const char* ts3plugin_name(void);
PLUGINS_EXPORTDLL const char* ts3plugin_version(void);
PLUGINS_EXPORTDLL int         ts3plugin_apiVersion(void);
PLUGINS_EXPORTDLL const char* ts3plugin_author(void);
PLUGINS_EXPORTDLL const char* ts3plugin_description(void);
PLUGINS_EXPORTDLL void        ts3plugin_setFunctionPointers(const struct TS3Functions funcs);
PLUGINS_EXPORTDLL int         ts3plugin_init(void);
PLUGINS_EXPORTDLL void        ts3plugin_shutdown(void);

/* Optional functions */
PLUGINS_EXPORTDLL int         ts3plugin_offersConfigure(void);
PLUGINS_EXPORTDLL void        ts3plugin_configure(void* handle, void* qParentWidget);
PLUGINS_EXPORTDLL void        ts3plugin_registerPluginID(const char* id);
PLUGINS_EXPORTDLL const char* ts3plugin_commandKeyword(void);
PLUGINS_EXPORTDLL int         ts3plugin_processCommand(uint64 serverConnectionHandlerID, const char* command);
PLUGINS_EXPORTDLL void        ts3plugin_currentServerConnectionChanged(uint64 serverConnectionHandlerID);
PLUGINS_EXPORTDLL const char* ts3plugin_infoTitle(void);
PLUGINS_EXPORTDLL void        ts3plugin_infoData(uint64 serverConnectionHandlerID, uint64 id, enum PluginItemType type, char** data);
PLUGINS_EXPORTDLL void        ts3plugin_freeMemory(void* data);
PLUGINS_EXPORTDLL int         ts3plugin_requestAutoload(void);
PLUGINS_EXPORTDLL void        ts3plugin_initMenus(struct PluginMenuItem*** menuItems, char** menuIcon);
PLUGINS_EXPORTDLL void        ts3plugin_initHotkeys(struct PluginHotkey*** hotkeys);

/* Event callbacks */
PLUGINS_EXPORTDLL void        ts3plugin_onConnectStatusChangeEvent(uint64 serverConnectionHandlerID, int newStatus, unsigned int errorNumber);
PLUGINS_EXPORTDLL void        ts3plugin_onServerErrorEvent(uint64 serverConnectionHandlerID, const char* errorMessage, unsigned int error, const char* returnCode, const char* extraMessage);
PLUGINS_EXPORTDLL void        ts3plugin_onTextMessageEvent(uint64 serverConnectionHandlerID, anyID targetMode, anyID toID, anyID fromID, const char* fromName, const char* fromUniqueIdentifier, const char* message, int ffIgnored);
PLUGINS_EXPORTDLL void        ts3plugin_onClientMoveEvent(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, uint64 newChannelID, int visibility, const char* moveMessage);
PLUGINS_EXPORTDLL void        ts3plugin_onTalkStatusChangeEvent(uint64 serverConnectionHandlerID, int status, int isReceivedWhisper, anyID clientID);
PLUGINS_EXPORTDLL void        ts3plugin_onNewChannelEvent(uint64 serverConnectionHandlerID, uint64 channelID, uint64 channelParentID);

#ifdef __cplusplus
}
#endif

#endif /* PLUGIN_INTERFACE_H */
