#include "lr35902.h"
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include "../../registry.h"
#include "opcodes.h"
#include "opcodes_cb.h"


/*********************************************************************************************************************/
// REGISTERING
static xprocessors::RegistryHandler<xprocessors::Cpu::Ptr> reg("lr35902", xprocessors::LR35902::create);


/*********************************************************************************************************************/
// OPCODES
enum class opcodes {
	COMMON_OPCODES,
	COMMON_OPCODES_CB
};


/*********************************************************************************************************************/
// OPCODES TABLES
constexpr auto opcode_tables{ []() constexpr {
	COMMON_OPCODES_TABLE(opcodes, result)

	for (int i = 0; i < 256; ++i) {
		// Change common opcodes
		if (i == 0xE0)
			result[i] = opcodes::UNIMPLEMENTED;
		if (i == 0xE2)
			result[i] = opcodes::UNIMPLEMENTED;
		if (i == 0xE4)
			result[i] = opcodes::UNIMPLEMENTED;
		if (i == 0xE8)
			result[i] = opcodes::UNIMPLEMENTED;
		if (i == 0xEA)
			result[i] = opcodes::UNIMPLEMENTED;
		if (i == 0xEC)
			result[i] = opcodes::UNIMPLEMENTED;
		if (i == 0xF0)
			result[i] = opcodes::UNIMPLEMENTED;
		if (i == 0xF2)
			result[i] = opcodes::UNIMPLEMENTED;
		if (i == 0xF4)
			result[i] = opcodes::UNIMPLEMENTED;
		if (i == 0xF8)
			result[i] = opcodes::UNIMPLEMENTED;
		if (i == 0xFA)
			result[i] = opcodes::UNIMPLEMENTED;
		if (i == 0xFC)
			result[i] = opcodes::UNIMPLEMENTED;
	}
	return result;
}()
};

constexpr auto opcode_cb_tables{ []() constexpr {
	COMMON_OPCODES_TABLE_CB(opcodes, result)

	return result;
}()
};

struct LR35902IO {
	static const uint8_t IF = 0x0f;
	static const uint8_t IE = 0xff;
};

namespace xprocessors {
	LR35902::LR35902() :
		Z80FamilyCpu(Device::MEM_AVAILABLE) {
		reset();
	}
	bool LR35902::checkCC(const opcode_t opcode) const
	{
		switch (opcode & 0b00111000) {
		case 0b00000000:
			return !_state.zeroFlag();
		case 0b00001000:
			return _state.zeroFlag();
		case 0b00010000:
			return !_state.carryFlag();
		case 0b00011000:
			return _state.carryFlag();
		default:
			return 0;
		}
	}
	uint8_t LR35902::get_m() const {
		return _handlerRead(_state.hl());
	}

	uint8_t LR35902::dec(const uint8_t value) {
		uint8_t result = value - 1;
		_state.adjustZ(result);
		_state.setFlags(LR35902Flags::NF);
		_state.setFlag(LR35902Flags::HF, (result & 0x0f) == 0x0f);
		return result;
	}
	uint8_t LR35902::inc(const uint8_t value) {
		uint8_t result = value + 1;
		_state.adjustZ(result);
		_state.resetFlags(LR35902Flags::NF);
		_state.setFlag(LR35902Flags::HF, (result & 0x0f) == 0);
		return result;
	}

