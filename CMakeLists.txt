cmake_minimum_required(VERSION 3.9)
project("Pluginlib")

file(COPY src/plugin.h DESTINATION include/hirzel)

# shared build
add_library(plugin SHARED "src/plugin.cpp")
# static build
add_library(plugin_static STATIC "src/plugin.cpp")
# static position independent build
add_library(plugin_staticpic STATIC "src/plugin.cpp")
set_target_properties(plugin_staticpic PROPERTIES POSITION_INDEPENDENT_CODE true)
