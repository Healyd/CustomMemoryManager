#include "CppUnitTest.h"
#include "HeapAllocator.h"

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
	};
}