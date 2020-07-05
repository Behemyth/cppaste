#pragma once

#include <memory>

#include "composition/module.h"


// Basic Types

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

// T - unique scope
class ValueApp
{
public:
	explicit ValueApp(ImplementationA implementation) : value_(implementation.Get())
	{
	}
	~ValueApp() = default;

	[[nodiscard]] int Get() const
	{
		return value_;
	}

private:
	int value_;
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

// const T& - singleton scope
class ConstReferenceApp
{
public:
	explicit ConstReferenceApp(const ImplementationA& implementation) : value_(implementation.Get())
	{
	}
	~ConstReferenceApp() = default;

	[[nodiscard]] int Get() const
	{
		return value_;
	}

private:
	int value_;
};

// T*- unique scope, ownership transfer
class PointerApp
{
public:
	explicit PointerApp(Interface* implementation) : value_(implementation->Get())
	{
	}
	~PointerApp() = default;

	[[nodiscard]] int Get() const
	{
		return value_;
	}

private:
	int value_;
};

// const T*- unique scope, ownership transfer
class ConstPointerApp
{
public:
	explicit ConstPointerApp(const Interface* implementation) : value_(implementation->Get())
	{
	}
	~ConstPointerApp() = default;

	[[nodiscard]] int Get() const
	{
		return value_;
	}

private:
	int value_;
};

// T&& - unique scope
class UniversalReferenceApp
{
public:
	explicit UniversalReferenceApp(ImplementationA&& implementation) : value_(implementation.Get())
	{
	}
	~UniversalReferenceApp() = default;

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

// std::shared_ptr - singleton scope
class SharedPtrApp
{
public:
	explicit SharedPtrApp(std::shared_ptr<Interface> implementation) : value_(implementation->Get())
	{
	}
	~SharedPtrApp() = default;

	[[nodiscard]] int Get() const
	{
		return value_;
	}

private:
	int value_;
};

// TODO
// std::weak_ptr - singleton scope
class WeakPtrApp
{
public:
	explicit WeakPtrApp(std::weak_ptr<Interface> implementation) : value_(implementation)
	{
	}
	~WeakPtrApp() = default;

	[[nodiscard]] int Get() const
	{
		if(const auto shared = value_.lock())
		{  // Has to be copied into a shared_ptr before usage
			return shared->Get();
		}
		return 0;
	}

private:
	std::weak_ptr<Interface> value_;
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