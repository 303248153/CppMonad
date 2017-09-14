#include <iostream>
#include "./Cases/TestEither.hpp"
#include "./Cases/TestList.hpp"
#include "./Cases/TestMaybe.hpp"
#include "./Cases/TestString.hpp"

namespace CppMonadTests {
	void testAll() {
		testEither();
		testList();
		testMaybe();
		testString();
	}
}

int main() {
	CppMonadTests::testAll();
	return 0;
}
