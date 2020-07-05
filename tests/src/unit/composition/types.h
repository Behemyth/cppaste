#pragma once

#include <memory>

#include "composition/module.h"


class Interface
{
public:
	Interface()			 = default;
	virtual ~Interface() = default;

	[[nodiscard]] virtual int Get() const = 0;
};

class InterfaceWithTemplateFunction
{
public:
	InterfaceWithTemplateFunction()			 = default;
	virtual ~InterfaceWithTemplateFunction() = default;

	template<typename T>
	[[nodiscard]] int Get() const;
};

class ImplementationWithTemplateFunction : public InterfaceWithTemplateFunction
{
public:
	ImplementationWithTemplateFunction()		   = default;
	~ImplementationWithTemplateFunction() override = default;

	template<typename T>
	[[nodiscard]] T Get() const
	{
		return T(3);
	}
};

template<>
class synodic::
	ImplementationConfiguration<InterfaceWithTemplateFunction, ImplementationWithTemplateFunction> :
	public ModuleConfig<InterfaceWithTemplateFunction>
{
public:
	ImplementationConfiguration()  = default;
	~ImplementationConfiguration() = default;

	constexpr uint8_t Priority()
	{
		return 1;
	}
};

class ImplementationA : public Interface
{
public:
	ImplementationA()			= default;
	~ImplementationA() override = default;

	[[nodiscard]] int Get() const override
	{
		return 3;
	}
};

template<>
class synodic::ImplementationConfiguration<Interface, ImplementationA> :
	public ModuleConfig<Interface>
{
public:
	ImplementationConfiguration()  = default;
	~ImplementationConfiguration() = default;

	constexpr uint8_t Priority()
	{
		return 1;
	}
};

class ImplementationB : public Interface
{
public:
	ImplementationB()			= default;
	~ImplementationB() override = default;

	[[nodiscard]] int Get() const override
	{
		return 4;
	}
};

template<>
class synodic::ImplementationConfiguration<Interface, ImplementationB> :
	public ModuleConfig<Interface>
{
public:
	ImplementationConfiguration()  = default;
	~ImplementationConfiguration() = default;

	constexpr uint8_t Priority()
	{
		return 0;
	}
};

// T& - singleton scope
class ReferenceApp
{
public:
	explicit ReferenceApp(ImplementationA& implementation) : value_(implementation.Get())
	{
	}
	~ReferenceApp() = default;

	[[nodiscard]] int Get() const
	{
		return value_;
	}

private:
	int value_;
};

// std::unique_ptr - unique scope
class UniquePtrApp
{
public:
	explicit UniquePtrApp(std::unique_ptr<Interface> implementation) : value_(implementation->Get())
	{
	}
	~UniquePtrApp() = default;

	[[nodiscard]] int Get() const
	{
		return value_;
	}

private:
	int value_;
};

template<typename InterfaceType = class Interface>
class TemplateApp
{
public:
	explicit TemplateApp(const InterfaceType& implementation) : value_(implementation.Get())
	{
	}
	~TemplateApp() = default;

	[[nodiscard]] int Get() const
	{
		return value_;
	}

private:
	int value_;
};

template<typename InterfaceType = class InterfaceWithTemplateFunction>
class TemplateFunctionApp
{
public:
	explicit TemplateFunctionApp(const InterfaceType& implementation) :
		value_(implementation.template Get<int>())
	{
	}
	~TemplateFunctionApp() = default;

	[[nodiscard]] int Get() const
	{
		return value_;
	}

private:
	int value_;
};