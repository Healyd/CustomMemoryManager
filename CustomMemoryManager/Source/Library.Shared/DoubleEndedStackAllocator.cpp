#include "DoubleEndedStackAllocator.h"
#include <exception>


namespace CustomMemoryManager::Allocators
{
	DoubleEndedStackAllocator::DoubleEndedStackAllocator(std::size_t stackSize_bytes, std::size_t alignment)
		: DoubleEndedStackAllocator(stackSize_bytes / 2, stackSize_bytes / 2, alignment)
	{
	}

	DoubleEndedStackAllocator::DoubleEndedStackAllocator(std::size_t topStackSize_bytes, std::size_t bottomStackSize_bytes, std::size_t alignment)
		: mTopStackSize_bytes(topStackSize_bytes), mBottomStackSize_bytes(bottomStackSize_bytes), mAlignment(alignment)
	{
#ifdef _DEBUG
		mBottomStackStart = std::calloc(1U, topStackSize_bytes + bottomStackSize_bytes);
#else
		mBottomStackStart = std::malloc(topStackSize_bytes + bottomStackSize_bytes);
#endif // _DEBUG

		mBottomStackCurrent = mBottomStackStart;
		mAllocLocationsBottom = new std::intptr_t[mBottomStackSize_bytes];

		mTopStackCurrent = reinterpret_cast<void*>(reinterpret_cast<std::intptr_t>(mBottomStackStart) + topStackSize_bytes + bottomStackSize_bytes);
		mTopStackStart = mTopStackCurrent;
		mAllocLocationsTop = new std::intptr_t[mTopStackSize_bytes];
	}

	DoubleEndedStackAllocator::DoubleEndedStackAllocator(DoubleEndedStackAllocator&& other)
		: mTopStackSize_bytes(other.mTopStackSize_bytes),
		mBottomStackSize_bytes(other.mBottomStackSize_bytes),
		mAlignment(other.mAlignment),
		mAllocLocationsTop(other.mAllocLocationsTop),
		mAllocLocationsBottom(other.mAllocLocationsBottom),
		mTopIndex(other.mTopIndex),
		mBottomIndex(other.mBottomIndex),
		mTopStackStart(other.mTopStackStart),
		mBottomStackStart(other.mBottomStackStart),
		mTopStackCurrent(other.mTopStackCurrent),
		mBottomStackCurrent(other.mBottomStackCurrent)
	{
		other.mAllocLocationsTop = nullptr;
		other.mAllocLocationsBottom = nullptr;
		other.mTopStackStart = nullptr;
		other.mBottomStackStart = nullptr;
		other.mTopStackCurrent = nullptr;
		other.mBottomStackCurrent = nullptr;
	}

	DoubleEndedStackAllocator& DoubleEndedStackAllocator::operator=(DoubleEndedStackAllocator&& other)
	{
		if (this != &other)
		{
			mTopStackSize_bytes = other.mTopStackSize_bytes;
			mBottomStackSize_bytes = other.mBottomStackSize_bytes;
			mAlignment = other.mAlignment;
			mAllocLocationsTop = other.mAllocLocationsTop;
			mAllocLocationsBottom = other.mAllocLocationsBottom;
			mTopIndex = other.mTopIndex;
			mBottomIndex = other.mBottomIndex;
			mTopStackStart = other.mTopStackStart;
			mBottomStackStart = other.mBottomStackStart;
			mTopStackCurrent = other.mTopStackCurrent;
			mBottomStackCurrent = other.mBottomStackCurrent;

			other.mAllocLocationsTop = nullptr;
			other.mAllocLocationsBottom = nullptr;
			other.mTopStackStart = nullptr;
			other.mBottomStackStart = nullptr;
			other.mTopStackCurrent = nullptr;
			other.mBottomStackCurrent = nullptr;
		}
		return *this;
	}

	DoubleEndedStackAllocator::~DoubleEndedStackAllocator()
	{
		if (mBottomStackStart != nullptr)
		{
			std::free(mBottomStackStart);
		}
		if (mAllocLocationsTop != nullptr)
		{
			std::free(mAllocLocationsTop);
		}
		if (mAllocLocationsBottom != nullptr)
		{
			std::free(mAllocLocationsBottom);
		}
	}