	void LR35902::xra(const uint8_t value) {
		_state.a() ^= value;
		_state.resetFlags(LR35902Flags::CF | LR35902Flags::NF | LR35902Flags::HF);
		_state.adjustZ(_state.a());
	}
	void LR35902::ora(const uint8_t value) {
		_state.a() |= value;
		_state.resetFlags(LR35902Flags::CF | LR35902Flags::NF | LR35902Flags::HF);
		_state.adjustZ(_state.a());
	}
	void LR35902::ana(const uint8_t value) {
		_state.a() &= value;
		_state.resetFlags(LR35902Flags::CF | LR35902Flags::NF);
		_state.setFlags(LR35902Flags::HF);
		_state.adjustZ(_state.a());
	}
	void LR35902::sub(const uint8_t value) {
		const uint16_t sum = _state.a() - value;
		const uint16_t carryIns = (sum ^ _state.a() ^ value);

		_state.a() = sum & 0xff;

		_state.resetFlags(LR35902Flags::HF | LR35902Flags::CF);
		_state.setFlags(LR35902Flags::NF);
		_state.adjustZ(_state.a());
		if (carryIns & 0x10)
			_state.setFlags(LR35902Flags::HF);

		if ((carryIns >> 8) & 0x1)
			_state.setFlags(LR35902Flags::CF);
	}
	void LR35902::sbc(const uint8_t value) {
		const uint8_t flag = _state.carryFlag() ? 1 : 0;
		const uint16_t sum = _state.a() - value - flag;
		const uint16_t carryIns = (sum ^ _state.a() ^ value);

		_state.a() = sum & 0xff;

		_state.resetFlags(LR35902Flags::HF | LR35902Flags::CF);
		_state.setFlags(LR35902Flags::NF);
		_state.adjustZ(_state.a());
		if (carryIns & 0x10)
			_state.setFlags(LR35902Flags::HF);

		if ((carryIns >> 8) & 0x1)
			_state.setFlags(LR35902Flags::CF);
	}
	void LR35902::cmp(const uint8_t value) {
		const uint16_t sum = _state.a() - value;
		const uint16_t carryIns = (sum ^ _state.a() ^ value);

		_state.resetFlags(LR35902Flags::HF | LR35902Flags::CF);
		_state.setFlags(LR35902Flags::NF);
		_state.adjustZ(sum & 0xff);
		if (carryIns & 0x10)
			_state.setFlags(LR35902Flags::HF);

		if ((carryIns >> 8) & 0x1)
			_state.setFlags(LR35902Flags::CF);
	}
	void LR35902::add(const uint8_t value)
	{
		const uint16_t sum = _state.a() + value;
		const uint16_t carryIns = sum ^ _state.a() ^ value;

		_state.a() = sum & 0xff;
		_state.resetFlags(LR35902Flags::NF | LR35902Flags::CF | LR35902Flags::HF);
		_state.adjustZ(_state.a());
		if (carryIns & 0x10)
			_state.setFlags(LR35902Flags::HF);
		if (sum > 0xff)
			_state.setFlags(LR35902Flags::CF);
	}
	void LR35902::adc(const uint8_t value)
	{
		const uint8_t flag = _state.carryFlag() ? 1 : 0;
		const uint16_t sum = _state.a() + value + flag;
		const uint16_t carryIns = sum ^ _state.a() ^ value;

		_state.a() = sum & 0xff;
		_state.resetFlags(LR35902Flags::NF | LR35902Flags::CF | LR35902Flags::HF);
		_state.adjustZ(_state.a());
		if (carryIns & 0x10)
			_state.setFlags(LR35902Flags::HF);
		if (sum > 0xff)
			_state.setFlags(LR35902Flags::CF);
	}
	void LR35902::add_hl(const uint16_t value)
	{
		uint32_t res = _state.hl() + value;
		_state.resetFlags(LR35902Flags::NF);
		if ((_state.hl() & 0xfff) + (value & 0xfff) > 0xfff)
			_state.setFlags(LR35902Flags::HF);
		else
			_state.resetFlags(LR35902Flags::HF);
		_state.hl() = res & 0xffff;
		if ((res & 0xffff0000) != 0)
			_state.setFlags(LR35902Flags::CF);
		else
			_state.resetFlags(LR35902Flags::CF);
		_elapsed_cycles += 4;
	}
	uint16_t LR35902::add_sp(const uint8_t value)
	{
		uint32_t res = _state.sp() + static_cast<signed char>(value);
		_state.resetFlags(LR35902Flags::NF | LR35902Flags::ZF);
		if ((_state.sp() & 0xf) + (value & 0xf) > 0xf)
			_state.setFlags(LR35902Flags::HF);
		else
			_state.resetFlags(LR35902Flags::HF);
		if ((_state.sp() & 0xff) + value > 0xff)
			_state.setFlags(LR35902Flags::CF);
		else
			_state.resetFlags(LR35902Flags::CF);
		_elapsed_cycles += 4;
		return res & 0xffff;
	}

