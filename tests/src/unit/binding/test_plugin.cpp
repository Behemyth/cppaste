import std;
import synodic.honesty.test;
import synodic.cppaste.binding;

using namespace honesty::test::literals;

namespace
{
	/**
	 * @brief Example plugin interface for testing
	 *
	 * Defines function pointer types and symbol names that a test plugin would export.
	 */
	struct TestPluginInterface : cppaste::PluginInterface<TestPluginInterface>
	{
		// Function pointer types matching C ABI exports
		using CreateFn = void* (*)(void* context);
		using DestroyFn = void (*)(void* instance);
		using ProcessFn = int (*)(void* instance, int value);

		// Symbol names to look up
		static constexpr std::string_view create_symbol = "test_create";
		static constexpr std::string_view destroy_symbol = "test_destroy";
		static constexpr std::string_view process_symbol = "test_process";
	};

	// ============================================================================
	// DynamicLibrary Tests
	// ============================================================================

	honesty::test::Suite DYNAMIC_LIBRARY_SUITE(
		"dynamic_library",
		[]() -> honesty::test::Generator
		{
			co_yield "default_construction"_test = [](const honesty::test::Requirements& requirements)
			{
				cppaste::DynamicLibrary library;
				requirements.Assert(!library.is_loaded());
				requirements.Expect(library.native_handle() == nullptr);
			};

			co_yield "path_construction"_test = [](const honesty::test::Requirements& requirements)
			{
				std::filesystem::path test_path = "nonexistent_library.dll";
				cppaste::DynamicLibrary library(test_path);

				requirements.Assert(!library.is_loaded());
				requirements.Expect(library.path() == test_path);
			};

			co_yield "load_nonexistent_library"_test = [](const honesty::test::Requirements& requirements)
			{
				cppaste::DynamicLibrary library(std::filesystem::path("nonexistent_library.dll"));
				auto result = library.load();

				requirements.Assert(!result);
				requirements.Assert(!library.is_loaded());
				requirements.Expect(!result.error.empty());
			};

			co_yield "move_construction"_test = [](const honesty::test::Requirements& requirements)
			{
				std::filesystem::path test_path = "test_library.dll";
				cppaste::DynamicLibrary library1(test_path);
				cppaste::DynamicLibrary library2(std::move(library1));

				requirements.Expect(library2.path() == test_path);
				requirements.Expect(library1.path().empty());
			};

			co_yield "move_assignment"_test = [](const honesty::test::Requirements& requirements)
			{
				std::filesystem::path path1 = "library1.dll";
				std::filesystem::path path2 = "library2.dll";

				cppaste::DynamicLibrary library1(path1);
				cppaste::DynamicLibrary library2(path2);

				library2 = std::move(library1);

				requirements.Expect(library2.path() == path1);
				requirements.Expect(library1.path().empty());
			};

			co_yield "unload_unloaded_library"_test = [](const honesty::test::Requirements& requirements)
			{
				cppaste::DynamicLibrary library;

				// Should not throw or crash
				library.unload();

				requirements.Assert(!library.is_loaded());
			};

			co_yield "get_symbol_unloaded"_test = [](const honesty::test::Requirements& requirements)
			{
				cppaste::DynamicLibrary library;

				auto fn = library.get_symbol<void(*)()>("test_symbol");

				requirements.Assert(fn == nullptr);
			};
		});

	honesty::test::SuiteRegistrar _1(DYNAMIC_LIBRARY_SUITE);

	// ============================================================================
	// PluginInterface Tests
	// ============================================================================

