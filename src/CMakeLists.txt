cmake_minimum_required(VERSION 3.16)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

####################################################### BUILD BEFORE ###############################################
function (build_external_project target file_name)

    set(CMAKELIST_CONTENT "
        cmake_minimum_required(VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})
        project(build_external_project)
        include(ExternalProject)
        ExternalProject_add(${target}
            SOURCE_DIR ${CMAKE_SOURCE_DIR}/third-party/opencv
            CMAKE_GENERATOR ${CMAKE_GENERATOR}
            CMAKE_ARGS ${ARGN})
        add_custom_target(build_external_project)
        add_dependencies(build_external_project ${target})
    ")

    set(TARGET_DIR "${CMAKE_CURRENT_BINARY_DIR}/ExternalProjects/${target}")

    file(WRITE "${TARGET_DIR}/build/CMakeLists.txt" "${CMAKELIST_CONTENT}")

    file(MAKE_DIRECTORY "${TARGET_DIR}" "${TARGET_DIR}/build")

    execute_process(COMMAND ${CMAKE_COMMAND}
            -G "${CMAKE_GENERATOR}"
            -A "${CMAKE_GENERATOR_PLATFORM}"
            -T "${CMAKE_GENERATOR_TOOLSET}"
            WORKING_DIRECTORY "${TARGET_DIR}/build")

    execute_process(COMMAND ${CMAKE_COMMAND}
            --build .
            --config ${CMAKE_BUILD_TYPE}
            WORKING_DIRECTORY "${TARGET_DIR}/build")

endfunction()

set(THIRD_PARTY_DIR "${CMAKE_CURRENT_LIST_DIR}/third-party")
set(THIRD_PARTY_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/third-party")

# Setup Opencv Build for android
set(opencv_CMake_args
        -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/third-party/opencv
        -DANDROID_ABI=${ANDROID_ABI}
        -DANDROID_NATIVE_API_LEVEL=${ANDROID_NATIVE_API_LEVEL}
        -DANDROID_ARM_NEON=ON
        -DANDROID_TOOLCHAIN=clang
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DANDROID_NDK=${ANDROID_NDK}
        -DANDROID_SDK=${ANDROID_SDK_ROOT}
        -DBUILD_SHARED_LIBS=OFF # build static lib
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

build_external_project(OpenCV "${THIRD_PARTY_DIR}/opencv" ${opencv_CMake_args})

####################################################################################################

set(OpenCV_DIR ${CMAKE_CURRENT_BINARY_DIR}/third-party/opencv/sdk/native/jni/abi-arm64-v8a)
find_package(OpenCV REQUIRED)
find_package(Qt6 COMPONENTS Quick Qml Multimedia REQUIRED)

qt_add_executable(${_TARGET}
       main.cpp
        #qcv_algorithm.cpp
        #qcv_algorithm.hpp
        qml/qml.qrc)

message("OpenCV_LIBS linking against : " ${OpenCV_LIBS})
target_link_libraries(${_TARGET}
        PRIVATE Qt6::Quick Qt6::Qml Qt6::Multimedia ${OpenCV_LIBS})

set_property(TARGET ${_TARGET} APPEND PROPERTY
        QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/android)

target_compile_definitions(${_TARGET}
        PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/android-build/libs/${ANDROID_ABI}/")

# copy binary to solve issues with androiddeployqt
add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND cmake
        ARGS -E copy "${CMAKE_CURRENT_BINARY_DIR}/lib${PROJECT_NAME}_${ANDROID_ABI}.so" "${CMAKE_CURRENT_BINARY_DIR}/android-build/libs/${ANDROID_ABI}/"
)


