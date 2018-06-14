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

	};
}