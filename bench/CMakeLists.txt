# Macro for benchs addition
macro(allocators_add_bench BENCHNAME)
	set(_bench_name bench_${BENCHNAME})
	add_executable(${_bench_name} ${ARGN})
		
	target_include_directories(${_bench_name} PRIVATE ${Benchmark_INCLUDE_DIRS})
	target_link_libraries(${_bench_name} PRIVATE ${Benchmark_BOTH_LIBRARIES} ${GTEST_LIBRARIES} Allocators)

	set_property(TARGET ${_bench_name} PROPERTY FOLDER "benchmarks")
endmacro()

# Add benchs
allocators_add_bench(
	BufferizedAllocator
	${CMAKE_CURRENT_SOURCE_DIR}/BufferizedAllocatorBench.cpp)
