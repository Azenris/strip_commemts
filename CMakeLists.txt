
cmake_minimum_required( VERSION 3.26.3 )

set( APP_NAME strip_comments )

project(
	${APP_NAME}
	VERSION 1.0
	LANGUAGES CXX
)

option( CMAKE_VERBOSE_MAKEFILE "" OFF )

add_executable( strip_comments src/main.cpp )

target_compile_definitions( strip_comments PRIVATE "$<$<CONFIG:Debug>:DEBUG>$<$<CONFIG:Release>:NDEBUG>" )
target_compile_definitions( strip_comments PRIVATE -DBUILD_TYPE="$<$<CONFIG:Debug>:DEBUG>$<$<CONFIG:Release>:RELEASE>" )
target_compile_features( strip_comments PRIVATE cxx_std_20 )

target_include_directories( strip_comments PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/" )
target_include_directories( strip_comments PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/assets/" )
target_include_directories( strip_comments PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/third_party/" )

if ( MSVC )
	message( STATUS "MSVC Build" )

	target_compile_definitions( strip_comments PRIVATE -D_CRT_SECURE_NO_WARNINGS )
	target_compile_definitions( strip_comments PRIVATE -D__PLATFORM_WINDOWS__ )

	# 4189 : local variable is initialized but not referenced
	# 4201 : nonstandard extension used: nameless struct/union
	# 4324 : structure was padded due to alignment specifier
	# 4505 : unreferenced function with internal linkage has been removed

	target_compile_options( strip_comments PRIVATE -WX -W4 -wd4189 -wd4201 -wd4324 -wd4505 -Zc:preprocessor )
	target_compile_options( strip_comments PRIVATE $<$<CONFIG:Debug>:-Z7 -FC> )
	target_compile_options( strip_comments PRIVATE $<$<CONFIG:Release>:-O2 -Ot -GF> )

	add_link_options( platform PRIVATE "/SUBSYSTEM:CONSOLE" )

	set( outputDirectory "${CMAKE_CURRENT_SOURCE_DIR}/final/$<$<CONFIG:Debug>:debug>$<$<CONFIG:Release>:release>" )

	set_target_properties(
		strip_comments
		PROPERTIES
		OUTPUT_NAME                      "strip_comments"
		RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${outputDirectory}/"
		RUNTIME_OUTPUT_DIRECTORY_RELEASE "${outputDirectory}/"
		ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${outputDirectory}/"
		ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${outputDirectory}/"
		LIBRARY_OUTPUT_DIRECTORY_DEBUG   "${outputDirectory}/"
		LIBRARY_OUTPUT_DIRECTORY_RELEASE "${outputDirectory}/"
	)
endif()
