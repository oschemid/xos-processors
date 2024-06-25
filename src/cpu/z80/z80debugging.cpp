#include "z80.h"
#include <sstream>


using namespace xprocessors::cpu;
using namespace xprocessors;


namespace xprocessors::cpu
{
	Z80dbg::Z80dbg() :
		Z80{ }
	{

	}
	void Z80dbg::setMemoryAccessor(std::function<uint8_t(const uint16_t)> fn)
	{
		_memoryfn = fn;
	}

	bool Z80dbg::isrunning()
	{
		if (_running)
		{
			if (_step)
			{
				if (_step1)
				{
					if (_opcode == FETCH)
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

	string Z80dbg::opcode() const
	{
		if (_opcode==FETCH)
		{
			uint16_t address = pc;
			std::stringstream disasm;
			disasm << std::hex << pc << " - " << disassembly(address);
			return disasm.str();
		}
		return "";
	}
}