#pragma once

#include "cpu_tests.h"


using namespace xprocessors;

namespace tests
{
	class lr35902_tests : public cpu_tests {
	protected:
		bool runTest(const string&, const uint64_t = 0);

		void write(const uint16_t, const uint8_t);
		const uint8_t read(const uint16_t);
		void out(const uint8_t, const uint8_t);
		void timer(const uint64_t);

		uint8_t selectedROMbanks;
		uint8_t memory2[0x10000 - 0x8000];
		uint8_t c;
		uint64_t previous_cycles;
	public:
		lr35902_tests();

		bool run();
	};
}
