#pragma once

namespace CppMonad {
	template <class T>
	struct Semigroup {
		static T append(const T& lhs, const T& rhs);
	};
	
	template <class T>
	static T append(const T& lhs, const T& rhs) {
		return Semigroup<T>::append(lhs, rhs);
	}
}
