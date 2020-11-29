#!/bin/bash
set -e # Stop on the first error

PROJECT_DIR=$(pwd)

cleanup() {
    printf "\nINFO: Cleaning up tmp directory\n"
    cd $PROJECT_DIR
    rm -rf tmp
}

trap 'cleanup' ERR

printf "INFO: Activating IDF virtual environment\n"
. ${1:-$HOME/esp/esp-idf/export.sh}

printf "INFO: Creating and entering tmp directory\n"
mkdir tmp
cd tmp

printf "\nINFO: Cloning and entering esp-homekit-sdk repo\n"
git clone --recursive https://github.com/espressif/esp-homekit-sdk.git
cd esp-homekit-sdk

COMMIT="040b0f301223ebc6995597328e5a5cc9f9739a02"
printf "\nINFO: Checking out to commit $COMMIT\n"
git checkout $COMMIT

printf "\nINFO: Copying sdkconfig to esp-homekit-sdk fan example\n"
cp $PROJECT_DIR/sdkconfig examples/fan/

printf "\nBuilding fan example\n"
cd examples/fan/
idf.py build

printf "\nINFO: Copying compiled libraries to project's lib directory\n"
cd build/esp-idf 
cp esp_hap_*/*.a $PROJECT_DIR/lib/
cp hkdf-sha/*.a $PROJECT_DIR/lib/
cp json_generator/*.a $PROJECT_DIR/lib/
cp json_parser/*.a $PROJECT_DIR/lib/
cp mu_srp/*.a $PROJECT_DIR/lib/

printf "\nINFO: Copying header files for main libraries to project's include directory\n"
cd $PROJECT_DIR/tmp/esp-homekit-sdk/components/homekit
cp esp_hap_core/include/hap.h $PROJECT_DIR/include
cp esp_hap_apple_profiles/include/*.h $PROJECT_DIR/include

cleanup

printf "\nINFO: esp-homekit-sdk library built successfully!\n"

exit 0