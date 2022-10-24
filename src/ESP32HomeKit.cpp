#ifndef ESP32HomeKit_h
#define ESP32HomeKit_h

#include <Arduino.h>
#include <ESP32HomeKit.h>
#include "../esp-homekit-lib/priv_include/hap.h"
#include "../esp-homekit-lib/priv_include/hap_apple_servs.h"
#include "../esp-homekit-lib/priv_include/hap_apple_chars.h"

char* ConvertStringToCharArray(String arduinoString) {
    int str_len = arduinoString.length() + 1; 
    char char_array[str_len];
    arduinoString.toCharArray(char_array, str_len);
    return char_array;
}

// Accesories
HAPAccessory::HAPAccessory(String deviceName) {
    _deviceName = deviceName;
}

int HAPAccessory::Initialise(String model, String manufacturer, String serial_num, String fw_rev, String pv, IdentityHandler identityHandler, HAPAccessoryType cid) {
    _model = model;
    _manufacturer = manufacturer;
    _serial_num = serial_num;
    _fw_rev = fw_rev;
    _pv = pv;
    _identityHandler = identityHandler;
    _cid = cid;
        
    hap_acc_cfg_t accessoryConfig = HAPAccessory::CreateAccessoryConfig();

    Accessory = hap_acc_create(&accessoryConfig);

    /* Initialize the HAP core */
    hap_init(HAP_TRANSPORT_WIFI);

    /* Add a dummy Product Data */
	uint8_t product_data[] = {'E', 'S', 'P', '3', '2', 'K', 'I', 'T'};
    hap_acc_add_product_data(Accessory, product_data, sizeof(product_data));
    return HAP_SUCCESS;
}

int HAPAccessory::AddService(HAPServiceHandle* service) {
    return hap_acc_add_serv(Accessory, service); 
}

int HAPAccessory::Register(String setupCode) {
    hap_add_accessory(Accessory);
    hap_set_setup_code(ConvertStringToCharArray(setupCode));
    return hap_start();
}

hap_acc_cfg_t HAPAccessory::CreateAccessoryConfig() {
    hap_acc_cfg_t hapConfig = {
        .name = ConvertStringToCharArray(_deviceName),
        .model = ConvertStringToCharArray(_model),
        .manufacturer = ConvertStringToCharArray(_manufacturer),
        .serial_num = ConvertStringToCharArray(_serial_num),
        .fw_rev = ConvertStringToCharArray(_fw_rev),
        .hw_rev = NULL,
        .pv = ConvertStringToCharArray(_pv),
        .cid = (hap_cid_t) _cid,
        .identify_routine = _identityHandler
    };

    return hapConfig;
}

// Services
HAPTemperatureSensor::HAPTemperatureSensor(String serviceName, float currentTemperature) {
    Service = hap_serv_temperature_sensor_create(currentTemperature);
    hap_serv_add_char(Service, hap_char_name_create(ConvertStringToCharArray(serviceName)));
}

#endif