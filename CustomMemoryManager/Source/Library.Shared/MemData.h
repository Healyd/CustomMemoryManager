/*
#pragma once
#include "MemoryEnums.h"
#include <string>

namespace CustomMemoryManager
{
	class MemoryManager;
	template <typename T>
	class MemPtr;

	template <typename T>
	class MemData final
	{
		friend class MemoryManager;
		friend class MemPtr<T>;

	public:
		MemData() = default;
		MemData(T* address, const AllocType type, const std::string& name, MemoryManager& manager);
		MemData(const MemData& other) = delete;
		MemData(MemData&& other);
		MemData& operator=(const MemData& other) = delete;
		MemData& operator=(MemData&& other);

	private:
		T* mAddress{ nullptr };
		AllocType mAllocType{ AllocType::NONE };
		std::string mAllocatorName;
		MemoryManager* mMemoryManager{ nullptr };
	};

	template <typename T>
	inline MemData<T>::MemData(T* address, const AllocType type, const std::string& name, MemoryManager& manager)
		: mAddress(address), mAllocType(type), mAllocatorName(name), mMemoryManager(&manager)
	{}

	//template <typename T>
	//inline MemData<T>::MemData(const MemData& other)
	//	: mAddress(other.mAddress), mAllocType(other.mAllocType), mAllocatorName(other.mAllocatorName), mMemoryManager(other.mMemoryManager)
	//{
	//	//other.mAddress = nullptr;
	//	//other.mAllocType = AllocType::NONE;
	//	//other.mAllocatorName = "";
	//	//other.mMemoryManager = nullptr;
	//}

	template <typename T>
	inline MemData<T>::MemData(MemData&& other)
		: mAddress(other.mAddress), mAllocType(other.mAllocType), mAllocatorName(std::move(other.mAllocatorName)), mMemoryManager(other.mMemoryManager)
	{
		other.mAddress = nullptr;
		other.mAllocType = AllocType::NONE;
		other.mAllocatorName = "";
		other.mMemoryManager = nullptr;
	}

	//template <typename T>
	//inline MemData<T>& MemData<T>::operator=(const MemData& other)
	//{
	//	mAddress = other.mAddress;
	//	mAllocType = other.mAllocType;
	//	mAllocatorName = other.mAllocatorName;
	//	mMemoryManager = other.mMemoryManager;
	//	//other.mAddress = nullptr;
	//	//other.mAllocType = AllocType::NONE;
	//	//other.mAllocatorName = "";
	//	//other.mMemoryManager = nullptr;
	//}

	template <typename T>
	inline MemData<T>& MemData<T>::operator=(MemData&& other)
	{
		mAddress = other.mAddress;
		mAllocType = other.mAllocType;
		mAllocatorName = std::move(other.mAllocatorName);
		mMemoryManager = other.mMemoryManager;
		other.mAddress = nullptr;
		other.mAllocType = AllocType::NONE;
		other.mAllocatorName = "";
		other.mMemoryManager = nullptr;
	}
}
*/