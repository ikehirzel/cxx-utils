cmake_minimum_required(VERSION 3.9)
project(varlib)

add_library(var src/var.cpp)
file(COPY src/var.h DESTINATION include/hirzel)
