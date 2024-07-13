#include "sm83.h"
#include <sstream>
#include <iomanip>


using namespace xprocessors::cpu;
using namespace xprocessors;


namespace xprocessors::cpu
{
	sm83dbg::sm83dbg() :
		sm83{ }
	{

	}
	void sm83dbg::setMemoryAccessor(std::function<uint8_t(const uint16_t)> fn)
	{
		_memoryfn = fn;
	}

	bool sm83dbg::isrunning()
	{
		if (_running)
		{
			if (_step)
			{
				if (_step1 == 0)
				{
					if (pins & PIN_M1)
					{
						_step1 = 1;
					}
				}
				else
				{
					if (!(pins & PIN_M1))
					{
						_running = false;
						_step1 = 0;
					}
				}
			}
		}
		return _running;
	}

	string sm83dbg::opcode() const
	{
		if ((_step1==0)&&(!(_running)))
		{
			uint16_t address = pc - 1;
			std::stringstream disasm;
			disasm << std::hex << pc << " - " << disassembly(address);
			disasm << " - A:" << std::setw(2) << std::setfill('0') << std::hex << (int)a;
			disasm << " F:" << std::setw(2) << std::setfill('0') << std::hex << (int)f;
			disasm << " B:" << std::setw(2) << std::setfill('0') << std::hex << (int)b;
			disasm << " C:" << std::setw(2) << std::setfill('0') << std::hex << (int)c;
			disasm << " D:" << std::setw(2) << std::setfill('0') << std::hex << (int)d;
			disasm << " E:" << std::setw(2) << std::setfill('0') << std::hex << (int)e;
			disasm << " HL:" << std::setw(2) << std::setfill('0') << std::hex << (int)hl;
			disasm << " SP:" << std::setw(2) << std::setfill('0') << std::hex << (int)sp;
			disasm << " PC:" << std::setw(2) << std::setfill('0') << std::hex << (int)(pc - 1);
			return disasm.str();
		}
		return "";
	}
}