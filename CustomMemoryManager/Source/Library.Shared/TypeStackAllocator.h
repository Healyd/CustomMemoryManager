#pragma once
#include "Allocator.h"
#include <cstdint>

namespace CustomMemoryManager::Allocators
{
	template <typename T>
	class TypeStackAllocator final
	{
	public:
		using value_type = T;
		using size_type = std::size_t;
		using pointer = T * ;
		using const_pointer = const pointer;
		using reference = T & ;
		using const_reference = const T&;

		template <typename U>
		struct rebind
		{
			typedef TypeStackAllocator<U> other;
		};

		TypeStackAllocator() = default;
		~TypeStackAllocator();

		T* allocate(std::size_t allocAmount_bytes);
		void deallocate(T* ptr, std::size_t allocAmount_bytes);

		const void * const GetStackTop() const;
		void Clear();
		std::uint32_t StackByteSize() const;
		std::uint32_t NumObjectsInStack() const;
		const void * const StackStart() const;

	private:
		std::uint32_t mStackSize_bytes{ 0 };
		std::intptr_t* mAllocLocations{ nullptr };
		std::int32_t mIndex{ 0 };
		void* mStackStart{ nullptr };
		void* mStackCurrent{ nullptr };
		std::uint32_t mNumObjects{ 0 };

		//std::stack<std::uint32_t> mAllocSizes;
	};
}