project(HLserver)

cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)

include_directories(.)

add_executable( cppserver
                main.cpp
                server/server.cpp
                utils/utils.cpp)
                
target_link_libraries(cppserver Threads::Threads)
