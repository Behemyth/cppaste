export module synodic.cppaste.composition:service_builder;

import std;
import :storage;

// Forward declare to avoid circular dependency
export namespace cppaste
{
	template<typename Interface, typename Storage>
	class Service;

	struct ServiceParameters;
}

export namespace cppaste
{
	/**
	 * @brief Unified service builder - works for both static and dynamic services
	 *
	 * Provides a consistent API regardless of storage choice.
	 * Storage selection can be explicit or automatic based on interface traits.
	 *
	 * @example
	 * // Automatic storage selection
	 * auto svc = ServiceBuilder<ICalculator>()
	 *     .implementation<SimpleCalculator>()
	 *     .create();
	 *
	 * // Explicit static storage
	 * auto svc = ServiceBuilder<ICalculator>()
	 *     .implementation<SimpleCalculator>()
	 *     .static_storage()
	 *     .create();
	 *
	 * // Explicit dynamic storage with config
	 * auto svc = ServiceBuilder<ICalculator>()
	 *     .implementation<SimpleCalculator>()
	 *     .dynamic_storage()
	 *     .with_logging(true)
	 *     .create(constructor_args);
	 */
	template<template<typename> class InterfaceTemplate>
	class ServiceBuilder
	{
		ServiceParameters params_;

	public:
		/**
		 * @brief Select implementation type
		 */
		template<typename Impl>
		auto implementation()
		{
			return ServiceBuilderWithImpl<InterfaceTemplate, Impl>{params_};
		}

		/**
		 * @brief Configure before selecting implementation
		 */
		auto& with_logging(bool enable)
		{
			params_.enable_logging = enable;
			return *this;
		}

		auto& with_name(std::string name)
		{
			params_.name = std::move(name);
			return *this;
		}

		auto& with_user_data(std::any data)
		{
			params_.user_data = std::move(data);
			return *this;
		}
	};

	/**
	 * @brief Builder state after implementation type is selected
	 */
	template<template<typename> class InterfaceTemplate, typename Impl>
	class ServiceBuilderWithImpl
	{
		ServiceParameters params_;

		enum class StorageChoice { Auto, Static, Dynamic };
		StorageChoice storage_choice_ = StorageChoice::Auto;

	public:
		explicit ServiceBuilderWithImpl(ServiceParameters params)
			: params_(std::move(params)) {}

		/**
		 * @brief Configuration methods - chainable
		 */
		auto& with_logging(bool enable)
		{
			params_.enable_logging = enable;
			return *this;
		}

		auto& with_name(std::string name)
		{
			params_.name = std::move(name);
			return *this;
		}

		auto& with_user_data(std::any data)
		{
			params_.user_data = std::move(data);
			return *this;
		}

		/**
		 * @brief Explicit storage selection
		 */
		auto& static_storage()
		{
			storage_choice_ = StorageChoice::Static;
			return *this;
		}

		auto& dynamic_storage()
		{
			storage_choice_ = StorageChoice::Dynamic;
			return *this;
		}

		/**
		 * @brief Create the service with constructor arguments
		 */
		template<typename... Args>
		auto create(Args&&... args)
		{
			// Determine which interface/storage to use
			if (storage_choice_ == StorageChoice::Static)
			{
				// Static storage - need concrete derived type for CRTP
				// This is a limitation - static requires a concrete service type
				// We'll use a helper to generate one
				return create_static_impl(std::forward<Args>(args)...);
			}
			else  // Auto or Dynamic
			{
				// Dynamic storage - use void interface
				using Interface = InterfaceTemplate<void>;
				using Storage = DynamicStorage<Interface>;
				return Service<Interface, Storage>(
					Impl(std::forward<Args>(args)...),
					params_
				);
			}
		}

	private:
		template<typename... Args>
		auto create_static_impl(Args&&... args)
		{
			// For static storage, we need a concrete Service type
			// Generate one on the fly
			using Interface = InterfaceTemplate<Service<
				InterfaceTemplate<void>,
				StaticStorage<Impl>
			>>;
			using Storage = StaticStorage<Impl>;

			return Service<Interface, Storage>(
				Impl(std::forward<Args>(args)...),
				params_
			);
		}
	};

	/**
	 * @brief Convenience: Start building a service
	 *
	 * @example
	 * auto svc = service<ICalculator>()
	 *     .implementation<SimpleCalculator>()
	 *     .create();
	 */
	template<template<typename> class InterfaceTemplate>
	auto service()
	{
		return ServiceBuilder<InterfaceTemplate>{};
	}
}
