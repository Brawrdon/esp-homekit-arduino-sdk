

#include <Arduino.h>
#include <HAPServices.h>
#include <HAPHelpers.h>

#include <hap_apple_chars.h>


HAPTemperatureSensor::HAPTemperatureSensor(String serviceName, double currentTemperature) {
    Name = serviceName;
    Handler = hap_serv_temperature_sensor_create(currentTemperature);
    hap_serv_add_char(Handler, hap_char_name_create(ConvertStringToCharArray(serviceName)));
}

