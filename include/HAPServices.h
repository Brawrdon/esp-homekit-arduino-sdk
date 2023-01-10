#ifndef HAPServices_h
#define HAPServices_h

#include "hap_apple_servs.h"

typedef hap_serv_t HAPServiceHandle;

class HAPService {
    public:
        HAPServiceHandle* Handler;
        String Name;
};

class HAPTemperatureSensor: public HAPService {
    public:
        HAPTemperatureSensor (String serviceName, double currentTemperature);
};

#endif