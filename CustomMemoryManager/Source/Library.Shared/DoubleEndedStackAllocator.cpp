#include "DoubleEndedStackAllocator.h"

namespace CustomMemoryManager::Allocators
{
	DoubleEndedStackAllocator::DoubleEndedStackAllocator(std::size_t stackSize_bytes, std::size_t alignment)
		: mTopStack(stackSize_bytes / 2, alignment), mBottomStack(stackSize_bytes / 2, alignment)
	{
	}

	DoubleEndedStackAllocator::DoubleEndedStackAllocator(std::size_t topStackSize_bytes, std::size_t bottomStackSize_bytes, std::size_t alignment)
		: mTopStack(topStackSize_bytes, alignment), mBottomStack(bottomStackSize_bytes, alignment)
	{
	}

	void* DoubleEndedStackAllocator::AllocateTop(std::size_t allocAmount_bytes)
	{
		return mTopStack.allocate(allocAmount_bytes);
	}

	void* DoubleEndedStackAllocator::AllocateBottom(std::size_t allocAmount_bytes)
	{
		return mBottomStack.allocate(allocAmount_bytes);
	}

	void DoubleEndedStackAllocator::DeallocateTop()
	{
		mTopStack.deallocate();
	}

	void DoubleEndedStackAllocator::DeallocateBottom()
	{
		mBottomStack.deallocate();
	}

	const void * const DoubleEndedStackAllocator::GetStackTop() const
	{
		return mTopStack.GetStackTop();
	}

	const void * const DoubleEndedStackAllocator::GetStackBottom() const
	{
		return mBottomStack.GetStackTop();
	}

	//const void * const DoubleEndedStackAllocator::GetStackStart() const
	//{
	//	return
	//}

	void DoubleEndedStackAllocator::Clear()
	{
		mTopStack.Clear();
		mBottomStack.Clear();
	}

	void DoubleEndedStackAllocator::ClearTop()
	{
		mTopStack.Clear();
	}

	void DoubleEndedStackAllocator::ClearBottom()
	{
		mBottomStack.Clear();
	}

	std::size_t DoubleEndedStackAllocator::StackSize_Bytes() const
	{
		return mTopStack.StackSize_Bytes() + mBottomStack.StackSize_Bytes();
	}

	std::size_t DoubleEndedStackAllocator::TopStackSize_Bytes() const
	{
		return mTopStack.StackSize_Bytes();
	}

	std::size_t DoubleEndedStackAllocator::BottomStackSize_Bytes() const
	{
		return mBottomStack.StackSize_Bytes();
	}

	StackAllocator::Iterator DoubleEndedStackAllocator::TopBegin()
	{
		return mTopStack.begin();
	}
	
	StackAllocator::Iterator DoubleEndedStackAllocator::TopEnd()
	{
		return mTopStack.end();
	}

	StackAllocator::Iterator DoubleEndedStackAllocator::BottomBegin()
	{
		return mBottomStack.begin();
	}

	StackAllocator::Iterator DoubleEndedStackAllocator::BottomEnd()
	{
		return mBottomStack.end();
	}
}