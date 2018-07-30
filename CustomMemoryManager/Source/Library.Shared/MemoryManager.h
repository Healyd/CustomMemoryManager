#pragma once
#include "Allocator.h"
#include "StackAllocator.h"
#include "DoubleEndedStackAllocator.h"
#include "PoolAllocator.h"
#include "HeapAllocator.h"
#include "MemoryEnums.h"
#include "MemPtr.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <utility>
#include <stdio.h>
#include <stdarg.h>

#define ALLOCATIONS_FOR_FILEOUTPUT 100000

//#define _OUTPUTFILE	// runs in debugoptimized mode
#define _NOOUTPUTFILE
//#define _STACK		// overrides _NOOUTPUTFILE
//#define _DSTACK
//#define _POOL
//#define _HEAP

namespace CustomMemoryManager
{
	class MemoryManager final
	{
	public:
#ifdef _MEMDEBUG
		struct Data
		{
			float mSumAllocationTime{ 0.0f };
			float mSumDeallocationTime{ 0.0f };
			std::uint64_t mNumAllocations{ 0U };
			std::uint64_t mNumDeallocations{ 0U };

			float mSumAllocationTime_Bottom{ 0.0f };
			float mSumDeallocationTime_Bottom{ 0.0f };
			std::uint64_t mNumAllocations_Bottom{ 0U };
			std::uint64_t mNumDeallocations_Bottom{ 0U };
		};
#endif // _MEMDEBUG

	public:
//#ifdef _MEMDEBUG
		MemoryManager();
//#else
//		MemoryManager() = default;
//#endif // _MEMDEBUG
		MemoryManager(const MemoryManager&) = delete;
		MemoryManager(MemoryManager&&) = delete;
		MemoryManager& operator=(const MemoryManager&) = delete;
		MemoryManager& operator=(MemoryManager&&) = delete;
		~MemoryManager();

		bool CreateAllocator(const std::string& name, const std::size_t size, const AllocType type);
		template <typename T>
		bool CreateAllocator(const std::string& name, const std::size_t size, AllocType type);
		
		// todo: support all allocators. currently only supports the heap allocator
		template <typename T>
		MemPtr<T> MemPtr_HeapAllocate(std::size_t allocAmount_bytes, const std::string& name, /*const AllocType type,*/ const std::size_t alignment = 0U, const std::string& fileName = "none", const std::size_t lineNumber = 0U, Allocators::Info info = Allocators::Info::NONE);
		void* Allocate(std::size_t allocAmount_bytes, const std::string& name, const AllocType type, const std::size_t alignment = 0U, const std::string& fileName = "none", const std::size_t lineNumber = 0U, Allocators::Info info = Allocators::Info::NONE);

		void Deallocate(void* ptr, const std::string& name, const AllocType type, const std::string fileName = "none", const std::size_t lineNumber = 0U, Allocators::Info info = Allocators::Info::NONE);

		Allocators::IAllocator* Get(const std::string& name, const AllocType type);
		std::vector<std::string> Get(const AllocType type);

#ifdef _MEMDEBUG
		const Data* const GetData(const std::string& name);
		void OutputFile(const std::string& allocatorName, const std::string& fileName, const std::size_t lineNumber, std::size_t allocationSize_bytes, void* ptr);
		void OutputFileAverages();
		bool AreAllAllocationsOver() const;
#endif // _MEMDEBUG

		//void IsValid(const std::string& name, const AllocType type);

	private:
		std::unordered_map<std::string, Allocators::StackAllocator> mStackAllocators;
		std::unordered_map<std::string, Allocators::DoubleEndedStackAllocator> mDoubleStackAllocators;
		std::unordered_map<std::string, Allocators::IAllocator*> mPoolAllocators;
		std::unordered_map<std::string, Allocators::HeapAllocator*> mHeapAllocators;
#ifdef _MEMDEBUG
		std::unordered_map<std::string, Data> mAllocatorData;
#endif // _MEMDEBUG
		std::string mDateTimeStamp;
		std::string mOutputDirectory;
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
#ifdef _MEMDEBUG
				mAllocatorData.emplace(std::move(name), Data());
#endif // _MEMDEBUG
				isCreated = true;
			}
		}
		return isCreated;
	}

	template <typename T>
	MemPtr<T> MemoryManager::MemPtr_HeapAllocate(std::size_t allocAmount_bytes, const std::string& name, /*const AllocType type,*/ const std::size_t alignment, const std::string& fileName, const std::size_t lineNumber, Allocators::Info info)
	{
		return MemPtr<T>(
			static_cast<T*>(Allocate(allocAmount_bytes, name, AllocType::HEAP, alignment, fileName, lineNumber, info)),		// Allocation Location
			AllocType::HEAP,	// Allocator Type
			name,				// Allocator Name
			*this				// MemoryManager Pointer
			);
	}
}

inline void* operator new(std::size_t size, const std::string& name, CustomMemoryManager::MemoryManager& memoryManager, const CustomMemoryManager::AllocType type, const std::size_t alignment = 0U, std::string fileName = "none", const std::size_t lineNumber = 0U, CustomMemoryManager::Allocators::Info info = CustomMemoryManager::Allocators::Info::NONE)
{
	return memoryManager.Allocate(size, name, type, alignment, fileName, lineNumber, info);
}

//inline void* operator new[] (std::size_t size, const std::string& name)
//{
//	return gMemoryManager.Allocate(size, name);
//}

inline void operator delete(void* ptr, const std::string& name, CustomMemoryManager::MemoryManager& memoryManager, const CustomMemoryManager::AllocType type, const std::size_t, const std::string fileName, const std::size_t lineNumber, CustomMemoryManager::Allocators::Info info)
{
	memoryManager.Deallocate(ptr, name, type, fileName, lineNumber, info);
}

