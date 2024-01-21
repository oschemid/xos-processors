#include "z80oldsolution.h"
#include "xprocessors.h"
#include "opcodes_cb.h"


enum class opcode {
	UNIMPLEMENTED,
	COMMON_OPCODES_CB
};


/*********************************************************************************************************************/
// opcodes table
constexpr auto opcodes{ []() constexpr {
	COMMON_OPCODES_TABLE_CB(opcode, result)
	return result;
}()
};

using namespace xprocessors;

/*********************************************************************************************************************/
void Z80Old::decode_opcode_cb(const prefix p) {
	const int8_t delta = (p != NO) ? static_cast<int8_t>(readArgument8()) : 0;
	const opcode_t opcode = readOpcode();

	switch (opcodes[opcode]) {
	case opcode::RLC_R:
		apply_ixy_r([this](const uint8_t r) { return rlc(r); }, opcode, p, delta);
		break;
	case opcode::RLC_HL:
		apply_hl([this](const uint8_t r) { return rlc(r); }, p, delta);
		break;
	case opcode::RL_R:
		apply_ixy_r([this](const uint8_t r) { return rl(r); }, opcode, p, delta);
		break;
	case opcode::RL_HL:
		apply_hl([this](const uint8_t r) { return rl(r); }, p, delta);
		break;
	case opcode::RR_R:
		apply_ixy_r([this](const uint8_t r) { return rr(r); }, opcode, p, delta);
		break;
	case opcode::RR_HL:
		apply_hl([this](const uint8_t r) { return rr(r); }, p, delta);
		break;
	case opcode::RRC_R:
		apply_ixy_r([this](const uint8_t r) { return rrc(r); }, opcode, p, delta);
		break;
	case opcode::RRC_HL:
		apply_hl([this](const uint8_t r) { return rrc(r); }, p, delta);
		break;
	case opcode::SRL_R:
		apply_ixy_r([this](const uint8_t r) { return srl(r); }, opcode, p, delta);
		break;
	case opcode::SRL_HL:
		apply_hl([this](const uint8_t r) { return srl(r); }, p, delta);
		break;
	case opcode::SRA_R:
		apply_ixy_r([this](const uint8_t r) { return sra(r); }, opcode, p, delta);
		break;
	case opcode::SRA_HL:
		apply_hl([this](const uint8_t r) { return sra(r); }, p, delta);
		break;
	case opcode::SLA_R:
		apply_ixy_r([this](const uint8_t r) { return sla(r); }, opcode, p, delta);
		break;
	case opcode::SLA_HL:
		apply_hl([this](const uint8_t r) { return sla(r); }, p, delta);
		break;
	case opcode::SLL_R:
		apply_ixy_r([this](const uint8_t r) { return sll(r); }, opcode, p, delta);
		break;
	case opcode::SLL_HL:
		apply_hl([this](const uint8_t r) { return sll(r); }, p, delta);
		break;
	case opcode::BIT_R:
		bit(decodeR(opcode), (opcode & 0b00111000) >> 3);
		break;
	case opcode::BIT_HL:
		if (p == DD) {
			bit(read8(_state.ix() + delta), (opcode & 0b00111000) >> 3);
			_elapsed_cycles += 2;
		}
		else if (p == FD) {
			bit(read8(_state.iy() + delta), (opcode & 0b00111000) >> 3);
			_elapsed_cycles += 2;
		}
		else {
			bit(read8(_state.hl()), (opcode & 0b00111000) >> 3);
			_elapsed_cycles++;
		}
		break;
	case opcode::SET_R:
		apply_ixy_r([this, opcode](const uint8_t r) { return r | (1 << ((opcode & 0b00111000) >> 3)); }, opcode, p, delta);
		break;
	case opcode::SET_HL:
		apply_hl([this, opcode](const uint8_t r) { return r | (1 << ((opcode & 0b00111000) >> 3)); }, p, delta);
		break;
	case opcode::RES_R:
		apply_ixy_r([this, opcode](const uint8_t r) { return r & ~(1 << ((opcode & 0b00111000) >> 3)); }, opcode, p, delta);
		break;
	case opcode::RES_HL:
		apply_hl([this, opcode](const uint8_t r) { return r & ~(1 << ((opcode & 0b00111000) >> 3)); }, p, delta);
		break;
	default:
		throw std::runtime_error("Unexpected opcode in decode_cb : " + opcode);
	}
}
/*********************************************************************************************************************/

uint8_t Z80Old::rlc(const uint8_t value) {
	const uint8_t result = (value << 1) | (value >> 7);
	_state.f() = (value & 0x80) ? Z80State::CF : 0;
	_state.setSZXY(result);
	_state.setP(result);
	return result;
}
uint8_t Z80Old::rrc(const uint8_t value) {
	const uint8_t result = (value >> 1) | (value << 7);
	_state.f() = (value & 0x01) ? Z80State::CF : 0;
	_state.setSZXY(result);
	_state.setP(result);
	return result;
}
uint8_t Z80Old::rr(const uint8_t value) {
	const uint8_t result = (value >> 1) | ((_state.carryFlag()) ? 0x80 : 0);
	_state.f() = (value & 0x01) ? Z80State::CF : 0;
	_state.setSZXY(result);
	_state.setP(result);
	return result;
}
uint8_t Z80Old::rl(const uint8_t value) {
	const uint8_t result = (value << 1) | ((_state.carryFlag()) ? 0x01 : 0);
	_state.f() = (value & 0x80) ? Z80State::CF : 0;
	_state.setSZXY(result);
	_state.setP(result);
	return result;
}
uint8_t Z80Old::srl(const uint8_t value) {
	const uint8_t result = value >> 1;
	_state.f() = (value & 0x01) ? Z80State::CF : 0;
	_state.setSZXY(result);
	_state.setP(result);
	return result;
}
uint8_t Z80Old::sra(const uint8_t value) {
	const uint8_t result = value >> 1 | (value & 0x80);
	_state.f() = (value & 0x01) ? Z80State::CF : 0;
	_state.setSZXY(result);
	_state.setP(result);
	return result;
}
uint8_t Z80Old::sla(const uint8_t value) {
	const uint8_t result = value << 1;
	_state.f() = (value & 0x80) ? Z80State::CF : 0;
	_state.setSZXY(result);
	_state.setP(result);
	return result;
}
uint8_t Z80Old::sll(const uint8_t value) {
	const uint8_t result = (value << 1) | 1;
	_state.f() = (value & 0x80) ? Z80State::CF : 0;
	_state.setSZXY(result);
	_state.setP(result);
	return result;
}
void Z80Old::bit(const uint8_t value, const uint8_t b) {
	_state.f() &= Z80State::CF;
	_state.f() |= Z80State::HF;
	_state.f() |= (value & (1 << b)) ? Z80State::SF : Z80State::ZF | Z80State::VF;
}