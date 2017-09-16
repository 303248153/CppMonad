#include <iostream>
#include <cassert>
#include <memory>
#include <CppMonad/Utils/Partial.hpp>

namespace CppMonadTests {
	using namespace CppMonad;
	
	void testSimplePartial() {
		auto action = Partial([](int a, int b, int c) { return a + b + c; });
		assert(action(1, 2, 3) == 6);
		assert(action(1)(2)(3) == 6);
		assert(action()()(1)()(2, 3) == 6);
	}
	
	void testRValuePartial() {
		int sum = 1;
		Partial([](std::unique_ptr<int> a, int& b) { b += *a; })
			(std::make_unique<int>(2))
			(std::ref(sum));
		assert(sum == 3);
	}
	
	void testTemplatePartial() {
		auto concat = Partial([](const auto& lhs, const auto& rhs) {
			return lhs + rhs;
		}); 
		auto abc = concat(std::string("a"))(std::string("bc"));
		assert(abc == "abc");
	}
	
	void testPartial() {
		std::cout << __func__ << std::endl;
		testSimplePartial();
		testRValuePartial();
		testTemplatePartial();
	}
}
