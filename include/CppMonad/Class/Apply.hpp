#pragma once
#include <utility>
#include "Functor.hpp"

namespace CppMonad {
	template <template <class> class F>
	struct Apply {
		template <class Func, class A>
		static auto apply1(
			const F<Func>& func,
			const F<A>& from) ->
			F<decltype(std::declval<Func>()(std::declval<A>()))> = delete;
	};
	
	template <template <class> class F, class Func, class A>
	static auto apply1(const F<Func>& func, const F<A>& from) {
		return Apply<F>::apply1(func, from);
	}
}
