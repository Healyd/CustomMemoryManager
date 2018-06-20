
namespace CustomMemoryManager::Allocators
{
	template <typename T>
	PoolAllocator<T>::PoolAllocator(std::size_t poolSize_bytes, std::size_t alignment)
		: mPoolSize_bytes(poolSize_bytes), mAlignment(alignment)
	{
#ifdef _DEBUG
		mPoolStart = std::calloc(1U, poolSize_bytes);// +DEBUG_EXTRA_SPACE_BYTES_POOL);
#else
		mPoolStart = std::malloc(poolSize_bytes);
#endif
		mPoolCurrent = mPoolStart;
		for (std::size_t i = 0; i < poolSize_bytes; i+=sizeof(std::intptr_t))
		{
			mNotInUse.push_back(reinterpret_cast<void*>(reinterpret_cast<std::intptr_t>(mPoolStart) + i));
			chunk ck;
			ck.current = reinterpret_cast<void*>(reinterpret_cast<std::intptr_t>(mPoolStart) + i);
			ck.chunkSize = (poolSize_bytes - i) / sizeof(T);
			mNotInUse2.push(ck);
		}

		chunk chk;
		chk.start = mPoolStart;
		chk.end = reinterpret_cast<void*>(reinterpret_cast<std::intptr_t>(mPoolStart) + mPoolSize_bytes);
		chk.chunkSize_bytes = (reinterpret_cast<std::intptr_t>(chk.end) - reinterpret_cast<std::intptr_t>(chk.start));
		chk.chunkSize_objects = (reinterpret_cast<std::intptr_t>(chk.end) - reinterpret_cast<std::intptr_t>(chk.start)) / sizeof(T);
		mNotInUseChunks.push(chk);
	}

	template <typename T>
	void* PoolAllocator<T>::allocate(std::size_t numObjects)
	{
		if (mNotInUse.empty() || numObjects == 0U)
		{
			return nullptr;
		}

		if (numObjects == 1U)
		{
			//const chunk& hunk = mNotInUse2.front();
			//void* ptr = hunk.current;
			//std::size_t size = hunk.chunkSize;
			//mNotInUse2.pop();

			void* ptr = mNotInUse.front();
			mNotInUse.erase(mNotInUse.begin());
			mInUse.push_back(ptr);
			return ptr;
		}

		// TODO: chunk detection, for supporting arrays. Currently no support for arrays. Nullptr will be returned.

		return nullptr;
	}

	template <typename T>
	void PoolAllocator<T>::deallocate(void* ptr)
	{
		std::vector<void*>::iterator it = std::find(mInUse.begin(), mInUse.end(), ptr);
		if (it == mInUse.end())
		{
			return;
		}
		void* ptrFound = *it;
		mInUse.erase(it);
		mNotInUse.push_back(ptrFound);
		ptr = nullptr;
	}

	template <typename T>
	void PoolAllocator<T>::Clear()
	{
		mNotInUse.clear();
		mInUse.clear();
		for (std::size_t i = 0; i < poolSize_bytes; i += sizeof(std::intptr_t))
		{
			mNotInUse.push_back(reinterpret_cast<void*>(reinterpret_cast<std::intptr_t>(mPoolStart) + i));
		}
		// TODO: Should I just transfer mInUse to mNotInUse and not work about having the pointers in order. For loop may just be more work than I need.
	}

	template <typename T>
	std::size_t PoolAllocator<T>::PoolSize_Bytes() const
	{
		return mPoolSize_bytes;
	}

	template <typename T>
	std::size_t PoolAllocator<T>::UsedSpace_Bytes() const
	{
		return mPoolSize_bytes - (mInUse.size() * sizeof(T));
	}

	template <typename T>
	std::size_t PoolAllocator<T>::PoolSize_NumObjects() const
	{
		return mInUse.size();
	}
}