#pragma once
#include <list>
#include <algorithm>
#include <sstream>
#include "../Class/Show.hpp"
#include "../Class/Functor.hpp"
#include "../Class/Semigroup.hpp"
#include "../Class/Monoid.hpp"
#include "../Class/Apply.hpp"
#include "../Class/Applicative.hpp"
#include "../Class/Bind.hpp"

namespace CppMonad {
	template <class T, class Alloc = std::allocator<T>>
	using List = std::list<T, Alloc>;
	
	template <class T>
	struct Show<List<T>> {
		static std::string show(const List<T>& value) {
			std::ostringstream stream;
			stream << "[";
			bool isFirst = true;
			for (const T& item : value) {
				if (!isFirst) {
					stream << ", ";
				} else {
					isFirst = false;
				}
				stream << Show<T>::show(item);
			}
			stream << "]";
			return stream.str();
		}
	};
	
	template <>
	struct Functor<List> {
		template <class Func, class A>
		static auto map(
			const Func& func,
			const List<A>& a) {
			List<decltype(func(*a.cbegin()))> b;
			std::transform(a.cbegin(), a.cend(), std::back_inserter(b), func);
			return b;
		}
	};
	
	template <class T>
	struct Semigroup<List<T>> {
		static List<T> append(const List<T>& a, const List<T>& b) {
			List<T> result;
			for (const auto& item : a) {
				result.emplace_back(item);
			}
			for (const auto& item : b) {
				result.emplace_back(item);
			}
			return result;
		}
	};
	
	template <class T>
	struct Monoid<List<T>> {
		static List<T> mempty() {
			return List<T>();
		}
	};
	
	template <>
	struct Apply<List> {
		template <class Func, class A>
		static auto apply1(
			const List<Func>& func,
			const List<A>& a) {
			List<decltype((*func.cbegin())(*a.cbegin()))> b;
			for (const auto& f : func) {
				for (const auto& item : a) {
					b.emplace_back(f(item));
				}
			}
			return b;
		}
	};
	
	template <>
	struct Applicative<List> {
		template <class T>
		static List<T> pure(T&& value) {
			return List<T>({ value });
		}
	};
	
	template <>
	struct Bind<List> {
		template <class Func, class A>
		static auto bind1(
			const List<A>& a,
			const Func& func) {
			decltype(func(*a.cbegin())) b;
			for (const auto& item : a) {
				for (const auto& result : func(item)) {
					b.emplace_back(result);
				}
			}
			return b;
		}
	};
}
