#include <HAPHelpers.h>

char* ConvertStringToCharArray(String arduinoString) {
    int str_len = arduinoString.length() + 1; 
    char* char_array = (char*) malloc(str_len);
    arduinoString.toCharArray(char_array, str_len);
    return char_array;
}

void HAPLog(String message) {
     Serial.println("(ESP_HOMEKIT_ARDUINO_SDK) -> " + message);
}

