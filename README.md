= CPPaste
Non-intrusive, sharable program framework for C++23 and beyond.
:toc:

== Features

=== Services - Static and Dynamic Polymorphism

Note: The name "Service" is used to avoid confusion with C++20 modules.

[cols="1,1,1"]
|===
||*Static* |*Dynamic*
|*Single Behavior*
|C++20 Concepts
|std::function or std::function_ref

|*Bundled Behavior*
|C++20 Concepts
|Abstract Base Class
|===

Allows shared downstream behavior for any case of polymorphism

There is a Directed Acyclic Graph(DAG) representing all services, both dynamic and static.
Static services are allowed to depend on dynamic services. However, the dynamic services state will be unresolved at compile time. To rectify this, each static service creates a dynamic service which is constructed when all dynamic dependencies are resolved.

By nature of an object existing, the state of the dependencies will also exist. Ownership transfer and lifetime requests are not managed by services themselves.

==== Agnostic Build System Integration
Communication with the build environment is done by language definitions through macros. C++ does not support compilation via https://en.cppreference.com/w/cpp/preprocessor/conditional[conditional inclusion] without them. With a standardized set of macros, it is the build system's responsibility to fulfill the macro data as it sees fit. A format is provided with CMake and Meson utilities to simplify build system integration. The implementation of those utilities can be found in another repository.

=== Dependency Injection
Dependency injection can be broken into two parts, the injection and dependency wiring. Injection in C++ is simply the passing of types to the constructor of another type. Dependencies are modelled after the same behaviors as CPPaste Services with the additional expression of lifetime.

* Unique ownership
* Shared ownership
* No ownership

With the restriction of keeping dependencies constructed in class constructors without direct knowledge of the types they are constructing, the problem of passing one concrete type to another becomes a the problem at the calling point. At this point passing the right data to the right constructor is all that needs to be done, and could be done automatically. While this is technically feasible with C++, it requires Undefined Behavior to accomplish. Until language Reflection or constructor type traits are standardized, this task must be done manually.

=== Bindings - Script and Language Interoperability
C++ does not support reflection and it is Synodic Software's opinion that language macros are evil and should be avoided at all cost. As a result, binding requires filling context information explicitly for all bound types. The language interoperability that you are using can make use of those bound type to provide bi-directional execution.

=== System Information - Distilled `constexpr` Utilities
System information and build macros distilled into `constexpr` utilities for your macro-less usage.