#include "CppUnitTest.h"
#include "HeapAllocator.h"
#include "Foo.h"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace CustomMemoryManager::Allocators;

namespace UnitTest
{
	TEST_CLASS(HeapAllocatorTest)
	{
	public:

		TEST_METHOD(Alloc_ThreeIntegers)
		{
			HeapAllocator heap(2*sizeof(std::uint32_t));

			std::uint32_t* ptr1 = static_cast<std::uint32_t*>(heap.allocate(sizeof(std::uint32_t)));
			Assert::IsNotNull(ptr1, L"1");

			std::uint32_t* ptr2 = static_cast<std::uint32_t*>(heap.allocate(sizeof(std::uint32_t)));
			Assert::IsNotNull(ptr2, L"2");

			std::uint32_t* ptr3 = static_cast<std::uint32_t*>(heap.allocate(sizeof(std::uint32_t)));
			Assert::IsNull(ptr3, L"3");
		}

		TEST_METHOD(DeAlloc_Integer)
		{
			HeapAllocator heap(2 * sizeof(std::uint32_t));

			Assert::AreEqual((std::size_t)0U, heap.UsedSize_Bytes(), L"Used Size 1");

			std::uint32_t* ptr1 = static_cast<std::uint32_t*>(heap.allocate(sizeof(std::uint32_t)));
			Assert::IsNotNull(ptr1, L"ptr1 Not Null");

			Assert::AreEqual(sizeof(std::uint32_t), heap.UsedSize_Bytes(), L"Used Size 2");

			heap.deallocate(ptr1);

			Assert::AreEqual((std::size_t)0U, heap.UsedSize_Bytes(), L"Used Size 3");
		}

		TEST_METHOD(AllocDealloc_Foo)
		{
			HeapAllocator heap(40 * sizeof(Foo));

			Foo* fooArray = static_cast<Foo*>(heap.allocate(20 * sizeof(Foo)));
			Assert::IsNotNull(fooArray, L"Foo array null.");

			Assert::AreEqual((std::size_t)(20*sizeof(Foo)), heap.UsedSize_Bytes());

			heap.deallocate(fooArray);

			Assert::AreEqual((std::size_t)(0U), heap.UsedSize_Bytes());

			Foo* foo1 = static_cast<Foo*>(heap.allocate(sizeof(Foo)));
			Assert::IsNotNull(foo1, L"Foo1 null.");
		}

		TEST_METHOD(AllocLoop)
		{
			HeapAllocator heap(400*sizeof(Foo));
			std::vector<Foo*> ptrs;
			for (std::uint32_t i = 0U; i < 4000; ++i)
			{
				ptrs.push_back(static_cast<Foo*>(heap.allocate(sizeof(Foo))));
			}

			heap.allocate(sizeof(Foo));

			for (std::uint32_t i = 0U; i < 4000; ++i)
			{
				heap.deallocate(ptrs.back());
				ptrs.pop_back();
			}
		}

		TEST_METHOD(AllocLoop_string)
		{
			HeapAllocator heap(4000 * sizeof(std::string));
			std::vector<std::string*> ptrs;
			for (std::uint32_t i = 0U; i < 4000; ++i)
			{
				std::string* temp = static_cast<std::string*>(heap.allocate(sizeof(std::string)));
				new (temp) std::string();
				ptrs.push_back(temp);
			}

			heap.allocate(sizeof(std::string));

			for (std::uint32_t i = 0U; i < 4000; ++i)
			{
				ptrs.front()->~basic_string();
				heap.deallocate(ptrs.front());
				ptrs.erase(ptrs.begin());
			}
		}

		TEST_METHOD(NextFit)
		{
			HeapAllocator heap(4 * sizeof(std::uint32_t));

			std::uint32_t* val1 = static_cast<std::uint32_t*>(heap.allocate(sizeof(std::uint32_t), 0U, CustomMemoryManager::Allocators::Info::HEAP_NEXTFIT));
			Assert::IsNotNull(val1);

			std::uint32_t* val2 = static_cast<std::uint32_t*>(heap.allocate(sizeof(std::uint32_t), 0U, CustomMemoryManager::Allocators::Info::HEAP_NEXTFIT));
			Assert::IsNotNull(val2);

			std::uint32_t* val3 = static_cast<std::uint32_t*>(heap.allocate(sizeof(std::uint32_t), 0U, CustomMemoryManager::Allocators::Info::HEAP_NEXTFIT));
			Assert::IsNotNull(val3);

			std::uint32_t* val4 = static_cast<std::uint32_t*>(heap.allocate(sizeof(std::uint32_t), 0U, CustomMemoryManager::Allocators::Info::HEAP_NEXTFIT));
			Assert::IsNotNull(val4);

			heap.deallocate(val1);
			heap.deallocate(val4);

			val1 = static_cast<std::uint32_t*>(heap.allocate(sizeof(std::uint32_t), 0U, CustomMemoryManager::Allocators::Info::HEAP_NEXTFIT));
			Assert::IsNotNull(val1);

			val4 = static_cast<std::uint32_t*>(heap.allocate(sizeof(std::uint32_t), 0U, CustomMemoryManager::Allocators::Info::HEAP_NEXTFIT));
			Assert::IsNotNull(val4);
		}
	};
}