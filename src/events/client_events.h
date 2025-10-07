/**
 * @file client_events.h
 * @brief Client event handlers
 * @author TeamSpeak Systems GmbH
 * @version 1.2.0
 */

#ifndef CLIENT_EVENTS_H
#define CLIENT_EVENTS_H

#include "teamspeak/public_definitions.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Handle client move event
 * @param serverConnectionHandlerID Server connection handler ID
 * @param clientID Client ID
 * @param oldChannelID Old channel ID
 * @param newChannelID New channel ID
 * @param visibility Visibility status
 */
void on_client_move(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID, 
                    uint64 newChannelID, int visibility);

/**
 * @brief Handle talk status change event
 * @param serverConnectionHandlerID Server connection handler ID
 * @param status Talk status
 * @param isReceivedWhisper Is received whisper
 * @param clientID Client ID
 */
void on_talk_status_changed(uint64 serverConnectionHandlerID, int status, int isReceivedWhisper, anyID clientID);

#ifdef __cplusplus
}
#endif

#endif /* CLIENT_EVENTS_H */
