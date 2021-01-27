cmake_minimum_required (VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)

add_library(gtest STATIC IMPORTED)

set_target_properties(gtest PROPERTIES
  IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/libgtest.a"
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/libs/googletest/include"
)

set(test_srcs "tests.cpp")

include(FetchContent) 
FetchContent_Declare(cppersist GIT_REPOSITORY https://github.com/tallalnparis4ev/cppersist GIT_TAG master) 
FetchContent_MakeAvailable(cppersist)
include_directories(${cppersist_SOURCE_DIR}) 
#Remove the following 4 lines if the MongoDB cache is not used 
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/tallalnparis4ev/cpr.git GIT_TAG experiment) 
FetchContent_MakeAvailable(cpr) 
# find_package(OpenSSL REQUIRED) #Remove this if you're communicating with the MongoDB server via HTTP and HTTPS is not needed
# target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE cpr::cpr) 


add_executable(tests ${test_srcs})
target_link_libraries(tests gtest cpr::cpr) 