#include "CppUnitTest.h"
#include "DoubleEndedStackAllocator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace CustomMemoryManager;
using namespace CustomMemoryManager::Allocators;

namespace UnitTest
{
	TEST_CLASS(DoubleEndedStackTest)
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
			DoubleEndedStackAllocator stackAllocator(4U);

			Assert::AreEqual((std::size_t)4U, stackAllocator.StackSize_Bytes());
		}

		TEST_METHOD(AllocTop)
		{
			DoubleEndedStackAllocator stackAllocator(8U, 0U, 0U);

			std::uint32_t* x = static_cast<std::uint32_t*>(stackAllocator.allocate(sizeof(std::uint32_t), Allocators::Info::TOP));
			*x = 1;
			//Assert::IsNotNull(x, L"1");
			Assert::AreEqual(1U, *x, L"2");

			std::uint32_t* y = static_cast<std::uint32_t*>(stackAllocator.allocate(sizeof(std::uint32_t), Allocators::Info::TOP));
			*y = 2;
			//Assert::IsNotNull(y, L"3");
			Assert::AreEqual(1U, *x, L"4");
			Assert::AreEqual(2U, *y, L"5");

			std::uint32_t* z = static_cast<std::uint32_t*>(stackAllocator.allocate(sizeof(std::uint32_t), Allocators::Info::TOP));
			//Assert::IsNull(z, L"6");
			z;
			Assert::AreEqual(1U, *x, L"7");
			Assert::AreEqual(2U, *y, L"8");

			Assert::AreEqual((std::size_t)8U, stackAllocator.UsedBytes_Top(), L"8.1");

			stackAllocator.Clear();

			Assert::AreEqual((std::size_t)0U, stackAllocator.UsedBytes_Top(), L"8.2");

			std::uint64_t* w = static_cast<std::uint64_t*>(stackAllocator.allocate(sizeof(std::uint64_t), Allocators::Info::TOP));
			//Assert::IsNotNull(w, L"9");
			*w = 64U;
			Assert::IsTrue(64U == *w, L"10");

			bool* another = static_cast<bool*>(stackAllocator.allocate(sizeof(bool), Allocators::Info::TOP));
			//Assert::IsNull(another, L"11");
			another;

			Assert::AreEqual((std::size_t)8U, stackAllocator.UsedBytes_Top(), L"12");
		}

		TEST_METHOD(AllocBottom)
		{
			DoubleEndedStackAllocator stackAllocator(0U, 8U, 0U);

			std::uint32_t* x = static_cast<std::uint32_t*>(stackAllocator.allocate(sizeof(std::uint32_t), Allocators::Info::BOTTOM));
			*x = 1;
			//Assert::IsNotNull(x, L"1");
			Assert::AreEqual(1U, *x, L"2");

			std::uint32_t* y = static_cast<std::uint32_t*>(stackAllocator.allocate(sizeof(std::uint32_t), Allocators::Info::BOTTOM));
			*y = 2;
			//Assert::IsNotNull(y, L"3");
			Assert::AreEqual(1U, *x, L"4");
			Assert::AreEqual(2U, *y, L"5");

			std::uint32_t* z = static_cast<std::uint32_t*>(stackAllocator.allocate(sizeof(std::uint32_t), Allocators::Info::BOTTOM));
			//Assert::IsNull(z, L"6");
			z;
			Assert::AreEqual(1U, *x, L"7");
			Assert::AreEqual(2U, *y, L"8");

			Assert::AreEqual((std::size_t)8U, stackAllocator.UsedBytes_Bottom(), L"8.1");

			stackAllocator.Clear();

			Assert::AreEqual((std::size_t)0U, stackAllocator.UsedBytes_Bottom(), L"8.2");

			std::uint64_t* w = static_cast<std::uint64_t*>(stackAllocator.allocate(sizeof(std::uint64_t), Allocators::Info::BOTTOM));
			//Assert::IsNotNull(w, L"9");
			*w = 64U;
			Assert::IsTrue(64U == *w, L"10");

			bool* another = static_cast<bool*>(stackAllocator.allocate(sizeof(bool), Allocators::Info::BOTTOM));
			//Assert::IsNull(another, L"11");
			another;

			Assert::AreEqual((std::size_t)8U, stackAllocator.UsedBytes_Bottom(), L"12");
		}

		TEST_METHOD(AllocTopAndBottom)
		{
			DoubleEndedStackAllocator stackAllocator(8U, 8U, 0U);

			std::uint32_t* x1 = static_cast<std::uint32_t*>(stackAllocator.allocate(sizeof(std::uint32_t), Allocators::Info::TOP));
			*x1 = 1;
			//Assert::IsNotNull(x1, L"1");
			Assert::AreEqual(1U, *x1, L"2");
			std::uint32_t* x2 = static_cast<std::uint32_t*>(stackAllocator.allocate(sizeof(std::uint32_t), Allocators::Info::BOTTOM));
			*x2 = 1;
			//Assert::IsNotNull(x2, L"1");
			Assert::AreEqual(1U, *x2, L"2");

			std::uint32_t* y1 = static_cast<std::uint32_t*>(stackAllocator.allocate(sizeof(std::uint32_t), Allocators::Info::TOP));
			*y1 = 2;
			//Assert::IsNotNull(y1, L"3");
			Assert::AreEqual(1U, *x1, L"4");
			Assert::AreEqual(2U, *y1, L"5");
			std::uint32_t* y2 = static_cast<std::uint32_t*>(stackAllocator.allocate(sizeof(std::uint32_t), Allocators::Info::BOTTOM));
			*y2 = 2;
			//Assert::IsNotNull(y2, L"3");
			Assert::AreEqual(1U, *x1, L"4");
			Assert::AreEqual(2U, *y2, L"5");

			std::uint32_t* z1 = static_cast<std::uint32_t*>(stackAllocator.allocate(sizeof(std::uint32_t), Allocators::Info::TOP));
			//Assert::IsNull(z1, L"6");
			Assert::AreEqual(1U, *x1, L"7");
			Assert::AreEqual(2U, *y2, L"8");
			std::uint32_t* z2 = static_cast<std::uint32_t*>(stackAllocator.allocate(sizeof(std::uint32_t), Allocators::Info::BOTTOM));
			//Assert::IsNull(z2, L"6");
			Assert::AreEqual(1U, *x1, L"7");
			Assert::AreEqual(2U, *y2, L"8");
			z1; z2;

			Assert::AreEqual((std::size_t)8U, stackAllocator.UsedBytes_Top(), L"8.1");
			Assert::AreEqual((std::size_t)8U, stackAllocator.UsedBytes_Bottom(), L"8.1");

			stackAllocator.Clear();

			Assert::AreEqual((std::size_t)0U, stackAllocator.UsedBytes_Top(), L"8.2");
			Assert::AreEqual((std::size_t)0U, stackAllocator.UsedBytes_Bottom(), L"8.2");

			std::uint64_t* w1 = static_cast<std::uint64_t*>(stackAllocator.allocate(sizeof(std::uint64_t), Allocators::Info::TOP));
			//Assert::IsNotNull(w1, L"9");
			*w1 = 64U;
			Assert::IsTrue(64U == *w1, L"10");
			std::uint64_t* w2 = static_cast<std::uint64_t*>(stackAllocator.allocate(sizeof(std::uint64_t), Allocators::Info::BOTTOM));
			//Assert::IsNotNull(w2, L"9");
			*w2 = 64U;
			Assert::IsTrue(64U == *w2, L"10");

			bool* another1 = static_cast<bool*>(stackAllocator.allocate(sizeof(bool), Allocators::Info::TOP));
			//Assert::IsNull(another1, L"11");
			bool* another2 = static_cast<bool*>(stackAllocator.allocate(sizeof(bool), Allocators::Info::BOTTOM));
			//Assert::IsNull(another2, L"11");
			another1; another2;

			Assert::AreEqual((std::size_t)8U, stackAllocator.UsedBytes_Top(), L"12");
			Assert::AreEqual((std::size_t)8U, stackAllocator.UsedBytes_Bottom(), L"12");
		}

	private:
		_CrtMemState DoubleEndedStackTest::sStartMemState;
	};
}