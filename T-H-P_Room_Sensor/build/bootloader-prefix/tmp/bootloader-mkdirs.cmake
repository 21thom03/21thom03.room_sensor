# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/thomas.dacosta/esp/v4.4/esp-idf/components/bootloader/subproject"
  "C:/Users/thomas.dacosta/Documents/21thom03.room_sensor/T-H-P_Room_Sensor/build/bootloader"
  "C:/Users/thomas.dacosta/Documents/21thom03.room_sensor/T-H-P_Room_Sensor/build/bootloader-prefix"
  "C:/Users/thomas.dacosta/Documents/21thom03.room_sensor/T-H-P_Room_Sensor/build/bootloader-prefix/tmp"
  "C:/Users/thomas.dacosta/Documents/21thom03.room_sensor/T-H-P_Room_Sensor/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Users/thomas.dacosta/Documents/21thom03.room_sensor/T-H-P_Room_Sensor/build/bootloader-prefix/src"
  "C:/Users/thomas.dacosta/Documents/21thom03.room_sensor/T-H-P_Room_Sensor/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/thomas.dacosta/Documents/21thom03.room_sensor/T-H-P_Room_Sensor/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
