Import("env")

import subprocess
import os
import shutil  
from pathlib import Path
import sys

idf_homekit_directory = "lib/esp-homekit-sdk/"
arduino_homekit_directory = Path("lib/homekit")

# Create the directory path for the ESP-IDF SDK
Path(idf_homekit_directory).mkdir(parents=True, exist_ok=True)

# Check if a .git folder exists, if so assume the project's been cloned.
git_folder = Path(idf_homekit_directory + ".git")
if not git_folder.exists() and not arduino_homekit_directory.exists(): # Add and lib/homekit so i can delete it
    commit = "040b0f301223ebc6995597328e5a5cc9f9739a02"
    process = subprocess.call(["git", "clone", "--recursive", "https://github.com/espressif/esp-homekit-sdk.git", idf_homekit_directory], stdout=open(os.devnull, 'wb'))
    process = subprocess.call(["git", "--git-dir", str(git_folder), "checkout", commit], stdout=open(os.devnull, 'wb'))
    shutil.copytree(idf_homekit_directory + "components/homekit", str(arduino_homekit_directory))
    shutil.rmtree(idf_homekit_directory)


for (dirpath, dirnames, filenames) in os.walk(arduino_homekit_directory):
    print(dirpath)


# ToDo:
# 1. Go through each folder and move includes and src to top level
# 2. Move priv_includes to top level
# 3. Replace <..> with "..." for component includes
# 4. Add priv_includes path
# ? Add a way to confirm that this has already been built, for example checking all directories have been moved. We can assume everything's in place and skip

 