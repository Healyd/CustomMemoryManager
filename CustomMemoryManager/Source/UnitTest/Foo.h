#pragma once
#include <string>

namespace UnitTest
{
	class Foo final
	{
	public:
		Foo() = default;
		Foo(const Foo&) = default;
		Foo(Foo&&) = default;
		Foo& operator=(Foo&) = default;
		Foo& operator=(Foo&&) = default;
		~Foo() = default;

		std::string data = "data";
	};
}