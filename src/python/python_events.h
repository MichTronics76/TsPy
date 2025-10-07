/**
 * @file python_events.h
 * @brief Python event dispatcher interface
 * @author TsPy Team
 * @version 1.4.0
 * 
 * Dispatches TeamSpeak events to Python scripts
 */

#ifndef PYTHON_EVENTS_H
#define PYTHON_EVENTS_H

#include <stdint.h>
#include "teamspeak/public_definitions.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the Python event dispatcher
 * @return 0 on success, non-zero on failure
 */
int python_events_init(void);

/**
 * @brief Shutdown the Python event dispatcher
 */
void python_events_shutdown(void);

/**
 * @brief Dispatch onConnectStatusChange event to Python
 * @param serverConnectionHandlerID Server connection handler ID
 * @param newStatus New connection status
 * @param errorNumber Error code
 */
void python_event_on_connect_status_changed(uint64 serverConnectionHandlerID, int newStatus, unsigned int errorNumber);

/**
 * @brief Dispatch onClientMove event to Python
 * @param serverConnectionHandlerID Server connection handler ID
 * @param clientID Client ID
 * @param oldChannelID Old channel ID
 * @param newChannelID New channel ID
 * @param visibility Visibility (0 = enter, 1 = leave)
 * @param moveMessage Optional move message
 */
void python_event_on_client_move(uint64 serverConnectionHandlerID, anyID clientID, 
                                  uint64 oldChannelID, uint64 newChannelID, 
                                  int visibility, const char* moveMessage);

/**
 * @brief Dispatch onTextMessage event to Python
 * @param serverConnectionHandlerID Server connection handler ID
 * @param targetMode Target mode (1=client, 2=channel, 3=server)
 * @param toID Recipient ID
 * @param fromID Sender client ID
 * @param fromName Sender name
 * @param fromUniqueIdentifier Sender unique ID
 * @param message Message text
 */
void python_event_on_text_message(uint64 serverConnectionHandlerID, anyID targetMode,
                                   anyID toID, anyID fromID, const char* fromName,
                                   const char* fromUniqueIdentifier, const char* message);

/**
 * @brief Dispatch onTalkStatusChange event to Python
 * @param serverConnectionHandlerID Server connection handler ID
 * @param status Talk status (0=not talking, 1=talking)
 * @param isReceivedWhisper Is received whisper
 * @param clientID Client ID
 */
void python_event_on_talk_status_change(uint64 serverConnectionHandlerID, int status,
                                         int isReceivedWhisper, anyID clientID);

#ifdef __cplusplus
}
#endif

#endif /* PYTHON_EVENTS_H */
