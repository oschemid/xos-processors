#pragma once

#include "cpu.h"
#include "types.h"

using namespace xprocessors;

namespace tests
{
	class cpu_tests {
	protected:
		Cpu* cpu;
		uint8_t memory[0x10000];
		bool finished;

		void load(const uint16_t, const string&);

		virtual void out(const uint8_t, const uint8_t) = 0;
		bool runTestCycles(const std::vector<std::pair<uint64_t, uint64_t>>);

		virtual bool runTest(const string&, const uint64_t = 0) = 0;

	public:
		cpu_tests();

		virtual bool run() = 0;
	};
}
