#pragma once
#include "StackAllocator.h"

namespace CustomMemoryManager::Allocators
{
	class DoubleEndedStackAllocator final : public IAllocator
	{
	public:
		//class Iterator final
		//{
		//	friend class DoubleEndedStackAllocator;
		//public:
		//	Iterator() = default;
		//	Iterator& operator++();
		//	Iterator& operator++(int);
		//	const void * const operator*() const;

		//private:
		//	Iterator(const std::uint32_t index, const DoubleEndedStackAllocator& owner);
		//	std::uint32_t mIndex{ 0 };
		//	const DoubleEndedStackAllocator* mOwner{ nullptr };
		//};

	public:
		DoubleEndedStackAllocator(std::size_t stackSize_bytes, std::size_t alignment = 0U);
		DoubleEndedStackAllocator(std::size_t topStackSize_bytes, std::size_t bottomStackSize_bytes, std::size_t alignment = 0U);
		DoubleEndedStackAllocator(const DoubleEndedStackAllocator&) = delete;
		DoubleEndedStackAllocator(DoubleEndedStackAllocator&& other);
		DoubleEndedStackAllocator& operator=(const DoubleEndedStackAllocator&) = delete;
		DoubleEndedStackAllocator& operator=(DoubleEndedStackAllocator&& other);
		~DoubleEndedStackAllocator();

		virtual void* allocate(std::size_t) override { return nullptr; };
		void* allocateTop(std::size_t allocateAmount_bytes);
		void* allocateBottom(std::size_t allocateAmount_bytes);
		virtual void deallocate() override { return; };
		void dallocateTop();
		void dallocateBottom();

		const void * const GetStackTop() const;
		const void * const GetStackBottom() const;

		void Clear();
		void ClearTop();
		void ClearBottom();

		std::size_t StackSize_Bytes() const;
		std::size_t TopStackSize_Bytes() const;
		std::size_t BottomStackSize_Bytes() const;

		//Iterator begin();
		//Iterator end();

	private:
		std::size_t mTopStackSize_bytes{ 0 };
		std::size_t mBottomStackSize_bytes{ 0 };
		std::size_t mAlignment{ 0 };
		std::intptr_t* mAllocLocationsTop{ nullptr };
		std::intptr_t* mAllocLocationsBottom{ nullptr };
		std::uint32_t mTopIndex{ 0 };
		std::uint32_t mBottomIndex{ 0 };
		void* mTopStackStart{ nullptr };
		void* mBottomStackStart{ nullptr };
		void* mTopStackCurrent{ nullptr };
		void* mBottomStackCurrent{ nullptr };
	};








#pragma region Old
	//class DoubleEndedStackAllocator final
	//{
	////public:
	////	class Iterator final
	////	{
	////		friend class DoubleEndedStackAllocator;
	////	public:
	////		Iterator() = default;
	////		Iterator& operator++();
	////		Iterator& operator++(int);
	////		const void * const operator*() const;

	////	private:
	////		Iterator(const std::uint32_t index, const DoubleEndedStackAllocator& owner);
	////		std::uint32_t mIndex{ 0 };
	////		const DoubleEndedStackAllocator* mOwner{ nullptr };
	////	};

	//public:
	//	DoubleEndedStackAllocator(std::size_t stackSize_bytes, std::size_t alignment = 0U);
	//	DoubleEndedStackAllocator(std::size_t topStackSize_bytes, std::size_t bottomStackSize_bytes, std::size_t alignment = 0U);
	//	DoubleEndedStackAllocator(const DoubleEndedStackAllocator&) = delete;
	//	DoubleEndedStackAllocator(DoubleEndedStackAllocator&& other) = default;
	//	DoubleEndedStackAllocator& operator=(const DoubleEndedStackAllocator&) = delete;
	//	DoubleEndedStackAllocator& operator=(DoubleEndedStackAllocator&& other) = default;
	//	~DoubleEndedStackAllocator();

	//	void* AllocateTop(std::size_t allocAmount_bytes);
	//	void* AllocateBottom(std::size_t allocAmount_bytes);
	//	void DeallocateTop();
	//	void DeallocateBottom();

	//	const void * const GetStackTop() const;
	//	const void * const GetStackBottom() const;
	//	//const void * const GetStackStart() const;

	//	void Clear();
	//	void ClearTop();
	//	void ClearBottom();

	//	std::size_t StackSize_Bytes() const;
	//	std::size_t TopStackSize_Bytes() const;
	//	std::size_t BottomStackSize_Bytes() const;

	//	StackAllocator::Iterator TopBegin();
	//	StackAllocator::Iterator TopEnd();
	//	StackAllocator::Iterator BottomBegin();
	//	StackAllocator::Iterator BottomEnd();

	//private:
	//	//virtual void* allocate(std::size_t allocAmount_bytes) override;
	//	//virtual void deallocate() override;

	//	StackAllocator mTopStack;
	//	StackAllocator mBottomStack;
	//};
#pragma endregion Old
}