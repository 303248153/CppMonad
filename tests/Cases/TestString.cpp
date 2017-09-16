#include <iostream>
#include <cassert>
#include <CppMonad/Data/String.hpp>

namespace CppMonadTests {
	using namespace CppMonad;
	
	void testStringShow() {
		assert(show(String("")) == "");
		assert(show(String("abc")) == "abc");
	}
	
	void testStringSemigroup() {
		assert(show(append(String("a"), String("bc"))) == "abc");
	}
	
	void testStringMonoid() {
		assert(show(mempty<String>()) == "");
	}
	
	void testString() {
		std::cout << __func__ << std::endl;
		testStringShow();
		testStringSemigroup();
		testStringMonoid();
	}
}
