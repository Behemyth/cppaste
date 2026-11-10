#include <gtest/gtest.h>

import std;
import example.binding;

namespace
{
	// ============================================================================
	// PluginContract Tests
	// ============================================================================

	TEST(ExamplePluginContract, FunctionPointerTypes)
	{
		// Verify the ABI function pointer types are correctly defined
		constexpr bool create_is_fn_ptr = std::is_function_v<
			std::remove_pointer_t<example::binding::plugin_abi::CreateFn>
		>;
		constexpr bool destroy_is_fn_ptr = std::is_function_v<
			std::remove_pointer_t<example::binding::plugin_abi::DestroyFn>
		>;
		constexpr bool get_info_is_fn_ptr = std::is_function_v<
			std::remove_pointer_t<example::binding::plugin_abi::GetInfoFn>
		>;

		ASSERT_TRUE(create_is_fn_ptr);
		ASSERT_TRUE(destroy_is_fn_ptr);
		ASSERT_TRUE(get_info_is_fn_ptr);
	}

	TEST(ExamplePluginContract, SymbolNames)
	{
		// Verify symbol names are defined correctly
		EXPECT_EQ(example::binding::plugin_abi::symbols::create, "plugin_create");
		EXPECT_EQ(example::binding::plugin_abi::symbols::destroy, "plugin_destroy");
		EXPECT_EQ(example::binding::plugin_abi::symbols::get_info, "plugin_get_info");
	}

	TEST(ExamplePluginContract, PluginInfoStructure)
	{
		// Verify PluginInfo can be constructed and accessed
		example::binding::PluginInfo info{
			"TestPlugin",
			"1.0.0",
			"Test Author"
		};

		EXPECT_EQ(std::string_view(info.name), "TestPlugin");
		EXPECT_EQ(std::string_view(info.version), "1.0.0");
		EXPECT_EQ(std::string_view(info.author), "Test Author");
	}

	TEST(ExamplePluginContract, PluginContractInterfaceType)
	{
		// Verify PluginContract has correct interface_type
		constexpr bool same_type = std::is_same_v<
			example::binding::PluginContract::interface_type,
			example::binding::PluginContract
		>;
		ASSERT_TRUE(same_type);
	}

	// ============================================================================
	// PluginHost Tests
	// ============================================================================

	TEST(ExamplePluginHost, DefaultConstruction)
	{
		example::binding::PluginHost host;

		ASSERT_FALSE(host.IsLoaded());
		ASSERT_FALSE(host.HasInstance());
		EXPECT_EQ(host.GetInstance(), nullptr);
		EXPECT_TRUE(host.GetPath().empty());
	}

	TEST(ExamplePluginHost, PathConstruction)
	{
		std::filesystem::path plugin_path = "test_plugin.dll";
		example::binding::PluginHost host(plugin_path);

		ASSERT_FALSE(host.IsLoaded());
		ASSERT_FALSE(host.HasInstance());
		EXPECT_EQ(host.GetPath(), plugin_path);
	}

	TEST(ExamplePluginHost, LoadNonexistentPlugin)
	{
		example::binding::PluginHost host(std::filesystem::path("nonexistent_plugin.dll"));

		bool loaded = host.Load();

		ASSERT_FALSE(loaded);
		ASSERT_FALSE(host.IsLoaded());
		EXPECT_FALSE(host.GetLastError().empty());
	}

	TEST(ExamplePluginHost, CreateWithoutLoad)
	{
		example::binding::PluginHost host;

		bool created = host.Create();

		ASSERT_FALSE(created);
		ASSERT_FALSE(host.HasInstance());
	}

	TEST(ExamplePluginHost, DestroyWithoutInstance)
	{
		example::binding::PluginHost host;

		// Should not throw or crash
		host.Destroy();

		ASSERT_FALSE(host.HasInstance());
	}

