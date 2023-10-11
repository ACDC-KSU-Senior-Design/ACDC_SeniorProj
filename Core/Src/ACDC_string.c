#include "ACDC_string.h"

int StringCompare(const char *str1, const char *str2){
    int i = 0;
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

int StringLength(const char *str){
    int i = 0;                  //Initilize a counter
    while(str[++i] != '\0'){}   //Increment i when not at the end of the string
    return i;                   //Return counter
}

char* StringConcat(char *dest, const char *source){
    int srcLen = StringLength(source);  //Length of the Source
    int dstLen = StringLength(dest);    //Length of the Destination

    for(int i = 0; i < srcLen + dstLen; i++)
        dest[i] = source[i-dstLen];     //Concat the source to the dest

    dest[srcLen + dstLen] = '\0';       //Add null terminating char

    return dest;                        //Return the Destination
}

int StringIndexOf(char *str, char c){
    for(int i = 0; i < StringLength(str); i++)
        if(str[i] == c)
            return i;

    return -1;
}

char* StringSubstring(char *str, int index){
    return str + index;
}

bool StringStartsWith(char *str, const char *compareWith){
    if(StringLength(str) < StringLength(compareWith))
        return false;
    
    str[StringLength(compareWith)] = '\0';
    return StringCompare(str, compareWith) == 0;
}

bool StringEndsWith(char *str, const char* compareWith){
    int stringDiff = StringLength(str) - StringLength(compareWith);
    
    if(stringDiff < 0)
        return false;
        
    return StringCompare(str + stringDiff, compareWith) == 0;
}
