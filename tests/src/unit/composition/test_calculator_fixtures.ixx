export module synodic.cppaste.test.calculator_fixtures;

import std;
import synodic.cppaste.composition;

/**
 * @brief Shared test fixtures for calculator interface testing
 */

export namespace test_fixtures
{
	/**
	 * @brief User-provided definition
	 *
	 * TODO(reflection): Annotate as [[service]] instead of manual definitions
	 */
	struct ICalculator : cppaste::InterfaceBase<ICalculator, void>
	{
		virtual auto Add(int a, int b) const -> int = 0;
		virtual auto Multiply(int a, int b) const -> int = 0;
		virtual auto GetName() const -> std::string_view = 0;
		virtual ~ICalculator() = default;
	};

	/**
	 * @brief CRTP wrapper for static polymorphism
	 *
	 * Uses deducing this for automatic dispatch to underlying implementation.
	 * The Derived parameter is typically a Service<ICalculatorStatic<Derived>, StaticStorage<T>>.
	 *
	 * TODO(reflection): This will be auto-generated from ICalculator definition
	 */
	template<typename Derived>
	struct ICalculatorStatic : cppaste::InterfaceBase<ICalculatorStatic<Derived>, Derived>
	{
		template<typename Self>
		constexpr auto Add(this Self&& self, int a, int b) -> int
		{
			return std::forward<Self>(self).GetStorage().get().Add(a, b);
		}

		template<typename Self>
		constexpr auto Multiply(this Self&& self, int a, int b) -> int
		{
			return std::forward<Self>(self).GetStorage().get().Multiply(a, b);
		}

		template<typename Self>
		constexpr auto GetName(this Self&& self) -> std::string_view
		{
			return std::forward<Self>(self).GetStorage().get().GetName();
		}
	};

	/**
	 * @brief Simple calculator implementation
	 *
	 * Performs basic arithmetic operations without modifications.
	 */
	struct SimpleCalculator
	{
		constexpr auto Add(int a, int b) const -> int
		{
			return a + b;
		}

		constexpr auto Multiply(int a, int b) const -> int
		{
			return a * b;
		}

		constexpr auto GetName() const -> std::string_view
		{
			return "SimpleCalculator";
		}
	};

	/**
	 * @brief Advanced calculator implementation with bonuses
	 *
	 * Adds a bonus to addition and doubles multiplication results.
	 */
	struct AdvancedCalculator
	{
		constexpr auto Add(int a, int b) const -> int
		{
			return a + b + 1;  // Adds bonus
		}

		constexpr auto Multiply(int a, int b) const -> int
		{
			return a * b * 2;  // Doubles result
		}

		constexpr auto GetName() const -> std::string_view
		{
			return "AdvancedCalculator";
		}
	};
}

/**
 * @brief Specialization of DynamicStorage for ICalculator
 *
 * This defines the vtable for runtime polymorphism.
 * MUST be outside any namespace for template specialization.
 *
 * TODO(reflection): Auto-generate from ICalculator definition
 */
template<>
class cppaste::DynamicStorage<test_fixtures::ICalculator>
{
public:
	using interface_type = test_fixtures::ICalculator;

	// Vtable interface (Concept) - just inherit the interface
	struct Concept : TypeErasureBase::Concept, test_fixtures::ICalculator
	{
	};

	// Vtable implementation (Model) - wraps StaticStorage<T>
	template<typename T>
	struct Model : Concept
	{
		StaticStorage<T> storage_;

		explicit Model(T data) :
			storage_(std::move(data))
		{
		}

		std::unique_ptr<TypeErasureBase::Concept> clone() const override
		{
			return std::make_unique<Model<T>>(storage_.get());
		}

		auto Add(int a, int b) const -> int override
		{
			return storage_.get().Add(a, b);
		}

		auto Multiply(int a, int b) const -> int override
		{
			return storage_.get().Multiply(a, b);
		}

		auto GetName() const -> std::string_view override
		{
			return storage_.get().GetName();
		}
	};

private:
	TypeErasureBase base_;

public:
	DynamicStorage() = default;

	template<typename T>
	DynamicStorage(T obj)
	{
		base_.pimpl_ = std::make_unique<Model<T>>(std::move(obj));
	}

	DynamicStorage(const DynamicStorage& other) :
		base_(other.base_)
	{
	}

	DynamicStorage(DynamicStorage&&) noexcept = default;

	DynamicStorage& operator=(const DynamicStorage& other)
	{
		base_ = other.base_;
		return *this;
	}

	DynamicStorage& operator=(DynamicStorage&&) noexcept = default;

	bool empty() const
	{
		return base_.empty();
	}

	/**
	 * @brief Get the vtable for dispatch
	 *
	 * Uses deducing this to handle const/non-const in a single method.
	 */
	template<typename Self>
	auto get_concept(this Self&& self)
	{
		if (self.base_.empty())
		{
			throw std::runtime_error("Empty dynamic storage");
		}

		using ReturnType = std::conditional_t<
			std::is_const_v<std::remove_reference_t<Self>>,
			const Concept*,
			Concept*
		>;
		return static_cast<ReturnType>(self.base_.pimpl_.get());
	}
};

/**
 * @brief Dynamic service for ICalculator
 *
 * Demonstrates the pattern that reflection will auto-generate. Note the direct
 * forwarding to implementation with SINGLE virtual dispatch (the call on this object).
 * No intermediate vtable layers.
 *
 * TODO(reflection): This entire class will be auto-generated. Reflection will
 * iterate over ICalculator's virtual methods and generate these overrides automatically.
 *
 * NOTE: In practice, you'd specialize for each concrete type (SimpleCalculator, etc.)
 * to avoid the get_impl cast. But this shows the general pattern.
 */
export namespace test_fixtures
{
	template<typename T>
	class DynamicCalculatorService : public cppaste::Service<ICalculator>
	{
		using Base = cppaste::Service<ICalculator>;

	public:
		// Explicit constructors to work around MSVC ICE with inherited constructors
		DynamicCalculatorService() : Base() {}

		template<typename U>
		requires (!std::is_same_v<std::decay_t<U>, DynamicCalculatorService>)
		DynamicCalculatorService(U&& obj) : Base(std::forward<U>(obj)) {}

		using Base::operator=;  // Inherit assignment operator

		auto Add(int a, int b) const -> int override
		{
			return Base::template get_impl<T>(*this).Add(a, b);
		}

		auto Multiply(int a, int b) const -> int override
		{
			return Base::template get_impl<T>(*this).Multiply(a, b);
		}

		auto GetName() const -> std::string_view override
		{
			return Base::template get_impl<T>(*this).GetName();
		}
	};

	// Convenience aliases for specific implementations
	using SimpleCalculatorService = DynamicCalculatorService<SimpleCalculator>;
	using AdvancedCalculatorService = DynamicCalculatorService<AdvancedCalculator>;
}
