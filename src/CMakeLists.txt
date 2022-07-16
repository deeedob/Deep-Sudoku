include(../cmake/external-build.cmake)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(sources
        main.cpp
        app.cpp
        board_model.cpp
        cell_data.cpp
        )
set(headers
        app.hpp
        board_model.hpp
        cell_data.hpp
        )

#setup the opencv build
set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/third-party")
set(THIRD_PARTY_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/third-party")

# find and install frugally deep and its dependencies!
include(../cmake/frugally-deep-build.cmake)
find_package(
        frugally-deep
        HINTS ${THIRD_PARTY_INSTALL_DIR}/frugally-deep
        NO_DEFAULT_PATH
        REQUIRED
)

if (ANDROID)
    message("ANDROID BUILD")
    add_definitions(-DANDROID_BUILD)

    include(../cmake/android-opencv-settings.cmake)

    # build opencv on first init
    build_external_project(OpenCV "${THIRD_PARTY_DIR}/opencv" ${opencv_CMake_args})
    set(OpenCV_DIR ${THIRD_PARTY_INSTALL_DIR}/opencv/sdk/native/jni/abi-${ANDROID_ABI})
    find_package(OpenCV REQUIRED)

    set(CMAKE_FIND_ROOT_PATH ${ANDROID_QT_ROOT})
    find_package(Qt6 COMPONENTS Core Qml Quick Multimedia Widgets Svg REQUIRED PATHS ${CMAKE_FIND_ROOT_PATH} NO_DEFAULT_PATH)

    qt_add_executable(QSudoku
            ${sources}
            ${headers}
            qml/qml.qrc)

    set(QT_ANDROID_EXTRA_LIBS
            ${OpenCV_LIBS})
    #qt_android_generate_deployment_settings(QSudoku)

    set_property(TARGET QSudoku APPEND PROPERTY
            QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/android)


else()
    # :::::::::::::::::: DESKTOP BUILD ::::::::::::::::::::::
    # currently for the Desktop Build OpenCV need to be installed on
    # the system to avoid huge build times
    message("DESKTOP BUILD")
    add_definitions(-DQT_QML_DEBUG)


    find_package(Qt6 COMPONENTS Core Qml Quick Multimedia Widgets Svg REQUIRED)
    find_package(OpenCV REQUIRED)

    list(APPEND sources enhanced_engine.cpp)
    list(APPEND headers enhanced_engine.hpp)
    qt_add_executable(QSudoku
            ${sources}
            ${headers}
            qml/qml.qrc)
endif()

target_link_libraries(QSudoku
        PRIVATE
        Qt6::Qml Qt6::Quick Qt6::Core Qt6::Multimedia Qt6::Widgets Qt6::Svg #${OpenCV_LIBS} #TODO this crashes qmlengine?!
        PUBLIC
        frugally-deep::fdeep
        )
# set compile options for the main executable
target_compile_options( QSudoku PRIVATE ${CXX_FLAGS}
        "$<$<CONFIG:Debug>:${CXX_FLAGS_DEBUG}>"
        "$<$<CONFIG:Release>:${CXX_FLAGS_RELEASE}>")
