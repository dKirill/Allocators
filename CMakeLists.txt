cmake_minimum_required(VERSION 3.10)
project(Allocators)

# Settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Options
option(ALLOCATORS_BUILD_BENCH "Build benchmarks for allocators library" ON)
option(ALLOCATORS_BUILD_TEST "Build test for allocators library" ON)
option(ALLOCATORS_INSTALL "To install or not to install. Might be useful if you add this library as submodule dep" ON)

# Add library
add_library(Allocators INTERFACE)
set(ALLOCATORS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(Allocators INTERFACE ${ALLOCATORS_INCLUDE_DIR})

#  HACK: have the files showing up in IDE, since interface libs dont show up by default
file(GLOB ALLOCATORS_HEADERS ${ALLOCATORS_INCLUDE_DIR} "*.hpp")
add_custom_target(allocators_headers SOURCES ${ALLOCATORS_HEADERS})

# Find dependencies
if(ALLOCATORS_BUILD_BENCH OR ALLOCATORS_BUILD_TEST)
	find_package(GTest REQUIRED)
endif(ALLOCATORS_BUILD_BENCH OR ALLOCATORS_BUILD_TEST)

if(ALLOCATORS_BUILD_BENCH)
	find_package(Benchmark REQUIRED)
endif(ALLOCATORS_BUILD_BENCH)

# Add benchmarks conditionally
if(ALLOCATORS_BUILD_BENCH)
	# Add bench directory
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/bench)
endif(ALLOCATORS_BUILD_BENCH)

# Add tests conditionally
if(ALLOCATORS_BUILD_TEST)
	# Add test directory
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif(ALLOCATORS_BUILD_TEST)

# Conditional install
if(ALLOCATORS_INSTALL)
	install(DIRECTORY ${ALLOCATORS_INCLUDE_DIR}/Allocators
		DESTINATION include)
endif(ALLOCATORS_INSTALL)
