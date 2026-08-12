cmake_minimum_required(VERSION 3.16)
project(OpenScale VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(OPENSCALE_BUILD_TESTS "Build tests" ON)
option(OPENSCALE_BUILD_EXAMPLES "Build examples" ON)
option(OPENSCALE_BUILD_TOOLS "Build tools" ON)

add_library(openscale STATIC
    src/tensor.cpp
    src/memory.cpp
    src/device.cpp
    src/scheduler.cpp
    src/profiler.cpp
    src/kernels.cpp
    src/mmap.cpp
    src/runtime.cpp
)

target_include_directories(openscale PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_definitions(openscale PRIVATE OPENSCALE_VERSION="0.1.0")

if(MSVC)
    target_compile_options(openscale PRIVATE /O2 /EHsc)
else()
    target_compile_options(openscale PRIVATE -O3 -ffast-math)
endif()

if(OPENSCALE_BUILD_EXAMPLES)
    add_executable(openscale_demo examples/basic.cpp)
    target_link_libraries(openscale_demo PRIVATE openscale)
endif()

if(OPENSCALE_BUILD_TOOLS)
    add_executable(openscale-benchmark tools/benchmark.cpp)
    target_link_libraries(openscale-benchmark PRIVATE openscale)
endif()

if(OPENSCALE_BUILD_TESTS)
    enable_testing()
    add_executable(openscale_tests tests/test_runtime.cpp)
    target_link_libraries(openscale_tests PRIVATE openscale)
    add_test(NAME OpenScaleRuntime COMMAND openscale_tests)
endif()
