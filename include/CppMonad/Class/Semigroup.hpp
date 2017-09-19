#pragma once

namespace CppMonad {
	template <class T>
	struct Semigroup {
		static T append(const T& a, const T& b) {
			return a + b;
		}
	};
	
	template <class T>
	static T append(const T& a, const T& b) {
		return Semigroup<T>::append(a, b);
	}
}
