#include <Arduino.h>
#include <WiFi.h>
#include <ESP32HomeKit.h>
#include <HAPAccessories.h>
#include <HAPServices.h>

const char *ssid = "ssid";
const char *password = "password";

/*
	This example provides a basic connection to HomeKit.
*/

void Callback(HAPWriteData data) {
	Serial.println("Switch flipped");
}

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

	HAPSensorAccessory hapSensorAccessory("ESP32 Lightbulb");

	HAPLightbulb HAPLightbulb("Lightbulb", true, Callback);

	hapSensorAccessory.AddService(HAPLightbulb);
	hapSensorAccessory.Register();
	
	HAPCore hapCore("111-22-333", "ES32");
	hapCore.Start();
}

void loop()
{
	/* Main loop code */
}
