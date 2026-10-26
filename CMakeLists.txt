# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.22)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(RootCDH)

# For silencing warnings of using mismatched esp IDF version and arduino lib version
idf_component_get_property(arduino_lib espressif__arduino-esp32 COMPONENT_LIB)
target_compile_options(${arduino_lib} PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:-Wno-error=missing-field-initializers>"
    "$<$<COMPILE_LANGUAGE:C,CXX>:-Wno-error=cpp>"
)	