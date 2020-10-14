cmake_minimum_required(VERSION 3.9)
project(logger)

add_library(fountain SHARED src/fountain.cpp)
add_library(fountain_static STATIC src/fountain.cpp)

file(COPY src/fountain.h DESTINATION include/hirzel)

