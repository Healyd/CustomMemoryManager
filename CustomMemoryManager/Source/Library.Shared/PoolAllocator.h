#pragma once
#include "Allocator.h"
#include <vector>
#include <queue>

namespace CustomMemoryManager::Allocators
{
#define DEBUG_EXTRA_SPACE_BYTES_POOL 300

	template <typename T>
	class PoolAllocator final : public IAllocator
	{
	public:
		PoolAllocator(std::size_t poolSize_bytes, std::size_t alignment = 0U);
		PoolAllocator(const PoolAllocator&) = delete;
		PoolAllocator(PoolAllocator&&) = delete;
		PoolAllocator& operator=(const PoolAllocator&) = delete;
		PoolAllocator& operator=(PoolAllocator&&) = delete;
		~PoolAllocator();

		virtual void* allocate(std::size_t numObjects, std::size_t alignment = 0U, Info info = Info::NONE) override;
		virtual void deallocate(void* ptr, Info info = Info::NONE) override;
		void deallocate(void* ptr, std::size_t numObjects);

		virtual std::size_t Size_Bytes() const override;
		virtual std::size_t UsedSize_Bytes() const override;

		void Clear();

		std::size_t PoolSize_Bytes() const;
		std::size_t UsedSpace_Bytes() const;
		std::size_t PoolSize_NumObjects() const;

	private:
		void* mPoolStart{ nullptr };
		void* mPoolCurrent{ nullptr };
		std::size_t mPoolSize_bytes{ 0U };
		std::size_t mAlignment{ 0U };

		std::deque<void*> mInUse;			// pointers to memory in the pool that are not available
		std::deque<void*> mNotInUse;		// pointers to memory in the pool that are available

		/*
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
		*/
	};
}
#include "PoolAllocator.inl"