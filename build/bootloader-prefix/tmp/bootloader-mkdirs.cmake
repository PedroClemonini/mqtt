# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/pedroclemonini/esp/esp-idf/components/bootloader/subproject"
  "/Users/pedroclemonini/esp/mqtt/build/bootloader"
  "/Users/pedroclemonini/esp/mqtt/build/bootloader-prefix"
  "/Users/pedroclemonini/esp/mqtt/build/bootloader-prefix/tmp"
  "/Users/pedroclemonini/esp/mqtt/build/bootloader-prefix/src/bootloader-stamp"
  "/Users/pedroclemonini/esp/mqtt/build/bootloader-prefix/src"
  "/Users/pedroclemonini/esp/mqtt/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/pedroclemonini/esp/mqtt/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/pedroclemonini/esp/mqtt/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
