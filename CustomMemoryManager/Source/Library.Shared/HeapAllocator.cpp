#include "HeapAllocator.h"
#include <cassert>

namespace CustomMemoryManager::Allocators
{
	HeapAllocator::HeapAllocator(std::size_t heapSize_bytes)
		: mHeapSize_Bytes(heapSize_bytes)
	{
		mHeapStart = std::malloc(heapSize_bytes);

		mLocations_Head = new HeapNode({ heapSize_bytes, mHeapStart });	// size, loc
		mLocations_End = mLocations_Head;
		mInActiveLocationsList_Head = mLocations_Head;
		mInActiveLocationsList_End = mLocations_Head;
#ifdef _MEMDEBUG
		++mNumInActiveNodes;
#endif // _MEMDEBUG
	}

	HeapAllocator::~HeapAllocator()
	{
		if (mLocations_Head != nullptr)
		{
			HeapNode* currentNode = mLocations_Head;
			while (currentNode != nullptr)
			{
				HeapNode* temp = currentNode;
				currentNode = currentNode->mMemNext;
				delete temp;
			}
		}
		/*if (mInActiveLocationsList_Head != nullptr)
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
		}*/
		if (mHeapStart != nullptr)
		{
			std::free(mHeapStart);
		}
	}

	void* HeapAllocator::allocate(std::size_t allocAmount_bytes, std::size_t, Info info)
	{
		void* ptr = nullptr;

		if (info == CustomMemoryManager::Allocators::NONE)
		{
			info = CustomMemoryManager::Allocators::HEAP_FIRSTFIT;
		}

		// Search InActive List.
		HeapNode* foundNode = nullptr;
		switch (info)
		{
		case CustomMemoryManager::Allocators::HEAP_FIRSTFIT:
			foundNode = FindNodeFirstFitSize(allocAmount_bytes, &mInActiveLocationsList_Head, &mInActiveLocationsList_End);
			break;
		case CustomMemoryManager::Allocators::HEAP_LASTFIT:
			foundNode = FindNodeLastFitSize(allocAmount_bytes, &mInActiveLocationsList_Head, &mInActiveLocationsList_End);
			break;
		case CustomMemoryManager::Allocators::HEAP_BESTFIT:
			foundNode = FindNodeBestFitSize(allocAmount_bytes, &mInActiveLocationsList_Head, &mInActiveLocationsList_End);
			break;
		default:
			break;
		}

		if (foundNode != nullptr)
		{
			assert(foundNode->mIsActive == false);
			HeapNode* temp = nullptr;
			if (foundNode->mSize_Bytes > allocAmount_bytes)
			{
				// Create a node with remaining space
				if (foundNode->mSize_Bytes - allocAmount_bytes != 0U)
				{
					temp = new HeapNode(
						{
							foundNode->mSize_Bytes - allocAmount_bytes,														// new available size
							reinterpret_cast<void*>(reinterpret_cast<std::intptr_t>(foundNode->mPtr) + allocAmount_bytes)	// new available location
						}
					);

					foundNode->mSize_Bytes = allocAmount_bytes;

					// Push back InActive List.
					MemInsertAfter(temp, &foundNode);
					PushBackNode(temp, &mInActiveLocationsList_Head, &mInActiveLocationsList_End);
#ifdef _MEMDEBUG
					++mNumInActiveNodes;
#endif // _MEMDEBUG
				}
			}

			// Remove InActive List.
			temp = RemoveNode(foundNode->mPtr, &mInActiveLocationsList_Head, &mInActiveLocationsList_End);
			assert(temp != nullptr);

			mHeapUsedSize_Bytes += allocAmount_bytes;

			// Push back Active List.
			PushBackNode(temp, &mActiveLocationsList_Head, &mActiveLocationsList_End);
			//mActive.emplace(temp->mPtr, temp);
			temp->mIsActive = true;
#ifdef _MEMDEBUG
			--mNumInActiveNodes;
			++mNumActiveNodes;
#endif // _MEMDEBUG

			ptr = temp->mPtr;
		}
		return ptr;
	}

