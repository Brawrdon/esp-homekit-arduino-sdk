#include <Arduino.h>
#include <HAPServices.h>
#include <HAPHelpers.h>

#include <hap_apple_chars.h>
#include <esp_hap_serv.h>

// ~ 15 charactersitics send write data from the Home app; rounded to 20 to be safe. 
// Arbitrarily assuming each characterstic can be used by 4 services leave us with 80 potential callbacks.
static HAPCallbackToService hapCallbackToServiceList[80];
static int hapCallbackToServiceListCount = 0;

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

        for (int j = 0; i < hapCallbackToServiceListCount; j++)
        {
            HAPCallbackToService callbackToService = hapCallbackToServiceList[j];
            __hap_char_t* hapCharacterstic = (__hap_char_t*) writeData.hc;
            if (hapCharacterstic->parent == callbackToService.ServiceHandler && callbackToService.CharactersticHandler == writeData.hc)
            {
                callbackToService.WriteCallback(writeData.val);
                *(writeData.status) = HAP_STATUS_SUCCESS;
                break;
            }
            
        }
        

        // ToDo: finish this else statement
        // else { 
        //     *(writeData.status) = HAP_STATUS_RES_ABSENT;
        // }

        if (*(writeData.status) == HAP_STATUS_SUCCESS) {
            hap_char_update_val(writeData.hc, &(writeData.val));
        } else {
            ret = HAP_FAIL;
        }
    }
    return ret;
}

HAPTemperatureSensor::HAPTemperatureSensor(String serviceName, double initialTemperature, HAPWriteCallback temperatureWriteCallback): HAPService(serviceName){
    Handler = hap_serv_temperature_sensor_create(initialTemperature);
    hap_serv_set_write_cb(Handler, OnCharacteristicWrite);   
}

HAPLightbulb::HAPLightbulb(String serviceName, bool initialState, HAPWriteCallback onOffStateWriteCallback): HAPService(serviceName){
    Handler = hap_serv_lightbulb_create(initialState);
    HAPCharacteristicHandler onOffStateCharacteristicHandler = hap_serv_get_char_by_uuid(Handler, HAP_CHAR_UUID_ON);
    hapCallbackToServiceList[hapCallbackToServiceListCount] = {
        .WriteCallback = onOffStateWriteCallback,
        .CharactersticHandler = onOffStateCharacteristicHandler,
        .ServiceHandler = Handler
    };

    hapCallbackToServiceListCount++;

    hap_serv_set_write_cb(Handler, OnCharacteristicWrite);   
}

void HAPLightbulb::AddBrightness(int initialValue, HAPWriteCallback brightnessWriteCallback) {
        hap_serv_add_char(Handler, hap_char_brightness_create(initialValue));
        HAPCharacteristicHandler brightnessCharacteristicHandler = hap_serv_get_char_by_uuid(Handler, HAP_CHAR_UUID_BRIGHTNESS);
        hapCallbackToServiceList[hapCallbackToServiceListCount] = {
        .WriteCallback = brightnessWriteCallback,
        .CharactersticHandler = brightnessCharacteristicHandler,
        .ServiceHandler = Handler
    };

    hapCallbackToServiceListCount++;
}
