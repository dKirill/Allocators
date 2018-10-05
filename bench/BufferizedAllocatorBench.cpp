//----------------------------------------------------------------------------------------
#include <cmath>
#include <cstdint>
#include <map>
#include <set>
#include <utility>
//----------------------------------------------------------------------------------------
#include <benchmark/benchmark.h>
//----------------------------------------------------------------------------------------
#include <Allocators/BufferizedAllocator.hpp>
//----------------------------------------------------------------------------------------

// MARK: Common
struct Data {
	Data(uint64_t idx)
	: m_some1(idx) {}
	
	uint64_t m_some1;
	uint64_t m_some2;
	double m_some3;
	float m_some4;
	char m_some5[10];
	bool m_some6;
};

bool operator<(const Data& left, const Data& right) noexcept {
	return left.m_some1 < right.m_some1;
}

// MARK: map
template<typename T, class Alloc>
static void stdMap(benchmark::State& state) {
	for(auto _ : state) {
		std::map<int32_t, T, std::less<int32_t>, Alloc> map;
		
		for(auto sameMapIter = 0; sameMapIter < state.range(1); ++sameMapIter) {
			for(auto idx = 0; idx < state.range(0); ++idx)
				benchmark::DoNotOptimize(map.emplace(idx, T(idx)).first);
			
			benchmark::ClobberMemory();
			
			for(auto idx = 0; idx < state.range(0); ++idx)
				map.erase(idx);
			
			benchmark::ClobberMemory();
		}
	}
}

BENCHMARK_TEMPLATE(stdMap, int32_t, std::allocator<std::pair<const int32_t, int32_t>>)
	->Ranges({{8, 8 << 10}, {1, 2 << 8}});
BENCHMARK_TEMPLATE(stdMap, int32_t, allocators::BufferizedAllocator<std::pair<const int32_t, int32_t>, 32>)
	->Ranges({{8, 8 << 10}, {1, 2 << 8}});
BENCHMARK_TEMPLATE(stdMap, int32_t, allocators::BufferizedAllocator<std::pair<const int32_t, int32_t>, 512>)
	->Ranges({{8, 8 << 10}, {1, 2 << 8}});
BENCHMARK_TEMPLATE(stdMap, int32_t, allocators::BufferizedAllocator<std::pair<const int32_t, int32_t>, 2048 << 3>)
	->Ranges({{8, 8 << 10}, {1, 2 << 8}});


BENCHMARK_TEMPLATE(stdMap, Data, std::allocator<std::pair<const int32_t, Data>>)
	->Ranges({{8, 8 << 10}, {1, 2 << 8}});
BENCHMARK_TEMPLATE(stdMap, Data, allocators::BufferizedAllocator<std::pair<const int32_t, Data>, 32>)
	->Ranges({{8, 8 << 10}, {1, 2 << 8}});
BENCHMARK_TEMPLATE(stdMap, Data, allocators::BufferizedAllocator<std::pair<const int32_t, Data>, 512>)
	->Ranges({{8, 8 << 10}, {1, 2 << 8}});
BENCHMARK_TEMPLATE(stdMap, Data, allocators::BufferizedAllocator<std::pair<const int32_t, Data>, 2048 << 3>)
	->Ranges({{8, 8 << 10}, {1, 2 << 8}});

// MARK: set
template<typename T, class Alloc>
static void stdSet(benchmark::State& state) {
	for(auto _ : state) {
		std::set<T, std::less<T>, Alloc> set;
		
		for(auto sameSetIter = 0; sameSetIter < state.range(1); ++sameSetIter) {
			for(auto idx = 0; idx < state.range(0); ++idx)
				benchmark::DoNotOptimize(set.emplace(idx).first);
			
			benchmark::ClobberMemory();
			
			for(auto idx = 0; idx < state.range(0); ++idx)
				set.erase(T(idx));
			
			benchmark::ClobberMemory();
		}
	}
}

BENCHMARK_TEMPLATE(stdSet, int32_t, std::allocator<int32_t>)
	->Ranges({{8, 8 << 10}, {1, 2 << 8}});
BENCHMARK_TEMPLATE(stdSet, int32_t, allocators::BufferizedAllocator<int32_t, 32>)
	->Ranges({{8, 8 << 10}, {1, 2 << 8}});
BENCHMARK_TEMPLATE(stdSet, int32_t, allocators::BufferizedAllocator<int32_t, 512>)
	->Ranges({{8, 8 << 10}, {1, 2 << 8}});
BENCHMARK_TEMPLATE(stdSet, int32_t, allocators::BufferizedAllocator<int32_t, 2048 << 3>)
	->Ranges({{8, 8 << 10}, {1, 2 << 8}});

BENCHMARK_TEMPLATE(stdSet, Data, std::allocator<Data>)
	->Ranges({{8, 8 << 10}, {1, 2 << 8}});
BENCHMARK_TEMPLATE(stdSet, Data, allocators::BufferizedAllocator<Data, 32>)
	->Ranges({{8, 8 << 10}, {1, 2 << 8}});
BENCHMARK_TEMPLATE(stdSet, Data, allocators::BufferizedAllocator<Data, 512>)
	->Ranges({{8, 8 << 10}, {1, 2 << 8}});
BENCHMARK_TEMPLATE(stdSet, Data, allocators::BufferizedAllocator<Data, 2048 << 3>)
	->Ranges({{8, 8 << 10}, {1, 2 << 8}});
