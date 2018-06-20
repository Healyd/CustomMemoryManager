#include "CppUnitTest.h"
#include "PoolAllocator.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace CustomMemoryManager::Allocators;

namespace UnitTest
{
	TEST_CLASS(PoolAllocatorTest)
	{
	public:

		TEST_METHOD(Constructor)
		{
			PoolAllocator<std::uint32_t> pool(8U);
		}

		TEST_METHOD(Allocator)
		{
			PoolAllocator<std::uint32_t> pool(8U);
			
			std::uint32_t* x1 = static_cast<std::uint32_t*>(pool.allocate(1));
			Assert::IsNotNull(x1, L"1");

			*x1 = 25U;
			Assert::AreEqual(25U, *x1, L"2");

			std::uint32_t* y1 = static_cast<std::uint32_t*>(pool.allocate(1));
			Assert::IsNotNull(y1, L"3");

			std::uint32_t* z1 = static_cast<std::uint32_t*>(pool.allocate(1));
			Assert::IsNull(z1, L"4");

			pool.deallocate(x1);

			std::uint32_t* x2 = static_cast<std::uint32_t*>(pool.allocate(1));
			Assert::IsNotNull(x2, L"5");
		}

		TEST_METHOD(Allocator_Alternate)
		{
			PoolAllocator<std::uint32_t> pool(sizeof(std::uint32_t)*5U);

			std::uint32_t* x1 = static_cast<std::uint32_t*>(pool.allocate(1));
			Assert::IsNotNull(x1, L"1");
			*x1 = 20U;
			std::uint32_t* x2 = static_cast<std::uint32_t*>(pool.allocate(1));
			Assert::IsNotNull(x2, L"2");
			*x2 = 21U;
			std::uint32_t* x3 = static_cast<std::uint32_t*>(pool.allocate(1));
			Assert::IsNotNull(x3, L"3");
			*x3 = 22U;
			std::uint32_t* x4 = static_cast<std::uint32_t*>(pool.allocate(1));
			Assert::IsNotNull(x4, L"4");
			*x4 = 23U;
			std::uint32_t* x5 = static_cast<std::uint32_t*>(pool.allocate(1));
			Assert::IsNotNull(x5, L"5");
			*x5 = 24U;

			std::uint32_t* x6 = static_cast<std::uint32_t*>(pool.allocate(1));
			Assert::IsNull(x6, L"6");

			pool.deallocate(x3);

			x6 = static_cast<std::uint32_t*>(pool.allocate(1));
			Assert::IsNotNull(x6, L"8");

			x3 = static_cast<std::uint32_t*>(pool.allocate(1));
			Assert::IsNull(x3, L"9");
		}

		TEST_METHOD(Allocator_Loop)
		{
			PoolAllocator<std::uint32_t> pool(sizeof(std::uint32_t) * 50U);

			std::vector<std::uint32_t*> pointers;

			for (std::uint32_t i = 0U; i < 50U; ++i)
			{
				pointers.push_back(static_cast<std::uint32_t*>(pool.allocate(1)));
			}

			Assert::AreEqual((std::size_t)50U, pool.PoolSize_NumObjects());

			std::uint32_t* ptr = pointers.back();
			pointers.pop_back();
			pool.deallocate(ptr);
			Assert::AreEqual((std::size_t)49U, pool.PoolSize_NumObjects());

			ptr = pointers.back();
			pointers.pop_back();
			pool.deallocate(ptr);
			Assert::AreEqual((std::size_t)48U, pool.PoolSize_NumObjects());

			ptr = pointers.back();
			pointers.pop_back();
			pool.deallocate(ptr);
			Assert::AreEqual((std::size_t)47U, pool.PoolSize_NumObjects());

			for (std::uint32_t i = 0U; i < 3U; ++i)
			{
				pointers.push_back(static_cast<std::uint32_t*>(pool.allocate(1)));
			}

			Assert::AreEqual((std::size_t)50U, pool.PoolSize_NumObjects());
		}
	};
}