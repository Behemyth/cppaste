/**
 * @brief Test plugin implementation for binding example tests
 *
 * This plugin is used to test the PluginHost loading and lifecycle functionality.
 */

import std;

#include "plugin_interface.h"

namespace
{
	/**
	 * @brief Simple test plugin implementation
	 */
	struct TestPlugin
	{
		void* context;
		int value;

		explicit TestPlugin(void* ctx)
			: context(ctx)
			, value(42)
		{}
	};

	// Plugin metadata
	constexpr const char* PLUGIN_NAME = "TestPlugin";
	constexpr const char* PLUGIN_VERSION = "1.0.0";
	constexpr const char* PLUGIN_AUTHOR = "CPPaste Test";
}

extern "C"
{

PLUGIN_EXPORT void* plugin_create(void* context)
{
	return new TestPlugin(context);
}

PLUGIN_EXPORT void plugin_destroy(void* instance)
{
	delete static_cast<TestPlugin*>(instance);
}

PLUGIN_EXPORT PluginInfo plugin_get_info(void)
{
	return PluginInfo{PLUGIN_NAME, PLUGIN_VERSION, PLUGIN_AUTHOR};
}

/**
 * @brief Get the stored value (custom plugin function)
 * @param instance Plugin instance
 * @return The value stored in the plugin
 */
PLUGIN_EXPORT int plugin_get_value(void* instance)
{
	if (!instance)
	{
		return -1;
	}
	return static_cast<TestPlugin*>(instance)->value;
}

/**
 * @brief Set the stored value (custom plugin function)
 * @param instance Plugin instance
 * @param value New value to store
 */
PLUGIN_EXPORT void plugin_set_value(void* instance, int value)
{
	if (instance)
	{
		static_cast<TestPlugin*>(instance)->value = value;
	}
}

/**
 * @brief Get the context pointer (for testing context passing)
 * @param instance Plugin instance
 * @return The context pointer passed to plugin_create
 */
PLUGIN_EXPORT void* plugin_get_context(void* instance)
{
	if (!instance)
	{
		return nullptr;
	}
	return static_cast<TestPlugin*>(instance)->context;
}

}
