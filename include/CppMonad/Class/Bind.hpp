#pragma once
#include <utility>

namespace CppMonad {
	template <template <class> class F>
	struct Bind {
		template <class Func, class A>
		static auto bind1(
			const F<A>& a,
			const Func& func) ->
			decltype(std::declval<const Func>()(std::declval<const A>())) = delete;
	};
	
	template <template <class> class F, class Func, class A>
	static auto bind1(const F<A>& a, const Func& func) {
		return Bind<F>::bind1(a, func);
	}
}
