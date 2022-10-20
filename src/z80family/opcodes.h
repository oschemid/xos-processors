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
	LD_R_R, \
	LD_R_N

#define COMMON_OPCODES_TABLE(opcodes, table) \
	std::array<opcodes,256> table{opcodes::UNIMPLEMENTED}; \
	for (int i = 0; i < 256; ++i) { \
		/* 0x00 */ \
		if (i == 0) \
			table[i] = opcodes::NOP; \
		if ((i & 0b11000111) == 0b00000110) \
			table[i] = opcodes::LD_R_N; \
		/* 0x01 */ \
		if ((i & 0b11000000) == 0b01000000) { \
			if (i == 0b01110110) \
				table[i] = opcodes::HALT; \
			else \
				table[i] = opcodes::LD_R_R; \
		} \
		/* 0x11 */ \
		if ((i & 0b11001111) == 0b11000001) \
			table[i] = opcodes::POP; \
		if ((i & 0b11000111) == 0b11000010) \
			table[i] = opcodes::JP_CC; \
		if (i == 0b11000011) \
			table[i] = opcodes::JP; \
		if ((i & 0b11000111) == 0b11000100) \
			table[i] = opcodes::CALL_CC; \
		if ((i & 0b11001111) == 0b11000101) \
			table[i] = opcodes::PUSH; \
		if (i == 0b11001101) \
			table[i] = opcodes::CALL; \
	}

#define COMMON_OPCODES_DECODING(opcodes, table, opcode) \
	switch (table[opcode]) { \
	case opcodes::NOP: \
		break; \
	case opcodes::HALT: \
		_halted = true; \
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
	case opcodes::LD_R_N: \
		decodeR(opcode >> 3, readArgument8()); \
		break; \
	case opcodes::LD_R_R: \
		decodeR(opcode >> 3, decodeR(opcode)); \
		break; \
	}