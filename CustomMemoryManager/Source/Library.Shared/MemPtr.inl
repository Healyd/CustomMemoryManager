#include "MemoryManager.h"

namespace CustomMemoryManager
{
	template <typename T>
	MemPtr<T>::MemPtr(T* ptr, const AllocType type, const std::string& name, MemoryManager& manager)
		: mAddress(ptr), mAllocType(type), mAllocatorName(name), mMemoryManager(&manager)//, mReferences(1U)
	{
		IncreaseReferenceCount();
	}

	template <typename T>
	MemPtr<T>::MemPtr(const MemPtr& other)
		: mAddress(other.mAddress), mAllocType(other.mAllocType), mAllocatorName(other.mAllocatorName), mMemoryManager(other.mMemoryManager), mReferences(other.mReferences)
	{
		// Up the Reference Count for the address
		IncreaseReferenceCount();
		//other.mReferences = mReferences;
	}

	template <typename T>
	MemPtr<T>::MemPtr(MemPtr&& other)
		: mAddress(other.mAddress), mAllocType(other.mAllocType), mAllocatorName(std::move(other.mAllocatorName)), mMemoryManager(other.mMemoryManager), mReferences(other.mReferences)
	{
		other.mAddress = nullptr;
		other.mAllocType = AllocType::NONE;
		other.mAllocatorName = "";
		other.mMemoryManager = nullptr;
		other.mReferences = 0U;
	}

	template <typename T>
	MemPtr<T>& MemPtr<T>::operator=(const MemPtr& other)
	{
		// If the address is not null it is referencing somewhere 
		// therefore needs ot decrement the reference count of where it currently points.
		if (mAddress != nullptr)
		{
			DecreaseReferenceCount();
		}

		mAddress = other.mAddress;
		mAllocType = other.mAllocType;
		mAllocatorName = other.mAllocatorName;
		mMemoryManager = other.mMemoryManager;

		// Up the Reference Count for the new address
		IncreaseReferenceCount();
	}

	template <typename T>
	MemPtr<T>& MemPtr<T>::operator=(MemPtr&& other)
	{
		mAddress = other.mAddress;
		mAllocType = other.mAllocType;
		mAllocatorName = std::move(other.mAllocatorName);
		mMemoryManager = other.mMemoryManager;
		mReferences = other.mReferences;
		other.mAddress = nullptr;
		other.mAllocType = AllocType::NONE;
		other.mAllocatorName = "";
		other.mMemoryManager = nullptr;
		other.mReferenes = 0U;
	}

	template <typename T>
	MemPtr<T>::~MemPtr()
	{
		DecreaseReferenceCount();
		// todo: should I delete the pointer on the last occurance. or should I 
		//if (mMemoryManager != nullptr && mReferences != nullptr && (*mReferences) <= 0U)
		//{
		//	HEAP_DEALLOC(mAddress, mAllocatorName, T, (*mMemoryManager));
		//	//~T();
		//	//mMemoryManager->Deallocate(mAddress, mAllocatorName, mAllocType);
		//}
	}

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

	template <typename T>
	void MemPtr<T>::Delete()
	{
		if (mMemoryManager != nullptr)// && mReferences != nullptr && (*mReferences) <= 0U)
		{
			//HEAP_DEALLOC(mAddress, mAllocatorName, T, (*mMemoryManager));
			~T();
			mMemoryManager->Deallocate(mAddress, mAllocatorName, mAllocType);
		}
	}

	template <typename T>
	void MemPtr<T>::IncreaseReferenceCount()
	{
		Allocators::IAllocator* allocator = mMemoryManager->Get(mAllocatorName, mAllocType);
		if (allocator != nullptr && mAllocType == AllocType::HEAP)
		{
			Allocators::HeapAllocator* heapAllocator = static_cast<Allocators::HeapAllocator*>(allocator);
			mReferences = heapAllocator->IncrementReference(mAddress);
		}
	}

	template <typename T>
	void MemPtr<T>::DecreaseReferenceCount()
	{
		Allocators::IAllocator* allocator = mMemoryManager->Get(mAllocatorName, mAllocType);
		if (allocator != nullptr && mAllocType == AllocType::HEAP)
		{
			Allocators::HeapAllocator* heapAllocator = static_cast<Allocators::HeapAllocator*>(allocator);
			mReferences = heapAllocator->DecrementReference(mAddress);
		}
	}
}