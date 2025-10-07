/**
 * @file command_handler.h
 * @brief Command handler for plugin commands
 * @author TeamSpeak Systems GmbH
 * @version 1.2.0
 */

#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "teamspeak/public_definitions.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Process a plugin command
 * @param serverConnectionHandlerID Server connection handler ID
 * @param command Command string to process
 * @return 0 if handled, 1 if not handled
 */
int process_command(uint64 serverConnectionHandlerID, const char* command);

#ifdef __cplusplus
}
#endif

#endif /* COMMAND_HANDLER_H */
