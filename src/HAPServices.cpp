#include <Arduino.h>
#include <HAPServices.h>
#include <HAPHelpers.h>

#include <../src/esp-homekit-sdk/hap_apple_chars.h>
#include <../src/esp-homekit-sdk/esp_hap_serv.h>

// ~ 15 charactersitics send write data from the Home app; rounded to 20 to be safe. 
// Arbitrarily assuming each characterstic can be used by 4 services leave us with 80 potential callbacks.
static HAPCallbackToServiceMap callbackToServiceMaps[80];
static int callbackToServiceMapsCount = 0;

HAPService::HAPService(String serviceName) {
    Name = serviceName;
}

static int OnCharacteristicWrite(hap_write_data_t write_data[], int count,void *serv_priv, void *write_priv)
{
    int ret = HAP_SUCCESS;
    hap_write_data_t writeData;
    for (int i = 0; i < count; i++) {
        writeData = write_data[i];
        __hap_char_t* hapCharacterstic = (__hap_char_t*) writeData.hc;

        for (int j = 0; i < callbackToServiceMapsCount; j++)
        {
            HAPCallbackToServiceMap callbackToServiceMap = callbackToServiceMaps[j];
            if (hapCharacterstic->parent == callbackToServiceMap.ServiceHandler && callbackToServiceMap.CharactersticHandler == writeData.hc)
            {
                callbackToServiceMap.WriteCallback(writeData.val);
                *(writeData.status) = HAP_STATUS_SUCCESS;
                break;
            } 
        }
        if (*(writeData.status) == HAP_STATUS_SUCCESS) {
            hap_char_update_val(writeData.hc, &(writeData.val));
        } else {
            ret = HAP_FAIL;
            String typeUuid = String(hapCharacterstic->type_uuid);
            HAPLog("Couldn't find any OnCharacteristicWrite callbacks for type with UUID: " + typeUuid);
        }
    }
    return ret;
}

// HAPTemperatureSensor
HAPTemperatureSensor::HAPTemperatureSensor(String serviceName, float initialTemperature): HAPService(serviceName){
    Handler = hap_serv_temperature_sensor_create(initialTemperature);
    hap_serv_add_char(Handler, hap_char_name_create(ConvertStringToCharArray(Name)));
}

// HAPHumiditySensor
HAPHumiditySensor::HAPHumiditySensor(String serviceName, float initialHumidity): HAPService(serviceName){
    Handler = hap_serv_humidity_sensor_create(initialHumidity);
    hap_serv_add_char(Handler, hap_char_name_create(ConvertStringToCharArray(Name)));
}

// HAPLightbulb
HAPLightbulb::HAPLightbulb(String serviceName, bool initialState, HAPWriteCallback onOffStateWriteCallback): HAPService(serviceName){
    Handler = hap_serv_lightbulb_create(initialState);
    hap_serv_add_char(Handler, hap_char_name_create(ConvertStringToCharArray(Name)));
    hap_serv_set_write_cb(Handler, OnCharacteristicWrite);   

    HAPCharacteristicHandler onOffStateCharacteristicHandler = hap_serv_get_char_by_uuid(Handler, HAP_CHAR_UUID_ON);
    callbackToServiceMaps[callbackToServiceMapsCount] = {
        .WriteCallback = onOffStateWriteCallback,
        .CharactersticHandler = onOffStateCharacteristicHandler,
        .ServiceHandler = Handler
    };

    callbackToServiceMapsCount++;

}

void HAPLightbulb::AddBrightness(int initialValue, HAPWriteCallback brightnessWriteCallback) {
        hap_serv_add_char(Handler, hap_char_brightness_create(initialValue));
        HAPCharacteristicHandler brightnessCharacteristicHandler = hap_serv_get_char_by_uuid(Handler, HAP_CHAR_UUID_BRIGHTNESS);
        callbackToServiceMaps[callbackToServiceMapsCount] = {
        .WriteCallback = brightnessWriteCallback,
        .CharactersticHandler = brightnessCharacteristicHandler,
        .ServiceHandler = Handler
    };

    callbackToServiceMapsCount++;
}

/* HAPLightSensor */
HAPLightSensor::HAPLightSensor(String serviceName, float initialLightLevel): HAPService(serviceName) {
    Handler = hap_serv_light_sensor_create(initialLightLevel);
    hap_serv_add_char(Handler, hap_char_name_create(ConvertStringToCharArray(Name)));
}

// Custom Services
HAPCustomService::HAPCustomService(String serviceName, String uuid): HAPService(serviceName) {
    Handler = hap_serv_create(ConvertStringToCharArray(uuid));
    if (!Handler) {
        HAPLog("Failed to create the custom service: " + serviceName);
        return;
    }
    hap_serv_add_char(Handler, hap_char_name_create(ConvertStringToCharArray(Name)));
    hap_serv_set_write_cb(Handler, OnCharacteristicWrite);   
}

HAPCharacteristicHandler HAPCustomService::AddFloatCharacteristic(String uuid, float initialValue, bool writtable) {
    uint16_t permission = HAP_CHAR_PERM_PR | HAP_CHAR_PERM_EV;
    if (writtable) {
        permission = HAP_CHAR_PERM_PR | HAP_CHAR_PERM_PW | HAP_CHAR_PERM_EV;
    }
        
    hap_char_t* characteristicHandler = hap_char_float_create(ConvertStringToCharArray(uuid), permission, initialValue);
    hap_serv_add_char(Handler, characteristicHandler);
    return characteristicHandler;
}

void HAPCustomService::AddWriteCallbackForCharacteristic(HAPCharacteristicHandler characteristicHandler, HAPWriteCallback writeCallback) {
    callbackToServiceMaps[callbackToServiceMapsCount] = {
        .WriteCallback = writeCallback,
        .CharactersticHandler = characteristicHandler,
        .ServiceHandler = Handler        
    };
    callbackToServiceMapsCount++;
}
