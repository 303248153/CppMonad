#pragma once
#include <utility>

namespace CppMonad {
	template <template <class> class F>
	struct Functor {
		template <class A, class Func>
		static auto map(
			const Func& func,
			const F<A>& from) ->
			F<decltype(func(std::declval<const A>()))> = delete;
	};
	
	template <template <class> class F, class A, class Func>
	static auto map(
		const Func& func,
		const F<A>& from) {
		return Functor<F>::map(func, from);
	}
}
