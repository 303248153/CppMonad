#include <iostream>
#include <cassert>
#include <CppMonad/Data/String.hpp>

namespace CppMonadTests {
	using namespace CppMonad;
	
	void testStringShow() {
		assert(show(String("")) == "");
		assert(show(String("abc")) == "abc");
	}
	
	void testString() {
		std::cout << __func__ << std::endl;
		testStringShow();
	}
}
