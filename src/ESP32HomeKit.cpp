#ifndef ESP32HomeKit_h
#define ESP32HomeKit_h

#include <Arduino.h>
#include <ESP32HomeKit.h>

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

static int fan_write(hap_write_data_t write_data[], int count, void *serv_priv, void *write_priv) {
 if (hap_req_get_ctrl_id(write_priv)) {
        ESP_LOGI(TAG, "Received write from %s", hap_req_get_ctrl_id(write_priv));
    }
    return 1;
}    

hap_acc_t* accesory;
hap_serv_t* service;

int HAPAccessory::Initialise(String model, String manufacturer, String serial_num, String fw_rev, String pv, IdentityHandler identityHandler, HAPAccessoryType cid) {
    _model = model;
    _manufacturer = manufacturer;
    _serial_num = serial_num;
    _fw_rev = fw_rev;
    _pv = pv;
    _identityHandler = identityHandler;
    _cid = cid;
    
    Serial.println("Initliased");
    hap_acc_cfg_t accessoryConfig = HAPAccessory::CreateAccessoryConfig();
    accesory = hap_acc_create(&accessoryConfig);
    Serial.println("Config created");

    /* Initialize the HAP core */
    hap_init(HAP_TRANSPORT_WIFI);
    Serial.println("Core initialised");


    /* Add a dummy Product Data */
	uint8_t product_data[] = {'E', 'S', 'P', '3', '2', 'K', 'I', 'T'};
    hap_acc_add_product_data(accesory, product_data, sizeof(product_data));
    int transport = hap_acc_add_wifi_transport_service(accesory, 0);
    Serial.println("Transport: " + transport);
    return HAP_SUCCESS;
}

int HAPAccessory::AddService(HAPServiceHandle* service) {
    Serial.println("Adding service");
    return hap_acc_add_serv(accesory, service); 
}

int HAPAccessory::Register(String setupCode) {
    Serial.println("Adding accessory");
    // hap_add_accessory(accesory);
    //  Serial.println("Added accessory");
    hap_set_setup_code(ConvertStringToCharArray(setupCode));
    // Serial.println("STarting accessory");
    // return hap_start();
}

int identify(hap_acc_t *acc) {
    Serial.println("Here");
    return 1;
}

hap_acc_cfg_t HAPAccessory::CreateAccessoryConfig() {
    // hap_acc_cfg_t hapConfig = {
    //     .name = ConvertStringToCharArray(_deviceName),
    //     .model = ConvertStringToCharArray(_model),
    //     .manufacturer = ConvertStringToCharArray(_manufacturer),
    //     .serial_num = ConvertStringToCharArray(_serial_num),
    //     .fw_rev = ConvertStringToCharArray(_fw_rev),
    //     .hw_rev = NULL,
    //     .pv = ConvertStringToCharArray(_pv),
    //     .cid = (hap_cid_t) _cid,
    //     .identify_routine = _identityHandler
    // };

        hap_acc_cfg_t hapConfig = {
        .name = "deviceName",
        .model = "model",
        .manufacturer = "manufacturer",
        .serial_num = "0001",
        .fw_rev = NULL,
        .hw_rev = NULL,
        .pv = "1.01",
        .cid = HAP_CID_SENSOR,
        .identify_routine = identify
    };

    return hapConfig;
}

// Services
HAPTemperatureSensor::HAPTemperatureSensor(String serviceName, float currentTemperature) {
         Serial.println("Creating service accessory");
    service = hap_serv_temperature_sensor_create(currentTemperature);
             Serial.println("Created service accessory");
    hap_serv_add_char(service, hap_char_name_create("FAN"));
             Serial.println("Add character service");
                 hap_serv_set_write_cb(service, fan_write);

}

#endif