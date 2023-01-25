#include <Arduino.h>
#include <HAPServices.h>
#include <HAPHelpers.h>

#include <hap_apple_chars.h>
#include <esp_hap_serv.h>

// ~ 15 charactersitics send write data from the Home app; rounded to 20 to be safe. 
// Arbitrarily assuming each characterstic can be used by 4 services leave us with 80 potential callbacks.
static HAPServiceAndCallback servicesAndCallbacksList[80];
static int serviceAndCallbackListCount = 0;

HAPService::HAPService(String serviceName) {
    Name = serviceName;
    hap_serv_add_char(Handler, hap_char_name_create(ConvertStringToCharArray(Name)));
}

static int OnCharacteristicWrite(hap_write_data_t write_data[], int count,void *serv_priv, void *write_priv)
{
    int ret = HAP_SUCCESS;
    hap_write_data_t writeData;
    for (int i = 0; i < count; i++) {
        writeData = write_data[i];
        __hap_char_t* hapCharacterstic = (__hap_char_t*) writeData.hc;
        for (int j = 0; i < serviceAndCallbackListCount; j++)
        {
            HAPServiceAndCallback serviceAndCallback = servicesAndCallbacksList[j];
            if (hapCharacterstic->parent == serviceAndCallback.ServiceHandler && serviceAndCallback.CharactersticHandler == writeData.hc)
            {
                serviceAndCallback.WriteCallback(writeData.val);
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

/* HAPTemperatureSensor */
HAPTemperatureSensor::HAPTemperatureSensor(String serviceName, double initialTemperature): HAPService(serviceName){
    Handler = hap_serv_temperature_sensor_create(initialTemperature);
}

/* HAPLightbulb */
HAPLightbulb::HAPLightbulb(String serviceName, bool initialState, HAPWriteCallback onOffStateWriteCallback): HAPService(serviceName){
    Handler = hap_serv_lightbulb_create(initialState);
    HAPCharacteristicHandler onOffStateCharacteristicHandler = hap_serv_get_char_by_uuid(Handler, HAP_CHAR_UUID_ON);
    servicesAndCallbacksList[serviceAndCallbackListCount] = {        
        .ServiceHandler = Handler,
        .WriteCallback = onOffStateWriteCallback,
        .CharactersticHandler = onOffStateCharacteristicHandler
    };

    serviceAndCallbackListCount++;
}

void HAPLightbulb::AddBrightnessCharacteristic(int initialValue, HAPWriteCallback brightnessWriteCallback) {
        hap_serv_add_char(Handler, hap_char_brightness_create(initialValue));
        HAPCharacteristicHandler brightnessCharacteristicHandler = hap_serv_get_char_by_uuid(Handler, HAP_CHAR_UUID_BRIGHTNESS);
        servicesAndCallbacksList[serviceAndCallbackListCount] = {
        .ServiceHandler = Handler,
        .WriteCallback = brightnessWriteCallback,
        .CharactersticHandler = brightnessCharacteristicHandler
    };

    serviceAndCallbackListCount++;
}

/* HAPLightSensor */
HAPLightSensor::HAPLightSensor(String serviceName, float initialLightLevel): HAPService(serviceName) {
    Handler = hap_serv_light_sensor_create(initialLightLevel);
}

/* HAPHumiditySensor */
HAPHumiditySensor::HAPHumiditySensor(String serviceName, float initialHumidity): HAPService(serviceName) {
    Handler = hap_serv_humidity_sensor_create(initialHumidity);
}

/* HAPMoistureSensor */
HAPMoistureSensor::HAPMoistureSensor(String serviceName, float initialMoistureLevel): HAPService(serviceName) {
    Handler = hap_serv_create("66b784b9-4752-4648-a036-89e468369d31");
    if (!Handler) {
        HAPLog("Failed to create a moisture sensor service");
        return;
    }

    hap_serv_add_char(Handler, hap_char_water_level_create(initialMoistureLevel));

}
