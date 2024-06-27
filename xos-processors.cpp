#include "tests/cpu_tests.h"


int main()
{
	tests::sm83_run();
	tests::z80_run();
	tests::i8080_run();
}
