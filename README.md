# ESP32 HomeKit Accessory Protocol SDK for Arduino

This library provides the official [ESP-IDF HomeKit SDK (#040b0f3)](https://github.com/espressif/esp-homekit-sdk/tree/040b0f301223ebc6995597328e5a5cc9f9739a02) for ESP32 devices running the Arduino framework.

**Note:** This wrapper uses a version of the SDK which can't be used in commercial products due to it not being MFi certified, feel free to use it in your hobby projects though!

## Installation

If you're using [PlatformIO](https://docs.platformio.org/en/latest/librarymanager/quickstart.html), update your `platformio.ini` file's dependancies or use the `pio` command in the directory containing your `platformio.ini` file:

```bash
pio lib install 'ESP32 HomeKit SDK for Arduino'
```
The packages uploaded to PlatformIO's repository and used by the Arduino IDE will contain the complete library, as the build script will be run before packaging. You can use the Arduino IDE to install the library by downloading the prebuilt ZIP file in the Releases section. 

## Usage

An example of implementing a basic fan accessory can be found in the examples folder. It's based off of the fan example from the origin ESP-IDF SDK.

As the Arduino style API wrappers haven't been implemented yet, if you have any questions or issues it's best that you visit the ESP-IDF HomeKit SDK repository for additional help.

## Building From Source

Building outside of PlatformIO requires Python 3. Running the `build.py` script will clone the ESP-IDF HomeKit SDK repository, place the required files in the correct directory structure and modify them to work with Arduino's build chain. The modified files aren't commited as I don't think I need to host copies of the SDK's source in this repository, or in a fork, since the changes for the port are minimal.

## To Do

- [ ] Add Arduino API wrappers to make it easier to use.

## License
[MIT](https://choosealicense.com/licenses/mit/)
