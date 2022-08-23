cmake_minimum_required( VERSION 3.2 )
qt_add_library( image_painter STATIC )
qt_add_qml_module(
		image_painter
		VERSION 1.0
		URI "ImagePainter"
		QML_FILES
		ImagePainter.qml
		SOURCES
		image_painter.cpp image_painter.hpp
)
set( QML_MODULE_PATH ${CMAKE_CURRENT_BINARY_DIR} PARENT_SCOPE )