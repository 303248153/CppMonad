#include <iostream>
#include <cassert>
#include <CppMonad/Data/Maybe.hpp>
#include <CppMonad/Data/String.hpp>

namespace CppMonadTests {
	using namespace CppMonad;
	
	void testMaybeShow() {
		assert(show(Just(String("abc"))) == "Just abc");
		assert(show(Nothing<String>()) == "Nothing");
	}
	
	void testMaybeFunctor() {
		auto str = Just(String("abc"));
		auto size = map([](const auto& str) { return str.size(); }, str);
		auto sizePow = map([](const auto& i) { return i * i; }, size);
		assert(show(sizePow) == "Just 9");
	}
	
	void testMaybeSemigroup() {
		auto lhsStr = pure<Maybe>(String("hello "));
		auto rhsStr = pure<Maybe>(String("world"));
		assert(show(append(lhsStr, rhsStr)) == "Just hello world");
	}
	
	void testMaybeMonoid() {
		auto lhsStr = mempty<Maybe<String>>();
		auto rhsStr = pure<Maybe>(String("world"));
		assert(show(append(lhsStr, rhsStr)) == "Nothing");
	}
	
	void testMaybeApply() {
		auto justStr = Just(String("abc"));
		auto nothingStr = Nothing<String>();
		auto justFunc = Just([](const auto& str) { return str.size(); });
		auto nothingFunc = Nothing<int(*)(String)>();
		assert(show(apply1(justFunc, justStr)) == "Just 3");
		assert(show(apply1(justFunc, nothingStr)) == "Nothing");
		assert(show(apply1(nothingFunc, justStr)) == "Nothing");
		assert(show(apply1(nothingFunc, nothingStr)) == "Nothing");
	}
	
	void testMaybeApplicative() {
		auto justStr = pure<Maybe>(String("abc"));
		assert(show(justStr) == "Just abc");
	}
	
	void testMaybeApplicativeLift() {
		auto concat = [](const auto& lhs) {
			return [lhs](const auto& rhs) {
				return lhs + rhs;
			};
		};
		auto lhsStr = pure<Maybe>(String("hello "));
		auto rhsStr = pure<Maybe>(String("world"));
		auto nothingStr = Nothing<String>();
		assert(show(lift2(concat, lhsStr, rhsStr)) == "Just hello world");
		assert(show(lift2(concat, lhsStr, nothingStr)) == "Nothing");
		assert(show(lift2(concat, nothingStr, rhsStr)) == "Nothing");
	}
	
	void testMaybe() {
		std::cout << __func__ << std::endl;
		testMaybeShow();
		testMaybeFunctor();
		testMaybeSemigroup();
		testMaybeMonoid();
		testMaybeApply();
		testMaybeApplicative();
		testMaybeApplicativeLift();
	}
}
