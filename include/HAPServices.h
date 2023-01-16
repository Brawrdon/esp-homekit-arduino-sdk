#ifndef HAPServices_h
#define HAPServices_h

#include "hap_apple_servs.h"

typedef hap_serv_t* HAPServiceHandle;
typedef hap_char_t* HAPCharacteristicHandler;
typedef hap_val_t HAPWriteData;
typedef void (*HAPWriteCallback)(HAPWriteData data);

struct HAPCallbackToService {
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
        HAPTemperatureSensor(String serviceName, double initialTemperature, HAPWriteCallback temperatureWriteCallback);
};

class HAPLightbulb: public HAPService {
    public:
        HAPLightbulb(String serviceName, bool initialState, HAPWriteCallback onOffStateWriteCallback);
        void AddBrightness(int initalValue, HAPWriteCallback brightWriteCallback);
};

#endif