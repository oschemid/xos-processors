#include "z80ng_tests.h"
#include "z80.h"
#include "../src/cpu/z80/z80debugging.h"
#include <iostream>
#include <chrono>
#include "data/z80/perfs.h"


using namespace std::chrono;
using namespace xprocessors::cpu;

namespace tests
{
	struct Z80NG
	{
		Z80 _cpu;
		uint8_t* _memory;

		Z80NG() {
			_cpu.init();
			_memory = new uint8_t[0x10000];
		}
		~Z80NG() {
			delete[] _memory;
		}

		uint8_t executeOne() {
			uint8_t cycles = 0;
			uint8_t instruction = 0;
			uint8_t maxinstruction = ((_memory[0] == 0xed) || (_memory[0] == 0xcb)) ? 3 : 2;
			if ((_memory[0] == 0xdd) || (_memory[0] == 0xfd)) {
				maxinstruction++;
			}
			bool transitionm1 = false;
			_cpu.reset();
			while (instruction < maxinstruction) {
				uint16_t pins = _cpu.getControlPins();
				transitionm1 = ((pins & (Z80::PIN_M1)) == 0) ? true : false;
				_cpu.tick();
				cycles++;
				pins = _cpu.getControlPins();
				if (pins & (Z80::PIN_MREQ | Z80::PIN_RD))
				{
					const uint16_t address = _cpu.getAddressBus();
					_cpu.setDataBus(_memory[address]);
				}
				if (pins & (Z80::PIN_M1)) {
					if (transitionm1)
						instruction++;
				}
			}
			return cycles;
		}
		void test_cycle(uint64_t instruction, uint8_t cycles)
		{
			_memory[0] = instruction >> 24;
			_memory[1] = (instruction >> 16) & 0xff;
			_memory[2] = (instruction >> 8) & 0xff;
			_memory[3] = instruction & 0xff;
			uint8_t result = executeOne() - 1;
			if (result != cycles) {
				std::cout << "Difference : " << std::hex << instruction << std::dec << " - " << uint16_t(cycles) << " expected != " << uint16_t(result) << std::endl;
			}
		}
		void load(const uint16_t offset, const std::string& filename) {
			std::ifstream ifs(filename, std::ios::binary | std::ios::ate);
			if (!ifs)
				throw std::runtime_error("Unable to read " + filename);
			auto end = ifs.tellg();
			ifs.seekg(0, std::ios::beg);
			auto filesize = std::size_t(end - ifs.tellg());

			if (!ifs.read((char*)_memory + offset, filesize))
				throw std::runtime_error("Error during reading " + filename);
		}

		bool runTest(const std::string& filename, const uint64_t cycles_expected) {
			uint64_t cycles = 0;
			load(0x100, filename);
			_memory[0] = 0xD3;
			_memory[1] = 0x00;
			_memory[5] = 0xD3;
			_memory[6] = 0x01;
			_memory[7] = 0xC9;
			_cpu.reset(0x100);

			Z80debugging debugger;
//			debugger.activateDisassembly();

			//auto start = high_resolution_clock::now();
			uint16_t instruction = 0;
			bool finished = false;
			while (!finished) {
//				debugger.debug(&_cpu, _memory);
				_cpu.tick();
				cycles++;
				const uint16_t pins = _cpu.getControlPins();
				if ((pins & (Z80::PIN_MREQ | Z80::PIN_RD)) == (Z80::PIN_MREQ | Z80::PIN_RD))
				{
					const uint16_t address = _cpu.getAddressBus();
					_cpu.setDataBus(_memory[address]);
				}
				if ((pins & (Z80::PIN_MREQ | Z80::PIN_WR)) == (Z80::PIN_MREQ | Z80::PIN_WR))
				{
					const uint16_t address = _cpu.getAddressBus();
					_memory[address] = _cpu.getDataBus();
				}
				if ((pins & (Z80::PIN_IORQ)))
				{
					if ((_cpu.getAddressBus() & 0x00ff) == 0) {
						finished = true;
					}
					if ((_cpu.getAddressBus() & 0x00ff) == 0x0001) {
						uint8_t operation = _cpu.c;

						if (operation == 2) { // print a character stored in E
							std::cout << (char)(_cpu.e);
						}
						else if (operation == 9) { // print from memory at (DE) until '$' char
							uint16_t addr = _cpu.de;
							do {
								std::cout << (char)(_memory[addr++]);
							} while (_memory[addr] != '$');
						}
					}
				}
			}
			//auto stop = high_resolution_clock::now();
			//auto duration = duration_cast<microseconds>(stop - start);
			//std::cout << duration.count() << std::endl;
			if (cycles_expected > 0)
				std::cout << std::endl << "Expected cycles : " << cycles_expected << " - Cycles : " << cycles << "(" << instruction << " instructions)" << std::endl;
			return true;
		}


