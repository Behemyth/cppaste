#pragma once

#include <memory>

namespace cppaste
{

	enum class ScopeType
	{
		UNIQUE,
		SHARED
	};


	template<ScopeType>
	class Scope;

	template<>
	class Scope<ScopeType::UNIQUE>
	{
	public:
		template<typename T>
		std::unique_ptr<T> Create();
	};

	template<typename T>
	std::unique_ptr<T> Scope<ScopeType::UNIQUE>::Create()
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
	std::shared_ptr<T> Scope<ScopeType::SHARED>::Create()
	{
		return std::make_shared<T>();
	}


	// Exports

	using UniqueScope = Scope<ScopeType::UNIQUE>;
	using SharedScope = Scope<ScopeType::SHARED>;

}
