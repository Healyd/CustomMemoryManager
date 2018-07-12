#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include "MemoryManager.h"
#include <random>
#include <time.h>
#include <iostream>

template <typename T>
class MemoryTest
{
public:
	MemoryTest(const std::vector<std::tuple<std::string, std::size_t, CustomMemoryManager::MemoryManager::AllocType>>& names);
	void Run();
private:
	void TestStackAllocator(const std::string& name, std::size_t allocAmount, std::size_t deallocAmount);
	void TestDStackAllocatorTop(const std::string& name, std::size_t allocAmount, std::size_t deallocAmount);
	void TestDStackAllocatorBottom(const std::string& name, std::size_t allocAmount, std::size_t deallocAmount);
	void TestPoolAllocator(const std::string& name, std::size_t allocAmount, std::size_t deallocAmount);
	void TestHeapAllocator(const std::string& name, std::size_t allocAmount, std::size_t deallocAmount);

	std::vector<std::tuple<std::string, std::size_t, CustomMemoryManager::MemoryManager::AllocType>> mTestNames;
	std::unordered_map<std::string, std::vector<T*>> mPointers;
};

template <typename T>
MemoryTest<T>::MemoryTest(const std::vector<std::tuple<std::string, std::size_t, CustomMemoryManager::MemoryManager::AllocType>>& names)
	: mTestNames(names)
{
	for (const auto& it : names)
	{
		mPointers.emplace(std::move(std::get<0>(it)), std::vector<T*>());
		//mPointers.emplace(std::move(std::get<0>(it) + "_Malloc"), std::vector<T*>());
	}
	srand((std::uint32_t)time(NULL));
}

template <typename T>
void MemoryTest<T>::Run()
{
	for (const auto& it : mTestNames)
	{
		CustomMemoryManager::MemoryManager::AllocType type = std::get<2>(it);
		if (type == CustomMemoryManager::MemoryManager::AllocType::STACK)
		{
			//				  (name			  , alloc		   , dealloc        )
			TestStackAllocator(std::get<0>(it), std::get<1>(it), std::get<1>(it));
		}
		else if (type == CustomMemoryManager::MemoryManager::AllocType::POOL)
		{
			TestPoolAllocator(std::get<0>(it), std::get<1>(it), std::get<1>(it));
		}
		else if (type == CustomMemoryManager::MemoryManager::AllocType::DOUBLESTACK)
		{
			TestDStackAllocatorTop(std::get<0>(it), std::get<1>(it), std::get<1>(it));
			TestDStackAllocatorBottom(std::get<0>(it), std::get<1>(it), std::get<1>(it));
		}
		else if (type == CustomMemoryManager::MemoryManager::AllocType::HEAP)
		{
			TestHeapAllocator(std::get<0>(it), std::get<1>(it), std::get<1>(it));
		}
	}
}

template <typename T>
void MemoryTest<T>::TestStackAllocator(const std::string& name, std::size_t allocAmount, std::size_t deallocAmount)
{
	if (allocAmount <= 0U || deallocAmount <= 0U)
	{
		return;
	}

	std::size_t numObjectsToDeallocate = rand() % deallocAmount + 1;
	std::size_t numObjectsToAllocate = rand() % allocAmount + 1;

	std::vector<T*>& pointers = mPointers.find(name)->second;

	for (std::size_t i = 0U; i < numObjectsToDeallocate; ++i)
	{
		if (pointers.empty())
			break;
		T* ptr = pointers.back();
		pointers.pop_back();
		STACK_DEALLOC(name, ptr);
	}

	CustomMemoryManager::Allocators::StackAllocator* stackAllocator = static_cast<CustomMemoryManager::Allocators::StackAllocator*>(GLOBAL_MEMORY_MANAGER.Get(name, CustomMemoryManager::MemoryManager::AllocType::STACK));

	// number of objects left we can allocate
	std::size_t x = (stackAllocator->StackSize_Bytes() - stackAllocator->UsedSpace_Bytes()) / sizeof(T) + 1;
	if (numObjectsToAllocate >= x)
	{
		numObjectsToAllocate = x;
	}

	for (std::size_t i = 0U; i < numObjectsToAllocate; ++i)
	{
		T* ptr = STACK_ALLOC(name) T();
		if (ptr == nullptr)
			break;
		pointers.push_back(ptr);
	}
}

template <typename T>
void MemoryTest<T>::TestDStackAllocatorTop(const std::string& name, std::size_t allocAmount, std::size_t deallocAmount)
{
	if (allocAmount <= 0U || deallocAmount <= 0U)
	{
		return;
	}

	std::size_t numObjectsToDeallocate = rand() % deallocAmount + 1;
	std::size_t numObjectsToAllocate = rand() % allocAmount + 1;

	std::vector<T*>& pointers = mPointers.find(name)->second;

	for (std::uint32_t i = 0; i < numObjectsToDeallocate; ++i)
	{
		if (pointers.empty())
			break;
		T* ptr = pointers.back();
		pointers.pop_back();
		DSTACK_DEALLOC_TOP(name, ptr);
	}

	CustomMemoryManager::Allocators::DoubleEndedStackAllocator* stackAlloc = static_cast<CustomMemoryManager::Allocators::DoubleEndedStackAllocator*>(GLOBAL_MEMORY_MANAGER.Get(name, CustomMemoryManager::MemoryManager::AllocType::DOUBLESTACK));
	std::size_t x = (stackAlloc->TopStackSize_Bytes() - stackAlloc->UsedBytes_Top()) / sizeof(T) + 1;
	if (numObjectsToAllocate >= x)
	{
		numObjectsToAllocate = x;
	}

	for (std::uint32_t i = 0; i < numObjectsToAllocate; ++i)
	{
		//T* ptr = static_cast<T*>(GLOBAL_MEMORY_MANAGER.Allocate(sizeof(T), name, CustomMemoryManager::MemoryManager::AllocType::DOUBLESTACK, CustomMemoryManager::Allocators::Info::TOP));
		T* ptr = DSTACK_ALLOC_TOP(name) T();
		if (ptr == nullptr)
			break;
		pointers.push_back(ptr);
	}
}

