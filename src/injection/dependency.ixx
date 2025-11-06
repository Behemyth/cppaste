export module synodic.cppaste.injection:dependency;

import std;
import :scope;

export namespace cppaste
{

	/**
	 * @brief A wrapper around types that provide injection metadata
	 */
	template<typename Expected, typename Given, ScopeType Scope>
	class Dependency
	{

	public:
		consteval Dependency() noexcept = default;
	};

}
