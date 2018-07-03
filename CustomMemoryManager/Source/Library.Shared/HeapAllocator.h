#include "Allocator.h"
#include <deque>

namespace CustomMemoryManager::Allocators
{
	class HeapAllocator final : public IAllocator
	{
	public:
		HeapAllocator(std::size_t heapSize_bytes);
		HeapAllocator(const HeapAllocator&) = default;
		HeapAllocator(HeapAllocator&&) = default;
		HeapAllocator& operator=(const HeapAllocator&) = default;
		HeapAllocator& operator=(HeapAllocator&&) = default;
		~HeapAllocator();

		virtual void* allocate(std::size_t allocAmount_bytes, std::size_t alignment = 0U, Info info = Info::NONE) override;
		virtual void deallocate(void* ptr, Info info = Info::NONE) override;
		virtual std::size_t Size_Bytes() const override;
		virtual std::size_t UsedSize_Bytes() const override;

#ifdef MemDebug
		std::size_t NumNodes() const;
		std::size_t NumActiveNodes() const;
		std::size_t NumInActiveNodes() const;
#endif // MemDebug

	private:
		struct HeapNode
		{
			std::size_t mSize_Bytes{ 0U };
			void* mPtr{ nullptr };
			HeapNode* mPrevious{ nullptr };
			HeapNode* mNext{ nullptr };
		};

		void PushBackNode(HeapNode* node, HeapNode** head, HeapNode** end);
		HeapNode* RemoveNode(void* ptr, HeapNode** head, HeapNode** end);
		HeapNode* FindNode(void* ptr, HeapNode** head, HeapNode** end);
		HeapNode* FindNodeFirstFitSize(std::size_t size_bytes, HeapNode** head, HeapNode** end);

		void* mHeapStart{ nullptr };
		std::size_t mHeapSize_Bytes{ 0U };
		std::size_t mHeapUsedSize_Bytes{ 0U };

#ifdef MemDebug
		std::size_t mNumActiveNodes{ 0U };
		std::size_t mNumInActiveNodes{ 0U };
#endif // MemDebug


		HeapNode* mActiveLocationsList_Head{ nullptr };
		HeapNode* mActiveLocationsList_End{ nullptr };
		HeapNode* mInActiveLocationsList_Head{ nullptr };
		HeapNode* mInActiveLocationsList_End{ nullptr };

		
		std::deque<void*> mInUse;
		std::deque<void*> mNotInUse;
	};
}