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
	
	void testMaybeApplicative() {
		auto justStr = pure<Maybe>(String("abc"));
		auto nothingStr = Nothing<String>();
		auto justFunc = pure<Maybe>([](const auto& str) { return str.size(); });
		auto nothingFunc = Nothing<int(*)(String)>();
		assert(show(seqApply(justFunc, justStr)) == "Just 3");
		assert(show(seqApply(justFunc, nothingStr)) == "Nothing");
		assert(show(seqApply(nothingFunc, justStr)) == "Nothing");
		assert(show(seqApply(nothingFunc, nothingStr)) == "Nothing");
	}
	
	void testMaybe() {
		std::cout << __func__ << std::endl;
		testMaybeShow();
		testMaybeFunctor();
		testMaybeApplicative();
	}
}
