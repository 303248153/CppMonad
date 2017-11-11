#pragma once
#include <variant>
#include <type_traits>
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

	// it's hacky too
	template <>
	struct Applicative<PartialEither> {
		template <class TLeft, class TRight>
		static Either<TLeft, TRight> pure(TRight&& value) {
			return Right<TLeft, TRight>(std::forward<TRight>(value));
		}
	};

	template <template <class, class> class F, class TLeft, class TRight,
		std::enable_if_t<std::is_same_v<F<TLeft, TRight>, Either<TLeft, TRight>>, int> = 0>
	static Either<TLeft, TRight> pure(TRight&& value) {
		return Applicative<PartialEither>::pure<TLeft, TRight>(
			std::forward<TRight>(value));
	}

	template <>
	struct Bind<PartialEither> {
		template <class Func, class TLeft, class TRight>
		static auto bind1(
			const Either<TLeft, TRight>& a,
			const Func& func) {
			using B = decltype(func(std::get<1>(a)));
			if (a.index() == 1) {
				return func(std::get<1>(a));
			}
			return B(std::get<0>(a));
		}
	};

	template <class Func, class TLeft, class TRight>
	static auto bind1(const Either<TLeft, TRight>& a, const Func& func) {
		return Bind<PartialEither>::bind1(a, func);
	}
}
