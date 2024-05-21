#include "i8080ng_tests.h"
#include "i8080.h"
#include "../src/cpu/z80/z80debugging.h"
#include <iostream>
#include <chrono>


using namespace std::chrono;
using namespace xprocessors::cpu;

namespace tests
{
	struct i8080NG
	{
		i8080dbg _cpu;
		uint8_t* _memory;

		i8080NG()
		{
			_cpu.setMemoryAccessor([this](uint16_t a) { return _memory[a]; });
			_cpu.init();
			_memory = new uint8_t[0x10000];
		}
		~i8080NG()
		{
			delete[] _memory;
		}

		uint8_t executeOne()
		{
			uint8_t cycles = 0;
			uint8_t instruction = 0;
			uint8_t maxinstruction = 2;
			bool transitionm1 = false;
			_cpu.reset();
			_cpu.setReady();
			while (instruction < maxinstruction)
			{
				uint16_t pins = _cpu.getControlPins();
				uint8_t databus = _cpu.getDataBus();
				if (pins & i8080::PIN_SYNC)
				{
					if (databus & i8080::STATUS_HLTA)
						instruction++;
					if (databus & i8080::STATUS_M1)
						instruction++;
					if (instruction == 2)
						break;
				}
				_cpu.tick();
				cycles++;
				pins = _cpu.getControlPins();
				if (pins & i8080::PIN_DBIN)
				{
					const uint16_t address = _cpu.getAddressBus();
					_cpu.setDataBus(_memory[address]);
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
			std::cout << std::hex << instruction << std::dec << " - ";
			uint8_t result = executeOne() - 1;
			if (result != cycles)
			{
				 std::cout << "Difference : " << uint16_t(cycles) << " expected != " << uint16_t(result) << std::endl;
			}
			else
			{
				std::cout << "OK" << std::endl;
			}
		}
		void load(const uint16_t offset, const std::string& filename)
		{
			std::ifstream ifs(filename, std::ios::binary | std::ios::ate);
			if (!ifs)
				throw std::runtime_error("Unable to read " + filename);
			auto end = ifs.tellg();
			ifs.seekg(0, std::ios::beg);
			auto filesize = std::size_t(end - ifs.tellg());

			if (!ifs.read((char*)_memory + offset, filesize))
				throw std::runtime_error("Error during reading " + filename);
		}

		bool runTest(const std::string& filename, const uint64_t cycles_expected)
		{
			uint64_t cycles = 0;
			load(0x100, filename);
			_memory[0] = 0xD3;
			_memory[1] = 0x00;
			_memory[5] = 0xD3;
			_memory[6] = 0x01;
			_memory[7] = 0xC9;
			_cpu.reset(0x100);
			_cpu.setReady();

			Z80debugging debugger;
			debugger.activateDisassembly();

			auto start = high_resolution_clock::now();
			uint32_t instruction = 0;
			bool finished = false;
			bool ioexpected = false;
//			_cpu.pause();
//			std::ofstream stream("log.log");
			while (!finished)
			{
				if (_cpu.isrunning())
				{
					_cpu.tick();
					cycles++;
					const uint16_t pins = _cpu.getControlPins();
					if (pins & i8080::PIN_SYNC)
					{
						if (_cpu.getDataBus() & i8080::STATUS_M1)
							instruction++;
						ioexpected = (_cpu.getDataBus() & i8080::STATUS_OUT);
					}
					if ((pins & i8080::PIN_DBIN))
					{
						const uint16_t address = _cpu.getAddressBus();
						_cpu.setDataBus(_memory[address]);
					}
					if ((pins & i8080::PIN_WR))
					{
						const uint16_t address = _cpu.getAddressBus();
						if (ioexpected)
						{
							if ((_cpu.getAddressBus() & 0x00ff) == 0)
							{
								cycles++; // STOPWRITE
								finished = true;
							}
							if ((_cpu.getAddressBus() & 0x00ff) == 0x0001)
							{
								uint8_t operation = _cpu.getc();
								if (operation == 2)
								{ // print a character stored in E
									std::cout << (char)(_cpu.gete());
								}
								else if (operation == 9)
								{ // print from memory at (DE) until '$' char
									uint16_t addr = _cpu.getde();
									do
									{
										std::cout << (char)(_memory[addr++]);
									} while (_memory[addr] != '$');
								}
							}
						}
						else
						{
							_memory[address] = _cpu.getDataBus();
						}
					}
				}
				else
				{
//					std::cout << _cpu.opcode() << std::endl;
//					stream << _cpu.opcode() << std::endl;
					_cpu.runStep();
				}
			}
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<milliseconds>(stop - start);
			std::cout << std::endl << "Duration : " << duration.count() << "ms" << std::endl;
			if (cycles_expected > 0)
				std::cout << "Expected cycles : " << cycles_expected << " - Cycles : " << cycles << "(" << instruction << " instructions)" << std::endl;
			return true;
		}
	};

	const std::vector<std::pair<uint64_t, uint64_t>> opcodes = {
		{ 0x00000000, 4 }, { 0x01000000, 10 }, { 0x02000000, 7 }, { 0x03000000, 5 }, { 0x04000000, 5 }, { 0x05000000, 5 }, { 0x06000000, 7 }, { 0x07000000, 4 },
		{ 0x09000000, 10 }, { 0x0a000000, 7 }, { 0x0b000000, 5 }, { 0x0c000000, 5 }, { 0x0d000000, 5 }, { 0x0e000000, 7 }, { 0x0f000000, 4 },
		{ 0x11000000, 10 }, { 0x12000000, 7 }, { 0x13000000, 5 }, { 0x14000000, 5 }, { 0x15000000, 5 }, { 0x16000000, 7 }, { 0x17000000, 4 },
		{ 0x19000000, 10 }, { 0x1a000000, 7 }, { 0x1b000000, 5 }, { 0x1c000000, 5 }, { 0x1d000000, 5 }, { 0x1e000000, 7 }, { 0x1f000000, 4 },
		{ 0x21000000, 10 }, { 0x22000000, 16 }, { 0x23000000, 5 }, { 0x24000000, 5 }, { 0x25000000, 5 }, { 0x26000000, 7 }, { 0x27000000, 4 },
		{ 0x29000000, 10 }, { 0x2a000000, 16 }, { 0x2b000000, 5 }, { 0x2c000000, 5 }, { 0x2d000000, 5 }, { 0x2e000000, 7 }, { 0x2f000000, 4 },
		{ 0x31000000, 10 }, { 0x32000000, 13 }, { 0x33000000, 5 }, { 0x34000000, 10 }, { 0x35000000, 10 }, { 0x36000000, 10 }, { 0x37000000, 4 },
		{ 0x39000000, 10 }, { 0x3a000000, 13 }, { 0x3b000000, 5 }, { 0x3c000000, 5 }, { 0x3d000000, 5 }, { 0x3e000000, 7 }, { 0x3f000000, 4 },
		{ 0x40000000, 5 }, { 0x41000000, 5 }, { 0x42000000, 5 }, { 0x43000000, 5 }, { 0x44000000, 5 }, { 0x45000000, 5 }, { 0x46000000, 7 }, { 0x47000000, 5},
		{ 0x48000000, 5 }, { 0x49000000, 5 }, { 0x4a000000, 5 }, { 0x4b000000, 5 }, { 0x4c000000, 5 }, { 0x4d000000, 5 }, { 0x4e000000, 7 }, { 0x4f000000, 5},
		{ 0x50000000, 5 }, { 0x51000000, 5 }, { 0x52000000, 5 }, { 0x53000000, 5 }, { 0x54000000, 5 }, { 0x55000000, 5 }, { 0x56000000, 7 }, { 0x57000000, 5},
		{ 0x58000000, 5 }, { 0x59000000, 5 }, { 0x5a000000, 5 }, { 0x5b000000, 5 }, { 0x5c000000, 5 }, { 0x5d000000, 5 }, { 0x5e000000, 7 }, { 0x5f000000, 5},
		{ 0x60000000, 5 }, { 0x61000000, 5 }, { 0x62000000, 5 }, { 0x63000000, 5 }, { 0x64000000, 5 }, { 0x65000000, 5 }, { 0x66000000, 7 }, { 0x67000000, 5},
		{ 0x68000000, 5 }, { 0x69000000, 5 }, { 0x6a000000, 5 }, { 0x6b000000, 5 }, { 0x6c000000, 5 }, { 0x6d000000, 5 }, { 0x6e000000, 7 }, { 0x6f000000, 5},
		{ 0x70000000, 7 }, { 0x71000000, 7 }, { 0x72000000, 7 }, { 0x73000000, 7 }, { 0x74000000, 7 }, { 0x75000000, 7 }, { 0x76000000, 7 }, { 0x77000000, 7},
		{ 0x78000000, 5 }, { 0x79000000, 5 }, { 0x7a000000, 5 }, { 0x7b000000, 5 }, { 0x7c000000, 5 }, { 0x7d000000, 5 }, { 0x7e000000, 7 }, { 0x7f000000, 5},
		{ 0x80000000, 4 }, { 0x81000000, 4 }, { 0x82000000, 4 }, { 0x83000000, 4 }, { 0x84000000, 4 }, { 0x85000000, 4 }, { 0x86000000, 7 }, { 0x87000000, 4},
		{ 0x88000000, 4 }, { 0x89000000, 4 }, { 0x8a000000, 4 }, { 0x8b000000, 4 }, { 0x8c000000, 4 }, { 0x8d000000, 4 }, { 0x8e000000, 7 }, { 0x8f000000, 4},
		{ 0x90000000, 4 }, { 0x91000000, 4 }, { 0x92000000, 4 }, { 0x93000000, 4 }, { 0x94000000, 4 }, { 0x95000000, 4 }, { 0x96000000, 7 }, { 0x97000000, 4},
		{ 0x98000000, 4 }, { 0x99000000, 4 }, { 0x9a000000, 4 }, { 0x9b000000, 4 }, { 0x9c000000, 4 }, { 0x9d000000, 4 }, { 0x9e000000, 7 }, { 0x9f000000, 4},
		{ 0xa0000000, 4 }, { 0xa1000000, 4 }, { 0xa2000000, 4 }, { 0xa3000000, 4 }, { 0xa4000000, 4 }, { 0xa5000000, 4 }, { 0xa6000000, 7 }, { 0xa7000000, 4},
		{ 0xa8000000, 4 }, { 0xa9000000, 4 }, { 0xaa000000, 4 }, { 0xab000000, 4 }, { 0xac000000, 4 }, { 0xad000000, 4 }, { 0xae000000, 7 }, { 0xaf000000, 4},
		{ 0xb0000000, 4 }, { 0xb1000000, 4 }, { 0xb2000000, 4 }, { 0xb3000000, 4 }, { 0xb4000000, 4 }, { 0xb5000000, 4 }, { 0xb6000000, 7 }, { 0xb7000000, 4},
		{ 0xb8000000, 4 }, { 0xb9000000, 4 }, { 0xba000000, 4 }, { 0xbb000000, 4 }, { 0xbc000000, 4 }, { 0xbd000000, 4 }, { 0xbe000000, 7 }, { 0xbf000000, 4},
		{ 0xc0000000, 11 }, { 0xc1000000, 10 }, { 0xc2000000, 10 }, { 0xc3000000, 10 }, { 0xc4000000, 17 }, { 0xc5000000, 11 }, { 0xc6000000, 7 }, { 0xc7000000, 11},
		{ 0xc8000000, 5 }, { 0xc9000000, 10 }, { 0xca000000, 10 }, { 0xcc000000, 11 }, { 0xcd000000, 17 }, { 0xce000000, 7 }, { 0xcf000000, 11},
		{ 0xd0000000, 11 }, { 0xd1000000, 10 }, { 0xd2000000, 10 }, { 0xd3000000, 10 }, { 0xd4000000, 17 }, { 0xd5000000, 11 }, { 0xd6000000, 7 }, { 0xd7000000, 11},
		{ 0xd8000000, 5 }, { 0xda000000, 10 }, { 0xdb000000, 10}, { 0xdc000000, 11 }, {0xde000000, 7}, { 0xdf000000, 11},
		{ 0xe0000000, 11 }, { 0xe1000000, 10 }, { 0xe2000000, 10 }, { 0xe3000000, 18 }, { 0xe4000000, 17 }, { 0xe5000000, 11 }, { 0xe6000000, 7 }, { 0xe7000000, 11},
		{ 0xe8000000, 5 }, { 0xe9000000, 5 }, { 0xea000000, 10 }, { 0xeb000000, 4}, { 0xec000000, 11 }, {0xee000000, 7}, { 0xdf000000, 11},
		{ 0xf0000000, 11 }, { 0xf1000000, 10 }, { 0xf2000000, 10 }, { 0xf3000000, 4 }, { 0xf4000000, 17 }, { 0xf5000000, 11 }, { 0xf6000000, 7 }, { 0xf7000000, 11},
		{ 0xf8000000, 5 }, { 0xf9000000, 5 }, { 0xfa000000, 10 }, { 0xfb000000, 4}, { 0xfc000000, 11 }, {0xfe000000, 7}, { 0xff000000, 11},
	};

	bool i8080_run()
	{
		i8080NG cpu;
		cpu.runTest("tests/data/i8080/TST8080.COM", 4924);
		cpu.runTest("tests/data/i8080/8080PRE.COM", 7817);
		cpu.runTest("tests/data/i8080/CPUTEST.COM", 255653383);
		cpu.runTest("tests/data/i8080/8080EXM.COM", 23803381171);
		//		cpu.runTestPerfs(1000000000);
		std::cout << "check cycles" << std::endl;
		for (auto [o, oo] : opcodes)
		{
			cpu.test_cycle(o, oo);
		}
		return true;
	}
}