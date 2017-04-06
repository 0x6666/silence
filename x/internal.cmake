
if("!${CMAKE_SYSTEM_NAME}" STREQUAL "!Windows")
	set(OS_WIN TRUE)
elseif("!${CMAKE_SYSTEM_NAME}" STREQUAL "!Linux")
	set(OS_LINUX TRUE)
elseif("!${CMAKE_SYSTEM_NAME}" STREQUAL "!Darwin")
	set(OS_MAC TRUE)
else()
	message(FATAL_ERROR "Unknown platform: ${CMAKE_SYSTEM_NAME}!")
endif()

set(X_OUTPUT_ROOT "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${X_OUTPUT_ROOT}/lib")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${X_BUILD_CFG} "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${X_OUTPUT_ROOT}/bin")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${X_BUILD_CFG} "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${X_OUTPUT_ROOT}/bin")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${X_BUILD_CFG} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")


# 查找是否含有某个选项
# example: _find_radio_option(values "R1|R2|R3" "R2" _result)
function(_find_radio_option _values _radio_options _default _output)

	set(${_output} "" PARENT_SCOPE)

	string(REGEX MATCHALL "[^|]+" _opt_list "${_radio_options}")

	set(_count 0)
	set(_data)

	foreach(_value ${_values})
		
		list(FIND _opt_list "${_value}" _index)
		if(NOT _index EQUAL -1)
			math(EXPR _count "${_count}+1")
			set(_data "${_value}")
		endif()
	endforeach()

	if(_count EQUAL 0)
		set(${_output} ${_default} PARENT_SCOPE)
	elseif(_count EQUAL 1)
		set(${_output} ${_data} PARENT_SCOPE)
	else()
		message(FATAL_ERROR "find more radio option!!!")
	endif()

endfunction()

macro(_append_target_property _target _property _value)
	get_target_property(_org_val ${_target} ${_property})
	if(_org_val)
		set_target_properties(${_target} PROPERTIES ${_property} "${_org_val} ${_value}")
	else()
		set_target_properties(${_target} PROPERTIES ${_property} "${_value}")
	endif()
endmacro()


macro(_create_public_header_ref _f _pkgn)
	set(_pub_h "${CMAKE_BINARY_DIR}/public_header/${_pkgn}/${_f}")
	_create_include_ref("${_f}" "${_pub_h}")
endmacro()

macro(_create_include_ref _f _pub_h)
	if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${_f}")
		message(FATAL_ERROR "${_f} does not exists!")
	endif()

	if(NOT EXISTS "${_pub_h}")
		get_filename_component(_pub_d "${_pub_h}" PATH)
		file(RELATIVE_PATH _relative_path "${_pub_d}" "${CMAKE_CURRENT_SOURCE_DIR}/${_f}")
		set(_cont "// This file is created automatically, please do not modify it!\n\n#include \"${_relative_path}\"\n\n")
		file(WRITE "${_pub_h}" "${_cont}")
	endif()
endmacro()

macro(_add_compale_flags _flag)
	set(X_COMPILE_FLAGS "${X_COMPILE_FLAGS} ${_flag}")
endmacro(_add_compale_flags)

macro(_add_ignorr_warn)
	set(_warns ${ARGN})
	if(OS_WIN)
		foreach(_w ${_warns})
			_add_compale_flags(${_w})
		endforeach()
	else()
		message("todo: ignore warning")
	endif()
endmacro(_add_ignorr_warn)

macro(_global_ignore_cmp_warn)
	_add_ignorr_warn(
		/wd4819
	)
endmacro(_global_ignore_cmp_warn)

function(_x_find_env)
	if(NOT X_ENV_PATH)
		if(OS_WIN)
			get_filename_component(
				X_ENV_PATH 
				"[HKEY_CURRENT_USER\\Software\\kingsoft\\Office\\wpsenv;qt-kso-integration]" 
				ABSOLUTE 
				)
			if(NOT X_ENV_PATH)
				message(FATAL_ERROR "Can not found X_ENV_PATH!")
			endif()
			message("X_ENV_PATH path: ${X_ENV_PATH}")
			set(X_ENV_PATH "${X_ENV_PATH}" CACHE PATH "env path")
		else()
			message(FATAL_ERROR "todo: X_ENV_PATH")
		endif()
	endif()
endfunction()

function(_x_find_qt)
	if(NOT X_ENV_PATH)
		_x_find_env()
		set(_path "${X_ENV_PATH}/3rdparty/qt")
		if(EXISTS "${_path}")
			set(X_QT_PATH "${_path}" CACHE FILEPATH "qt path")
		else()
			message(FATAL_ERROR "Can not found ${_path}")
		endif()
	endif()
endfunction()

#_x_find_qt()


macro(_generate_make_bat)
	if(CMAKE_GENERATOR MATCHES "^Visual Studio")
		configure_file("${X_CMAKE_DIR}/template/make.bat.in" "${CMAKE_BINARY_DIR}/make.bat" @ONLY)
	endif()
endmacro()
