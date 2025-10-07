/**
 * @file hotkey_handler.c
 * @brief Hotkey handler implementation
 * @author TeamSpeak Systems GmbH
 * @version 1.2.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hotkey_handler.h"
#include "core/plugin_main.h"
#include "utils/logging.h"
#include "utils/string_utils.h"

/* Helper function to create a hotkey */
static struct PluginHotkey* create_hotkey(const char* keyword, const char* description)
{
    struct PluginHotkey* hotkey = (struct PluginHotkey*)malloc(sizeof(struct PluginHotkey));
    if (hotkey == NULL) {
        return NULL;
    }
    
    safe_strcpy(hotkey->keyword, PLUGIN_HOTKEY_BUFSZ, keyword);
    safe_strcpy(hotkey->description, PLUGIN_HOTKEY_BUFSZ, description);
    return hotkey;
}

void init_hotkeys(struct PluginHotkey*** hotkeys)
{
    const size_t hotkeyCount = 1;
    size_t n = 0;
    
    log_info("Initializing hotkeys...");

    /* Allocate array of pointers - TeamSpeak will free this via ts3plugin_freeMemory */
    *hotkeys = (struct PluginHotkey**)malloc(sizeof(struct PluginHotkey*) * (hotkeyCount + 1));
    if (*hotkeys == NULL) {
        log_error("Failed to allocate hotkeys array");
        return;
    }

    /* Create hotkeys - each allocated separately so TeamSpeak can free them */
    (*hotkeys)[n++] = create_hotkey("tspy_toggle", "Toggle TsPy feature");
    (*hotkeys)[n++] = NULL; /* Terminate array */
    
    log_info("Hotkeys initialized (%zu items)", hotkeyCount);
}

void handle_hotkey(const char* keyword)
{
    struct TS3Functions* ts3Functions = get_ts3_functions();
    
    log_info("Hotkey pressed: %s", keyword);

    if (strcmp(keyword, "tspy_toggle") == 0) {
        if (ts3Functions && ts3Functions->printMessageToCurrentTab) {
            ts3Functions->printMessageToCurrentTab("TsPy feature toggled via hotkey");
        }
    } else {
        log_warning("Unknown hotkey: %s", keyword);
    }
}