#define OVERLOADED_MACRO(M, ...) _OVR(M, NUM_ARGS(__VA_ARGS__)) (__VA_ARGS__)
#define _OVR(macroName, number_of_args) _OVR_EXPAND(macroName, number_of_args)
#define _OVR_EXPAND(macroName, number_of_args) macroName##number_of_args

#define EXPAND(x) x 
#define NUM_ARGS_N(_1,_2,_3,_4,_5,_6,_7,_8,_9,N,...) N 
#define NUM_ARGS(...) EXPAND(NUM_ARGS_N(__VA_ARGS__, 9,8,7,6,5,4,3,2,1,0))

inline CustomMemoryManager::MemoryManager gMemoryManager;

#define GLOBAL_MEMORY_MANAGER gMemoryManager

// todo: implement default allocators already exiting in the manager.
/*
#define DEFAULT_STACK	"DefaultStack"
#define DEFAULT_DSTACK	"DefaultDoubleStack"
#define DEFAULT_POOL	"DefaultPool"
#define DEFAULT_HEAP	"DefaultHeap"
*/
//////////////////////////////////

// STACK_ALLOC(std::string name, std::size_t alignment = 0U)
#define STACK_ALLOC(...)				OVERLOADED_MACRO(STACK_ALLOC, __VA_ARGS__)
#define STACK_ALLOC1(name)				STACK_ALLOC2(name, 0U) 
#define STACK_ALLOC2(name, alignment)	new (name, gMemoryManager, CustomMemoryManager::AllocType::STACK,	alignment, __FILE__, __LINE__)
//#define STACK_ALLOC3(name, alignment, memoryManager)	new (name, memoryManager, CustomMemoryManager::AllocType::STACK,	alignment, __FILE__, __LINE__)

#define DSTACK_ALLOC_TOP(name)		new (name, gMemoryManager, CustomMemoryManager::AllocType::DOUBLESTACK,	0U, __FILE__, __LINE__ , CustomMemoryManager::Allocators::Info::TOP)
#define DSTACK_ALLOC_BOTTOM(name)	new (name, gMemoryManager, CustomMemoryManager::AllocType::DOUBLESTACK,	0U, __FILE__, __LINE__ , CustomMemoryManager::Allocators::Info::BOTTOM)

#define POOL_ALLOC(name)			new (name, gMemoryManager, CustomMemoryManager::AllocType::POOL,		0U, __FILE__, __LINE__ )

#define HEAP_ALLOC_FIRSTFIT(name)	new (name, gMemoryManager, CustomMemoryManager::AllocType::HEAP,		0U, __FILE__, __LINE__, CustomMemoryManager::Allocators::Info::HEAP_FIRSTFIT)
#define HEAP_ALLOC_LASTFIT(name)	new (name, gMemoryManager, CustomMemoryManager::AllocType::HEAP,		0U, __FILE__, __LINE__, CustomMemoryManager::Allocators::Info::HEAP_LASTFIT)
#define HEAP_ALLOC_NEXTFIT(name)	new (name, gMemoryManager, CustomMemoryManager::AllocType::HEAP,		0U, __FILE__, __LINE__, CustomMemoryManager::Allocators::Info::HEAP_NEXTFIT)
#define HEAP_ALLOC_BESTFIT(name)	new (name, gMemoryManager, CustomMemoryManager::AllocType::HEAP,		0U, __FILE__, __LINE__, CustomMemoryManager::Allocators::Info::HEAP_BESTFIT)

////////////////////////////////

#define STACK_DEALLOC(name, ptr)			gMemoryManager.Deallocate(ptr, name, CustomMemoryManager::AllocType::STACK, __FILE__, __LINE__ )
#define DSTACK_DEALLOC_TOP(name, ptr)		gMemoryManager.Deallocate(ptr, name, CustomMemoryManager::AllocType::DOUBLESTACK, __FILE__, __LINE__ , CustomMemoryManager::Allocators::Info::TOP)
#define DSTACK_DEALLOC_BOTTOM(name, ptr)	gMemoryManager.Deallocate(ptr, name, CustomMemoryManager::AllocType::DOUBLESTACK, __FILE__, __LINE__ , CustomMemoryManager::Allocators::Info::BOTTOM)
#define POOL_DEALLOC(ptr, name, type)		ptr->~type(); gMemoryManager.Deallocate(ptr, name, CustomMemoryManager::AllocType::POOL, __FILE__, __LINE__ );

#define HEAP_DEALLOC(...)								OVERLOADED_MACRO(HEAP_DEALLOC, __VA_ARGS__)
#define HEAP_DEALLOC3(ptr, name, type)					HEAP_DEALLOC4(ptr, name, type, GLOBAL_MEMORY_MANAGER) //ptr->~type(); gMemoryManager.Deallocate(ptr, name, CustomMemoryManager::AllocType::HEAP, __FILE__, __LINE__ );
#define HEAP_DEALLOC4(ptr, name, type, memoryManager)	ptr->~type(); memoryManager.Deallocate(ptr, name, CustomMemoryManager::AllocType::HEAP, __FILE__, __LINE__ );

#define CREATE_STACK(name, size)		gMemoryManager.CreateAllocator(name, size, CustomMemoryManager::AllocType::STACK)
#define CREATE_DSTACK(name, size)		gMemoryManager.CreateAllocator(name, size, CustomMemoryManager::AllocType::DOUBLESTACK)
#define CREATE_POOL(name, type, size)	gMemoryManager.CreateAllocator<type>(name, size, CustomMemoryManager::AllocType::POOL)
#define CREATE_HEAP(name, size)			gMemoryManager.CreateAllocator(name, size, CustomMemoryManager::AllocType::HEAP)