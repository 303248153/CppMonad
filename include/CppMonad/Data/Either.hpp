#pragma once
#include <variant>
#include "../Class/Show.hpp"
#include "../Class/Functor.hpp"
#include "../Class/Semigroup.hpp"
#include "../Class/Monoid.hpp"
#include "../Class/Apply.hpp"
#include "../Class/Applicative.hpp"
#include "../Class/Bind.hpp"

namespace CppMonad {
	template <class TLeft, class TRight>
	using Either = std::variant<TLeft, TRight>;

	template <class T>
	struct PartialEither { };

	template <class TLeft, class TRight>
	Either<TLeft, TRight> Left(TLeft&& value) {
		return Either<TLeft, TRight>(std::forward<TLeft>(value));
	}

	template <class TLeft, class TRight>
	Either<TLeft, TRight> Right(TRight&& value) {
		return Either<TLeft, TRight>(std::forward<TRight>(value));
	}

	template <class TLeft, class TRight>
	struct Show<Either<TLeft, TRight>> {
		static std::string show(const Either<TLeft, TRight>& value) {
			if (value.index() == 0) {
				return std::string("Left ") + Show<TLeft>::show(std::get<0>(value));
			}
			return std::string("Right ") + Show<TRight>::show(std::get<1>(value));
		}
	};

	// it's hacky and may not work with nested type,
	// but c++ isn't support partial template specialization with template template class
	template <>
	struct Functor<PartialEither> {
		template <class TLeft, class TRight, class Func>
		static auto map(
			const Func& func,
			const Either<TLeft, TRight>& a) {
			Either<TLeft, decltype(func(std::declval<const TRight>()))> b;
			if (a.index() == 0) {
				b = std::get<0>(a);
			} else {
				b = func(std::get<1>(a));
			}
			return b;
		}
	};

	template <class TLeft, class TRight, class Func>
	static auto map(
		const Func& func,
		const Either<TLeft, TRight>& a) {
		return Functor<PartialEither>::map(func, a);
	}

	template <class TLeft, class TRight>
	struct Semigroup<Either<TLeft, TRight>> {
		static Either<TLeft, TRight> append(
			const Either<TLeft, TRight>& a, const Either<TLeft, TRight>& b) {
			if (a.index() == 0) {
				return b;
			}
			return a;
		}
	};

	template <class TLeft, class TRight>
	struct Monoid<Either<TLeft, TRight>> {
		static Either<TLeft, TRight> mempty() {
			return Either<TLeft, TRight>();
		}
	};

	// it's also hacky
	template <>
	struct Apply<PartialEither> {
		template <class Func, class TLeft, class TRight>
		static auto apply1(
			const Either<TLeft, Func>& func,
			const Either<TLeft, TRight>& a) {
			Either<TLeft, decltype(std::get<1>(func)(std::get<1>(a)))> b;
			if (func.index() == 1 && a.index() == 1) {
				b = std::get<1>(func)(std::get<1>(a));
			} else if (func.index() == 0) {
				b = std::get<0>(func);
			} else {
				b = std::get<0>(a);
			}
			return b;
		}
	};
}
