#pragma once

namespace CppMonad {
	template <class T>
	struct Monoid {
		static T mempty() {
			return T();
		}
	};
	
	template <class T>
	static T mempty() {
		return Monoid<T>::mempty();
	}
}
