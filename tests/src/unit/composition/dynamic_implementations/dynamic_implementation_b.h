// Defines the module implementations for the test suite

#pragma once

#include "../dynamic_interface_types.h"
#include "synodic/composition/module.h"

//class DynamicImplementationB final : public cppaste::DynamicImplementation<DynamicInterfaceA, DynamicImplementationB>
//{
//public:
//	constexpr DynamicImplementationB() = default;
//
//	[[nodiscard]] int Get() const override;
//
//	static constexpr uint8_t Priority()
//	{
//		return 0;
//	}
//};