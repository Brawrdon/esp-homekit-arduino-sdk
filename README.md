# ESP32 HomeKit Accessory Protocol SDK for Arduino

This library provides the official [ESP-IDF HomeKit SDK](https://github.com/espressif/esp-homekit-sdk) for ESP32 devices running the Arduino framework.

**Note:** This wrapper uses a version of the SDK which can't be used in commercial products due to it not being MFi certified, feel free to use it in your hobby projects though!

## Installation

If you're using [PlatformIO](https://docs.platformio.org/en/latest/librarymanager/quickstart.html), update your `platformio.ini` file's dependancies or use the `pio` command in the directory containing your `platformio.ini` file:

```bash
pio lib install 'ESP32 HomeKit SDK for Arduino'
```

You can use the Arduino IDE to install the library by downloading the prebuilt ZIP file in the Releases section.

## Usage

An example of implementing a basic fan accessory can be found in the examples folder. It's based off of the fan example from the origin ESP-IDF SDK.

As the Arduino style API wrappers haven't been implemented yet, if you have any questions or issues it's best that you visit the ESP-IDF HomeKit SDK repository for additional help.

## To Do

- [ ] Add Arduino API wrappers to make it easier to use.

## License
[MIT](https://choosealicense.com/licenses/mit/)