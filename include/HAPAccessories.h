#ifndef HAPAccessories_h
#define HAPAccessories_h

#include <../src/esp-homekit-sdk/hap.h>
#include <Arduino.h>
#include <HAPServices.h>

typedef hap_acc_t HAPAccessoryHandle;
typedef hap_cid_t HAPAccessoryType;

class HAPAccessory {
    public:
        HAPAccessory(String deviceName, String model, String manufacturer, String serial_num , String fw_rev, String pv, HAPAccessoryType cid);
        HAPAccessoryHandle* Accessory;

        int AddService(HAPService service);
        int Register();
    protected:
        String _deviceName;
        String _model;
        String _manufacturer;
        String _serial_num;
        String _fw_rev;
        String _pv;

        hap_acc_cfg_t CreateAccessoryConfig(hap_cid_t cid);
        virtual hap_cid_t GetCid() = 0;
        
};

class HAPSensorAccessory: public HAPAccessory {
    public:
        HAPSensorAccessory(String deviceName, String model = "EspFan01", String manufacturer = "Espressif", String serial_num = "001122334455", String fw_rev = "0.0.1", String pv = "1.0.0");
    private:
        virtual hap_cid_t GetCid();
};

#endif