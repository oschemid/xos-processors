#include "i8080.h"
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include "../registry.h"
#include "opcodes.h"


/*********************************************************************************************************************/
// REGISTERING
static xprocessors::CpuRegistryHandler reg("i8080", xprocessors::Intel8080::create);


/*********************************************************************************************************************/
// OPCODES
enum class opcodes {
	COMMON_OPCODES
};


/*********************************************************************************************************************/
// OPCODES TABLES
constexpr auto opcode_tables{ []() constexpr {
	COMMON_OPCODES_TABLE(opcodes, result)
	return result;
}()
};



namespace xprocessors {
	Intel8080::Intel8080() :
		Z80FamilyCpu() {
		reset();
	}
	uint8_t Intel8080::get_m() const {
		return _handlerRead(_state.hl());
	}

	uint8_t Intel8080::dec(const uint8_t value) {
		uint8_t result = value - 1;
		_state.adjustSZ(result);
		_state.setParityFlag(parity(result));
		_state.setFlag(Intel8080Flags::HF, !((result & 0x0f) == 0x0f));
		return result;
	}
	uint8_t Intel8080::inc(const uint8_t value) {
		uint8_t result = value + 1;
		_state.adjustSZ(result);
		_state.setParityFlag(parity(result));
		_state.setFlag(Intel8080Flags::HF, (result & 0x0f) == 0);
		return result;
	}

	void Intel8080::xra(const uint8_t value) {
		_state.a() ^= value;
		_state.resetFlags(Intel8080Flags::CF | Intel8080Flags::HF);
		_state.setParityFlag(parity(_state.a()));
		_state.adjustSZ(_state.a());
	}
	void Intel8080::ora(const uint8_t value) {
		_state.a() |= value;
		_state.resetFlags(Intel8080Flags::CF | Intel8080Flags::HF);
		_state.setParityFlag(parity(_state.a()));
		_state.adjustSZ(_state.a());
	}
	void Intel8080::ana(const uint8_t value) {
		uint8_t result = _state.a() & value;
		_state.resetFlags(Intel8080Flags::CF);
		_state.setParityFlag(parity(result));
		_state.adjustSZ(result);
		_state.setFlag(Intel8080Flags::HF, ((_state.a() | value) & 0x08) != 0);
		_state.a() = result;
	}
	void Intel8080::sub(const uint8_t value) {
		if (value > _state.a())
			_state.setFlags(Intel8080Flags::CF);
		else
			_state.resetFlags(Intel8080Flags::CF);
		_state.setFlag(Intel8080Flags::HF, (_state.a() & 0x0f) - (value & 0x0f) >= 0);
		_state.a() -= value;
		_state.adjustSZ(_state.a());
		_state.setParityFlag(parity(_state.a()));
	}
	void Intel8080::sbc(const uint8_t value) {
		const uint8_t flag = _state.carryFlag() ? 1 : 0;
		if (value + flag > _state.a())
			_state.setFlags(Intel8080Flags::CF);
		else
			_state.resetFlags(Intel8080Flags::CF);
		_state.setFlag(Intel8080Flags::HF, (_state.a() & 0x0f) - (value & 0x0f) - flag >= 0);
		_state.a() -= value + flag;
		_state.adjustSZ(_state.a());
		_state.setParityFlag(parity(_state.a()));
	}
	void Intel8080::cmp(const uint8_t value) {
		if (value > _state.a())
			_state.setFlags(Intel8080Flags::CF);
		else
			_state.resetFlags(Intel8080Flags::CF);
		_state.setFlag(Intel8080Flags::HF, (value & 0xf) > (_state.a() & 0xf) ? 0 : 1);
		uint8_t r = _state.a() - value;
		_state.adjustSZ(r);
		_state.setParityFlag(parity(r));
	}
	void Intel8080::add(const uint8_t value)
	{
		uint16_t sum = _state.a() + value;
		_state.setFlag(Intel8080Flags::HF, ((value & 0x0f) + (_state.a() & 0x0f)) > 0x0f);
		_state.a() = sum & 0xff;
		_state.adjustSZ(_state.a());
		_state.setParityFlag(parity(_state.a()));
		if (sum > 0xff)
			_state.setFlags(Intel8080Flags::CF);
		else
			_state.resetFlags(Intel8080Flags::CF);
	}
	void Intel8080::adc(const uint8_t value)
	{
		const uint8_t flag = _state.carryFlag() ? 1 : 0;
		uint16_t sum = _state.a() + value + flag;
		_state.setFlag(Intel8080Flags::HF, ((value & 0x0f) + (_state.a() & 0x0f) + flag) > 0x0f);
		_state.a() = sum & 0xff;
		_state.adjustSZ(_state.a());
		_state.setParityFlag(parity(_state.a()));
		if (sum > 0xff)
			_state.setFlags(Intel8080Flags::CF);
		else
			_state.resetFlags(Intel8080Flags::CF);
	}
	void Intel8080::daa()
	{
		uint16_t temp = _state.a();
		if (((_state.a() & 0xf) > 9) || (_state.halfCarryFlag())) {
			temp += 6;
			_state.setFlag(Intel8080Flags::HF, ((_state.a() & 0xf) > 9) ? 1 : 0);
		}
		if ((temp >> 4 > 9) || (_state.carryFlag())) {
			temp += 0x60;
			_state.setFlags(Intel8080Flags::CF);
		}
		_state.a() = temp & 0xff;
		_state.adjustSZ(_state.a());
		_state.setParityFlag(parity(_state.a()));
	}
	void Intel8080::add_hl(const uint16_t value)
	{
		uint32_t res = _state.hl() + value;
		_state.hl() = res & 0xffff;
		if ((res & 0xffff0000) != 0)
			_state.setFlags(Intel8080Flags::CF);
		else
			_state.resetFlags(Intel8080Flags::CF);
		_elapsed_cycles += 6;
	}

