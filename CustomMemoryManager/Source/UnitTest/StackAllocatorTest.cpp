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
#ifdef _DEBUG
			Assert::IsFalse(stackAllocator.IsStackOverflow());
#endif //_DEBUG

			std::uint32_t* x = static_cast<std::uint32_t*>(stackAllocator.allocate(sizeof(std::uint32_t)));
			*x = 1;
			//Assert::IsNotNull(x);
			//Assert::AreEqual((std::size_t)1U, stackAllocator.StackSize_NumObjects());
			Assert::AreEqual(1U, *x);

			std::uint32_t* y = static_cast<std::uint32_t*>(stackAllocator.allocate(sizeof(std::uint32_t)));
			*y = 2;
			//Assert::IsNotNull(y);
			//Assert::AreEqual((std::size_t)2U, stackAllocator.StackSize_NumObjects());
			Assert::AreEqual(1U, *x);
			Assert::AreEqual(2U, *y);

			Assert::AreEqual((std::size_t)8U, stackAllocator.UsedSpace_Bytes());
#ifdef _DEBUG
			Assert::IsFalse(stackAllocator.IsStackOverflow());
#endif //_DEBUG

			std::uint32_t* z = static_cast<std::uint32_t*>(stackAllocator.allocate(sizeof(std::uint32_t)));
			*z = 44;
#ifdef _DEBUG
			Assert::IsTrue(stackAllocator.IsStackOverflow());
#endif //_DEBUG
			//Assert::IsNull(z);
			//Assert::AreEqual((std::size_t)2U, stackAllocator.StackSize_NumObjects());
			Assert::AreEqual(1U, *x);
			Assert::AreEqual(2U, *y);

			stackAllocator.Clear();
			//Assert::AreEqual((std::size_t)0U, stackAllocator.StackSize_NumObjects());
			Assert::AreEqual((std::size_t)0U, stackAllocator.UsedSpace_Bytes());

			std::uint64_t* w = static_cast<std::uint64_t*>(stackAllocator.allocate(sizeof(std::uint64_t)));
			//Assert::IsNotNull(w);
			*w = 64U;
			Assert::IsTrue(64U == *w);
			//Assert::AreEqual((std::size_t)1U, stackAllocator.StackSize_NumObjects());

			bool* another = static_cast<bool*>(stackAllocator.allocate(sizeof(bool)));
			//Assert::IsNull(another);
			another;

			Assert::AreNotEqual((std::size_t)8U, stackAllocator.UsedSpace_Bytes());		// should be 9 b/c of the bool
			//Assert::IsTrue(stackAllocator.IsStackOverflow());
		}

		TEST_METHOD(Deallocate)
		{
			StackAllocator stackAllocator(8U);

			std::uint32_t* x = static_cast<std::uint32_t*>(stackAllocator.allocate(sizeof(std::uint32_t)));
			*x = 256;

			//Assert::IsNotNull(x);
			//Assert::AreEqual((std::size_t)1U, stackAllocator.StackSize_NumObjects());
			stackAllocator.deallocate();
			//Assert::AreEqual((std::size_t)0U, stackAllocator.StackSize_NumObjects());
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
			
			
			Foo* ptr3 = static_cast<Foo*>(new ("Stack1", memoryManager, MemoryManager::AllocType::STACK) Foo());
			ptr3->~Foo();
			operator delete (ptr3, "Stack1", memoryManager, MemoryManager::AllocType::STACK);
			//memoryManager.Deallocate(ptr3, "Stack1", MemoryManager::AllocType::STACK);
			
			
			//MemPtr<Foo> memPtr2 = static_cast<Foo*>(new ("Stack1", memoryManager, MemoryManager::AllocType::STACK) Foo());
			//memPtr2->~Foo();
			//memoryManager.Deallocate(memPtr2.Get(), "Stack1", MemoryManager::AllocType::STACK);

			//
			//MemPtr<Foo> memPtr = MakeMemPtr_Raw<Foo>("Stack1", MemoryManager::AllocType::STACK, memoryManager);
			//memPtr->~Foo();
			//memoryManager.Deallocate(memPtr.Get(), "Stack1", MemoryManager::AllocType::STACK);
		}

		TEST_METHOD(StackOverFlow)
		{

		}

	private:
		_CrtMemState StackAllocatorTest::sStartMemState;
	};
}