cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0091 NEW)

# Project name
project(RtypeNetwork)

# Add subdirectories for Server and Client
add_subdirectory(server)
add_subdirectory(client)
