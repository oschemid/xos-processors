#include "z80.h"
#include "types.h"
#include "opcodes.h"


enum class opcode {
	COMMON_OPCODES,
	LD_R_HL,
	LD_HL_R,

	DI,
	EI,
	DECODE_CB,
	DECODE_DD,
	DECODE_ED,
	DECODE_FD,

	ADD_HL_SS,
	ADD_N,
	ADD_R,
	ADD_HL,
	ADC_N,
	ADC_R,
	ADC_HL,
	SUB_N,
	SUB_R,
	SUB_HL,
	SBC_N,
	SBC_R,
	SBC_HL,
	AND_N,
	AND_R,
	AND_HL,
	XOR_N,
	XOR_R,
	XOR_HL,
	OR_N,
	OR_R,
	OR_HL,
	CP_N,
	CP_R,
	CP_HL,
	INC_R,
	INC_HL,
	INC_SS,
	DEC_R,
	DEC_HL,
	DEC_SS,
	DAA,
	CPL,
	SCF,
	CCF,
	LD_HL_NNNN,
	LD_NNNN_HL,
	LD_SS_NN,
	LD_A_DE,
	LD_DE_A,
	LD_A_BC,
	LD_BC_A,
	LD_HL_N,
	LD_A_NNNN,
	LD_NNNN_A,
	RLCA,
	RRCA,
	RLA,
	RRA,
	EX_DE,
	EX_AF,
	EXX,
	EX_SP,
	LD_SP_HL,
	JP_HL,
	JR,
	JR_C,
	DJNZ,
	RET,
	RET_CC,
	RST,
	IN_N,
	OUT_N,
};


