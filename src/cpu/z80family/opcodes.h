#pragma once
#include <array>


#define COMMON_OPCODES \
	UNIMPLEMENTED, \
	NOP, \
	HALT, \
	PUSH, \
	POP, \
	JP, \
	JP_CC, \
	CALL, \
	CALL_CC, \
	RET, \
	RET_CC, \
	RST, \
	LD_R_R, \
	LD_R_N, \
	LD_RR_NN, \
	LD_BC_A, \
	LD_DE_A, \
	LD_A_BC, \
	LD_A_DE, \
	ADD_R, \
	ADD_N, \
	ADD_HL_RR, \
	ADC_R, \
	ADC_N, \
	SUB_R, \
	SUB_N, \
	SBC_R, \
	SBC_N, \
	ANA_R, \
	ANA_N, \
	ORA_R, \
	ORA_N, \
	XRA_R, \
	XRA_N, \
	CMP_R, \
	CMP_N, \
	INC_R, \
	DEC_R, \
	INC_RR, \
	DEC_RR

#define COMMON_OPCODES_TABLE(opcodes, table) \
	std::array<opcodes,256> table{opcodes::UNIMPLEMENTED}; \
	for (int i = 0; i < 256; ++i) { \
		/* 0x00 */ \
		/* 0x00aaa000*/ \
		if (i == 0) \
			table[i] = opcodes::NOP; \
		/* 0x00aaa001*/ \
		if ((i & 0b11001111) == 0b00000001) \
			table[i] = opcodes::LD_RR_NN; \
		if ((i & 0b11001111) == 0b00001001) \
			table[i] = opcodes::ADD_HL_RR; \
		/* 0x00aaa010*/ \
		if (i == 0b00000010) \
			table[i] = opcodes::LD_BC_A; \
		if (i == 0b00010010) \
			table[i] = opcodes::LD_DE_A; \
		if (i == 0b00001010) \
			table[i] = opcodes::LD_A_BC; \
		if (i == 0b00011010) \
			table[i] = opcodes::LD_A_DE; \
		/* 0x00aaa011*/ \
		if ((i & 0b11001111) == 0b00000011) \
			table[i] = opcodes::INC_RR; \
		if ((i & 0b11001111) == 0b00001011) \
			table[i] = opcodes::DEC_RR; \
		/* 0x00aaa100*/ \
		if ((i & 0b11000111) == 0b00000100) \
			table[i] = opcodes::INC_R; \
		/* 0x00aaa101*/ \
		if ((i & 0b11000111) == 0b00000101) \
			table[i] = opcodes::DEC_R; \
		/* 0x00aaa110*/ \
		if ((i & 0b11000111) == 0b00000110) \
			table[i] = opcodes::LD_R_N; \
		/* 0x01 */ \
		if ((i & 0b11000000) == 0b01000000) { \
			if (i == 0b01110110) \
				table[i] = opcodes::HALT; \
			else \
				table[i] = opcodes::LD_R_R; \
		} \
		/* 0x10 */ \
		if ((i & 0b11111000) == 0b10000000) \
			table[i] = opcodes::ADD_R; \
		if ((i & 0b11111000) == 0b10001000) \
			table[i] = opcodes::ADC_R; \
		if ((i & 0b11111000) == 0b10010000) \
			table[i] = opcodes::SUB_R; \
		if ((i & 0b11111000) == 0b10011000) \
			table[i] = opcodes::SBC_R; \
		if ((i & 0b11111000) == 0b10100000) \
			table[i] = opcodes::ANA_R; \
		if ((i & 0b11111000) == 0b10101000) \
			table[i] = opcodes::XRA_R; \
		if ((i & 0b11111000) == 0b10110000) \
			table[i] = opcodes::ORA_R; \
		if ((i & 0b11111000) == 0b10111000) \
			table[i] = opcodes::CMP_R; \
		/* 0x11 */ \
		/* 0x11aaa000 */ \
		if ((i & 0b11000111) == 0b11000000) \
			table[i] = opcodes::RET_CC; \
		/* 0x11aaa001 */ \
		if ((i & 0b11001111) == 0b11000001) \
			table[i] = opcodes::POP; \
		if (i == 0b11001001) \
			table[i] = opcodes::RET; \
		/* 0x11aaa010 */ \
		if ((i & 0b11000111) == 0b11000010) \
			table[i] = opcodes::JP_CC; \
		/* 0x11aaa011 */ \
		if (i == 0b11000011) \
			table[i] = opcodes::JP; \
		/* 0x11aaa100 */ \
		if ((i & 0b11000111) == 0b11000100) \
			table[i] = opcodes::CALL_CC; \
		/* 0x11aaa101 */ \
		if ((i & 0b11001111) == 0b11000101) \
			table[i] = opcodes::PUSH; \
		if (i == 0b11001101) \
			table[i] = opcodes::CALL; \
		/* 0x11aaa110 */ \
		if (i == 0b11000110) \
			table[i] = opcodes::ADD_N; \
		if (i == 0b11001110) \
			table[i] = opcodes::ADC_N; \
		if (i == 0b11010110) \
			table[i] = opcodes::SUB_N; \
		if (i == 0b11011110) \
			table[i] = opcodes::SBC_N; \
		if (i == 0b11100110) \
			table[i] = opcodes::ANA_N; \
		if (i == 0b11101110) \
			table[i] = opcodes::XRA_N; \
		if (i == 0b11110110) \
			table[i] = opcodes::ORA_N; \
		if (i == 0b11111110) \
			table[i] = opcodes::CMP_N; \
		/* 0x11aaa111 */ \
        if ((i & 0b11000111) == 0b11000111) \
			table[i] = opcodes::RST; \
	}

