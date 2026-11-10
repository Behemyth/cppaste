module;

// System headers belong in the global module fragment, not in the module's purview.
#ifndef _WIN32
#include <dlfcn.h>
#endif

export module synodic.cppaste.binding:plugin;

import std;

export namespace cppaste
{
	/**
	 * @brief Platform-specific library handle type
	 */
#ifdef _WIN32
	using LibraryHandle = void*;  // HMODULE
#else
	using LibraryHandle = void*;  // dlopen handle
#endif

	/**
	 * @brief Result of loading a library
	 */
	struct LibraryLoadResult
	{
		LibraryHandle handle;
		std::string error;

		explicit operator bool() const { return handle != nullptr; }
	};

	/**
	 * @brief Platform-agnostic dynamic library loader
	 *
	 * Wraps LoadLibrary/dlopen for cross-platform plugin loading.
	 * Used by PluginLoader to manage plugin lifecycle.
	 */
	class DynamicLibrary
	{
		LibraryHandle handle_ = nullptr;
		std::filesystem::path path_;

	public:
		DynamicLibrary() = default;

		explicit DynamicLibrary(std::filesystem::path path)
			: path_(std::move(path))
		{}

		DynamicLibrary(const DynamicLibrary&) = delete;
		DynamicLibrary& operator=(const DynamicLibrary&) = delete;

		DynamicLibrary(DynamicLibrary&& other) noexcept
			: handle_(std::exchange(other.handle_, nullptr))
			, path_(std::move(other.path_))
		{}

		DynamicLibrary& operator=(DynamicLibrary&& other) noexcept
		{
			if (this != &other)
			{
				unload();
				handle_ = std::exchange(other.handle_, nullptr);
				path_ = std::move(other.path_);
			}
			return *this;
		}

		~DynamicLibrary()
		{
			unload();
		}

		/**
		 * @brief Load the dynamic library
		 * @return Result containing handle or error message
		 */
		auto load() -> LibraryLoadResult;

		/**
		 * @brief Unload the dynamic library
		 */
		void unload();

		/**
		 * @brief Check if library is loaded
		 */
		bool is_loaded() const { return handle_ != nullptr; }

		/**
		 * @brief Get a symbol from the loaded library
		 * @tparam T Function pointer type
		 * @param name Symbol name
		 * @return Function pointer or nullptr if not found
		 */
		template<typename T>
		auto get_symbol(std::string_view name) const -> T
		{
			return reinterpret_cast<T>(get_symbol_impl(name));
		}

		/**
		 * @brief Get the library file path
		 */
		const std::filesystem::path& path() const { return path_; }

		/**
		 * @brief Get the native handle
		 */
		LibraryHandle native_handle() const { return handle_; }

	private:
		auto get_symbol_impl(std::string_view name) const -> void*;
	};

	/**
	 * @brief CRTP base for defining plugin interfaces
	 *
	 * Provides type-safe C ABI contract definition. Derived types define
	 * the function pointer types and symbol names that plugins must export.
	 *
	 * Example:
	 * @code
	 * struct GamePlugin : PluginInterface<GamePlugin>
	 * {
	 *     // Function pointer types matching C ABI exports
	 *     using CreateFn = void* (*)(void* context);
	 *     using DestroyFn = void (*)(void* instance);
	 *     using UpdateFn = void (*)(void* instance, float delta);
	 *
	 *     // Symbol names to look up
	 *     static constexpr std::string_view create_symbol = "game_create";
	 *     static constexpr std::string_view destroy_symbol = "game_destroy";
	 *     static constexpr std::string_view update_symbol = "game_update";
	 * };
	 * @endcode
	 *
	 * @tparam Derived The concrete plugin interface type (CRTP)
	 */
	template<typename Derived>
	struct PluginInterface
	{
		using interface_type = Derived;
	};

	/**
	 * @brief Manages loading and lifecycle of a plugin
	 *
	 * PluginLoader wraps a DynamicLibrary and provides type-safe access
	 * to plugin functions defined by a PluginInterface.
	 *
	 * @tparam Interface A type derived from PluginInterface
	 */
	template<typename Interface>
	class PluginLoader
	{
		static_assert(std::is_base_of_v<PluginInterface<Interface>, Interface>,
			"Interface must derive from PluginInterface<Interface>");

