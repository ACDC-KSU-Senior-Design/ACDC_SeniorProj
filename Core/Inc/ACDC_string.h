#ifndef __ACDC_STRING_H
#define __ACDC_STRING_H

#include "ACDC_stdbool.h"

int StringCompare(const char *str1, const char *str2);
int StringLength(const char *str);
char* StringConcat(char *dest, const char *source);
int StringIndexOf(char *str, char c);
char* StringSubstring(char *str, int index);
bool StringStartsWith(char *str, const char *compareWith);
bool StringEndsWith(char *str, const char* compareWith);

#endif