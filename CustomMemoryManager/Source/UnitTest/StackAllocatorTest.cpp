#include "CppUnitTest.h"
#include "StackAllocator.h"
#include "TypeStackAllocator.h"
#include "MemoryManager.h"
#include "MemPtr.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace CustomMemoryManager;
using namespace CustomMemoryManager::Allocators;

namespace UnitTest
{
	TEST_CLASS(StackAllocatorTest)
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
			StackAllocator stackAllocator(4U);

			Assert::AreEqual((std::size_t)4U, stackAllocator.StackSize_Bytes());
		}

		TEST_METHOD(Alloc)
		{
			StackAllocator stackAllocator(8U);

			std::uint32_t* x = static_cast<std::uint32_t*>(stackAllocator.allocate(sizeof(std::uint32_t)));
			*x = 1;
			Assert::IsNotNull(x);
			Assert::AreEqual((std::size_t)1U, stackAllocator.StackSize_NumObjects());
			Assert::AreEqual(1U, *x);

			std::uint32_t* y = static_cast<std::uint32_t*>(stackAllocator.allocate(sizeof(std::uint32_t)));
			*y = 2;
			Assert::IsNotNull(y);
			Assert::AreEqual((std::size_t)2U, stackAllocator.StackSize_NumObjects());
			Assert::AreEqual(1U, *x);
			Assert::AreEqual(2U, *y);

			std::uint32_t* z = static_cast<std::uint32_t*>(stackAllocator.allocate(sizeof(std::uint32_t)));
			Assert::IsNull(z);
			Assert::AreEqual((std::size_t)2U, stackAllocator.StackSize_NumObjects());
			Assert::AreEqual(1U, *x);
			Assert::AreEqual(2U, *y);

			stackAllocator.Clear();
			Assert::AreEqual((std::size_t)0U, stackAllocator.StackSize_NumObjects());

			std::uint64_t* w = static_cast<std::uint64_t*>(stackAllocator.allocate(sizeof(std::uint64_t)));
			Assert::IsNotNull(w);
			*w = 64U;
			Assert::IsTrue(64U == *w);
			Assert::AreEqual((std::size_t)1U, stackAllocator.StackSize_NumObjects());

			bool* another = static_cast<bool*>(stackAllocator.allocate(sizeof(bool)));
			Assert::IsNull(another);
		}

		TEST_METHOD(Deallocate)
		{
			StackAllocator stackAllocator(8U);

			std::uint32_t* x = static_cast<std::uint32_t*>(stackAllocator.allocate(sizeof(std::uint32_t)));
			*x = 256;

			Assert::IsNotNull(x);
			Assert::AreEqual((std::size_t)1U, stackAllocator.StackSize_NumObjects());
			stackAllocator.deallocate();
			Assert::AreEqual((std::size_t)0U, stackAllocator.StackSize_NumObjects());
		}

		TEST_METHOD(Iterator_Construction)
		{
			StackAllocator::Iterator it;
			auto exp = [&it] { *it; };
			Assert::ExpectException<std::exception>(exp);

			StackAllocator stackAllocator(8U);
			std::uint32_t* var1 = static_cast<std::uint32_t*>(stackAllocator.allocate(sizeof(std::uint32_t)));
			std::uint32_t* var2 = static_cast<std::uint32_t*>(stackAllocator.allocate(sizeof(std::uint32_t)));
			var1; var2;

			it = stackAllocator.begin();
			const void* x = *it;
			const void* y = stackAllocator.StackStart();
			x; y;
			Assert::AreEqual(*it, stackAllocator.StackStart());
			++it;
			void* z = const_cast<void*>(y);
			const void* yy = *it; yy;
			void* zz = (void*)((std::uint32_t*)z + 1); zz;
			Assert::IsTrue(yy == zz);
		}

		TEST_METHOD(test)
		{
			MemoryManager memoryManager;
			Assert::IsTrue(memoryManager.CreateAllocator("Stack1", 43*sizeof(Foo), MemoryManager::AllocType::STACK), L"Allocator Not Created.");
			//std::uint32_t* x = static_cast<uint32_t*>(memoryManager.Allocate(8U, "Stack1", MemoryManager::AllocType::STACK));
			//std::uint32_t* y = static_cast<uint32_t*>(memoryManager.Allocate(8U, "Stack1", MemoryManager::AllocType::STACK));
			//std::uint32_t* x = new ("Stack1", memoryManager, MemoryManager::AllocType::STACK) uint32_t();
			//std::uint32_t* y = new ("Stack1", memoryManager, MemoryManager::AllocType::STACK) uint32_t();
			//Assert::IsNotNull(x);
			//Assert::IsNull(y);
			//StackAllocator* stack = static_cast<StackAllocator*>(memoryManager.Get("Stack1", MemoryManager::AllocType::STACK));
			//MemPtr<Foo> memPtr2 = new ("Stack1", memoryManager, MemoryManager::AllocType::STACK) Foo();
			MemPtr<Foo> memPtr = MakeMemPtr_Raw<Foo>("Stack1", MemoryManager::AllocType::STACK, memoryManager);
	/*		MemPtr<Foo> memPtr3 = static_cast<Foo*>(stack->allocate(sizeof(Foo)));
			MemPtr<Foo> memPtr4 = static_cast<Foo*>(stack->allocate(sizeof(Foo)));
			Foo* foo = static_cast<Foo*>(stack->allocate(sizeof(Foo)));
			Foo* foo1 = static_cast<Foo*>(stack->allocate(sizeof(Foo)));
			Foo* foo2 = static_cast<Foo*>(malloc(sizeof(Foo)));
			foo1, foo;
			free(foo2);*/

			memPtr->~Foo();
			memoryManager.Deallocate(memPtr.Get(), "Stack1", MemoryManager::AllocType::STACK);
			//MemPtr<Foo> memPtr2 = new ("Stack1", memoryManager, MemoryManager::AllocType::STACK) Foo();
			//Assert::IsNotNull(memPtr);
			std::shared_ptr<Foo> sh;
			//auto exp = [&memPtr] {memPtr.operator->(); };
			//Assert::ExpectException<std::exception>(exp);
		}

	private:
		_CrtMemState StackAllocatorTest::sStartMemState;
	};
}