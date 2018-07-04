#pragma once
#include "Allocator.h"
#include "StackAllocator.h"
#include "DoubleEndedStackAllocator.h"
#include "PoolAllocator.h"
#include "HeapAllocator.h"
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
			HEAP,
			MAX
		};

#ifdef MemDebug
		struct Data
		{
			float mAverageAllocationTime{ 0.0f };
			float mAverageDeallocationTime{ 0.0f };
			std::uint64_t mNumAllocations{ 0U };
			std::uint64_t mNumDeallocations{ 0U };

			float mAverageAllocationTime_Bottom{ 0.0f };
			float mAverageDeallocationTime_Bottom{ 0.0f };
			std::uint64_t mNumAllocations_Bottom{ 0U };
			std::uint64_t mNumDeallocations_Bottom{ 0U };
		};
#endif // MemDebug


	public:
		MemoryManager() = default;
		MemoryManager(const MemoryManager&) = delete;
		MemoryManager(MemoryManager&&) = delete;
		MemoryManager& operator=(const MemoryManager&) = delete;
		MemoryManager& operator=(MemoryManager&&) = delete;
		~MemoryManager();

		bool CreateAllocator(const std::string& name, const std::size_t size, const AllocType type);
		template <typename T>
		bool CreateAllocator(const std::string& name, const std::size_t size, const AllocType type);
		void* Allocate(std::size_t allocAmount_bytes, const std::string& name, const AllocType type, Allocators::Info info = Allocators::Info::NONE);
		//MemData Allocate_GetData(std::size_t allocAmount_bytes, const std::string& name, const AllocType type);
		void Deallocate(void* ptr, const std::string& name, const AllocType type, Allocators::Info info = Allocators::Info::NONE);

		Allocators::IAllocator* Get(const std::string& name, const AllocType type);
		std::vector<std::string> Get(const AllocType type);

#ifdef MemDebug
		const Data* const GetData(const std::string& name);
#endif // MemDebug

		//void IsValid(const std::string& name, const AllocType type);

	private:
		std::unordered_map<std::string, Allocators::StackAllocator> mStackAllocators;
		std::unordered_map<std::string, Allocators::DoubleEndedStackAllocator> mDoubleStackAllocators;
		std::unordered_map<std::string, Allocators::IAllocator*> mPoolAllocators;
		std::unordered_map<std::string, Allocators::HeapAllocator*> mHeapAllocators;
#ifdef MemDebug
		std::unordered_map<std::string, Data> mAllocatorData;
#endif // MemDebug
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
#ifdef MemDebug
				mAllocatorData.emplace(std::move(name), Data());
#endif // MemDebug
				isCreated = true;
			}
		}
		return isCreated;
	}
}

//inline CustomMemoryManager::MemoryManager gMemoryManager;

inline void* operator new(std::size_t size, const std::string& name, CustomMemoryManager::MemoryManager& memoryManager, const CustomMemoryManager::MemoryManager::AllocType type, CustomMemoryManager::Allocators::Info info = CustomMemoryManager::Allocators::Info::NONE)
{
	return memoryManager.Allocate(size, name, type, info);
}

//inline void* operator new[] (std::size_t size, const std::string& name)
//{
//	return gMemoryManager.Allocate(size, name);
//}

inline void operator delete(void* ptr, const std::string& name, CustomMemoryManager::MemoryManager& memoryManager, const CustomMemoryManager::MemoryManager::AllocType type, CustomMemoryManager::Allocators::Info info)
{
	memoryManager.Deallocate(ptr, name, type, info);
}

//inline void operator delete[](void* ptr)
//{
//	gMemoryManager.Deallocate(ptr);
//}

inline CustomMemoryManager::MemoryManager gMemoryManager;

#define GLOBAL_MEMORY_MANAGER gMemoryManager

#define DEFAULT_STACK	"DefaultStack"
#define DEFAULT_DSTACK	"DefaultDoubleStack"
#define DEFAULT_POOL	"DefaultPool"
#define DEFAULT_HEAP	"DefaultHeap"

#define STACK_ALLOC(name)			new (name, gMemoryManager, CustomMemoryManager::MemoryManager::AllocType::STACK)
#define DSTACK_ALLOC_TOP(name)		new (name, gMemoryManager, CustomMemoryManager::MemoryManager::AllocType::DOUBLESTACK, CustomMemoryManager::Allocators::Info::TOP)
#define DSTACK_ALLOC_BOTTOM(name)	new (name, gMemoryManager, CustomMemoryManager::MemoryManager::AllocType::DOUBLESTACK, CustomMemoryManager::Allocators::Info::BOTTOM)
#define POOL_ALLOC(name)			new (name, gMemoryManager, CustomMemoryManager::MemoryManager::AllocType::POOL)
#define HEAP_ALLOC(name)			new (name, gMemoryManager, CustomMemoryManager::MemoryManager::AllocType::HEAP)

#define STACK_DEALLOC(name, ptr)			gMemoryManager.Deallocate(ptr, name, CustomMemoryManager::MemoryManager::AllocType::STACK)
#define DSTACK_DEALLOC_TOP(name, ptr)		gMemoryManager.Deallocate(ptr, name, CustomMemoryManager::MemoryManager::AllocType::DOUBLESTACK, CustomMemoryManager::Allocators::Info::TOP)
#define DSTACK_DEALLOC_BOTTOM(name, ptr)	gMemoryManager.Deallocate(ptr, name, CustomMemoryManager::MemoryManager::AllocType::DOUBLESTACK, CustomMemoryManager::Allocators::Info::BOTTOM)
#define POOL_DEALLOC(ptr, name, type)		ptr->~type(); gMemoryManager.Deallocate(ptr, name, CustomMemoryManager::MemoryManager::AllocType::POOL);
#define HEAP_DEALLOC(ptr, name, type)		ptr->~type(); gMemoryManager.Deallocate(ptr, name, CustomMemoryManager::MemoryManager::AllocType::HEAP);

#define CREATE_STACK(name, size)		gMemoryManager.CreateAllocator(name, size, CustomMemoryManager::MemoryManager::AllocType::STACK)
#define CREATE_DSTACK(name, size)		gMemoryManager.CreateAllocator(name, size, CustomMemoryManager::MemoryManager::AllocType::DOUBLESTACK)
#define CREATE_POOL(name, type, size)	gMemoryManager.CreateAllocator<type>(name, size, CustomMemoryManager::MemoryManager::AllocType::POOL)
#define CREATE_HEAP(name, size)			gMemoryManager.CreateAllocator(name, size, CustomMemoryManager::MemoryManager::AllocType::HEAP)