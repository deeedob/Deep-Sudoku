include( ../cmake/external-build.cmake )

set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )

set( QTLibs
     Core
     Qml
     Quick
     Multimedia
     Widgets
     Gui
     Svg )

set( sources
     main.cpp
     app.cpp
     board_model.cpp
     cell_data.cpp
     media_capture.cpp
     deep_solver.cpp
     img_proc/cv_segmentation.cpp
     img_proc/cv_img_mat.cpp
     img_proc/cv_hough_finder.cpp
     )
set( headers
     app.hpp
     board_model.hpp
     cell_data.hpp
     media_capture.hpp
     deep_solver.hpp
     img_proc/cv_segmentation.hpp
     img_proc/cv_img_mat.hpp
     img_proc/cv_hough_finder.hpp
     )

set( resources
     qml/qml.qrc
     qml/custom/custom.qrc
     qml/images/images.qrc
     )

#setup the opencv build
set( THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/third-party" CACHE PATH "Third-Party directory" )
set( THIRD_PARTY_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/third-party" CACHE PATH "Third-Party installation directory" )

if( ANDROID )
	message( "ANDROID BUILD" )
	add_definitions( -DANDROID_BUILD )
	
	# build opencv on first init
	include( ../cmake/android-opencv-settings.cmake )
	build_external_project( OpenCV "${THIRD_PARTY_DIR}/opencv" ${opencv_CMake_args} )
	set( OpenCV_DIR ${THIRD_PARTY_INSTALL_DIR}/opencv/sdk/native/jni/abi-${ANDROID_ABI} )
	find_package( OpenCV REQUIRED PATHS ${OpenCV_DIR} )
	
	set( CMAKE_FIND_ROOT_PATH ${ANDROID_QT_ROOT} )
	find_package( Qt6 COMPONENTS ${QTLibs} REQUIRED PATHS ${CMAKE_FIND_ROOT_PATH} NO_DEFAULT_PATH )
	qt_standard_project_setup()
	
	qt_add_executable( QSudoku
	                   ${sources}
	                   ${headers}
	                   ${resources}
	                   )
	
	qt_add_qml_module( QSudoku
	                   URI App
	                   VERSION 1.0
	                   SOURCES
	                   app.cpp app.hpp
	                   )
	
	set( QT_ANDROID_EXTRA_LIBS
	     ${OpenCV_LIBS}
	     frugally-deep::fdeep
	     )
	set( ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android" )
	qt_android_generate_deployment_settings( QSudoku )
	
	set_property( TARGET QSudoku APPEND PROPERTY
	              QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/android )
	qt_android_add_apk_target( QSudoku )

else()
	# :::::::::::::::::: DESKTOP BUILD ::::::::::::::::::::::
	message( "DESKTOP BUILD" )
	
	# build opencv on first init
	include( ../cmake/desktop-opencv-settings.cmake )
	build_external_project( OpenCV
	                        "${THIRD_PARTY_DIR}/opencv"
	                        ${opencv_CMake_args} )
	set( OpenCV_DIR ${THIRD_PARTY_INSTALL_DIR}/opencv/sdk/native/jni/ )
	find_package( OpenCV
	              HINTS ${THIRD_PARTY_INSTALL_DIR}/opencv
	              NO_DEFAULT_PATH
	              REQUIRED )
	
	find_package( Qt6 COMPONENTS ${QTLibs} REQUIRED )
	qt_standard_project_setup()
	
	list( APPEND sources enhanced_engine.cpp )
	list( APPEND headers enhanced_engine.hpp )
	qt_add_executable( QSudoku
	                   ${sources}
	                   ${headers}
	                   ${resources}
	                   )
	qt_add_qml_module( QSudoku
	                   URI App
	                   VERSION 1.0
	                   SOURCES
	                   app.cpp app.hpp
	                   )
	# :::::::::::::::::: END OF DESKTOP BUILD ::::::::::::::::::::::
endif()

add_subdirectory( modules/ImagePainter )
set( QML_IMPORT_PATH ${QML_MODULE_PATH} CACHE STRING "path for custom qml modules!" )
message( "QML IMPORT PATH: " ${QML_MODULE_PATH} )

target_include_directories( QSudoku PUBLIC
                            "${THIRD_PARTY_INSTALL_DIR}/frugally-deep"
                            "${CMAKE_CURRENT_SOURCE_DIR}/lib/frugally-deep"
                            )
configure_file( ../ai_model/fdeep_model.json ${CMAKE_CURRENT_BINARY_DIR}/model/fdeep_model.json )

# Linking all libraries
set( QTLinker )
foreach( Lib ${QTLibs} )
	list( APPEND QTLinker "Qt6::${Lib}" )
endforeach()
MESSAGE( "LINKING TO : " ${QTLinker} )

target_link_libraries( QSudoku
                       PRIVATE
                       ${QTLinker}
                       ${OpenCV_LIBS}
                       image_painterplugin
                       PUBLIC
                       fdeep
                       )

# set compile options for the main executable
target_compile_options( QSudoku PRIVATE ${CXX_FLAGS}
                        "$<$<CONFIG:Debug>:${CXX_FLAGS_DEBUG}>"
                        "$<$<CONFIG:Release>:${CXX_FLAGS_RELEASE}>" )