#pragma once

#include <concepts>

namespace cppaste
{

	class DependencyType
	{
	};

	template<class Scope, typename Interface, std::derived_from<Interface> Implementation>
	class Dependency : DependencyType, public Scope::template scope<Interface, Implementation>
	{

	public:
		Dependency() noexcept = default;


		template<std::derived_from<Interface> T>
		auto Bind() const noexcept;
	};


	//template<typename Interface, std::derived_from<Interface> Implementation>
	//template<std::derived_from<Interface> T>
	//auto Dependency<Interface, Implementation>::Bind() const noexcept
	//{
	//	return Dependency<Interface, T>();
	//}

}
