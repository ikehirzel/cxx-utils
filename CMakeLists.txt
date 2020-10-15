cmake_minimum_required(VERSION 3.9)
project(hxutils)

file(GLOB HEADERS src/*.h)
file(GLOB SOURCES src/*.cpp)

add_library(hxutils SHARED ${SOURCES})
add_library(hxutils_static STATIC ${SOURCES})

file(COPY ${HEADERS} DESTINATION include/hirzel)
