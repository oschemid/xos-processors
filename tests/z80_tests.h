#pragma once

#include "cpu.h"
#include "types.h"

using namespace xprocessors;

namespace tests
{
	class z80_tests {
	protected:
		Cpu* cpu;
		uint8_t memory[0x10000];
		bool finished;

		void out(const uint8_t, const uint8_t);
		void load(const uint16_t, const string&);
		bool runTest(const string&, const uint64_t = 0);
		bool runTestCycles();

	public:
		z80_tests();

		bool run();
	};
}