		DynamicLibrary library_;
		bool symbols_resolved_ = false;

	public:
		using interface_type = Interface;

		PluginLoader() = default;

		explicit PluginLoader(std::filesystem::path plugin_path)
			: library_(std::move(plugin_path))
		{}

		/**
		 * @brief Load the plugin library
		 * @return True if load succeeded
		 */
		auto load() -> bool
		{
			auto result = library_.load();
			if (!result)
			{
				last_error_ = std::move(result.error);
				return false;
			}
			symbols_resolved_ = false;
			return true;
		}

		/**
		 * @brief Unload the plugin library
		 */
		void unload()
		{
			library_.unload();
			symbols_resolved_ = false;
		}

		/**
		 * @brief Check if plugin is loaded
		 */
		bool is_loaded() const { return library_.is_loaded(); }

		/**
		 * @brief Get a function from the plugin
		 * @tparam FnPtr Function pointer type
		 * @param symbol_name Name of the exported symbol
		 * @return Function pointer or nullptr
		 */
		template<typename FnPtr>
		auto get_function(std::string_view symbol_name) const -> FnPtr
		{
			return library_.get_symbol<FnPtr>(symbol_name);
		}

		/**
		 * @brief Get the last error message
		 */
		const std::string& last_error() const { return last_error_; }

		/**
		 * @brief Get the plugin path
		 */
		const std::filesystem::path& path() const { return library_.path(); }

	private:
		std::string last_error_;
	};

	/**
	 * @brief Plugin registry for managing multiple loaded plugins
	 *
	 * Provides centralized management of plugin instances, useful for
	 * editors that need to load/unload multiple game plugins.
	 *
	 * @tparam Interface The plugin interface type
	 */
	template<typename Interface>
	class PluginRegistry
	{
	public:
		using loader_type = PluginLoader<Interface>;

		/**
		 * @brief Load a plugin from path
		 * @param path Path to the plugin library
		 * @param id Unique identifier for this plugin instance
		 * @return True if loaded successfully
		 */
		auto load(const std::filesystem::path& path, std::string id) -> bool
		{
			auto loader = std::make_unique<loader_type>(path);
			if (!loader->load())
			{
				return false;
			}
			plugins_.emplace(std::move(id), std::move(loader));
			return true;
		}

		/**
		 * @brief Unload a plugin by ID
		 * @param id Plugin identifier
		 * @return True if plugin was found and unloaded
		 */
		auto unload(const std::string& id) -> bool
		{
			auto it = plugins_.find(id);
			if (it == plugins_.end())
			{
				return false;
			}
			it->second->unload();
			plugins_.erase(it);
			return true;
		}

		/**
		 * @brief Unload all plugins
		 */
		void unload_all()
		{
			for (auto& [id, loader] : plugins_)
			{
				loader->unload();
			}
			plugins_.clear();
		}

		/**
		 * @brief Get a loaded plugin by ID
		 * @param id Plugin identifier
		 * @return Pointer to loader or nullptr if not found
		 */
		auto get(const std::string& id) -> loader_type*
		{
			auto it = plugins_.find(id);
			return it != plugins_.end() ? it->second.get() : nullptr;
		}

		/**
		 * @brief Get a loaded plugin by ID (const)
		 */
		auto get(const std::string& id) const -> const loader_type*
		{
			auto it = plugins_.find(id);
			return it != plugins_.end() ? it->second.get() : nullptr;
		}

		/**
		 * @brief Check if a plugin is loaded
		 */
		bool contains(const std::string& id) const
		{
			return plugins_.contains(id);
		}

		/**
		 * @brief Get all loaded plugin IDs
		 */
		auto plugin_ids() const -> std::vector<std::string>
		{
			std::vector<std::string> ids;
			ids.reserve(plugins_.size());
			for (const auto& [id, _] : plugins_)
			{
				ids.push_back(id);
			}
			return ids;
		}

