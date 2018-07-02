
namespace CustomMemoryManager::Allocators
{
	template <typename T>
	PoolAllocator<T>::PoolAllocator(std::size_t poolSize_bytes, std::size_t alignment)
		: mPoolSize_bytes(poolSize_bytes), mAlignment(alignment)
	{
#ifdef _DEBUG
		mPoolStart = std::calloc(1U, poolSize_bytes);		// +DEBUG_EXTRA_SPACE_BYTES_POOL);
#else
		mPoolStart = std::malloc(poolSize_bytes);
#endif
		mPoolCurrent = mPoolStart;
		for (std::size_t i = 0; i < poolSize_bytes; i += sizeof(T))
		{
			mNotInUse.push_back(reinterpret_cast<void*>(reinterpret_cast<std::intptr_t>(mPoolStart) + i));
			/*
			chunk ck;
			ck.current = reinterpret_cast<void*>(reinterpret_cast<std::intptr_t>(mPoolStart) + i);
			ck.chunkSize = (poolSize_bytes - i) / sizeof(T);
			mNotInUse2.push(ck);
			*/
		}

		/*
		chunk chk;
		chk.start = mPoolStart;
		chk.end = reinterpret_cast<void*>(reinterpret_cast<std::intptr_t>(mPoolStart) + mPoolSize_bytes);
		chk.chunkSize_bytes = (reinterpret_cast<std::intptr_t>(chk.end) - reinterpret_cast<std::intptr_t>(chk.start));
		chk.chunkSize_objects = (reinterpret_cast<std::intptr_t>(chk.end) - reinterpret_cast<std::intptr_t>(chk.start)) / sizeof(T);
		mNotInUseChunks.push(chk);
		*/
	}

	template <typename T>
	PoolAllocator<T>::~PoolAllocator()
	{
		if (mPoolStart != nullptr)
		{
			for (auto* ptr : mInUse)
			{
				static_cast<T*>(ptr)->~T();
			}

			std::free(mPoolStart);
		}
	}

	template <typename T>
	void* PoolAllocator<T>::allocate(std::size_t numObjects, std::size_t, Info)
	{
		numObjects /= sizeof(T);
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
		else
		{
			bool found = false;
			// Iterate available pointers
			for (void* ptr : mNotInUse)
			{
				void* tempPtr = ptr;
				std::vector<void*> tempVector;
				tempVector.push_back(tempPtr);
				// Try and find a memory chunck large enough
				for (std::size_t i = 1U; i < numObjects; ++i)
				{
					tempPtr = reinterpret_cast<void*>(reinterpret_cast<std::intptr_t>(tempPtr) + sizeof(T));
					tempVector.push_back(tempPtr);
					// If not found
					if (std::find(mNotInUse.begin(), mNotInUse.end(), tempPtr) == mNotInUse.end())
					{
						found = false;
						break;
					}
					// Maybe found, depends on the end result of 'found' when exiting the for loop.
					found = true;
				}
				if (found)
				{
					for (const auto& it : tempVector)
					{
						mNotInUse.erase(std::find(mNotInUse.begin(), mNotInUse.end(), it));
						mInUse.push_back(it);
					}
					return ptr;
				}
			}
		}

		// TODO: chunk detection, for supporting arrays. Currently no support for arrays. Nullptr will be returned.

		return nullptr;
	}

	template <typename T>
	void PoolAllocator<T>::deallocate(void* ptr, Info)
	{
		std::deque<void*>::iterator it = std::find(mInUse.begin(), mInUse.end(), ptr);
		if (it == mInUse.end())
		{
			return;
		}
		void* ptrFound = *it;
		mInUse.erase(it);
		mNotInUse.push_front(ptrFound);
		ptr = nullptr;					// TODO: not sure about this line, if it works, need to test
	}

	template <typename T>
	void PoolAllocator<T>::deallocate(void* ptr, std::size_t numObjects)
	{
		void* tempPtr = ptr;
		for (std::size_t i = 0; i < numObjects; ++i)
		{
			std::deque<void*>::iterator it = std::find(mInUse.begin(), mInUse.end(), tempPtr);
			if (it == mInUse.end())
			{
				throw std::exception("Invalid Deallocation!");
			}
			void* ptrFound = *it;
			mInUse.erase(it);
			mNotInUse.push_back(ptrFound);
			tempPtr = reinterpret_cast<void*>(reinterpret_cast<std::intptr_t>(tempPtr) + sizeof(T));
		}
		ptr = nullptr;					// TODO: not sure about this line, if it works, need to test
	}

	template <typename T>
	void PoolAllocator<T>::Clear()
	{
		mNotInUse.clear();
		mInUse.clear();
		mNotInUse.shrink_to_fit();
		mInUse.shrink_to_fit();
		for (std::size_t i = 0; i < mPoolSize_bytes; i += sizeof(T))
		{
			mNotInUse.push_back(reinterpret_cast<void*>(reinterpret_cast<std::intptr_t>(mPoolStart) + i));
		}
		// TODO: Should I just transfer mInUse to mNotInUse and not work about having the pointers in order. For loop may just be more work than I need.
		// On the other hand, if it is in order and I clear it. Re-allocation on the stack my work better.
	}

	template <typename T>
	std::size_t PoolAllocator<T>::PoolSize_Bytes() const
	{
		return mPoolSize_bytes;
	}

	template <typename T>
	std::size_t PoolAllocator<T>::UsedSpace_Bytes() const
	{
		return (mInUse.size() * sizeof(T));
	}

	template <typename T>
	std::size_t PoolAllocator<T>::Size_Bytes() const
	{
		return mPoolSize_bytes;
	}

	template <typename T>
	std::size_t PoolAllocator<T>::UsedSize_Bytes() const
	{
		std::size_t size = sizeof(T);
		size;
		return (mInUse.size() * sizeof(T));
	}

	template <typename T>
	std::size_t PoolAllocator<T>::PoolSize_NumObjects() const
	{
		return mInUse.size();
	}
}