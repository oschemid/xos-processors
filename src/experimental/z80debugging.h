#pragma once
#include "z80.h"
#include <fstream>


namespace xprocessors::cpu
{
	class Z80debugging
	{
	public:
		Z80debugging();
		void activateDisassembly() { _disassembly = true; }

		void debug(Z80*, std::function<uint8_t(const uint16_t)>);

	protected:
		std::ofstream _disasm_fd;
		bool _disassembly{ false };
		void disassembly(const Z80*, std::function<uint8_t(const uint16_t)>);
		void disassemblyopcode(const Z80*, std::function<uint8_t(const uint16_t)>);
		void disassemblyopcode_dd(const Z80*, std::function<uint8_t(const uint16_t)>);
		void disassemblyopcode_ed(const Z80*, std::function<uint8_t(const uint16_t)>);
		void disassemblyopcode_fd(const Z80*, std::function<uint8_t(const uint16_t)>);
	};
}