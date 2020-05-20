#pragma once

namespace test
{
	class BasicImplementation final
	{

	public:
		BasicImplementation()  = default;
		~BasicImplementation() = default;


		[[nodiscard]] int Get() const
		{
			return 5;
		}
	};
}