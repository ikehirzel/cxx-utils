cmake_minimum_required(VERSION 3.16)
project("cxx-utils")

file(GLOB TEST_SOURCES "src/*.cpp")

if(WIN32)
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

if(MSVC)
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
	add_compile_options(/W4 /WX)
else()
	add_compile_options(${TARGET} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()


foreach(TEST ${TEST_SOURCES})
	get_filename_component(TARGET ${TEST} NAME_WE)
	add_executable(${TARGET} ${TEST})
	target_include_directories(${TARGET} PRIVATE "include")
	set_target_properties(${TARGET} PROPERTIES CXX_STANDARD 17)
endforeach()
