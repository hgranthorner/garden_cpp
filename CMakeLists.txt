cmake_minimum_required(VERSION 3.1)

project(fullstack)

include(ExternalProject)

ExternalProject_Add(
    client
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/client"
    PREFIX client
    INSTALL_COMMAND ""
)
ExternalProject_Add(
    server 
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/server"
    PREFIX server
    INSTALL_COMMAND ""
)

add_custom_target(compile_commands ALL
    COMMAND jq -s '.[0] + .[1]' 
            ${CMAKE_CURRENT_BINARY_DIR}/server/src/server-build/compile_commands.json
            ${CMAKE_CURRENT_BINARY_DIR}/client/src/client-build/compile_commands.json 
            > ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
    DEPENDS
            ${CMAKE_CURRENT_BINARY_DIR}/server/src/server-build/compile_commands.json
            ${CMAKE_CURRENT_BINARY_DIR}/client/src/client-build/compile_commands.json
    COMMENT "Set up compile_commands.json for clangd"
)

# Make the post_build target depend on both client and server
add_dependencies(compile_commands client server)

