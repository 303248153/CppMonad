#pragma once

namespace CppMonad {
	template <class T>
	class Monoid {
		static T mempty();
	};
	
	template <class T>
	static T mempty() {
		return Monoid<T>::mempty();
	}
}
