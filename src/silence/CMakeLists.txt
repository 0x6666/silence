
x_package_begin(silence EXECUTABLE)
	x_package_sources(
		audio.h
		silence.cpp
		stdafx.cpp
		stdafx.h
		targetver.h
	)
	x_ignorr_warn(
		/wd4996
	)
x_package_end()
