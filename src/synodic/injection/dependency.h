#pragma once

#include <concepts>

namespace cppaste
{

	// Creates a link between two related types
	template<class Scope, typename Interface, std::derived_from<Interface> Implementation>
	class Dependency : public Scope::template scope<Interface, Implementation>
	{

	public:
		Dependency() noexcept = default;


		template<std::derived_from<Interface> T>
		auto Bind() const noexcept;
	};


	template<class Scope, typename Interface, std::derived_from<Interface> Implementation>
	template<std::derived_from<Interface> T>
	auto Dependency<Scope, Interface, Implementation>::Bind() const noexcept
	{
		return Dependency<Scope, Interface, T>();
	}

}
