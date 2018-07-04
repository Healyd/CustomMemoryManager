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
	};
}