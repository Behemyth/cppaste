export module synodic.cppaste.injection:scope;

import std;

export namespace cppaste
{

	enum class ScopeType
	{
		UNIQUE,
		SHARED
	};


	/**
	 * @brief Scopes define the behavior of the same type between dependencies
	 */
	template<ScopeType>
	class Scope;

	template<>
	class Scope<ScopeType::UNIQUE>
	{
	public:
		template<typename T>
		auto Create() -> std::unique_ptr<T>;
	};

	template<typename T>
	auto Scope<ScopeType::UNIQUE>::Create() -> std::unique_ptr<T>
	{
		return std::make_unique<T>();
	}

	template<>
	class Scope<ScopeType::SHARED>
	{
	public:
		template<typename T>
		std::shared_ptr<T> Create();
	};

	template<typename T>
	auto Scope<ScopeType::SHARED>::Create() -> std::shared_ptr<T>
	{
		return std::make_shared<T>();
	}


	/**
	 * @brief Exports
	 */

	using UniqueScope = Scope<ScopeType::UNIQUE>;
	using SharedScope = Scope<ScopeType::SHARED>;

}