	honesty::test::Suite PLUGIN_INTERFACE_SUITE(
		"plugin_interface",
		[]() -> honesty::test::Generator
		{
			co_yield "interface_type_alias"_test = [](const honesty::test::Requirements& requirements)
			{
				// Verify that interface_type is correctly defined
				constexpr bool same_type = std::is_same_v<
					TestPluginInterface::interface_type,
					TestPluginInterface
				>;
				requirements.Assert(same_type);
			};

			co_yield "symbol_names_defined"_test = [](const honesty::test::Requirements& requirements)
			{
				requirements.Expect(TestPluginInterface::create_symbol == "test_create");
				requirements.Expect(TestPluginInterface::destroy_symbol == "test_destroy");
				requirements.Expect(TestPluginInterface::process_symbol == "test_process");
			};

			co_yield "function_pointer_types"_test = [](const honesty::test::Requirements& requirements)
			{
				// Verify function pointer types are callable
				constexpr bool create_is_fn_ptr = std::is_function_v<
					std::remove_pointer_t<TestPluginInterface::CreateFn>
				>;
				constexpr bool destroy_is_fn_ptr = std::is_function_v<
					std::remove_pointer_t<TestPluginInterface::DestroyFn>
				>;
				constexpr bool process_is_fn_ptr = std::is_function_v<
					std::remove_pointer_t<TestPluginInterface::ProcessFn>
				>;

				requirements.Assert(create_is_fn_ptr);
				requirements.Assert(destroy_is_fn_ptr);
				requirements.Assert(process_is_fn_ptr);
			};
		});

	honesty::test::SuiteRegistrar _2(PLUGIN_INTERFACE_SUITE);

	// ============================================================================
	// PluginLoader Tests
	// ============================================================================

	honesty::test::Suite PLUGIN_LOADER_SUITE(
		"plugin_loader",
		[]() -> honesty::test::Generator
		{
			co_yield "default_construction"_test = [](const honesty::test::Requirements& requirements)
			{
				cppaste::PluginLoader<TestPluginInterface> loader;

				requirements.Assert(!loader.is_loaded());
				requirements.Expect(loader.path().empty());
			};

			co_yield "path_construction"_test = [](const honesty::test::Requirements& requirements)
			{
				std::filesystem::path plugin_path = "test_plugin.dll";
				cppaste::PluginLoader<TestPluginInterface> loader(plugin_path);

				requirements.Assert(!loader.is_loaded());
				requirements.Expect(loader.path() == plugin_path);
			};

			co_yield "load_nonexistent_plugin"_test = [](const honesty::test::Requirements& requirements)
			{
				cppaste::PluginLoader<TestPluginInterface> loader(
					std::filesystem::path("nonexistent_plugin.dll")
				);

				bool loaded = loader.load();

				requirements.Assert(!loaded);
				requirements.Assert(!loader.is_loaded());
				requirements.Expect(!loader.last_error().empty());
			};

			co_yield "unload_unloaded_plugin"_test = [](const honesty::test::Requirements& requirements)
			{
				cppaste::PluginLoader<TestPluginInterface> loader;

				// Should not throw or crash
				loader.unload();

				requirements.Assert(!loader.is_loaded());
			};

			co_yield "get_function_unloaded"_test = [](const honesty::test::Requirements& requirements)
			{
				cppaste::PluginLoader<TestPluginInterface> loader;

				auto create_fn = loader.get_function<TestPluginInterface::CreateFn>(
					TestPluginInterface::create_symbol
				);

				requirements.Assert(create_fn == nullptr);
			};

			co_yield "interface_type_alias"_test = [](const honesty::test::Requirements& requirements)
			{
				constexpr bool same_type = std::is_same_v<
					cppaste::PluginLoader<TestPluginInterface>::interface_type,
					TestPluginInterface
				>;
				requirements.Assert(same_type);
			};
		});

	honesty::test::SuiteRegistrar _3(PLUGIN_LOADER_SUITE);

	// ============================================================================
	// PluginRegistry Tests
	// ============================================================================

