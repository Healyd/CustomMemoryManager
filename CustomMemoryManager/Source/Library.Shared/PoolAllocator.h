#pragma once
#include "Allocator.h"
#include <vector>
#include <queue>

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
		virtual void deallocate(void* ptr) override;

		void Clear();

		std::size_t PoolSize_Bytes() const;
		std::size_t UsedSpace_Bytes() const;
		std::size_t PoolSize_NumObjects() const;

	private:
		void* mPoolStart{ nullptr };
		void* mPoolCurrent{ nullptr };
		std::size_t mPoolSize_bytes{ 0U };
		std::size_t mAlignment{ 0U };

		std::vector<void*> mInUse;		// pointers to memory in the pool that are available
		std::vector<void*> mNotInUse;	// pointers to memory in the pool that are not available

		struct chunk
		{
			void* current{ nullptr };
			std::size_t chunkSize{ 0U };

			void* start{ nullptr };
			void* end{ nullptr };
			std::size_t chunkSize_bytes{ 0U };
			std::size_t chunkSize_objects{ 0U };
		};
		std::queue<chunk> mNotInUseChunks;

		std::queue<chunk> mInUse2;
		std::queue<chunk> mNotInUse2;
	};
}
#include "PoolAllocator.inl"