	void HeapAllocator::deallocate(void* ptr, Info)
	{
		if (ptr == nullptr)
		{
			return;
		}

		HeapNode* temp = RemoveNode(ptr, &mActiveLocationsList_Head, &mActiveLocationsList_End);
		//HeapNode* temp = nullptr;
		//std::unordered_map<void*, HeapNode*>::iterator tempIt = mActive.at(ptr);
		//if (tempIt == mActive.end())
		//{
			//return;
		//}
		//auto tempIt = mActive.extract(ptr);
		//temp = tempIt.mapped();
		//temp = tempIt->second;
		//mActive.erase(ptr);
		if (temp != nullptr)
		{
			assert(temp->mIsActive == true);
			mHeapUsedSize_Bytes -= temp->mSize_Bytes;
			if (temp->mMemPrevious != nullptr && temp->mMemPrevious->mIsActive == false)
			{
				temp->mMemPrevious->mSize_Bytes += temp->mSize_Bytes;
				temp->mMemPrevious->mMemNext = temp->mMemNext;
				if (temp->mMemNext != nullptr)
					temp->mMemNext->mMemPrevious = temp->mMemPrevious;
				if (temp == mLocations_End)
					mLocations_End = temp->mMemPrevious;
				delete temp;
#ifdef _MEMDEBUG
				--mNumActiveNodes;
#endif // _MEMDEBUG
			}
			else if (temp->mMemNext != nullptr && temp->mMemNext->mIsActive == false)
			{
				temp->mMemNext->mSize_Bytes += temp->mSize_Bytes;
				temp->mMemNext->mPtr = temp->mPtr;
				if (temp->mMemPrevious != nullptr)
					temp->mMemPrevious->mMemNext = temp->mMemNext;
				temp->mMemNext->mMemPrevious = temp->mMemPrevious;
				if (temp == mLocations_Head)
					mLocations_Head = temp->mMemNext;
				delete temp;
#ifdef _MEMDEBUG
				--mNumActiveNodes;
#endif // _MEMDEBUG
			}
			else
			{
				temp->mIsActive = false;
				PushFrontNode(temp, &mInActiveLocationsList_Head, &mInActiveLocationsList_End);
#ifdef _MEMDEBUG
				--mNumActiveNodes;
				++mNumInActiveNodes;
#endif // _MEMDEBUG
			}
			mLocations_Head;
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

#ifdef _MEMDEBUG
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
#endif // _MEMDEBUG

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

	void HeapAllocator::PushFrontNode(HeapNode* node, HeapNode** head, HeapNode** end)
	{
		if (*head == nullptr && *end == nullptr)
		{
			*head = node;
			*end = node;
			node->mNext = nullptr;
			node->mPrevious = nullptr;
			return;
		}
		assert(*head != nullptr);
		(*head)->mPrevious = node;
		node->mNext = *head;
		node->mPrevious = nullptr;
		*head = node;
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
				if (current->mNext == nullptr)
					break;
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
		assert(*head != nullptr && *end != nullptr);

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

	HeapAllocator::HeapNode* HeapAllocator::FindNodeLastFitSize(std::size_t size_bytes, HeapNode** head, HeapNode** end)
	{
		// Empty.
		if (*head == nullptr && *end == nullptr)
		{
			return nullptr;
		}
		assert(*head != nullptr && *end != nullptr);

		// One Item.
		if (*head == *end && (*end)->mSize_Bytes >= size_bytes)
		{
			return *end;
		}

		// Greather Than One Item.
		HeapNode* current = *end;
		while (current != nullptr)
		{
			if (current->mSize_Bytes >= size_bytes)
			{
				break;
			}
			current = current->mPrevious;
		}
		return current;
	}

	HeapAllocator::HeapNode* HeapAllocator::FindNodeBestFitSize(std::size_t size_bytes, HeapNode** head, HeapNode** end)
	{
		// Empty.
		if (*head == nullptr && *end == nullptr)
		{
			return nullptr;
		}
		assert(*head != nullptr && *end != nullptr);

		// One Item.
		if (*head == *end && (*head)->mSize_Bytes >= size_bytes)
		{
			return *head;
		}

		// Greather Than One Item.
		HeapNode* current = *head;
		std::size_t sizebytes_cache = std::numeric_limits<std::size_t>::max();
		HeapNode* found = nullptr;
		while (current != nullptr)
		{
			// if current size is large enough and is less than the previously cached size store it.
			if (current->mSize_Bytes > size_bytes && current->mSize_Bytes < sizebytes_cache)
			{
				sizebytes_cache = current->mSize_Bytes;
				found = current;
			}
			else if (current->mSize_Bytes == size_bytes)
			{
				found = current;
				break;
			}
			current = current->mNext;
		}
		current = found;
		return current;
	}

	void HeapAllocator::MemPushBackNode(HeapNode* node, HeapNode** head, HeapNode** end)
	{
		if (*head == nullptr && *end == nullptr)
		{
			*head = node;
			*end = node;
			node->mMemNext = nullptr;
			node->mMemPrevious = nullptr;
			return;
		}

		assert(*end != nullptr);

		(*end)->mMemNext = node;
		node->mMemPrevious = *end;
		node->mMemNext = nullptr;
		*end = node;
	}

	void HeapAllocator::MemPushFrontNode(HeapNode* node, HeapNode** head, HeapNode** end)
	{
		if (*head == nullptr && *end == nullptr)
		{
			*head = node;
			*end = node;
			node->mMemNext = nullptr;
			node->mMemPrevious = nullptr;
			return;
		}
		assert(*head != nullptr);
		(*head)->mMemPrevious = node;
		node->mMemNext = *head;
		node->mMemPrevious = nullptr;
		*head = node;
	}

	HeapAllocator::HeapNode* HeapAllocator::MemRemoveNode(void* ptr, HeapNode** head, HeapNode** end)
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
			temp->mMemPrevious = nullptr;
			temp->mMemNext = nullptr;
			return temp;
		}

		// Check the Head Node.
		HeapNode* current = *end;
		if (current->mPtr == ptr)
		{
			current->mMemPrevious->mMemNext = nullptr;
			*end = current->mMemPrevious;
			current->mMemPrevious = nullptr;
			return current;
		}

		// Check the End Node.
		current = *head;
		if (current->mPtr == ptr)
		{
			current->mMemNext->mMemPrevious = nullptr;
			*head = current->mMemNext;
			current->mMemNext = nullptr;
			return current;
		}

		// Check the Middle Nodes.
		while (current != nullptr)
		{
			if (current->mPtr == ptr)
			{
				current->mMemPrevious->mMemNext = current->mMemNext;
				current->mMemNext->mMemPrevious = current->mMemPrevious;
				current->mMemNext = nullptr;
				current->mMemPrevious = nullptr;
				break;
			}
			current = current->mMemNext;
		}
		return current;
	}

	HeapAllocator::HeapNode* HeapAllocator::MemFindNode(void* ptr, HeapNode** head, HeapNode** end)
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
			current = current->mMemNext;
		}
		return current;
	}

	HeapAllocator::HeapNode* HeapAllocator::MemFindNodeFirstFitSize(std::size_t size_bytes, HeapNode** head, HeapNode** end)
	{
		// Empty.
		if (*head == nullptr && *end == nullptr)
		{
			return nullptr;
		}

		// One Item.
		if (*head == *end && (*head)->mSize_Bytes >= size_bytes && (*head)->mIsActive == false)
		{
			return *head;
		}

		// Greather Than One Item.
		HeapNode* current = *head;
		while (current != nullptr)
		{
			if (current->mSize_Bytes >= size_bytes && current->mIsActive == false)
			{
				break;
			}
			current = current->mMemNext;
		}
		return current;
	}

	void HeapAllocator::MemInsertAfter(HeapNode* node, HeapNode** nodeAfter)
	{
		if (node == nullptr || nodeAfter == nullptr)
			return;
		if (*nodeAfter == nullptr)
			return;

		if (*nodeAfter == mLocations_End)
		{
			MemPushBackNode(node, &mLocations_Head, &mLocations_End);
			return;
		}

		node->mMemPrevious = *nodeAfter;
		node->mMemNext = (*nodeAfter)->mMemNext;

		assert(node->mMemNext != nullptr);
		assert(node->mMemPrevious != nullptr);
		assert((*nodeAfter)->mMemNext != nullptr);

		(*nodeAfter)->mMemNext->mMemPrevious = node;
		(*nodeAfter)->mMemNext = node;
	}
}