	honesty::test::Suite PLUGIN_REGISTRY_SUITE(
		"plugin_registry",
		[]() -> honesty::test::Generator
		{
			co_yield "default_construction"_test = [](const honesty::test::Requirements& requirements)
			{
				cppaste::PluginRegistry<TestPluginInterface> registry;

				requirements.Assert(registry.size() == 0);
				requirements.Expect(registry.plugin_ids().empty());
			};

			co_yield "load_nonexistent_plugin"_test = [](const honesty::test::Requirements& requirements)
			{
				cppaste::PluginRegistry<TestPluginInterface> registry;

				bool loaded = registry.load(
					std::filesystem::path("nonexistent_plugin.dll"),
					"test_plugin"
				);

				requirements.Assert(!loaded);
				requirements.Assert(registry.size() == 0);
				requirements.Expect(!registry.contains("test_plugin"));
			};

			co_yield "get_nonexistent_plugin"_test = [](const honesty::test::Requirements& requirements)
			{
				cppaste::PluginRegistry<TestPluginInterface> registry;

				auto* plugin = registry.get("nonexistent");

				requirements.Assert(plugin == nullptr);
			};

			co_yield "get_const_nonexistent_plugin"_test = [](const honesty::test::Requirements& requirements)
			{
				const cppaste::PluginRegistry<TestPluginInterface> registry;

				const auto* plugin = registry.get("nonexistent");

				requirements.Assert(plugin == nullptr);
			};

			co_yield "contains_nonexistent"_test = [](const honesty::test::Requirements& requirements)
			{
				cppaste::PluginRegistry<TestPluginInterface> registry;

				requirements.Assert(!registry.contains("nonexistent"));
			};

			co_yield "unload_nonexistent"_test = [](const honesty::test::Requirements& requirements)
			{
				cppaste::PluginRegistry<TestPluginInterface> registry;

				bool unloaded = registry.unload("nonexistent");

				requirements.Assert(!unloaded);
			};

			co_yield "unload_all_empty"_test = [](const honesty::test::Requirements& requirements)
			{
				cppaste::PluginRegistry<TestPluginInterface> registry;

				// Should not throw or crash
				registry.unload_all();

				requirements.Assert(registry.size() == 0);
			};

			co_yield "plugin_ids_empty"_test = [](const honesty::test::Requirements& requirements)
			{
				cppaste::PluginRegistry<TestPluginInterface> registry;

				auto ids = registry.plugin_ids();

				requirements.Assert(ids.empty());
			};

			co_yield "loader_type_alias"_test = [](const honesty::test::Requirements& requirements)
			{
				constexpr bool same_type = std::is_same_v<
					cppaste::PluginRegistry<TestPluginInterface>::loader_type,
					cppaste::PluginLoader<TestPluginInterface>
				>;
				requirements.Assert(same_type);
			};
		});

	honesty::test::SuiteRegistrar _4(PLUGIN_REGISTRY_SUITE);

	// ============================================================================
	// LibraryLoadResult Tests
	// ============================================================================

	honesty::test::Suite LIBRARY_LOAD_RESULT_SUITE(
		"library_load_result",
		[]() -> honesty::test::Generator
		{
			co_yield "success_result"_test = [](const honesty::test::Requirements& requirements)
			{
				void* dummy_handle = reinterpret_cast<void*>(0x12345678);
				cppaste::LibraryLoadResult result{dummy_handle, {}};

				requirements.Assert(static_cast<bool>(result));
				requirements.Assert(result.handle == dummy_handle);
				requirements.Expect(result.error.empty());
			};

			co_yield "failure_result"_test = [](const honesty::test::Requirements& requirements)
			{
				cppaste::LibraryLoadResult result{nullptr, "Failed to load library"};

				requirements.Assert(!static_cast<bool>(result));
				requirements.Assert(result.handle == nullptr);
				requirements.Expect(result.error == "Failed to load library");
			};

			co_yield "bool_conversion"_test = [](const honesty::test::Requirements& requirements)
			{
				cppaste::LibraryLoadResult success{reinterpret_cast<void*>(1), {}};
				cppaste::LibraryLoadResult failure{nullptr, "error"};

				if (success)
				{
					requirements.Assert(true);
				}
				else
				{
					requirements.Assert(false);
				}

				if (failure)
				{
					requirements.Assert(false);
				}
				else
				{
					requirements.Assert(true);
				}
			};
		});

	honesty::test::SuiteRegistrar _5(LIBRARY_LOAD_RESULT_SUITE);
}
