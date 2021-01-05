#pragma once

namespace synodic::resolver
{
	template <class TDeps, class T>
	using resolve_template_t = T;//typename resolve_template_impl<TDeps, aux::remove_qualifiers_t<T>>::type;

}
