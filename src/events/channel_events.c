/**
 * @file channel_events.c
 * @brief Channel event handlers implementation
 * @author TeamSpeak Systems GmbH
 * @version 1.2.0
 */

#include "channel_events.h"
#include "core/plugin_main.h"
#include "utils/logging.h"

void on_new_channel(uint64 serverConnectionHandlerID, uint64 channelID, uint64 channelParentID)
{
    log_debug("New channel: Server=%llu, Channel=%llu, Parent=%llu",
             (unsigned long long)serverConnectionHandlerID,
             (unsigned long long)channelID,
             (unsigned long long)channelParentID);
}

void on_new_channel_created(uint64 serverConnectionHandlerID, uint64 channelID, uint64 channelParentID,
                            anyID invokerID, const char* invokerName, const char* invokerUniqueIdentifier)
{
    (void)invokerUniqueIdentifier; /* Unused parameter */
    
    log_info("Channel created: Server=%llu, Channel=%llu, Parent=%llu, Invoker=%s (ID=%d)",
             (unsigned long long)serverConnectionHandlerID,
             (unsigned long long)channelID,
             (unsigned long long)channelParentID,
             invokerName ? invokerName : "Unknown",
             invokerID);
}

void on_del_channel(uint64 serverConnectionHandlerID, uint64 channelID, anyID invokerID,
                    const char* invokerName, const char* invokerUniqueIdentifier)
{
    (void)invokerUniqueIdentifier; /* Unused parameter */
    
    log_info("Channel deleted: Server=%llu, Channel=%llu, Invoker=%s (ID=%d)",
             (unsigned long long)serverConnectionHandlerID,
             (unsigned long long)channelID,
             invokerName ? invokerName : "Unknown",
             invokerID);
}
