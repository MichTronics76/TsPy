/**
 * @file logging.h
 * @brief Logging utility functions
 * @author TeamSpeak Systems GmbH
 * @version 1.2.0
 */

#ifndef LOGGING_H
#define LOGGING_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Log levels
 */
typedef enum {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR
} LogLevel;

/**
 * @brief Log a debug message
 * @param format Format string
 * @param ... Variable arguments
 */
void log_debug(const char* format, ...);

/**
 * @brief Log an info message
 * @param format Format string
 * @param ... Variable arguments
 */
void log_info(const char* format, ...);

/**
 * @brief Log a warning message
 * @param format Format string
 * @param ... Variable arguments
 */
void log_warning(const char* format, ...);

/**
 * @brief Log an error message
 * @param format Format string
 * @param ... Variable arguments
 */
void log_error(const char* format, ...);

#ifdef __cplusplus
}
#endif

#endif /* LOGGING_H */
