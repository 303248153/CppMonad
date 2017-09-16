#include <iostream>
#include <cassert>
#include <functional>
#include <CppMonad/Data/Maybe.hpp>
#include <CppMonad/Data/String.hpp>
#include <CppMonad/Utils/Partial.hpp>

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
		auto justStrA = Just(String("abc"));
		auto justStrB = Just(String("abcde"));
		auto justFunc = Just(Partial([](auto&& a, auto&& b) { return a.size() + b.size(); }));
		auto nothingStr = Nothing<String>();
		auto nothingFunc = Nothing<PartialHolder<int(*)(String, String)>>();
		assert(show(applyN(justFunc, justStrA, justStrB)) == "Just 8");
		assert(show(applyN(justFunc, justStrA, nothingStr)) == "Nothing");
		assert(show(applyN(nothingFunc, justStrA, justStrB)) == "Nothing");
		assert(show(applyN(nothingFunc, nothingStr, nothingStr)) == "Nothing");
	}
	
	void testMaybeApplyLift() {
		auto concat = Partial([](auto&& a, auto&& b, auto&& c) { return a + b + c; });
		auto justStrA = pure<Maybe>(String("hello"));
		auto justStrB = pure<Maybe>(String(" "));
		auto justStrC = pure<Maybe>(String("world"));
		auto nothingStr = Nothing<String>();
		assert(show(liftN(concat, justStrA, justStrB, justStrC)) == "Just hello world");
		assert(show(liftN(concat, justStrA, justStrB, nothingStr)) == "Nothing");
		assert(show(liftN(concat, nothingStr, justStrB, justStrC)) == "Nothing");
	}
	
	void testMaybeApplicative() {
		auto justStr = pure<Maybe>(String("abc"));
		assert(show(justStr) == "Just abc");
	}
	
	void testMaybe() {
		std::cout << __func__ << std::endl;
		testMaybeShow();
		testMaybeFunctor();
		testMaybeSemigroup();
		testMaybeMonoid();
		testMaybeApply();
		testMaybeApplyLift();
		testMaybeApplicative();
	}
}
