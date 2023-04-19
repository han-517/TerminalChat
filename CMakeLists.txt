cmake_minimum_required(VERSION 3.26)
project(TerminalChat)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(SRC_LIST net.cpp)

add_executable(Server main.cpp Server.cpp ${SRC_LIST})

target_include_directories(
    Server PUBLIC
    include)


