cmake_minimum_required (VERSION 2.8)

include("x/check.cmake")

project(Silence)

include("x/x.cmake")

x_sub_dir(silence)

#x_extren_package(
#	QtCore SHARED
#	LOCATION ${X_QT_PATH}/lib
#	BINARY_NAMES_DEBUG QtCored4
#	BINARY_NAMES_RELEASE QtCore4
#)


x_finish_project()

