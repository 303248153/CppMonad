#pragma once
#include <sstream>
#include <type_traits>

namespace CppMonad {
	template <class T>
	struct Show {
		static std::string show(const T& value) {
			if constexpr (std::is_same_v<T, std::string>) {
				return static_cast<T>(value);
			} else {
				std::ostringstream stream;
				stream << value;
				return stream.str();
			}
		}
	};
	
	template <class T>
	static std::string show(const T& value) {
		return Show<T>::show(value);
	}
}
