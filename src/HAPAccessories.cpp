#include <HAPAccessories.h>
#include <HAPHelpers.h>

int Indentification(HAPAccessoryHandle* accessoryHandle) {
	return 1;
}

HAPAccessory::HAPAccessory(String deviceName, String model, String manufacturer, String serial_num, String fw_rev , String pv, HAPAccessoryType cid) {
    _deviceName = deviceName;
    _model = model;
    _manufacturer = manufacturer;
    _serial_num = serial_num;
    _fw_rev = fw_rev;
    _pv = pv;
    
	hap_cfg_t hap_cfg;
	hap_get_config(&hap_cfg);
	hap_cfg.unique_param = UNIQUE_NAME;
	hap_set_config(&hap_cfg);

	uint8_t product_data[] = {'E', 'S', 'P', '3', '2', 'K', 'I', 'T'};
	hap_acc_add_product_data(Accessory, product_data, sizeof(product_data));

    hap_acc_cfg_t hapConfig = CreateAccessoryConfig(cid);
    Accessory = hap_acc_create(&hapConfig);
}

hap_acc_cfg_t HAPAccessory::CreateAccessoryConfig(hap_cid_t cid) {
    hap_acc_cfg_t hapConfig = {
        .name = ConvertStringToCharArray(_deviceName),
        .model = ConvertStringToCharArray(_model),
        .manufacturer = ConvertStringToCharArray(_manufacturer),
        .serial_num = ConvertStringToCharArray(_serial_num),
        .fw_rev = ConvertStringToCharArray(_fw_rev),
        .hw_rev = NULL,
        .pv = ConvertStringToCharArray(_pv),
        .identify_routine = Indentification
    };

    return hapConfig;
}


int HAPAccessory::AddService(HAPService service) {
    Serial.println("(ESP_HOMEKIT_ARDUINO_SDK) -> " + _deviceName + ": Adding service " + service.Name);
    return hap_acc_add_serv(Accessory, service.Handler);
}

int HAPAccessory::Register() {
    Serial.println("(ESP_HOMEKIT_ARDUINO_SDK) -> " + _deviceName + ": Registering accessory");
	 hap_add_accessory(Accessory);
     return HAP_SUCCESS;
}


HAPSensorAccessory::HAPSensorAccessory(String deviceName, String model, String manufacturer, String serial_num, String fw_rev, String pv) : HAPAccessory(deviceName, model, manufacturer, serial_num, fw_rev, pv, HAP_CID_SENSOR) {};

hap_cid_t HAPSensorAccessory::GetCid() {
    return HAP_CID_SENSOR;
}

