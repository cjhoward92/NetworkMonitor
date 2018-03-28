cmake_minimum_required (VERSION 3.9 FATAL_ERROR)

set(PROJECT_NAME NetworkMonitor)
set(SQLITE_NAME sqlite)

# define project for C language
project(${PROJECT_NAME} C)

message("Building on: ${CMAKE_SYSTEM_NAME}")

add_subdirectory(sqlite)
add_subdirectory(src)