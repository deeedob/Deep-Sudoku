cmake_minimum_required(VERSION 3.16)

project(QTSudoku LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if(CMAKE_CXX_COMPILER_LOADED)
    message(STATUS "Compiler path: ${CMAKE_CXX_COMPILER}")
    message(STATUS "Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
    message(STATUS "Compiler version:
             ${CMAKE_CXX_COMPILER_VERSION}")
endif()

# option for outputting compiler information at runtime
option(OUTPUT_COMPILER_INFO
        "Output compiler information when launching the main executable." ON)


# variables to store additional compiler flags
set(CXX_FLAGS)
set(CXX_FLAGS_DEBUG)
set(CXX_FLAGS_RELEASE)

# append global compiler flags to list
list(APPEND CXX_FLAGS "-fPIC" "-Wall" "-Wextra" "-pedantic" "-Werror" "-fdeclspec")


# append compile flags depending on the selected compiler and build type
if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
    list(APPEND CXX_FLAGS "-fno-rtti" "-fno-exceptions")
    list(APPEND CXX_FLAGS_DEBUG "-Wsuggest-final-types" "-Wsuggest-final-methods" "-Wsuggest-override")
    list(APPEND CXX_FLAGS_RELEASE "-O3" "-Wno-unused")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES Clang)
    list(APPEND CXX_FLAGS "-fno-rtti" "-fno-exceptions" "-Qunused-arguments" "-fcolor-diagnostics")
    list(APPEND CXX_FLAGS_DEBUG "-Wdocumentation")
    list(APPEND CXX_FLAGS_RELEASE "-O3" "-Wno-unused")
endif()

# output compiler flags
include(CMakePrintHelpers)
cmake_print_variables(CXX_FLAGS)
cmake_print_variables(CXX_FLAGS_DEBUG)
cmake_print_variables(CXX_FLAGS_RELEASE)


add_subdirectory(src)
