export module synodic.cppaste.composition:lifecycle;

import std;
import :interface;

export namespace cppaste
{
	/**
	 * @brief Lifecycle state enumeration
	 *
	 * Defines the states a service/plugin can be in during its lifetime.
	 * Follows a linear progression with explicit transition states.
	 */
	enum class LifecycleState
	{
		Uninstalled, // Not loaded into memory
		Installed,   // Loaded but dependencies not yet resolved
		Resolved,    // Dependencies satisfied, ready to start
		Starting,    // Transitioning to active state
		Active,      // Running normally
		Stopping     // Transitioning back to resolved state
	};

	/**
	 * @brief Lifecycle event callbacks
	 *
	 * Used for external observers to react to lifecycle transitions.
	 */
	template<typename T>
	struct LifecycleCallbacks
	{
		std::function<void(T&)> on_installed;  // Called after Install()
		std::function<void(T&)> on_resolved;   // Called after Resolve()
		std::function<void(T&)> on_starting;   // Called when Start() begins
		std::function<void(T&)> on_active;     // Called when fully active
		std::function<void(T&)> on_stopping;   // Called when Stop() begins
	};

	/**
	 * @brief Interface for lifecycle-aware services
	 *
	 * Provides standard lifecycle hooks that services can implement.
	 * This is the base interface for any service that needs managed
	 * installation, dependency resolution, and activation.
	 *
	 * For plugin-based architectures:
	 * - Install() → Load DLL into memory
	 * - Resolve() → Satisfy dependencies
	 * - Start() → Activate the plugin
	 * - Stop() → Deactivate (but keep loaded)
	 * - Uninstall() → Unload from memory
	 *
	 * TODO(reflection): Auto-generate C ABI shims from this interface
	 */
	struct ILifecycle : InterfaceBase<ILifecycle, void>
	{
		/**
		 * @brief Install the service (load into memory)
		 * @return True if installation succeeded
		 */
		virtual auto Install() -> bool = 0;

		/**
		 * @brief Resolve dependencies
		 * @return True if all dependencies are satisfied
		 */
		virtual auto Resolve() -> bool = 0;

		/**
		 * @brief Start the service (transition to active)
		 * @return True if start succeeded
		 */
		virtual auto Start() -> bool = 0;

		/**
		 * @brief Stop the service (transition back to resolved)
		 */
		virtual void Stop() = 0;

		/**
		 * @brief Uninstall the service (unload from memory)
		 */
		virtual void Uninstall() = 0;

		/**
		 * @brief Get the current lifecycle state
		 */
		virtual auto GetState() const -> LifecycleState = 0;
	};

	/**
	 * @brief Mixin providing lifecycle state management
	 *
	 * Derived classes implement the actual lifecycle logic, this mixin
	 * provides state tracking and transition validation.
	 */
	class LifecycleMixin
	{
		LifecycleState state_ = LifecycleState::Uninstalled;

	protected:
		constexpr LifecycleMixin() = default;

		/**
		 * @brief Set the current lifecycle state
		 */
		void SetState(this auto&& self, LifecycleState state) { self.state_ = state; }

		/**
		 * @brief Check if transition to a state is valid
		 */
		bool CanTransitionTo(this auto const& self, LifecycleState target)
		{
			switch (target)
			{
			case LifecycleState::Uninstalled:
				// Can uninstall from Installed or Resolved
				return self.state_ == LifecycleState::Installed ||
				       self.state_ == LifecycleState::Resolved;
			case LifecycleState::Installed:
				return self.state_ == LifecycleState::Uninstalled;
			case LifecycleState::Resolved:
				return self.state_ == LifecycleState::Installed ||
				       self.state_ == LifecycleState::Stopping;
			case LifecycleState::Starting:
				return self.state_ == LifecycleState::Resolved;
			case LifecycleState::Active:
				return self.state_ == LifecycleState::Starting;
			case LifecycleState::Stopping:
				return self.state_ == LifecycleState::Active;
			default:
				return false;
			}
		}

		/**
		 * @brief Direct state accessor for derived class overrides
		 */
		LifecycleState GetStateImpl() const { return state_; }

	public:
		LifecycleState GetState(this auto const& self) { return self.state_; }

		bool IsInstalled(this auto const& self)
		{
			return self.state_ != LifecycleState::Uninstalled;
		}

		bool IsResolved(this auto const& self)
		{
			return self.state_ == LifecycleState::Resolved ||
			       self.state_ == LifecycleState::Starting ||
			       self.state_ == LifecycleState::Active ||
			       self.state_ == LifecycleState::Stopping;
		}

		bool IsActive(this auto const& self) { return self.state_ == LifecycleState::Active; }

