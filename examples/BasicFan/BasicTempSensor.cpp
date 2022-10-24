#include <Arduino.h>
#include <WiFi.h>
#include <ESP32HomeKit.h>

const char *ssid = "ssid";
const char *password = "password";


int Indentification(HAPAccessoryHandle* accessoryHandle) {
	Serial.println("Establishing connection to WiFi..");
	return 1;
}

void setup()
{
	Serial.begin(115200);

	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(1000);
		Serial.println("Establishing connection to WiFi..");
	}

	Serial.println("Connected to network.");

	HAPAccessory hapAccessory("ESP32 Fan");
	hapAccessory.Initialise("EspFan01", "Espressif", "001122334455", "0.0.1", "1.0.0", Indentification, HAP_ACCESSORY_SENSOR);

	HAPTemperatureSensor hapTemperatureSensor("Temperature", 0.0);
	hapAccessory.AddService(hapTemperatureSensor.Service);
	
	hapAccessory.Register("111-22-333");
}

void loop()
{
	/* Main loop code */
}
