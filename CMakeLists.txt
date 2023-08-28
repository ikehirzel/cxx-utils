cmake_minimum_required(VERSION 3.9)
project("cxx-utils")

file(GLOB HEADERS src/*.hpp)
file(GLOB TEST_SOURCES src/test/*.cpp)

foreach(TEST ${TEST_SOURCES})
	get_filename_component(TARGET ${TEST} NAME_WE)
	add_executable(${TARGET} ${TEST})
	target_include_directories(${TARGET} PRIVATE "include")
	set_target_properties(${TARGET} PROPERTIES C_STANDARD 99 CXX_STANDARD 11)
endforeach()
