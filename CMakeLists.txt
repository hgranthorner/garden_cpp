cmake_minimum_required(VERSION 3.31)

project(fullstack)

include(ExternalProject)

set(BUILD_ROOT "${PROJECT_SOURCE_DIR}/build")
set(FETCHCONTENT_BASE_DIR "${BUILD_ROOT}/_deps")
set(CLIENT_ROOT "${BUILD_ROOT}/client")
set(SERVER_ROOT "${BUILD_ROOT}/server")

file(MAKE_DIRECTORY ${FETCHCONTENT_BASE_DIR})

add_subdirectory(src/client)
add_subdirectory(src/server)

add_custom_target(compile_commands ALL
    COMMAND ${CMAKE_COMMAND} -E echo "Checking for compile_commands.json files..."
    COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --green "Attempting to merge compile_commands.json files if they exist"
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/merge_compile_commands.cmake
    COMMENT "Set up compile_commands.json for clangd"
)

add_dependencies(compile_commands server client)

add_custom_target(clean_server
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/server
    COMMENT "Cleaning server project"
)

add_custom_target(clean_client
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/client
    COMMENT "Cleaning server project"
)
