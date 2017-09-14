#pragma once
#include <list>
#include <algorithm>
#include <sstream>
#include "../Class/Show.hpp"
#include "../Class/Functor.hpp"
#include "../Class/Applicative.hpp"

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
		template <class A, class Func>
		static auto map(
			const Func& func,
			const List<A>& from) {
			List<decltype(func(std::declval<A>()))> to;
			std::transform(from.cbegin(), from.cend(), std::back_inserter(to), func);
			return to;
		}
	};
	
	template <>
	struct Applicative<List> {
		template <class T>
		static List<T> pure(T&& value) {
			return List<T>({ value });
		}
		
		template <class Func, class A>
		static auto seqApply(
			const List<Func>& func,
			const List<A>& from) {
			List<decltype(std::declval<Func>()(std::declval<A>()))> to;
			for (const auto& f : func) {
				for (const auto& a : from) {
					to.emplace_back(f(a));
				}
			}
			return to;
		}
	};
}
