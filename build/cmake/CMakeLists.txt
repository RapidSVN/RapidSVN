cmake_minimum_required (VERSION 3.10)

project(RapidSVN)

set(CMAKE_CXX_STANDARD 14)

enable_testing()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake_modules ${CMAKE_MODULE_PATH})

include(CheckFunctionExists)
include(CheckIncludeFile)

macro(report_mandatory_not_found component)
    message(SEND_ERROR "${component} library/component could not be found and is a mandatory dependency
    HINT:
    - add the ${component} install directory to the CMAKE_PREFIX_PATH variable (-DCMAKE_PREFIX_PATH=\"/path/to/${component}-install-dir;/path/to/other/dirs\"")
endmacro()


# find dependencies
find_package(APR)
find_package(SVN)

set(WX_VERSION "3.0")
find_package(WX)

set(TOP_DIR ${CMAKE_CURRENT_LIST_DIR}/../..)
set(LIBRAPIDSVN_DIR ${TOP_DIR}/librapidsvn)
set(LIBSVNCPP_DIR ${TOP_DIR}/libsvncpp)
set(RAPIDSVN_DIR ${TOP_DIR}/rapidsvn)

add_subdirectory(libsvncpp)
add_subdirectory(librapidsvn)
add_subdirectory(rapidsvn)
