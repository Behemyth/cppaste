export module synodic.cppaste.binding:backends.nanobind;

import std;

export namespace cppaste::binding::nanobind
{
	/**
	 * @brief Metadata for a bound type
	 *
	 * Stores information needed to generate Python bindings for a C++ type.
	 * This is used pre-reflection to manually register types for binding.
	 *
	 * TODO(reflection): Auto-populate from [[bind]] annotated types
	 */
	struct TypeMetadata
	{
		std::string_view name;           // Python-visible type name
		std::string_view module_name;    // Module to register in
		std::string_view docstring;      // Type documentation
		std::size_t type_size;           // sizeof(T)
		std::size_t type_align;          // alignof(T)
	};

	/**
	 * @brief Metadata for a bound method
	 */
	struct MethodMetadata
	{
		std::string_view name;           // Python method name
		std::string_view cpp_name;       // C++ method name (for lookup)
		std::string_view docstring;      // Method documentation
		std::string_view signature;      // Human-readable signature
		bool is_static = false;          // Is this a static method?
		bool is_const = false;           // Is this a const method?
	};

	/**
	 * @brief Metadata for a bound property
	 */
	struct PropertyMetadata
	{
		std::string_view name;           // Python property name
		std::string_view docstring;      // Property documentation
		bool readable = true;            // Has getter
		bool writable = true;            // Has setter
	};

	/**
	 * @brief Registry of types to be bound
	 *
	 * Collects type metadata that can be used by binding generators
	 * (nanobind, pybind11, etc.) to create Python modules.
	 */
	class BindingRegistry
	{
	public:
		struct TypeEntry
		{
			TypeMetadata type;
			std::vector<MethodMetadata> methods;
			std::vector<PropertyMetadata> properties;
		};

		/**
		 * @brief Register a type for binding
		 */
		void register_type(TypeMetadata meta)
		{
			types_.emplace_back(TypeEntry{std::move(meta), {}, {}});
		}

		/**
		 * @brief Add a method to the most recently registered type
		 */
		void add_method(MethodMetadata meta)
		{
			if (!types_.empty())
			{
				types_.back().methods.push_back(std::move(meta));
			}
		}

		/**
		 * @brief Add a property to the most recently registered type
		 */
		void add_property(PropertyMetadata meta)
		{
			if (!types_.empty())
			{
				types_.back().properties.push_back(std::move(meta));
			}
		}

		/**
		 * @brief Get all registered types
		 */
		const std::vector<TypeEntry>& types() const { return types_; }

		/**
		 * @brief Clear all registrations
		 */
		void clear() { types_.clear(); }

	private:
		std::vector<TypeEntry> types_;
	};

	/**
	 * @brief CRTP base for types that provide their own binding metadata
	 *
	 * Types inheriting from this can define their binding metadata statically.
	 * The binding generator will call T::get_binding_metadata() to retrieve it.
	 *
	 * Example:
	 * @code
	 * class MyService : public Bindable<MyService>
	 * {
	 * public:
	 *     static auto get_type_metadata() -> TypeMetadata
	 *     {
	 *         return {"MyService", "mymodule", "My service class", sizeof(MyService), alignof(MyService)};
	 *     }
	 *
	 *     static auto get_methods() -> std::vector<MethodMetadata>
	 *     {
	 *         return {{"do_thing", "DoThing", "Does a thing", "() -> None", false, false}};
	 *     }
	 * };
	 * @endcode
	 *
	 * @tparam T The derived bindable type
	 */
	template<typename T>
	struct Bindable
	{
		/**
		 * @brief Check if type provides binding metadata
		 */
		static constexpr bool has_binding_metadata =
			requires { T::get_type_metadata(); };

		/**
		 * @brief Register this type in a registry
		 */
		static void register_in(BindingRegistry& registry)
		{
			if constexpr (has_binding_metadata)
			{
				registry.register_type(T::get_type_metadata());

				if constexpr (requires { T::get_methods(); })
				{
					for (auto& m : T::get_methods())
					{
						registry.add_method(std::move(m));
					}
				}

				if constexpr (requires { T::get_properties(); })
				{
					for (auto& p : T::get_properties())
					{
						registry.add_property(std::move(p));
					}
				}
			}
		}
	};

	/**
	 * @brief Helper to bind lifecycle services to Python
	 *
	 * Provides utilities for exposing ILifecycle-compatible services
	 * through the C ABI to Python bindings.
	 */
	struct LifecycleBindingHelper
	{
		/**
		 * @brief Standard lifecycle method names for Python
		 */
		static constexpr std::string_view initialize_name = "initialize";
		static constexpr std::string_view update_name = "update";
		static constexpr std::string_view render_name = "render";
		static constexpr std::string_view pause_name = "pause";
		static constexpr std::string_view resume_name = "resume";
		static constexpr std::string_view shutdown_name = "shutdown";

		/**
		 * @brief Generate method metadata for lifecycle interface
		 */
		static auto get_lifecycle_methods() -> std::vector<MethodMetadata>
		{
			return {
				{initialize_name, "Initialize", "Initialize the service", "() -> bool", false, false},
				{update_name, "Update", "Update the service", "(delta_time: float) -> None", false, false},
				{render_name, "Render", "Render the service output", "() -> None", false, false},
				{pause_name, "Pause", "Pause the service", "() -> None", false, false},
				{resume_name, "Resume", "Resume the service", "() -> None", false, false},
				{shutdown_name, "Shutdown", "Shutdown the service", "() -> None", false, false},
			};
		}
	};

	/**
	 * @brief Helper to bind plugin loaders to Python
	 *
	 * Provides utilities for exposing PluginLoader and PluginRegistry
	 * to Python for editor integration.
	 */
	struct PluginBindingHelper
	{
		/**
		 * @brief Standard plugin loader method names for Python
		 */
		static constexpr std::string_view load_name = "load";
		static constexpr std::string_view unload_name = "unload";
		static constexpr std::string_view is_loaded_name = "is_loaded";
		static constexpr std::string_view path_name = "path";

		/**
		 * @brief Generate method metadata for plugin loader
		 */
		static auto get_loader_methods() -> std::vector<MethodMetadata>
		{
			return {
				{load_name, "load", "Load the plugin library", "() -> bool", false, false},
				{unload_name, "unload", "Unload the plugin library", "() -> None", false, false},
				{is_loaded_name, "is_loaded", "Check if plugin is loaded", "() -> bool", false, true},
			};
		}

		/**
		 * @brief Generate property metadata for plugin loader
		 */
		static auto get_loader_properties() -> std::vector<PropertyMetadata>
		{
			return {
				{path_name, "Path to the plugin library", true, false},
			};
		}
	};
}

