/**
 * @file string_utils.c
 * @brief String utility functions implementation
 * @author TeamSpeak Systems GmbH
 * @version 1.2.0
 */

#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <Windows.h>
#endif

#include "string_utils.h"

void safe_strcpy(char* dest, size_t destSize, const char* src)
{
    if (dest == NULL || src == NULL || destSize == 0) {
        return;
    }

#ifdef _WIN32
    strcpy_s(dest, destSize, src);
#else
    strncpy(dest, src, destSize - 1);
    dest[destSize - 1] = '\0';
#endif
}

#ifdef _WIN32
int wchar_to_utf8(const wchar_t* str, char** result)
{
    if (str == NULL || result == NULL) {
        return -1;
    }

    int outlen = WideCharToMultiByte(CP_UTF8, 0, str, -1, 0, 0, 0, 0);
    if (outlen <= 0) {
        return -1;
    }

    *result = (char*)malloc(outlen);
    if (*result == NULL) {
        return -1;
    }

    if (WideCharToMultiByte(CP_UTF8, 0, str, -1, *result, outlen, 0, 0) == 0) {
        free(*result);
        *result = NULL;
        return -1;
    }

    return 0;
}
#endif
