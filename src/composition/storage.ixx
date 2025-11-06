export module synodic.cppaste.composition:storage;

import std;

export namespace cppaste
{
	/**
	 * @brief Base for type-erased storage
	 *
	 * Provides shared infrastructure for all DynamicStorage specializations.
	 * Each interface needs its own vtable (Concept/Model), but they all share
	 * the same basic structure: a unique_ptr to a polymorphic base.
	 *
	 * TODO(reflection): With C++26 reflection, vtable generation will be automated,
	 * eliminating the need for manual Concept/Model specializations.
	 */
	class TypeErasureBase
	{
	public:
		/**
		 * @brief Abstract base for type-erased storage vtables
		 *
		 * Each DynamicStorage<Interface> specialization creates a derived Concept
		 * with virtual methods matching the interface.
		 */
		struct Concept
		{
			constexpr virtual ~Concept() = default;
			constexpr virtual std::unique_ptr<Concept> clone() const = 0;
		};

		// The type-erased polymorphic object
		std::unique_ptr<Concept> pimpl_;

	public:
		constexpr TypeErasureBase() = default;

		constexpr TypeErasureBase(const TypeErasureBase& other)
			: pimpl_(other.pimpl_ ? other.pimpl_->clone() : nullptr) {}

		constexpr TypeErasureBase(TypeErasureBase&&) noexcept = default;

		constexpr TypeErasureBase& operator=(const TypeErasureBase& other)
		{
			pimpl_ = other.pimpl_ ? other.pimpl_->clone() : nullptr;
			return *this;
		}

		constexpr TypeErasureBase& operator=(TypeErasureBase&&) noexcept = default;

		constexpr bool empty() const { return !pimpl_; }
	};

	/**
	 * @brief Static storage policy
	 *
	 * Stores the implementation directly by value for zero-cost abstraction.
	 * Interface type is provided by the Service template context.
	 */
	template<typename T>
	class StaticStorage
	{
		T data_;

	public:
		using value_type = T;

		constexpr StaticStorage(T data) : data_(std::move(data)) {}

		/**
		 * @brief Access the underlying implementation with perfect forwarding
		 *
		 * Uses deducing this to automatically handle const/non-const and
		 * value category. The returned reference matches the storage's state.
		 */
		template<typename Self>
		constexpr auto get(this Self&& self) -> decltype(auto)
		{
			return std::forward<Self>(self).data_;
		}

		// Non-forwarding accessors for explicit lvalue/const access
		constexpr auto get_ref() const -> const T& { return data_; }
		constexpr auto get_ref() -> T& { return data_; }
	};

	/**
	 * @brief Dynamic storage policy - wraps StaticStorage with type erasure
	 *
	 * DynamicStorage is a transparent layer on top of StaticStorage that adds runtime
	 * polymorphism through type erasure. It internally holds a StaticStorage<T> within
	 * the type-erased wrapper, making static storage the foundation for all service types.
	 *
	 * TODO(reflection): Generate Concept/Model specializations automatically from
	 * [[service]] interface definitions, eliminating manual vtable boilerplate.
	 */
	template<typename Interface>
	class DynamicStorage
	{
	public:
		using interface_type = Interface;

		/**
		 * @brief Abstract vtable for this interface
		 *
		 * TODO(reflection): Auto-generate from interface methods
		 * Each method should forward to the underlying StaticStorage<T>::get()
		 */
		struct Concept : TypeErasureBase::Concept
		{
			// Pure virtual methods matching the interface
			// Specializations must define these
		};

		/**
		 * @brief Concrete vtable implementation wrapping StaticStorage<T>
		 */
		template<typename T>
		struct Model : Concept
		{
			StaticStorage<T> storage_;

			constexpr explicit Model(T data) : storage_(std::move(data)) {}

			constexpr std::unique_ptr<TypeErasureBase::Concept> clone() const override
			{
				return std::make_unique<Model<T>>(storage_.get());
			}
		};

	private:
		TypeErasureBase base_;

	public:
		constexpr DynamicStorage() = default;

		/**
		 * @brief Construct with implementation, wrapping in StaticStorage
		 */
		template<typename T>
		constexpr DynamicStorage(T obj)
		{
			base_.pimpl_ = std::make_unique<Model<T>>(std::move(obj));
		}

		constexpr DynamicStorage(const DynamicStorage& other) : base_(other.base_) {}
		constexpr DynamicStorage(DynamicStorage&&) noexcept = default;
		constexpr DynamicStorage& operator=(const DynamicStorage& other) { base_ = other.base_; return *this; }
		constexpr DynamicStorage& operator=(DynamicStorage&&) noexcept = default;

		constexpr bool empty() const { return base_.empty(); }

		/**
		 * @brief Get the vtable for virtual dispatch
		 *
		 * Service specializations for dynamic interfaces call this to get the Concept
		 * pointer, then invoke virtual methods on it.
		 *
		 * TODO(reflection): Generate Service specializations that bridge from pure
		 * virtual interface methods to get_concept()->method() calls.
		 */
		template<typename Self>
		constexpr auto get_concept(this Self&& self)
		{
			if (self.base_.empty())
				throw std::runtime_error("Attempting to access empty dynamic storage");

			using ReturnType = std::conditional_t<
				std::is_const_v<std::remove_reference_t<Self>>,
				const Concept*,
				Concept*
			>;
			return static_cast<ReturnType>(self.base_.pimpl_.get());
		}
	};

	/**
	 * @brief Convenience aliases for storage policies
	 *
	 * Provides a clean way to reference storage policies through a single
	 * StoragePolicy<Interface> template, making it easier to write generic
	 * code that works with different storage strategies.
	 */
	template<typename Interface>
	struct StoragePolicy
	{
		// For static polymorphism - Interface is implicit from Service context
		template<typename T>
		using Static = StaticStorage<T>;

		// For dynamic polymorphism
		using Dynamic = DynamicStorage<Interface>;
	};

}
