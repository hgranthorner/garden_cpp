cmake_minimum_required(VERSION 3.31)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(client)

include(FetchContent)

FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)

FetchContent_Declare(
    fmt
    URL https://github.com/fmtlib/fmt/archive/refs/tags/11.1.1.tar.gz
)

FetchContent_MakeAvailable(json fmt)

add_executable(${PROJECT_NAME} 
main.cpp
document.cpp
)

add_subdirectory(../shared shared)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json shared fmt::fmt)

execute_process(COMMAND em++ --cflags
    OUTPUT_VARIABLE EM_CFLAGS)


set(COMPILER_FLAGS "-Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wunused -Wold-style-cast -Wsign-conversion -Wnull-dereference -fno-common -fsanitize=address -fsanitize=undefined -fsanitize=leak -fno-omit-frame-pointer -Wno-import-preprocessor-directive-pedantic")

set(LINKER_FLAGS "-fsanitize=address -fsanitize=undefined")

# Configure emscripten-specific options
set_target_properties(${PROJECT_NAME} PROPERTIES
    COMPILE_FLAGS "${COMPILER_FLAGS} ${EM_CFLAGS}"
    SUFFIX ".html"
    LINK_FLAGS "\
        -sASSERTIONS=2           # Enable runtime checks \
        -sSAFE_HEAP=1           # Add memory access checks \
        -sSTACK_OVERFLOW_CHECK=2 # Add stack overflow checks \
        -fsanitize=address \
        -s ENVIRONMENT='web' \
        -s USE_ES6_IMPORT_META=0 \
        -s EXPORTED_RUNTIME_METHODS=['ccall','cwrap'] \
        -s EXPORTED_FUNCTIONS=['_main'] \
        -s ALLOW_MEMORY_GROWTH=1 \
        -s NO_EXIT_RUNTIME=1 \
        -s FETCH \
        --shell-file ${CMAKE_CURRENT_SOURCE_DIR}/index.html \
        --bind \
        -O0"
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/index.js
            ${CMAKE_BINARY_DIR}/index.js
    COMMENT "Copying index.js to build directory"
)

