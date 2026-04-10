cmake_policy(VERSION 3.22.0)
cmake_minimum_required(VERSION 3.22.0)

cmake_policy(SET CMP0167 NEW)

project(ml_tests)

include(CTest)

# Set a default build type if none was specified
set(default_build_type "Release")
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
    set(default_build_type "Debug")
endif()

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wdouble-promotion -Wchar-subscripts -Wnull-dereference -Wmisleading-indentation -Wuninitialized)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_compile_options(-stdlib=libc++)
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        add_compile_options(-Og)
    endif()
else()
    add_compile_options(-O3)
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_compile_options(-stdlib=libc++)
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        add_compile_options(-fno-omit-frame-pointer)
    endif()
endif()

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
    message(STATUS "Detected ARM64 architecture")
elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
    message(STATUS "Detected x86_64 architecture")
    add_compile_options(-msse -msse2 -msse3 -msse4 -msse4.1 -msse4.2 -mfpmath=sse)
else()
    message(WARNING "Unknown architecture: ${CMAKE_SYSTEM_PROCESSOR}")
endif()

#
# Boost
#
find_package(Boost CONFIG REQUIRED)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

#
# CNL
#
include(FetchContent)

FetchContent_Declare(
    cnl_src
    GIT_REPOSITORY https://github.com/johnmcfarlane/cnl.git
    GIT_TAG v1.1.7
    SOURCE_SUBDIR __unused__
)

FetchContent_MakeAvailable(cnl_src)

add_library(cnl INTERFACE)
target_include_directories(cnl INTERFACE
    ${cnl_src_SOURCE_DIR}/include
)
#
# Set up output directories
#
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

#
# Targets
#
add_library(ml INTERFACE)
target_include_directories(ml INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(ml INTERFACE cnl)


# add support for boost test framework
add_definitions(-DBOOST_TEST_DYN_LINK)
link_directories(${Boost_LIBRARY_DIRS})

#
# build tests
#

if(BUILD_TESTING)
    find_package(Boost CONFIG REQUIRED COMPONENTS unit_test_framework)

    add_executable(test_math test/math.cpp)
    target_link_libraries(test_math PRIVATE
        ml
        Boost::unit_test_framework
    )
    target_compile_definitions(test_math PRIVATE BOOST_TEST_DYN_LINK)
    add_test(NAME math COMMAND test_math)

    add_executable(test_cnl_support test/cnl_support.cpp)
    target_link_libraries(test_cnl_support PRIVATE
        ml
        Boost::unit_test_framework
    )
    target_compile_definitions(test_cnl_support PRIVATE BOOST_TEST_DYN_LINK)
    add_test(NAME cnl_support COMMAND test_cnl_support)

    add_executable(test_closed_interval test/closed_interval.cpp)
    target_link_libraries(test_closed_interval PRIVATE
        ml
        Boost::unit_test_framework
    )
    target_compile_definitions(test_closed_interval PRIVATE BOOST_TEST_DYN_LINK)
    add_test(NAME closed_interval COMMAND test_closed_interval)
endif()
