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
		HeapNode* currentNode = mInActiveLocationsList_Head;
		void* outPtr = nullptr;
		while (currentNode != nullptr)
		{
			// If found a node large enough
			if (currentNode->mSize_Bytes >= allocAmount_bytes)
			{
				// Create a node with remaining space
				HeapNode* temp = new HeapNode(
					{
						currentNode->mSize_Bytes - allocAmount_bytes,													// new available size
						static_cast<void*>(static_cast<std::intptr_t*>(currentNode->mPtr) + allocAmount_bytes),			// new available location
						nullptr,																						// new previous location
						nullptr																							// new next location
					}
				);
				// Append Node
				//mInActiveLocationsList_End->mNext = temp;
				//temp->mPrevious = mInActiveLocationsList_End;

				PushBackNode(temp, mInActiveLocationsList_End);

				// Cache node to remove
				temp = mInActiveLocationsList_Head;
				mInActiveLocationsList_Head = mInActiveLocationsList_Head->mNext;		// remove it
				if (mInActiveLocationsList_Head != nullptr)
					mInActiveLocationsList_Head->mPrevious = nullptr;

				// Create the requested node to then add to the acive locations list
				temp->mNext = nullptr;
				temp->mPrevious = nullptr;
				temp->mSize_Bytes = allocAmount_bytes;

				// Keep track of heap used size.
				mHeapUsedSize_Bytes += allocAmount_bytes;

				if (mActiveLocationsList_Head == nullptr)
				{
					mActiveLocationsList_Head = temp;
					mActiveLocationsList_End = mActiveLocationsList_Head;
				}
				else
				{
					mActiveLocationsList_End->mNext = temp;
					temp->mPrevious = mActiveLocationsList_End;
				}
				outPtr = temp->mPtr;
				break;
			}
		}

		return outPtr;
	}

	void HeapAllocator::deallocate(void* ptr, Info)
	{
		HeapNode* temp = RemoveNode(ptr, mActiveLocationsList_Head, mActiveLocationsList_End);
		if (temp != nullptr)
		{
			mHeapUsedSize_Bytes -= temp->mSize_Bytes;
			PushBackNode(temp, mInActiveLocationsList_End);
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

	void HeapAllocator::PushBackNode(HeapNode* node, HeapNode* end)
	{
		if (end == nullptr)
		{
			return;
		}

		assert(end != nullptr);

		end->mNext = node;
		node->mPrevious = end;
		node->mNext = nullptr;
		end = node;
	}

	HeapAllocator::HeapNode* HeapAllocator::RemoveNode(void* ptr, HeapNode* head, HeapNode* end)
	{
		// Nothing has Been Allocated.
		if (head == nullptr && end == nullptr)
		{
			return nullptr;
		}

		// Only One Node in the List.
		if (head == end && head->mPtr == ptr)
		{
			HeapNode* temp = head;
			head = nullptr;
			end = nullptr;
			temp->mPrevious = nullptr;
			temp->mNext = nullptr;
			return temp;
		}

		// Check the Head Node.
		HeapNode* current = end;
		if (current->mPtr == ptr)
		{
			current->mPrevious->mNext = nullptr;
			end = current->mPrevious;
			current->mPrevious = nullptr;
			return current;
		}

		// Check the End Node.
		current = head;
		if (current->mPtr == ptr)
		{
			current->mNext->mPrevious = nullptr;
			head = current->mNext;
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

	HeapAllocator::HeapNode* HeapAllocator::FindNode(void* ptr, HeapNode* head, HeapNode* end)
	{
		// Empty.
		if (head == nullptr && end == nullptr)
		{
			return nullptr;
		}

		// One Item.
		if (head == end && head->mPtr == ptr)
		{
			return head;
		}

		// Greather Than One Item.
		HeapNode* current = head;
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

	HeapAllocator::HeapNode* HeapAllocator::FindNodeFirstFitSize(std::size_t size_bytes, HeapNode* head, HeapNode* end)
	{
		// Empty.
		if (head == nullptr && end == nullptr)
		{
			return nullptr;
		}

		// One Item.
		if (head == end && head->mSize_Bytes >= size_bytes)
		{
			return head;
		}

		// Greather Than One Item.
		HeapNode* current = head;
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