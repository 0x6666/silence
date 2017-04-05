cmake_minimum_required (VERSION 2.8)

include("x/check.cmake")

project(silence)
#PROJECT_NAME

include("x/x.cmake")

x_package_begin(silence EXECUTABLE)
	x_package_sources(
		src/audio.h
		src/silence.cpp
		src/stdafx.cpp
		src/stdafx.h
		src/targetver.h
	)
x_package_end()

#x_extren_package(
#	QtCore SHARED
#	LOCATION ${X_QT_PATH}/lib
#	BINARY_NAMES_DEBUG QtCored4
#	BINARY_NAMES_RELEASE QtCore4
#)
