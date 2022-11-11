#pragma once

#include "cpu_tests.h"


using namespace xprocessors;

namespace tests
{
	class i8080_tests : public cpu_tests {
	protected:
		void out(const uint8_t, const uint8_t);
		bool runTest(const string&, const uint64_t = 0);

	public:
		i8080_tests();

		bool run();
	};
}