/*********************************************************************************************************************/
// OPCODES TABLES
constexpr auto opcodes{ []() constexpr {
	COMMON_OPCODES_TABLE(opcode, result)
	for (int i = 0; i < 256; ++i) {
		if (result[i] == opcode::LD_R_R) {
			if ((i & 7) == 0b110)
				result[i] = opcode::LD_R_HL;
			if ((i & 0x38) == 0b00110000)
				result[i] = opcode::LD_HL_R;
		}
		if ((i & 0b11000111) == 0b00000110)
			result[i] = (i == 0b00110110) ? opcode::LD_HL_N : opcode::LD_R_N;
		if ((i & 0b11001111) == 0b00000001)
			result[i] = opcode::LD_SS_NN;
		if ((i & 0b11001111) == 0b00001001)
			result[i] = opcode::ADD_HL_SS;
		if ((i & 0b11000111) == 0b00000100)
			result[i] = (i == 0b00110100) ? opcode::INC_HL : opcode::INC_R;
		if ((i & 0b11001111) == 0b00000011)
			result[i] = opcode::INC_SS;
		if ((i & 0b11000111) == 0b00000101)
			result[i] = (i == 0b00110101) ? opcode::DEC_HL : opcode::DEC_R;
		if ((i & 0b11001111) == 0b00001011)
			result[i] = opcode::DEC_SS;
		if ((i & 0b11111000) == 0b10000000)
			result[i] = ((i & 0x07) == 0x06) ? opcode::ADD_HL : opcode::ADD_R;
		if ((i & 0b11111000) == 0b10001000)
			result[i] = ((i & 0x07) == 0x06) ? opcode::ADC_HL : opcode::ADC_R;
		if ((i & 0b11111000) == 0b10010000)
			result[i] = ((i & 0x07) == 0x06) ? opcode::SUB_HL : opcode::SUB_R;
		if ((i & 0b11111000) == 0b10011000)
			result[i] = ((i & 0x07) == 0x06) ? opcode::SBC_HL : opcode::SBC_R;
		if ((i & 0b11111000) == 0b10100000)
			result[i] = ((i & 0x07) == 0x06) ? opcode::AND_HL : opcode::AND_R;
		if ((i & 0b11111000) == 0b10101000)
			result[i] = ((i & 0x07) == 0x06) ? opcode::XOR_HL : opcode::XOR_R;
		if ((i & 0b11111000) == 0b10110000)
			result[i] = ((i & 0x07) == 0x06) ? opcode::OR_HL : opcode::OR_R;
		if ((i & 0b11111000) == 0b10111000)
			result[i] = ((i & 0x07) == 0x06) ? opcode::CP_HL : opcode::CP_R;
		if ((i & 0b11100111) == 0b00100000)
			result[i] = opcode::JR_C;
		if ((i & 0b11000111) == 0b11000000)
			result[i] = opcode::RET_CC;
		if ((i & 0b11000111) == 0b11000111)
			result[i] = opcode::RST;
		switch (i) {
		case 0x02:
			result[i] = opcode::LD_BC_A;
			break;
		case 0x07:
			result[i] = opcode::RLCA;
			break;
		case 0x08:
			result[i] = opcode::EX_AF;
			break;
		case 0x0A:
			result[i] = opcode::LD_A_BC;
			break;
		case 0x0F:
			result[i] = opcode::RRCA;
			break;
		case 0x10:
			result[i] = opcode::DJNZ;
			break;
		case 0x12:
			result[i] = opcode::LD_DE_A;
			break;
		case 0x17:
			result[i] = opcode::RLA;
			break;
		case 0x18:
			result[i] = opcode::JR;
			break;
		case 0x1A:
			result[i] = opcode::LD_A_DE;
			break;
		case 0x1F:
			result[i] = opcode::RRA;
			break;
		case 0x22:
			result[i] = opcode::LD_NNNN_HL;
			break;
		case 0x27:
			result[i] = opcode::DAA;
			break;
		case 0x2A:
			result[i] = opcode::LD_HL_NNNN;
			break;
		case 0x2F:
			result[i] = opcode::CPL;
			break;
		case 0x32:
			result[i] = opcode::LD_NNNN_A;
			break;
		case 0x37:
			result[i] = opcode::SCF;
			break;
		case 0x3A:
			result[i] = opcode::LD_A_NNNN;
			break;
		case 0x3F:
			result[i] = opcode::CCF;
			break;
		case 0xC3:
			result[i] = opcode::JP;
			break;
		case 0xC6:
			result[i] = opcode::ADD_N;
			break;
		case 0xc9:
			result[i] = opcode::RET;
			break;
		case 0xCB:
			result[i] = opcode::DECODE_CB;
			break;
			case 0xCE:
				result[i] = opcode::ADC_N;
				break;
			case 0xD3:
				result[i] = opcode::OUT_N;
				break;
			case 0xD6:
				result[i] = opcode::SUB_N;
				break;
			case 0xD9:
				result[i] = opcode::EXX;
				break;
			case 0xDB:
				result[i] = opcode::IN_N;
				break;
			case 0xDD:
				result[i] = opcode::DECODE_DD;
				break;
			case 0xDE:
				result[i] = opcode::SBC_N;
				break;
			case 0xE3:
				result[i] = opcode::EX_SP;
				break;
			case 0xE6:
				result[i] = opcode::AND_N;
				break;
			case 0xE9:
				result[i] = opcode::JP_HL;
				break;
			case 0xEB:
				result[i] = opcode::EX_DE;
				break;
			case 0xED:
				result[i] = opcode::DECODE_ED;
				break;
			case 0xEE:
				result[i] = opcode::XOR_N;
				break;
			case 0xF3:
				result[i] = opcode::DI;
				break;
			case 0xF6:
				result[i] = opcode::OR_N;
				break;
			case 0xF9:
				result[i] = opcode::LD_SP_HL;
				break;
			case 0xFB:
				result[i] = opcode::EI;
				break;
			case 0xFD:
				result[i] = opcode::DECODE_FD;
				break;
			case 0xFE:
				result[i] = opcode::CP_N;
				break;
			}
		}
		return result;
	}()
};


using namespace xprocessors;

