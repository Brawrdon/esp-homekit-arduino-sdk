#ifndef ESP32HomeKit_h
#define ESP32HomeKit_h

#include <Arduino.h>
#include <ESP32HomeKit.h>
#include "../esp-homekit-lib/priv_include/hap.h"


char* ConvertStringToCharArray(String arduinoString) {
    int str_len = arduinoString.length() + 1; 
    char char_array[str_len];
    arduinoString.toCharArray(char_array, str_len);
    return char_array;
}

HAPAccessory::HAPAccessory(String deviceName) {
    _deviceName = deviceName;
}

int HAPAccessory::InitialiseHAP(String model, String manufacturer, String serial_num, String fw_rev, String pv, IdentityRoutine identityRoutine, Cid cid) {
    _model = model;
    _manufacturer = manufacturer;
    _serial_num = serial_num;
    _fw_rev = fw_rev;
    _pv = pv;
    _identityRoutine = identityRoutine;
    _cid = cid;
        

    hap_acc_cfg_t accessoryConfig HAPAccessory::CreateAccessoryConfig();

    _accessory = hap_acc_create(&accessoryConfig);

    /* Initialize the HAP core */
    hap_init(HAP_TRANSPORT_WIFI);

    return HAP_SUCCESS;
}

hap_acc_cfg_t HAPAccessory::CreateAccessoryConfig() {
    hap_acc_cfg_t cfg = {
        .name = ConvertStringToCharArray(_deviceName),
        .model = ConvertStringToCharArray(_model),
        .manufacturer = ConvertStringToCharArray(_manufacturer),
        .serial_num = ConvertStringToCharArray(_serial_num),
        .fw_rev = ConvertStringToCharArray(_fw_rev),
        .hw_rev = NULL,
        .pv = ConvertStringToCharArray(_pv),
        .cid = _cid,
        .identify_routine = _identityRoutine
    };
}

#endif