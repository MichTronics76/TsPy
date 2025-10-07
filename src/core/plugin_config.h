/**
 * @file plugin_config.h
 * @brief Plugin configuration management
 * @author TeamSpeak Systems GmbH
 * @version 1.4.0
 */

#ifndef PLUGIN_CONFIG_H
#define PLUGIN_CONFIG_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize plugin configuration
 * @param configPath Path to configuration directory
 * @return true on success, false on failure
 */
bool init_plugin_config(const char* configPath);

/**
 * @brief Cleanup plugin configuration
 */
void cleanup_plugin_config(void);

/**
 * @brief Load configuration from file
 * @return true on success, false on failure
 */
bool load_config(void);

/**
 * @brief Save configuration to file
 * @return true on success, false on failure
 */
bool save_config(void);

#ifdef __cplusplus
}
#endif

#endif /* PLUGIN_CONFIG_H */
