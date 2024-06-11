# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/Thom.Dcst/esp4.4/v4.4/esp-idf/tools/kconfig"
  "C:/Users/Thom.Dcst/Documents/21thom03.github/T-H-P_Room_Sensor/build/bootloader/kconfig_bin"
  "C:/Users/Thom.Dcst/Documents/21thom03.github/T-H-P_Room_Sensor/build/bootloader/mconf-idf-prefix"
  "C:/Users/Thom.Dcst/Documents/21thom03.github/T-H-P_Room_Sensor/build/bootloader/mconf-idf-prefix/tmp"
  "C:/Users/Thom.Dcst/Documents/21thom03.github/T-H-P_Room_Sensor/build/bootloader/mconf-idf-prefix/src/mconf-idf-stamp"
  "C:/Users/Thom.Dcst/Documents/21thom03.github/T-H-P_Room_Sensor/build/bootloader/mconf-idf-prefix/src"
  "C:/Users/Thom.Dcst/Documents/21thom03.github/T-H-P_Room_Sensor/build/bootloader/mconf-idf-prefix/src/mconf-idf-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/Thom.Dcst/Documents/21thom03.github/T-H-P_Room_Sensor/build/bootloader/mconf-idf-prefix/src/mconf-idf-stamp/${subDir}")
endforeach()
