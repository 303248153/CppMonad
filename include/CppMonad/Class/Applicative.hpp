#pragma once
#include <utility>

namespace CppMonad {
	template <template <class> class F>
	struct Applicative {
		template <class T>
		static F<T> pure(T&& value) = delete;
		
		template <class Func, class A>
		static auto seqApply(
			const F<Func>& func,
			const F<A>& from) ->
			F<decltype(std::declval<Func>()(std::declval<A>()))> = delete;
	};
	
	template <template <class> class F, class T>
	static F<T> pure(T&& value) {
		return Applicative<F>::pure(std::forward<T>(value));
	}
	
	template <template <class> class F, class Func, class A>
	static auto seqApply(const F<Func>& func, const F<A>& from) {
		return Applicative<F>::seqApply(func, from);
	}
}
