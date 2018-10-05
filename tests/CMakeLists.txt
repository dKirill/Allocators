# Macro for tests addition
macro(allocators_add_test TESTNAME)
	set(_unittest_name unittest_${TESTNAME})
	add_executable(${_unittest_name} ${ARGN})
		
	target_include_directories(${_unittest_name} PRIVATE ${GTEST_INCLUDE_DIRS})
	target_link_libraries(${_unittest_name} PRIVATE ${GTEST_BOTH_LIBRARIES} Allocators)

	set_property(TARGET ${_unittest_name} PROPERTY FOLDER "tests")
endmacro()

# Add tests
allocators_add_test(
	BufferizedAllocator
	${CMAKE_CURRENT_SOURCE_DIR}/BufferizedAllocatorTest.cpp)
