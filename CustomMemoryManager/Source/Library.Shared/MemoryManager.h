#pragma once
#include "Allocator.h"
#include "StackAllocator.h"
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
		void* Allocate(std::size_t allocAmount_bytes, const std::string& name, const AllocType type);
		//MemData Allocate_GetData(std::size_t allocAmount_bytes, const std::string& name, const AllocType type);
		void Deallocate(void* ptr, const std::string& name, const AllocType type);

		Allocators::IAllocator* Get(const std::string& name, const AllocType type);
		//void IsValid(const std::string& name, const AllocType type);

	private:
		std::unordered_map<std::string, Allocators::StackAllocator> mStackAllocators;
		//std::vector < std::pair<const std::string, Allocators::StackAllocator> > mStackAllocators;
	};
}

//inline CustomMemoryManager::MemoryManager gMemoryManager;
//
inline void* operator new(std::size_t size, const std::string& name, CustomMemoryManager::MemoryManager& memoryManager, const CustomMemoryManager::MemoryManager::AllocType type)
{
	return memoryManager.Allocate(size, name, type);
}
//
//inline void* operator new[] (std::size_t size, const std::string& name)
//{
//	return gMemoryManager.Allocate(size, name);
//}
//
inline void operator delete(void* ptr, const std::string& name, CustomMemoryManager::MemoryManager& memoryManager, const CustomMemoryManager::MemoryManager::AllocType type)
{
	memoryManager.Deallocate(ptr, name, type);
}
//
//inline void operator delete[](void* ptr)
//{
//	gMemoryManager.Deallocate(ptr);
//}