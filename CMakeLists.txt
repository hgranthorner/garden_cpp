# Set the Emscripten toolchain file
set(CMAKE_TOOLCHAIN_FILE $ENV{EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake)

cmake_minimum_required(VERSION 3.31)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(emscripten_probe)

include(FetchContent)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)

FetchContent_MakeAvailable(json)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

execute_process(COMMAND em++ --cflags
    OUTPUT_VARIABLE EM_CFLAGS)

# Configure emscripten-specific options
set_target_properties(${PROJECT_NAME} PROPERTIES
    COMPILE_FLAGS "${EM_CFLAGS}"
    SUFFIX ".html"
    LINK_FLAGS "\
        -s ENVIRONMENT='web' \
        -s USE_ES6_IMPORT_META=0 \
        -s EXPORTED_RUNTIME_METHODS=['ccall','cwrap'] \
        -s EXPORTED_FUNCTIONS=['_main'] \
        -s ALLOW_MEMORY_GROWTH=1 \
        -s NO_EXIT_RUNTIME=1 \
        --shell-file ${PROJECT_SOURCE_DIR}/src/index.html \
        --bind \
        -O3"
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${PROJECT_SOURCE_DIR}/src/index.js
            ${CMAKE_BINARY_DIR}/index.js
    COMMENT "Copying index.js to build directory"
)
