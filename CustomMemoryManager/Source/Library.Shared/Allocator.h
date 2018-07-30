#pragma once
#include <cstdlib>
#include <cstdint>
#include <exception>

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
		FASTd,
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

	protected:
		inline void CheckAlignment(std::size_t& alignment)
		{
			if (alignment % 2U != 0U)
			{
				throw std::exception("Odd Byte Alignment");
			}

			if (alignment > 0U)
			{
				alignment = alignment - 1;
			}
		}

		inline void AlignPointer(void** ptr, std::size_t alignment)
		{
			for (std::size_t i = 0U; i < alignment + 1; ++i)
			{
				if (reinterpret_cast<std::uintptr_t>(*ptr) % (alignment + 1) == 0)
				{
					break;
				}
				*ptr = reinterpret_cast<void*>(reinterpret_cast<std::intptr_t>(*ptr) + 0x01);
			}
		}
	};
}