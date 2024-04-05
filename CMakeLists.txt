cmake_minimum_required(VERSION 3.24)
project(CNN)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fopenmp")
#find_package(BerkeleyDB, REQUIRED)
#find_library(BerkeleyDB HINTS "home/berkleyDB/db-6.2.32/build_unix" REQUIRED)
#include_directories(${BERKELEY_SOURCE_DIR}/inc)
#link_directories("/home/berkeleyDB/db-6.2.32/build_unix")
#add_executable(foo ${FOO_SRCS})
#include_directories("/home/berkeleyDB/db-6.2.32/build_unix")

include_directories(/db-6.2.32/build_unix/db_cxx.h)
add_library(berkeley SHARED ${PROJECT_SOURCE_DIR}/db-6.2.32/src/blob/blob_fileops.c)

add_executable(CNN main.cpp db_cxx.h)
