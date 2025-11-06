export module example.platform.interface;

import std;
import synodic.cppaste.composition;

export namespace example
{
	/**
	 * @brief Platform properties interface using unified polymorphism framework
	 *
	 * This demonstrates how to define a single interface that works with
	 * static and dynamic polymorphism using CRTP with deducing this.
	 */

	// Forward declaration for CRTP
	template<typename Derived>
	struct IPlatformProperties;

	/**
	 * @brief CRTP interface definition
	 *
	 * With automatic dispatch, interface methods directly access storage
	 * through the service's GetStorage() method.
	 */
	template<typename Derived>
	struct IPlatformProperties : cppaste::InterfaceBase<IPlatformProperties<Derived>, Derived>
	{
		using interface_type = IPlatformProperties;

		// Public API methods using deducing this and automatic dispatch
		template<typename Self>
		constexpr auto GetPlatformName(this Self&& self) -> std::string_view
		{
			return std::forward<Self>(self).GetStorage().get().GetPlatformName();
		}

		template<typename Self>
		constexpr auto GetExecutableExtension(this Self&& self) -> std::string_view
		{
			return std::forward<Self>(self).GetStorage().get().GetExecutableExtension();
		}

		template<typename Self>
		constexpr auto GetDynamicLibraryExtension(this Self&& self) -> std::string_view
		{
			return std::forward<Self>(self).GetStorage().get().GetDynamicLibraryExtension();
		}

		template<typename Self>
		auto PrintInfo(this Self&& self, std::ostream& out)
		{
			return std::forward<Self>(self).GetStorage().get().PrintInfo(out);
		}
	};
}  // namespace example

/**
 * @brief Specialization of DynamicStorage for IPlatformProperties
 *
 * This defines the vtable for runtime polymorphism.
 * MUST be outside any namespace for template specialization.
 */
template<>
class cppaste::DynamicStorage<example::IPlatformProperties<void>>
{
public:
	using interface_type = example::IPlatformProperties<void>;

	// Vtable interface (Concept)
	struct Concept : TypeErasureBase::Concept
	{
		virtual auto GetPlatformName() -> std::string_view			  = 0;
		virtual auto GetExecutableExtension() -> std::string_view	  = 0;
		virtual auto GetDynamicLibraryExtension() -> std::string_view = 0;
		virtual void PrintInfo(std::ostream& out)					  = 0;
	};

	// Vtable implementation (Model)
	template<typename T>
	struct Model : Concept
	{
		T data_;

		explicit Model(T data) :
			data_(std::move(data))
		{
		}

		std::unique_ptr<TypeErasureBase::Concept> clone() const override
		{
			return std::make_unique<Model<T>>(data_);
		}

		auto GetPlatformName() -> std::string_view override
		{
			return data_.GetPlatformName();
		}

		auto GetExecutableExtension() -> std::string_view override
		{
			return data_.GetExecutableExtension();
		}

		auto GetDynamicLibraryExtension() -> std::string_view override
		{
			return data_.GetDynamicLibraryExtension();
		}

		void PrintInfo(std::ostream& out) override
		{
			data_.PrintInfo(out);
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

	auto get_concept() const -> const Concept*
	{
		if (base_.empty())
		{
			throw std::runtime_error("Empty dynamic storage");
		}
		return static_cast<const Concept*>(base_.pimpl_.get());
	}

	auto get_concept() -> Concept*
	{
		if (base_.empty())
		{
			throw std::runtime_error("Empty dynamic storage");
		}
		return static_cast<Concept*>(base_.pimpl_.get());
	}

	// Legacy method names for backward compatibility
	bool Empty() const { return empty(); }
	auto GetConcept() const -> const Concept* { return get_concept(); }
	auto GetConcept() -> Concept* { return get_concept(); }
};

// Re-enter example namespace for type aliases
export namespace example
{
	// Type aliases for convenience
	using PlatformProperties = IPlatformProperties<void>;
}
