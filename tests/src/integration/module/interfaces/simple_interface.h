#pragma once


namespace test
{

	//template<typename T>
	//concept simple_interface = requires(T a)
	//{
	//	true;
	//};

	
	class SimpleInterface
	{

	public:
		virtual ~SimpleInterface() = 0;

		virtual int Get() = 0;
	};


}