		bool runTestPerfs(const uint64_t cycles_expected) {
			uint64_t total_cycles = 0;
			uint64_t total_microseconds = 0;
			for (auto [name, mem] : perfs_tests) {
				_cpu.reset();
				uint64_t cycles = 0;
				uint8_t i = 0;
				for (auto m : mem) {
					_memory[i++] = m;
				}
				auto start = high_resolution_clock::now();
				while (cycles < cycles_expected) {
					_cpu.tick();
					cycles++;
					auto pins = _cpu.getControlPins();
					if ((pins & (Z80::PIN_MREQ | Z80::PIN_RD)) == (Z80::PIN_MREQ | Z80::PIN_RD))
					{
						const uint16_t address = _cpu.getAddressBus();
						_cpu.setDataBus(_memory[address]);
					}
					if ((pins & (Z80::PIN_MREQ | Z80::PIN_WR)) == (Z80::PIN_MREQ | Z80::PIN_WR))
					{
						const uint16_t address = _cpu.getAddressBus();
						_memory[address] = _cpu.getDataBus();
					}
					if ((pins & (Z80::PIN_IORQ)))
					{
						if ((_cpu.getAddressBus() & 0xff) == 1) {
							uint8_t operation = _cpu.c;

							if (operation == 2) { // print a character stored in E
								std::cout << (char)(_cpu.e);
							}
							else if (operation == 9) { // print from memory at (DE) until '$' char
								uint16_t addr = _cpu.de;
								do {
									std::cout << (char)(_memory[addr++]);
								} while (_memory[addr] != '$');
							}

						}

					}
				}
				auto stop = high_resolution_clock::now();
				auto duration = duration_cast<microseconds>(stop - start);
				total_cycles += cycles;
				total_microseconds += duration.count();
				std::cout << name << ": " << duration.count() << std::endl;
			}
			std::cout << "total : " << total_cycles / total_microseconds << " MHz" << std::endl;
			return true;
		}
	};

