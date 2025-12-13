/**
 * @file plugin_interface.h
 * @brief C ABI plugin interface
 *
 * This header defines the C ABI contract that plugins must implement.
 * Include this from plugin projects to ensure correct function signatures.
 *
 * Required exports:
 * - plugin_create(context) -> instance
 * - plugin_destroy(instance)
 *
 * Optional exports:
 * - plugin_get_info() -> PluginInfo
 */

#ifndef BINDING_PLUGIN_INTERFACE_H
#define BINDING_PLUGIN_INTERFACE_H

#include <cppaste/export.h>

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	 * @brief Plugin information structure
	 */
	typedef struct
	{
		const char* name; /**< Plugin display name */
		const char* version; /**< Plugin version string */
		const char* author; /**< Plugin author */
	} PluginInfo;

	/**
	 * @brief Create a plugin instance (REQUIRED)
	 * @param context Host-provided context pointer (may be NULL)
	 * @return Opaque plugin instance pointer
	 *
	 * Example:
	 * @code
	 * CPPASTE_EXPORT void* plugin_create(void* context) {
	 *     return new MyPlugin(context);
	 * }
	 * @endcode
	 */
	/* CPPASTE_EXPORT void* plugin_create(void* context); */

	/**
	 * @brief Destroy a plugin instance (REQUIRED)
	 * @param instance Plugin instance from plugin_create
	 *
	 * Example:
	 * @code
	 * CPPASTE_EXPORT void plugin_destroy(void* instance) {
	 *     delete static_cast<MyPlugin*>(instance);
	 * }
	 * @endcode
	 */
	/* CPPASTE_EXPORT void plugin_destroy(void* instance); */

	/**
	 * @brief Get plugin information (OPTIONAL)
	 * @return Plugin metadata
	 *
	 * Example:
	 * @code
	 * CPPASTE_EXPORT PluginInfo plugin_get_info(void) {
	 *     return {"MyPlugin", "1.0.0", "Author Name"};
	 * }
	 * @endcode
	 */
	/* CPPASTE_EXPORT PluginInfo plugin_get_info(void); */

#ifdef __cplusplus
}
#endif

#endif /* BINDING_PLUGIN_INTERFACE_H */
