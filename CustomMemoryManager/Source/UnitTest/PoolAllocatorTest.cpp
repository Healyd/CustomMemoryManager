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
		TEST_METHOD_INITIALIZE(Initialize)
		{
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#ifdef _DEBUG
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(Constructor)
		{
			PoolAllocator<std::uint32_t> pool(8U);
		}

		TEST_METHOD(Allocator)
		{
			PoolAllocator<std::uint32_t> pool(2U*sizeof(std::uint32_t));
			
			std::uint32_t* x1 = static_cast<std::uint32_t*>(pool.allocate(1*sizeof(std::uint32_t)));
			Assert::IsNotNull(x1, L"1");

			*x1 = 25U;
			Assert::AreEqual(25U, *x1, L"2");

			std::uint32_t* y1 = static_cast<std::uint32_t*>(pool.allocate(1*sizeof(std::uint32_t)));
			Assert::IsNotNull(y1, L"3");

			std::uint32_t* z1 = static_cast<std::uint32_t*>(pool.allocate(1*sizeof(std::uint32_t)));
			Assert::IsNull(z1, L"4");

			pool.deallocate(x1);

			std::uint32_t* x2 = static_cast<std::uint32_t*>(pool.allocate(1*sizeof(std::uint32_t)));
			Assert::IsNotNull(x2, L"5");
		}

		TEST_METHOD(Allocator_Alternate)
		{
			PoolAllocator<std::uint32_t> pool(sizeof(std::uint32_t)*5U);

			std::uint32_t* x1 = static_cast<std::uint32_t*>(pool.allocate(1*sizeof(std::uint32_t)));
			Assert::IsNotNull(x1, L"1");
			*x1 = 20U;
			std::uint32_t* x2 = static_cast<std::uint32_t*>(pool.allocate(1*sizeof(std::uint32_t)));
			Assert::IsNotNull(x2, L"2");
			*x2 = 21U;
			std::uint32_t* x3 = static_cast<std::uint32_t*>(pool.allocate(1*sizeof(std::uint32_t)));
			Assert::IsNotNull(x3, L"3");
			*x3 = 22U;
			std::uint32_t* x4 = static_cast<std::uint32_t*>(pool.allocate(1*sizeof(std::uint32_t)));
			Assert::IsNotNull(x4, L"4");
			*x4 = 23U;
			std::uint32_t* x5 = static_cast<std::uint32_t*>(pool.allocate(1*sizeof(std::uint32_t)));
			Assert::IsNotNull(x5, L"5");
			*x5 = 24U;

			std::uint32_t* x6 = static_cast<std::uint32_t*>(pool.allocate(1*sizeof(std::uint32_t)));
			Assert::IsNull(x6, L"6");

			pool.deallocate(x3);

			x6 = static_cast<std::uint32_t*>(pool.allocate(1*sizeof(std::uint32_t)));
			Assert::IsNotNull(x6, L"8");

			x3 = static_cast<std::uint32_t*>(pool.allocate(1*sizeof(std::uint32_t)));
			Assert::IsNull(x3, L"9");
		}

		TEST_METHOD(Allocator_Loop)
		{
			PoolAllocator<std::uint32_t> pool(sizeof(std::uint32_t) * 50U);

			std::vector<std::uint32_t*> pointers;

			for (std::uint32_t i = 0U; i < 50U; ++i)
			{
				pointers.push_back(static_cast<std::uint32_t*>(pool.allocate(1 * sizeof(std::uint32_t))));
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
				pointers.push_back(static_cast<std::uint32_t*>(pool.allocate(1 * sizeof(std::uint32_t))));
			}

			Assert::AreEqual((std::size_t)50U, pool.PoolSize_NumObjects());
		}

		TEST_METHOD(Allocator_Array_Simple)
		{
			PoolAllocator<std::uint32_t> pool(50U * sizeof(std::uint32_t));

			std::uint32_t* intArray = static_cast<std::uint32_t*>(pool.allocate(40 * sizeof(std::uint32_t)));
			Assert::IsNotNull(intArray);
			Assert::AreEqual((std::size_t)40, pool.PoolSize_NumObjects());

			std::uint32_t* intArray2 = static_cast<std::uint32_t*>(pool.allocate(11 * sizeof(std::uint32_t)));
			Assert::IsNull(intArray2);
			Assert::AreEqual((std::size_t)40, pool.PoolSize_NumObjects());

			pool.deallocate(intArray, 40);
			Assert::AreEqual((std::size_t)0, pool.PoolSize_NumObjects());
		}

		TEST_METHOD(Allocator_Array)
		{
			PoolAllocator<Foo> fooPool(10 * sizeof(Foo));
			
			Foo* foo1 = static_cast<Foo*>(fooPool.allocate(1 * sizeof(Foo)));
			Assert::IsNotNull(foo1);
			Assert::AreEqual((std::size_t)1, fooPool.PoolSize_NumObjects());

			Foo* foo2 = static_cast<Foo*>(fooPool.allocate(1 * sizeof(Foo)));
			Assert::IsNotNull(foo2);
			Assert::AreEqual((std::size_t)2, fooPool.PoolSize_NumObjects());

			Foo* fooArray1 = static_cast<Foo*>(fooPool.allocate(7 * sizeof(Foo)));
			Assert::IsNotNull(fooArray1);
			Assert::AreEqual((std::size_t)9, fooPool.PoolSize_NumObjects());

			Foo* foo3 = static_cast<Foo*>(fooPool.allocate(1 * sizeof(Foo)));
			Assert::IsNotNull(foo3);
			Assert::AreEqual((std::size_t)10, fooPool.PoolSize_NumObjects());

			fooPool.deallocate(fooArray1, 7);
			Assert::AreEqual((std::size_t)3, fooPool.PoolSize_NumObjects());

			Foo* fooArray2 = static_cast<Foo*>(fooPool.allocate(7 * sizeof(Foo)));
			Assert::IsNotNull(fooArray2);
			Assert::AreEqual((std::size_t)10, fooPool.PoolSize_NumObjects());
		}

			private:
				_CrtMemState PoolAllocatorTest::sStartMemState;
	};
}