
namespace CustomMemoryManager
{
	//template <typename T>
	//MemData<T>::MemData(T* address, const MemoryManager::AllocType type, const std::string& name, MemoryManager& manager)
	//	: mAddress(address), mAllocType(type), mAllocatorName(name), mMemoryManager(&manager)
	//{}

	//template <typename T>
	//MemData<T>::MemData(const MemData<T>& other)
	//	: mAddress(other.mAddress), mAllocType(other.mAllocType), mAllocatorName(other.mAllocatorName), mMemoryManager(other.mMemoryManager)
	//{}

	template <typename T>
	MemPtr<T>::MemPtr(T* ptr)
		:	mAddress(ptr)
	{
	}

	//template <typename T>
	//MemPtr<T>::MemPtr(MemData<T>& memData)
	//	: mMemData(memData)
	//{
	//}

	template <typename T>
	MemPtr<T>::MemPtr(MemData<T>&& memData)
		:	mMemData(std::move(memData))
	{
	}

	//template <typename T>
	//MemPtr<T>::~MemPtr()
	//{
	//	if (mMemoryManager != nullptr)
	//	{
	//		~T();
	//		mMemoryManager->Deallocate(Get(), mAllocatorName, mAllocType);
	//	}
	//}

	template <typename T>
	T& MemPtr<T>::operator*()
	{
		return *Get();
	}

	template <typename T>
	T* MemPtr<T>::operator->()
	{
		return Get();
	}

	template <typename T>
	T* MemPtr<T>::Get()
	{
		if (mMemData.mAddress == nullptr)
		{
			throw std::exception("Address is Null!");
		}
		return mMemData.mAddress;
	}

	template <typename T>
	bool MemPtr<T>::IsNull() const
	{
		if (mMemData.mAddress == nullptr)
		{
			return false;
		}
		return true;
	}

	template <typename T>
	bool MemPtr<T>::IsValid() const
	{
		if (mMemData.mMemoryManager == nullptr)
		{
			return false;
		}
		// TODO: an IsValid() for the memory manager??
		//mMemoryManager->IsValid(mAllocatorName, mAllocType);
		return false;
	}
}