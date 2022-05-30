cmake_minimum_required(VERSION 3.16)

project(QTSudoku LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)
### Setup android toolchain and arch
set(ANDROID_ABI arm64-v8a)
set(ANDROID_NATIVE_API_LEVEL 23)
set(ANDROID_NDK /opt/android-sdk/ndk/22.1.7171670)
set(ANDROID_SDK_ROOT /opt/android-sdk)
set(CMAKE_FIND_ROOT_PATH /home/dennis/Qt/6.3.0/android_arm64_v8a)
set(CMAKE_TOOLCHAIN_FILE /opt/android-sdk/ndk/22.1.7171670/build/cmake/android.toolchain.cmake)
set(CMAKE_PREFIX_PATH /home/dennis/Qt/6.3.0/android_arm64_v8a)
set(QT_HOST_PATH /home/dennis/Qt/6.3.0/gcc_64)

set(CMAKE_C_COMPILER ${ANDROID_NDK}/toolchains/llvm/prebuilt/linux-x86_64/bin/clang)
set(CMAKE_CXX_COMPILER ${ANDROID_NDK}/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++)

set(ANDROID_STL c++_static) # or shared
set(CMAKE_GENERATOR Ninja)

# setting for Qt-Creator
set(QT_ANDROID_ABIS arm64-v8a)
set(QT_ANDROID_BUILD_ALL_ABIS FALSE)
set(QT_CREATOR_SKIP_PACKAGE_MANAGER_SETUP OFF)
set(QT_DEFAULT_ANDROID_ABIS armeabi-v7a arm64-v8a x86 x86_64)
set(QT_NO_GLOBAL_APK_TARGET_PART_OF_ALL ON)

# Enable warnings
if(CMAKE_CXX_COMPILER_ID STREQUAL Clang
        OR CMAKE_CXX_COMPILER_ID STREQUAL AppleClang)
    set(warning_options
            -Werror
            -Weverything
            -Wno-c++98-compat
            -Wno-c++98-compat-pedantic
            -Wno-documentation-unknown-command
            -Wno-exit-time-destructors
            -Wno-padded
            -Wno-undefined-reinterpret-cast
            -Wno-float-equal
            # These are disabled because of automoc
            -Wno-missing-prototypes
            -Wno-header-hygiene
            -Wno-old-style-cast
            )
elseif(MSVC)
    set(warning_options /WX /W4 /wd4503 /wd4510 /wd4512 /wd4610)
endif()

add_subdirectory(src)
