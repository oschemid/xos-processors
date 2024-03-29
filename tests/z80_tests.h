#pragma once
#include "cpu_tests.h"

using namespace xprocessors;

namespace tests
{
	class z80_tests : public cpu_tests {
	protected:
		void out(const uint8_t, const uint8_t);
		bool runTest(const string&, const uint64_t = 0);
		bool runTestPerfs(const uint64_t cycles_expected);

	public:
		z80_tests();

		bool run();
	};
}