#define COMMON_OPCODES_DECODING(opcodes, table, opcode) \
	switch (table[opcode]) { \
	case opcodes::NOP: \
		break; \
	case opcodes::HALT: \
		_halted = true; \
		_elapsed_cycles += Cost::EXTRAHALT; \
		break; \
	case opcodes::POP: \
		decodePop(opcode, pop()); \
		break; \
	case opcodes::PUSH: \
		push(decodePush(opcode)); \
		break; \
	case opcodes::JP: \
		jump(readArgument16()); \
		break; \
	case opcodes::JP_CC: \
		jump(readArgument16(), checkCC(opcode)); \
		break; \
	case opcodes::CALL: \
		call(readArgument16()); \
		break; \
	case opcodes::CALL_CC: \
		call(readArgument16(), checkCC(opcode)); \
		break; \
	case opcodes::RET: \
		ret(); \
		break; \
	case opcodes::RET_CC: \
		ret(checkCC(opcode)); \
		_elapsed_cycles += Cost::EXTRARET; \
		break; \
	case opcodes::RST: \
		push(_state.pc()); \
		_state.pc() = opcode & 0b00111000; \
        break; \
	case opcodes::LD_R_N: \
		decodeR(opcode >> 3, readArgument8()); \
		break; \
	case opcodes::LD_R_R: \
		decodeR(opcode >> 3, decodeR(opcode)); \
		if (((opcode & 0b111) != 0b110)&&((opcode&0b111000) != 0b110000)) \
			_elapsed_cycles += Cost::READ_WRITE_R; \
		break; \
	case opcodes::LD_RR_NN: \
		decodeRR(opcode, readArgument16()); \
		break; \
	case opcodes::ADD_HL_RR: \
		add_hl(decodeRR(opcode)); \
		break; \
	case opcodes::LD_BC_A: \
		write8(_state.bc(), _state.a()); \
		break; \
	case opcodes::LD_DE_A: \
		write8(_state.de(), _state.a()); \
		break; \
	case opcodes::LD_A_BC: \
		_state.a() = read8(_state.bc()); \
		break; \
	case opcodes::LD_A_DE: \
		_state.a() = read8(_state.de()); \
		break; \
	case opcodes::ADD_R: \
		add(decodeR(opcode)); \
		break; \
	case opcodes::ADD_N: \
		add(readArgument8()); \
		break; \
	case opcodes::ADC_R: \
		adc(decodeR(opcode)); \
		break; \
	case opcodes::ADC_N: \
		adc(readArgument8()); \
		break; \
	case opcodes::SUB_R: \
		sub(decodeR(opcode)); \
		break; \
	case opcodes::SUB_N: \
		sub(readArgument8()); \
		break; \
	case opcodes::SBC_R: \
		sbc(decodeR(opcode)); \
		break; \
	case opcodes::SBC_N: \
		sbc(readArgument8()); \
		break; \
	case opcodes::ANA_R: \
		ana(decodeR(opcode)); \
		break; \
	case opcodes::ANA_N: \
		ana(readArgument8()); \
		break; \
	case opcodes::XRA_R: \
		xra(decodeR(opcode)); \
		break; \
	case opcodes::XRA_N: \
		xra(readArgument8()); \
		break; \
	case opcodes::ORA_R: \
		ora(decodeR(opcode)); \
		break; \
	case opcodes::ORA_N: \
		ora(readArgument8()); \
		break; \
	case opcodes::CMP_R: \
		cmp(decodeR(opcode)); \
		break; \
	case opcodes::CMP_N: \
		cmp(readArgument8()); \
		break; \
	case opcodes::INC_R: \
		decodeR(opcode >> 3, inc(decodeR(opcode >> 3))); \
		if ((opcode&0b111000) != 0b110000) \
			_elapsed_cycles += Cost::READ_WRITE_R; \
		break; \
	case opcodes::DEC_R: \
		decodeR(opcode >> 3, dec(decodeR(opcode >> 3))); \
		if ((opcode&0b111000) != 0b110000) \
			_elapsed_cycles += Cost::READ_WRITE_R; \
		break; \
	case opcodes::INC_RR: \
		decodeRR(opcode, decodeRR(opcode) + 1); \
		_elapsed_cycles += Cost::READ_WRITE_RR; \
		break; \
	case opcodes::DEC_RR: \
		decodeRR(opcode, decodeRR(opcode) - 1); \
		_elapsed_cycles += Cost::READ_WRITE_RR; \
		break; \
	}