	void LR35902::jr(const bool condition) {
		const int8_t delta = static_cast<signed char>(readArgument8());
		if (condition) {
			_state.pc() += delta;
			_elapsed_cycles += 4;
		}
	}
	void LR35902::daa()
	{
		if (_state.addSubFlag()) {
			if (_state.carryFlag()) {
				_state.a() -= 0x60;
			}
			if (_state.halfCarryFlag()) {
				_state.a() -= 0x06;
			}
		}
		else {
			if ((_state.a() > 0x99) || (_state.carryFlag())) {
				_state.a() += 0x60;
				_state.setFlags(LR35902Flags::CF);
			}
			if (((_state.a() & 0xf) > 9) || (_state.halfCarryFlag())) {
				_state.a() += 6;
			}
		}
		_state.adjustZ(_state.a());
		_state.resetFlags(LR35902Flags::HF);
	}
	void LR35902::dad(const uint16_t value)
	{
		uint32_t res = _state.hl() + value;
		_state.h() = res >> 8;
		_state.l() = res & 0xff;
		if ((res & 0xffff0000) != 0)
			_state.setFlags(LR35902Flags::CF);
		else
			_state.resetFlags(LR35902Flags::CF);
	}

	void LR35902::interrupt_handler() {
		if ((interrupt_enabled != 0) && (!_halted))
			return;
		const uint8_t mask = read8(0xff00 + LR35902IO::IE);
		const uint8_t request = read8(0xff00 + LR35902IO::IF);
		uint8_t request_enabled = request & mask;

		if (request_enabled)
			_halted = false;
		if (interrupt_enabled != 0)
			return;
		uint8_t handler = 1;
		uint16_t int_pc = 0x40;
		while (request_enabled) {
			if (request_enabled & 1) {
				write8(0xff00 + LR35902IO::IF, request & ~handler);
				interrupt_enabled = 2;
				push(_state.pc());
				_state.pc() = int_pc;
				request_enabled = 0;
			}
			else {
				handler <<= 1;
				request_enabled >>= 1;
				int_pc += 0x08;
			}
		}
	}
	uint8_t LR35902::executeOne() {
		interrupt_handler();
		if (interrupt_enabled == 1)
			interrupt_enabled = 0;
		if (_halted) {
			return 4;
		}

		const opcode_t opcode = readOpcode();
		decode_opcode(opcode);
		return 4;
	}

	bool LR35902::interrupt(const uint8_t inte) {
		if (interrupt_enabled == 0)
			interrupt_request = inte;
		return (interrupt_enabled == 0) ? true : false;
	}

	bool LR35902::reset(const uint16_t address) {
		_state.pc() = address;
		_elapsed_cycles = 0;
		_stopped = false;
		interrupt_enabled = 2;
		interrupt_request = 8;
		//		return Z80FamilyCpu::reset(address);
		Z80FamilyCpu::reset(address);
		_state.sp() = 0xfffe;
		_state.a() = 0x01;
		_state.setFlags(0xb0);
		_state.b() = 0;
		_state.c() = 0x13;
		_state.d() = 0;
		_state.e() = 0xd8;
		_state.h() = 0x01;
		_state.l() = 0x4d;
		return true;
	}

