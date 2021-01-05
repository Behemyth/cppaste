#pragma once

namespace synodic
{

	template<typename T>
	concept is_dependency = true;

	class DependencyBase
	{
	};

	template<typename Interface, typename Implementation>
	class Dependency : DependencyBase
	{

	public:
		Dependency() noexcept = default;


		template<is_dependency T>
		auto Bind() const noexcept;
	};


	template<typename Interface, typename Implementation>
	template<is_dependency T>
	auto Dependency<Interface, Implementation>::Bind() const noexcept
	{
		return Dependency<Interface, T>();
	}

}
