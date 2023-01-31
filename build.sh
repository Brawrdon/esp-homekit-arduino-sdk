#!/bin/bash

# ESP_IDF_VERSION="4.1"
PROJECT_PATH=$(pwd);
BUILD_DIRECTORY_NAME="build"
ESP_HOMEKIT_SDK_COMMIT_HASH="fac2032426d3cd29d8b6cc2663d0e7945d1d020d";


function cleanup {
    cd "$PROJECT_PATH";
    if [[ -d "build" ]]
    then
        printf "\n** Deleting temporary build files **"\n;
        rm -rf "$BUILD_DIRECTORY_NAME";
    fi

}

trap cleanup EXIT;

printf "\n** ESP32 HomeKit Arduino SDK Logs **\n"

# Skip if esp-homekit-sdk source files already exist
if [[ -d "src/esp-homekit-sdk" ]]
then
    printf "** esp-homekit-sdk source files exist, skipping clone. **\n";
    exit 0;
fi

# Setup
printf "** Creating temporary build folder **\n"
mkdir $BUILD_DIRECTORY_NAME;
cd $BUILD_DIRECTORY_NAME || { echo "Couldn't enter the build $BUILD_DIRECTORY_NAME folder."; exit 1; }

# Clone the esp-homekit-sdk git project
printf "\n** Cloning espressif/esp-homekit-sdk.git **\n"
git clone --quiet https://github.com/espressif/esp-homekit-sdk.git &> /dev/null;
cd esp-homekit-sdk || { echo "Couldn't enter the esp-homekit-sdk repo folder; git clone probably failed."; exit 1; }
git checkout --quiet $ESP_HOMEKIT_SDK_COMMIT_HASH &> /dev/null;
git submodule update --quiet --init --recursive &> /dev/null;

# Copy esp-homekit-sdk source files
printf "\n** Copying esp-homekit-sdk source files **\n"
cd "$PROJECT_PATH" || { echo "Couldn't enter the project folder."; exit 1; }
mkdir -p "src/esp-homekit-sdk" || { echo "Couldn't create the esp-homekit-sdk folder in the project's source directory."; exit 1; }
cd "$PROJECT_PATH/$BUILD_DIRECTORY_NAME/esp-homekit-sdk/components/homekit" || { echo "Couldn't enter the esp-homekit-sdk components folder."; exit 1; }
# Delete conflicting files
find . -name "jsondump.c" -exec rm {} \;
find . -name "simple.c" -exec rm {} \;
find . -name "*json_generator.*" -exec rm {} \;
find . -name "*json_parser.*" -exec rm {} \;
find . -name "*test*.*" -exec rm {} \;

find . -type f -name "*.c" -exec cp {} "$PROJECT_PATH/src/esp-homekit-sdk/" \;
find . -type f -name "*.h" -exec cp {} "$PROJECT_PATH/src/esp-homekit-sdk/" \;

printf "\n** Replacing < > with \" \" in include headers **\n"
cd "$PROJECT_PATH/src/esp-homekit-sdk/" || { echo "Couldn't enter the esp-homekit-sdk folder."; exit 1; }
find . -type f -name "*.c" -exec sed -i -r 's/<(.*?hap.*?)>/"\1"/' {} \;
find . -type f -name "*.c" -exec sed -i -r 's/<(.*?mfi.*?)>/"\1"/' {} \;
find . -type f -name "*.h" -exec sed -i -r 's/<(.*?hap.*?)>/"\1"/' {} \;
find . -type f -name "*.h" -exec sed -i -r 's/<(.*?mfi.*?)>/"\1"/' {} \;

# Add config to required files
printf "\n** Adding config to esp-homekit-sdk source files **\n"
sed -i 's/#include <esp_http_server.h>/#include <esp_http_server.h>\n#include "esp_hap_config.h"/' "$PROJECT_PATH/src/esp-homekit-sdk/hap_platform_httpd.c"
sed -i 's/#include "esp_mfi_i2c.h"/#include "esp_mfi_i2c.h"\n#include "esp_hap_config.h"/' "$PROJECT_PATH/src/esp-homekit-sdk/esp_mfi_i2c.c"
sed -i 's/#include "hap_platform_os.h"/#include "hap_platform_os.h"\n#include "esp_hap_config.h"/' "$PROJECT_PATH/src/esp-homekit-sdk/esp_hap_main.c"
sed -i 's/#include <string.h>/#include <string.h>\n#include "esp_hap_config.h"/' "$PROJECT_PATH/src/esp-homekit-sdk/hap_platform_keystore.c"