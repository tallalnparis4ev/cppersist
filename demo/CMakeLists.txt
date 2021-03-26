cmake_minimum_required (VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)

project (demo)

add_executable(${CMAKE_PROJECT_NAME} fib.cpp)

include(FetchContent) 
FetchContent_Declare(cppersist GIT_REPOSITORY https://github.com/tallalnparis4ev/cppersist GIT_TAG master) 
FetchContent_MakeAvailable(cppersist)
include_directories(${cppersist_SOURCE_DIR}) 
#Remove the following 4 lines if the MongoDB cache is not used 
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/tallalnparis4ev/cpr.git GIT_TAG experiment) 
FetchContent_MakeAvailable(cpr) 
# find_package(OpenSSL REQUIRED) #Remove this if you're communicating with the MongoDB server via HTTP and HTTPS is not needed
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE cpr::cpr) 
