/**
 * @file client_events.c
 * @brief Client event handlers implementation
 * @author TeamSpeak Systems GmbH
 * @version 1.2.0
 */

#include "client_events.h"
#include "core/plugin_main.h"
#include "utils/logging.h"

void on_client_move(uint64 serverConnectionHandlerID, anyID clientID, uint64 oldChannelID,
                    uint64 newChannelID, int visibility)
{
    log_debug("Client moved: Server=%llu, Client=%d, From=%llu, To=%llu, Visibility=%d",
             (unsigned long long)serverConnectionHandlerID,
             clientID,
             (unsigned long long)oldChannelID,
             (unsigned long long)newChannelID,
             visibility);
}

void on_talk_status_changed(uint64 serverConnectionHandlerID, int status, int isReceivedWhisper, anyID clientID)
{
    const char* statusStr = "Unknown";
    
    switch (status) {
        case STATUS_NOT_TALKING:
            statusStr = "Not Talking";
            break;
        case STATUS_TALKING:
            statusStr = "Talking";
            break;
        case STATUS_TALKING_WHILE_DISABLED:
            statusStr = "Talking (Disabled)";
            break;
        default:
            break;
    }

    log_debug("Talk status changed: Server=%llu, Client=%d, Status=%s, Whisper=%d",
             (unsigned long long)serverConnectionHandlerID,
             clientID,
             statusStr,
             isReceivedWhisper);
}
