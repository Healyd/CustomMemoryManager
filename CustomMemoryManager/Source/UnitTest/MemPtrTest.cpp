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
			manager.CreateAllocator("Heap", 2 * sizeof(std::int32_t), AllocType::HEAP);

			MemPtr<std::int32_t> intpointer = std::move(manager.MemPtr_HeapAllocate<std::int32_t>(sizeof(std::int32_t), "Heap"));
			MemPtr<std::int32_t> intpointer2 = intpointer;
			MemPtr<std::int32_t> intpointer4 = intpointer;
			MemPtr<std::int32_t> intpointer5 = intpointer4;
			{
				MemPtr<std::int32_t> intpointer6 = intpointer4;
			}
			Assert::AreEqual(intpointer.Get(), intpointer2.Get());
			MemPtr<std::int32_t> intpointer3 = std::move(intpointer2);
			Assert::IsNull(intpointer2.Get());
			Assert::IsNull(intpointer2.operator->());
			intpointer2.operator*();
			Assert::IsNotNull(intpointer3.Get());
			intpointer2;
		}


	private:
		_CrtMemState MemPtrTest::sStartMemState;
	};
}