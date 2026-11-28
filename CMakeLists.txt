cmake_policy(VERSION 3.25.0)
cmake_minimum_required(VERSION 3.25.0)

project(ml LANGUAGES CXX)

option(ML_NO_BOOST "Disable Boost-dependent parts of ml" OFF)

option(ML_BUILD_BENCHMARKS "Build the Google Benchmark performance benchmarks" OFF)

add_library(ml INTERFACE)

include(CTest)
include(FetchContent)

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

add_compile_options(
    -Wall -Wextra -Wdouble-promotion -Wchar-subscripts
    -Wnull-dereference -Wmisleading-indentation -Wuninitialized
)

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

#
# Architecture specific settings.
#
add_library(ml_simd INTERFACE)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm64|aarch64|ARM64)$")
    message(STATUS "Detected ARM64 architecture")

    set(ML_SIMD_BACKEND NEON)
    target_compile_options(ml_simd INTERFACE
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:
        -march=armv8-a+simd
        >
    )
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64)$")
    message(STATUS "Detected x86_64 architecture")

    set(ML_SIMD_BACKEND SSE)
    target_compile_options(ml_simd INTERFACE
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:
        -msse4.1
        -mfpmath=sse
        >
    )
else()
    message(WARNING "Unknown architecture: ${CMAKE_SYSTEM_PROCESSOR}")
endif()

#
# boost
#
if(ML_NO_BOOST)
    target_compile_definitions(ml INTERFACE ML_NO_BOOST)
else()
    find_package(Boost CONFIG REQUIRED)

    if(TARGET Boost::headers)
        set(BOOST_HEADERS_TARGET Boost::headers)
    elseif(TARGET Boost::boost)
        set(BOOST_HEADERS_TARGET Boost::boost)
    else()
        message(FATAL_ERROR "Boost target is not available.")
    endif()

    set_target_properties(${BOOST_HEADERS_TARGET} PROPERTIES SYSTEM TRUE)
endif()


#
# CNL
#
FetchContent_Declare(
    cnl_src
    GIT_REPOSITORY https://github.com/johnmcfarlane/cnl.git
    GIT_TAG v1.1.7
    SOURCE_SUBDIR __unused__
    SYSTEM
)

FetchContent_MakeAvailable(cnl_src)

add_library(cnl INTERFACE)
target_include_directories(cnl SYSTEM INTERFACE
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
target_include_directories(ml SYSTEM INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(ml INTERFACE
    ml_simd
    cnl
    ${BOOST_HEADERS_TARGET}
)

#
# build tests
#

if(BUILD_TESTING)
    #
    # Boost
    #
    find_package(Boost CONFIG REQUIRED COMPONENTS unit_test_framework)

    add_executable(test_closed_interval test/test_closed_interval.cpp)
    target_link_libraries(test_closed_interval PRIVATE
        ml
        Boost::unit_test_framework
    )
    target_compile_definitions(test_closed_interval PRIVATE BOOST_TEST_DYN_LINK)
    add_test(NAME closed_interval COMMAND test_closed_interval)

    add_executable(test_cnl_support test/test_cnl_support.cpp)
    target_link_libraries(test_cnl_support PRIVATE
        ml
        Boost::unit_test_framework
    )
    target_compile_definitions(test_cnl_support PRIVATE BOOST_TEST_DYN_LINK)
    add_test(NAME cnl_support COMMAND test_cnl_support)

    add_executable(test_functions test/test_functions.cpp)
    target_link_libraries(test_functions PRIVATE
        ml
        Boost::unit_test_framework
    )
    target_compile_definitions(test_functions PRIVATE BOOST_TEST_DYN_LINK)
    add_test(NAME functions COMMAND test_functions)

    add_executable(test_vec2 test/test_vec2.cpp)
    target_link_libraries(test_vec2 PRIVATE
        ml
        Boost::unit_test_framework
    )
    target_compile_definitions(test_vec2 PRIVATE BOOST_TEST_DYN_LINK)
    add_test(NAME vec2 COMMAND test_vec2)

    add_executable(test_tvec2 test/test_tvec2.cpp)
    target_link_libraries(test_tvec2 PRIVATE
        ml
        Boost::unit_test_framework
    )
    target_compile_definitions(test_tvec2 PRIVATE BOOST_TEST_DYN_LINK)
    add_test(NAME tvec2 COMMAND test_tvec2)

    add_executable(test_vec2_fixed test/test_vec2_fixed.cpp)
    target_link_libraries(test_vec2_fixed PRIVATE
        ml
        Boost::unit_test_framework
    )
    target_compile_definitions(test_vec2_fixed PRIVATE BOOST_TEST_DYN_LINK)
    add_test(NAME vec2_fixed COMMAND test_vec2_fixed)

    add_executable(test_vec4 test/test_vec4.cpp)
    target_link_libraries(test_vec4 PRIVATE
        ml
        Boost::unit_test_framework
    )
    target_compile_definitions(test_vec4 PRIVATE BOOST_TEST_DYN_LINK)
    add_test(NAME vec4 COMMAND test_vec4)

    if(ML_SIMD_BACKEND STREQUAL "SSE" OR ML_SIMD_BACKEND STREQUAL "NEON")
        add_executable(test_vec4_simd test/test_vec4_simd.cpp)
        target_link_libraries(test_vec4_simd PRIVATE
            ml
            Boost::unit_test_framework
        )
        target_compile_definitions(test_vec4_simd PRIVATE BOOST_TEST_DYN_LINK)
        add_test(NAME vec4_simd COMMAND test_vec4_simd)
    endif()

    add_executable(test_mat3x3 test/test_mat3x3.cpp)
    target_link_libraries(test_mat3x3 PRIVATE
        ml
        Boost::unit_test_framework
    )
    target_compile_definitions(test_mat3x3 PRIVATE BOOST_TEST_DYN_LINK)
    add_test(NAME mat3x3 COMMAND test_mat3x3)

    add_executable(test_mat4x4 test/test_mat4x4.cpp)
    target_link_libraries(test_mat4x4 PRIVATE
        ml
        Boost::unit_test_framework
    )
    target_compile_definitions(test_mat4x4 PRIVATE BOOST_TEST_DYN_LINK)
    add_test(NAME mat4x4 COMMAND test_mat4x4)

    if(ML_SIMD_BACKEND STREQUAL "SSE" OR ML_SIMD_BACKEND STREQUAL "NEON")
        add_executable(test_mat4x4_simd test/test_mat4x4_simd.cpp)
        target_link_libraries(test_mat4x4_simd PRIVATE
            ml
            Boost::unit_test_framework
        )
        target_compile_definitions(test_mat4x4_simd PRIVATE BOOST_TEST_DYN_LINK)
        add_test(NAME mat4x4_simd COMMAND test_mat4x4_simd)
    endif()

endif()

#
# Benchmarks
#
if(ML_BUILD_BENCHMARKS)
    # Keep Google Benchmark from configuring its own (gtest-based) test suite.
    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Build the Google Benchmark test suite" FORCE)

    FetchContent_Declare(
        benchmark_src
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.8.3
    )
    FetchContent_MakeAvailable(benchmark_src)

    add_executable(bench_vec4 bench/bench_vec4.cpp)
    target_link_libraries(bench_vec4 PRIVATE
        ml
        benchmark::benchmark
    )

    add_executable(bench_mat4x4 bench/bench_mat4x4.cpp)
    target_link_libraries(bench_mat4x4 PRIVATE
        ml
        benchmark::benchmark
    )
endif()
