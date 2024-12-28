cmake_minimum_required(VERSION 3.31)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# First define the main project
project(fullstack)

# Build the server with default toolchain
add_subdirectory(src/server)

set_target_properties(server
    PROPERTIES
    CMAKE_CURRENT_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/server"
)

if (NOT TARGET client)
    execute_process(
        COMMAND emcmake ${CMAKE_COMMAND}
            -S "${CMAKE_CURRENT_SOURCE_DIR}/src/client"
            -B "${CMAKE_CURRENT_BINARY_DIR}/client"
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -G ${CMAKE_GENERATOR}
        RESULT_VARIABLE client_config
    )

    if(client_config EQUAL 0)
        add_custom_target(client
            COMMAND ${CMAKE_COMMAND} --build "${CMAKE_CURRENT_BINARY_DIR}/client"
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/client"
            COMMENT "Building client with Emscripten"
        )
    else()
        message(WARNING "Failed to configure client build")
    endif()
endif()

add_custom_target(compile_commands ALL
    COMMAND ${CMAKE_COMMAND} -E echo "Checking for compile_commands.json files..."
    COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --green "Attempting to merge compile_commands.json files if they exist"
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/merge_compile_commands.cmake
    COMMENT "Set up compile_commands.json for clangd"
)

add_dependencies(compile_commands server client)

add_custom_target(clean_client
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/client
    COMMENT "Cleaning client project"
)
