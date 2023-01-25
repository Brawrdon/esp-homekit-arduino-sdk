#ifndef HAPServices_h
#define HAPServices_h

#include "hap_apple_servs.h"

typedef hap_serv_t* HAPServiceHandle;
typedef hap_char_t* HAPCharacteristicHandler;
typedef hap_val_t HAPWriteData;
typedef void (*HAPWriteCallback)(HAPWriteData data);

struct HAPCallbackToServiceMap {
    HAPWriteCallback WriteCallback;
    HAPCharacteristicHandler CharactersticHandler;
    HAPServiceHandle ServiceHandler;
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
        void AddBrightness(int initalValue, HAPWriteCallback brightWriteCallback);
};

class HAPLightSensor: public HAPService {
    public:
        HAPLightSensor(String serviceName, float initialLightLevel);
};

class HAPHumiditySensor: public HAPService {
    public:
        HAPHumiditySensor(String serviceName, float initialHumidity);
};
#endif