	TEST(ExamplePluginHost, UnloadWithoutLoad)
	{
		example::binding::PluginHost host;

		// Should not throw or crash
		host.Unload();

		ASSERT_FALSE(host.IsLoaded());
	}

	TEST(ExamplePluginHost, GetInfoWithoutLoad)
	{
		example::binding::PluginHost host;

		auto info = host.GetInfo();

		ASSERT_FALSE(info.has_value());
	}

	TEST(ExamplePluginHost, MoveConstruction)
	{
		std::filesystem::path plugin_path = "test_plugin.dll";
		example::binding::PluginHost host1(plugin_path);
		example::binding::PluginHost host2(std::move(host1));

		EXPECT_EQ(host2.GetPath(), plugin_path);
		EXPECT_TRUE(host1.GetPath().empty());
	}

	TEST(ExamplePluginHost, MoveAssignment)
	{
		std::filesystem::path path1 = "plugin1.dll";
		std::filesystem::path path2 = "plugin2.dll";

		example::binding::PluginHost host1(path1);
		example::binding::PluginHost host2(path2);

		host2 = std::move(host1);

		EXPECT_EQ(host2.GetPath(), path1);
		EXPECT_TRUE(host1.GetPath().empty());
	}

	// ============================================================================
	// Integration-style Tests (without actual plugin)
	// ============================================================================

	TEST(ExamplePluginWorkflow, FullLifecycleWithoutPlugin)
	{
		// Test the expected workflow even without a real plugin
		example::binding::PluginHost host(std::filesystem::path("fake_plugin.dll"));

		// Initial state
		ASSERT_FALSE(host.IsLoaded());
		ASSERT_FALSE(host.HasInstance());

		// Attempt load (will fail, but tests error handling)
		bool loaded = host.Load();
		ASSERT_FALSE(loaded);

		// Cleanup should be safe
		host.Destroy();
		host.Unload();

		ASSERT_FALSE(host.IsLoaded());
		ASSERT_FALSE(host.HasInstance());
	}

	TEST(ExamplePluginWorkflow, MultipleUnloadCalls)
	{
		example::binding::PluginHost host(std::filesystem::path("test.dll"));

		// Multiple unload calls should be safe
		host.Unload();
		host.Unload();
		host.Unload();

		ASSERT_FALSE(host.IsLoaded());
	}

	TEST(ExamplePluginWorkflow, MultipleDestroyCalls)
	{
		example::binding::PluginHost host;

		// Multiple destroy calls should be safe
		host.Destroy();
		host.Destroy();
		host.Destroy();

		ASSERT_FALSE(host.HasInstance());
	}

	// ============================================================================
	// Plugin Loading Integration Tests (require the compiled test_plugin library)
	// ============================================================================

	TEST(ExamplePluginLoading, LoadTestPlugin)
	{
		// The build passes the test plugin's path; its file name differs by platform
		std::filesystem::path plugin_path = CPPASTE_TEST_PLUGIN_PATH;

		example::binding::PluginHost host(plugin_path);
		bool loaded = host.Load();

		ASSERT_TRUE(loaded);
		ASSERT_TRUE(host.IsLoaded());
		ASSERT_FALSE(host.HasInstance());

		host.Unload();
		ASSERT_FALSE(host.IsLoaded());
	}

	TEST(ExamplePluginLoading, CreateAndDestroyInstance)
	{
		std::filesystem::path plugin_path = CPPASTE_TEST_PLUGIN_PATH;
		example::binding::PluginHost host(plugin_path);

		ASSERT_TRUE(host.Load());

		// Create instance
		bool created = host.Create(nullptr);
		ASSERT_TRUE(created);
		ASSERT_TRUE(host.HasInstance());
		ASSERT_NE(host.GetInstance(), nullptr);

		// Destroy instance
		host.Destroy();
		ASSERT_FALSE(host.HasInstance());
		ASSERT_EQ(host.GetInstance(), nullptr);

		host.Unload();
	}

