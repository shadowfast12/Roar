cmake_minimum_required(VERSION 3.26)
project(roar)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(roar
        src/main.cpp
        src/server.cpp
        src/client.cpp
)
