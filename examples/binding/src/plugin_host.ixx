export module example.binding:plugin_host;

import std;
import synodic.cppaste.binding;
import :plugin_contract;

export namespace example::binding
{
	/**
	 * @brief High-level plugin host
	 *
	 * Manages loading a plugin, creating instances, and cleanup.
	 * Wraps the raw C ABI with a convenient C++ interface.
	 */
	class PluginHost
	{
		cppaste::PluginLoader<PluginContract> loader_;
		void* instance_ = nullptr;

		// Cached function pointers
		plugin_abi::CreateFn create_fn_ = nullptr;
		plugin_abi::DestroyFn destroy_fn_ = nullptr;
		plugin_abi::GetInfoFn get_info_fn_ = nullptr;

	public:
		PluginHost() = default;

		explicit PluginHost(std::filesystem::path plugin_path)
			: loader_(std::move(plugin_path))
		{}

		~PluginHost()
		{
			Unload();
		}

		PluginHost(const PluginHost&) = delete;
		PluginHost& operator=(const PluginHost&) = delete;

		PluginHost(PluginHost&& other) noexcept
			: loader_(std::move(other.loader_))
			, instance_(std::exchange(other.instance_, nullptr))
			, create_fn_(std::exchange(other.create_fn_, nullptr))
			, destroy_fn_(std::exchange(other.destroy_fn_, nullptr))
			, get_info_fn_(std::exchange(other.get_info_fn_, nullptr))
		{}

		PluginHost& operator=(PluginHost&& other) noexcept
		{
			if (this != &other)
			{
				Unload();
				loader_ = std::move(other.loader_);
				instance_ = std::exchange(other.instance_, nullptr);
				create_fn_ = std::exchange(other.create_fn_, nullptr);
				destroy_fn_ = std::exchange(other.destroy_fn_, nullptr);
				get_info_fn_ = std::exchange(other.get_info_fn_, nullptr);
			}
			return *this;
		}

		/**
		 * @brief Load the plugin library and resolve symbols
		 * @return true if load succeeded and required symbols were found
		 */
		auto Load() -> bool
		{
			if (!loader_.load())
			{
				return false;
			}

			// Resolve required functions
			create_fn_ = loader_.get_function<plugin_abi::CreateFn>(plugin_abi::symbols::create);
			destroy_fn_ = loader_.get_function<plugin_abi::DestroyFn>(plugin_abi::symbols::destroy);

			if (!create_fn_ || !destroy_fn_)
			{
				loader_.unload();
				return false;
			}

			// Resolve optional functions
			get_info_fn_ = loader_.get_function<plugin_abi::GetInfoFn>(plugin_abi::symbols::get_info);

			return true;
		}

		/**
		 * @brief Unload the plugin and destroy any active instance
		 */
		void Unload()
		{
			Destroy();
			loader_.unload();
			create_fn_ = nullptr;
			destroy_fn_ = nullptr;
			get_info_fn_ = nullptr;
		}

		/**
		 * @brief Check if plugin is loaded
		 */
		bool IsLoaded() const { return loader_.is_loaded(); }

		/**
		 * @brief Create a plugin instance
		 * @param context Context pointer to pass to the plugin (may be nullptr)
		 * @return true if creation succeeded
		 */
		auto Create(void* context = nullptr) -> bool
		{
			if (!create_fn_ || instance_)
			{
				return false;
			}
			instance_ = create_fn_(context);
			return instance_ != nullptr;
		}

		/**
		 * @brief Destroy the current plugin instance
		 */
		void Destroy()
		{
			if (instance_ && destroy_fn_)
			{
				destroy_fn_(instance_);
				instance_ = nullptr;
			}
		}

		/**
		 * @brief Get plugin information
		 * @return Plugin info if available
		 */
		auto GetInfo() const -> std::optional<PluginInfo>
		{
			if (get_info_fn_)
			{
				return get_info_fn_();
			}
			return std::nullopt;
		}

		/**
		 * @brief Check if a plugin instance exists
		 */
		bool HasInstance() const { return instance_ != nullptr; }

		/**
		 * @brief Get the raw instance pointer
		 *
		 * Use this to pass the instance to plugin-specific functions
		 * that the host knows about.
		 */
		void* GetInstance() const { return instance_; }

		/**
		 * @brief Get the plugin path
		 */
		const std::filesystem::path& GetPath() const { return loader_.path(); }

		/**
		 * @brief Get last error message
		 */
		const std::string& GetLastError() const { return loader_.last_error(); }
	};
}