	TEST(ExamplePluginLoading, GetPluginInfo)
	{
		std::filesystem::path plugin_path = CPPASTE_TEST_PLUGIN_PATH;
		example::binding::PluginHost host(plugin_path);

		ASSERT_TRUE(host.Load());

		auto info = host.GetInfo();
		ASSERT_TRUE(info.has_value());

		EXPECT_EQ(std::string_view(info->name), "TestPlugin");
		EXPECT_EQ(std::string_view(info->version), "1.0.0");
		EXPECT_EQ(std::string_view(info->author), "cppaste Test");

		host.Unload();
	}

	TEST(ExamplePluginLoading, CreateWithContext)
	{
		std::filesystem::path plugin_path = CPPASTE_TEST_PLUGIN_PATH;
		example::binding::PluginHost host(plugin_path);

		ASSERT_TRUE(host.Load());

		// Create with a context pointer
		int context_value = 12345;
		bool created = host.Create(&context_value);
		ASSERT_TRUE(created);
		ASSERT_TRUE(host.HasInstance());

		host.Destroy();
		host.Unload();
	}

	TEST(ExamplePluginLoading, MultipleHosts)
	{
		std::filesystem::path plugin_path = CPPASTE_TEST_PLUGIN_PATH;
		example::binding::PluginHost host1(plugin_path);
		example::binding::PluginHost host2(plugin_path);

		ASSERT_TRUE(host1.Load());
		ASSERT_TRUE(host2.Load());

		// Create instances in both hosts
		ASSERT_TRUE(host1.Create());
		ASSERT_TRUE(host2.Create());

		// Both should have independent instances
		ASSERT_TRUE(host1.HasInstance());
		ASSERT_TRUE(host2.HasInstance());
		ASSERT_NE(host1.GetInstance(), host2.GetInstance());

		host1.Unload();
		host2.Unload();
	}

	TEST(ExamplePluginLoading, ReloadPlugin)
	{
		std::filesystem::path plugin_path = CPPASTE_TEST_PLUGIN_PATH;
		example::binding::PluginHost host(plugin_path);

		ASSERT_TRUE(host.Load());

		// First load/create/destroy cycle
		ASSERT_TRUE(host.Create());
		host.Destroy();
		host.Unload();

		// Second load/create cycle
		ASSERT_TRUE(host.Load());
		ASSERT_TRUE(host.Create());
		ASSERT_TRUE(host.HasInstance());

		host.Unload();
	}

	TEST(ExamplePluginLoading, DestructorCleanup)
	{
		std::filesystem::path plugin_path = CPPASTE_TEST_PLUGIN_PATH;

		{
			example::binding::PluginHost host(plugin_path);
			ASSERT_TRUE(host.Load());
			host.Create();
			// Let destructor handle cleanup
		}

		// If we get here without crashing, the destructor worked correctly
		ASSERT_TRUE(true);
	}

	TEST(ExamplePluginLoading, DoubleCreateFails)
	{
		std::filesystem::path plugin_path = CPPASTE_TEST_PLUGIN_PATH;
		example::binding::PluginHost host(plugin_path);

		ASSERT_TRUE(host.Load());

		// First create should succeed
		ASSERT_TRUE(host.Create());

		// Second create should fail (already have instance)
		ASSERT_FALSE(host.Create());

		host.Unload();
	}

	TEST(ExamplePluginLoading, OperationsAfterUnload)
	{
		std::filesystem::path plugin_path = CPPASTE_TEST_PLUGIN_PATH;
		example::binding::PluginHost host(plugin_path);

		ASSERT_TRUE(host.Load());
		host.Unload();

		// These should all be safe and return appropriate values
		ASSERT_FALSE(host.IsLoaded());
		ASSERT_FALSE(host.Create());
		ASSERT_FALSE(host.GetInfo().has_value());

		// These should be no-ops
		host.Destroy();
		host.Unload();

		ASSERT_TRUE(true);
	}

}