	const std::vector<std::pair<uint64_t, uint64_t>> opcodes = {
		// Group 1 - 8-Bit Load Group
		{ 0x40000000, 4 }, { 0xdd400000, 8 }, { 0xfd400000, 8 }, // LD r,r
		{ 0x06000000, 7 }, { 0xdd060000, 11 }, { 0xfd060000, 11 }, // LD r,n
		{ 0x46000000, 7 }, { 0xdd460100, 19 }, { 0xfd460100, 19 }, // LD r,(hl)
		{ 0x70000000, 7 }, { 0xdd700300, 19 }, { 0xfd700300, 19 }, // LD (hl),r
		{ 0x36300000, 10 }, { 0xdd360000, 19 }, { 0xfd361010, 19 }, // LD (hl),n
		{ 0x0a000000, 7 }, { 0x1a000000, 7 }, { 0x3A345600, 13 }, // LD A,(dd)
		{ 0x02000000, 7 }, { 0x12000000, 7 }, { 0x32000000, 13 }, // LD (dd),A
//		{ 0xed570000, 9 }, { 0xed5f0000, 9 }, { 0xed470000, 9 }, { 0xed4f0000, 9 }, // LD A,I LD A,R

		// Group 2 - 16-Bit Load Group
		{ 0x01000000, 10 }, { 0xdd210000, 14 }, { 0xfd210000, 14 }, // LD ss, nn
		{ 0x2a000000, 16 }, /*{0xed4b0000, 20}, */ {0xdd2a0000, 20}, {0xfd2a0000, 20}, // Ld ss, (nn)
		{ 0x22000000, 16 }, {0xed430000, 20}, { 0xdd220000, 20 }, {0xfd220000, 20}, // LD (nn), ss
		{ 0xf9000000, 6 }, { 0xddf90000, 10 }, { 0xfdf90000, 10 }, // LD SP, HL
		{ 0xe5000000, 11 }, { 0xdde50000, 15 }, { 0xfde50000, 15 }, // PUSH
		{ 0xe1000000, 10 }, { 0xdde10000, 14 }, { 0xfde10000, 14 }, // POP

		// Group 3 - Exchange, Block Transfer, Search Group
		{ 0xeb000000, 4 }, { 0x08000000, 4 }, { 0xd9000000, 4 }, {0xe3000000, 19 }, { 0xdde30000, 23 }, { 0xfde30000, 23 }, // EX
		{ 0xeda00000, 16 }, {0xedb00000, 21 }, {0xeda80000, 16}, {0xedb80000, 21}, {0xeda10000, 16}, {0xedb10000, 21}, {0xeda90000, 16}, {0xedb90000, 21}, // LDI, LDD, CPI, CPD

		// Group 4 - 8-Bit Arithmetic and Logical Group
		{ 0x80000000, 4 }, { 0x86000000, 7 }, { 0xdd800000, 8 }, {0xfd800000, 8 }, {0xc6000000, 7 }, { 0xdd860000, 19 }, { 0xfd860000, 19 }, // ADD
		{ 0x88000000, 4 }, { 0x8e000000, 7 }, { 0xdd880000, 8 }, {0xfd880000, 8 }, {0xce000000, 7 }, { 0xdd8e0000, 19 }, { 0xfd8e0000, 19 }, // ADC
		{ 0x90000000, 4 }, { 0x96000000, 7 }, { 0xdd900000, 8 }, {0xfd900000, 8 }, {0xd6000000, 7 }, { 0xdd960000, 19 }, { 0xfd960000, 19 }, // SUB
		{ 0x98000000, 4 }, { 0x9e000000, 7 }, { 0xdd980000, 8 }, {0xfd980000, 8 }, {0xde000000, 7 }, { 0xdd9e0000, 19 }, { 0xfd9e0000, 19 }, // SBC
		{ 0xa0000000, 4 }, { 0xa6000000, 7 }, { 0xdda00000, 8 }, {0xfda00000, 8 }, {0xe6000000, 7 }, { 0xdda60000, 19 }, { 0xfda60000, 19 }, // AND
		{ 0xb0000000, 4 }, { 0xb6000000, 7 }, { 0xddb00000, 8 }, {0xfdb00000, 8 }, {0xf6000000, 7 }, { 0xddb60000, 19 }, { 0xfdb60000, 19 }, // OR
		{ 0xa8000000, 4 }, { 0xae000000, 7 }, { 0xdda80000, 8 }, {0xfda80000, 8 }, {0xee000000, 7 }, { 0xddae0000, 19 }, { 0xfdae0000, 19 }, // XOR
		{ 0xb8000000, 4 }, { 0xbe000000, 7 }, { 0xddb80000, 8 }, {0xfdb80000, 8 }, {0xfe000000, 7 }, { 0xddbe0000, 19 }, { 0xfdbe0000, 19 }, // CP
		{ 0x04000000, 4 }, { 0xdd040000, 8 }, { 0xfd040000, 8 }, {0x34000000, 11 }, {0xdd340000, 23 }, { 0xfd340000, 23 }, // INC
		{ 0x05000000, 4 }, { 0xdd050000, 8 }, { 0xfd050000, 8 }, {0x35000000, 11 }, {0xdd350000, 23 }, { 0xfd350000, 23 }, // DEC

		// Group 5 - General-Purpose Arithmetic and CPU Control Group
		{ 0x27000000, 4 }, {0xed440000, 8}, // DAA, NEG
		{ 0x2f000000, 4 }, { 0x37000000, 4 }, { 0x3f000000, 4 }, // CPL, SCF, CCF
		{ 0x00000000, 4 }, /*{0x76000000, 4},*/ {0xf3000000, 4}, {0xfb000000, 4}, // NOP, HALT, DI, EI
//		{ 0xed460000, 8 }, { 0xed560000, 8 }, { 0xed5e0000, 8 }, // IM
		// Group 6 - 16-Bit Arithmetic Group
		{ 0x09000000, 11 }, { 0xdd190000, 15 }, { 0xfd290000, 15 }, // ADD
		{ 0xed4a0000, 15 }, { 0xed420000, 15 }, // ADC, SBC
		{ 0x03000000, 6 }, { 0xdd230000, 10 }, {0xfd230000, 10 }, // INC
		{ 0x0b000000, 6 }, { 0xdd2b0000, 10 }, {0xfd2b0000, 10 }, // DEC

		// Group 7 - Rotate and Shift Group
		{ 0x07000000, 4}, { 0x0f000000, 4 }, { 0x17000000, 4 }, { 0x1f000000, 4 }, // RLCA, RLA, RRCA, RRA
		{ 0xcb000000, 8 }, { 0xcb060000, 15 }, { 0xddcb0006, 23 }, { 0xfdcb0006, 23 }, { 0xddcb0001, 23 }, { 0xfdcb0002, 23 }, // RLC
		{ 0xcb100000, 8 }, { 0xcb160000, 15 }, { 0xddcb0016, 23 }, { 0xfdcb0016, 23 }, { 0xddcb0011, 23 }, { 0xfdcb0012, 23 }, // RL
		{ 0xcb080000, 8 }, { 0xcb0e0000, 15 }, { 0xddcb000e, 23 }, { 0xfdcb000e, 23 }, { 0xddcb0009, 23 }, { 0xfdcb000a, 23 }, // RRC
		{ 0xcb180000, 8 }, { 0xcb1e0000, 15 }, { 0xddcb001e, 23 }, { 0xfdcb001e, 23 }, { 0xddcb0019, 23 }, { 0xfdcb001a, 23 }, // RR
		{ 0xcb200000, 8 }, { 0xcb260000, 15 }, { 0xddcb0026, 23 }, { 0xfdcb0026, 23 }, { 0xddcb0021, 23 }, { 0xfdcb0022, 23 }, // SLA
		{ 0xcb300000, 8 }, { 0xcb360000, 15 }, { 0xddcb0036, 23 }, { 0xfdcb0036, 23 }, { 0xddcb0031, 23 }, { 0xfdcb0032, 23 }, // SLL
		{ 0xcb280000, 8 }, { 0xcb2e0000, 15 }, { 0xddcb002e, 23 }, { 0xfdcb002e, 23 }, { 0xddcb0029, 23 }, { 0xfdcb002a, 23 }, // SRA
		{ 0xcb380000, 8 }, { 0xcb3e0000, 15 }, { 0xddcb003e, 23 }, { 0xfdcb003e, 23 }, { 0xddcb0039, 23 }, { 0xfdcb003a, 23 }, // SRL
		{ 0xed670000, 18 }, { 0xed6f0000, 18 }, // RRD, RLD

		// Group 8 - Bit Set, Reset and Test Group
		{ 0xcb400000, 8 }, { 0xcb460000, 12 }, { 0xddcb0046, 20 }, { 0xfdcb0046, 20 }, // BIT
		{ 0xcbc00000, 8 }, { 0xcbc60000, 15 }, { 0xddcb00c6, 23 }, { 0xfdcb00c6, 23 }, { 0xddcb00c1, 23 }, { 0xfdcb00c2, 23 }, // SET
		{ 0xcb800000, 8 }, { 0xcb860000, 15 }, { 0xddcb0086, 23 }, { 0xfdcb0086, 23 }, { 0xddcb0081, 23 }, { 0xfdcb0082, 23 }, // RES

		// Group 9 - Jump Group
		{ 0xc3100000, 10 }, { 0xe9000000, 4 }, { 0xdde90000, 8 }, { 0xfde90000, 8 }, { 0xf2100000, 10 }, // JP
		{ 0xc2100000, 10 }, { 0xca100000, 10 }, // JP CC
		{ 0x18010000, 12 }, { 0x20010000, 12 }, { 0x28010000, 7 }, { 0x10010000, 13 }, // JR, DJNZ

		// Group 10 - Call and Return Group
		{ 0xcd145000, 17 }, { 0xc4145000, 17 }, { 0xcc145000, 10 }, // CALL
		{ 0xc9000000, 10 }, { 0xc0000000, 11 }, { 0xc8000000, 5 }, /*{0xed450000, 14}, {0xed4d0000, 14},*/ // RET
		{ 0xc7000000, 11 }, // RST

		// Group 11 -  Input and Output Group
		{ 0xdb010000, 11 }, {0xed400000, 12}, {0xed700000, 12}, // IN
//		{ 0xeda20000, 16 }, { 0xedb20000, 21 }, // INI
//		{ 0xedaa0000, 16 }, { 0xedba0000, 21 }, // IND
		{ 0xd3010000, 11 }, /*{0xed410000, 12}, {0xed710000, 12},*/ // OUT
//		{ 0xeda30000, 16 }, { 0xedb30000, 21 }, // OUTI
//		{ 0xedab0000, 16 }, { 0xedbb0000, 21 }, // OUTD
	};
	bool z80_run()
	{
		Z80NG cpu;
		cpu.runTest("tests/data/Z80/prelim.com", 8721Ui64);
//		cpu.runTestPerfs(1000000000);
		cpu.runTest("tests/data/Z80/zexdoc.com", 46734978649Ui64);
		std::cout << "check cycles" << std::endl;
		for (auto [o,oo] : opcodes) {
			cpu.test_cycle(o, oo);
		}

		return true;
	}
}