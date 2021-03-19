cmake_policy(VERSION 3.6.0)
cmake_minimum_required (VERSION 3.6.0)

project (ml_tests)

set(CMAKE_BUILD_TYPE Release)

#
# Enable C++14, SSE and optimize code.
#
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -stdlib=libc++ -Wall -O3 -msse -msse2 -msse3 -msse4 -msse4.1 -msse4.2 -mfpmath=sse")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -O2 -msse -msse2 -msse3 -msse4 -msse4.1 -msse4.2 -mfpmath=sse")
endif()

#
# Set up output directories
#
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin )

#
# Set up project include directories
#
include_directories(include ../3rd-party/cnl/include)

# add support for boost test framework
add_definitions(-DBOOST_TEST_DYN_LINK) 
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
link_directories ( ${Boost_LIBRARY_DIRS} )

#
# build tests
#

add_executable(test_math.linux test/math.cpp)
target_link_libraries(test_math.linux
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    )

add_executable(test_cnl_support.linux test/cnl_support.cpp)
target_link_libraries(test_cnl_support.linux 
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    )

add_executable(test_closed_interval.linux test/closed_interval.cpp)
target_link_libraries(test_closed_interval.linux 
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    )
