#include "hap.h"
#include "hap_apple_servs.h"
#include "hap_apple_chars.h"

#define HAP_SERVICE_ACCESSORY_INFORMATION         "3E"
#define HAP_SERVICE_PROTOCOL_INFORMATION          "A2"
#define HAP_SERVICE_FAN                           "40"
#define HAP_SERVICE_GARAGE_DOOR_OPENER            "41"
#define HAP_SERVICE_LIGHTBULB                     "43"
#define HAP_SERVICE_LOCK_MANAGEMENT               "44"
#define HAP_SERVICE_LOCK_MECHANISM                "45"
#define HAP_SERVICE_SWITCH                        "49"
#define HAP_SERVICE_OUTLET                        "47"
#define HAP_SERVICE_THERMOSTAT                    "4A"
#define HAP_SERVICE_AIR_QUALITY_SENSOR            "8D"
#define HAP_SERVICE_SECURITY_SYSTEM               "7E"
#define HAP_SERVICE_CARBON_MONOXIDE_SENSOR        "7F"
#define HAP_SERVICE_CONTACT_SENSOR                "80"
#define HAP_SERVICE_DOOR                          "81"
#define HAP_SERVICE_HUMIDITY_SENSOR               "82"
#define HAP_SERVICE_LEAK_SENSOR                   "83"
#define HAP_SERVICE_LIGHT_SENSOR                  "84"
#define HAP_SERVICE_MOTION_SENSOR                 "85"
#define HAP_SERVICE_OCCUPANCY_SENSOR              "86"
#define HAP_SERVICE_SMOKE_SENSOR                  "87"
#define HAP_SERVICE_STATLESS_PROGRAMMABLE_SWITCH  "89"
#define HAP_SERVICE_TEMPERATURE_SENSOR            "8A"
#define HAP_SERVICE_WINDOW                        "8B"
#define HAP_SERVICE_WINDOW_COVERING               "8C"
#define HAP_SERVICE_BATTERY_SERVICE               "96"
#define HAP_SERVICE_CARBON_DIOXIDE_SENSOR         "97"
#define HAP_SERVICE_FAN_V2                        "B7"
#define HAP_SERVICE_SLAT                          "B9"
#define HAP_SERVICE_FILTER_MAINTENANCE            "BA"
#define HAP_SERVICE_AIR_PURIFIER                  "BB"
#define HAP_SERVICE_HEATER_COOLER                 "BC"
#define HAP_SERVICE_HUMIDIFIER_DEHUMIDIFIER       "BD"
#define HAP_SERVICE_SERVICE_LABEL                 "CC"
#define HAP_SERVICE_IRRIGATION_SYSTEM             "CF"
#define HAP_SERVICE_VALVE                         "D0"
#define HAP_SERVICE_FAUCET                        "D7"

typedef hap_acc_t HAPAccessoryHandle;
typedef hap_serv_t HAPServiceHandle;
typedef hap_acc_cfg_t HAPAccessoryConfig;
typedef enum {
    HAP_ACCESSORY_NONE = 0,
    HAP_ACCESSORY_OTHER,
    HAP_ACCESSORY_BRIDGE,
    HAP_ACCESSORY_FAN,
    HAP_ACCESSORY_GARAGE_DOOR_OPENER,
    HAP_ACCESSORY_LIGHTING,
    HAP_ACCESSORY_LOCK,
    HAP_ACCESSORY_OUTLET,
    HAP_ACCESSORY_SWITCH,
    HAP_ACCESSORY_THERMOSTAT,
    HAP_ACCESSORY_SENSOR,
    HAP_ACCESSORY_SECURITY_SYSTEM,
    HAP_ACCESSORY_DOOR,
    HAP_ACCESSORY_WINDOW,
    HAP_ACCESSORY_WINDOW_COVERING,
    HAP_ACCESSORY_PROGRAMMABLE_SWITCH,
    HAP_ACCESSORY_RESERVED,
    HAP_ACCESSORY_IP_CAMERA,
    HAP_ACCESSORY_VIDEO_DOORBELL,
    HAP_ACCESSORY_AIR_PURIFIER,
    HAP_ACCESSORY_HEATER,
    HAP_ACCESSORY_AIR_CONDITIONER,
    HAP_ACCESSORY_HUMIDIFIER,
    HAP_ACCESSORY_DEHUMIDIFIER,
    HAP_ACCESSORY_MAX,
} HAPAccessoryType;

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

class HAPAccessory {
    public:
        HAPAccessory(String deviceName);
        HAPAccessoryHandle* Accessory;

        int Setup(String model, String manufacturer, String serial_num, String fw_rev, String pv, HAPAccessoryType cid);
        int AddService(HAPServiceHandle* service);
        int Register();
    private:
        String _deviceName;
        String _model;
        String _manufacturer;
        String _serial_num;
        String _fw_rev;
        String _pv;
        HAPAccessoryType _cid;

        hap_acc_cfg_t CreateAccessoryConfig();
};

// Services
class HAPTemperatureSensor {
    public:
        HAPTemperatureSensor (String serviceName, double currentTemperature);
        HAPServiceHandle* Service; 
};
