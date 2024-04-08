cmake_minimum_required(VERSION 3.24)
project(CNN)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fopenmp")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -libmongocxx")
#include_directories(/db-6.2.32/build_unix/)

#find_package(libmongocxx REQUIRED)
#find_package(libbsoncxx REQUIRED)
#include_directories(${LIBMONGOCXX_INCLUDE_DIR})
#include_directories(${LIBBSONCXX_INCLUDE_DIR})
#include_directories("/home/epsilon/mongo-cxx-driver-r3.10.1/build/src/mongocxx/v_noabi")
#include_directories("/home/epsilon/mongo-cxx-driver-r3.10.1/build/src/bsoncxx/v_noabi")
#include_directories("/home/epsilon/mongo-cxx-driver-r3.10.1/build/src/libmongoc-1.0")
#include_directories("/usr/local/include/libbson-1.0")
#include_directories("/usr/local/lib")

find_package(MPI REQUIRED)

add_executable(CNN main.cpp Worker.cpp Worker.h)
target_link_libraries(CNN PRIVATE MPI::MPI_CXX)

#add_custom_command(
 #       OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/CNN.out
  #      COMMAND MakeTable ${CMAKE_CURRENT_BINARY_DIR}/CNN.out
   #     DEPENDS MakeTable
#)

#target_link_libraries(CNN ${LIBMONGOCXX_LIBRARIES})
#target_link_libraries(CNN ${LIBBSONCXX_LIBRARIES})


#find_library(${PROJECT_SOURCE_DIR}/db-6.2.32/build_unix/.libs)
#find_library(BerkelyDB
 #       NAMES libdb_cxx-6.2
  #      HINTS "${PROJECT_SOURCE_DIR}/db-6.2.32/build_unix/.libs"
   #     )
#target_link_libraries(CNN libdb-6.2)
