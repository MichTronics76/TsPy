/**
 * @file menu_handler.h
 * @brief Menu handler for plugin UI
 * @author TeamSpeak Systems GmbH
 * @version 1.2.0
 */

#ifndef MENU_HANDLER_H
#define MENU_HANDLER_H

#include "teamspeak/public_definitions.h"
#include "plugin_definitions.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize menu items
 * @param menuItems Pointer to menu items array
 * @param menuIcon Pointer to menu icon path
 */
void init_menu_items(struct PluginMenuItem*** menuItems, char** menuIcon);

/**
 * @brief Handle menu item selection
 * @param serverConnectionHandlerID Server connection handler ID
 * @param menuID Menu item ID
 */
void handle_menu_item(uint64 serverConnectionHandlerID, int menuID);

#ifdef __cplusplus
}
#endif

#endif /* MENU_HANDLER_H */
