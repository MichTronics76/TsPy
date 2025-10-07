/**
 * @file menu_handler.c
 * @brief Menu handler implementation
 * @author TeamSpeak Systems GmbH
 * @version 1.2.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "menu_handler.h"
#include "core/plugin_main.h"
#include "utils/logging.h"
#include "utils/string_utils.h"

enum {
    MENU_ID_INFO = 1,
    MENU_ID_TOGGLE,
    MENU_ID_SEPARATOR,
    MENU_ID_ABOUT
};

/* Helper function to create a menu item */
static struct PluginMenuItem* create_menu_item(enum PluginMenuType type, int id, const char* text, const char* icon)
{
    struct PluginMenuItem* menuItem = (struct PluginMenuItem*)malloc(sizeof(struct PluginMenuItem));
    if (menuItem == NULL) {
        return NULL;
    }
    
    menuItem->type = type;
    menuItem->id = id;
    safe_strcpy(menuItem->text, PLUGIN_MENU_BUFSZ, text);
    safe_strcpy(menuItem->icon, PLUGIN_MENU_BUFSZ, icon);
    return menuItem;
}

void init_menu_items(struct PluginMenuItem*** menuItems, char** menuIcon)
{
    const size_t itemCount = 4;
    size_t n = 0;
    
    log_info("Initializing menu items...");

    /* Allocate array of pointers - TeamSpeak will free this via ts3plugin_freeMemory */
    *menuItems = (struct PluginMenuItem**)malloc(sizeof(struct PluginMenuItem*) * (itemCount + 1));
    if (*menuItems == NULL) {
        log_error("Failed to allocate menu items array");
        return;
    }

    /* Create menu items - each allocated separately so TeamSpeak can free them */
    (*menuItems)[n++] = create_menu_item(PLUGIN_MENU_TYPE_GLOBAL, MENU_ID_INFO, "TsPy Plugin Info", "");
    (*menuItems)[n++] = create_menu_item(PLUGIN_MENU_TYPE_GLOBAL, MENU_ID_TOGGLE, "Toggle Feature", "");
    (*menuItems)[n++] = create_menu_item(PLUGIN_MENU_TYPE_GLOBAL, MENU_ID_SEPARATOR, "", "");
    (*menuItems)[n++] = create_menu_item(PLUGIN_MENU_TYPE_GLOBAL, MENU_ID_ABOUT, "About", "");
    (*menuItems)[n++] = NULL; /* Terminate array */

    /* Set menu icon (optional) - TeamSpeak will free this too */
    *menuIcon = NULL;
    
    log_info("Menu items initialized (%zu items)", itemCount);
}

void handle_menu_item(uint64 serverConnectionHandlerID, int menuID)
{
    struct TS3Functions* ts3Functions = get_ts3_functions();
    
    (void)serverConnectionHandlerID; /* May be used for server-specific operations */
    
    log_info("Menu item selected: %d", menuID);

    switch (menuID) {
        case MENU_ID_INFO:
            if (ts3Functions && ts3Functions->printMessageToCurrentTab) {
                ts3Functions->printMessageToCurrentTab("TsPy Plugin - Professional TeamSpeak 3 Plugin");
            }
            break;
            
        case MENU_ID_TOGGLE:
            if (ts3Functions && ts3Functions->printMessageToCurrentTab) {
                ts3Functions->printMessageToCurrentTab("Feature toggled");
            }
            break;
            
        case MENU_ID_ABOUT:
            if (ts3Functions && ts3Functions->printMessageToCurrentTab) {
                char msg[256];
                snprintf(msg, sizeof(msg), "TsPy Plugin v%s - %s", PLUGIN_VERSION, PLUGIN_AUTHOR);
                ts3Functions->printMessageToCurrentTab(msg);
            }
            break;
            
        default:
            log_warning("Unknown menu ID: %d", menuID);
            break;
    }
}
