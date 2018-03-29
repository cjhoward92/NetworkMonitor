if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_definitions(-DWIN23)
endif()

# this needs to be crafted to work on all os
add_executable(${PROJECT_NAME} main.c win32/threading.c)

# add sqlite-static as a library
target_link_libraries (${PROJECT_NAME} LINK_PUBLIC ${SQLITE_DYN_NAME})