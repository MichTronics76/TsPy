/**
 * @file channel_events.h
 * @brief Channel event handlers
 * @author TeamSpeak Systems GmbH
 * @version 1.2.0
 */

#ifndef CHANNEL_EVENTS_H
#define CHANNEL_EVENTS_H

#include "teamspeak/public_definitions.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Handle new channel event
 * @param serverConnectionHandlerID Server connection handler ID
 * @param channelID Channel ID
 * @param channelParentID Parent channel ID
 */
void on_new_channel(uint64 serverConnectionHandlerID, uint64 channelID, uint64 channelParentID);

/**
 * @brief Handle channel created event
 * @param serverConnectionHandlerID Server connection handler ID
 * @param channelID Channel ID
 * @param channelParentID Parent channel ID
 * @param invokerID Invoker client ID
 * @param invokerName Invoker name
 * @param invokerUniqueIdentifier Invoker unique ID
 */
void on_new_channel_created(uint64 serverConnectionHandlerID, uint64 channelID, uint64 channelParentID,
                            anyID invokerID, const char* invokerName, const char* invokerUniqueIdentifier);

/**
 * @brief Handle channel deleted event
 * @param serverConnectionHandlerID Server connection handler ID
 * @param channelID Channel ID
 * @param invokerID Invoker client ID
 * @param invokerName Invoker name
 * @param invokerUniqueIdentifier Invoker unique ID
 */
void on_del_channel(uint64 serverConnectionHandlerID, uint64 channelID, anyID invokerID, 
                    const char* invokerName, const char* invokerUniqueIdentifier);

#ifdef __cplusplus
}
#endif

#endif /* CHANNEL_EVENTS_H */
