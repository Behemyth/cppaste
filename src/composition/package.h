#pragma once

#include <boost/di.hpp>

#include "module.h"

namespace synodic
{

	template<typename... Modules>
	class Package
	{

	public:
		explicit constexpr Package(Module<Modules>&... modules);


		template<typename T>
		T Create();

	private:
		boost::di::injector<Modules...> staticInjector_;
	};

	template<typename... Modules>
	constexpr Package<Modules...>::Package(Module<Modules>&... modules) :
		staticInjector_(boost::di::make_injector(modules.Inject()...))
	{
	}

	template<typename... Modules>
	template<typename T>
	T Package<Modules...>::Create()
	{
		//static_assert(boost::di::is_creatable<T>(injector_), "Not Createable");

		return staticInjector_.template create<T>();
	}

}
