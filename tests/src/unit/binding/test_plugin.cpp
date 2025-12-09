#include <gtest/gtest.h>

import std;
import synodic.cppaste.binding;

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

	TEST(DynamicLibrary, DefaultConstruction)
	{
		cppaste::DynamicLibrary library;
		ASSERT_FALSE(library.is_loaded());
		EXPECT_EQ(library.native_handle(), nullptr);
	}

	TEST(DynamicLibrary, PathConstruction)
	{
		std::filesystem::path test_path = "nonexistent_library.dll";
		cppaste::DynamicLibrary library(test_path);

		ASSERT_FALSE(library.is_loaded());
		EXPECT_EQ(library.path(), test_path);
	}

	TEST(DynamicLibrary, LoadNonexistentLibrary)
	{
		cppaste::DynamicLibrary library(std::filesystem::path("nonexistent_library.dll"));
		auto result = library.load();

		ASSERT_FALSE(result);
		ASSERT_FALSE(library.is_loaded());
		EXPECT_FALSE(result.error.empty());
	}

	TEST(DynamicLibrary, MoveConstruction)
	{
		std::filesystem::path test_path = "test_library.dll";
		cppaste::DynamicLibrary library1(test_path);
		cppaste::DynamicLibrary library2(std::move(library1));

		EXPECT_EQ(library2.path(), test_path);
		EXPECT_TRUE(library1.path().empty());
	}

	TEST(DynamicLibrary, MoveAssignment)
	{
		std::filesystem::path path1 = "library1.dll";
		std::filesystem::path path2 = "library2.dll";

		cppaste::DynamicLibrary library1(path1);
		cppaste::DynamicLibrary library2(path2);

		library2 = std::move(library1);

		EXPECT_EQ(library2.path(), path1);
		EXPECT_TRUE(library1.path().empty());
	}

	TEST(DynamicLibrary, UnloadUnloadedLibrary)
	{
		cppaste::DynamicLibrary library;

		// Should not throw or crash
		library.unload();

		ASSERT_FALSE(library.is_loaded());
	}

	TEST(DynamicLibrary, GetSymbolUnloaded)
	{
		cppaste::DynamicLibrary library;

		auto fn = library.get_symbol<void(*)()>("test_symbol");

		ASSERT_EQ(fn, nullptr);
	}

	// ============================================================================
	// PluginInterface Tests
	// ============================================================================

	TEST(PluginInterface, InterfaceTypeAlias)
	{
		// Verify that interface_type is correctly defined
		constexpr bool same_type = std::is_same_v<
			TestPluginInterface::interface_type,
			TestPluginInterface
		>;
		ASSERT_TRUE(same_type);
	}

	TEST(PluginInterface, SymbolNamesDefined)
	{
		EXPECT_EQ(TestPluginInterface::create_symbol, "test_create");
		EXPECT_EQ(TestPluginInterface::destroy_symbol, "test_destroy");
		EXPECT_EQ(TestPluginInterface::process_symbol, "test_process");
	}

	TEST(PluginInterface, FunctionPointerTypes)
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

		ASSERT_TRUE(create_is_fn_ptr);
		ASSERT_TRUE(destroy_is_fn_ptr);
		ASSERT_TRUE(process_is_fn_ptr);
	}

	// ============================================================================
	// PluginLoader Tests
	// ============================================================================

	TEST(PluginLoader, DefaultConstruction)
	{
		cppaste::PluginLoader<TestPluginInterface> loader;

		ASSERT_FALSE(loader.is_loaded());
		EXPECT_TRUE(loader.path().empty());
	}

	TEST(PluginLoader, PathConstruction)
	{
		std::filesystem::path plugin_path = "test_plugin.dll";
		cppaste::PluginLoader<TestPluginInterface> loader(plugin_path);

		ASSERT_FALSE(loader.is_loaded());
		EXPECT_EQ(loader.path(), plugin_path);
	}

	TEST(PluginLoader, LoadNonexistentPlugin)
	{
		cppaste::PluginLoader<TestPluginInterface> loader(
			std::filesystem::path("nonexistent_plugin.dll")
		);

		bool loaded = loader.load();

		ASSERT_FALSE(loaded);
		ASSERT_FALSE(loader.is_loaded());
		EXPECT_FALSE(loader.last_error().empty());
	}

	TEST(PluginLoader, UnloadUnloadedPlugin)
	{
		cppaste::PluginLoader<TestPluginInterface> loader;

		// Should not throw or crash
		loader.unload();

		ASSERT_FALSE(loader.is_loaded());
	}

	TEST(PluginLoader, GetFunctionUnloaded)
	{
		cppaste::PluginLoader<TestPluginInterface> loader;

		auto create_fn = loader.get_function<TestPluginInterface::CreateFn>(
			TestPluginInterface::create_symbol
		);

		ASSERT_EQ(create_fn, nullptr);
	}

	TEST(PluginLoader, InterfaceTypeAlias)
	{
		constexpr bool same_type = std::is_same_v<
			cppaste::PluginLoader<TestPluginInterface>::interface_type,
			TestPluginInterface
		>;
		ASSERT_TRUE(same_type);
	}

	// ============================================================================
	// PluginRegistry Tests
	// ============================================================================

	TEST(PluginRegistry, DefaultConstruction)
	{
		cppaste::PluginRegistry<TestPluginInterface> registry;

		ASSERT_EQ(registry.size(), 0);
		EXPECT_TRUE(registry.plugin_ids().empty());
	}

	TEST(PluginRegistry, LoadNonexistentPlugin)
	{
		cppaste::PluginRegistry<TestPluginInterface> registry;

		bool loaded = registry.load(
			std::filesystem::path("nonexistent_plugin.dll"),
			"test_plugin"
		);

		ASSERT_FALSE(loaded);
		ASSERT_EQ(registry.size(), 0);
		EXPECT_FALSE(registry.contains("test_plugin"));
	}

	TEST(PluginRegistry, GetNonexistentPlugin)
	{
		cppaste::PluginRegistry<TestPluginInterface> registry;

		auto* plugin = registry.get("nonexistent");

		ASSERT_EQ(plugin, nullptr);
	}

	TEST(PluginRegistry, GetConstNonexistentPlugin)
	{
		const cppaste::PluginRegistry<TestPluginInterface> registry;

		const auto* plugin = registry.get("nonexistent");

		ASSERT_EQ(plugin, nullptr);
	}

	TEST(PluginRegistry, ContainsNonexistent)
	{
		cppaste::PluginRegistry<TestPluginInterface> registry;

		ASSERT_FALSE(registry.contains("nonexistent"));
	}

	TEST(PluginRegistry, UnloadNonexistent)
	{
		cppaste::PluginRegistry<TestPluginInterface> registry;

		bool unloaded = registry.unload("nonexistent");

		ASSERT_FALSE(unloaded);
	}

	TEST(PluginRegistry, UnloadAllEmpty)
	{
		cppaste::PluginRegistry<TestPluginInterface> registry;

		// Should not throw or crash
		registry.unload_all();

		ASSERT_EQ(registry.size(), 0);
	}

	TEST(PluginRegistry, PluginIdsEmpty)
	{
		cppaste::PluginRegistry<TestPluginInterface> registry;

		auto ids = registry.plugin_ids();

		ASSERT_TRUE(ids.empty());
	}

	TEST(PluginRegistry, LoaderTypeAlias)
	{
		constexpr bool same_type = std::is_same_v<
			cppaste::PluginRegistry<TestPluginInterface>::loader_type,
			cppaste::PluginLoader<TestPluginInterface>
		>;
		ASSERT_TRUE(same_type);
	}

	// ============================================================================
	// LibraryLoadResult Tests
	// ============================================================================

	TEST(LibraryLoadResult, SuccessResult)
	{
		void* dummy_handle = reinterpret_cast<void*>(0x12345678);
		cppaste::LibraryLoadResult result{dummy_handle, {}};

		ASSERT_TRUE(static_cast<bool>(result));
		ASSERT_EQ(result.handle, dummy_handle);
		EXPECT_TRUE(result.error.empty());
	}

	TEST(LibraryLoadResult, FailureResult)
	{
		cppaste::LibraryLoadResult result{nullptr, "Failed to load library"};

		ASSERT_FALSE(static_cast<bool>(result));
		ASSERT_EQ(result.handle, nullptr);
		EXPECT_EQ(result.error, "Failed to load library");
	}

	TEST(LibraryLoadResult, BoolConversion)
	{
		cppaste::LibraryLoadResult success{reinterpret_cast<void*>(1), {}};
		cppaste::LibraryLoadResult failure{nullptr, "error"};

		if (success)
		{
			ASSERT_TRUE(true);
		}
		else
		{
			ASSERT_TRUE(false);
		}

		if (failure)
		{
			ASSERT_TRUE(false);
		}
		else
		{
			ASSERT_TRUE(true);
		}
	}

}

