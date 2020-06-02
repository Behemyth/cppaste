#include "composition/package.h"
#include "doctest/doctest.h"

// Modules have the capability to provide an implementation to an interface templated over possible
// implementation types


template<typename T>
concept StaticConcept = true;


class DynamicInterface
{
public:
	DynamicInterface()			= default;
	virtual ~DynamicInterface() = default;

	virtual int Get() = 0;
};

// template<StaticConcept T>
// class StaticInterface : DynamicInterface
//{
// public:
//	StaticInterface()  = default;
//	~StaticInterface() = default;
//
//	int Get() override
//	{
//		return implementation.Get();
//	}
//
// private:
//	T implementation;
//};


class ImplementationA : public DynamicInterface
{
public:
	ImplementationA() = default;
	~ImplementationA() override = default;

	[[nodiscard]] int Get() override
	{
		return 3;
	}
};

class Test : synodic::Implementation<DynamicInterface, ImplementationA>
{

	Test() : synodic::Implementation<DynamicInterface, ImplementationA>(false)
	{
	}
	~Test() override = default;

	bool Instantiate() override
	{
		return true;
	}
	
};

// class ImplementationB : synodic::Implementation<DynamicInterface, ImplementationB>
//{
// public:
//	ImplementationB() : synodic::Implementation<DynamicInterface, ImplementationB>(false)
//	{
//	}
//	~ImplementationB() override = default;
//
//	bool Instantiate() override
//	{
//		return true;
//	}
//
//	[[nodiscard]] int Get() override
//	{
//		return 4;
//	}
//};


// class StaticAppA
//{
// public:
//	template<StaticConcept staticInterface>
//	explicit StaticAppA(staticInterface implementation) : value_(implementation->Get())
//	{
//	}
//	~StaticAppA() = default;
//
//	[[nodiscard]] int Get() const
//	{
//		return value_;
//	}
//
// private:
//	int value_;
//};

// template<StaticConcept staticInterface>
// class StaticAppB
//{
// public:
//	explicit StaticAppB(staticInterface implementation) : value_(implementation->Get())
//	{
//	}
//	~StaticAppB() = default;
//
//	[[nodiscard]] int Get() const
//	{
//		return value_;
//	}
//
// private:
//	int value_;
//};


class DynamicApp
{
public:
	explicit DynamicApp(std::unique_ptr<DynamicInterface> implementation) :
		value_(implementation->Get())
	{
	}
	~DynamicApp() = default;

	[[nodiscard]] int Get() const
	{
		return value_;
	}

private:
	int value_;
};


// TEST_CASE("Static Polymorphic Injection")
//{
//	synodic::Module<DynamicInterface> staticModule;
//
//	synodic::Package testPackage(staticModule);
//	const auto app = testPackage.Create<StaticAppA>();
//
//	CHECK(app.Get() == 4);
//}


// All modules can be null

// TEST_CASE("No Implementations")
//{
//	synodic::Package testPackage;
//	const auto app = testPackage.Create<DynamicApp>();
//}


// Modules have the capability to provide an implementation to an interface with dynamic inheritance

TEST_CASE("Dynamic Polymorphic Injection")
{
	 synodic::Module<DynamicInterface> dynamicModule;

	 synodic::Package testPackage(dynamicModule);
	 const auto app = testPackage.Create<DynamicApp>();

	CHECK(app.Get() == 2);
}


// Modules can be implicitly cast to their implementation type

TEST_CASE("Type Cast")
{
	// TODO: Write test
}


// Module implementations can be loaded from a dll implicitly

TEST_CASE("Implicit Dynamic Linking")
{
	// TODO: Write test - Issue #10
}


// Module implementations can be loaded from a dll explicitly

TEST_CASE("Explicit Dynamic Linking")
{
	// TODO: Write test - Issue #10
}


// The current module can unloaded and loaded into another implementation

TEST_CASE("Implementation Swap")
{
	// TODO: Write test - Issue #11
}