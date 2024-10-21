cmake_minimum_required(VERSION 3.15)
project(Server)

# Set CMake output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Default to Release build if no build type is specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Platform-specific settings
if (WIN32)
  # Windows-specific compiler flags
  add_definitions(-D_WIN32_WINNT=0x0A00)  # Enable modern Windows features, adjust as necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")  # MSVC equivalent of -Wall -Wextra
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")  # MSVC Release optimization
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi /Od")  # Debug info and no optimization
else()
  # Linux/Unix compiler flags
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -ffast-math")
endif()

# Include source files
file(GLOB_RECURSE SOURCES "src/*.cpp" "../../gameEngine/RTypeGame.cpp")

# Find and link libraries
find_package(asio REQUIRED)
find_package(yaml-cpp REQUIRED)

# Create executable target
add_executable(Server ${SOURCES})

# Link external libraries
target_link_libraries(Server PRIVATE asio::asio yaml-cpp)

# MSVC-specific settings: set output to the correct folder
if (MSVC)
  # Use multithreaded runtime
  target_compile_options(Server PRIVATE /MT)  # or /MD for dynamic CRT
  # Set C++ standard for MSVC
  target_compile_features(Server PRIVATE cxx_std_20)
endif()
