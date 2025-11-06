export module synodic.cppaste.composition:service;

import std;
import :storage;
import :interface;

export namespace cppaste
{
	/**
	 * @brief Core Service composition class
	 *
	 * Service template provides:
	 * - Service<Interface, Impl> : Static polymorphism
	 * - Service<Interface>       : Dynamic polymorphism
	 *
	 * @tparam Interface The service interface
	 * @tparam Impl Optional implementation type for static polymorphism
	 */
	template<typename Interface, typename Impl = void>
	class Service;

	/**
	 * @brief Static service specialization
	 */
	template<typename Interface, typename Impl>
	class Service
	{
		StaticStorage<Impl> storage_;

	public:
		using interface_type = Interface;
		using implementation_type = Impl;

		constexpr Service(Impl impl) : storage_(std::move(impl)) {}

		constexpr auto GetStorage(this auto&& self) -> decltype(auto)
		{
			return std::forward<decltype(self)>(self).storage_;
		}
	};

	/**
	 * @brief Dynamic service specialization
	 * TODO(reflection): Auto-generate virtual method overrides that forward to get_impl<T>.
	 * For each virtual method in Interface, generate override that calls get_impl<T>(*this).Method(...).
	 * Until then, derived classes like DynamicCalculatorService<T> provide manual overrides.
	 */
	template<typename Interface>
	class Service<Interface, void> : public Interface
	{
	private:
		// Type-erased storage for the actual implementation
		struct StorageBase
		{
			virtual ~StorageBase() = default;
			virtual auto clone() const -> std::unique_ptr<StorageBase> = 0;
		};

		template<typename T>
		struct StorageImpl : StorageBase
		{
			StaticStorage<T> impl_;

			explicit StorageImpl(T obj) : impl_(std::move(obj)) {}

			auto clone() const -> std::unique_ptr<StorageBase> override
			{
				// Create copy - get() returns const T& when called on const StorageImpl
				return std::make_unique<StorageImpl<T>>(impl_.get());
			}

			// Return references to the stored implementation
			// Use non-forwarding get_ref() instead of forwarding get()
			constexpr auto get() const -> const T& { return impl_.get_ref(); }
			constexpr auto get() -> T& { return impl_.get_ref(); }
		};

		std::unique_ptr<StorageBase> storage_;

	protected:
		// Helper to get implementation - for use in generated virtual overrides
		template<typename T, typename Self>
		static auto get_impl(Self&& self) -> decltype(auto)
		{
			if (!self.storage_)
			{
				throw std::runtime_error("Empty service");
			}

			using ReturnType = std::conditional_t<
				std::is_const_v<std::remove_reference_t<Self>>,
				const T&,
				T&
			>;
			auto* impl = static_cast<StorageImpl<T>*>(self.storage_.get());
			return static_cast<ReturnType>(impl->get());
		}

	public:
		using interface_type = Interface;

		constexpr Service() : Interface(), storage_(nullptr) {}

		template<typename T>
		requires (!std::is_same_v<std::decay_t<T>, Service>)
		constexpr Service(T&& obj)
			: Interface()
			, storage_(std::make_unique<StorageImpl<std::decay_t<T>>>(std::forward<T>(obj)))
		{}

		Service(const Service& other)
			: Interface()
			, storage_(other.storage_ ? other.storage_->clone() : nullptr)
		{}

		Service(Service&&) noexcept = default;

		auto operator=(const Service& other) -> Service&
		{
			if (this != &other)
			{
				storage_ = other.storage_ ? other.storage_->clone() : nullptr;
			}
			return *this;
		}

		auto operator=(Service&&) noexcept -> Service& = default;

		template<typename T>
		auto operator=(T&& obj) -> Service&
		{
			storage_ = std::make_unique<StorageImpl<std::decay_t<T>>>(std::forward<T>(obj));
			return *this;
		}

		bool empty() const { return storage_ == nullptr; }

		// TODO(reflection): Auto-generate virtual method overrides here
		// For each virtual method in Interface:
		//   ReturnType MethodName(Args... args) [const] override {
		//       return get_impl<ImplementationType>(*this).MethodName(args...);
		//   }
		//
		// Without reflection, derived classes must provide overrides that call get_impl<T>.
		// This provides SINGLE virtual dispatch, not double.
	};
}
