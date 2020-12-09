#!/bin/bash
set -e # Stop on the first error

PROJECT_DIR=$(pwd)

cleanup() {
    if [ "$1" == "cleanup" ]; then
        printf "\nINFO: Cleaning up tmp directory\n"
        cd $PROJECT_DIR
        rm -rf tmp
    fi
}

trap "cleanup $1" ERR
trap "cleanup $1" SIGINT

printf "INFO: Creating and entering tmp directory\n"
mkdir -p tmp
cd tmp

if [ ! -d "$PROJECT_DIR/tmp/esp-idf3-homekit-sdk" ] 
then
    printf "\nINFO: Cloning and entering esp-idf3-homekit-sdk repo\n"
    git clone --recursive https://github.com/Brawrdon/esp-idf3-homekit-sdk.git
else
    printf "\nINFO: Entering esp-idf3-homekit-sdk repo\n"
fi
cd esp-idf3-homekit-sdk


BRANCH="esp-idf-3.3.4"
printf "\nINFO: Checking out to branch: $BRANCH\n"
git checkout $BRANCH

printf "\nINFO: Copying sdkconfig to esp-idf3-homekit-sdk fan example\n"
cp $PROJECT_DIR/sdkconfig examples/fan/

printf "\nBuilding fan example\n"
cd examples/fan/
make

printf "\nINFO: Copying compiled libraries to project's lib directory\n"
cd build/ 
cp esp_hap_*/*.a $PROJECT_DIR/lib/
cp hkdf-sha/*.a $PROJECT_DIR/lib/
cp json_generator/*.a $PROJECT_DIR/lib/
cp json_parser/*.a $PROJECT_DIR/lib/
cp mu_srp/*.a $PROJECT_DIR/lib/

printf "\nINFO: Copying header files for main libraries to project's include directory\n"
cd $PROJECT_DIR/tmp/esp-idf3-homekit-sdk/components/homekit
cp esp_hap_core/include/hap.h $PROJECT_DIR/include
cp esp_hap_core/src/priv_includes/esp_hap_ip_services.h $PROJECT_DIR/include
cp esp_hap_apple_profiles/include/*.h $PROJECT_DIR/include

cleanup $1

printf "\nINFO: esp-idf3-homekit-sdk library built successfully!\n"

exit 0

