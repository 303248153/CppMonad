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
			F<decltype(std::declval<const Func>()(std::declval<const A>()))> = delete;
	};
	
	template <template <class> class F, class Func, class A>
	static auto apply1(const F<Func>& func, const F<A>& from) {
		return Apply<F>::apply1(func, from);
	}
	
	template <template <class> class F, class Func, class A, class B>
	static auto lift2(
		const Func& func,
		const F<A>& a,
		const F<B>& b) {
		return apply1(map(func, a), b);
	}
}
