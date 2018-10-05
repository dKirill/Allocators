//
// Created by Kirill Delimbetov on 17.06.2018.
// GitHub: https://github.com/dKirill
// 
// Copyright © 2018 Kirill Delimbetov.
// 
// This source code is licensed under the license found in the LICENSE file
// in the root directory of this source tree.
//

#pragma once

//----------------------------------------------------------------------------------------
#include <algorithm>
#include <array>
#include <bitset>
#include <cstdlib>
#include <cstdint>
#include <memory>
#include <new>
#include <type_traits>
#include <vector>
//----------------------------------------------------------------------------------------
#include <Allocators/Def.hpp>
//----------------------------------------------------------------------------------------

namespace allocators {
	/************************************************************************************/
	template<typename T, size_t ObjectCount>
	struct Buffer {
		static constexpr size_t step = sizeof(T);
		
		using BufferType = std::array<uint8_t, step * ObjectCount>;
		using Bitset = std::bitset<ObjectCount>;
		
		void free(const size_t from, const size_t n) {
			DEB("from=" << from << " n=" << n);
			for(auto idx = from; idx < from + n; ++idx)
				m_occupation[idx] = false;
			
			m_firstFree = (m_firstFree > from) ? from : m_firstFree;
		}
		
		void occupy(const size_t from, const size_t n) {
			DEB("from=" << from << " n=" << n);
			for(auto idx = from; idx < from + n; ++idx)
				m_occupation[idx] = true;
			
			for(auto idx = from + n; idx < ObjectCount; ++idx) {
				if(m_occupation[idx] == false) {
					m_firstFree = idx;
					return;
				}
			}
			
			m_firstFree = ObjectCount; // past the end
		}
		
		alignas(T) BufferType m_buffer;
		Bitset m_occupation = Bitset(ObjectCount);
		size_t m_firstFree = 0;
	};
	
	/************************************************************************************/
	/************************************************************************************/
	template<typename T, size_t SingleBufferObjectCount = 32>
	class BufferizedAllocator {
	public:
		// MARK: Allocator traits
		using value_type = T;
		using propagate_on_container_copy_assignment = std::true_type;
		using propagate_on_container_move_assignment = std::true_type;
		using propagate_on_container_swap = std::true_type;
		template< class U > struct rebind { using other = BufferizedAllocator<U, SingleBufferObjectCount>; };
		// other traits are OK by default
		
		// MARK: Allocator types
		using Buffer = Buffer<T, SingleBufferObjectCount>;
		using uBuffer = std::unique_ptr<Buffer>;
		
		// MARK: Allocator methods
		/********************************************************************************/
		BufferizedAllocator()
		: BufferizedAllocator(1) {}
		
		/********************************************************************************/
		BufferizedAllocator(const size_t hint)
		: m_buffers(hint) {
			DEB("hint=" << hint);
			for(auto& pbuff : m_buffers)
				pbuff = std::make_unique<Buffer>();
			
			std::sort(m_buffers.begin(), m_buffers.end());
		}
		
		/********************************************************************************/
		[[nodiscard]] inline T* allocate(const size_t n) {
			DEB("n=" << n);
			// if requested contigious length is greater than single buffer length, throw
			if(n > SingleBufferObjectCount)
				throw std::bad_alloc();
			
			// try to find fitting place
			for(auto& buffer : m_buffers) {
				size_t freeInARow = 0;
				
				for(auto idx = buffer->m_firstFree;
					idx < SingleBufferObjectCount;
					++idx) {
					if(buffer->m_occupation[idx] == false) {
						++freeInARow;
						
						// if required space is found
						if(freeInARow == n) {
							const auto firstOccupiedIndex = idx - n + 1;
							
							// update bitset
							buffer->occupy(firstOccupiedIndex, n);
							
							return reinterpret_cast<T*>(buffer->m_buffer.data()
								+ Buffer::step * firstOccupiedIndex);
						}
					}
					else
						freeInARow = 0;
				}
			}
			
			// if there are no free memory available, add another buffer
			DEB("not enough free memory atm. need another buffer");
			auto newBuffer = std::make_unique<Buffer>();
			const auto uppBound = std::upper_bound(
				m_buffers.cbegin(),
				m_buffers.cend(),
				newBuffer);
			
			// insert it in a way that keeps m_buffers sorted
			auto inserted = m_buffers.insert(uppBound, std::move(newBuffer));
			
			// update bitset
			(*inserted)->occupy(0, n);
			
			return reinterpret_cast<T*>((*inserted)->m_buffer.data());
		}
		
		/********************************************************************************/
		inline void deallocate(T* p, const std::size_t n) noexcept {
			DEB("p=" << reinterpret_cast<uint64_t>(p) << " n=" << n);
			// first find buffer that contains p
			const auto uppBound = std::upper_bound(
				m_buffers.cbegin(),
				m_buffers.cend(),
				p,
				[](const auto& left, const auto& right) {
					return reinterpret_cast<uint8_t*>(left) < right->m_buffer.data();
				});
			const auto bufferIter = uppBound - 1;
			
			// well it shouldnt happen but this method gives no error handling options
			if(bufferIter < m_buffers.cbegin())
				return;
			
			const auto casted = reinterpret_cast<uint8_t*>(p);
			const auto diffBytes = casted - (*bufferIter)->m_buffer.data();
			const auto diffSteps = size_t(diffBytes) / Buffer::step;
			
			(*bufferIter)->free(diffSteps, n);
		}
		
		/********************************************************************************/
		void shrink() {
			DEB("");
			// remove if no element is occupied
			const auto removeFrom = std::remove_if(
				m_buffers.begin(),
				m_buffers.end(),
				[] (const auto& buff) {
					return buff->m_buffer.none();
				});
			
			m_buffers.erase(removeFrom, m_buffers.end());
		}
		
	private:
		// Have to use vector of pointers to array, because cant have pointers to objects
		// if they invalidate on vector resize.
		// Vector is sorted
		std::vector<uBuffer> m_buffers;
		// some bitmap with bit for single object block memory being used?
	};
	
	// MARK: Comparison operators
	/************************************************************************************/
	// If the types are not the same, allocators can't be equal
	template <class T, class U>
	constexpr typename std::enable_if<std::is_same<T, U>::value == false, bool>::type
	operator==(const BufferizedAllocator<T>&, const BufferizedAllocator<U>&) {
		return false;
	}

	/************************************************************************************/
	// If the types are the same, have to compare addrs
	template <class T, class U>
	inline typename std::enable_if<std::is_same<T, U>::value, bool>::type
	operator==(
		const BufferizedAllocator<T>& first,
		const BufferizedAllocator<U>& second) {
		return &first == &second;
	}
	
	/************************************************************************************/
	template <class T, class U>
	inline bool operator!=(
		const BufferizedAllocator<T>& first,
		const BufferizedAllocator<U>& second) {
		return &first != &second;
	}
}
