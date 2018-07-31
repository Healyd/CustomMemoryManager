#include "CppUnitTest.h"
#include "MemoryManager.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace CustomMemoryManager;

namespace UnitTest
{
	TEST_CLASS(MemoryManagerTest)
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

		TEST_METHOD(CreateStackAllocator)
		{
			MemoryManager manager;
			Assert::IsTrue(manager.CreateAllocator("Stack1", 400, AllocType::STACK), L"Stack not create");
		}

		TEST_METHOD(CreateDoubleStackAllocator)
		{
			MemoryManager manager;
			Assert::IsTrue(manager.CreateAllocator("DoubleStack1", 400, AllocType::DOUBLESTACK), L"Double Stack not create");
		}

		TEST_METHOD(CreatePoolAllocator)
		{
			MemoryManager manager;
			Assert::IsTrue(manager.CreateAllocator<Foo>("Pool1", 400, AllocType::POOL), L"Pool not create");
		}

		TEST_METHOD(CreateHeapAllocator)
		{
			MemoryManager manager;
			Assert::IsTrue(manager.CreateAllocator("Heap1", 400, AllocType::HEAP), L"Heap not create");


		}


	private:
		_CrtMemState MemoryManagerTest::sStartMemState;
	};
}