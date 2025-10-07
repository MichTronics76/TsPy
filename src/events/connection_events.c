/**
 * @file connection_events.c
 * @brief Connection event handlers implementation
 * @author TeamSpeak Systems GmbH
 * @version 1.2.0
 */

#include "connection_events.h"
#include "core/plugin_main.h"
#include "utils/logging.h"

void on_connect_status_changed(uint64 serverConnectionHandlerID, int newStatus, unsigned int errorNumber)
{
    log_info("Connection status changed: Server=%llu, Status=%d, Error=%u", 
             (unsigned long long)serverConnectionHandlerID, newStatus, errorNumber);

    /* Handle different connection states */
    switch (newStatus) {
        case STATUS_DISCONNECTED:
            log_info("Disconnected from server");
            break;
        case STATUS_CONNECTING:
            log_info("Connecting to server...");
            break;
        case STATUS_CONNECTED:
            log_info("Connected to server");
            break;
        case STATUS_CONNECTION_ESTABLISHING:
            log_info("Establishing connection...");
            break;
        case STATUS_CONNECTION_ESTABLISHED:
            log_info("Connection established");
            break;
        default:
            log_debug("Unknown connection status: %d", newStatus);
            break;
    }

    if (errorNumber != 0) {
        log_error("Connection error occurred: %u", errorNumber);
    }
}
