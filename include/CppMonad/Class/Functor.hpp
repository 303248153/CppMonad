#pragma once
#include <utility>

namespace CppMonad {
	template <template <class> class F>
	struct Functor {
		template <class Func, class A>
		static auto map(
			const Func& func,
			const F<A>& a) ->
			F<decltype(func(std::declval<const A>()))> = delete;
	};
	
	template <template <class> class F, class Func, class A>
	static auto map(
		const Func& func,
		const F<A>& a) {
		return Functor<F>::map(func, a);
	}
}
