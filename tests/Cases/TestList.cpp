#include <iostream>
#include <cassert>
#include <functional>
#include <CppMonad/Data/List.hpp>

namespace CppMonadTests {
	using namespace CppMonad;
	
	void testListShow() {
		assert(show(List<int>()) == "[]");
		assert(show(List<int>({ 1, 2, 3 })) == "[1, 2, 3]");
	}
	
	void testListFunctor() {
		List<int> a = { 1, 2, 3 };
	 	List<bool> b = map([](const auto& i) { return i % 2 == 0; }, a);
	 	assert(show(b) == "[0, 1, 0]");
	}
	
	void testListApplicative() {
		auto list = pure<List>(1);
		auto func = List<std::function<int(int)>>({
			[](const auto& i) { return i; },
			[](const auto& i) { return i * 2; }
		});
		assert(show(seqApply(func, list)) == "[1, 2]");
		assert(show(seqApply(func, seqApply(func, list))) == "[1, 2, 2, 4]");
	}
	
	void testList() {
		std::cout << __func__ << std::endl;
		testListShow();
		testListFunctor();
		testListApplicative();
	}
}
