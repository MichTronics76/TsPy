/**
 * @file python_api.h
 * @brief Python API module for TeamSpeak functions
 * @author TsPy Team
 * @version 1.3.0
 * 
 * Exposes TeamSpeak functions to Python scripts
 */

#ifndef PYTHON_API_H
#define PYTHON_API_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the Python API module
 * @return 0 on success, non-zero on failure
 */
int python_api_init(void);

/**
 * @brief Shutdown the Python API module
 */
void python_api_shutdown(void);

#ifdef __cplusplus
}
#endif

#endif /* PYTHON_API_H */
