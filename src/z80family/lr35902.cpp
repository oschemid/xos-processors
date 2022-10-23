#include "lr35902.h"
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <format>
#include <stdexcept>
#include "../registry.h"
#include "opcodes.h"


/*********************************************************************************************************************/
// REGISTERING
static xprocessors::CpuRegistryHandler reg("lr35902", xprocessors::LR35902::create);


/*********************************************************************************************************************/
// OPCODES
enum class opcodes {
	COMMON_OPCODES
};


/*********************************************************************************************************************/
// OPCODES TABLES
constexpr auto opcode_tables{ []() constexpr {
	COMMON_OPCODES_TABLE(opcodes, result)

	for (int i = 0; i < 256; ++i) {
		if ((i & 0b11000111) == 0b00000100)
			result[i] = opcodes::INC_R;
		if ((i & 0b11000111) == 0b00000101)
			result[i] = opcodes::DEC_R;
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



namespace xprocessors {
	LR35902::LR35902() :
		Z80FamilyCpu() {
		reset();
	}
	uint8_t LR35902::get_m() const {
		return _handlerRead(_state.hl());
	}

	uint8_t LR35902::dec(const uint8_t value) {
		uint8_t result = value - 1;
		_state.adjustSZ(result);
		parityBit = parity(result);
		auxCarryBit = !((result & 0x0f) == 0x0f);
		return result;
	}
	uint8_t LR35902::inc(const uint8_t value) {
		uint8_t result = value + 1;
		_state.adjustSZ(result);
		parityBit = parity(result);
		auxCarryBit = (result & 0x0f) == 0;
		return result;
	}

	void LR35902::xra(const uint8_t value) {
		_state.a() ^= value;
		_state.resetFlags(LR35902Flags::CF);
		auxCarryBit = 0;
		parityBit = parity(_state.a());
		_state.adjustSZ(_state.a());
	}
	void LR35902::ora(const uint8_t value) {
		_state.a() |= value;
		_state.resetFlags(LR35902Flags::CF);
		auxCarryBit = 0;
		parityBit = parity(_state.a());
		_state.adjustSZ(_state.a());
	}
	void LR35902::ana(const uint8_t value) {
		uint8_t result = _state.a() & value;
		_state.resetFlags(LR35902Flags::CF);
		parityBit = parity(result);
		_state.adjustSZ(result);
		auxCarryBit = ((_state.a() | value) & 0x08) != 0;
		_state.a() = result;
	}
	void LR35902::sub(const uint8_t value) {
		if (value > _state.a())
			_state.setFlags(LR35902Flags::CF);
		else
			_state.resetFlags(LR35902Flags::CF);
		auxCarryBit = (_state.a() & 0x0f) - (value & 0x0f) >= 0;
		_state.a() -= value;
		_state.adjustSZ(_state.a());
		parityBit = parity(_state.a());
	}
	void LR35902::sbc(const uint8_t value) {
		const uint8_t flag = _state.carryFlag() ? 1 : 0;
		if (value + flag > _state.a())
			_state.setFlags(LR35902Flags::CF);
		else
			_state.resetFlags(LR35902Flags::CF);
		auxCarryBit = (_state.a() & 0x0f) - (value & 0x0f) - flag >= 0;
		_state.a() -= value + flag;
		_state.adjustSZ(_state.a());
		parityBit = parity(_state.a());
	}
	void LR35902::cmp(const uint8_t value) {
		if (value > _state.a())
			_state.setFlags(LR35902Flags::CF);
		else
			_state.resetFlags(LR35902Flags::CF);
		auxCarryBit = (value & 0xf) > (_state.a() & 0xf) ? 0 : 1;
		uint8_t r = _state.a() - value;
		_state.adjustSZ(r);
		parityBit = parity(r);
	}
	void LR35902::add(const uint8_t value)
	{
		uint16_t sum = _state.a() + value;
		auxCarryBit = ((value & 0x0f) + (_state.a() & 0x0f)) > 0x0f;
		_state.a() = sum & 0xff;
		_state.adjustSZ(_state.a());
		parityBit = parity(_state.a());
		if (sum > 0xff)
			_state.setFlags(LR35902Flags::CF);
		else
			_state.resetFlags(LR35902Flags::CF);
	}
	void LR35902::adc(const uint8_t value)
	{
		const uint8_t flag = _state.carryFlag() ? 1 : 0;
		uint16_t sum = _state.a() + value + flag;
		auxCarryBit = ((value & 0x0f) + (_state.a() & 0x0f) + flag) > 0x0f;
		_state.a() = sum & 0xff;
		_state.adjustSZ(_state.a());
		parityBit = parity(_state.a());
		if (sum > 0xff)
			_state.setFlags(LR35902Flags::CF);
		else
			_state.resetFlags(LR35902Flags::CF);
	}
	void LR35902::daa()
	{
		uint16_t temp = _state.a();
		if (((_state.a() & 0xf) > 9) || (auxCarryBit)) {
			temp += 6;
			auxCarryBit = ((_state.a() & 0xf) > 9) ? 1 : 0;
		}
		if ((temp >> 4 > 9) || (_state.carryFlag())) {
			temp += 0x60;
			_state.setFlags(LR35902Flags::CF);
		}
		_state.a() = temp & 0xff;
		_state.adjustSZ(_state.a());
		parityBit = parity(_state.a());
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

	const uint8_t LR35902::executeOne() {
		if (interrupt_enabled == 0 && interrupt_request < 8) {
			interrupt_enabled = 2;
			push(_state.pc());
			_state.pc() = interrupt_request << 3;
			interrupt_request = 8;
		}
		else if (interrupt_enabled == 1)
			interrupt_enabled = 0;

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
		interrupt_enabled = 2;
		interrupt_request = 8;
		return Z80FamilyCpu::reset(address);
	}

	/*********************************************************************************************************************/
	void LR35902::decode_opcode(const uint8_t opcode) {
		if (opcode_tables[opcode] != opcodes::UNIMPLEMENTED) {
			switch (opcode_tables[opcode]) {
			default:
				COMMON_OPCODES_DECODING(opcodes, opcode_tables, opcode)
			}
			return;
		}
		uint16_t cycle = 0;
		uint16_t tmp16 = 0;

		switch (opcode) {
		case 0x01: /* LXI B */
			_state.c() = readArgument8();
			_state.b() = readArgument8();
			cycle = 10;
			break;
		case 0x02: /* STAX B */
			_handlerWrite(_state.bc(), _state.a());
			cycle = 7;
			break;
		case 0x03: /* INX B */
			_state.c()++;
			if (_state.c() == 0)
				_state.b()++;
			cycle = 5;
			break;
		case 0x07: /* RLC */
			if (_state.a() >> 7)
				_state.setFlags(LR35902Flags::CF);
			else
				_state.resetFlags(LR35902Flags::CF);
			_state.a() = (_state.carryFlag() ? 1 : 0) | (_state.a() << 1);
			cycle = 4;
			break;
		case 0x09: /* DAD BC */
			dad(_state.bc());
			cycle = 10;
			break;
		case 0x0A: /* LDAX B */
			_state.a() = _handlerRead(_state.bc());
			cycle = 7;
			break;
		case 0x0B: /* DCX B */
			_state.bc() = _state.bc() - 1;
			cycle = 5;
			break;
		case 0x0F: /* RRC */
			if (_state.a() & 1)
				_state.setFlags(LR35902Flags::CF);
			else
				_state.resetFlags(LR35902Flags::CF);
			_state.a() = (_state.carryFlag() ? 0x80 : 0) | (_state.a() >> 1);
			cycle = 4;
			break;

		case 0x11: /* LXI D */
			_state.e() = readArgument8();
			_state.d() = readArgument8();
			cycle = 10;
			break;
		case 0x12: /* STAX D */
			_handlerWrite(_state.de(), _state.a());
			cycle = 7;
			break;
		case 0x13: /* INX D */
			_state.e()++;
			if (_state.e() == 0)
				_state.d()++;
			cycle = 5;
			break;
		case 0x17: /* RAL */
		{
			uint8_t flag = (_state.carryFlag()) ? 1 : 0;
			if (_state.a() >> 7)
				_state.setFlags(LR35902Flags::CF);
			else
				_state.resetFlags(LR35902Flags::CF);

			_state.a() = (_state.a() << 1) | (flag);
		}
		cycle = 4;
		break;
		case 0x19: /* DAD D */
			dad(_state.de());
			cycle = 10;
			break;
		case 0x1A: /* LDAX D */
			_state.a() = _handlerRead(_state.de());
			cycle = 7;
			break;
		case 0x1B: /* DCX D */
			_state.de() = _state.de() - 1;
			cycle = 5;
			break;
		case 0x1F: /* RAR */
		{
			uint8_t flag = (_state.carryFlag()) ? 1 : 0;
			if (_state.a() & 1)
				_state.setFlags(LR35902Flags::CF);
			else
				_state.resetFlags(LR35902Flags::CF);
			_state.a() = (_state.a() >> 1) | (flag << 7);
		}
		cycle = 4;
		break;
		case 0x21: /* LXI H */
			_state.l() = readArgument8();
			_state.h() = readArgument8();
			cycle = 10;
			break;
		case 0x22: /* SHLD */
			tmp16 = readArgument16();
			_handlerWrite(tmp16, _state.l());
			_handlerWrite(tmp16 + 1, _state.h());
			cycle = 16;
			break;
		case 0x23: /* INX H */
			_state.l()++;
			if (_state.l() == 0)
				_state.h()++;
			cycle = 5;
			break;
		case 0x27: /* DAA */
			daa();
			cycle = 4;
			break;
		case 0x29: /* DAD H */
			dad(_state.hl());
			cycle = 10;
			break;
		case 0x2A: /* LHLD */
			tmp16 = readArgument16();
			_state.l() = _handlerRead(tmp16);
			_state.h() = _handlerRead(tmp16 + 1);
			cycle = 16;
			break;
		case 0x2B: /* DCX H */
			tmp16 = _state.hl() - 1;
			_state.h() = tmp16 >> 8;
			_state.l() = tmp16 & 0xff;
			cycle = 5;
			break;
		case 0x2F: /* CMA */
			_state.a() = ~_state.a();
			cycle = 4;
			break;

		case 0x31: /* LXI SP */
			_state.sp() = readArgument16();
			cycle = 10;
			break;
		case 0x32: /* STA */
			_handlerWrite(readArgument16(), _state.a());
			cycle = 13;
			break;
		case 0x33: /* INX SP */
			_state.sp() = _state.sp() + 1;
			cycle = 5;
			break;
		case 0x37: /* STC */
			_state.setFlags(LR35902Flags::CF);
			cycle = 4;
			break;
		case 0x39: /* DAD SP */
			dad(_state.sp());
			cycle = 10;
			break;
		case 0x3A: /* LDA */
			_state.a() = _handlerRead(readArgument16());
			cycle = 13;
			break;
		case 0x3B: /* DCX SP */
			_state.sp() = _state.sp() - 1;
			cycle = 5;
			break;
		case 0x3F: /* CMC */
			if (_state.carryFlag())
				_state.resetFlags(LR35902Flags::CF);
			else
				_state.setFlags(LR35902Flags::CF);
			cycle = 4;
			break;

		case 0x88: /* ADC B */
			adc(_state.b());
			cycle = 4;
			break;
		case 0x89: /* ADC C */
			adc(_state.c());
			cycle = 4;
			break;
		case 0x8A: /* ADC D */
			adc(_state.d());
			cycle = 4;
			break;
		case 0x8B: /* ADC E */
			adc(_state.e());
			cycle = 4;
			break;
		case 0x8C: /* ADC H */
			adc(_state.h());
			cycle = 4;
			break;
		case 0x8D: /* ADC L */
			adc(_state.l());
			cycle = 4;
			break;
		case 0x8E: /* ADC M */
			adc(get_m());
			cycle = 7;
			break;
		case 0x8F: /* ADC A */
			adc(_state.a());
			cycle = 4;
			break;


		case 0xC7: /* RST 0 */
			push(_state.pc());
			_state.pc() = 0;
			break;

			//		case 0xCF: /* RST 1 */

		case 0xD3: /* OUT */
			_handlerOut(readArgument8(), _state.a());
			cycle = 10;
			break;
			//		case 0xD7: /* RST 2 */
		case 0XDB: /* IN */
			_state.a() = _handlerIn(readArgument8());
			cycle = 10;
			break;
			//		case 0xDF: /* RST 3 */

		case 0xE3: /* XTHL */
			tmp16 = _handlerRead(_state.sp()) | (_handlerRead(_state.sp() + 1) << 8);
			_handlerWrite(_state.sp(), _state.l());
			_handlerWrite(_state.sp() + 1, _state.h());
			_state.hl() = tmp16;
			cycle = 18;
			break;
		case 0xE6: /* ANI */
			ana(readArgument8());
			cycle = 7;
			break;
			//		case 0XE7: /* RST 4 */
		case 0xE9: /* PCHL */
			_state.pc() = _state.hl();
			cycle = 5;
			break;
		case 0xEB: /* XCHG */ { uint8_t tmp = _state.d(); _state.d() = _state.h(); _state.h() = tmp; tmp = _state.e(); _state.e() = _state.l(); _state.l() = tmp; } cycle = 4; break;
		case 0xEE: /* XRI */
			xra(readArgument8());
			cycle = 7;
			break;
			//		case 0xEF: /* RST 5 */

		case 0xF3: /* DI */
			interrupt_enabled = 0;
			cycle = 4;
			break;
		case 0xF6: /* ORI */
			ora(readArgument8());
			cycle = 7;
			break;
			//		case 0xF7: /* RST 6 */
		case 0xF9: /* SPHL */
			_state.sp() = _state.hl();
			cycle = 5;
			break;
		case 0xFB: /* EI */
			interrupt_enabled = 1;
			cycle = 4;
			break;
		case 0xFE: /* CPI */
			cmp(readArgument8());
			cycle = 7;
			break;
			//		case 0xFF: /* RST 7 */

		default: unimplemented(opcode); break;
		}
	}
}