/*********************************************************************************************************************/
void Z80::decode_opcode(const uint8_t opcode) {
	uint16_t tmp16;
	uint8_t tmp8;

	switch (opcodes[opcode]) {
	case opcode::DI:
		iff1 = false;
		iff2 = false;
		break;
	case opcode::EI:
		iff1 = true;
		iff2 = true;
		iff1_waiting = true;
		break;
	case opcode::DECODE_CB:
		decode_opcode_cb(current_prefix);
		break;
	case opcode::DECODE_DD:
		if (current_prefix != NO)
			unimplemented();
		else {
			current_prefix = DD;
			decode_opcode(readOpcode());
			current_prefix = NO;
		}
		break;
	case opcode::DECODE_ED:
		decode_opcode_ed();
		break;
	case opcode::DECODE_FD:
		if (current_prefix != NO)
			unimplemented();
		else {
			current_prefix = FD;
			decode_opcode(readOpcode());
			current_prefix = NO;
		}
		break;
	case opcode::LD_HL_R:
		apply_hl([this, opcode](const uint8_t) { prefix tmp = current_prefix; current_prefix = NO; uint8_t t = decodeR(opcode); current_prefix = tmp; return t; }, current_prefix, (current_prefix != NO) ? readArgument8() : 0);
		if (current_prefix == NO)
			_elapsed_cycles -= 3;
		else
			_elapsed_cycles += 2;
		break;
	case opcode::LD_SP_HL:
		if (current_prefix == NO)
			_state.sp() = _state.hl();
		else if (current_prefix == DD)
			_state.sp() = _state.ix();
		else
			_state.sp() = _state.iy();
		_elapsed_cycles += 2;
		break;
	case opcode::LD_R_HL:
		if (current_prefix == NO) {
			decodeR(opcode >> 3, read8(_state.hl()));
		}
		else if (current_prefix == DD) {
			current_prefix = NO;
			decodeR(opcode >> 3, read8(_state.ix() + static_cast<int8_t>(readArgument8())));
			current_prefix = DD;
			_elapsed_cycles += 5;
		}
		else if (current_prefix == FD) {
			current_prefix = NO;
			decodeR(opcode >> 3, read8(_state.iy() + static_cast<int8_t>(readArgument8())));
			current_prefix = FD;
			_elapsed_cycles += 5;
		}
		break;
	case opcode::LD_HL_N:
	{
		int8_t delta = (current_prefix != NO) ? readArgument8() : 0;
		apply_hl([this](const uint8_t) { return readArgument8(); }, current_prefix, delta);
		_elapsed_cycles -= 4;
		//if (p == NO)
		//	_elapsed_cycles--;
	}
	break;
	case opcode::LD_A_BC:
		_state.a() = read8(_state.bc());
		//		_elapsed_cycles--;
		break;
	case opcode::LD_A_DE:
		_state.a() = read8(_state.de());
		//		_elapsed_cycles--;
		break;
	case opcode::LD_BC_A:
		write8(_state.bc(), _state.a());
		break;
	case opcode::LD_DE_A:
		write8(_state.de(), _state.a());
		break;
	case opcode::LD_SS_NN:
		decodeRR(opcode, readArgument16());
		break;
	case opcode::LD_A_NNNN:
		_state.a() = read8(readArgument16());
		//		_elapsed_cycles--;
		break;
	case opcode::LD_NNNN_A:
		write8(readArgument16(), _state.a());
		break;
	case opcode::LD_HL_NNNN:
		tmp16 = readArgument16();
		if (current_prefix == NO)
			_state.hl() = read16(tmp16);
		else if (current_prefix == DD)
			_state.ix() = read16(tmp16);
		else if (current_prefix == FD)
			_state.iy() = read16(tmp16);
		break;

	case opcode::LD_NNNN_HL:
		write16(readArgument16(), decodeRR(opcode));
		break;

	case opcode::ADD_HL_SS:
		add_ss(opcode, current_prefix);
		break;
	case opcode::ADD_N:
		add(readArgument8());
		//		_elapsed_cycles--;
		break;
	case opcode::ADD_R:
		add(decodeR(opcode));
		break;
	case opcode::ADD_HL:
		if (current_prefix == DD) {
			add(read8(_state.ix() + static_cast<signed char>(readArgument8())));
			_elapsed_cycles += 3;
		}
		else if (current_prefix == FD) {
			add(read8(_state.iy() + static_cast<signed char>(readArgument8())));
			_elapsed_cycles += 3;
		}
		else {
			add(read8(_state.hl()));
			//			_elapsed_cycles--;
		}
		break;
	case opcode::ADC_N:
		add(readArgument8(), (_state.carryFlag()) ? 1 : 0);
		//		_elapsed_cycles--;
		break;
	case opcode::ADC_R:
		add(decodeR(opcode), (_state.carryFlag()) ? 1 : 0);
		break;
	case opcode::ADC_HL:
		if (current_prefix == DD) {
			add(read8(_state.ix() + static_cast<signed char>(readArgument8())), (_state.carryFlag()) ? 1 : 0);
			_elapsed_cycles += 3;
		}
		else if (current_prefix == FD) {
			add(read8(_state.iy() + static_cast<signed char>(readArgument8())), (_state.carryFlag()) ? 1 : 0);
			_elapsed_cycles += 3;
		}
		else {
			add(read8(_state.hl()), (_state.carryFlag()) ? 1 : 0);
			//			_elapsed_cycles--;
		}
		break;
	case opcode::SUB_N:
		sub(readArgument8());
		//		_elapsed_cycles--;
		break;
	case opcode::SUB_R:
		sub(decodeR(opcode));
		break;
	case opcode::SUB_HL:
		if (current_prefix == DD) {
			sub(read8(_state.ix() + static_cast<signed char>(readArgument8())));
			_elapsed_cycles += 3;
		}
		else if (current_prefix == FD) {
			sub(read8(_state.iy() + static_cast<signed char>(readArgument8())));
			_elapsed_cycles += 3;
		}
		else {
			sub(read8(_state.hl()));
			_elapsed_cycles--;
		}
		break;
	case opcode::SBC_N:
		sub(readArgument8(), (_state.carryFlag()) ? 1 : 0);
		_elapsed_cycles--;
		break;
	case opcode::SBC_R:
		sub(decodeR(opcode), (_state.carryFlag()) ? 1 : 0);
		break;
	case opcode::SBC_HL:
		if (current_prefix == DD) {
			sub(read8(_state.ix() + static_cast<signed char>(readArgument8())), (_state.carryFlag()) ? 1 : 0);
			_elapsed_cycles += 3;
		}
		else if (current_prefix == FD) {
			sub(read8(_state.iy() + static_cast<signed char>(readArgument8())), (_state.carryFlag()) ? 1 : 0);
			_elapsed_cycles += 3;
		}
		else {
			sub(read8(_state.hl()), (_state.carryFlag()) ? 1 : 0);
			_elapsed_cycles--;
		}
		break;
	case opcode::AND_N:
		ana(readArgument8());
		_elapsed_cycles--;
		break;
	case opcode::AND_R:
		ana(decodeR(opcode));
		break;
	case opcode::AND_HL:
		if (current_prefix == DD) {
			ana(read8(_state.ix() + static_cast<signed char>(readArgument8())));
			_elapsed_cycles += 3;
		}
		else if (current_prefix == FD) {
			ana(read8(_state.iy() + static_cast<signed char>(readArgument8())));
			_elapsed_cycles += 3;
		}
		else {
			ana(read8(_state.hl()));
			_elapsed_cycles--;
		}
		break;
	case opcode::XOR_N:
		xra(readArgument8());
		_elapsed_cycles--;
		break;
	case opcode::XOR_R:
		xra(decodeR(opcode));
		break;
	case opcode::XOR_HL:
		if (current_prefix == DD) {
			xra(read8(_state.ix() + static_cast<signed char>(readArgument8())));
			_elapsed_cycles += 3;
		}
		else if (current_prefix == FD) {
			xra(read8(_state.iy() + static_cast<signed char>(readArgument8())));
			_elapsed_cycles += 3;
		}
		else {
			xra(read8(_state.hl()));
			_elapsed_cycles--;
		}
		break;
	case opcode::OR_N:
		ora(readArgument8());
		_elapsed_cycles--;
		break;
	case opcode::OR_R:
		ora(decodeR(opcode));
		break;
	case opcode::OR_HL:
		if (current_prefix == DD) {
			ora(read8(_state.ix() + static_cast<signed char>(readArgument8())));
			_elapsed_cycles += 3;
		}
		else if (current_prefix == FD) {
			ora(read8(_state.iy() + static_cast<signed char>(readArgument8())));
			_elapsed_cycles += 3;
		}
		else {
			ora(read8(_state.hl()));
			_elapsed_cycles--;
		}
		break;
	case opcode::CP_N:
		cp(readArgument8());
		_elapsed_cycles--;
		break;
	case opcode::CP_R:
		cp(decodeR(opcode));
		break;
	case opcode::CP_HL:
		if (current_prefix == DD) {
			cp(read8(_state.ix() + static_cast<signed char>(readArgument8())));
			_elapsed_cycles += 3;
		}
		else if (current_prefix == 2) {
			cp(read8(_state.iy() + static_cast<signed char>(readArgument8())));
			_elapsed_cycles += 3;
		}
		else {
			cp(read8(_state.hl()));
			_elapsed_cycles--;
		}
		break;
	case opcode::DAA:
		daa();
		break;
	case opcode::CPL:
		_state.a() = ~_state.a();
		_state.setFlags(Z80State::HF | Z80State::NF);
		break;
	case opcode::SCF:
		_state.setFlags(Z80State::CF);
		_state.resetFlags(Z80State::NF | Z80State::XF | Z80State::YF | Z80State::HF);
		_state.f() |= _state.a() & (Z80State::XF | Z80State::YF);
		break;
	case opcode::CCF:
		if (_state.carryFlag()) {
			_state.resetFlags(Z80State::NF | Z80State::CF | Z80State::XF | Z80State::YF);
			_state.setFlags(Z80State::HF);
		}
		else {
			_state.resetFlags(Z80State::NF | Z80State::HF | Z80State::XF | Z80State::YF);
			_state.setFlags(Z80State::CF);
		}
		_state.f() |= _state.a() & (Z80State::XF | Z80State::YF);
		break;
	case opcode::RLCA:
		rla(true);
		break;
	case opcode::RRCA:
		rra(true);
		break;
	case opcode::RLA:
		rla(false);
		break;
	case opcode::RRA:
		rra(false);
		break;
	case opcode::INC_R:
		decodeR(opcode >> 3, inc(decodeR(opcode >> 3)));
		//		apply_r([this](const uint8_t r) { return inc(r); }, opcode >> 3, current_prefix);
		break;
	case opcode::INC_HL:
		apply_hl([this](const uint8_t r) { return inc(r); }, current_prefix, (current_prefix == NO) ? 0 : static_cast<int8_t>(readArgument8()));
		if (current_prefix != NO)
			_elapsed_cycles += 4;
		break;
	case opcode::DEC_R:
		decodeR(opcode >> 3, dec(decodeR(opcode >> 3)));
		//		apply_r([this](const uint8_t r) { return dec(r); }, opcode >> 3, current_prefix);
		break;
	case opcode::DEC_HL:
		apply_hl([this](const uint8_t r) { return dec(r); }, current_prefix, (current_prefix == NO) ? 0 : static_cast<int8_t>(readArgument8()));
		if (current_prefix != NO)
			_elapsed_cycles += 4;
		break;
	case opcode::INC_SS:
		decodeRR(opcode, decodeRR(opcode) + 1);
		_elapsed_cycles += 2;
		break;
	case opcode::DEC_SS:
		decodeRR(opcode, decodeRR(opcode) - 1);
		_elapsed_cycles += 2;
		break;
	case opcode::EX_AF:
		_state.exchange(Z80State::AF);
		break;
	case opcode::EXX:
		_state.exchange(Z80State::BC);
		_state.exchange(Z80State::DE);
		_state.exchange(Z80State::HL);
		break;
	case opcode::EX_DE:
		exchange_de_hl();
		break;
	case opcode::EX_SP:
		if (current_prefix == NO)
			_state.hl() = exchange_sp(_state.hl());
		else if (current_prefix == DD)
			_state.ix() = exchange_sp(_state.ix());
		else
			_state.iy() = exchange_sp(_state.iy());
		_elapsed_cycles += 3;
		break;
	case opcode::JP_HL:
		_state.pc() = decodeRR(0x20);
		break;
	case opcode::JR:
		tmp8 = readArgument8();
		_state.pc() = _state.pc() + static_cast<signed char>(tmp8);
		_elapsed_cycles += 4;
		break;
	case opcode::JR_C:
		tmp8 = readArgument8();
		if (checkCondition2(opcode)) {
			_state.pc() = _state.pc() + static_cast<signed char>(tmp8);
			_elapsed_cycles += 4;
		}
		else {
			_elapsed_cycles--;
		}
		break;
	case opcode::DJNZ:
		tmp8 = readArgument8();
		if (--_state.b() == 0) {
		}
		else {
			_state.pc() = _state.pc() + static_cast<signed char>(tmp8);
			_elapsed_cycles += 5;
		}
		break;
	case opcode::RET:
		_state.pc() = pop();
		break;
	case opcode::RET_CC:
		if (checkCC(opcode))
			_state.pc() = pop();
		_elapsed_cycles++;
		break;
	case opcode::RST:
		push(_state.pc());
		_state.pc() = opcode - 0xc7;
		_elapsed_cycles++;
		break;
	case opcode::IN_N:
		_state.a() = _handlerIn(readArgument8());
		_elapsed_cycles += 3;
		break;
	case opcode::OUT_N:
		_handlerOut(readArgument8(), _state.a());
		_elapsed_cycles += 3;
		break;
	default:
		COMMON_OPCODES_DECODING(opcode, opcodes, opcode)
			break;
	}
}