	uint8_t Intel8080::executeOne() {
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

	bool Intel8080::interrupt(const uint8_t inte) {
		if (interrupt_enabled == 0)
			interrupt_request = inte;
		return (interrupt_enabled == 0) ? true : false;
	}

	bool Intel8080::reset(const uint16_t address) {
		_state.pc() = address;
		_elapsed_cycles = 0;
		_executed_instructions = 0;
		interrupt_enabled = 2;
		interrupt_request = 8;
		Z80FamilyCpu::reset(address);
		return true;
	}

	/*********************************************************************************************************************/
	void Intel8080::decode_opcode(const uint8_t opcode) {
		uint16_t tmp16 = 0;
		if (opcode_tables[opcode] != opcodes::UNIMPLEMENTED) {
			COMMON_OPCODES_DECODING(opcodes, opcode_tables, opcode)
				return;
		}

		switch (opcode) {
		case 0x07: /* RLC */
			if (_state.a() >> 7)
				_state.setFlags(Intel8080Flags::CF);
			else
				_state.resetFlags(Intel8080Flags::CF);
			_state.a() = (_state.carryFlag() ? 1 : 0) | (_state.a() << 1);
			break;
		case 0x0F: /* RRC */
			if (_state.a() & 1)
				_state.setFlags(Intel8080Flags::CF);
			else
				_state.resetFlags(Intel8080Flags::CF);
			_state.a() = (_state.carryFlag() ? 0x80 : 0) | (_state.a() >> 1);
			break;

		case 0x17: /* RAL */
		{
			uint8_t flag = (_state.carryFlag()) ? 1 : 0;
			if (_state.a() >> 7)
				_state.setFlags(Intel8080Flags::CF);
			else
				_state.resetFlags(Intel8080Flags::CF);

			_state.a() = (_state.a() << 1) | (flag);
		}
		break;
		case 0x1F: /* RAR */
		{
			uint8_t flag = (_state.carryFlag()) ? 1 : 0;
			if (_state.a() & 1)
				_state.setFlags(Intel8080Flags::CF);
			else
				_state.resetFlags(Intel8080Flags::CF);
			_state.a() = (_state.a() >> 1) | (flag << 7);
		}
		break;
		case 0x22: /* SHLD */
			write16(readArgument16(), _state.hl());
			break;
		case 0x27: /* DAA */
			daa();
			break;
		case 0x2A: /* LHLD */
			_state.hl() = read16(readArgument16());
			break;
		case 0x2F: /* CMA */
			_state.a() = ~_state.a();
			break;

		case 0x32: /* STA */
			write8(readArgument16(), _state.a());
			break;
		case 0x37: /* STC */
			_state.setFlags(Intel8080Flags::CF);
			break;
		case 0x3A: /* LDA */
			_state.a() = read8(readArgument16());
			break;
		case 0x3F: /* CMC */
			if (_state.carryFlag())
				_state.resetFlags(Intel8080Flags::CF);
			else
				_state.setFlags(Intel8080Flags::CF);
			break;

		case 0xC7: /* RST 0 */
			push(_state.pc());
			_state.pc() = 0;
			break;
			//		case 0xCF: /* RST 1 */

		case 0xD3: /* OUT */
			_handlerOut(readArgument8(), _state.a());
			_elapsed_cycles += 3;
			break;
			//		case 0xD7: /* RST 2 */
		case 0XDB: /* IN */
			_state.a() = _handlerIn(readArgument8());
			_elapsed_cycles += 3;
			break;
			//		case 0xDF: /* RST 3 */

		case 0xE3: /* XTHL */
			tmp16 = read16(_state.sp());
			write16(_state.sp(), _state.hl());
			_state.hl() = tmp16;
			_elapsed_cycles += 2;
			break;
			//		case 0XE7: /* RST 4 */
		case 0xE9: /* PCHL */
			_state.pc() = _state.hl();
			_elapsed_cycles++;
			break;
		case 0xEB: /* XCHG */
			tmp16 = _state.de();
			_state.de() = _state.hl();
			_state.hl() = tmp16;
			break;
			//		case 0xEF: /* RST 5 */

		case 0xF3: /* DI */
			interrupt_enabled = 0;
			break;
			//		case 0xF7: /* RST 6 */
		case 0xF9: /* SPHL */
			_state.sp() = _state.hl();
			_elapsed_cycles++;
			break;
		case 0xFB: /* EI */
			interrupt_enabled = 1;
			break;
			//		case 0xFF: /* RST 7 */

		default: unimplemented(opcode); break;
		}
	}
}
