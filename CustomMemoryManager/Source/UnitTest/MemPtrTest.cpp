#include "CppUnitTest.h"
#include "MemPtr.h"
#include "MemoryManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace CustomMemoryManager;

namespace UnitTest
{
	TEST_CLASS(MemPtrTest)
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

		TEST_METHOD(TestMethod1)
		{
			MemoryManager manager;
			manager.CreateAllocator("Stack", 2 * sizeof(std::int32_t), AllocType::STACK);

			MemData<std::int32_t> data = manager.Allocate_GetData<std::int32_t>(sizeof(std::int32_t), "Stack", AllocType::STACK);
			MemData<std::int32_t> data2 = std::move(data);

			MemPtr<std::int32_t> intpointer = manager.Allocate_GetData<std::int32_t>(sizeof(std::int32_t), "Stack", AllocType::STACK);
			intpointer;
		}


	private:
		_CrtMemState MemPtrTest::sStartMemState;
	};
}