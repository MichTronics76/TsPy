/**
 * @file plugin_main.h
 * @brief Main plugin header file
 * @author TeamSpeak Systems GmbH
 * @version 1.2.0
 * 
 * Copyright (c) TeamSpeak Systems GmbH
 */

#ifndef PLUGIN_MAIN_H
#define PLUGIN_MAIN_H

#include "ts3_functions.h"
#include "teamspeak/public_definitions.h"
#include "teamspeak/public_errors.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Plugin version information
 */
#define PLUGIN_NAME        "TsPy Plugin"
#define PLUGIN_VERSION     "1.4.0"
#define PLUGIN_AUTHOR      "TeamSpeak Systems GmbH"
#define PLUGIN_DESCRIPTION "Professional TeamSpeak 3 client plugin with Python scripting"
#define PLUGIN_API_VERSION 26

/**
 * @brief Buffer size constants
 */
#define PATH_BUFSIZE       512
#define COMMAND_BUFSIZE    128
#define INFODATA_BUFSIZE   128
#define SERVERINFO_BUFSIZE 256
#define CHANNELINFO_BUFSIZE 512
#define RETURNCODE_BUFSIZE 128

/**
 * @brief Get the TeamSpeak 3 functions structure
 * @return Pointer to TS3Functions structure
 */
struct TS3Functions* get_ts3_functions(void);

/**
 * @brief Set the TeamSpeak 3 functions structure
 * @param funcs The TS3Functions structure to set
 */
void set_ts3_functions(const struct TS3Functions funcs);

/**
 * @brief Get the plugin ID
 * @return Pointer to plugin ID string
 */
const char* get_plugin_id(void);

/**
 * @brief Set the plugin ID
 * @param id The plugin ID to set
 */
void set_plugin_id(const char* id);

#ifdef __cplusplus
}
#endif

#endif /* PLUGIN_MAIN_H */
