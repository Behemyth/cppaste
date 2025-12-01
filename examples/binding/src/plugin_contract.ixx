export module example.binding:plugin_contract;

import std;
import synodic.cppaste.binding;

export namespace example::binding
{
	/**
	 * @brief Plugin information structure
	 *
	 * Returned by plugins to identify themselves.
	 */
	struct PluginInfo
	{
		const char* name;
		const char* version;
		const char* author;
	};

	/**
	 * @brief C ABI function signatures for plugins
	 *
	 * Plugins must export functions matching these signatures.
	 */
	namespace plugin_abi
	{
		/// Create a new plugin instance
		/// @param context Host-provided context pointer (may be nullptr)
		/// @return Opaque handle to plugin instance
		using CreateFn = void* (*)(void* context);

		/// Destroy a plugin instance
		/// @param instance Handle returned by Create
		using DestroyFn = void (*)(void* instance);

		/// Get plugin information
		/// @return Plugin metadata
		using GetInfoFn = PluginInfo (*)(void);

		/**
		 * @brief Standard symbol names for plugins
		 */
		namespace symbols
		{
			constexpr std::string_view create = "plugin_create";
			constexpr std::string_view destroy = "plugin_destroy";
			constexpr std::string_view get_info = "plugin_get_info";
		}
	}

	/**
	 * @brief Plugin interface definition
	 *
	 * Used with cppaste::PluginLoader to type-safely load plugin DLLs.
	 */
	struct PluginContract : cppaste::PluginInterface<PluginContract>
	{
		using CreateFn = plugin_abi::CreateFn;
		using DestroyFn = plugin_abi::DestroyFn;
		using GetInfoFn = plugin_abi::GetInfoFn;
	};
}
