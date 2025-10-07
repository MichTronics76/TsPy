/**
 * @file hotkey_handler.h
 * @brief Hotkey handler for plugin
 * @author TeamSpeak Systems GmbH
 * @version 1.2.0
 */

#ifndef HOTKEY_HANDLER_H
#define HOTKEY_HANDLER_H

#include "teamspeak/public_definitions.h"
#include "plugin_definitions.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize hotkeys
 * @param hotkeys Pointer to hotkeys array
 */
void init_hotkeys(struct PluginHotkey*** hotkeys);

/**
 * @brief Handle hotkey press
 * @param keyword Hotkey keyword
 */
void handle_hotkey(const char* keyword);

#ifdef __cplusplus
}
#endif

#endif /* HOTKEY_HANDLER_H */
