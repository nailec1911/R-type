cmake_minimum_required(VERSION 3.10)

project(Client)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ffast-math")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES "src/*.cpp" "../../gameEngine/*.cpp")

find_package(asio REQUIRED)
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

add_executable(Client ${SOURCES})
target_link_libraries(Client PRIVATE asio::asio sfml-system sfml-window sfml-graphics sfml-network sfml-audio)

