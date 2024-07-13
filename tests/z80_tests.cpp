#include "z80.h"

#include "cpu_tests.h"


using namespace xprocessors::cpu;

namespace tests
{
	struct Z80impl
	{
		Z80dbg _cpu;
		uint8_t* _memory;

		Z80impl() {
			_cpu.init();
			_memory = new uint8_t[0x10000];
		}
		~Z80impl() {
			delete[] _memory;
		}

		bool runTest(const std::string& filename, const uint64_t cycles, const uint64_t instructions)
		{
			load(_memory + 0x100, filename);
			_memory[0] = 0xD3;
			_memory[1] = 0x00;
			_memory[5] = 0xD3;
			_memory[6] = 0x01;
			_memory[7] = 0xC9;
			_cpu.reset(0x100);

			Supervisor supervisor(filename, cycles, instructions);
			supervisor.start();

			bool finished = false;
			bool ioflag = false;
			while (!finished) {
				if (_cpu.isfetch())
					supervisor.inc_instructions();
				_cpu.tick();
				supervisor.inc_cycles();
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
					if (ioflag)
					{
						ioflag = false;
						continue;
					}
					ioflag = true;
					if ((_cpu.getAddressBus() & 0x00ff) == 0) {
						supervisor.inc_cycles(); // IOWRITE
						supervisor.inc_cycles(); // NOP
						finished = true;
					}
					if ((_cpu.getAddressBus() & 0x00ff) == 0x0001) {
						uint8_t operation = _cpu.getc();

						if (operation == 2) { // print a character stored in E
							std::cout << (char)(_cpu.gete());
						}
						else if (operation == 9) { // print from memory at (DE) until '$' char
							uint16_t addr = _cpu.getde();
							do {
								std::cout << (char)(_memory[addr++]);
							} while (_memory[addr] != '$');
						}
					}
				}
			}

			supervisor.stop();
			return true;
		}
	};

	bool z80_run()
	{
		Z80impl cpu;
		cpu.runTest("tests/data/Z80/prelim.com", 8721Ui64, 899);
		cpu.runTest("tests/data/Z80/zexdoc.com", 46734978649Ui64, 5764169747);
		cpu.runTest("tests/data/Z80/zexall.com", 46734978649Ui64, 5764169747);

		return true;
	}
}