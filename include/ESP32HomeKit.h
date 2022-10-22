typedef hap_acc_t AccessoryHandle;
typedef hap_acc_cfg_t AccessoryConfig;
typedef enum {
    HAP_CID_NONE = 0,
    HAP_CID_OTHER,
    HAP_CID_BRIDGE,
    HAP_CID_FAN,
    HAP_CID_GARAGE_DOOR_OPENER,
    HAP_CID_LIGHTING,
    HAP_CID_LOCK,
    HAP_CID_OUTLET,
    HAP_CID_SWITCH,
    HAP_CID_THERMOSTAT,
    HAP_CID_SENSOR,
    HAP_CID_SECURITY_SYSTEM,
    HAP_CID_DOOR,
    HAP_CID_WINDOW,
    HAP_CID_WINDOW_COVERING,
    HAP_CID_PROGRAMMABLE_SWITCH,
    HAP_CID_RESERVED,
    HAP_CID_IP_CAMERA,
    HAP_CID_VIDEO_DOORBELL,
    HAP_CID_AIR_PURIFIER,
    HAP_CID_HEATER,
    HAP_CID_AIR_CONDITIONER,
    HAP_CID_HUMIDIFIER,
    HAP_CID_DEHUMIDIFIER,
    HAP_CID_MAX,
} AccessoryType;


typedef int (*IdentityHandler) (AccessoryHandle* accessoryHandle);


class HAPAccessory {
   public:
    HAPAccessory (String deviceName);
    AccessoryHandle* _accessory;

    int InitialiseHAP(String model, String manufacturer, String serial_num, String fw_rev, String pv, IdentityHandler identityHandler, AccessoryType cid);
   private:
    String _deviceName;
    String _model;
    String _manufacturer;
    String _serial_num;
    String _fw_rev;
    String _pv;
    IdentityHandler _identityHandler;
    AccessoryType _cid;

    hap_acc_t* _accessory;
    hap_serv_t* service;

    hap_acc_cfg_t CreateAccessoryConfig();
};
