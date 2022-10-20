#pragma once

#include "cpu_tests.h"


using namespace xprocessors;

namespace tests
{
	class lr35902_tests : public cpu_tests {
	protected:
		void out(const uint8_t, const uint8_t);
		bool runTest(const string&, const uint64_t = 0);

	public:
		lr35902_tests();

		bool run();
	};
}
