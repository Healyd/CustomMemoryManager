#include "HeapAllocator.h"
#include <cassert>

namespace CustomMemoryManager::Allocators
{
	HeapAllocator::HeapAllocator(std::size_t heapSize_bytes)
		: mHeapSize_Bytes(heapSize_bytes)
	{
		mHeapStart = std::malloc(heapSize_bytes);

		mInActiveLocationsList_Head = new HeapNode({ heapSize_bytes, mHeapStart, nullptr, nullptr }); // size, loc, prev, next
		mInActiveLocationsList_End = mInActiveLocationsList_Head;
#ifdef MemDebug
		++mNumInActiveNodes;
#endif // MemDebug
	}

	HeapAllocator::~HeapAllocator()
	{
		if (mInActiveLocationsList_Head != nullptr)
		{
			HeapNode* currentNode = mInActiveLocationsList_Head;
			while (currentNode != nullptr)
			{
				HeapNode* temp = currentNode;
				currentNode = currentNode->mNext;
				std::free(temp);
			}
		}
		if (mActiveLocationsList_Head != nullptr)
		{
			HeapNode* currentNode = mActiveLocationsList_Head;
			while (currentNode != nullptr)
			{
				HeapNode* temp = currentNode;
				currentNode = currentNode->mNext;
				std::free(temp);
			}
		}
		if (mHeapStart != nullptr)
		{
			std::free(mHeapStart);
		}
	}

	void* HeapAllocator::allocate(std::size_t allocAmount_bytes, std::size_t, Info)
	{
		void* ptr = nullptr;

		HeapNode* foundNode = FindNodeFirstFitSize(allocAmount_bytes, &mInActiveLocationsList_Head, &mInActiveLocationsList_Head);

		if (foundNode != nullptr)
		{
			HeapNode* temp = nullptr;
			if (foundNode->mSize_Bytes > allocAmount_bytes)
			{
				// Create a node with remaining space
				temp = new HeapNode(
					{
						foundNode->mSize_Bytes - allocAmount_bytes,													// new available size
						static_cast<void*>(static_cast<std::intptr_t*>(foundNode->mPtr) + allocAmount_bytes),		// new available location
						nullptr,																					// new previous location
						nullptr																						// new next location
					}
				);

				foundNode->mSize_Bytes = allocAmount_bytes;

				// Push back InActive List.
				PushBackNode(temp, &mInActiveLocationsList_Head, &mInActiveLocationsList_End);
#ifdef MemDebug
				++mNumInActiveNodes;
#endif // MemDebug
			}

			// Remove InActive List.
			temp = RemoveNode(foundNode->mPtr, &mInActiveLocationsList_Head, &mInActiveLocationsList_End);
			assert(temp != nullptr);

			mHeapUsedSize_Bytes += allocAmount_bytes;

			// Push back Active List.
			PushBackNode(temp, &mActiveLocationsList_Head, &mActiveLocationsList_End);

#ifdef MemDebug
			--mNumInActiveNodes;
			++mNumActiveNodes;
#endif // MemDebug

			ptr = temp->mPtr;
		}
		return ptr;
	}

	void HeapAllocator::deallocate(void* ptr, Info)
	{
		HeapNode* temp = RemoveNode(ptr, &mActiveLocationsList_Head, &mActiveLocationsList_End);
		if (temp != nullptr)
		{
			mHeapUsedSize_Bytes -= temp->mSize_Bytes;
			PushBackNode(temp, &mInActiveLocationsList_Head, &mInActiveLocationsList_End);
#ifdef MemDebug
			--mNumActiveNodes;
			++mNumInActiveNodes;
#endif // MemDebug
		}
	}

	std::size_t HeapAllocator::Size_Bytes() const
	{
		return mHeapSize_Bytes;
	}

	std::size_t HeapAllocator::UsedSize_Bytes() const
	{
		return mHeapUsedSize_Bytes;
	}

#ifdef MemDebug
	std::size_t HeapAllocator::NumNodes() const
	{
		return mNumActiveNodes + mNumInActiveNodes;
	}

	std::size_t HeapAllocator::NumActiveNodes() const
	{
		return mNumActiveNodes;
	}

	std::size_t HeapAllocator::NumInActiveNodes() const
	{
		return mNumInActiveNodes;
	}
#endif // MemDebug


	void HeapAllocator::PushBackNode(HeapNode* node, HeapNode** head, HeapNode** end)
	{
		if (*head == nullptr && *end == nullptr)
		{
			*head = node;
			*end = node;
			node->mNext = nullptr;
			node->mPrevious = nullptr;
			return;
		}

		assert(*end != nullptr);

		(*end)->mNext = node;
		node->mPrevious = *end;
		node->mNext = nullptr;
		*end = node;
	}

	HeapAllocator::HeapNode* HeapAllocator::RemoveNode(void* ptr, HeapNode** head, HeapNode** end)
	{
		// Nothing has Been Allocated.
		if (*head == nullptr && *end == nullptr)
		{
			return nullptr;
		}

		// Only One Node in the List.
		if (*head == *end && (*head)->mPtr == ptr)
		{
			HeapNode* temp = *head;
			*head = nullptr;
			*end = nullptr;
			temp->mPrevious = nullptr;
			temp->mNext = nullptr;
			return temp;
		}

		// Check the Head Node.
		HeapNode* current = *end;
		if (current->mPtr == ptr)
		{
			current->mPrevious->mNext = nullptr;
			*end = current->mPrevious;
			current->mPrevious = nullptr;
			return current;
		}

		// Check the End Node.
		current = *head;
		if (current->mPtr == ptr)
		{
			current->mNext->mPrevious = nullptr;
			*head = current->mNext;
			current->mNext = nullptr;
			return current;
		}

		// Check the Middle Nodes.
		while (current != nullptr)
		{
			if (current->mPtr == ptr)
			{
				current->mPrevious->mNext = current->mNext;
				current->mNext->mPrevious = current->mPrevious;
				current->mNext = nullptr;
				current->mPrevious = nullptr;
				break;
			}
			current = current->mNext;
		}
		return current;
	}

	HeapAllocator::HeapNode* HeapAllocator::FindNode(void* ptr, HeapNode** head, HeapNode** end)
	{
		// Empty.
		if (*head == nullptr && *end == nullptr)
		{
			return nullptr;
		}

		// One Item.
		if (*head == *end && (*head)->mPtr == ptr)
		{
			return *head;
		}

		// Greather Than One Item.
		HeapNode* current = *head;
		while (current != nullptr)
		{
			if (current->mPtr == ptr)
			{
				break;
			}
			current = current->mNext;
		}
		return current;
	}

	HeapAllocator::HeapNode* HeapAllocator::FindNodeFirstFitSize(std::size_t size_bytes, HeapNode** head, HeapNode** end)
	{
		// Empty.
		if (*head == nullptr && *end == nullptr)
		{
			return nullptr;
		}

		// One Item.
		if (*head == *end && (*head)->mSize_Bytes >= size_bytes)
		{
			return *head;
		}

		// Greather Than One Item.
		HeapNode* current = *head;
		while (current != nullptr)
		{
			if (current->mSize_Bytes >= size_bytes)
			{
				break;
			}
			current = current->mNext;
		}
		return current;
	}
}