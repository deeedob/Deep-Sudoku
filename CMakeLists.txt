cmake_minimum_required(VERSION 3.16)

set(_TARGET qtsudoku)

project(${_TARGET} VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ADROID_PACKAGE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/android)
set(CMAKE_AUTOMOC ON)
set(ARCHITECTURE arm64-v8a)
set(QT_FOR_ANDROID_DIR  /home/dennis/Qt/6.3.0/android_arm64_v8a/lib/cmake/Qt6)
set(LIB_OPENCV_DIR /home/dennis/Qt/opencv-3.4.3-android-sdk/OpenCV-android-sdk/sdk/native/jni)


# BUILD OPENCV

#set(OpenCV_DIR "/home/dennis/Qt/opencv-3.4.3-android-sdk/OpenCV-android-sdk/sdk/native/jni")
set(OpenCV_DIR "${CMAKE_SOURCE_DIR}/3rdparty/opencv/build/install/sdk/native/jni")
set(OpenCV_LIB_DIR "${CMAKE_SOURCE_DIR}/3rdparty/opencv/build/install/sdk/native/libs/${ARCHITECTURE}")

#list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
#message("CMAKE_MODULE_PATH=${CMAKE_MODULE_PATH}")


find_package(Qt6 6.3 COMPONENTS Quick Qml Multimedia REQUIRED PATHS ${QT_FOR_ANDROID_DIR})
#find_package(OpenCV HINTS ${OpenCV_DIR} REQUIRED)

message(${OpenCV_DIR})
include_directories(${_TARGET} ${OpenCV_DIR}/include)
#include_directories(BEFORE ${_TARGET} /opt/android-sdk/ndk/22.1.7171670/toolchains/llvm/prebuilt/linux-x86_64/lib64/clang/11.0.5/include)

set(ANDROID_EXTRA_LIBS
    ${OpenCV_LIB_DIR}/libopencv_core.so
        ${OpenCV_LIB_DIR}/libopencv_core.so
        ${OpenCV_LIB_DIR}/libopencv_imgproc.so
        ${OpenCV_LIB_DIR}/libopencv_dnn.so
        ${OpenCV_LIB_DIR}/libtbb.so
        CACHE INTERNAL "")
#include_directories()
#find_library(OPEN_CV_LIB
#            NAMES OpenCV
#            PATHS ${OpenCV_LIB_DIR})

add_library( opencv_lib SHARED IMPORTED )
set_target_properties( opencv_lib PROPERTIES IMPORTED_LOCATION
    ${OpenCV_LIB_DIR}/libopencv_core.so
        ${OpenCV_LIB_DIR}/libopencv_imgproc.so
        ${OpenCV_LIB_DIR}/libopencv_dnn.so
        ${OpenCV_LIB_DIR}/libtbb.s${OpenCV_LIB_DIR}/libopencv_core.so
        ${OpenCV_LIB_DIR}/libopencv_imgproc.so
        ${OpenCV_LIB_DIR}/libopencv_dnn.so
        ${OpenCV_LIB_DIR}/libtbb.s
)
#add_library(opencv STATIC )

#find_package(OpenCV REQUIRED PATHS /home/dennis/Qt/opencv-3.4.3-android-sdk/OpenCV-android-sdk/sdk/native/jni/OpenCVConfig.cmake)
if (OpenCV_FOUND)
    message(OPENCV FOUND!)
    #message(INCLUDING  OpenCV: ${OpenCV_INCLUDE_DIRS})
    #target_include_directories(${_TARGET} INTERFACE ${OpenCV_INCLUDE_DIRS})
    #include_directories(${_TARGET} ${LIB_OPENCV_DIR}/include)
else()
    message("No CMAKE_MODULE_PATH path for OpenCV configured")
endif()

add_subdirectory(src)
#set_property(TARGET ${_TARGET} APPEND PROPERTY
#    QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android)

