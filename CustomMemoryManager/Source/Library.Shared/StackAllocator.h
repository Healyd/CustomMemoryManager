#pragma once
#include "Allocator.h"
#include <cstdint>

namespace CustomMemoryManager::Allocators
{

#define GBtoBYTES(gigabytes) gigabytes*1024*1024*1024
#define MBtoBYTES(megabytes) megabytes*1024*1024

#define DEFAULT_STACKSIZE_BYTES GBtoBYTES(0.5f)
#define DEFAULT_STACKALIGNMENT 0U

#ifdef _DEBUG
	#define DEBUG_EXTRA_SPACE_BYTES 3000
#endif // _DEBUG


	class StackAllocator final : public IAllocator
	{
	private:
		struct Element
		{
		};

	public:
		class Iterator final
		{
			friend class StackAllocator;
		public:
			Iterator() = default;
			Iterator& operator++();
			Iterator operator++(int);
			const void * const operator*() const;

		private:
			Iterator(const std::uint32_t index, const StackAllocator& owner);
			std::uint32_t mIndex{ 0 };
			const StackAllocator* mOwner{ nullptr };
		};

	public:
		StackAllocator(std::size_t stackSize_bytes = DEFAULT_STACKSIZE_BYTES, std::size_t alignment = DEFAULT_STACKALIGNMENT);
		StackAllocator(const StackAllocator&) = delete;
		StackAllocator(StackAllocator&& other);
		StackAllocator& operator=(const StackAllocator&) = delete;
		StackAllocator& operator=(StackAllocator&& other);
		~StackAllocator();

		virtual void* allocate(std::size_t allocAmount_bytes, Info info = Info::NONE) override;
		virtual void deallocate(void* ptr, Info info = Info::NONE) override;
		void deallocate();

		const void * const GetStackTop() const;
		void Clear();
		std::size_t StackSize_Bytes() const;
		std::size_t StackSize_NumObjects() const;
		std::size_t UsedSpace_Bytes() const;
		const void * const StackStart() const;

#ifdef _DEBUG
		bool IsStackOverflow() const;
#endif //_DEBUG

		Iterator begin();
		Iterator end();

	private:
		std::size_t mStackSize_bytes{ 0 };
		std::size_t mAllignment{ 0 };
		std::intptr_t* mAllocLocations{ nullptr };
		std::uint32_t mIndex{ 0 };
		void* mStackStart{ nullptr };
		void* mStackCurrent{ nullptr };
		std::uint32_t mNumObjects{ 0 };
	};
}