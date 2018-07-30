#include "Allocator.h"
#include <deque>
#include <unordered_map>
#include <list>

namespace CustomMemoryManager::Allocators
{
	class HeapAllocator final : public IAllocator
	{
	public:
		HeapAllocator(std::size_t heapSize_bytes);
		HeapAllocator(const HeapAllocator&) = delete;
		HeapAllocator(HeapAllocator&&) = delete;
		HeapAllocator& operator=(const HeapAllocator&) = delete;
		HeapAllocator& operator=(HeapAllocator&&) = delete;
		~HeapAllocator();

		virtual void* allocate(std::size_t allocAmount_bytes, std::size_t alignment = 0U, Info info = Info::HEAP_FIRSTFIT) override;
		virtual void deallocate(void* ptr, Info info = Info::NONE) override;
		virtual std::size_t Size_Bytes() const override;
		virtual std::size_t UsedSize_Bytes() const override;

		std::size_t* IncrementReference(void* ptr);
		std::size_t* DecrementReference(void* ptr);

		void ReferenceCounting_GC();
		
#ifdef _MEMDEBUG
		std::size_t NumNodes() const;
		std::size_t NumActiveNodes() const;
		std::size_t NumInActiveNodes() const;
#endif // _MEMDEBUG

		struct HeapNode
		{
			std::size_t mSize_Bytes{ 0U };
			void* mPtr{ nullptr };
			//void* mMemStart{ nullptr };			// start of the memory chunk for aligned memory blocks
			HeapNode* mPrevious{ nullptr };		// for the mLocationsActive/Inactive lists
			HeapNode* mNext{ nullptr };
			HeapNode* mMemPrevious{ nullptr };	// for the mLocations list
			HeapNode* mMemNext{ nullptr };
			bool mIsActive{ false };
			std::size_t mReferences{ 0U };
		};

	private:
		void PushBackNode(HeapNode* node, HeapNode** head, HeapNode** end);
		void PushFrontNode(HeapNode* node, HeapNode** head, HeapNode** end);
		HeapNode* RemoveNode(void* ptr, HeapNode** head, HeapNode** end);
		HeapNode* FindNode(void* ptr, HeapNode** head, HeapNode** end);
		HeapNode* FindNodeFirstFitSize(std::size_t size_bytes, HeapNode** head, HeapNode** end);
		HeapNode* FindNodeLastFitSize(std::size_t size_bytes, HeapNode** head, HeapNode** end);
		HeapNode* FindNodeBestFitSize(std::size_t size_bytes, HeapNode** head, HeapNode** end);
		HeapNode* FindNodeNextFitSize(std::size_t size_bytes);// , HeapNode** head, HeapNode** end);

		void MemPushBackNode(HeapNode* node, HeapNode** head, HeapNode** end);
		void MemPushFrontNode(HeapNode* node, HeapNode** head, HeapNode** end);
		HeapNode* MemRemoveNode(void* ptr, HeapNode** head, HeapNode** end);
		HeapNode* MemFindNode(void* ptr, HeapNode** head, HeapNode** end);
		HeapNode* MemFindNodeFirstFitSize(std::size_t size_bytes, HeapNode** head, HeapNode** end);
		void MemInsertAfter(HeapNode* node, HeapNode** nodeAfter);

		void* mHeapStart{ nullptr };
		std::size_t mHeapSize_Bytes{ 0U };
		std::size_t mHeapUsedSize_Bytes{ 0U };

#ifdef _MEMDEBUG
		std::size_t mNumActiveNodes{ 0U };
		std::size_t mNumInActiveNodes{ 0U };
#endif // _MEMDEBUG


		HeapNode* mActiveLocationsList_Head{ nullptr };
		HeapNode* mActiveLocationsList_End{ nullptr };
		HeapNode* mInActiveLocationsList_Head{ nullptr };
		HeapNode* mInActiveLocationsList_End{ nullptr };

		HeapNode* mLocations_Head{ nullptr };
		HeapNode* mLocations_End{ nullptr };

		HeapNode* mNextFit{ nullptr };	// should be used only with InActiveLocations

		//std::list<HeapNode> mLocations_Head;

		//std::unordered_map<void*, HeapNode> mInActive;
		std::unordered_map<void*, HeapNode*> mActive;

		std::deque<void*> mInUse;
		std::deque<void*> mNotInUse;
	};
}