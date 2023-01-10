#include <HAPHelpers.h>

char* ConvertStringToCharArray(String arduinoString) {
    int str_len = arduinoString.length() + 1; 
    char* char_array = (char*) malloc(str_len);
    arduinoString.toCharArray(char_array, str_len);
    return char_array;
}