/**********************************************************************************************************************/
void Z80::add_ss(const opcode_t opcode, const prefix p) {
	const uint16_t a = decodeRR(0x20);
	const uint16_t b = decodeRR(opcode);
	const uint16_t s = a + b;

	decodeRR(0x20, s);
	_state.resetFlags(Z80State::NF | Z80State::CF | Z80State::HF);
	if (a > 0xffff - b)
		_state.setFlags(Z80State::CF);
	if (((s ^ a ^ b) >> 11) & 0x1)
		_state.setFlags(Z80State::HF);
	_elapsed_cycles += 7;
}
void Z80::rla(const bool c) {
	const bool carry = (c) ? _state.a() & 0x80 : _state.carryFlag();

	_state.resetFlags(Z80State::NF | Z80State::HF | Z80State::CF);
	if (_state.a() & 0x80)
		_state.setFlags(Z80State::CF);
	_state.a() <<= 1;
	_state.a() |= (carry) ? 1 : 0;
}
void Z80::rra(const bool c) {
	const bool carry = (c) ? _state.a() & 0x01 : _state.carryFlag();

	_state.resetFlags(Z80State::NF | Z80State::HF | Z80State::CF);
	if (_state.a() & 0x01)
		_state.setFlags(Z80State::CF);
	_state.a() >>= 1;
	_state.a() |= (carry) ? 0x80 : 0;
}
void Z80::daa() {
	uint8_t carry = 0;
	uint8_t diff = 0;
	if ((_state.carryFlag()) || (_state.a() > 0x99)) {
		diff += 0x60;
		carry = Z80State::CF;
	}
	if ((_state.halfCarryFlag()) || ((_state.a() & 0xf) > 9))
		diff += 6;

	if (_state.addSubFlag()) {
		if ((_state.a() & 0xf) > 5)
			_state.resetFlags(Z80State::HF);
		_state.a() -= diff;
	}
	else {
		if ((_state.a() & 0xf) > 9)
			_state.setFlags(Z80State::HF);
		else
			_state.resetFlags(Z80State::HF);
		_state.a() += diff;
	}
	_state.setSZXY(_state.a());
	_state.setP(_state.a());
	_state.f() |= carry;
}
void Z80::add(const uint8_t value, const uint8_t flag)
{
	const uint16_t sum = _state.a() + value + flag;
	const uint16_t carryIns = sum ^ _state.a() ^ value;

	_state.a() = sum & 0xff;

	_state.resetFlags(Z80State::NF | Z80State::HF | Z80State::PF | Z80State::CF);
	_state.setSZXY(_state.a());
	if (sum > 0xff)
		_state.setFlags(Z80State::CF);
	if (carryIns & 0x10)
		_state.setFlags(Z80State::HF);
	if (((carryIns >> 7) & 0x1) ^ ((carryIns >> 8) & 0x1))
		_state.setFlags(Z80State::PF);
}
void Z80::sub(const uint8_t value, const uint8_t flag)
{
	const uint16_t sum = _state.a() - value - flag;
	const uint16_t carryIns = (sum ^ _state.a() ^ value);

	_state.a() = sum & 0xff;

	_state.resetFlags(Z80State::HF | Z80State::PF | Z80State::CF);
	_state.setFlags(Z80State::NF);
	_state.setSZXY(_state.a());
	if (carryIns & 0x10)
		_state.setFlags(Z80State::HF);

	if (((carryIns >> 7) & 0x1) ^ ((carryIns >> 8) & 0x1))
		_state.setFlags(Z80State::PF);
	if ((carryIns >> 8) & 0x1)
		_state.setFlags(Z80State::CF);
}
void Z80::cp(const uint8_t value)
{
	const uint16_t sum = _state.a() - value;
	const uint16_t carryIns = (sum ^ _state.a() ^ value);

	_state.f() = Z80State::NF;
	if (sum == 0)
		_state.setFlags(Z80State::ZF);
	else
		_state.setFlags(sum & Z80State::SF);
	_state.setFlags(value & (Z80State::XF | Z80State::YF));

	if (carryIns & 0x10)
		_state.setFlags(Z80State::HF);
	if (((carryIns >> 7) & 0x1) ^ ((carryIns >> 8) & 0x1))
		_state.setFlags(Z80State::PF);
	if ((carryIns >> 8) & 0x1)
		_state.setFlags(Z80State::CF);
}
void Z80::ana(const uint8_t value) {
	_state.a() &= value;

	_state.f() = 0;
	_state.setSZXY(_state.a());
	_state.setP(_state.a());
	_state.setFlags(Z80State::HF);
}
void Z80::ora(const uint8_t value) {
	_state.a() |= value;

	_state.f() = 0;
	_state.setSZXY(_state.a());
	_state.setP(_state.a());
}
void Z80::xra(const uint8_t value) {
	_state.a() ^= value;

	_state.f() = 0;
	_state.setSZXY(_state.a());
	_state.setP(_state.a());
}

