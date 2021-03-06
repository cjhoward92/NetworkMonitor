#--------------------------------------------
# SQLite build script for amalgamation
#--------------------------------------------

option(SQLITE_BUILD_STATIC  "Build SQLite static library" ON)
option(SQLITE_BUILD_SHARED  "Build SQLite shared library" ON)
option(WITH_SQLITE_DEBUG    "Build SQLite debug features" OFF)
option(WITH_SQLITE_MEMDEBUG "Build SQLite memory debug features" OFF)
option(WITH_SQLITE_RTREE    "Build R*Tree index extension" OFF)

set(SQLITE_WIN_BUILD OFF)
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(SQLITE_WIN_BUILD ON)
endif()

set(SQLITE_WINRT_BUILD OFF)
if(${CMAKE_SYSTEM_NAME} MATCHES "WindowsStore")
    set(SQLITE_WINRT_BUILD ON)
endif()

# ---------------------
# Version detection
# ---------------------
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/sqlite3.h" _sqlite_api_h_VER_STRING REGEX ".*#define[ ]+SQLITE_VERSION[ ]+")

string(REGEX MATCH "[0-9\\.]+" SQLITE_VER_STRING ${_sqlite_api_h_VER_STRING})
string(REGEX MATCHALL "[0-9]+" _sqlite_ver_LIST "${SQLITE_VER_STRING}")
list(LENGTH _sqlite_ver_LIST _sqlite_list_len)
list(GET _sqlite_ver_LIST 0 SQLITE_VER_MAJOR)
list(GET _sqlite_ver_LIST 1 SQLITE_VER_MINOR)
list(GET _sqlite_ver_LIST 2 SQLITE_VER_PATCH)
if(_sqlite_list_len EQUAL 4)
    list(GET _sqlite_ver_LIST 3 SQLITE_VER_PATCHLEVEL)
    message("Patch level: ${SQLITE_VER_PATCHLEVEL}")
endif()

message("FOUND: SQLite version = ${SQLITE_VER_STRING}")

# Use CACHE INTERNAL "" to make the variables available in other directories
set(SQLITE_DYN_NAME "${SQLITE_NAME}${SQLITE_VER_MAJOR}" CACHE INTERNAL "")
set(SQLITE_STATIC_NAME "${SQLITE_DYN_NAME}-static" CACHE INTERNAL "")

find_package(Threads REQUIRED)

# add include path for project
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(SRC_LIB_FILE ${CMAKE_CURRENT_SOURCE_DIR}/sqlite${SQLITE_VER_MAJOR}.c)
set(SRC_SHELL_FILE ${CMAKE_CURRENT_SOURCE_DIR}/shell.c)
file(GLOB HDR_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

# adding compilation flags depending on options
if(WITH_SQLITE_DEBUG)
    add_definitions(-DSQLITE_DEBUG)
endif()
if(WITH_SQLITE_MEMDEBUG)
    add_definitions(-DSQLITE_MEMDEBUG)
endif()
if(WITH_SQLITE_RTREE)
    add_definitions(-DSQLITE_ENABLE_RTREE)
endif()

if(SQLITE_WINRT_BUILD)
    # Don't use tools that are unavailable on RT platforms
    add_definitions(-DSQLITE_OS_WINRT)
endif()

if (SQLITE_BUILD_STATIC)
    # build static library
    add_library(${SQLITE_STATIC_NAME} STATIC ${SRC_LIB_FILE})
endif()

if (SQLITE_BUILD_SHARED)
    # build dynamic library
    add_library(${SQLITE_DYN_NAME} SHARED ${SRC_LIB_FILE})
    if (SQLITE_WIN_BUILD)
        #then we do dll library, so need to export api
        set_target_properties(${SQLITE_DYN_NAME} PROPERTIES DEFINE_SYMBOL "SQLITE_API=__declspec(dllexport)")
    endif()
    target_include_directories(${SQLITE_DYN_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# build shell executable
add_executable(shell ${SRC_SHELL_FILE})

if (SQLITE_BUILD_SHARED)
    # preferred is shared library
    target_link_libraries(shell ${SQLITE_DYN_NAME} ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS})
elseif(SQLITE_BUILD_STATIC)
    target_link_libraries(shell ${SQLITE_STATIC_NAME} ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS})
else()
    # no static or dynamic option selected
    message(FATAL_ERROR "either static or dynamic option should be selected")
endif()

set(SQLITE_OUT_DIR $<TARGET_FILE_DIR:${SQLITE_DYN_NAME}> CACHE INTERNAL "")