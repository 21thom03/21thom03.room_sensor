# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/thomas.dacosta/esp/v4.4/esp-idf/tools/kconfig"
  "C:/Users/thomas.dacosta/Documents/21thom03.room_sensor/T-H-P_Room_Sensor/build/bootloader/kconfig_bin"
  "C:/Users/thomas.dacosta/Documents/21thom03.room_sensor/T-H-P_Room_Sensor/build/bootloader/mconf-idf-prefix"
  "C:/Users/thomas.dacosta/Documents/21thom03.room_sensor/T-H-P_Room_Sensor/build/bootloader/mconf-idf-prefix/tmp"
  "C:/Users/thomas.dacosta/Documents/21thom03.room_sensor/T-H-P_Room_Sensor/build/bootloader/mconf-idf-prefix/src/mconf-idf-stamp"
  "C:/Users/thomas.dacosta/Documents/21thom03.room_sensor/T-H-P_Room_Sensor/build/bootloader/mconf-idf-prefix/src"
  "C:/Users/thomas.dacosta/Documents/21thom03.room_sensor/T-H-P_Room_Sensor/build/bootloader/mconf-idf-prefix/src/mconf-idf-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/thomas.dacosta/Documents/21thom03.room_sensor/T-H-P_Room_Sensor/build/bootloader/mconf-idf-prefix/src/mconf-idf-stamp/${subDir}")
endforeach()
