import subprocess
import os
import glob
import shutil  
from distutils.dir_util import copy_tree
from pathlib import Path
import sys

idf_homekit_directory = "lib/esp-homekit-sdk/"
arduino_homekit_directory = "lib/homekit"

priv_includes = "lib/priv_includes"

if len(glob.glob("lib/*.c")) == 0:
    # Create the directory path for the ESP-IDF SDK
    Path(idf_homekit_directory).mkdir(parents=True, exist_ok=True)

    # Check if a .git folder exists, if so assume the project's been cloned.
    git_folder = Path(idf_homekit_directory + ".git")
    if not git_folder.exists() and not Path(arduino_homekit_directory).exists(): # Add and lib/homekit so i can delete it
        commit = "040b0f301223ebc6995597328e5a5cc9f9739a02"
        process = subprocess.call(["git", "clone", "--recursive", "https://github.com/espressif/esp-homekit-sdk.git", idf_homekit_directory], stdout=open(os.devnull, 'wb'))
        process = subprocess.call(["git", "--git-dir", str(git_folder), "checkout", commit], stdout=open(os.devnull, 'wb'))
        shutil.copytree(idf_homekit_directory + "components/homekit", arduino_homekit_directory)
        shutil.rmtree(idf_homekit_directory)

    for (dirpath, dirnames, filenames) in os.walk(Path(arduino_homekit_directory)):
        # 1. Ignore example and test(s)
        if "example" in dirpath or "test" in dirpath or "tests" in dirpath:
            continue
        for file in filenames:
            if file.endswith(".c") or file.endswith(".h") and file[:-2] not in mfi_source:
                shutil.move(dirpath + "/" + file, "lib/")

    #
    shutil.rmtree(arduino_homekit_directory)

replacements = {"<jsmn/jsmn.h>": "\"jsmn.h\"", "\"esp32/rom/ets_sys.h\"": "\"rom/ets_sys.h\""}
esp_hap_config = "#include \"esp_hap_config.h\"\n"
for (dirpath, dirnames, filenames) in os.walk("lib"):
    for file in filenames:
        if file.endswith(".h"):
            replacements["<" + file + ">"] = "\"" + file + "\""

    for file in filenames:
        lines = []
        with open("lib/" + file) as infile:
            for line in infile:
                for src, target in replacements.items():
                    line = line.replace(src, target)
                
                # Messy manual additions / changes for specific files and lines
                if file == "esp_hap_main.c" and "hap_platform_os.h" in line:
                    line += esp_hap_config
                if file == "hap_platform_httpd.c" and "<esp_http_server.h>" in line:
                    line += esp_hap_config
                if file == "esp_mfi_i2c.c" and "esp_mfi_i2c.h" in line:
                    line += esp_hap_config
                
                lines.append(line)
                    
        with open("lib/" + file, 'w') as outfile:
            for line in lines:
                outfile.write(line)