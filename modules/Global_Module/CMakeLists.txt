cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
cmake_policy(VERSION 2.6)

project(Global_Module)

add_library(Global_Module "GLOBAL_Module.c"
	                      "GLOBAL_readings.c")

target_include_directories(Global_Module PRIVATE "include")

