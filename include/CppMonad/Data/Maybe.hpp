#pragma once
#include <optional>
#include "../Class/Show.hpp"
#include "../Class/Functor.hpp"
#include "../Class/Semigroup.hpp"
#include "../Class/Monoid.hpp"
#include "../Class/Apply.hpp"
#include "../Class/Applicative.hpp"
#include "../Class/Bind.hpp"

namespace CppMonad {
	template <class T>
	using Maybe = std::optional<T>;
	
	template <class T>
	Maybe<T> Just(T&& value) {
		return Maybe<T>(std::forward<T>(value));
	}
	
	template <class T>
	Maybe<T> Nothing() {
		return Maybe<T>();
	}
	
	template <class T>
	struct Show<Maybe<T>> {
		static std::string show(const Maybe<T>& value) {
			if (value.has_value()) {
				return std::string("Just ") + Show<T>::show(value.value());
			}
			return std::string("Nothing");
		}
	};
	
	template <>
	struct Functor<Maybe> {
		template <class Func, class A>
		static auto map(
			const Func& func,
			const Maybe<A>& a) {
			using B = decltype(func(a.value()));
			if (a.has_value()) {
				return Just<B>(func(a.value()));
			}
			return Nothing<B>();
		}
	};
	
	template <class T>
	struct Semigroup<Maybe<T>> {
		static Maybe<T> append(const Maybe<T>& a, const Maybe<T>& b) {
			if (a.has_value() && b.has_value()) {
				return Just<T>(Semigroup<T>::append(a.value(), b.value()));
			}
			return Nothing<T>();
		}
	};
	
	template <class T>
	struct Monoid<Maybe<T>> {
		static Maybe<T> mempty() {
			return Nothing<T>();
		}
	};
	
	template <>
	struct Apply<Maybe> {
		template <class Func, class A>
		static auto apply1(
			const Maybe<Func>& func,
			const Maybe<A>& a) {
			using B = decltype(func.value()(a.value()));
			if (func.has_value() && a.has_value()) {
				return Just<B>(func.value()(a.value()));
			}
			return Nothing<B>();
		}
	};
	
	template <>
	struct Applicative<Maybe> {
		template <class T>
		static Maybe<T> pure(T&& value) {
			return Just(std::forward<T>(value));
		}
	};
	
	template <>
	struct Bind<Maybe> {
		template <class Func, class A>
		static auto bind1(
			const Maybe<A>& a,
			const Func& func) {
			using B = decltype(func(a.value()));
			if (a.has_value()) {
				return func(a.value());
			}
			return B();
		}
	};
}
