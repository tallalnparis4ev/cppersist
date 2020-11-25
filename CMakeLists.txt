# replace PROJECT_NAME by the name of your clion project

cmake_minimum_required(VERSION 3.0)
project(cppersist)

set(CMAKE_CXX_STANDARD 17)
set(main toy.cpp)

include_directories(crypto)
set(sources cache.hpp diskcache.hpp mongodbcache.hpp persister.hpp crypto/sha256.cpp)

add_executable(cppersist ${sources} ${main})

###############################################################################
## CPR HTTP Request Setup #####################################################
###############################################################################
find_package(OpenSSL REQUIRED)
include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/whoshuu/cpr.git GIT_TAG c8d33915dbd88ad6c92b258869b03aba06587ff9) # the commit hash for 1.5.0
FetchContent_MakeAvailable(cpr)

target_link_libraries(cppersist PRIVATE cpr::cpr)