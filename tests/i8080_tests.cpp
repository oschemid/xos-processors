#include "i8080_tests.h"
#include "../src/z80family/i8080.h"
#include <iostream>
#include <fstream>


tests::i8080_tests::i8080_tests() {
	cpu = Cpu::create("i8080");
	cpu->read([this](const uint16_t p) { return memory[p]; });
	cpu->write([this](const uint16_t p, const uint8_t v) { memory[p] = v; return true; });
}

void tests::i8080_tests::load(const uint16_t offset, const string& filename) {
	std::ifstream ifs(filename, std::ios::binary | std::ios::ate);
	if (!ifs)
		throw std::runtime_error("Unable to read " + filename);
	auto end = ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	auto filesize = std::size_t(end - ifs.tellg());

	if (!ifs.read((char*)memory + offset, filesize))
		throw std::runtime_error("Error during reading " + filename);
}

void tests::i8080_tests::out(const uint8_t p, const uint8_t v) {
	if (p == 0) {
		finished = true;
		return;
	}
	if (p == 1) {
		auto cpuc = static_cast<xprocessors::Intel8080*>(cpu);
		uint8_t operation = cpuc->c();

		if (operation == 2) { // print a character stored in E
			std::cout << (char)(cpuc->e());
		}
		else if (operation == 9) { // print from memory at (DE) until '$' char
			uint16_t addr = cpuc->de();
			do {
				std::cout << (char)(memory[addr++]);
			} while (memory[addr] != '$');
		}

	}
}
bool tests::i8080_tests::runTestCycles() {
	const std::pair<uint64_t, uint64_t> opcodes[] = {
		// Group 1 - 8-Bit Load Group
		{ 0x40000000, 5 }, // LD r,r
		{ 0x06000000, 7 }, // LD r,n
		{ 0x46000000, 7 }, // LD r,(hl)
		{ 0x70000000, 7 }, // LD (hl),r
		{ 0x36300000, 10 }, // LD (hl),n
		//{ 0x0a000000, 7 }, { 0x1a000000, 7 }, { 0x3A345600, 13 }, // LD A,(dd)
		//{ 0x02000000, 7 }, { 0x12000000, 7 }, { 0x32000000, 13 }, // LD (dd),A

		//// Group 2 - 16-Bit Load Group
		//{ 0x01000000, 10 }, // LD ss, nn
		//{ 0x2a000000, 16 }, { 0xed4b0000, 20 }, { 0xdd2a0000, 20 }, { 0xfd2a0000, 20 }, // Ld ss, (nn)
		//{ 0x22000000, 16 }, { 0xed430000, 20 }, { 0xdd220000, 20 }, { 0xfd220000, 20 }, // LD (nn), ss
		//{ 0xf9000000, 6 }, { 0xddf90000, 10 }, { 0xfdf90000, 10 }, // LD SP, HL
		//{ 0xe5000000, 11 }, { 0xdde50000, 15 }, { 0xfde50000, 15 }, // PUSH
		//{ 0xe1000000, 10 }, { 0xdde10000, 14 }, { 0xfde10000, 14 }, // POP

		//// Group 3 - Exchange, Block Transfer, Search Group
		//{ 0xeb000000, 4 }, { 0x08000000, 4 }, { 0xd9000000, 4 }, {0xe3000000, 19 }, { 0xdde30000, 23 }, { 0xfde30000, 23 }, // EX
		//{ 0xeda00000, 16 }, {0xedb00000, 21 }, { 0xeda80000, 16 }, {0xedb80000, 21 }, { 0xeda10000, 16 }, {0xedb10000, 21 }, { 0xeda90000, 16 }, { 0xedb90000, 21 }, // LDI, LDD, CPI, CPD

		//// Group 4 - 8-Bit Arithmetic and Logical Group
		//{ 0x80000000, 4 }, { 0x86000000, 7 }, { 0xdd800000, 8 }, {0xfd800000, 8 }, {0xc6000000, 7 }, { 0xdd860000, 19 }, { 0xfd860000, 19 }, // ADD
		//{ 0x88000000, 4 }, { 0x8e000000, 7 }, { 0xdd880000, 8 }, {0xfd880000, 8 }, {0xce000000, 7 }, { 0xdd8e0000, 19 }, { 0xfd8e0000, 19 }, // ADC
		//{ 0x90000000, 4 }, { 0x96000000, 7 }, { 0xdd900000, 8 }, {0xfd900000, 8 }, {0xd6000000, 7 }, { 0xdd960000, 19 }, { 0xfd960000, 19 }, // SUB
		//{ 0x98000000, 4 }, { 0x9e000000, 7 }, { 0xdd980000, 8 }, {0xfd980000, 8 }, {0xde000000, 7 }, { 0xdd9e0000, 19 }, { 0xfd9e0000, 19 }, // SBC
		//{ 0xa0000000, 4 }, { 0xa6000000, 7 }, { 0xdda00000, 8 }, {0xfda00000, 8 }, {0xe6000000, 7 }, { 0xdda60000, 19 }, { 0xfda60000, 19 }, // AND
		//{ 0xb0000000, 4 }, { 0xb6000000, 7 }, { 0xddb00000, 8 }, {0xfdb00000, 8 }, {0xf6000000, 7 }, { 0xddb60000, 19 }, { 0xfdb60000, 19 }, // OR
		//{ 0xa8000000, 4 }, { 0xae000000, 7 }, { 0xdda80000, 8 }, {0xfda80000, 8 }, {0xee000000, 7 }, { 0xddae0000, 19 }, { 0xfdae0000, 19 }, // XOR
		//{ 0xb8000000, 4 }, { 0xbe000000, 7 }, { 0xddb80000, 8 }, {0xfdb80000, 8 }, {0xfe000000, 7 }, { 0xddbe0000, 19 }, { 0xfdbe0000, 19 }, // CP
		{ 0x04000000, 5 }, {0x34000000, 10 }, // INC
		{ 0x05000000, 5 }, {0x35000000, 10 }, // DEC

		//// Group 5 - General-Purpose Arithmetic and CPU Control Group
		//{ 0x27000000, 4 }, { 0xed440000, 8 }, // DAA, NEG
		//{ 0x2f000000, 4 }, { 0x37000000, 4 }, { 0x3f000000, 4 }, // CPL, SCF, CCF
		//{ 0x00000000, 4 }, { 0x76000000, 4 }, { 0xf3000000, 4 }, { 0xfb000000, 4 }, // NOP, HALT, DI, EI
		//{ 0xed460000, 8 }, { 0xed560000, 8 }, { 0xed5e0000, 8 }, // IM
		//// Group 6 - 16-Bit Arithmetic Group
		//{ 0x09000000, 11 }, { 0xdd190000, 15 }, { 0xfd290000, 15 }, // ADD
		//{ 0xed4a0000, 15 }, { 0xed420000, 15 }, // ADC, SBC
		//{ 0x03000000, 6 }, { 0xdd230000, 10 }, {0xfd230000, 10 }, // INC
		//{ 0x0b000000, 6 }, { 0xdd2b0000, 10 }, {0xfd2b0000, 10 }, // DEC

		//// Group 7 - Rotate and Shift Group
		//{ 0x07000000, 4}, { 0x0f000000, 4 }, { 0x17000000, 4 }, { 0x1f000000, 4 }, // RLCA, RLA, RRCA, RRA
		//{ 0xcb000000, 8 }, { 0xcb060000, 15 }, { 0xddcb0006, 23 }, { 0xfdcb0006, 23 }, { 0xddcb0001, 23 }, { 0xfdcb0002, 23 }, // RLC
		//{ 0xcb100000, 8 }, { 0xcb160000, 15 }, { 0xddcb0016, 23 }, { 0xfdcb0016, 23 }, { 0xddcb0011, 23 }, { 0xfdcb0012, 23 }, // RL
		//{ 0xcb080000, 8 }, { 0xcb0e0000, 15 }, { 0xddcb000e, 23 }, { 0xfdcb000e, 23 }, { 0xddcb0009, 23 }, { 0xfdcb000a, 23 }, // RRC
		//{ 0xcb180000, 8 }, { 0xcb1e0000, 15 }, { 0xddcb001e, 23 }, { 0xfdcb001e, 23 }, { 0xddcb0019, 23 }, { 0xfdcb001a, 23 }, // RR
		//{ 0xcb200000, 8 }, { 0xcb260000, 15 }, { 0xddcb0026, 23 }, { 0xfdcb0026, 23 }, { 0xddcb0021, 23 }, { 0xfdcb0022, 23 }, // SLA
		//{ 0xcb300000, 8 }, { 0xcb360000, 15 }, { 0xddcb0036, 23 }, { 0xfdcb0036, 23 }, { 0xddcb0031, 23 }, { 0xfdcb0032, 23 }, // SLL
		//{ 0xcb280000, 8 }, { 0xcb2e0000, 15 }, { 0xddcb002e, 23 }, { 0xfdcb002e, 23 }, { 0xddcb0029, 23 }, { 0xfdcb002a, 23 }, // SRA
		//{ 0xcb380000, 8 }, { 0xcb3e0000, 15 }, { 0xddcb003e, 23 }, { 0xfdcb003e, 23 }, { 0xddcb0039, 23 }, { 0xfdcb003a, 23 }, // SRL
		//{ 0xed670000, 18 }, { 0xed6f0000, 18 }, // RRD, RLD

		//// Group 8 - Bit Set, Reset and Test Group
		//{ 0xcb400000, 8 }, { 0xcb460000, 12 }, { 0xddcb0046, 20 }, { 0xfdcb0046, 20 }, // BIT
		//{ 0xcbc00000, 8 }, { 0xcbc60000, 15 }, { 0xddcb00c6, 23 }, { 0xfdcb00c6, 23 }, { 0xddcb00c1, 23 }, { 0xfdcb00c2, 23 }, // SET
		//{ 0xcb800000, 8 }, { 0xcb860000, 15 }, { 0xddcb0086, 23 }, { 0xfdcb0086, 23 }, { 0xddcb0081, 23 }, { 0xfdcb0082, 23 }, // RES

		//// Group 9 - Jump Group
		//{ 0xc3100000, 10 }, { 0xe9000000, 4 }, { 0xdde90000, 8 }, { 0xfde90000, 8 }, { 0xf2100000, 10 }, // JP
		//{ 0x18010000, 12 }, { 0x20010000, 12 }, { 0x28010000, 7 }, { 0x10010000, 13 }, // JR, DJNZ

		//// Group 10 - Call and Return Group
		//{ 0xcd145000, 17 }, { 0xc4145000, 17 }, { 0xcc145000, 10 }, // CALL
		//{ 0xc9000000, 10 }, { 0xc0000000, 11 }, { 0xc8000000, 5 }, { 0xed450000, 14 }, { 0xed4d0000, 14 }, // RET
		//{ 0xc7000000, 11 }, // RST

		//// Group 11 -  Input and Output Group
		//{ 0xdb010000, 11 }, { 0xed400000, 12 }, { 0xed700000, 12 }, // IN
		//{ 0xeda20000, 16 }, { 0xedb20000, 21 }, // INI
		//{ 0xedaa0000, 16 }, { 0xedba0000, 21 }, // IND
		//{ 0xd3010000, 11 }, { 0xed410000, 12 }, { 0xed710000, 12 }, // OUT
		//{ 0xeda30000, 16 }, { 0xedb30000, 21 }, // OUTI
		//{ 0xedab0000, 16 }, { 0xedbb0000, 21 }, // OUTD
	};

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
bool tests::i8080_tests::runTest(const string& filename, const uint64_t cycles_expected) {
	uint64_t cycles = 0;
	load(0x100, filename);
	memory[0] = 0xD3;
	memory[1] = 0x00;
	memory[5] = 0xD3;
	memory[6] = 0x01;
	memory[7] = 0xC9;
	cpu->reset(0x100);
	cpu->in([](const uint8_t) { return 0; });
	cpu->out([this](const uint8_t p, const uint8_t v) { out(p, v); });

	finished = false;

	while (!finished) {
		cycles += cpu->executeOne();
	}
	if (cycles_expected > 0)
		std::cout << std::endl << "Expected cycles : " << cycles_expected << " - Cycles : " << cpu->elapsed_cycles();
	return true;
}

bool tests::i8080_tests::run() {
	std::cout << "Check cycles" << std::endl;
	runTestCycles();
	std::cout << std::endl;
	std::cout << "TST8080.COM" << std::endl;
	runTest("tests/data/i8080/TST8080.COM", 4924);
	std::cout << std::endl;
	std::cout << "8080PRE.COM" << std::endl;
	runTest("tests/data/i8080/8080PRE.COM", 7817);
	std::cout << std::endl;
	std::cout << "8080EXM.COM" << std::endl;
	runTest("tests/data/i8080/8080EXM.COM", 23803381171);
	std::cout << std::endl;
	std::cout << "CPUTEST.COM" << std::endl;
	runTest("tests/data/i8080/CPUTEST.COM", 255653383);
	std::cout << std::endl;
	return true;
}