#include "StackAllocator.h"
#include <cstdlib>
#include <cassert>
#include <exception>

namespace CustomMemoryManager::Allocators
{
	StackAllocator::StackAllocator(std::size_t stackSize_bytes, std::size_t alignment)
		: mStackSize_bytes(stackSize_bytes), mAllignment(alignment)
	{
#ifdef _DEBUG
		mStackStart = std::calloc(1U, stackSize_bytes + DEBUG_EXTRA_SPACE_BYTES);
#else
		mStackStart = std::malloc(stackSize_bytes);
#endif // _DEBUG

		mStackCurrent = mStackStart;
		mAllocLocations = new std::intptr_t[mStackSize_bytes];
	}

	StackAllocator::~StackAllocator()
	{
		if (mStackStart != nullptr)
		{
			std::free(mStackStart);			// Free the Stack.
		}
		if (mAllocLocations != nullptr)
		{
			std::free(mAllocLocations);		// Free the array holding stack address locations.
		}
	}

	StackAllocator::StackAllocator(StackAllocator&& other)
		: mStackSize_bytes(other.mStackSize_bytes),
		mAllignment(other.mAllignment),
		mAllocLocations(other.mAllocLocations),
		mIndex(other.mIndex),
		mStackStart(other.mStackStart),
		mStackCurrent(other.mStackCurrent)//,
		//mNumObjects(other.mNumObjects)
	{
		other.mAllocLocations = nullptr;
		other.mStackStart = nullptr;
		other.mStackCurrent = nullptr;
	}

	StackAllocator& StackAllocator::operator=(StackAllocator&& other)
	{
		if (this != &other)
		{
			mStackSize_bytes = other.mStackSize_bytes;
			mAllignment = other.mAllignment;
			mAllocLocations = other.mAllocLocations;
			mIndex = other.mIndex;
			mStackStart = other.mStackStart;
			mStackCurrent = other.mStackCurrent;
			//mNumObjects = other.mNumObjects;

			other.mAllocLocations = nullptr;
			other.mStackStart = nullptr;
			other.mStackCurrent = nullptr;
		}

		return *this;
	}

	void* StackAllocator::allocate(std::size_t allocAmount_bytes, std::size_t, Info)
	{
		// Cache mStackCurrent as a std::intptr_t.
		std::intptr_t currentStackLocation = reinterpret_cast<std::intptr_t>(mStackCurrent);

		// Determine the stack space we have to work with.
		std::uintptr_t usedSpace = currentStackLocation - reinterpret_cast<std::intptr_t>(mStackStart);
		std::uintptr_t newSpace = usedSpace + allocAmount_bytes;
		newSpace; usedSpace;

		// Too large: then no stack allocation.
		if (newSpace > mStackSize_bytes)
		{
			//throw std::exception("Stack Overflow!");
			return nullptr;
		}

		// Store the address to this alloc location.
		mAllocLocations[mIndex] = currentStackLocation;
		++mIndex;
		//++mNumObjects;

		// Get the current stack top location.
		void* temp = mStackCurrent;
		mStackCurrent = reinterpret_cast<void*>(currentStackLocation + allocAmount_bytes);
		return temp;
	}

	void StackAllocator::deallocate(void*, Info)
	{
		if (mIndex <= 0U)
		{
			return;
			//throw std::exception("The Stack is Empty!");
		}
		mStackCurrent = reinterpret_cast<void*>(mAllocLocations[--mIndex]);

		//--mNumObjects;
	}

	void StackAllocator::deallocate()
	{
		deallocate(nullptr);
	}

	const void * const StackAllocator::GetStackTop() const
	{
		return mStackCurrent;
	}

	void StackAllocator::Clear()
	{
		// TODO: have objects call their destructors here
		mStackCurrent = mStackStart;
		//mNumObjects = 0U;
		mIndex = 0U;
	}

	std::size_t StackAllocator::StackSize_Bytes() const
	{
		return mStackSize_bytes;
	}

	std::size_t StackAllocator::StackSize_NumObjects() const
	{
		return mNumObjects;
	}

	std::size_t StackAllocator::UsedSpace_Bytes() const
	{
		return reinterpret_cast<std::size_t>(mStackCurrent) - reinterpret_cast<std::size_t>(mStackStart);
	}

	const void * const StackAllocator::StackStart() const
	{
		return mStackStart;
	}

#ifdef _DEBUG
	bool StackAllocator::IsStackOverflow() const
	{
		std::size_t endOfStack = reinterpret_cast<std::size_t>(mStackStart) + mStackSize_bytes;
		//std::size_t endOfDebugStack = endOfStack + DEBUG_EXTRA_SPACE_BYTES;

		for (std::size_t i = 0U; i < DEBUG_EXTRA_SPACE_BYTES; i+=sizeof(std::intptr_t))
		{
			std::intptr_t* ptr = reinterpret_cast<std::intptr_t*>(endOfStack + i);
			if (*ptr != NULL)
			{
				return true;
			}
		}
		return false;
	}
#endif

	StackAllocator::Iterator StackAllocator::begin()
	{
		return Iterator(0U, *this);
	}

	StackAllocator::Iterator StackAllocator::end()
	{
		return Iterator(mNumObjects, *this);
	}

#pragma region Iterator
	StackAllocator::Iterator::Iterator(const std::uint32_t index, const StackAllocator& owner)
		: mIndex(index), mOwner(&owner)
	{
	}

	StackAllocator::Iterator& StackAllocator::Iterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::exception("No Owner!");
		}
		if (mIndex < mOwner->mNumObjects)
		{
			++mIndex;
		}
		return *this;
	}

	StackAllocator::Iterator StackAllocator::Iterator::operator++(int)
	{
		Iterator temp = *this;
		operator++();
		return temp;
	}

	const void* const StackAllocator::Iterator::operator*() const
	{
		if (mOwner == nullptr)
		{
			throw std::exception("No Owner!");
		}
		if (mOwner->mIndex == 0U)
		{
			throw std::exception("No Data on the Stack!");
		}
		return reinterpret_cast<void*>(mOwner->mAllocLocations[mIndex]);
	}
#pragma endregion Iterator
}