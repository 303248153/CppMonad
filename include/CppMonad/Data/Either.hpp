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
		template <class Func, class TLeft, class TRight>
		static auto map(
			const Func& func,
			const Either<TLeft, TRight>& a) {
			using NextRight = decltype(func(std::get<1>(a)));
			if (a.index() == 0) {
				return Left<TLeft, NextRight>(static_cast<TLeft>(std::get<0>(a)));
			}
			return Right<TLeft, NextRight>(func(std::get<1>(a)));
		}
	};

	template <class Func, class TLeft, class TRight>
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

	// it's hacky too
	template <>
	struct Apply<PartialEither> {
		template <class Func, class TLeft, class TRight>
		static auto apply1(
			const Either<TLeft, Func>& func,
			const Either<TLeft, TRight>& a) {
			using NextRight = decltype(std::get<1>(func)(std::get<1>(a)));
			if (func.index() == 1 && a.index() == 1) {
				return Right<TLeft, NextRight>(std::get<1>(func)(std::get<1>(a)));
			} else if (func.index() == 0) {
				return Left<TLeft, NextRight>(static_cast<TLeft>(std::get<0>(func)));
			} else {
				return Left<TLeft, NextRight>(static_cast<TLeft>(std::get<0>(a)));
			}
		}
	};
	
	template <class Func, class TLeft, class Head>
	static auto applyN(
		const Either<TLeft, Func>& func,
		const Either<TLeft, Head>& head) {
		return Apply<PartialEither>::apply1(func, head);
	}
	
	template <class Func, class TLeft, class Head, class... Rest>
	static auto applyN(
		const Either<TLeft, Func>& func,
		const Either<TLeft, Head>& head,
		const Either<TLeft, Rest>&... rest) {
		return applyN(applyN(func, head), rest...);
	}
	
	template <class Func, class TLeft, class Head>
	static auto liftN(
		const Func& func,
		const Either<TLeft, Head>& head) {
		return map(func, head);
	}
	
	template <class Func, class TLeft, class Head, class... Rest>
	static auto liftN(
		const Func& func,
		const Either<TLeft, Head>& head,
		const Either<TLeft, Rest>&... rest) {
		return applyN(liftN(func, head), rest...);
	}
}
