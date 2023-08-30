cmake_minimum_required(VERSION 3.16)
project("cxx-utils")

if(WIN32)
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

if(MSVC)
	add_compile_options(/W4 /WX)
else()
	add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

file(GLOB COMMON_SOURCES "src/hirzel/*.cpp")
add_library(common OBJECT ${COMMON_SOURCES})
list(APPEND TARGETS common)

file(GLOB TEST_SOURCES "src/*.cpp")
foreach(TEST ${TEST_SOURCES})
	get_filename_component(TARGET ${TEST} NAME_WE)
	add_executable(${TARGET} ${TEST} $<TARGET_OBJECTS:common>)
	list(APPEND TARGETS ${TARGET})
endforeach()

foreach(TARGET ${TARGETS})
	target_include_directories(${TARGET} PRIVATE "include")
	set_target_properties(${TARGET} PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS OFF
	)
endforeach()
