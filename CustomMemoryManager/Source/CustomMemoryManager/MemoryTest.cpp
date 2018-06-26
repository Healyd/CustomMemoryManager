//#include "MemoryTest.h"
//#include "MemoryManager.h"
//#include <random>
//#include <time.h>
//#include <iostream>
//#include <string>
//
//using namespace CustomMemoryManager;
//
//template <typename T>
//MemoryTest<T>::MemoryTest()
//{
//	srand((std::uint32_t)time(NULL));
//}
//
//template <typename T>
//void MemoryTest<T>::TestStackAllocator_Ints(const std::string& name, std::uint32_t allocAmount, std::uint32_t deallocAmount)
//{
//	if (allocAmount <= 0U || deallocAmount <= 0U)
//	{
//		return;
//	}
//
//	std::size_t numObjectsToDeallocate = rand() % deallocAmount + 1;
//	std::size_t numObjectsToAllocate = rand() % allocAmount + 1;
//
//	for (std::size_t i = 0U; i < numObjectsToDeallocate; ++i)
//	{
//		if (!mIntVector.empty())
//		{
//			T* ptr = mIntVector.back();
//			mIntVector.pop_back();
//			//todo: start timer
//			//STACK_DEALLOC(name, intPtr);
//			GLOBAL_MEMORY_MANAGER.Deallocate(ptr, name, CustomMemoryManager::MemoryManager::AllocType::STACK);
//			//todo: stop timer
//		}
//	}
//
//	Allocators::StackAllocator* stackAllocator = static_cast<Allocators::StackAllocator*>(GLOBAL_MEMORY_MANAGER.Get(name, MemoryManager::AllocType::STACK));
//
//	std::size_t x = (stackAllocator->StackSize_Bytes() - stackAllocator->UsedSpace_Bytes()) / sizeof(T) + 1;
//	if (numObjectsToAllocate >= x)
//	{
//		numObjectsToAllocate = x;
//	}
//
//	for (std::size_t i = 0U; i < numObjectsToAllocate; ++i)
//	{
//		T* ptr = static_cast<T*>(GLOBAL_MEMORY_MANAGER.Allocate(sizeof(T), name, CustomMemoryManager::MemoryManager::AllocType::STACK));
//		//std::uint32_t* ptr = STACK_ALLOC(name) std::uint32_t();
//		mIntVector.push_back(ptr);
//	}
//}
