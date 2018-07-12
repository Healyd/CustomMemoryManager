#pragma once
#include <cstdlib>
#include <cstdint>

//#ifdef _DEBUG
//#define _MEMDEBUG
//#endif // _DEBUG
//
//#ifdef _DEBUGOPTIMIZED
//#define _MEMDEBUG
//#endif // _DEBUGOPTIMIZED


//#ifdef _DEBUG
//	#define ALLOCATOR_DEBUG
//#endif // DEBUG

#define POINTER_SIZE sizeof(std::intptr_t)

namespace CustomMemoryManager::Allocators
{
	enum Info
	{
		NONE,
		TOP,
		BOTTOM,
		HEAP_FIRSTFIT,
		HEAP_LASTFIT,
		HEAP_NEXTFIT,
		HEAP_BESTFIT,
		MAX
	};

	class IAllocator
	{
	public:
		virtual ~IAllocator() {};
		virtual void* allocate(std::size_t allocAmount_bytes, std::size_t alignment = 0U, Info info = Info::NONE) = 0;
		virtual void deallocate(void* ptr, Info info = Info::NONE) = 0;
		virtual std::size_t Size_Bytes() const = 0;
		virtual std::size_t UsedSize_Bytes() const = 0;
	};
}