if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_definitions(-DWIN23)
endif()
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    add_definitions(-DLINUX)
endif()

# this needs to be crafted to work on all os
add_executable(${PROJECT_NAME} main.c win32/threading.c win32/network.c config.c utils.c cJSON.c)

# add sqlite as a library
target_link_libraries (${PROJECT_NAME} LINK_PUBLIC ${SQLITE_DYN_NAME})

# copy contents of sqlite build to src build
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD 
  COMMAND ${CMAKE_COMMAND} -E copy_directory
      ${SQLITE_OUT_DIR} $<TARGET_FILE_DIR:${PROJECT_NAME}>)