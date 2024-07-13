#include "i8080.h"
#include "cpu_tests.h"




using namespace std::chrono;
using namespace xprocessors::cpu;


namespace tests
{
	struct i8080impl
	{
		i8080dbg _cpu;
		uint8_t* _memory;

		i8080impl()
		{
			_cpu.init();
			_memory = new uint8_t[0x10000];
		}
		~i8080impl()
		{
			delete[] _memory;
		}

		bool runTest(const std::string& filename, const uint64_t cycles, const uint64_t instructions)
		{
			Supervisor supervisor(filename, cycles, instructions);

			load(_memory+0x100, filename);
			_memory[0] = 0xD3;
			_memory[1] = 0x00;
			_memory[5] = 0xD3;
			_memory[6] = 0x01;
			_memory[7] = 0xC9;
			_cpu.reset(0x100);
			_cpu.setReady();

			supervisor.start();

			bool finished = false;
			bool ioexpected = false;
			while (!finished)
			{
				_cpu.tick();
				supervisor.inc_cycles();

				const uint16_t pins = _cpu.getControlPins();
				if (pins & i8080::PIN_SYNC)
				{
					if (_cpu.getDataBus() & i8080::STATUS_M1)
						supervisor.inc_instructions();
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
							supervisor.inc_cycles(); // STOPWRITE
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
			supervisor.stop();
			return true;
		}
	};

	bool i8080_run()
	{
		i8080impl cpu;
		cpu.runTest("tests/data/i8080/TST8080.COM", 4924, 651);
		cpu.runTest("tests/data/i8080/8080PRE.COM", 7817, 1061);
		cpu.runTest("tests/data/i8080/CPUTEST.COM", 255653383, 33971311);
		cpu.runTest("tests/data/i8080/8080EXM.COM", 23803381171, 2919050698);
		return true;
	}
}