//----------------------------------------------------------------------------------------
#include <map>
#include <set>
//----------------------------------------------------------------------------------------
#include <gtest/gtest.h>
//----------------------------------------------------------------------------------------
#include <Allocators/BufferizedAllocator.hpp>
//----------------------------------------------------------------------------------------

void* operator new(std::size_t sz) {
	std::printf("global op new called, size = %zu\n",sz);
	return std::malloc(sz);
}
void operator delete(void* ptr) noexcept
{
	std::puts("global op delete called");
	std::free(ptr);
}

TEST(BufferizedAllocator, set) {
	allocators::BufferizedAllocator<int> alloc;
	std::set<int, std::less<int>, allocators::BufferizedAllocator<int>> set(std::move(alloc));
//	const auto allocator = set.get_allocator();
	
	for(auto idx = 0; idx < 33; ++idx)
		set.insert(idx);
	
	for(auto idx = 0; idx < 10; ++idx)
		set.erase(2 * idx);
	
	for(auto idx = 0; idx < 33; ++idx)
		set.insert(idx);
}
