cmake_minimum_required(VERSION 3.0)
project(cppersist)

set(CMAKE_CXX_STANDARD 17)
set(main ${TEST})

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS} -pthread")

include_directories(crypto)

add_executable(cppersist ${sources} ${main})

###############################################################################
## CPR HTTP Request Setup #####################################################
###############################################################################
# find_package(OpenSSL REQUIRED)
# include(FetchContent)
# FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/whoshuu/cpr.git GIT_TAG c8d33915dbd88ad6c92b258869b03aba06587ff9) # the commit hash for 1.5.0
# FetchContent_MakeAvailable(cpr)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_link_libraries(cppersist PRIVATE stdc++fs)
# target_link_libraries(cppersist PRIVATE stdc++fs cpr::cpr)
else()
  target_link_libraries(cppersist)
	# target_link_libraries(cppersist PRIVATE cpr::cpr)
endif()
