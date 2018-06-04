#pragma once
#include "MemoryManager.h"
#include <string>

namespace CustomMemoryManager
{
	template <typename T>
	class MemPtr final
	{
	public:
		MemPtr(T* ptr);
		MemPtr(const MemPtr&) = default;
		MemPtr(MemPtr&&) = default;
		MemPtr& operator=(const MemPtr&) = default;
		MemPtr& operator=(MemPtr&&) = default;
		~MemPtr() = default;

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
	};
}
#include "MemPtr.inl"