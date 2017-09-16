#pragma once
#include <optional>
#include "../Class/Show.hpp"
#include "../Class/Functor.hpp"
#include "../Class/Semigroup.hpp"
#include "../Class/Monoid.hpp"
#include "../Class/Apply.hpp"
#include "../Class/Applicative.hpp"

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
		template <class A, class Func>
		static auto map(
			const Func& func,
			const Maybe<A>& from) {
			Maybe<decltype(func(std::declval<A>()))> to;
			if (from.has_value()) {
				to.emplace(func(from.value()));
			}
			return to;
		}
	};
	
	template <class T>
	struct Semigroup<Maybe<T>> {
		static Maybe<T> append(const Maybe<T>& lhs, const Maybe<T>& rhs) {
			if (lhs.has_value() && rhs.has_value()) {
				return Just<T>(Semigroup<T>::append(lhs.value(), rhs.value()));
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
			const Maybe<A>& from) {
			if (func.has_value() && from.has_value()) {
				return Just(func.value()(from.value()));
			}
			return Nothing<decltype(std::declval<Func>()(std::declval<A>()))>();
		}
	};
	
	template <>
	struct Applicative<Maybe> {
		template <class T>
		static Maybe<T> pure(T&& value) {
			return Just(std::forward<T>(value));
		}
	};
}
