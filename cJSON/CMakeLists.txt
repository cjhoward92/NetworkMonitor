include(GNUInstallDirs)

set(CJSON_PROJECT_VERSION_MAJOR 1)
set(CJSON_PROJECT_VERSION_MINOR 7)
set(CJSON_PROJECT_VERSION_PATCH 5)
set(CJSON_VERSION_SO 1)
set(CJSON_UTILS_VERSION_SO 1)
set(CJSON_PROJECT_VERSION "${CJSON_PROJECT_VERSION_MAJOR}.${CJSON_PROJECT_VERSION_MINOR}.${CJSON_PROJECT_VERSION_PATCH}")


option(ENABLE_PUBLIC_SYMBOLS "Export library symbols." On)
if (ENABLE_PUBLIC_SYMBOLS)
    list(APPEND custom_compiler_flags -fvisibility=hidden)
    add_definitions(-DCJSON_EXPORT_SYMBOLS -DCJSON_API_VISIBILITY)
endif()
option(ENABLE_HIDDEN_SYMBOLS "Hide library symbols." Off)
if (ENABLE_HIDDEN_SYMBOLS)
    add_definitions(-DCJSON_HIDE_SYMBOLS -UCJSON_API_VISIBILITY)
endif()

option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(ENABLE_TARGET_EXPORT "Enable exporting of CMake targets. Disable when it causes problems!" ON)

#cJSON
set(CJSON_LIB cjson CACHE INTERNAL "")

file(GLOB HEADERS cJSON.h)
set(SOURCES cJSON.c)

option(BUILD_SHARED_AND_STATIC_LIBS "Build both shared and static libraries" Off)
option(CJSON_OVERRIDE_BUILD_SHARED_LIBS "Override BUILD_SHARED_LIBS with CJSON_BUILD_SHARED_LIBS" OFF)
option(CJSON_BUILD_SHARED_LIBS "Overrides BUILD_SHARED_LIBS if CJSON_OVERRIDE_BUILD_SHARED_LIBS is enabled" ON)

if ((CJSON_OVERRIDE_BUILD_SHARED_LIBS AND CJSON_BUILD_SHARED_LIBS) OR ((NOT CJSON_OVERRIDE_BUILD_SHARED_LIBS) AND BUILD_SHARED_LIBS))
    set(CJSON_LIBRARY_TYPE SHARED)
else()
    set(CJSON_LIBRARY_TYPE STATIC)
endif()


if (NOT BUILD_SHARED_AND_STATIC_LIBS)
    add_library("${CJSON_LIB}" "${CJSON_LIBRARY_TYPE}" "${HEADERS}" "${SOURCES}")
else()
    # See https://cmake.org/Wiki/CMake_FAQ#How_do_I_make_my_shared_and_static_libraries_have_the_same_root_name.2C_but_different_suffixes.3F
    add_library("${CJSON_LIB}" SHARED "${HEADERS}" "${SOURCES}")
    add_library("${CJSON_LIB}-static" STATIC "${HEADERS}" "${SOURCES}")
    set_target_properties("${CJSON_LIB}-static" PROPERTIES OUTPUT_NAME "${CJSON_LIB}")
    set_target_properties("${CJSON_LIB}-static" PROPERTIES PREFIX "lib")
endif()
if (NOT WIN32)
    target_link_libraries("${CJSON_LIB}" m)
endif()

set_target_properties("${CJSON_LIB}"
    PROPERTIES
        SOVERSION "${CJSON_VERSION_SO}"
        VERSION "${CJSON_PROJECT_VERSION}")

# Enable the use of locales
option(ENABLE_LOCALES "Enable the use of locales" ON)
if(ENABLE_LOCALES)
	add_definitions(-DENABLE_LOCALES)
endif()

set(CJSON_OUT_DIR $<TARGET_FILE_DIR:${CJSON_LIB}> CACHE INTERNAL "")