	void* DoubleEndedStackAllocator::allocate(std::size_t allocAmount_bytes, std::size_t, Info info)
	{
		if (info == Info::TOP)
		{
			return allocateTop(allocAmount_bytes);
		}
		else if (info == Info::BOTTOM)
		{
			return allocateBottom(allocAmount_bytes);
		}
		return nullptr;
	}

	void* DoubleEndedStackAllocator::allocateTop(std::size_t allocAmount_bytes)
	{
		// Cache mTopStackCurrent as a std::intptr_t.
		std::intptr_t currentStackLocation = reinterpret_cast<std::intptr_t>(mTopStackCurrent);

		// Determine the stack space we have to work with.
		std::uintptr_t usedSpace = reinterpret_cast<std::intptr_t>(mTopStackStart) - currentStackLocation;
		std::uintptr_t newSpace = usedSpace + allocAmount_bytes;

		// Too large: then no stack allocation.
		if (newSpace > mTopStackSize_bytes)
		{
			//throw std::exception("Stack Overflow!");
			return nullptr;
		}

		mTopStackCurrent = reinterpret_cast<void*>(currentStackLocation - allocAmount_bytes);

		// Store the address to this alloc location.
		mAllocLocationsTop[mTopIndex] = currentStackLocation;// reinterpret_cast<std::intptr_t>(mTopStackCurrent);
		++mTopIndex;

		// Get the current stack top location.
		return mTopStackCurrent;
	}

	void* DoubleEndedStackAllocator::allocateBottom(std::size_t allocAmount_bytes)
	{
		// Cache mStackCurrent as a std::intptr_t.
		std::intptr_t currentStackLocation = reinterpret_cast<std::intptr_t>(mBottomStackCurrent);

		// Determine the stack space we have to work with.
		std::uintptr_t usedSpace = currentStackLocation - reinterpret_cast<std::intptr_t>(mBottomStackStart);
		std::uintptr_t newSpace = usedSpace + allocAmount_bytes;

		// Too large: then no stack allocation.
		if (newSpace > mBottomStackSize_bytes)
		{
			//throw std::exception("Stack Overflow!");
			return nullptr;
		}

		// Store the address to this alloc location.
		mAllocLocationsBottom[mBottomIndex] = currentStackLocation;
		++mBottomIndex;

		// Get the current stack top location.
		void* temp = mBottomStackCurrent;
		mBottomStackCurrent = reinterpret_cast<void*>(currentStackLocation + allocAmount_bytes);
		return temp;
	}

	void DoubleEndedStackAllocator::deallocate(void*, Info info)
	{
		if (info == Info::TOP)
		{
			dallocateTop();
		}
		else if (info == Info::BOTTOM)
		{
			dallocateBottom();
		}
	}

	void DoubleEndedStackAllocator::dallocateTop()
	{
		if (mTopIndex <= 0U)
		{
			return;
			//throw std::exception("The Top Stack is Empty!");
		}
		mTopStackCurrent = reinterpret_cast<void*>(mAllocLocationsTop[--mTopIndex]);
	}

	void DoubleEndedStackAllocator::dallocateBottom()
	{
		if (mBottomIndex <= 0U)
		{
			return;
			//throw std::exception("The Bottom Stack is Empty!");
		}
		mBottomStackCurrent = reinterpret_cast<void*>(mAllocLocationsBottom[--mBottomIndex]);
	}

	const void * const DoubleEndedStackAllocator::GetStackTop() const
	{
		return mTopStackStart;
	}

	const void * const DoubleEndedStackAllocator::GetStackBottom() const
	{
		return mBottomStackStart;
	}

	void DoubleEndedStackAllocator::Clear()
	{
		// TODO: have objects call their destructors here
		mTopStackCurrent = mTopStackStart;
		mBottomStackCurrent = mBottomStackStart;
		mTopIndex = 0U;
		mBottomIndex = 0U;
	}

	void DoubleEndedStackAllocator::ClearTop()
	{
		mTopStackCurrent = mTopStackStart;
		mTopIndex = 0U;
	}

	void DoubleEndedStackAllocator::ClearBottom()
	{
		mBottomStackCurrent = mBottomStackStart;
		mBottomIndex = 0U;
	}

