// Defines the module implementations for the test suite

#pragma once

#include "../dynamic_interface_types.h"
#include "synodic/composition/module.h"

//class DynamicImplementationA final : public cppaste::DynamicImplementation<DynamicInterfaceA, DynamicImplementationA>
//{
//public:
//	constexpr DynamicImplementationA() = default;
//
//	[[nodiscard]] int Get() const override;
//
//	static constexpr uint8_t Priority()
//	{
//		return 1;
//	}
//};