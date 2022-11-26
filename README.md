# ESP32 HomeKit Accessory Protocol SDK for Arduino

This library provides the official [ESP-IDF HomeKit SDK (#fac2032)](https://github.com/espressif/esp-homekit-sdk/tree/fac2032426d3cd29d8b6cc2663d0e7945d1d020d) for ESP32 devices running the Arduino framework.

Version 2.0.0 is in active development and will be available soon!

**Note:** This wrapper uses a version of the SDK which can't be used in commercial products due to it not being MFi certified, feel free to use it in your hobby projects though!

## Installation

If you're using [PlatformIO](https://docs.platformio.org/en/latest/librarymanager/quickstart.html), update your `platformio.ini` file's dependancies or use the `pio` command in the directory containing your `platformio.ini` file:

```bash
pio lib install 'ESP32 HomeKit SDK for Arduino'
```
The packages uploaded to PlatformIO's repository and used by the Arduino IDE will contain the complete library, as the build script will be run before packaging. You can use the Arduino IDE to install the library by downloading the prebuilt ZIP file in the Releases section. 

## Usage
At the moment, only a temperature sensor with a default value can be created. I'm in the process of adding the ability to read / write to a sensor, as well as adding all other available sensors.

```cpp
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
	HAPCore hapCore("111-22-333", "ES32");
	hapCore.Start();
}

void loop()
{
	/* Main loop code */
}
```

## Building From Source

If you want to make edits to the library, run the `build.sh` script. It will clone the ESP-IDF HomeKit SDK repository, copy the required source files and modify them to work with Arduino's build chain. The modified files aren't commited as I don't think I need to host copies of the SDK's source in this repository, or in a fork, since the changes for the port are minimal.

## To Do

- [x] Add Arduino API wrappers to make it easier to use.
- [] Add other sensors.
- [] Better documentation.

## License
[MIT](https://choosealicense.com/licenses/mit/)
