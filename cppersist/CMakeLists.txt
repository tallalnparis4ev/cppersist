cmake_minimum_required(VERSION 3.15.2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#========= Change these lines for your project
project(cppersist)
add_executable(${CMAKE_PROJECT_NAME} ${TEST})
#=============================================

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

#Get cpr if needed
if(MONGO)
  FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/tallalnparis4ev/cpr.git GIT_TAG stable)
  FetchContent_MakeAvailable(cpr)
  set(CPR "cpr::cpr")
endif()
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Threads::Threads std::filesystem ${CPR})