		/**
		 * @brief Number of loaded plugins
		 */
		std::size_t size() const { return plugins_.size(); }

	private:
		std::unordered_map<std::string, std::unique_ptr<loader_type>> plugins_;
	};
}

// Platform-specific implementations
// Included directly in the partition since module :private is not allowed in partitions

#ifdef _WIN32

namespace cppaste
{
	// Forward declare Windows API functions to avoid including windows.h
	extern "C" {
		__declspec(dllimport) void* __stdcall LoadLibraryW(const wchar_t* lpLibFileName);
		__declspec(dllimport) int __stdcall FreeLibrary(void* hLibModule);
		__declspec(dllimport) void* __stdcall GetProcAddress(void* hModule, const char* lpProcName);
		__declspec(dllimport) unsigned long __stdcall GetLastError();
		__declspec(dllimport) unsigned long __stdcall FormatMessageW(
			unsigned long dwFlags,
			const void* lpSource,
			unsigned long dwMessageId,
			unsigned long dwLanguageId,
			wchar_t* lpBuffer,
			unsigned long nSize,
			void* Arguments
		);
		__declspec(dllimport) void* __stdcall LocalFree(void* hMem);
	}

	namespace
	{
		constexpr unsigned long FORMAT_MESSAGE_ALLOCATE_BUFFER = 0x00000100;
		constexpr unsigned long FORMAT_MESSAGE_FROM_SYSTEM = 0x00001000;
		constexpr unsigned long FORMAT_MESSAGE_IGNORE_INSERTS = 0x00000200;

		auto get_last_error_message() -> std::string
		{
			unsigned long error_code = GetLastError();
			if (error_code == 0)
			{
				return {};
			}

			wchar_t* buffer = nullptr;
			unsigned long size = FormatMessageW(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				nullptr,
				error_code,
				0,  // Default language
				reinterpret_cast<wchar_t*>(&buffer),
				0,
				nullptr
			);

			std::string message;
			if (size > 0 && buffer)
			{
				// Convert wide string to narrow string
				int narrow_size = static_cast<int>(size);
				message.resize(narrow_size);
				for (int i = 0; i < narrow_size; ++i)
				{
					message[i] = static_cast<char>(buffer[i]);
				}
				// Trim trailing whitespace
				while (!message.empty() && (message.back() == '\n' || message.back() == '\r'))
				{
					message.pop_back();
				}
				LocalFree(buffer);
			}
			return message;
		}
	}

	auto DynamicLibrary::load() -> LibraryLoadResult
	{
		if (handle_)
		{
			return {handle_, {}};
		}

		handle_ = LoadLibraryW(path_.c_str());
		if (!handle_)
		{
			return {nullptr, get_last_error_message()};
		}
		return {handle_, {}};
	}

	void DynamicLibrary::unload()
	{
		if (handle_)
		{
			FreeLibrary(handle_);
			handle_ = nullptr;
		}
	}

	auto DynamicLibrary::get_symbol_impl(std::string_view name) const -> void*
	{
		if (!handle_)
		{
			return nullptr;
		}
		// Create null-terminated string
		std::string name_str(name);
		return GetProcAddress(handle_, name_str.c_str());
	}
}

#else  // POSIX (Linux, macOS)

namespace cppaste
{
	auto DynamicLibrary::load() -> LibraryLoadResult
	{
		if (handle_)
		{
			return {handle_, {}};
		}

		handle_ = dlopen(path_.c_str(), RTLD_NOW | RTLD_LOCAL);
		if (!handle_)
		{
			const char* error = dlerror();
			return {nullptr, error ? error : "Unknown error"};
		}
		return {handle_, {}};
	}

	void DynamicLibrary::unload()
	{
		if (handle_)
		{
			dlclose(handle_);
			handle_ = nullptr;
		}
	}

	auto DynamicLibrary::get_symbol_impl(std::string_view name) const -> void*
	{
		if (!handle_)
		{
			return nullptr;
		}
		std::string name_str(name);
		return dlsym(handle_, name_str.c_str());
	}
}

#endif
