class HAPAccessory {
   public:
    HAPAccessory (String deviceName);

    typedef int (*IdentityRoutine) (hap_acc_t *acc);
    typedef hap_cid_t Cid;
    typedef hap_acc_cfg_t AccessoryConfig;

    int InitialiseHAP(String model, String manufacturer, String serial_num, String fw_rev, String pv, IdentityRoutine identityRoutine, Cid cid);
   private:
    String _deviceName;
    String _model;
    String _manufacturer;
    String _serial_num;
    String _fw_rev;
    String _pv;
    IdentityRoutine _identityRoutine;
    Cid _cid;

    hap_acc_t* _accessory;
    hap_serv_t* service;

    hap_acc_cfg_t CreateAccessoryConfig();
};
