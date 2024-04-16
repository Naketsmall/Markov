cmake_minimum_required(VERSION 3.24)
project(CNN)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fopenmp")

find_package(MPI REQUIRED)

add_executable(CNN main.cpp Worker.cpp Worker.h MarkovChain.cpp MarkovChain.h Master.cpp Master.h resources.h)
target_link_libraries(CNN PRIVATE MPI::MPI_CXX)

