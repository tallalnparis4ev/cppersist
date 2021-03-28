cmake_minimum_required(VERSION 3.1)
project(cppersist)

set(CMAKE_CXX_STANDARD 17)
set(main ${TEST})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(cppersist ${main})

# ###############################################################################
# ## CPR HTTP Request Setup #####################################################
# ###############################################################################
include(FetchContent) 
FetchContent_Declare(cppersist GIT_REPOSITORY https://github.com/tallalnparis4ev/cppersist GIT_TAG master) 
FetchContent_MakeAvailable(cppersist)
include_directories(${cppersist_SOURCE_DIR}) 

if(HTTPS OR HTTP)
  FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/tallalnparis4ev/cpr.git GIT_TAG experiment) 
  FetchContent_MakeAvailable(cpr)
  set(CPR "cpr::cpr")
endif()

if(HTTPS) 
  find_package(OpenSSL REQUIRED)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(FS "stdc++fs")
endif()

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Threads::Threads ${CPR} ${FS}) 