template <typename T>
void MemoryTest<T>::TestDStackAllocatorBottom(const std::string& name, std::size_t allocAmount, std::size_t deallocAmount)
{
	if (allocAmount <= 0U || deallocAmount <= 0U)
	{
		return;
	}

	std::size_t numObjectsToDeallocate = rand() % deallocAmount + 1;
	std::size_t numObjectsToAllocate = rand() % allocAmount + 1;

	std::vector<T*>& pointers = mPointers.find(name)->second;

	for (std::uint32_t i = 0; i < numObjectsToDeallocate; ++i)
	{
		if (pointers.empty())
			break;
		T* ptr = pointers.back();
		pointers.pop_back();
		DSTACK_DEALLOC_BOTTOM(name, ptr);
	}

	CustomMemoryManager::Allocators::DoubleEndedStackAllocator* stackAlloc = static_cast<CustomMemoryManager::Allocators::DoubleEndedStackAllocator*>(GLOBAL_MEMORY_MANAGER.Get(name, CustomMemoryManager::MemoryManager::AllocType::DOUBLESTACK));
	std::size_t x = (stackAlloc->BottomStackSize_Bytes() - stackAlloc->UsedBytes_Bottom()) / sizeof(T) + 1;
	if (numObjectsToAllocate >= x)
	{
		numObjectsToAllocate = x;
	}

	for (std::uint32_t i = 0; i < numObjectsToAllocate; ++i)
	{
		T* ptr = DSTACK_ALLOC_BOTTOM(name) T();
		if (ptr == nullptr)
			break;
		pointers.push_back(ptr);
	}
}

template <typename T>
void MemoryTest<T>::TestPoolAllocator(const std::string& name, std::size_t allocAmount, std::size_t deallocAmount)
{
	if (allocAmount <= 0U || deallocAmount <= 0U)
	{
		return;
	}

	std::size_t numObjectsToDeallocate = rand() % deallocAmount + 1;
	std::size_t numObjectsToAllocate = rand() % allocAmount + 1;

	std::vector<T*>& pointers = mPointers.find(name)->second;

	for (std::uint32_t i = 0; i < numObjectsToDeallocate; ++i)
	{
		if (pointers.empty())
			break;
		T* ptr = pointers.back();
		pointers.pop_back();
		POOL_DEALLOC(ptr, name, T);
	}

	CustomMemoryManager::Allocators::PoolAllocator<T>* stackAlloc = static_cast<CustomMemoryManager::Allocators::PoolAllocator<T>*>(GLOBAL_MEMORY_MANAGER.Get(name, CustomMemoryManager::MemoryManager::AllocType::POOL));

	// number of objects left we can allocate
	std::size_t x = (stackAlloc->PoolSize_Bytes() - stackAlloc->UsedSpace_Bytes()) / sizeof(T) + 1;
	if (numObjectsToAllocate >= x)
	{
		numObjectsToAllocate = x;
	}

	for (std::uint32_t i = 0; i < numObjectsToAllocate; ++i)
	{
		T* ptr = POOL_ALLOC(name) T();
		if (ptr == nullptr)
			break;
		pointers.push_back(ptr);
	}
}

template <typename T>
void MemoryTest<T>::TestHeapAllocator(const std::string& name, std::size_t allocAmount, std::size_t deallocAmount)
{
	if (allocAmount <= 0U || deallocAmount <= 0U)
	{
		return;
	}

	std::size_t numObjectsToDeallocate = rand() % deallocAmount + 1;
	std::size_t numObjectsToAllocate = rand() % allocAmount + 1;

	std::vector<T*>& pointers = mPointers.find(name)->second;

	for (std::uint32_t i = 0U; i < numObjectsToDeallocate; ++i)
	{
		if (pointers.empty())
			break;
		T* ptr = pointers.back();
		pointers.pop_back();
		HEAP_DEALLOC(ptr, name, T);
	}

	CustomMemoryManager::Allocators::HeapAllocator* heapAllocator = static_cast<CustomMemoryManager::Allocators::HeapAllocator*>(GLOBAL_MEMORY_MANAGER.Get(name, CustomMemoryManager::MemoryManager::AllocType::HEAP));

	// number of objects left we can allocate
	std::size_t x = (heapAllocator->Size_Bytes() - heapAllocator->UsedSize_Bytes()) / sizeof(T) + 1;
	if (numObjectsToAllocate >= x)
	{
		numObjectsToAllocate = x;
	}

	for (std::uint32_t i = 0U; i < numObjectsToAllocate; ++i)
	{
		//T* ptr = HEAP_ALLOC_FIRSTFIT(name) T();
		//T* ptr = HEAP_ALLOC_LASTFIT(name) T();
		//T* ptr = HEAP_ALLOC_NEXTFIT(name) T();
		T* ptr = HEAP_ALLOC_BESTFIT(name) T();
		if (ptr == nullptr)
			break;
		pointers.push_back(ptr);
	}
}