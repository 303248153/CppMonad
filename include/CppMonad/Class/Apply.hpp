#pragma once
#include <utility>
#include "Functor.hpp"

namespace CppMonad {
	template <template <class> class F>
	struct Apply {
		template <class Func, class A>
		static auto apply1(
			const F<Func>& func,
			const F<A>& a) ->
			F<decltype(std::declval<const Func>()(std::declval<const A>()))> = delete;
	};
	
	template <template <class> class F, class Func, class Head>
	static auto applyN(const F<Func>& func, const F<Head>& head) {
		return Apply<F>::apply1(func, head);
	}
	
	template <template <class> class F, class Func, class Head, class... Rest>
	static auto applyN(const F<Func>& func, const F<Head>& head, const F<Rest>&... rest) {
		return applyN(applyN(func, head), rest...);
	}
	
	template <template <class> class F, class Func, class Head>
	static auto liftN(const Func& func, const F<Head>& head) {
		return map(func, head);
	}
	
	template <template <class> class F, class Func, class Head, class... Rest>
	static auto liftN(const Func& func, const F<Head>& head, const F<Rest>&... rest) {
		return applyN(liftN(func, head), rest...);
	}
}
