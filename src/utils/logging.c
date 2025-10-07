/**
 * @file logging.c
 * @brief Logging utility implementation
 * @author TeamSpeak Systems GmbH
 * @version 1.2.0
 */

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "logging.h"
#include "core/plugin_main.h"
#include "teamlog/logtypes.h"

static const char* get_log_level_string(LogLevel level)
{
    switch (level) {
        case LOG_LEVEL_DEBUG:   return "DEBUG";
        case LOG_LEVEL_INFO:    return "INFO";
        case LOG_LEVEL_WARNING: return "WARNING";
        case LOG_LEVEL_ERROR:   return "ERROR";
        default:                return "UNKNOWN";
    }
}

static void log_message(LogLevel level, const char* format, va_list args)
{
    char buffer[1024];
    
    vsnprintf(buffer, sizeof(buffer), format, args);
    buffer[sizeof(buffer) - 1] = '\0';

    /* Print to console */
    printf("[TsPy][%s] %s\n", get_log_level_string(level), buffer);

    /* Send to TeamSpeak log */
    struct TS3Functions* ts3Functions = get_ts3_functions();
    if (ts3Functions != NULL && ts3Functions->logMessage != NULL) {
        int ts3LogLevel;
        switch (level) {
            case LOG_LEVEL_DEBUG:
                ts3LogLevel = LogLevel_DEBUG;
                break;
            case LOG_LEVEL_INFO:
                ts3LogLevel = LogLevel_INFO;
                break;
            case LOG_LEVEL_WARNING:
                ts3LogLevel = LogLevel_WARNING;
                break;
            case LOG_LEVEL_ERROR:
                ts3LogLevel = LogLevel_ERROR;
                break;
            default:
                ts3LogLevel = LogLevel_INFO;
                break;
        }
        ts3Functions->logMessage(buffer, ts3LogLevel, "TsPy Plugin", 0);
    }
}

void log_debug(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    log_message(LOG_LEVEL_DEBUG, format, args);
    va_end(args);
}

void log_info(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    log_message(LOG_LEVEL_INFO, format, args);
    va_end(args);
}

void log_warning(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    log_message(LOG_LEVEL_WARNING, format, args);
    va_end(args);
}

void log_error(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    log_message(LOG_LEVEL_ERROR, format, args);
    va_end(args);
}
