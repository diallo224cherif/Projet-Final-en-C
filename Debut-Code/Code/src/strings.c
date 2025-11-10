//
// Created by brouc on 22/10/2025.
//

#include "strings.h"

size_t my_strlen(const char *s) {
    size_t n = 0;
    while (s && s[n] != '\0') n++;
    return n;
}

int my_strcmp(const char *a, const char *b) {
    size_t i = 0;
    while (a[i] && b[i] && a[i] == b[i]) i++;
    return (unsigned char)a[i] - (unsigned char)b[i];
}

char *my_strcpy(char *dst, const char *src) {
    size_t i = 0;
    while (src[i]) { dst[i] = src[i]; i++; }
    dst[i] = '\0';
    return dst;
}

char *my_strncpy(char *dst, const char *src, size_t n) {
    size_t i = 0;
    for (; i < n && src[i]; i++) dst[i] = src[i];
    for (; i < n; i++) dst[i] = '\0';
    return dst;
}
