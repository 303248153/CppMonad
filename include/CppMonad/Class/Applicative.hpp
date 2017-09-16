#pragma once
#include <utility>
#include "Apply.hpp"

namespace CppMonad {
	template <template <class> class F>
	struct Applicative {
		template <class T>
		static F<T> pure(T&& value) = delete;
	};
	
	template <template <class> class F, class T>
	static F<T> pure(T&& value) {
		return Applicative<F>::pure(std::forward<T>(value));
	}
	
	template <template <class> class F, class Func, class A, class B>
	static auto lift2(
		const Func& func,
		const F<A>& a,
		const F<B>& b) {
		return apply1(map(func, a), b);
	}
}
