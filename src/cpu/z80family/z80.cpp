#include "z80.h"
#include "xprocessors.h"
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <array>
#include "../../registry.h"


static xprocessors::RegistryHandler<xprocessors::Cpu::Ptr> reg("Z80", xprocessors::Z80::create);

using namespace xprocessors;

namespace xprocessors {
	Z80::Z80() :
		Z80FamilyCpu(Device::IO_AVAILABLE|Device::MEM_AVAILABLE) {
		reset();
	}

	// Function : (HL) <- fn((HL)) or (IX+d) <- fn((IX+d)) or (IY+d) <- fn((IY+d))
	void Z80::apply_hl(const fnuint8_t fn, const prefix p, const int8_t d) {
		switch (p) {
		case NO:
			write8(_state.hl(), fn(read8(_state.hl())));
			_elapsed_cycles++;
			break;
		case DD:
			write8(_state.ix() + d, fn(read8(_state.ix() + d)));
			_elapsed_cycles += 2;
			break;
		case FD:
			write8(_state.iy() + d, fn(read8(_state.iy() + d)));
			_elapsed_cycles += 2;
			break;
		}
	}

	void Z80::apply_r(const fnuint8_t fn, const opcode_t opcode, const prefix p) {
		decodeR(opcode, fn(decodeR(opcode)));
	}

	void Z80::apply_ixy_r(const fnuint8_t fn, const opcode_t opcode, const prefix p, const int8_t d) {
		switch (p) {
		case NO:
			decodeR(opcode, fn(decodeR(opcode)));
			break;
		case DD:
			decodeR(opcode, fn(read8(_state.ix() + d)));
			write8(_state.ix() + d, decodeR(opcode));
			_elapsed_cycles += 2;
			break;
		case FD:
			decodeR(opcode, fn(read8(_state.iy() + d)));
			write8(_state.iy() + d, decodeR(opcode));
			_elapsed_cycles += 2;
			break;
		}
	}

	/******************************************************************************/
	void Z80::unimplemented()
	{
		_state.pc()--;
		auto opcode = readOpcode();
		std::cout << std::hex << int(opcode) << std::endl;
		throw std::runtime_error("Instruction not implemented");
	}
	void Z80::illegal()
	{
		_state.pc()--;
		auto opcode = readOpcode();
		throw std::runtime_error("Illegal instruction " + opcode);
	}
	uint8_t Z80::executeOne() {
		if (iff1) {
			if (iff1_waiting)
				iff1_waiting = false;
			else {
				if (interrupt_waiting) {
					interrupt_waiting = false;
					iff1 = false;
					iff2 = false;
					_halted = false;
					switch (im) {
					case interrupt_mode::mode_0:
						decode_opcode(interrupt_request);
						break;
					case interrupt_mode::mode_1:
						push(_state.pc());
						_state.pc() = 0x0038;
						break;
					case interrupt_mode::mode_2:
						push(_state.pc());
						_state.pc() = (_state.i() << 8) | interrupt_request;
						_state.pc() = readArgument16();
						break;
					default:
						return 0;
					}
					return 0;
				}
			}
		}

		const std::uint8_t opcode = (_halted) ? 0x00 : readOpcode();
		if (_halted)
			_elapsed_cycles += Cost::READ_OPCODE;
		current_prefix = NO;
		decode_opcode(opcode);
		return 0;
	}

	bool Z80::interrupt(const uint8_t inte) {
		if (iff1) {
			interrupt_waiting = true;
			interrupt_request = inte;
		}
		return true;
	}

	bool Z80::reset(const uint16_t address) {
		_elapsed_cycles = 0;

		iff1 = false;
		iff2 = false;
		iff1_waiting = false;
		im = interrupt_mode::mode_0;
		_state.pc() = address;
		interrupt_waiting = false;
		return Z80FamilyCpu::reset(address);
	}
}

bool Z80::checkCondition2(const opcode_t opcode) const
{
	switch (opcode & 0b00011000) {
	case 0b00000000:
		return !_state.zeroFlag();
	case 0b00001000:
		return _state.zeroFlag();
	case 0b00010000:
		return !_state.carryFlag();
	case 0b00011000:
		return _state.carryFlag();
	}
	throw std::runtime_error("checkCondition2 : illegal value " + opcode);
}
