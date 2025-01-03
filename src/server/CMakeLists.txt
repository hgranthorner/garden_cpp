cmake_minimum_required(VERSION 3.31)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(server)

include(FetchContent)

FetchContent_Declare(
    asio
    URL https://github.com/chriskohlhoff/asio/archive/refs/tags/asio-1-32-0.zip
)

FetchContent_Declare(
    crow
    URL https://github.com/CrowCpp/Crow/archive/refs/tags/v1.2.0.tar.gz
)

FetchContent_Declare(
    SQLiteCpp
    URL https://github.com/SRombauts/SQLiteCpp/archive/refs/tags/3.3.2.tar.gz
)

FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)

FetchContent_Declare(
    rapidfuzz
    URL https://github.com/rapidfuzz/rapidfuzz-cpp/archive/refs/tags/v3.2.0.tar.gz
)

FetchContent_MakeAvailable(asio SQLiteCpp json rapidfuzz)

set(ASIO_INCLUDE_DIR ${asio_SOURCE_DIR}/asio/include)

FetchContent_MakeAvailable(crow)

add_executable(${PROJECT_NAME} main.cpp match.cpp db.cpp)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Wshadow
    -Wunused
    -Wold-style-cast
    -Wsign-conversion
    -Wnull-dereference
    -fno-common
    -fsanitize=address
    -fsanitize=undefined
    -fno-omit-frame-pointer
    -Wno-import-preprocessor-directive-pedantic
)

target_link_options(${PROJECT_NAME} PRIVATE
    -fsanitize=address
    -fsanitize=undefined
)

target_include_directories(${PROJECT_NAME} 
INTERFACE
${ASIO_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
Crow::Crow
SQLiteCpp
nlohmann_json::nlohmann_json
rapidfuzz::rapidfuzz
shared
)

