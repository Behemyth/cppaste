// Defines the module interfaces for the test suite

#pragma once


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