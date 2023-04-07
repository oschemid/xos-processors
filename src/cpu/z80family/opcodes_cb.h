#pragma once
#include <array>


#define COMMON_OPCODES_CB \
	RLC_R, \
	RLC_HL, \
	RL_R, \
	RL_HL, \
	RRC_R, \
	RRC_HL, \
	RR_R, \
	RR_HL, \
	BIT_R, \
	BIT_HL, \
	SET_R, \
	SET_HL, \
	RES_R, \
	RES_HL, \
	SRL_R, \
	SRL_HL, \
	SRA_R, \
	SRA_HL, \
	SLA_R, \
	SLA_HL, \
	SLL_R, \
	SLL_HL

#define COMMON_OPCODES_TABLE_CB(opcodes, table) \
	std::array<opcodes,256> table{opcodes::UNIMPLEMENTED}; \
	for (int i = 0; i < 256; ++i) { \
		if ((i & 0b11111000) == 0b00000000) \
			table[i] = ((i & 0x07) == 0x06) ? opcodes::RLC_HL : opcodes::RLC_R; \
		if ((i & 0b11111000) == 0b00010000) \
			table[i] = ((i & 0x07) == 0x06) ? opcodes::RL_HL : opcodes::RL_R; \
		if ((i & 0b11111000) == 0b00001000) \
			table[i] = ((i & 0x07) == 0x06) ? opcodes::RRC_HL : opcodes::RRC_R; \
		if ((i & 0b11111000) == 0b00011000) \
			table[i] = ((i & 0x07) == 0x06) ? opcodes::RR_HL : opcodes::RR_R; \
		if ((i & 0b11111000) == 0b00100000) \
			table[i] = ((i & 0x07) == 0x06) ? opcodes::SLA_HL : opcodes::SLA_R; \
		if ((i & 0b11111000) == 0b00101000) \
			table[i] = ((i & 0x07) == 0x06) ? opcodes::SRA_HL : opcodes::SRA_R; \
		if ((i & 0b11111000) == 0b00110000) \
			table[i] = ((i & 0x07) == 0x06) ? opcodes::SLL_HL : opcodes::SLL_R; \
		if ((i & 0b11111000) == 0b00111000) \
			table[i] = ((i & 0x07) == 0x06) ? opcodes::SRL_HL : opcodes::SRL_R; \
		if ((i & 0b11000000) == 0b01000000) \
			table[i] = ((i & 0x07) == 0x06) ? opcodes::BIT_HL : opcodes::BIT_R; \
		if ((i & 0b11000000) == 0b11000000) \
			table[i] = ((i & 0x07) == 0x06) ? opcodes::SET_HL : opcodes::SET_R; \
		if ((i & 0b11000000) == 0b10000000) \
			table[i] = ((i & 0x07) == 0x06) ? opcodes::RES_HL : opcodes::RES_R; \
	}
