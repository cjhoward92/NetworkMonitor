cmake_minimum_required (VERSION 3.9 FATAL_ERROR)

set(PROJECT_NAME NetworkMonitor)
set(SQLITE_NAME sqlite)
set(FINAL_BUILD_DIR "${CMAKE_SOURCE_DIR}/build/src/Debug")

# define project for C language
project(${PROJECT_NAME} C)

message("Building on: ${CMAKE_SYSTEM_NAME}")

# add in the compiler flags
# Set default compile flags for GCC
if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GCC detected, adding compile flags")
    
    # extra compiler flags
    SET(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Wextra -Werror -Wpedantic")

    # extra linker flags
    # SET(GCC_COVERAGE_LINK_FLAGS    "-lgcov")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
endif(CMAKE_COMPILER_IS_GNUCXX)

# add in the linker flags
# SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )

add_subdirectory(sqlite)
add_subdirectory(cJSON)
add_subdirectory(src)