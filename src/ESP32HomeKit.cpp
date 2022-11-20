#ifndef ESP32HomeKit_h
#define ESP32HomeKit_h

#include <Arduino.h>
#include <ESP32HomeKit.h>

char* ConvertStringToCharArray(String arduinoString) {
    int str_len = arduinoString.length() + 1; 
    char* char_array = (char*) malloc(str_len);
    arduinoString.toCharArray(char_array, str_len);
    return char_array;
}

int Indentification(HAPAccessoryHandle* accessoryHandle) {
	return 1;
}

// HAP Core
HAPCore::HAPCore(String setupCode) {
	hap_init(HAP_TRANSPORT_WIFI);
	hap_set_setup_code("111-22-333");
	hap_set_setup_id("ES32");
}

int HAPCore::Start() {
    return hap_start();
}

// Accesories
HAPAccessory::HAPAccessory(String deviceName) {
    _deviceName = deviceName;

	hap_cfg_t hap_cfg;
	hap_get_config(&hap_cfg);
	hap_cfg.unique_param = UNIQUE_NAME;
	hap_set_config(&hap_cfg);

	uint8_t product_data[] = {'E', 'S', 'P', '3', '2', 'K', 'I', 'T'};
	hap_acc_add_product_data(Accessory, product_data, sizeof(product_data));
}

int HAPAccessory::AddService(HAPServiceHandle* service) {
    Serial.println("Adding service");
    return hap_acc_add_serv(Accessory, service);
}

int HAPAccessory::Register() {
	 hap_add_accessory(Accessory);
     return HAP_SUCCESS;
}

int HAPAccessory::Setup(String model, String manufacturer, String serial_num, String fw_rev, String pv, HAPAccessoryType cid) {
    _model = model;
    _manufacturer = manufacturer;
    _serial_num = serial_num;
    _fw_rev = fw_rev;
    _pv = pv;
    _cid = cid;

	hap_acc_cfg_t cfg = CreateAccessoryConfig();

	Accessory = hap_acc_create(&cfg);
    return HAP_SUCCESS;
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
        .identify_routine = Indentification
    };

    return hapConfig;
}

// Services
HAPTemperatureSensor::HAPTemperatureSensor(String serviceName, double currentTemperature) {
    Service = hap_serv_temperature_sensor_create(currentTemperature);
    hap_serv_add_char(Service, hap_char_name_create(ConvertStringToCharArray(serviceName)));
}

#endif