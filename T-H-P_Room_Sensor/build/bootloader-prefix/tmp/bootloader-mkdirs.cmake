# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/thom/Documents/esp-idf/v4.4/esp-idf/components/bootloader/subproject"
  "/Users/thom/Documents/Prog/21thom03.github/T-H-P_Room_Sensor/build/bootloader"
  "/Users/thom/Documents/Prog/21thom03.github/T-H-P_Room_Sensor/build/bootloader-prefix"
  "/Users/thom/Documents/Prog/21thom03.github/T-H-P_Room_Sensor/build/bootloader-prefix/tmp"
  "/Users/thom/Documents/Prog/21thom03.github/T-H-P_Room_Sensor/build/bootloader-prefix/src/bootloader-stamp"
  "/Users/thom/Documents/Prog/21thom03.github/T-H-P_Room_Sensor/build/bootloader-prefix/src"
  "/Users/thom/Documents/Prog/21thom03.github/T-H-P_Room_Sensor/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/thom/Documents/Prog/21thom03.github/T-H-P_Room_Sensor/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
