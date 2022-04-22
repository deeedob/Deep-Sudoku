cmake_minimum_required(VERSION 3.16)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Setup Opencv Build for android
include(ExternalProject)
set(opencv_CMake_args
        -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/opencv-prefix
        -DANDROID_ABI=${ANDROID_ABI}
        -DANDROID_NATIVE_API_LEVEL=${ANDROID_NATIVE_API_LEVEL}
        -DANDROID_ARM_NEON=ON
        -DANDROID_TOOLCHAIN=clang
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DANDROID_NDK=${ANDROID_NDK}
        -DANDROID_SDK=${ANDROID_SDK_ROOT}
        -DBUILD_SHARED_LIBS=ON
        -DCMAKE_BUILD_TYPE=Release
        -DBUILD_ANDROID_PROJECTS=OFF
        -DWITH_OPENCL=ON
        -DWITH_TBB=ON
        -DENABLE_NEON=ON
        -DBUILD_TESTS=OFF
        -DBUILD_PERF_TESTS=OFF
        )
if(UNIX)
    set(opencv_CMake_args ${opencv_CMake_args} -DCMAKE_CXX_FLAGS=-fPIC)
endif()


ExternalProject_Add(OpenCVBuild
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/third-party/opencv
        CMAKE_ARGS ${opencv_CMake_args}
        )
set(OpenCV_DIR ${CMAKE_CURRENT_BINARY_DIR}/opencv-prefix/sdk/native/jni/abi-arm64-v8a)
find_package(OpenCV REQUIRED)
find_package(Qt6 COMPONENTS Quick Qml Multimedia REQUIRED)

qt_add_executable(${_TARGET}
       main.cpp
        qcv_algorithm.cpp
        qcv_algorithm.hpp
        qml/qml.qrc)

message("OpenCV_LIBS linking against : " ${OpenCV_LIBS})
target_link_libraries(${_TARGET}
        PRIVATE Qt6::Quick Qt6::Qml Qt6::Multimedia ${OpenCV_LIBS})

set_property(TARGET ${_TARGET} APPEND PROPERTY
        QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/android)

target_compile_definitions(${_TARGET}
        PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)


