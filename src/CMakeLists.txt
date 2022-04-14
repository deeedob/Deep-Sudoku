cmake_minimum_required(VERSION 3.16)

qt_add_executable(${_TARGET}
        main.cpp
        qcvimageprocessor.cpp
        qcvimageprocessor.hpp
        )

qt_add_qml_module(${_TARGET}
        URI ${_TARGET}
        VERSION 1.0
        QML_FILES qml/main.qml qml/PhotoPreview.qml
        )

set_target_properties(${_TARGET} PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        )

target_compile_definitions(${_TARGET}
        PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${_TARGET}
        #PRIVATE Qt6::Quick Qt6::Qml Qt6::Multimedia)
        PRIVATE Qt6::Quick Qt6::Qml Qt6::Multimedia ${opencv_lib})
#PRIVATE Qt6::Quick Qt6::Qml Qt6::Multimedia PRIVATE Qt6::Core)
