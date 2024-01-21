#include "z80debugging.h"

using namespace xprocessors::cpu;


Z80debugging::Z80debugging()
{
}

void Z80debugging::debug(Z80* cpu, uint8_t* memory)
{
	if (_disassembly)
		disassembly(cpu, memory);
}

void Z80debugging::disassembly(const Z80* cpu, const uint8_t* memory)
{
	if (!_disasm_fd.is_open())
		_disasm_fd.open("disasm.txt");
	if (cpu->current_step == Z80::FETCH)
		disassemblyopcode(cpu, memory);
}