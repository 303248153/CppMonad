#include <iostream>
#include <cassert>
#include <CppMonad/Data/String.hpp>
#include <CppMonad/Data/Either.hpp>

namespace CppMonadTests {
	using namespace CppMonad;

	void testEitherShow() {
		auto left = Left<int, String>(1);
		auto right = Right<int, String>("abc");
		assert(show(left) == "Left 1");
		assert(show(right) == "Right abc");
	}

	void testEitherFunctor() {
		auto left = Left<int, String>(1);
		auto right = Right<int, String>("abc");
		auto mappedLeft = map([](const auto& str) { return str.size(); }, left);
		auto mappedRight = map([](const auto& str) { return str.size(); }, right);
		assert(show(mappedLeft) == "Left 1");
		assert(show(mappedRight) == "Right 3");
	}

	void testEitherSemigroup() {
		auto left = Left<int, String>(1);
		auto right = Right<int, String>("abc");
		assert(show(append(left, right)) == "Right abc");
		assert(show(append(right, left)) == "Right abc");
	}

	void testEitherMonoid() {
		auto empty = mempty<Either<int, String>>();
		assert(show(empty) == "Left 0");
	}

	void testEitherApply() {

	}

	void testEitherApplicative() {
	}

	void testEitherBind() {
	}

	void testEither() {
		std::cout << __func__ << std::endl;
		testEitherShow();
		testEitherFunctor();
		testEitherSemigroup();
		testEitherMonoid();
		testEitherApply();
		testEitherApplicative();
		testEitherBind();
	}
}