	/*********************************************************************************************************************/
	void LR35902::decode_opcode(const uint8_t opcode) {
		if (opcode_tables[opcode] != opcodes::UNIMPLEMENTED) {
			COMMON_OPCODES_DECODING(opcodes, opcode_tables, opcode)
			return;
		}
		uint16_t cycle = 0;
		uint16_t tmp16 = 0;

		switch (opcode) {
		case 0x07: /* RLCA */
			_state.resetFlags(LR35902Flags::ALL);
			if (_state.a() & 0x80)
				_state.setFlags(LR35902Flags::CF);
			_state.a() <<= 1;
			_state.a() |= (_state.carryFlag()) ? 1 : 0;
			break;
		case 0x08: /* LD (nnnn), SP */
			write16(readArgument16(), _state.sp());
			break;
		case 0x0F: /* RRCA */
			_state.resetFlags(LR35902Flags::ALL);
			if (_state.a() & 1)
				_state.setFlags(LR35902Flags::CF);
			_state.a() >>= 1;
			_state.a() |= (_state.carryFlag() ? 0x80 : 0);
			break;

		case 0x10: /* STOP d8 */
			_stopped = true;
			_state.pc()++;
			break;
		case 0x17: /* RLA */
		{
			uint8_t flag = (_state.carryFlag()) ? 1 : 0;
			_state.resetFlags(LR35902Flags::ALL);
			if (_state.a() & 0x80)
				_state.setFlags(LR35902Flags::CF);
			_state.a() <<= 1;
			_state.a() |= flag;
		}
		break;
		case 0x18: /* JR */
			jr(true);
			break;
		case 0x1F: /* RAR */
		{
			uint8_t flag = (_state.carryFlag()) ? 1 : 0;
			_state.resetFlags(LR35902Flags::ALL);
			if (_state.a() & 1)
				_state.setFlags(LR35902Flags::CF);
			_state.a() >>= 1;
			_state.a() |= flag << 7;
		}
		break;
		case 0x20: /* JR NZ */
			jr(!_state.zeroFlag());
			break;
		case 0x22: /* LD (HL+) */
			write8(_state.hl(), _state.a());
			_state.hl()++;
			break;
		case 0x27: /* DAA */
			daa();
			break;
		case 0x28: /* JR Z */
			jr(_state.zeroFlag());
			break;
		case 0x2A: /* LD A, (HL+) */
			_state.a() = read8(_state.hl());
			_state.hl()++;
			break;
		case 0x2F: /* CPL */
			_state.a() = ~_state.a();
			_state.setFlags(LR35902Flags::HF | LR35902Flags::NF);
			break;

		case 0x30: /* JR NC */
			jr(!_state.carryFlag());
			break;
		case 0x32: /* LD (HL-) */
			write8(_state.hl(), _state.a());
			_state.hl()--;
			break;
		case 0x37: /* SCF */
			_state.resetFlags(LR35902Flags::NF | LR35902Flags::HF);
			_state.setFlags(LR35902Flags::CF);
			break;
		case 0x38: /* JR C */
			jr(_state.carryFlag());
			break;
		case 0x3A: /* LD A, (HL-) */
			_state.a() = read8(_state.hl());
			_state.hl()--;
			break;
		case 0x3F: /* CCF */
			_state.resetFlags(LR35902Flags::NF | LR35902Flags::HF);
			if (_state.carryFlag())
				_state.resetFlags(LR35902Flags::CF);
			else
				_state.setFlags(LR35902Flags::CF);
			break;

		case 0xCB:
			decode_opcode_cb(readOpcode());
			break;

		case 0xD9: /* RETI */
			ret();
			interrupt_enabled = 1;
			break;

		case 0xE0: /* LDH */
			write8(0xff00 + readArgument8(), _state.a());
			break;
		case 0xE2: /* LD (C), A */
			write8(0xff00 + _state.c(), _state.a());
			break;
		case 0xE8: /* ADD SP */
			_state.sp() = add_sp(static_cast<signed char>(readArgument8()));
			_elapsed_cycles += LR35902Costs::EXTRAPUSH;
			break;
		case 0xE9: /* JP HL */
			_state.pc() = _state.hl();
			break;
		case 0xEA: /* LD (), A */
			write8(readArgument16(), _state.a());
			break;

		case 0xF0: /* LDH */
			_state.a() = read8(0xff00 + readArgument8());
			break;
		case 0xF2: /* LD A, (C) */
			_state.a() = read8(0xff00 + _state.c());
			break;
		case 0xF3: /* DI */
			interrupt_enabled = 2;
			break;
		case 0xF8: /* LDHL */
			_state.hl() = add_sp(static_cast<signed char>(readArgument8()));
			break;
		case 0xF9: /* SPHL */
			_state.sp() = _state.hl();
			_elapsed_cycles += LR35902Costs::EXTRAPUSH;
			break;
		case 0xFA: /* LD A, () */
			_state.a() = read8(readArgument16());
			break;
		case 0xFB: /* EI */
			interrupt_enabled = 1;
			break;

		default: unimplemented(opcode); break;
		}
	}

