#pragma once
#include <string>

namespace UnitTest
{
	class Foo final
	{
	public:
		Foo();
		Foo(const Foo&) = default;
		Foo(Foo&&) = default;
		Foo& operator=(Foo&) = default;
		Foo& operator=(Foo&&) = default;
		~Foo();

		std::string data = "data";
	};
}