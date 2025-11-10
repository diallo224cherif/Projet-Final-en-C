#ifndef STRINGS_H
#define STRINGS_H

#include <stddef.h>

size_t my_strlen(const char *s);
int    my_strcmp(const char *a, const char *b);
char  *my_strcpy(char *dst, const char *src);
char  *my_strncpy(char *dst, const char *src, size_t n);

#endif