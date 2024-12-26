cmake_minimum_required(VERSION 3.31)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(server)

include(FetchContent)

FetchContent_Declare(
	asio
	GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
	GIT_TAG asio-1-32-0
)

FetchContent_Declare(
    crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
	GIT_TAG v1.2.0
)

FetchContent_MakeAvailable(asio)

set(ASIO_INCLUDE_DIR ${asio_SOURCE_DIR}/asio/include)
message(${ASIO_INCLUDE_DIR})

FetchContent_MakeAvailable(crow)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} INTERFACE ${ASIO_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE Crow::Crow)

