cmake_minimum_required( VERSION 3.16 )

project( QTSudoku LANGUAGES CXX )
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

if( CMAKE_CXX_COMPILER_LOADED )
	message( STATUS "Compiler path: ${CMAKE_CXX_COMPILER}" )
	message( STATUS "Compiler ID: ${CMAKE_CXX_COMPILER_ID}" )
	message( STATUS "Compiler version:
             ${CMAKE_CXX_COMPILER_VERSION}" )
endif()

# option for outputting compiler information at runtime
option( OUTPUT_COMPILER_INFO
        "Output compiler information when launching the main executable." ON )

# variables to store additional compiler flags
set( CXX_FLAGS )
set( CXX_FLAGS_DEBUG )
set( CXX_FLAGS_RELEASE )

# append global compiler flags to list
list( APPEND CXX_FLAGS "-fPIC" "-Wall" "-Wextra" "-pedantic" )

# append compile flags depending on the selected compiler and build type
if( CMAKE_CXX_COMPILER_ID MATCHES GNU )
	list( APPEND CXX_FLAGS "-fno-rtti" "-fno-exceptions" )
	list( APPEND CXX_FLAGS_DEBUG "-Wsuggest-final-types" "-Wsuggest-final-methods" "-Wsuggest-override" )
	list( APPEND CXX_FLAGS_RELEASE "-O3" "-Wno-unused" )
endif()

if( CMAKE_CXX_COMPILER_ID MATCHES Clang )
	list( APPEND CXX_FLAGS "-frtti" "-fno-exceptions" "-fcolor-diagnostics" "-fdeclspec" )
	list( APPEND CXX_FLAGS_DEBUG "-Wdocumentation" "-Wno-unused" )
	list( APPEND CXX_FLAGS_RELEASE "-O3" "-Wno-unused" )
endif()
# output compiler flags
include( CMakePrintHelpers )
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} cmake )
cmake_print_variables( CXX_FLAGS )
cmake_print_variables( CXX_FLAGS_DEBUG )
cmake_print_variables( CXX_FLAGS_RELEASE )

add_subdirectory( src )