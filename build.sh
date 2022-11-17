#!/bin/bash

# ESP_IDF_VERSION="4.1"
PROJECT_PATH=$(pwd);
BUILD_DIRECTORY_NAME="build"
ESP_HOMEKIT_SDK_COMMIT_HASH="fac2032426d3cd29d8b6cc2663d0e7945d1d020d";


function cleanup {
    cd "$PROJECT_PATH";
    printf "\n** Deleting temporary build files **";
    rm -rf "$BUILD_DIRECTORY_NAME";
}

trap cleanup EXIT;

# Setup
printf "** Creating temporary build folder **\n"
mkdir $BUILD_DIRECTORY_NAME;
cd $BUILD_DIRECTORY_NAME || { echo "Couldn't enter the build $BUILD_DIRECTORY_NAME folder."; exit 1; }

# Clone the esp-homekit-sdk git project
printf "\n** Cloning espressif/esp-homekit-sdk.git **\n"
git clone https://github.com/espressif/esp-homekit-sdk.git 1> /dev/null;
cd esp-homekit-sdk || { echo "Couldn't enter the esp-homekit-sdk repo folder; git clone probably failed."; exit 1; }
git checkout $ESP_HOMEKIT_SDK_COMMIT_HASH 1> /dev/null;
git submodule update --init --recursive 1> /dev/null;

# Copy esp-homekit-sdk source files
printf "\n** Copying esp-homekit-sdk source files **\n"
cd "$PROJECT_PATH" || { echo "Couldn't enter the project folder."; exit 1; }
mkdir -p "src/esp-homekit-sdk" || { echo "Couldn't create the esp-homekit-sdk folder in the project's source directory."; exit 1; }
cd "$PROJECT_PATH/$BUILD_DIRECTORY_NAME/esp-homekit-sdk/components/homekit" || { echo "Couldn't enter the esp-homekit-sdk folder."; exit 1; }
# Delete conflicting files
find . -name "jsondump.c" -exec rm {} \;
find . -name "simple.c" -exec rm {} \;
find . -name "*json_generator*" -exec rm {} \;
find . -name "*json_parser*" -exec rm {} \;
find . -name "*test*" -exec rm {} \;

find . -type f -name "*.c" -exec cp {} "$PROJECT_PATH/src/esp-homekit-sdk/" \;
find . -type f -name "*.h" -exec cp {} "$PROJECT_PATH/src/esp-homekit-sdk/" \;

# Add config to required files
printf "\n** Adding config to esp-homekit-sdk source files **\n"
sed -i 's/#include <esp_http_server.h>/#include <esp_http_server.h>\n#include <esp_hap_config.h>/' "$PROJECT_PATH/src/esp-homekit-sdk/hap_platform_httpd.c"
sed -i 's/#include "esp_mfi_i2c.h"/#include "esp_mfi_i2c.h"\n#include <esp_hap_config.h>/' "$PROJECT_PATH/src/esp-homekit-sdk/esp_mfi_i2c.c"
sed -i 's/#include <hap_platform_os.h>/#include <hap_platform_os.h>\n#include <esp_hap_config.h>/' "$PROJECT_PATH/src/esp-homekit-sdk/esp_hap_main.c"
sed -i 's/#include <string.h>/#include <string.h>\n#include <esp_hap_config.h>/' "$PROJECT_PATH/src/esp-homekit-sdk/hap_platform_keystore.c"