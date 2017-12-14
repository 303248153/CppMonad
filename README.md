# Header only library implements haskell types in c++17

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/4bf07f94d26640d6a9645406b46bee20)](https://www.codacy.com/app/303248153/CppMonad?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=303248153/CppMonad&amp;utm_campaign=Badge_Grade)
[![Build Status](https://travis-ci.org/303248153/CppMonad.svg?branch=master)](https://travis-ci.org/303248153/CppMonad)

# Implemented types:

- Either
- List
- Maybe
- String

# How to use:

Add "CppMonad/include" to include path, then include header files:

``` c++
#include <CppMonad/Data/Either.hpp>
#include <CppMonad/Data/List.hpp>
#include <CppMonad/Data/Maybe.hpp>
#include <CppMonad/Data/String.hpp>
#include <CppMonad/Utils/Partial.hpp>
```

# Examples:

### Either

**Basic**

``` c++
auto left = Left<int, String>(1);
auto right = Right<int, String>("abc");
assert(show(left) == "Left 1");
assert(show(right) == "Right abc");
```

**Functor**

``` c++
auto left = Left<int, String>(1);
auto right = Right<int, String>("abc");
auto mappedLeft = map([](const auto& str) { return str.size(); }, left);
auto mappedRight = map([](const auto& str) { return str.size(); }, right);
assert(show(mappedLeft) == "Left 1");
assert(show(mappedRight) == "Right 3");
```

**Semigroup**

``` c++
auto left = Left<int, String>(1);
auto right = Right<int, String>("abc");
assert(show(append(left, right)) == "Right abc");
assert(show(append(right, left)) == "Right abc");
```

**Monoid**

``` c++
auto empty = mempty<Either<int, String>>();
assert(show(empty) == "Left 0");
```

**Apply**

``` c++
auto rightStrA = Right<int, String>("abc");
auto rightStrB = Right<int, String>("abcde");
auto rightFunc = Right<int>(Partial([](auto&& a, auto&& b) { return a.size() + b.size(); }));
auto leftStr = Left<int, String>(1);
auto leftFunc = Left<int, PartialHolder<std::size_t(*)(String, String)>>(2);
assert(show(applyN(rightFunc, rightStrA, rightStrB)) == "Right 8");
assert(show(applyN(rightFunc, rightStrA, leftStr)) == "Left 1");
assert(show(applyN(leftFunc, rightStrA, rightStrB)) == "Left 2");
assert(show(applyN(leftFunc, leftStr, leftStr)) == "Left 2");
```

**Lift**

``` c++
auto concat = Partial([](auto&& a, auto&& b, auto&& c) { return a + b + c; });
auto rightStrA = Right<int, String>("hello");
auto rightStrB = Right<int, String>(" ");
auto rightStrC = Right<int, String>("world");
auto leftStr = Left<int, String>(1);
assert(show(liftN(concat, rightStrA, rightStrB, rightStrC)) == "Right hello world");
assert(show(liftN(concat, rightStrA, rightStrB, leftStr)) == "Left 1");
assert(show(liftN(concat, leftStr, rightStrB, rightStrC)) == "Left 1");
```

**Applicative**

``` c++
auto rightStr = pure<Either, int>(String("abc"));
assert(show(rightStr) == "Right abc");
```

**Bind**

``` c++
auto rightStr = Right<int>(String("abc"));
auto rightSize = bind1(rightStr, [](auto&& str) { return Right<int>(str.size()); });
assert(show(rightSize) == "Right 3");
auto leftStr = Left<int, String>(1);
auto leftSize = bind1(leftStr, [](auto&& str) { return Right<int>(str.size()); });
assert(show(leftSize) == "Left 1");
```

### List

**Basic**

``` c++
assert(show(List<int>()) == "[]");
assert(show(List<int>({ 1, 2, 3 })) == "[1, 2, 3]");
```

**Functor**

``` c++
List<int> a = { 1, 2, 3 };
List<bool> b = map([](const auto& i) { return i % 2 == 0; }, a);
assert(show(b) == "[0, 1, 0]");
```

**Semigroup**

``` c++
List<int> lhs = { 1 };
List<int> rhs = { 2, 3 };
assert(show(append(lhs, rhs)) == "[1, 2, 3]");
```

**Monoid**

``` c++
auto lhs = mempty<List<int>>();
List<int> rhs = { 2, 3 };
assert(show(append(lhs, rhs)) == "[2, 3]");
```

**Apply**

``` c++
auto list = List<int>({ 1 });
auto func = List<std::function<int(int)>>({
	[](const auto& i) { return i; },
	[](const auto& i) { return i * 2; }
});
assert(show(applyN(func, list)) == "[1, 2]");
assert(show(applyN(func, applyN(func, list))) == "[1, 2, 2, 4]");
```

**Applicative**

``` c++
auto list = pure<List>(1);
assert(show(list) == "[1]");
```

**Bind**

``` c++
auto list = List<String>({ "he", "l", "lo" });
auto actual = bind1(list, [](auto& item) {
	List<int> result;
	for (const auto& c : item) {
		result.emplace_back(c);
	}
	return result;
});
auto excepted = List<int>({ 'h', 'e', 'l', 'l', 'o' });
assert(show(actual) == show(excepted));
```

### Maybe

**Basic**

``` c++
assert(show(Just(String("abc"))) == "Just abc");
assert(show(Nothing<String>()) == "Nothing");
```

**Functor**

``` c++
auto str = Just(String("abc"));
auto size = map([](const auto& str) { return str.size(); }, str);
auto sizePow = map([](const auto& i) { return i * i; }, size);
assert(show(sizePow) == "Just 9");
```

**Semigroup**

``` c++
auto lhsStr = pure<Maybe>(String("hello "));
auto rhsStr = pure<Maybe>(String("world"));
assert(show(append(lhsStr, rhsStr)) == "Just hello world");
```

**Monoid**

``` c++
auto lhsStr = mempty<Maybe<String>>();
auto rhsStr = pure<Maybe>(String("world"));
assert(show(append(lhsStr, rhsStr)) == "Nothing");
```

**Apply**

``` c++
auto justStrA = Just(String("abc"));
auto justStrB = Just(String("abcde"));
auto justFunc = Just(Partial([](auto&& a, auto&& b) { return a.size() + b.size(); }));
auto nothingStr = Nothing<String>();
auto nothingFunc = Nothing<PartialHolder<std::size_t(*)(String, String)>>();
assert(show(applyN(justFunc, justStrA, justStrB)) == "Just 8");
assert(show(applyN(justFunc, justStrA, nothingStr)) == "Nothing");
assert(show(applyN(nothingFunc, justStrA, justStrB)) == "Nothing");
assert(show(applyN(nothingFunc, nothingStr, nothingStr)) == "Nothing");
```

**Lift**

``` c++
auto concat = Partial([](auto&& a, auto&& b, auto&& c) { return a + b + c; });
auto justStrA = pure<Maybe>(String("hello"));
auto justStrB = pure<Maybe>(String(" "));
auto justStrC = pure<Maybe>(String("world"));
auto nothingStr = Nothing<String>();
assert(show(liftN(concat, justStrA, justStrB, justStrC)) == "Just hello world");
assert(show(liftN(concat, justStrA, justStrB, nothingStr)) == "Nothing");
assert(show(liftN(concat, nothingStr, justStrB, justStrC)) == "Nothing");
```

**Applicative**

``` c++
auto justStr = pure<Maybe>(String("abc"));
assert(show(justStr) == "Just abc");
```

**Bind**

``` c++
auto justStr = Just(String("abc"));
auto justSize = bind1(justStr, [](auto&& str) { return Just(str.size()); });
assert(show(justSize) == "Just 3");
auto nothingStr = Nothing<String>();
auto nothingSize = bind1(nothingStr, [](auto&& str) { return Just(str.size()); });
assert(show(nothingSize) == "Nothing");
```

### String

**Basic**

``` c++
assert(show(String("")) == "");
assert(show(String("abc")) == "abc");
```

**Semigroup**

``` c++
assert(show(append(String("a"), String("bc"))) == "abc");
```

**Monoid**

``` c++
assert(show(mempty<String>()) == "");
```

# License

LICENSE: MIT LICENSE<br/>
Copyright © 2017 303248153@github<br/>
If you have any license issue please contact 303248153@qq.com.<br/>