		bool IsTransitioning(this auto const& self)
		{
			return self.state_ == LifecycleState::Starting ||
			       self.state_ == LifecycleState::Stopping;
		}
	};

	/**
	 * @brief Default lifecycle implementation with no-op methods
	 *
	 * Provides a default implementation that can be used as a base
	 * for services that only need to override specific lifecycle methods.
	 */
	class DefaultLifecycle : public ILifecycle, public LifecycleMixin
	{
	public:
		auto Install() -> bool override
		{
			if (!CanTransitionTo(LifecycleState::Installed))
				return false;
			SetState(LifecycleState::Installed);
			return true;
		}

		auto Resolve() -> bool override
		{
			if (!CanTransitionTo(LifecycleState::Resolved))
				return false;
			SetState(LifecycleState::Resolved);
			return true;
		}

		auto Start() -> bool override
		{
			if (!CanTransitionTo(LifecycleState::Starting))
				return false;
			SetState(LifecycleState::Starting);
			SetState(LifecycleState::Active);
			return true;
		}

		void Stop() override
		{
			if (!CanTransitionTo(LifecycleState::Stopping))
				return;
			SetState(LifecycleState::Stopping);
			SetState(LifecycleState::Resolved);
		}

		void Uninstall() override
		{
			// Must stop first if active
			if (GetState() == LifecycleState::Active)
			{
				Stop();
			}
			if (!CanTransitionTo(LifecycleState::Uninstalled))
				return;
			SetState(LifecycleState::Uninstalled);
		}

		auto GetState() const -> LifecycleState override
		{
			return GetStateImpl();
		}
	};

	/**
	 * @brief Service wrapper that adds lifecycle management
	 *
	 * Wraps any service implementation and manages its lifecycle state.
	 * The wrapped service can optionally implement lifecycle methods.
	 *
	 * @tparam T The service implementation type
	 */
	template<typename T>
	class LifecycleService : public ILifecycle, public LifecycleMixin
	{
		T impl_;
		LifecycleCallbacks<T> callbacks_;

	public:
		template<typename... Args>
		explicit LifecycleService(Args&&... args)
			: impl_(std::forward<Args>(args)...)
		{}

		/**
		 * @brief Set lifecycle event callbacks
		 */
		void SetCallbacks(LifecycleCallbacks<T> callbacks)
		{
			callbacks_ = std::move(callbacks);
		}

		/**
		 * @brief Access the underlying implementation
		 */
		T& get() { return impl_; }
		const T& get() const { return impl_; }

		// ILifecycle implementation

		auto Install() -> bool override
		{
			if (!CanTransitionTo(LifecycleState::Installed))
				return false;

			bool success = true;
			if constexpr (requires { impl_.Install(); })
			{
				success = impl_.Install();
			}

			if (success)
			{
				SetState(LifecycleState::Installed);
				if (callbacks_.on_installed)
					callbacks_.on_installed(impl_);
			}

			return success;
		}

		auto Resolve() -> bool override
		{
			if (!CanTransitionTo(LifecycleState::Resolved))
				return false;

			bool success = true;
			if constexpr (requires { impl_.Resolve(); })
			{
				success = impl_.Resolve();
			}

			if (success)
			{
				SetState(LifecycleState::Resolved);
				if (callbacks_.on_resolved)
					callbacks_.on_resolved(impl_);
			}

			return success;
		}

		auto Start() -> bool override
		{
			if (!CanTransitionTo(LifecycleState::Starting))
				return false;

			SetState(LifecycleState::Starting);
			if (callbacks_.on_starting)
				callbacks_.on_starting(impl_);

			bool success = true;
			if constexpr (requires { impl_.Start(); })
			{
				success = impl_.Start();
			}

			if (success)
			{
				SetState(LifecycleState::Active);
				if (callbacks_.on_active)
					callbacks_.on_active(impl_);
			}
			else
			{
				// Rollback to resolved on failure
				SetState(LifecycleState::Resolved);
			}

			return success;
		}

		void Stop() override
		{
			if (!CanTransitionTo(LifecycleState::Stopping))
				return;

			SetState(LifecycleState::Stopping);
			if (callbacks_.on_stopping)
				callbacks_.on_stopping(impl_);

			if constexpr (requires { impl_.Stop(); })
			{
				impl_.Stop();
			}

			SetState(LifecycleState::Resolved);
		}

		void Uninstall() override
		{
			// Must stop first if active
			if (GetStateImpl() == LifecycleState::Active)
			{
				Stop();
			}

			if (!CanTransitionTo(LifecycleState::Uninstalled))
				return;

			if constexpr (requires { impl_.Uninstall(); })
			{
				impl_.Uninstall();
			}

			SetState(LifecycleState::Uninstalled);
		}

		auto GetState() const -> LifecycleState override
		{
			return GetStateImpl();
		}
	};
}
