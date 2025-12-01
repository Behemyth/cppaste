import std;
import synodic.honesty.test;
import example.binding;

using namespace honesty::test::literals;

namespace
{
	// ============================================================================
	// PluginContract Tests
	// ============================================================================

	honesty::test::Suite PLUGIN_CONTRACT_SUITE(
		"example_plugin_contract",
		[]() -> honesty::test::Generator
		{
			co_yield "function_pointer_types"_test = [](const honesty::test::Requirements& requirements)
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

				requirements.Assert(create_is_fn_ptr);
				requirements.Assert(destroy_is_fn_ptr);
				requirements.Assert(get_info_is_fn_ptr);
			};

			co_yield "symbol_names"_test = [](const honesty::test::Requirements& requirements)
			{
				// Verify symbol names are defined correctly
				requirements.Expect(example::binding::plugin_abi::symbols::create == "plugin_create");
				requirements.Expect(example::binding::plugin_abi::symbols::destroy == "plugin_destroy");
				requirements.Expect(example::binding::plugin_abi::symbols::get_info == "plugin_get_info");
			};

			co_yield "plugin_info_structure"_test = [](const honesty::test::Requirements& requirements)
			{
				// Verify PluginInfo can be constructed and accessed
				example::binding::PluginInfo info{
					"TestPlugin",
					"1.0.0",
					"Test Author"
				};

				requirements.Expect(std::string_view(info.name) == "TestPlugin");
				requirements.Expect(std::string_view(info.version) == "1.0.0");
				requirements.Expect(std::string_view(info.author) == "Test Author");
			};

			co_yield "plugin_contract_interface_type"_test = [](const honesty::test::Requirements& requirements)
			{
				// Verify PluginContract has correct interface_type
				constexpr bool same_type = std::is_same_v<
					example::binding::PluginContract::interface_type,
					example::binding::PluginContract
				>;
				requirements.Assert(same_type);
			};
		});

	honesty::test::SuiteRegistrar _1(PLUGIN_CONTRACT_SUITE);

	// ============================================================================
	// PluginHost Tests
	// ============================================================================

	honesty::test::Suite PLUGIN_HOST_SUITE(
		"example_plugin_host",
		[]() -> honesty::test::Generator
		{
			co_yield "default_construction"_test = [](const honesty::test::Requirements& requirements)
			{
				example::binding::PluginHost host;

				requirements.Assert(!host.IsLoaded());
				requirements.Assert(!host.HasInstance());
				requirements.Expect(host.GetInstance() == nullptr);
				requirements.Expect(host.GetPath().empty());
			};

			co_yield "path_construction"_test = [](const honesty::test::Requirements& requirements)
			{
				std::filesystem::path plugin_path = "test_plugin.dll";
				example::binding::PluginHost host(plugin_path);

				requirements.Assert(!host.IsLoaded());
				requirements.Assert(!host.HasInstance());
				requirements.Expect(host.GetPath() == plugin_path);
			};

			co_yield "load_nonexistent_plugin"_test = [](const honesty::test::Requirements& requirements)
			{
				example::binding::PluginHost host(std::filesystem::path("nonexistent_plugin.dll"));

				bool loaded = host.Load();

				requirements.Assert(!loaded);
				requirements.Assert(!host.IsLoaded());
				requirements.Expect(!host.GetLastError().empty());
			};

			co_yield "create_without_load"_test = [](const honesty::test::Requirements& requirements)
			{
				example::binding::PluginHost host;

				bool created = host.Create();

				requirements.Assert(!created);
				requirements.Assert(!host.HasInstance());
			};

			co_yield "destroy_without_instance"_test = [](const honesty::test::Requirements& requirements)
			{
				example::binding::PluginHost host;

				// Should not throw or crash
				host.Destroy();

				requirements.Assert(!host.HasInstance());
			};

			co_yield "unload_without_load"_test = [](const honesty::test::Requirements& requirements)
			{
				example::binding::PluginHost host;

				// Should not throw or crash
				host.Unload();

				requirements.Assert(!host.IsLoaded());
			};

			co_yield "get_info_without_load"_test = [](const honesty::test::Requirements& requirements)
			{
				example::binding::PluginHost host;

				auto info = host.GetInfo();

				requirements.Assert(!info.has_value());
			};

			co_yield "move_construction"_test = [](const honesty::test::Requirements& requirements)
			{
				std::filesystem::path plugin_path = "test_plugin.dll";
				example::binding::PluginHost host1(plugin_path);
				example::binding::PluginHost host2(std::move(host1));

				requirements.Expect(host2.GetPath() == plugin_path);
				requirements.Expect(host1.GetPath().empty());
			};

			co_yield "move_assignment"_test = [](const honesty::test::Requirements& requirements)
			{
				std::filesystem::path path1 = "plugin1.dll";
				std::filesystem::path path2 = "plugin2.dll";

				example::binding::PluginHost host1(path1);
				example::binding::PluginHost host2(path2);

				host2 = std::move(host1);

				requirements.Expect(host2.GetPath() == path1);
				requirements.Expect(host1.GetPath().empty());
			};
		});

	honesty::test::SuiteRegistrar _2(PLUGIN_HOST_SUITE);

	// ============================================================================
	// Integration-style Tests (without actual plugin)
	// ============================================================================

	honesty::test::Suite PLUGIN_WORKFLOW_SUITE(
		"example_plugin_workflow",
		[]() -> honesty::test::Generator
		{
			co_yield "full_lifecycle_without_plugin"_test = [](const honesty::test::Requirements& requirements)
			{
				// Test the expected workflow even without a real plugin
				example::binding::PluginHost host(std::filesystem::path("fake_plugin.dll"));

				// Initial state
				requirements.Assert(!host.IsLoaded());
				requirements.Assert(!host.HasInstance());

				// Attempt load (will fail, but tests error handling)
				bool loaded = host.Load();
				requirements.Assert(!loaded);

				// Cleanup should be safe
				host.Destroy();
				host.Unload();

				requirements.Assert(!host.IsLoaded());
				requirements.Assert(!host.HasInstance());
			};

			co_yield "multiple_unload_calls"_test = [](const honesty::test::Requirements& requirements)
			{
				example::binding::PluginHost host(std::filesystem::path("test.dll"));

				// Multiple unload calls should be safe
				host.Unload();
				host.Unload();
				host.Unload();

				requirements.Assert(!host.IsLoaded());
			};

			co_yield "multiple_destroy_calls"_test = [](const honesty::test::Requirements& requirements)
			{
				example::binding::PluginHost host;

				// Multiple destroy calls should be safe
				host.Destroy();
				host.Destroy();
				host.Destroy();

				requirements.Assert(!host.HasInstance());
			};
		});

	honesty::test::SuiteRegistrar _3(PLUGIN_WORKFLOW_SUITE);

	// ============================================================================
	// Plugin Loading Integration Tests (require compiled test_plugin DLL)
	// ============================================================================

	honesty::test::Suite PLUGIN_LOADING_SUITE(
		"example_plugin_loading",
		[]() -> honesty::test::Generator
		{
			co_yield "load_test_plugin"_test = [](const honesty::test::Requirements& requirements)
			{
				// The test plugin DLL should be in the same directory as the test executable
				std::filesystem::path plugin_path = "test_plugin.dll";

				example::binding::PluginHost host(plugin_path);
				bool loaded = host.Load();

				requirements.Assert(loaded);
				requirements.Assert(host.IsLoaded());
				requirements.Assert(!host.HasInstance());

				host.Unload();
				requirements.Assert(!host.IsLoaded());
			};

			co_yield "create_and_destroy_instance"_test = [](const honesty::test::Requirements& requirements)
			{
				std::filesystem::path plugin_path = "test_plugin.dll";
				example::binding::PluginHost host(plugin_path);

				requirements.Assert(host.Load());

				// Create instance
				bool created = host.Create(nullptr);
				requirements.Assert(created);
				requirements.Assert(host.HasInstance());
				requirements.Assert(host.GetInstance() != nullptr);

				// Destroy instance
				host.Destroy();
				requirements.Assert(!host.HasInstance());
				requirements.Assert(host.GetInstance() == nullptr);

				host.Unload();
			};

			co_yield "get_plugin_info"_test = [](const honesty::test::Requirements& requirements)
			{
				std::filesystem::path plugin_path = "test_plugin.dll";
				example::binding::PluginHost host(plugin_path);

				requirements.Assert(host.Load());

				auto info = host.GetInfo();
				requirements.Assert(info.has_value());

				requirements.Expect(std::string_view(info->name) == "TestPlugin");
				requirements.Expect(std::string_view(info->version) == "1.0.0");
				requirements.Expect(std::string_view(info->author) == "CPPaste Test");

				host.Unload();
			};

			co_yield "create_with_context"_test = [](const honesty::test::Requirements& requirements)
			{
				std::filesystem::path plugin_path = "test_plugin.dll";
				example::binding::PluginHost host(plugin_path);

				requirements.Assert(host.Load());

				// Create with a context pointer
				int context_value = 12345;
				bool created = host.Create(&context_value);
				requirements.Assert(created);
				requirements.Assert(host.HasInstance());

				host.Destroy();
				host.Unload();
			};

			co_yield "multiple_hosts"_test = [](const honesty::test::Requirements& requirements)
			{
				std::filesystem::path plugin_path = "test_plugin.dll";
				example::binding::PluginHost host1(plugin_path);
				example::binding::PluginHost host2(plugin_path);

				requirements.Assert(host1.Load());
				requirements.Assert(host2.Load());

				// Create instances in both hosts
				requirements.Assert(host1.Create());
				requirements.Assert(host2.Create());

				// Both should have independent instances
				requirements.Assert(host1.HasInstance());
				requirements.Assert(host2.HasInstance());
				requirements.Assert(host1.GetInstance() != host2.GetInstance());

				host1.Unload();
				host2.Unload();
			};

			co_yield "reload_plugin"_test = [](const honesty::test::Requirements& requirements)
			{
				std::filesystem::path plugin_path = "test_plugin.dll";
				example::binding::PluginHost host(plugin_path);

				requirements.Assert(host.Load());

				// First load/create/destroy cycle
				requirements.Assert(host.Create());
				host.Destroy();
				host.Unload();

				// Second load/create cycle
				requirements.Assert(host.Load());
				requirements.Assert(host.Create());
				requirements.Assert(host.HasInstance());

				host.Unload();
			};

			co_yield "destructor_cleanup"_test = [](const honesty::test::Requirements& requirements)
			{
				std::filesystem::path plugin_path = "test_plugin.dll";

				{
					example::binding::PluginHost host(plugin_path);
					requirements.Assert(host.Load());
					host.Create();
					// Let destructor handle cleanup
				}

				// If we get here without crashing, the destructor worked correctly
				requirements.Assert(true);
			};

			co_yield "double_create_fails"_test = [](const honesty::test::Requirements& requirements)
			{
				std::filesystem::path plugin_path = "test_plugin.dll";
				example::binding::PluginHost host(plugin_path);

				requirements.Assert(host.Load());

				// First create should succeed
				requirements.Assert(host.Create());

				// Second create should fail (already have instance)
				requirements.Assert(!host.Create());

				host.Unload();
			};

			co_yield "operations_after_unload"_test = [](const honesty::test::Requirements& requirements)
			{
				std::filesystem::path plugin_path = "test_plugin.dll";
				example::binding::PluginHost host(plugin_path);

				requirements.Assert(host.Load());
				host.Unload();

				// These should all be safe and return appropriate values
				requirements.Assert(!host.IsLoaded());
				requirements.Assert(!host.Create());
				requirements.Assert(!host.GetInfo().has_value());

				// These should be no-ops
				host.Destroy();
				host.Unload();

				requirements.Assert(true);
			};
		});

	honesty::test::SuiteRegistrar _4(PLUGIN_LOADING_SUITE);
}
