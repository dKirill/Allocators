//----------------------------------------------------------------------------------------
#include <map>
#include <set>
//----------------------------------------------------------------------------------------
#include <gtest/gtest.h>
//----------------------------------------------------------------------------------------
#include <Allocators/BufferizedAllocator.hpp>
//----------------------------------------------------------------------------------------

TEST(BufferizedAllocator, set) {
	std::set<int, std::less<int>, allocators::BufferizedAllocator<int>> set;
	
	for(auto idx = 0; idx < 33; ++idx)
		set.insert(idx);
	
	for(auto idx = 0; idx < 10; ++idx)
		set.erase(2 * idx);
	
	for(auto idx = 0; idx < 33; ++idx)
		set.insert(idx);
}
