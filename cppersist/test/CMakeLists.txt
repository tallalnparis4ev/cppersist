cmake_minimum_required(VERSION 3.15.2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add gtest lib
add_library(gtest STATIC IMPORTED)

set_target_properties(gtest PROPERTIES
  IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/libgtest.a"
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/libs/googletest/include"
)

project(cppersist)
set(test_srcs "tests.cpp")

#Get cppersist 
include(FetchContent)
FetchContent_Declare(cppersist GIT_REPOSITORY https://github.com/tallalnparis4ev/cppersist GIT_TAG master)
FetchContent_MakeAvailable(cppersist)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${cppersist_SOURCE_DIR}/cppersist/cmake)

#Link threading and filesystem libraries
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(Filesystem REQUIRED)

include_directories(${cppersist_SOURCE_DIR})

#These tests need CPR
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/tallalnparis4ev/cpr.git GIT_TAG stable)
FetchContent_MakeAvailable(cpr)
set(CPR "cpr::cpr")

add_executable(tests ${test_srcs})
target_link_libraries(tests PRIVATE gtest Threads::Threads std::filesystem ${CPR}) 


