#pragma once

namespace CustomMemoryManager
{
	template <typename T>
	MemPtr<T>::MemPtr(T* ptr)
		:	mAddress(ptr)
	{
	}

	template <typename T>
	MemPtr<T>::MemPtr(MemData& memData)
		:	mMemData(memData)
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
		if (mAddress == nullptr)
		{
			throw std::exception("Address is Null!");
		}
		return mAddress;
	}

	template <typename T>
	bool MemPtr<T>::IsNull() const
	{
		if (mAddress == nullptr)
		{
			return false;
		}
		return true;
	}

	template <typename T>
	bool MemPtr<T>::IsValid() const
	{
		if (mMemoryManager == nullptr)
		{
			return false;
		}
		// TODO: an IsValid() for the memory manager??
		//mMemoryManager->IsValid(mAllocatorName, mAllocType);
		return false;
	}
}