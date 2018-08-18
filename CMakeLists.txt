cmake_minimum_required (VERSION 3.9 FATAL_ERROR)

set(PROJECT_NAME NetworkMonitor)
set(SQLITE_NAME sqlite)
set(FINAL_BUILD_DIR "${CMAKE_SOURCE_DIR}/build/src/Debug")

# define project for C language
project(${PROJECT_NAME} C)

message("Building on: ${CMAKE_SYSTEM_NAME}")
message("Building with ${CMAKE_C_COMPILER_ID}")

# add in the compiler flags
# Set default compile flags for GCC
if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    message(STATUS "GCC detected, adding compile flags")
    
    add_definitions(-D_GNU_SOURCE)
    add_definitions(-D_POSIX_SOURCE)

    # extra compiler flags
    SET(GCC_COVERAGE_COMPILE_FLAGS "-pthread -std=c11")

    # extra linker flags
    # SET(GCC_COVERAGE_LINK_FLAGS    "-lgcov")

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
endif()

# add in the linker flags
# SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )

add_subdirectory(sqlite)
add_subdirectory(src)