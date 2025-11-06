export module synodic.cppaste.composition:interface;

import std;

export namespace cppaste
{
	/**
	 * @brief Base class for service interfaces
	 *
	 * Provides common type information for all service interfaces.
	 * With C++26 reflection, this will be automatically applied to [[service]] annotated types.
	 *
	 * @tparam Interface The interface type itself (CRTP pattern)
	 * @tparam Derived The concrete derived type (void for dynamic polymorphism)
	 */
	template<typename Interface, typename Derived>
	struct InterfaceBase
	{
		using interface_type = Interface;
		using derived_type = Derived;

		/**
		 * @brief Indicates if this interface allows dynamic (runtime) polymorphism
		 *
		 * When true: DynamicStorage can wrap implementations
		 * When false: Only StaticStorage is permitted (compile-time optimization)
		 *
		 * TODO(reflection): Auto-detect from [[service(dynamic)]] annotation
		 */
		static constexpr bool allows_dynamic = !std::is_void_v<Derived>;

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
	struct InterfaceBase<Interface, void>
	{
		using interface_type = Interface;
		using derived_type = void;

		static constexpr bool allows_dynamic = true;

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
