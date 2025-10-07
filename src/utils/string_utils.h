/**
 * @file string_utils.h
 * @brief String utility functions
 * @author TeamSpeak Systems GmbH
 * @version 1.2.0
 */

#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Safe string copy function
 * @param dest Destination buffer
 * @param destSize Size of destination buffer
 * @param src Source string
 */
void safe_strcpy(char* dest, size_t destSize, const char* src);

#ifdef _WIN32
/**
 * @brief Convert wchar_t to UTF-8 encoded string
 * @param str Wide character string
 * @param result Pointer to result string (will be allocated)
 * @return 0 on success, -1 on failure
 */
int wchar_to_utf8(const wchar_t* str, char** result);
#endif

#ifdef __cplusplus
}
#endif

#endif /* STRING_UTILS_H */
