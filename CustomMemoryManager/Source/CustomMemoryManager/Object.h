#pragma once
#include <string>

namespace Test
{
	class Object final
	{
	public:
		Object();
		Object(const Object&) = default;
		Object(Object&&) = default;
		Object& operator=(Object&) = default;
		Object& operator=(Object&&) = default;
		~Object();

		std::string data = "dataObject";
	};
}