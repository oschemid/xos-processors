#include "i8080.h"
#include <sstream>


namespace xprocessors::cpu
{
	i8080dbg::i8080dbg() :
		i8080{ }
	{

	}
	void i8080dbg::setMemoryAccessor(std::function<uint8_t(const uint16_t)> fn)
	{
		_memoryfn = fn;
	}

	bool i8080dbg::isrunning()
	{
		if (_running)
		{
			if (_step)
			{
				if (_step1)
				{
					if ((_pins & PIN_SYNC) && (_databus & STATUS_M1))
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

	string i8080dbg::opcode() const
	{
		if ((_pins & PIN_SYNC) && (_databus & STATUS_M1))
		{
			uint16_t address = pc;
			std::stringstream disasm;
			disasm << std::hex << pc << " - " << disassembly(address);
			return disasm.str();
		}
		return "";
	}
}