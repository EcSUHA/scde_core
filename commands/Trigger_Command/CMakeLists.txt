cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
cmake_policy(VERSION 2.6)

project(Trigger_Command)


add_library(Trigger_Command "Trigger_Command.c")

target_include_directories(Trigger_Command PRIVATE "include")
