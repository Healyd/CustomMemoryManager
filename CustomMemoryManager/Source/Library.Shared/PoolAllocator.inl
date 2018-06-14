
namespace CustomMemoryManager::Allocators
{
	template <typename T>
	PoolAllocator<T>::PoolAllocator(std::size_t poolSize_bytes, std::size_t alignment)
		: mPoolSize_bytes(poolSize_bytes), mAlignment(alignment)
	{
#ifdef _DEBUG
		mPoolStart = std::calloc(1U, poolSize_bytes + DEBUG_EXTRA_SPACE_BYTES_POOL);
#else
		mPoolStart = std::malloc(poolSize_bytes);
#endif
		mPoolCurrent = mPoolStart;
		for (std::uint32_t i = 0; i < poolSize_bytes; ++i)
		{
			mNotInUse.push_back(reinterpret_cast<void*>(reinterpret_cast<std::intptr_t>(mPoolStart) + i));
		}
		mPoolCurrent;
	}

	template <typename T>
	void* PoolAllocator<T>::allocate(std::size_t numObjects)
	{
		numObjects;
		void * ptr = mPoolCurrent;
		//mPoolCurrent + mPoolCurrent + numObjects * sizeof(T);
		return ptr;
	}

	template <typename T>
	void PoolAllocator<T>::deallocate()
	{

	}

	template <typename T>
	void PoolAllocator<T>::Clear()
	{

	}
}