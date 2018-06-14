#pragma once
#include <cstdlib>
#include <cstdint>

//#ifdef _DEBUG
//	#define ALLOCATOR_DEBUG
//#endif // DEBUG

#define POINTER_SIZE sizeof(std::intptr_t)

namespace CustomMemoryManager::Allocators
{
	class IAllocator
	{
	public:
		virtual void* allocate(std::size_t allocAmount_bytes) = 0;
		virtual void deallocate() = 0;
	};
}