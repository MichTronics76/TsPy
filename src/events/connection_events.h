/**
 * @file connection_events.h
 * @brief Connection event handlers
 * @author TeamSpeak Systems GmbH
 * @version 1.2.0
 */

#ifndef CONNECTION_EVENTS_H
#define CONNECTION_EVENTS_H

#include "teamspeak/public_definitions.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Handle connection status change event
 * @param serverConnectionHandlerID Server connection handler ID
 * @param newStatus New connection status
 * @param errorNumber Error number if any
 */
void on_connect_status_changed(uint64 serverConnectionHandlerID, int newStatus, unsigned int errorNumber);

#ifdef __cplusplus
}
#endif

#endif /* CONNECTION_EVENTS_H */
