#ifndef HAPServices_h
#define HAPServices_h

#include "hap_apple_servs.h"

typedef hap_serv_t* HAPServiceHandle;
typedef hap_char_t* HAPCharacteristicHandler;
typedef hap_val_t HAPWriteData;
typedef void (*HAPWriteCallback)(HAPWriteData data);

struct HAPServiceAndCallback {
    HAPServiceHandle ServiceHandler;
    HAPWriteCallback WriteCallback;
    HAPCharacteristicHandler CharactersticHandler;
};

class HAPService {
    public:
        HAPService(String serviceName);
        HAPServiceHandle Handler;
        String Name; 
};

class HAPTemperatureSensor: public HAPService {
    public:
        HAPTemperatureSensor(String serviceName, double initialTemperature);
};

class HAPLightbulb: public HAPService {
    public:
        HAPLightbulb(String serviceName, bool initialState, HAPWriteCallback onOffStateWriteCallback);
        void AddBrightnessCharacteristic(int initalValue, HAPWriteCallback brightWriteCallback);
};

class HAPLightSensor: public HAPService {
    public:
        HAPLightSensor(String serviceName, float initialLightLevel);
};

class HAPHumiditySensor: public HAPService {
    public:
        HAPHumiditySensor(String serviceName, float initialHumidity);
};

class HAPMoistureSensor: public HAPService {
    public:
        HAPMoistureSensor(String serviceName, float initialMoistureLevel);
};
#endif