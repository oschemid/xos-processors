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
				if (_step1)
				{
					if (current_step == FETCH)
					{
						_running = false;
						_step1 = false;
					}
				}
				else
				{
					_step1 = true;
				}
			}
		}
		return _running;
	}

	string sm83dbg::opcode() const
	{
		if (current_step==FETCH)
		{
			uint16_t address = pc;
			std::stringstream disasm;
			disasm << std::hex << pc << " - " << disassembly(address);
			disasm << " - A:" << std::setw(2) << std::setfill('0') << std::hex << (int)a;
			disasm << " F:" << std::setw(2) << std::setfill('0') << std::hex << (int)f;
			disasm << " B:" << std::setw(2) << std::setfill('0') << std::hex << (int)b;
			disasm << " C:" << std::setw(2) << std::setfill('0') << std::hex << (int)c;
			disasm << " D:" << std::setw(2) << std::setfill('0') << std::hex << (int)d;
			disasm << " E:" << std::setw(2) << std::setfill('0') << std::hex << (int)e;
			return disasm.str();
		}
		return "";
	}
}