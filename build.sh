#!/bin/bash

# ESP_IDF_VERSION="4.1"
PROJECT_PATH=$(pwd);
PRIV_INCLUDE_PATH="$(pwd)/esp-homekit-lib/priv_include";
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
mkdir -p $PRIV_INCLUDE_PATH
mkdir $BUILD_DIRECTORY_NAME;
cd $BUILD_DIRECTORY_NAME || { echo "Couldn't enter the build $BUILD_DIRECTORY_NAME folder."; exit 1; }

# Clone and enter the fan example directory
printf "\n** Cloning espressif/esp-homekit-sdk.git **\n"
git clone https://github.com/espressif/esp-homekit-sdk.git 1> /dev/null;
cd esp-homekit-sdk || { echo "Couldn't enter the esp-homekit-sdk repo folder; git clone probably failed."; exit 1; }
git checkout $ESP_HOMEKIT_SDK_COMMIT_HASH 1> /dev/null;
git submodule update --init --recursive 1> /dev/null;

# Copy esp-homekit-sdk source files
printf "\n** Copying esp-homekit-sdk source files **\n"
cd "$PROJECT_PATH" || { echo "Couldn't enter the project folder."; exit 1; }
cd "src" || { echo "Couldn't enter the src directory."; exit 1; }
mkdir -p "src/esp-homekit-sdk" || { echo "Couldn't create the esp-homekit-sdk folder in the project's source directory."; exit 1; }
cd "$PROJECT_PATH/$BUILD_DIRECTORY_NAME/esp-homekit-sdk/components/homekit" || { echo "Couldn't enter the esp-homekit-sdk folder."; exit 1; }
find . -type f -name "*.c" -exec cp {} "$PROJECT_PATH/src/esp-homekit-sdk" \;
find . -type f -name "*.h" -exec cp {} "$PROJECT_PATH/priv_include" \;

# Add config to required files
printf "\n** Adding config to esp-homekit-sdk source files **\n"
cp "$PROJECT_PATH/esp-homekit-lib/esp_hap_config.h" "$PROJECT_PATH/$BUILD_DIRECTORY_NAME/lib/$LIBRARY_IN_ARDUINO_NAME/include/"
sed -i 's/#include <esp_http_server.h>/#include <esp_http_server.h>\n#include <esp_hap_config.h>/' "$PROJECT_PATH/$BUILD_DIRECTORY_NAME/lib/$LIBRARY_IN_ARDUINO_NAME/src/hap_platform_httpd.c"
sed -i 's/#include <esp_http_server.h>/#include <esp_http_server.h>\n#include <esp_hap_config.h>/' "$PROJECT_PATH/$BUILD_DIRECTORY_NAME/lib/$LIBRARY_IN_ARDUINO_NAME/src/esp_mfi_i2c.c"
sed -i 's/#include <hap_platform_os.h>/#include <hap_platform_os.h>\n#include <esp_hap_config.h>/' "$PROJECT_PATH/$BUILD_DIRECTORY_NAME/lib/$LIBRARY_IN_ARDUINO_NAME/src/esp_hap_main.c"

printf "\n** Building PlatformIO Project **\n"
cp "$PROJECT_PATH/esp-homekit-lib/main.cpp" "$PROJECT_PATH/$BUILD_DIRECTORY_NAME/src/"
cd "$PROJECT_PATH/$BUILD_DIRECTORY_NAME" || { echo "Couldn't enter the build folder."; exit 1; }
pio run --environment esp32doit-devkit-v1


# Copy built esp-homekit-sdk libraries
# printf "\n** Copying built esp-homekit-sdk library files **\n"
# cp "$(find . -name "libesp_hap_apple_profiles.a")" "$PROJECT_PATH/esp-homekit-lib" || { echo "Couldn't copy libesp_hap_apple_profiles.a."; exit 1; };
# cp "$(find . -name "libesp_hap_core.a")" "$PROJECT_PATH/esp-homekit-lib"|| { echo "Couldn't copy libesp_hap_core.a."; exit 1; };
# cp "$(find . -name "libesp_hap_extras.a")" "$PROJECT_PATH/esp-homekit-lib"|| { echo "Couldn't copy libesp_hap_extras.a."; exit 1; };
# cp "$(find . -name "libesp_hap_platform.a")" "$PROJECT_PATH/esp-homekit-lib"|| { echo "Couldn't copy libesp_hap_platform.a."; exit 1; };
# cp "$(find . -name "libhkdf-sha.a")" "$PROJECT_PATH/esp-homekit-lib"|| { echo "Couldn't copy libhkdf-sha.a."; exit 1; };
# cp "$(find . -name "libjson_generator.a")" "$PROJECT_PATH/esp-homekit-lib"|| { echo "Couldn't copy libjson_generator.a."; exit 1; };
# cp "$(find . -name "libjson_parser.a")" "$PROJECT_PATH/esp-homekit-lib"|| { echo "Couldn't copy libjson_parser.a."; exit 1; };
# cp "$(find . -name "libmu_srp.a")" "$PROJECT_PATH/esp-homekit-lib"|| { echo "Couldn't copy libmu_srp.a."; exit 1; };