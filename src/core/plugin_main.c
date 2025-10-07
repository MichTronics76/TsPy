/**
 * @file plugin_main.c
 * @brief Main plugin implementation
 * @author TeamSpeak Systems GmbH
 * @version 1.2.0
 * 
 * Copyright (c) TeamSpeak Systems GmbH
 */

#if defined(WIN32) || defined(__WIN32__) || defined(_WIN32)
#pragma warning(disable : 4100) /* Disable Unreferenced parameter warning */
#include <Windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "plugin_main.h"
#include "utils/logging.h"
#include "utils/string_utils.h"

/* Global variables */
static struct TS3Functions g_ts3Functions;
static char* g_pluginID = NULL;

/* ========================================================================
 * Internal Functions
 * ======================================================================== */

struct TS3Functions* get_ts3_functions(void)
{
    return &g_ts3Functions;
}

void set_ts3_functions(const struct TS3Functions funcs)
{
    g_ts3Functions = funcs;
}

const char* get_plugin_id(void)
{
    return g_pluginID;
}

void set_plugin_id(const char* id)
{
    if (g_pluginID != NULL) {
        free(g_pluginID);
        g_pluginID = NULL;
    }
    
    if (id != NULL) {
        const size_t sz = strlen(id) + 1;
        g_pluginID = (char*)malloc(sz * sizeof(char));
        if (g_pluginID != NULL) {
            safe_strcpy(g_pluginID, sz, id);
            log_info("Plugin ID registered: %s", g_pluginID);
        }
    }
}


