add_executable(${PROJECT_NAME} main.c)

# add sqlite-static as a library
target_link_libraries (${PROJECT_NAME} LINK_PUBLIC ${SQLITE_DYN_NAME})