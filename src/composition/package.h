#pragma once

#include <boost/di.hpp>

#include "module.h"

namespace synodic
{

	template<typename... Modules>
	class Package
	{

	public:
		explicit constexpr Package(synodic::Module<Modules>&... modules);


		template<typename T>
		T Create();

	private:
		boost::di::injector<Modules...> injector_;
	};

	template<typename... Modules>
	constexpr Package<Modules...>::Package(synodic::Module<Modules>&... modules) :
		injector_(boost::di::make_injector(modules.Injector()...))
	{
	}

	template<typename... Modules>
	template<typename T>
	T Package<Modules...>::Create()
	{
		//static_assert(boost::di::is_creatable<T>(injector_), "Not Createable");

		return injector_.template create<T>();
	}

}