	void LR35902::decode_opcode_cb(const opcode_t opcode) {
		switch (opcode_cb_tables[opcode]) {
		case opcodes::RLC_R:
		case opcodes::RLC_HL:
			decodeR(opcode, rlc(decodeR(opcode)));
			break;
		case opcodes::RL_R:
		case opcodes::RL_HL:
			decodeR(opcode, rl(decodeR(opcode)));
			break;
		case opcodes::RR_R:
		case opcodes::RR_HL:
			decodeR(opcode, rr(decodeR(opcode)));
			break;
		case opcodes::RRC_R:
		case opcodes::RRC_HL:
			decodeR(opcode, rrc(decodeR(opcode)));
			break;
		case opcodes::SRL_R:
		case opcodes::SRL_HL:
			decodeR(opcode, srl(decodeR(opcode)));
			break;
		case opcodes::SRA_R:
		case opcodes::SRA_HL:
			decodeR(opcode, sra(decodeR(opcode)));
			break;
		case opcodes::SLA_R:
		case opcodes::SLA_HL:
			decodeR(opcode, sla(decodeR(opcode)));
			break;
		case opcodes::SLL_R:
		case opcodes::SLL_HL:
			/* SWAP */
			decodeR(opcode, swap(decodeR(opcode)));
			break;
		case opcodes::BIT_R:
		case opcodes::BIT_HL:
			_state.resetFlags(LR35902Flags::NF);
			_state.setFlags(LR35902Flags::HF);
			_state.setFlag(LR35902Flags::ZF, (decodeR(opcode) & (1 << ((opcode >> 3) & 0x07))) == 0);
			break;
		case opcodes::SET_R:
		case opcodes::SET_HL:
			decodeR(opcode, decodeR(opcode) | (1 << ((opcode & 0b00111000) >> 3)));
			break;
		case opcodes::RES_R:
		case opcodes::RES_HL:
			decodeR(opcode, decodeR(opcode) & ~(1 << ((opcode & 0b00111000) >> 3)));
			break;
		default:
			throw std::runtime_error("Unexpected opcode in decode_cb : " + opcode);
		}
	}

	uint8_t LR35902::rlc(const uint8_t value) {
		_state.setFlag(LR35902Flags::CF, ((value & 0x80) == 0x80));
		_state.setFlag(LR35902Flags::ZF, (value == 0));
		_state.resetFlags(LR35902Flags::HF | LR35902Flags::NF);
		return ((value << 1) | (value >> 7));
	}
	uint8_t LR35902::rl(const uint8_t value) {
		const uint8_t result = (value << 1) | (_state.carryFlag() ? 1 : 0);
		_state.setFlag(LR35902Flags::CF, ((value & 0x80) == 0x80));
		_state.setFlag(LR35902Flags::ZF, (result == 0));
		_state.resetFlags(LR35902Flags::HF | LR35902Flags::NF);
		return result;
	}
	uint8_t LR35902::rrc(const uint8_t value) {
		_state.setFlag(LR35902Flags::CF, ((value & 0x01) == 0x01));
		_state.setFlag(LR35902Flags::ZF, (value == 0));
		_state.resetFlags(LR35902Flags::HF | LR35902Flags::NF);
		return ((value >> 1) | (value << 7));
	}
	uint8_t LR35902::rr(const uint8_t value) {
		const uint8_t result = (value >> 1) | (_state.carryFlag() ? 0x80 : 0);
		_state.setFlag(LR35902Flags::CF, ((value & 0x01) == 0x01));
		_state.setFlag(LR35902Flags::ZF, (result == 0));
		_state.resetFlags(LR35902Flags::HF | LR35902Flags::NF);
		return result;
	}
	uint8_t LR35902::srl(const uint8_t value) {
		const uint8_t result = value >> 1;
		_state.setFlag(LR35902Flags::CF, ((value & 0x01) == 0x01));
		_state.setFlag(LR35902Flags::ZF, (result == 0));
		_state.resetFlags(LR35902Flags::HF | LR35902Flags::NF);
		return result;
	}
	uint8_t LR35902::sra(const uint8_t value) {
		const uint8_t result = value >> 1 | (value & 0x80);
		_state.setFlag(LR35902Flags::CF, ((value & 0x01) == 0x01));
		_state.setFlag(LR35902Flags::ZF, (result == 0));
		_state.resetFlags(LR35902Flags::HF | LR35902Flags::NF);
		return result;
	}
	uint8_t LR35902::sla(const uint8_t value) {
		const uint8_t result = value << 1;
		_state.setFlag(LR35902Flags::CF, ((value & 0x80) == 0x80));
		_state.setFlag(LR35902Flags::ZF, (result == 0));
		_state.resetFlags(LR35902Flags::HF | LR35902Flags::NF);
		return result;
	}
	uint8_t LR35902::swap(const uint8_t value) {
		const uint8_t result = (value << 4) | (value >> 4);
		_state.resetFlags(LR35902Flags::ALL);
		_state.setFlag(LR35902Flags::ZF, (result == 0));
		return result;
	}

}
