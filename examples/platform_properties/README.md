# Platform Properties Example

This example demonstrates CPPaste's service system for platform-specific behavior in user code.

## Overview

When creating a large project you may want to define properties that are specific to a certain platform. Traditionally, you might create an enum `enum class Platform { Windows, Linux, MacOS };` and use `if constexpr` throughout your code.

With CPPaste, you can create a `PlatformProperties` interface and multiple platform-specific implementations. The framework automatically selects the correct implementation at compile time based on priority and availability.

## Key Features Demonstrated

- **Compile-Time Polymorphism**: Using `StaticInterface` and `StaticImplementation`
- **Automatic Service Selection**: Based on `Enabled()` and `Priority()` methods
- **Clean Abstraction**: Platform-specific details isolated in implementations
