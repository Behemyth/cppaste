#include "composition/module.h"
#include "composition/package.h"
#include "doctest/doctest.h"

//class Interface : synodic::Interface
//{
//public:
//	Interface()			 = default;
//	virtual ~Interface() = default;
//
//	virtual int Get() = 0;
//};
//
//class Implementation : Interface
//{
//public:
//	Implementation()   = default;
//	~Implementation() = default;
//
//
//	bool Instantiate() override
//	{
//		return false;
//	}
//
//	[[nodiscard]] int Get() override
//	{
//		return 3;
//	}
//};
//
//
//TEST_CASE("Mutable Module")
//{
//	class App
//	{
//	public:
//		explicit App(std::unique_ptr<Interface> implementation) : value_(implementation->Get())
//		{
//		}
//		~App() = default;
//
//		[[nodiscard]] int Get() const
//		{
//			return value_;
//		}
//
//	private:
//		int value_;
//	};
//	
//	synodic::Module<Interface> mutableModule;
//
//	synodic::Package testPackage(mutableModule);
//	const auto app = testPackage.Create<App>();
//
//	CHECK(app.Get() == 2);
//}
//
//
//TEST_CASE("Const Module")
//{
//	class App
//	{
//	public:
//		explicit App(std::unique_ptr<Interface> implementation) : value_(implementation->Get())
//		{
//		}
//		~App() = default;
//
//		[[nodiscard]] int Get() const
//		{
//			return value_;
//		}
//
//	private:
//		int value_;
//	};
//
//	const synodic::Module<Interface> constModule;
//
//	synodic::Package testPackage(constModule);
//	const auto app = testPackage.Create<App>();
//
//	CHECK(app.Get() == 2);
//}
