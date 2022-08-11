include( ../cmake/external-build.cmake )

set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )

set( QTLibs
     Core
     Qml
     Quick
     Multimedia
     Widgets
     Svg )

set( sources
     main.cpp
     app.cpp
     board_model.cpp
     cell_data.cpp
     media_capture.cpp
     cv_segmentation.cpp
     cv_img_mat.cpp
     deep_solver.cpp
     cv_hough_finder.cpp
     )
set( headers
     app.hpp
     board_model.hpp
     cell_data.hpp
     media_capture.hpp
     cv_segmentation.hpp
     cv_img_mat.hpp
     deep_solver.hpp
     cv_hough_finder.hpp
     )
set( resources
     qml/qml.qrc
     qml/custom/custom.qrc
     qml/images/images.qrc
     )

#setup the opencv build
set( THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/third-party" )
set( THIRD_PARTY_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/third-party" )

# find and install frugally deep and its dependencies!
include( ../cmake/frugally-deep-build.cmake )
find_package(
		frugally-deep
		HINTS ${THIRD_PARTY_INSTALL_DIR}/frugally-deep
		NO_DEFAULT_PATH
		REQUIRED
)

if( ANDROID )
	message( "ANDROID BUILD" )
	add_definitions( -DANDROID_BUILD )
	
	# build opencv on first init
	include( ../cmake/android-opencv-settings.cmake )
	build_external_project( OpenCV "${THIRD_PARTY_DIR}/opencv" ${opencv_CMake_args} )
	set( OpenCV_DIR ${THIRD_PARTY_INSTALL_DIR}/opencv/sdk/native/jni/abi-${ANDROID_ABI} )
	find_package( OpenCV REQUIRED )
	
	set( CMAKE_FIND_ROOT_PATH ${ANDROID_QT_ROOT} )
	find_package( Qt6 COMPONENTS ${QTLibs} REQUIRED PATHS ${CMAKE_FIND_ROOT_PATH} NO_DEFAULT_PATH )
	
	qt_add_executable( QSudoku
	                   ${sources}
	                   ${headers}
	                   ${resources}
	                   )
	
	set( QT_ANDROID_EXTRA_LIBS
	     ${OpenCV_LIBS} )
	#qt_android_generate_deployment_settings(QSudoku)
	
	set_property( TARGET QSudoku APPEND PROPERTY
	              QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/android )

else()
	# :::::::::::::::::: DESKTOP BUILD ::::::::::::::::::::::
	# currently for the Desktop Build OpenCV need to be installed on
	# the system to avoid huge build times
	message( "DESKTOP BUILD" )
	add_definitions( -DQT_QML_DEBUG )
	
	# build opencv on first init
	include( ../cmake/desktop-opencv-settings.cmake )
	build_external_project( OpenCV "${THIRD_PARTY_DIR}/opencv" ${opencv_CMake_args} )
	set( OpenCV_DIR ${THIRD_PARTY_INSTALL_DIR}/opencv/sdk/native/jni/ )
	find_package( OpenCV
	              HINTS ${THIRD_PARTY_INSTALL_DIR}/opencv
	              NO_DEFAULT_PATH
	              REQUIRED )
	find_package( Qt6 COMPONENTS ${QTLibs} REQUIRED )
	
	list( APPEND sources enhanced_engine.cpp )
	list( APPEND headers enhanced_engine.hpp )
	qt_add_executable( QSudoku
	                   ${sources}
	                   ${headers}
	                   ${resources}
	                   )
	
	# :::::::::::::::::: END OF DESKTOP BUILD ::::::::::::::::::::::
endif()

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
                       PUBLIC
                       frugally-deep::fdeep
                       )
# set compile options for the main executable
target_compile_options( QSudoku PRIVATE ${CXX_FLAGS}
                        "$<$<CONFIG:Debug>:${CXX_FLAGS_DEBUG}>"
                        "$<$<CONFIG:Release>:${CXX_FLAGS_RELEASE}>" )