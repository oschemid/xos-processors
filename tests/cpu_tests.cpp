#include "cpu_tests.h"
#include <fstream>
#include <iostream>


tests::cpu_tests::cpu_tests() :
	finished(false) {

}

void tests::cpu_tests::load(const uint16_t offset, const string& filename) {
	std::ifstream ifs(filename, std::ios::binary | std::ios::ate);
	if (!ifs)
		throw std::runtime_error("Unable to read " + filename);
	auto end = ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	auto filesize = std::size_t(end - ifs.tellg());

	if (!ifs.read((char*)memory + offset, filesize))
		throw std::runtime_error("Error during reading " + filename);
}

bool tests::cpu_tests::runTestCycles(const std::vector<std::pair<uint64_t, uint64_t>> opcodes) {
	cpu->in([this](const uint8_t) { return 0; });
	cpu->out([this](const uint8_t, const uint8_t) {});

	for (auto pair : opcodes) {
		auto mem = pair.first;
		cpu->reset();
		memory[0] = (mem >> 24) & 0xff;
		memory[1] = (mem >> 16) & 0xff;
		memory[2] = (mem >> 8) & 0xff;
		memory[3] = mem & 0xff;

		cpu->executeOne();
		if (cpu->elapsed_cycles() != pair.second) {
			std::cout << std::hex << mem << std::dec << " - Expected cycles : " << pair.second << " - Cycles : " << cpu->elapsed_cycles() << std::endl;
		}
	}
	return true;
}