#pragma once
#include "MemoryManager.h"
#include <string>

namespace CustomMemoryManager
{
	template <typename T>
	class MemPtr final
	{
	public:
		struct MemData
		{
		public:
			MemData() = default;
			
			MemData(T* address, const MemoryManager::AllocType type, const std::string& name, MemoryManager& manager)
				: mAddress(address), mAllocType(type), mAllocatorName(name), mMemoryManager(&manager)
			{}

			MemData(const MemData& other)
				: mAddress(other.mAddress), mAllocType(other.mAllocType), mAllocatorName(other.mAllocatorName), mMemoryManager(other.mMemoryManager)
			{}

			T* mAddress{ nullptr };
			MemoryManager::AllocType mAllocType{ MemoryManager::AllocType::NONE };
			std::string mAllocatorName;
			MemoryManager* mMemoryManager{ nullptr };
		};

		MemPtr(T* ptr);
		MemPtr(MemData& memData);
		MemPtr(const MemPtr&) = delete;
		MemPtr(MemPtr&&) = delete;
		MemPtr& operator=(const MemPtr&) = delete;
		MemPtr& operator=(MemPtr&&) = delete;
		~MemPtr() = default;

		inline void SetPtr(T* ptr) { mAddress = ptr; };

		T& operator*();
		T* operator->();
		T* Get();
		bool IsNull() const;
		bool IsValid() const;

	private:
		T* mAddress{ nullptr };
		MemoryManager::AllocType mAllocType{ MemoryManager::AllocType::NONE };
		std::string mAllocatorName;
		MemoryManager* mMemoryManager{ nullptr };		//TODO: extent to maybe point directly to the allocator this object is in.
		MemData mMemData;
	};

	template <typename T>
	inline T* MakeMemPtr_Raw(const std::string& name, MemoryManager::AllocType type, MemoryManager& manager)
	{
		return static_cast<T*>(new (name, manager, type) T());
	}

	//template <typename T>
	//inline MemPtr<T>::MemData MakeMemPtrData(const std::string& name, MemoryManager::AllocType type, MemoryManager& manager)
	//{
	//	return MemPtr<T>::MemData(
	//		static_cast<T*>(new (name, manager, type) T()), 
	//		type, name, manager);
	//}
}
#include "MemPtr.inl"