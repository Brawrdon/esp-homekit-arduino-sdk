#include <Arduino.h>
#include <WiFi.h>
#include <ESP32HomeKit.h>

const char *ssid = "ssid";
const char *password = "password";

/*
	This example provides a basic connection to HomeKit.
	You can't read/write values at the moment but it's coming I promise.
*/


void setup()
{
	Serial.begin(9600);

	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(1000);
		Serial.println("Establishing connection to WiFi..");
	}

	Serial.println("Connected to network.");

	HAPAccessory hapAccessory("ESP32 Fan");
	hapAccessory.Setup("EspFan01", "Espressif", "001122334455", "0.0.1", "1.0.0", HAP_ACCESSORY_SENSOR);

	HAPTemperatureSensor hapTemperatureSensor("Temperature", 10.0);
	hapAccessory.AddService(hapTemperatureSensor.Service);

	hapAccessory.Register();
	HAPCore hapCore("111-22-333");
	hapCore.Start();
}

void loop()
{
	/* Main loop code */
}