	std::size_t DoubleEndedStackAllocator::StackSize_Bytes() const
	{
		return mTopStackSize_bytes + mBottomStackSize_bytes;
	}

	std::size_t DoubleEndedStackAllocator::TopStackSize_Bytes() const
	{
		return mTopStackSize_bytes;
	}

	std::size_t DoubleEndedStackAllocator::BottomStackSize_Bytes() const
	{
		return mBottomStackSize_bytes;
	}

	std::size_t DoubleEndedStackAllocator::UsedBytes() const
	{
		return UsedBytes_Top() + UsedBytes_Bottom();
	}

	std::size_t DoubleEndedStackAllocator::UsedBytes_Top() const
	{
		return reinterpret_cast<std::size_t>(mTopStackStart) - reinterpret_cast<std::size_t>(mTopStackCurrent);
	}

	std::size_t DoubleEndedStackAllocator::UsedBytes_Bottom() const
	{
		return reinterpret_cast<std::size_t>(mBottomStackCurrent) - reinterpret_cast<std::size_t>(mBottomStackStart);
	}


#pragma region Old
	//DoubleEndedStackAllocator::DoubleEndedStackAllocator(std::size_t stackSize_bytes, std::size_t alignment)
	//	: mTopStack(stackSize_bytes / 2, alignment), mBottomStack(stackSize_bytes / 2, alignment)
	//{
	//}

	//DoubleEndedStackAllocator::DoubleEndedStackAllocator(std::size_t topStackSize_bytes, std::size_t bottomStackSize_bytes, std::size_t alignment)
	//	: mTopStack(topStackSize_bytes, alignment), mBottomStack(bottomStackSize_bytes, alignment)
	//{
	//}

	//void* DoubleEndedStackAllocator::AllocateTop(std::size_t allocAmount_bytes)
	//{
	//	return mTopStack.allocate(allocAmount_bytes);
	//}

	//void* DoubleEndedStackAllocator::AllocateBottom(std::size_t allocAmount_bytes)
	//{
	//	return mBottomStack.allocate(allocAmount_bytes);
	//}

	//void DoubleEndedStackAllocator::DeallocateTop()
	//{
	//	mTopStack.deallocate();
	//}

	//void DoubleEndedStackAllocator::DeallocateBottom()
	//{
	//	mBottomStack.deallocate();
	//}

	//const void * const DoubleEndedStackAllocator::GetStackTop() const
	//{
	//	return mTopStack.GetStackTop();
	//}

	//const void * const DoubleEndedStackAllocator::GetStackBottom() const
	//{
	//	return mBottomStack.GetStackTop();
	//}

	////const void * const DoubleEndedStackAllocator::GetStackStart() const
	////{
	////	return
	////}

	//void DoubleEndedStackAllocator::Clear()
	//{
	//	mTopStack.Clear();
	//	mBottomStack.Clear();
	//}

	//void DoubleEndedStackAllocator::ClearTop()
	//{
	//	mTopStack.Clear();
	//}

	//void DoubleEndedStackAllocator::ClearBottom()
	//{
	//	mBottomStack.Clear();
	//}

	//std::size_t DoubleEndedStackAllocator::StackSize_Bytes() const
	//{
	//	return mTopStack.StackSize_Bytes() + mBottomStack.StackSize_Bytes();
	//}

	//std::size_t DoubleEndedStackAllocator::TopStackSize_Bytes() const
	//{
	//	return mTopStack.StackSize_Bytes();
	//}

	//std::size_t DoubleEndedStackAllocator::BottomStackSize_Bytes() const
	//{
	//	return mBottomStack.StackSize_Bytes();
	//}

	//StackAllocator::Iterator DoubleEndedStackAllocator::TopBegin()
	//{
	//	return mTopStack.begin();
	//}
	//
	//StackAllocator::Iterator DoubleEndedStackAllocator::TopEnd()
	//{
	//	return mTopStack.end();
	//}

	//StackAllocator::Iterator DoubleEndedStackAllocator::BottomBegin()
	//{
	//	return mBottomStack.begin();
	//}

	//StackAllocator::Iterator DoubleEndedStackAllocator::BottomEnd()
	//{
	//	return mBottomStack.end();
	//}
#pragma endregion Old
}