cmake_minimum_required(VERSION 3.9)
project(logger)

# setting up shared build
add_library(fountain SHARED src/fountain.cpp lib/var/src/var.cpp)
target_include_directories(fountain PRIVATE lib/var/build/include)

# setting up static build
add_library(fountain_static STATIC src/fountain.cpp lib/var/src/var.cpp)
target_include_directories(fountain_static PRIVATE lib/var/build/include)

# setting up static pic build
add_library(fountain_staticpic STATIC src/fountain.cpp lib/var/src/var.cpp)
set_target_properties(fountain_static PROPERTIES POSITION_INDEPENDENT_CODE true)
target_include_directories(fountain_staticpic PRIVATE lib/var/build/include)

file(COPY src/fountain.h lib/var/src/var.h DESTINATION include/hirzel)

