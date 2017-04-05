
#cmake代码目录
set(X_CMAKE_DIR ${CMAKE_CURRENT_LIST_DIR})

# 导入参数解释函数
include(CMakeParseArguments)

include("${X_CMAKE_DIR}/internal.cmake")

# x_package_begin(<name> [STATIC|SHARED|EXECUTABLE|CONSOLE])
# 开始定义一个工程
macro(x_package_begin pkg_name)

	set(pkg_types "STATIC|SHARED|EXECUTABLE|CONSOLE")

	cmake_parse_arguments(_ARG "" "" ""  ${ARGN})
	set(_argn ${_ARG_UNPARSED_ARGUMENTS})

	_find_radio_option(${_argn} ${pkg_types} "EXECUTABLE" pkg_type)

	if("${pkg_type}" STREQUAL "")
		message(FATAL_ERROR "could not find the package type")
	endif()

	set(X_PACKAGE_NAME "${pkg_name}")
	set(X_PACKAGE_TYPE "${pkg_type}")
	set(X_EXECUTABLE_ENTRYPOINT "main")
	set(X_HAS_PUBLIC_HEADER)
	set(X_PACKAGE_SOURCE_FILES)

	# c++11
	if(${OS_LINUX})
		add_compile_options(-std=c++11)
	endif()

	include_directories(${CMAKE_CURRENT_BINARY_DIR})
	include_directories("${CMAKE_BINARY_DIR}/public_header")

endmacro(x_package_begin)

# 结束一个工程的定义
macro(x_package_end)

	set(_src_to_compile ${X_PACKAGE_SOURCE_FILES})

	if("${X_PACKAGE_TYPE}!" STREQUAL "STATIC!")
		add_library(${X_PACKAGE_NAME} STATIC ${_src_to_compile})
	elseif("${X_PACKAGE_TYPE}!" STREQUAL "SHARED!")
		add_library(${X_PACKAGE_NAME} SHARED ${_src_to_compile})
	elseif("${X_PACKAGE_TYPE}!" STREQUAL "EXECUTABLE!")
		if(OS_WIN)
			add_executable(${X_PACKAGE_NAME} WIN32 ${_src_to_compile})
			if(X_EXECUTABLE_ENTRYPOINT)
				_append_target_property(${X_PACKAGE_NAME} LINK_FLAGS " /ENTRY:\"${X_EXECUTABLE_ENTRYPOINT}CRTStartup\" ")
			endif()
		else()
			add_executable(${X_PACKAGE_NAME} ${_src_to_compile})
		endif()
	elseif("${X_PACKAGE_TYPE}!" STREQUAL "CONSOLE!")
		add_executable(${X_PACKAGE_NAME} ${_src_to_compile})
	else()
		message(FATAL_ERROR "invalid package type")
	endif()

	set_target_properties(${X_PACKAGE_NAME} PROPERTIES COMPILE_FLAGS "/wd4819")

endmacro(x_package_end)

# x_package_sources([PCH header.h source.cpp] a.h b.c c.cpp ...)
# 定义工程源代码文件
macro(x_package_sources)
	set(_source_files ${ARGN})
	foreach(_f ${_source_files})
		if(_f MATCHES "(PCH)")
		else()
			string(TOLOWER "${_f}" _f)
			if(_f MATCHES "(\\.c|\\.cpp|\\.h|\\.rc|\\.def)")
			else()
				message(FATAL_ERROR "Unrecognized source file ${_f}")
			endif()
		endif()
	endforeach()
	list(APPEND X_PACKAGE_SOURCE_FILES ${_source_files})
endmacro(x_package_sources)


macro(x_public_header)
	foreach(_f ${ARGN})
		_create_public_header_ref(${_f} ${X_PACKAGE_NAME})
	endforeach()
endmacro()

# x_extren_package(_pkg_name _pkg_type
#			<LOCATION _location>
# )
macro(x_extren_package _pkg_name _pkg_type)
	set(_args ${ARGN})
	cmake_parse_arguments(
		X_EXT_PKG
		""
		"LOCATION;IMPT_LOCATION"
		"PUBLIC_HEADER;DEPENDS;BINARY_NAMES;BINARY_NAMES_DEBUG;BINARY_NAMES_RELEASE"
		${_args}
	)

	# output dir
	set(_outputdir "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

	# import location
	if(NOT X_EXT_PKG_IMPT_LOCATION)
		set(X_EXT_PKG_IMPT_LOCATION ${X_EXT_PKG_LOCATION})
	endif()

	# file names
	if(X_EXT_PKG_BINARY_NAMES)
		set(_names ${X_EXT_PKG_BINARY_NAMES})
	else()
		set(_names ${X_EXT_PKG_BINARY_NAMES_${X_BUILD_CFG}})
	endif()

	set(_output_files)
	foreach(_name ${_names})
		if(OS_WIN)
			set(_model_path "${X_EXT_PKG_LOCATION}/${_name}.dll")
			set(_lib "${X_EXT_PKG_IMPT_LOCATION}/${_name}.lib")
		elseif(OS_LINUX)
			set(_so_file_name "lib${_name}.so")
			set(_model_path "${X_EXT_PKG_LOCATION}/${_so_file_name}")
			set(_lib ${_model_path})
		else()
			message(FATAL_ERROR "invalid os")
		endif()

		if("${_pkg_type}!" STREQUAL "SHARED!")
			if(NOT EXISTS ${_outputdir})
				file(MAKE_DIRECTORY "${_outputdir}")
			endif()

			get_filename_component(_filename "${_model_path}" NAME)

			if(OS_WIN)
				set(_cp_command COMMAND ${CMAKE_COMMAND} -E copy ${_model_path} ${_outputdir}/${_filename})
			else()
				set(_cp_command COMMAND cp -R ${_model_path}* ${_outputdir})
			endif()

			add_custom_command(
				OUTPUT ${_outputdir}/${_filename}
				${_cp_command}
				WORKING_DIRECTORY ${_package_destdir}
				MAIN_DEPENDENCY "${_model_path}"
			)

			list(APPEND _output_files ${_outputdir}/${_filename})
		endif()
	endforeach()

	# package type
	if("${_pkg_type}!" STREQUAL "STATIC!")
		add_custom_target(${_pkg_name})
	elseif("${_pkg_type}!" STREQUAL "SHARED!")
		add_custom_target(${_pkg_name} ALL DEPENDS ${_output_files})
	else()
		message(FATAL_ERROR "${_pkg_type} is not a valid ext pkg type")
	endif()

	# dependent
	if(X_EXT_PKG_DEPENDS)
		add_dependencies(${_pkg_name} ${X_EXT_PKG_DEPENDS})
	endif()

	# public header
	if(X_EXT_PKG_PUBLIC_HEADER)
		set_property(TARGET ${_pkg_name} PROPERTY X_HEADER_DIR "${X_EXT_PKG_PUBLIC_HEADER}")
	endif()
	set_property(TARGET ${_pkg_name} PROPERTY X_IMPT_LOCATION "${_model_path}")
	set_property(TARGET ${_pkg_name} PROPERTY X_IMPT_IMPLIB "${_lib}")
	set_property(TARGET ${_pkg_name} PROPERTY X_PACKAGE_TYPE "EXTERN")

endmacro(x_extren_package)
