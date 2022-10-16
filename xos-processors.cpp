#include "tests/i8080_tests.h"
#include "tests/z80_tests.h"


int main()
{
	{
		auto t = tests::i8080_tests::i8080_tests();
		t.run();
	}
	{
		auto t = tests::z80_tests::z80_tests();
		t.run();
	}
}

