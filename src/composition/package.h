#pragma once

#include "module.h"

namespace synodic
{

	template<typename... Modules>
	class Package
	{
		
	public:
		explicit Package(synodic::Module<Modules>&... modules);


		template<typename T>
		T Create();
		
	private:
		
		boost::di::injector<Modules...> injector_;
		
	};

	template<typename... Modules>
	Package<Modules...>::Package(synodic::Module<Modules>&... modules) :
		injector_(boost::di::make_injector(modules.Injector()...))
	{
	}
	
	template<typename... Modules>
	template<typename T>
	T Package<Modules...>::Create()
	{
		return injector_.template create<T>();
	}
	
}



