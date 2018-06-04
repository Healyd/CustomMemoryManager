#pragma once
#include "Allocator.h"
#include <cstdint>

namespace CustomMemoryManager::Allocators
{
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
		StackAllocator(std::size_t stackSize_bytes, std::size_t alignment = 0U);
		StackAllocator(const StackAllocator&) = delete;
		StackAllocator(StackAllocator&& other);
		StackAllocator& operator=(const StackAllocator&) = delete;
		StackAllocator& operator=(StackAllocator&&) = default;
		~StackAllocator();

		virtual void* allocate(std::size_t allocAmount_bytes) override;
		virtual void deallocate() override;

		const void * const GetStackTop() const;
		void Clear();
		std::size_t StackSize_Bytes() const;
		std::size_t StackSize_NumObjects() const;
		const void * const StackStart() const;

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