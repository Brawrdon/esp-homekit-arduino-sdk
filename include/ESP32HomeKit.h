#ifndef ESP32HomeKit_h
#define ESP32HomeKit_h

#include "hap.h"
#include "hap_apple_chars.h"

#include <HAPServices.h>

typedef hap_acc_t HAPAccessoryHandle;
typedef hap_acc_cfg_t HAPAccessoryConfig;

typedef int (*IdentityHandler) (HAPAccessoryHandle* accessoryHandle);

class HAPCore {
    public:
        /** @brief Constructor for the HAP Core.
         *
         * The constructor for the HAP Core. 
         * This object should be created after all Accessories and Sensors have been set up.
         * @param[in]  setupCode  The setup code used when pairing the accessory. It must consist of only numbers and be in the following format xxx-xx-xxx.
         * @param[in]  setupId  Unique ID pairing ID. Must be 4 characters long
         */
        HAPCore(String setupCode, String setupId);
        int Start();
};

#endif
