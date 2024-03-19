/**
 * @file ACDC_string.h
 * @author Devin Marx
 * @brief Header file containing String functions
 * @version 0.2
 * @date 2023-1-16
 * 
 * @copyright Copyright (c) 2023-2024
 */

#ifndef __ACDC_STRING_H
#define __ACDC_STRING_H

#include "ACDC_stdbool.h"

/// @brief Copies the string pointed by source (including the null character) to the destination dest
/// @param destination Destination buffer to copy the source string to
/// @param source String to be copied
/// @return Pointer to the copied string.
char* StringCopy(char* dest, const char* source);

/// @brief Compares two strings character by character. 
///        Returns 0 if the strings are equal, >0 if the first non-matching character in str1 is greater than str2,
///        and <0 if the first non-matching character in str1 is less than str2.
/// @param str1 First string to compare with
/// @param str2 Second string to compare with
/// @return 0 if str1 == str2, >0 if the first non-matching character in str1 > str2,
///         and <0 if the first non-matching character in str1 < str2.
int StringCompare(const char *str1, const char *str2);

/// @brief Calculates the length of the string str.
/// @param str String to find the length of
/// @return Length of the string.
int StringLength(const char *str);

/// @brief Appends the content of the string source to the end of the string dest
/// @param dest Destination string to which the source string will be appended
/// @param source Source string to be appended to the destination
/// @return Pointer to the concatenated string (dest + source). The original string is modified
char* StringConcat(char *dest, const char *source);

/// @brief Finds and returns the index of first occurance the character c in the string str
/// @param str String to search
/// @param c Character to search for
/// @return Index of the char c in the string str. Returns -1 if the character is not found.
int StringIndexOf(const char *str, char c);

/// @brief Returns a substring of the input string str starting from the specified index
/// @param str Input string
/// @param index Starting index of the substring
/// @return Pointer to the substring starting from the given index.
char* StringSubstring(char *str, int index);

/// @brief Checks if the given string str starts with the specified substring compareWith
/// @param str Input string
/// @param compareWith Substring to check for at the beginning of str
/// @return True if the string str starts with compareWith, otherwise false.
bool StringStartsWith(char *str, const char *compareWith);

/// @brief Checks if the given string str ends with the specified substring compareWith
/// @param str Input string
/// @param compareWith Substring to check for at the end of the string str
/// @return True if the string str ends with compareWith, otherwise false.
bool StringEndsWith(const char *str, const char* compareWith);

/// @brief Converts all lowercase characters a-z in the string str to uppercase
/// @param str Input string to convert
/// @return Pointer to the modified string
char* StringToUpper(char* str);

/// @brief Converts all uppercase characters A-Z in the string str to lowercase
/// @param str Input string to convert
/// @return Pointer to the modified string
char* StringToLower(char* str);

/// @brief Checks if all characters in the input string are uppercase characters (can only contain characters)
/// @param str Input string to convert
/// @return True if all characters are uppercase characters, otherwise false
bool StringIsUpper(const char* str);

/// @brief Checks if all characters in the input string are lowercase characters (can only contain characters)
/// @param str Input string to convert
/// @return Trueif all characters are lowercase characaters, otherwise false
bool StringIsLower(const char* str);

/// @brief Checks if all characters in the input string are numeric
/// @param str Input string to check
/// @return True if all characters are numeric, otherwise false
bool StringIsNumeric(const char* str);

/// @brief Checks if all characters in the input string are alphabetic.
/// @param str Input string to check
/// @return True if all characters are alphabetic, otherwise false.
bool StringIsAlphabetic(const char* str);

/// @brief Checks if all characters in the input string are alphanumeric.
/// @param str Input string to check
/// @return True if all characters are alphanumeric, otherwise false.
bool StringIsAlphanumeric(const char* str);

/// @brief Converts an int to a string.
/// @param num number to convert
/// @param buffer pointer to the char to store the converted number in
void StringConvert(int num, char* buffer);

#endif