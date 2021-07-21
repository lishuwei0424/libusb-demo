#/*build by lishuwei  date:2021-07-22
cmake_minimum_required(VERSION 3.10)
project(libusb-demo)

set(VERION_SOFT, 0.1)
configure_file (
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_BINARY_DIR}/config.h"
)
include_directories(
    ${PROJECT_SOURCE_DIR}/log4cxx
    ${PROJECT_SOURCE_DIR}/inc/usb
    ${PROJECT_SOURCE_DIR}/inc/hotplug
)
#SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(libusb-demo  ${PROJECT_SOURCE_DIR}/src/hotplug.cpp  ${PROJECT_SOURCE_DIR}/app/test.cpp)
target_link_libraries(libusb-demo  ${PROJECT_SOURCE_DIR}/lib/usb/libusb-1.0.so pthread ${PROJECT_SOURCE_DIR}/lib/log4cxx//liblog4cxx.so)