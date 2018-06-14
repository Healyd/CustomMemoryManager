#pragma once
#include "Allocator.h"
#include <vector>

namespace CustomMemoryManager::Allocators
{
#define DEBUG_EXTRA_SPACE_BYTES_POOL 300

	template <typename T>
	class PoolAllocator : public IAllocator
	{
	public:
		PoolAllocator(std::size_t poolSize_bytes, std::size_t alignment = 0U);
		PoolAllocator(const PoolAllocator&) = default;
		PoolAllocator(PoolAllocator&&) = default;
		PoolAllocator& operator=(const PoolAllocator&) = default;
		PoolAllocator& operator=(PoolAllocator&&) = default;
		~PoolAllocator() = default;

		virtual void* allocate(std::size_t numObjects) override;
		virtual void deallocate() override;

		void Clear();

	private:
		void* mPoolStart{ nullptr };
		void* mPoolCurrent{ nullptr };
		std::size_t mPoolSize_bytes{ 0U };
		std::size_t mAlignment{ 0U };

		std::vector<void*> mInUse;
		std::vector<void*> mNotInUse;
	};
}
#include "PoolAllocator.inl"