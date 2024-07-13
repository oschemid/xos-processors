#include "sm83.h"

#include <fstream>
#include <iostream>
#include <chrono>


using namespace std::chrono;
using namespace xprocessors::cpu;


namespace tests
{
	struct sm83impl
	{
		sm83dbg _cpu;
		uint8_t* _memory;
		uint8_t* _memory2;
		uint8_t _selectedROMbanks{ 1 };

		sm83impl()
		{
			_cpu.init();
			_memory = new uint8_t[0x10000];
			_memory2 = new uint8_t[0x10000-0x8000];
		}
		~sm83impl()
		{
			delete[] _memory2;
			delete[] _memory;
		}

		void timer()
		{
			static const uint16_t frequency[4] = { 1024,16,64,256 };
			static uint64_t previous_cycles{ 0 };
			static uint64_t cycles{ 0 };
			const uint8_t tac = _memory2[0xff07-0x8000];
			cycles++;
			if (tac & 0x04)
			{
				if (previous_cycles == 0)
					previous_cycles = cycles;
				const uint16_t delta = static_cast<uint16_t>((cycles - previous_cycles) / frequency[tac & 0x03]);
				if (delta > 0)
				{
					uint16_t tima = _memory2[0xff05-0x8000] + delta;
					if (tima > 0xff)
					{
						tima = _memory2[0xff06-0x8000];
						_memory2[0xff0f-0x8000] |= 0x04;
						_cpu.interrupt(0x04);
					}
					_memory2[0xff05-0x8000] = tima & 0xff;
					previous_cycles = cycles;
				}
			}
			else
			{
				previous_cycles = 0;
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
			load(0x0, filename);
			_cpu.reset(0x100);
			_cpu.setMemoryAccessor([this](const uint16_t a) { return _memory[a]; });

			auto start = high_resolution_clock::now();
			uint32_t instruction = 0;
			bool finished = false;
			bool ioexpected = false;
			char c;

//			_cpu.pause();
//			std::ofstream stre("log.log");

			while (!finished)
			{
//				if (!_cpu.isrunning())
//				{
//					stre << _cpu.opcode() << std::endl;
//					_cpu.runStep();
//					continue;
//				}
				_cpu.tick();
				if (_cpu.getpc() == 0x06f1)
					finished = true;
				timer();
				cycles++;
				const uint16_t pins = _cpu.getControlPins();
				if ((pins & (sm83::PIN_RD)) == (sm83::PIN_RD))
				{
					const uint16_t address = _cpu.getAddressBus();
					if (address < 0x4000)
						_cpu.setDataBus(_memory[address]);
					else if (address < 0x8000)
						_cpu.setDataBus(_memory[_selectedROMbanks * 0x4000 + (address - 0x4000)]);
					else
						_cpu.setDataBus(_memory2[address-0x8000]);
				}
				if ((pins & (sm83::PIN_WR)) == (sm83::PIN_WR))
				{
					const uint16_t address = _cpu.getAddressBus();
					if (address < 0x8000)
					{
						if ((address >= 0x2000) && (address < 0x4000))
						{
							_selectedROMbanks = _cpu.getDataBus() & 0x1f;
							if (_selectedROMbanks == 0)
								_selectedROMbanks = 1;
						}
					}
					else if (address >= 0xff00)
					{
						if (address >= 0xff80)
							_memory2[address-0x8000] = _cpu.getDataBus();
						switch (address & 0xff)
						{
						case 1:
							c = _cpu.getDataBus();
							break;
						case 2:
							if (_cpu.getDataBus() == 0x81)
								std::cout << (char)c;
							break;
						default:
							_memory2[address-0x8000] = _cpu.getDataBus();
							break;
						}
					}
					else
					{
						_memory2[address - 0x8000] = _cpu.getDataBus();
					}
				}
			}
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<milliseconds>(stop - start);
			std::cout << std::endl << "Tests Duration : " << duration.count() << "ms" << std::endl;
			if (cycles_expected > 0)
				std::cout << "Expected cycles : " << cycles_expected << " - Cycles : " << cycles << "(" << instruction << " instructions)" << std::endl;
			return true;
		}
	};


	bool sm83_run()
	{
		sm83impl cpu;
//		cpu.runTest("tests/data/sm83/blargg/02-interrupts.gb", 0);
		cpu.runTest("tests/data/sm83/blargg/cpu_instrs.gb", 0);
		return true;
	}
}