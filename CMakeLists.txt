cmake_minimum_required(VERSION 3.26)
project(roar)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(roar
        main.cpp
        server.h
        server.cpp
        client.h
        client.cpp
        server.h
)
