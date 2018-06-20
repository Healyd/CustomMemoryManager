#pragma once
#include "Allocator.h"
#include "StackAllocator.h"
#include "DoubleEndedStackAllocator.h"
#include "PoolAllocator.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <utility>

namespace CustomMemoryManager
{
	class MemoryManager final
	{
	public:
		enum AllocType
		{
			NONE,
			STACK,
			DOUBLESTACK,
			POOL,
			MAX
		};

	public:
		MemoryManager() = default;
		MemoryManager(const MemoryManager&) = delete;
		MemoryManager(MemoryManager&&) = delete;
		MemoryManager& operator=(const MemoryManager&) = delete;
		MemoryManager& operator=(MemoryManager&&) = delete;
		~MemoryManager() = default;

		bool CreateAllocator(const std::string& name, const std::size_t size, const AllocType type);
		template <typename T>
		bool CreateAllocator(const std::string& name, const std::size_t size, const AllocType type);
		void* Allocate(std::size_t allocAmount_bytes, const std::string& name, const AllocType type, Allocators::Info info = Allocators::Info::NONE);
		//MemData Allocate_GetData(std::size_t allocAmount_bytes, const std::string& name, const AllocType type);
		void Deallocate(void* ptr, const std::string& name, const AllocType type, Allocators::Info info = Allocators::Info::NONE);

		Allocators::IAllocator* Get(const std::string& name, const AllocType type);
		std::vector<std::string> Get(const AllocType type);

		//void IsValid(const std::string& name, const AllocType type);

	private:
		std::unordered_map<std::string, Allocators::StackAllocator> mStackAllocators;
		std::unordered_map<std::string, Allocators::DoubleEndedStackAllocator> mDoubleStackAllocators;
		std::unordered_map<std::string, Allocators::IAllocator*> mPoolAllocators;
	};

	template <typename T>
	inline bool MemoryManager::CreateAllocator(const std::string& name, const std::size_t size, const AllocType type)
	{
		bool isCreated = false;
		if (type == AllocType::POOL)
		{
			// If Not found: then add it.
			if (mPoolAllocators.find(name) == mPoolAllocators.end())
			{
				mPoolAllocators.emplace(std::move(name), std::move(new Allocators::PoolAllocator<T>(size)));
				isCreated;
			}
		}
		return isCreated;
	}
}

//inline CustomMemoryManager::MemoryManager gMemoryManager;

inline void* operator new(std::size_t size, const std::string& name, CustomMemoryManager::MemoryManager& memoryManager, const CustomMemoryManager::MemoryManager::AllocType type)
{
	return memoryManager.Allocate(size, name, type);
}

//inline void* operator new[] (std::size_t size, const std::string& name)
//{
//	return gMemoryManager.Allocate(size, name);
//}

inline void operator delete(void* ptr, const std::string& name, CustomMemoryManager::MemoryManager& memoryManager, const CustomMemoryManager::MemoryManager::AllocType type)
{
	memoryManager.Deallocate(ptr, name, type);
}

//inline void operator delete[](void* ptr)
//{
//	gMemoryManager.Deallocate(ptr);
//}