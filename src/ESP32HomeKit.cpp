#include <Arduino.h>
#include <ESP32HomeKit.h>
#include <HAPHelpers.h>


// HAP Core
HAPCore::HAPCore(String setupCode, String setupId) {
	hap_init(HAP_TRANSPORT_WIFI);
	hap_set_setup_code(ConvertStringToCharArray(setupCode));
	hap_set_setup_id(ConvertStringToCharArray(setupId));
}

int HAPCore::Start() {
    return hap_start();
}

// Accesories



