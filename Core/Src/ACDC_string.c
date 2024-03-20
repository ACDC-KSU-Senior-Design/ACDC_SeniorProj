/**
 * @file ACDC_string.c
 * @brief Implementation of string manipulation functions.
 * 
 * This file contains the implementation of various string manipulation functions,
 * including functions for case conversion, checking character types, and string comparison.
 * 
 * @author Devin Marx
 * @version 0.2
 * @date 2023-1-16
 * 
 * @copyright Copyright (c) 2023-2024
 */

#include "ACDC_string.h"

#define UPPER_TO_LOWER 32   //Number of characters to move to reach a lowercase character from a uppercase character
#define LOWER_TO_UPPER 32

char* StringCopy(char* dest, const char* source){
    int32_t i = 0;
    while(source[i] != '\0'){
        dest[i] = source[i];
        i++;
    }
    dest[i] = '\0';
    
    return dest;
}

int32_t StringCompare(const char *str1, const char *str2){
    int32_t i = 0;
    for(i = 0; i < StringLength(str1); i++) //Iterate over Length of string
        if(str1[i] != str2[i])               //If strings are not equal
            break;                          //Leave the for loop
    
    if(str1[i] < str2[i])
        return -1;
    else if(str1[i] > str2[i])
        return 1;
    else   
        return 0;
}

int32_t StringLength(const char *str){
    int32_t i = 0;                  //Initilize a counter
    while(str[++i] != '\0'){}   //Increment i when not at the end of the string
    return i;                   //Return counter
}

char* StringConcat(char *dest, const char *source){
    int32_t dstLen = StringLength(dest);    // Length of the Source
    int32_t i = dstLen;                     // Create iterator for the concatenation 
    while(source[i - dstLen] != '\0'){
        dest[i] = source[i - dstLen];   // Concat the source to the dest
        i++;
    }
    dest[i] = '\0';                     // Add null terminating char
    return dest;                        // Return the Destination point32_ter
}

int32_t StringIndexOf(const char *str, char c){
    for(int32_t i = 0; i < StringLength(str); i++)
        if(str[i] == c)
            return i;

    return -1;
}

char* StringSubstring(char *str, int32_t index){
    return str + index;
}

bool StringStartsWith(char *str, const char *compareWith){
    if(StringLength(str) < StringLength(compareWith))
        return false;
    
    str[StringLength(compareWith)] = '\0';
    return StringCompare(str, compareWith) == 0;
}

bool StringEndsWith(const char *str, const char* compareWith){
    int32_t stringDiff = StringLength(str) - StringLength(compareWith);
    
    if(stringDiff < 0)
        return false;
        
    return StringCompare(str + stringDiff, compareWith) == 0;
}


char* StringToUpper(char* str){
    if(str[0] == '\0')  // If the string is empty
        return str;     // return the original string

    for(int32_t i = 0; str[i] != '\0'; i++)
        if(str[i] >= 'a' && str[i] <= 'z')
            str[i] -= LOWER_TO_UPPER;

    return str;
}

char* StringToLower(char* str){
    if(str[0] == '\0')  // If the string is empty
        return str;     // return the original string

    for(int32_t i = 0; str[i] != '\0'; i++)
        if(str[i] >= 'A' && str[i] <= 'Z')
            str[i] += UPPER_TO_LOWER;

    return str;
}

bool StringIsUpper(const char* str){
    if(str[0] == '\0')
        return false;

    int32_t index = 0;
    while(str[index] != '\0'){
        if(str[index] < 'A' || str[index] > 'Z')
            return false;
        index++;
    }
    
    return true;
}

bool StringIsLower(const char* str){
    if(str[0] == '\0')
        return false;

    int32_t index = 0;
    while(str[index] != '\0'){
        if(str[index] < 'a' || str[index] > 'z')
            return false;
        index++;
    }
    
    return true;
}

bool StringIsNumeric(const char* str){
    if(str[0] == '\0')  // If the string is empty
        return false;

    int32_t index = 0;
    while(str[index] != '\0'){                    // Iterate over the entire string
        if(str[index] < '0' || str[index] > '9')  // If the index is not a number 0-9
            return false;
        index++;
    }

    return true;
}

bool StringIsAlphabetic(const char* str){
    if(str[0] == '\0')  // If the string is empty
        return false;
    
    int32_t index = 0;
    while(str[index] != '\0'){                                                              // Iterate over the entire string
        if(str[index] < 'A' || str[index] > 'z' || (str[index] > 'Z' && str[index] < 'a'))  // If the index is not a letter a-z or A-Z
            return false;
        index++;
    }
    
    return true;
}

bool StringIsAlphanumeric(const char* str){
    if(str[0] == '\0')  // If the string is empty
        return false;

    int32_t index = 0;
    while(str[index] != '\0'){  // Iterate over the entire string      
        if(str[index] < '0' || str[index] > 'z' || (str[index] > 'Z' && str[index] < 'a')  
           || (str[index] > '9' && str[index] < 'A'))   // Checks if the current index is a letter a-z or A-Z or number 0-9
                return false;
        index++;
    }

    return true;
}

char *StringConvert(int32_t num){
    // maximum number of characters in a int32_t (-2,147,483,647 -> 2,147,483,647 or 11 chars)
    static char buffer[12];         // 11 numbers + 1 null terminating bit
    bool isNeg = false;
    int strLen = 0;

    if(num == 0)                        // If the number is 0
        return StringCopy(buffer, "0"); // Set dest to 0 and return
    else if(num < 0){                   // If the number is negative
        isNeg = true;                   // Set the isNegative variable true
        num = -num;                     // Temporarily convert it to positive
    }

    while(num != 0){                             // If the remaining number to convert is 0 (no more characters to convert)
        buffer[strLen++] = "0123456789"[num % 10]; // Grab the next digit in the 1's place
        num /= 10;                               // Shift the entire number down 1 place (Ex. 1234 -> 123 -> 12 -> 1)
    }

    if(isNeg)                           // If the number was negative
        buffer[strLen++] = '-';         // Append the negative sign onto the string

    for(int i = 0; i < strLen/2; i++){  // Reverse the string
        char temp = buffer[i];
        buffer[i] = buffer[strLen-i-1];
        buffer[strLen-i-1] = temp;
    }
    buffer[strLen] = '\0';              // Append the null terminating character
    return buffer;
}