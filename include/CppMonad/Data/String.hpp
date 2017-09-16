#pragma once
#include <string>
#include "../Class/Show.hpp"
#include "../Class/Functor.hpp"
#include "../Class/Semigroup.hpp"
#include "../Class/Monoid.hpp"

namespace CppMonad {
	using String = std::string;
	
	template <>
	struct Semigroup<String> {
		static String append(const String& lhs, const String& rhs) {
			return lhs + rhs;
		}
	};
	
	template <>
	struct Monoid<String> {
		static String mempty() {
			return "";
		}
	};
}
