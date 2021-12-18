cmake_minimum_required(VERSION 3.9)
project("cxx-utils")

file(GLOB HEADERS src/*.h)
file(GLOB TEST_SOURCES src/test/*.cpp)

foreach(TEST ${TEST_SOURCES})
	get_filename_component(TARGET ${TEST} NAME_WE)
	add_executable(${TARGET} ${TEST})
	target_include_directories(${TARGET} PRIVATE "include")
endforeach()