uint8_t Z80::inc(const uint8_t value) {
	uint8_t result = value + 1;
	_state.resetFlags(Z80State::NF | Z80State::HF | Z80State::VF);
	_state.setSZXY(result);
	if (result == 0x80)
		_state.setFlags(Z80State::VF);
	if ((result & 0x0f) == 0x00)
		_state.setFlags(Z80State::HF);
	return result;
}
uint8_t Z80::dec(const uint8_t value) {
	uint8_t result = value - 1;
	_state.resetFlags(Z80State::HF | Z80State::VF);
	_state.setFlags(Z80State::NF);
	_state.setSZXY(result);
	if (result == 0x7f)
		_state.setFlags(Z80State::VF);
	if ((result & 0x0f) == 0x0f)
		_state.setFlags(Z80State::HF);
	return result;
}

void Z80::exchange_de_hl() {
	const uint16_t t = _state.de();
	_state.de() = _state.hl();
	_state.hl() = t;
}
uint16_t Z80::exchange_sp(const uint16_t hl) {
	const uint16_t sp = read16(_state.sp());
	write16(_state.sp(), hl);
	return sp;
}
void Z80::call(const uint16_t address, const uint8_t condition) {
	if (condition) {
		push(_state.pc());
		_state.pc() = address;
		_elapsed_cycles++;
	}
}
