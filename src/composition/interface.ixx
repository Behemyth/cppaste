export module synodic.cppaste.composition:interface;

import std;

export namespace cppaste
{
	/**
	 * @brief Interface polymorphism mode
	 *
	 * Controls how an interface can be used for polymorphism.
	 */
	enum class InterfaceMode
	{
		DYNAMIC, // Pure virtual interface for runtime polymorphism and plugins
		STATIC,  // Static-only, maximum performance, no dynamic wrapping allowed
		HYBRID   // Static implementation that can also be wrapped dynamically
	};

	/**
	 * @brief Base class for service interfaces
	 *
	 * Provides common type information for all service interfaces.
	 * With C++26 reflection, this will be automatically applied to [[service]] annotated types.
	 *
	 * @tparam Interface The interface type itself (CRTP pattern)
	 * @tparam Derived The concrete derived type (void for dynamic polymorphism)
	 * @tparam Mode The polymorphism mode (auto-detected if not specified)
	 */
	template<typename Interface, typename Derived = void,
	         InterfaceMode Mode = std::is_void_v<Derived> ? InterfaceMode::DYNAMIC : InterfaceMode::HYBRID>
	struct InterfaceBase
	{
		using interface_type = Interface;
		using derived_type = Derived;

		/// The polymorphism mode for this interface
		static constexpr InterfaceMode mode = Mode;

		/// Whether this interface supports dynamic (runtime) polymorphism
		static constexpr bool allows_dynamic = (Mode != InterfaceMode::STATIC);

		/// Whether this interface supports static (compile-time) polymorphism
		static constexpr bool allows_static = (Mode != InterfaceMode::DYNAMIC);

		// Validate mode/type combinations
		static_assert(!(std::is_void_v<Derived> && Mode == InterfaceMode::STATIC),
			"Static mode requires a Derived type");
		static_assert(!(std::is_void_v<Derived> && Mode == InterfaceMode::HYBRID),
			"Hybrid mode requires a Derived type");

	protected:
		constexpr InterfaceBase() = default;
		~InterfaceBase() = default;
	};

	/**
	 * @brief Specialization for dynamic polymorphic interfaces
	 *
	 * When Derived = void, the interface is purely abstract (virtual methods).
	 * This enables runtime polymorphism and type erasure.
	 */
	template<typename Interface>
	struct InterfaceBase<Interface, void, InterfaceMode::DYNAMIC>
	{
		using interface_type = Interface;
		using derived_type = void;

		static constexpr InterfaceMode mode	 = InterfaceMode::DYNAMIC;
		static constexpr bool allows_dynamic = true;
		static constexpr bool allows_static = false;

	protected:
		constexpr InterfaceBase() = default;
		virtual ~InterfaceBase() = default;
	};

	// TODO(reflection): Generate interface dispatch methods automatically
	// Example of what reflection should generate:
	//
	// For an interface with method: int Add(int a, int b)
	//
	// Static version (CRTP):
	// template<typename Self>
	// constexpr auto Add(this Self&& self, int a, int b) -> int {
	//     return std::forward<Self>(self).GetStorage().get().Add(a, b);
	// }
	//
	// Dynamic version (virtual):
	// virtual auto Add(int a, int b) -> int = 0;
}
