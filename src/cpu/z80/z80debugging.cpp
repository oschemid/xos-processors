#include "z80debugging.h"

using namespace xprocessors::cpu;


Z80debugging::Z80debugging()
{
}

void Z80debugging::debug(Z80* cpu, std::function<uint8_t(const uint16_t)> memory_fn)
{
	if (_disassembly)
		disassembly(cpu, memory_fn);
}

void Z80debugging::disassembly(const Z80* cpu, std::function<uint8_t(const uint16_t)> memory_fn)
{
	if (!_disasm_fd.is_open())
		_disasm_fd.open("disasm.txt");
	if (cpu->current_step == Z80::FETCH)
		disassemblyopcode(cpu, memory_fn);
}