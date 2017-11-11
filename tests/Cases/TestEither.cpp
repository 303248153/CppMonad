#include <iostream>
#include <cassert>
#include <CppMonad/Data/String.hpp>
#include <CppMonad/Data/Either.hpp>
#include <CppMonad/Utils/Partial.hpp>

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
		auto rightStrA = Right<int, String>("abc");
		auto rightStrB = Right<int, String>("abcde");
		auto rightFunc = Right<int>(Partial([](auto&& a, auto&& b) { return a.size() + b.size(); }));
		auto leftStr = Left<int, String>(1);
		auto leftFunc = Left<int, PartialHolder<std::size_t(*)(String, String)>>(2);
		assert(show(applyN(rightFunc, rightStrA, rightStrB)) == "Right 8");
		assert(show(applyN(rightFunc, rightStrA, leftStr)) == "Left 1");
		assert(show(applyN(leftFunc, rightStrA, rightStrB)) == "Left 2");
		assert(show(applyN(leftFunc, leftStr, leftStr)) == "Left 2");
	}

	void testEitherApplyLift() {
		auto concat = Partial([](auto&& a, auto&& b, auto&& c) { return a + b + c; });
		auto rightStrA = Right<int, String>("hello");
		auto rightStrB = Right<int, String>(" ");
		auto rightStrC = Right<int, String>("world");
		auto leftStr = Left<int, String>(1);
		assert(show(liftN(concat, rightStrA, rightStrB, rightStrC)) == "Right hello world");
		assert(show(liftN(concat, rightStrA, rightStrB, leftStr)) == "Left 1");
		assert(show(liftN(concat, leftStr, rightStrB, rightStrC)) == "Left 1");
	}

	void testEitherApplicative() {
		auto rightStr = pure<Either, int>(String("abc"));
		assert(show(rightStr) == "Right abc");
	}

	void testEitherBind() {
		auto rightStr = Right<int>(String("abc"));
		auto rightSize = bind1(rightStr, [](auto&& str) { return Right<int>(str.size()); });
		assert(show(rightSize) == "Right 3");
		auto leftStr = Left<int, String>(1);
		auto leftSize = bind1(leftStr, [](auto&& str) { return Right<int>(str.size()); });
		assert(show(leftSize) == "Left 1");
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
