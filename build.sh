#!/bin/bash

# ESP_IDF_VERSION="4.1"
PROJECT_PATH=$(pwd);
ESP_HOMEKIT_SDK_COMMIT_HASH="fac2032426d3cd29d8b6cc2663d0e7945d1d020d";


function cleanup {
    cd "$PROJECT_PATH";
    printf "\n** Deleting temporary build files **";
    rm -rf build;
}

trap cleanup EXIT;

# Setup
printf "** Creating temporary build folder **\n"
mkdir build;
cd build || { echo "Couldn't enter the build directory."; exit 1; }

# Clone and enter the fan example directory
printf "\n** Cloning espressif/esp-homekit-sdk.git **\n"
git clone https://github.com/espressif/esp-homekit-sdk.git 1> /dev/null;
cd esp-homekit-sdk || { echo "Couldn't enter the esp-homekit-sdk repo directory; git clone probably failed."; exit 1; }
git checkout $ESP_HOMEKIT_SDK_COMMIT_HASH 1> /dev/null;
git submodule update --init --recursive 1> /dev/null;

# Copy source header files
printf "\n** Copying header files **\n"
cp "$(find . -name "hap.h")" "$PROJECT_PATH/esp-homekit-lib/include" || { echo "Couldn't copy hap.h."; exit 1; };
cp "$(find . -name "hap_apple_chars.h")" "$PROJECT_PATH/esp-homekit-lib/include" || { echo "Couldn't copy hap_apple_chars..h."; exit 1; };
cp "$(find . -name "hap_apple_servs.h")" "$PROJECT_PATH/esp-homekit-lib/include" || { echo "Couldn't copy hap_apple_servs.h."; exit 1; };

# Setup IDF environment and build the example
printf "\n** Building esp-homekit-sdk fan example to extract libaries **\n"
cd examples/fan/ || { echo "Couldn't enter the fan example directory."; exit 1; }
idf.py set-target esp32 1> /dev/null || { echo "Couldn't set ESP32 as the build target. Check that you have exported the IDF environment variables."; exit 1; }
idf.py build 1> /dev/null || { echo "Build failed."; exit 1; }

# Copy built esp-homekit-sdk libraries
printf "\n** Copying built esp-homekit-sdk library files **\n"
cp "$(find . -name "libesp_hap_apple_profiles.a")" "$PROJECT_PATH/esp-homekit-lib" || { echo "Couldn't copy libesp_hap_apple_profiles.a."; exit 1; };
cp "$(find . -name "libesp_hap_core.a")" "$PROJECT_PATH/esp-homekit-lib"|| { echo "Couldn't copy libesp_hap_core.a."; exit 1; };
cp "$(find . -name "libesp_hap_extras.a")" "$PROJECT_PATH/esp-homekit-lib"|| { echo "Couldn't copy libesp_hap_extras.a."; exit 1; };
cp "$(find . -name "libesp_hap_platform.a")" "$PROJECT_PATH/esp-homekit-lib"|| { echo "Couldn't copy libesp_hap_platform.a."; exit 1; };
cp "$(find . -name "libhkdf-sha.a")" "$PROJECT_PATH/esp-homekit-lib"|| { echo "Couldn't copy libhkdf-sha.a."; exit 1; };
cp "$(find . -name "libjson_generator.a")" "$PROJECT_PATH/esp-homekit-lib"|| { echo "Couldn't copy libjson_generator.a."; exit 1; };
cp "$(find . -name "libjson_parser.a")" "$PROJECT_PATH/esp-homekit-lib"|| { echo "Couldn't copy libjson_parser.a."; exit 1; };
cp "$(find . -name "libmu_srp.a")" "$PROJECT_PATH/esp-homekit-lib"|| { echo "Couldn't copy libmu_srp.a."; exit 1; };