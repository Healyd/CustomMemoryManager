#pragma once
#include "MemoryEnums.h"
#include <string>

namespace CustomMemoryManager
{
	class MemoryManager;
	class IAllocator;

	template <typename T>
	class MemPtr final
	{
		friend class MemoryManager;

	public:
		MemPtr() = default;
		MemPtr(const MemPtr& other);
		MemPtr(MemPtr&& other);
		MemPtr& operator=(const MemPtr& other);
		MemPtr& operator=(MemPtr&& other);
		~MemPtr();

		T& operator*();
		T* operator->();
		T* Get();
		bool IsNull() const;
		bool IsValid() const;
		void Delete();

	private:
		MemPtr(T* ptr, const AllocType type, const std::string& name, MemoryManager& manager);
		void IncreaseReferenceCount();
		void DecreaseReferenceCount();

		T* mAddress{ nullptr };
		AllocType mAllocType{ AllocType::NONE };
		std::string mAllocatorName;
		MemoryManager* mMemoryManager{ nullptr };		//TODO: extent to maybe point directly to the allocator this object is in.
		IAllocator* mAllocator{ nullptr };
		std::size_t* mReferences{ nullptr };			// points to the reference count in the heap allocator

		//Allocators::HeapAllocator::HeapNode* hNode{ nullptr };


		//mutable std::size_t mReferences{ 0U };
	};

	//template <typename T>
	//inline T* MakeMemPtr_Raw(const std::string& name, MemoryManager::AllocType type, MemoryManager& manager)
	//{
	//	return static_cast<T*>(new (name, manager, type, "makememptr_raw", 0U) T());
	//}

	//template <typename T>
	//inline MemPtr<T>::MemData MakeMemPtrData(const std::string& name, MemoryManager::AllocType type, MemoryManager& manager)
	//{
	//	return MemPtr<T>::MemData(
	//		static_cast<T*>(new (name, manager, type) T()), 
	//		type, name, manager);
	//}
}
#include "MemPtr.inl"