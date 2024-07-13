#include "z80.h"


using namespace xprocessors::cpu;

const std::vector<Z80::opcode_steps> Z80::opcodes_timing_cb[] =
{
	// 0x00 - 0x0F
	{ RLC_B, FETCH }, // RLC B
	{ RLC_C, FETCH }, // RLC C
	{ RLC_D, FETCH }, // RLC D
	{ RLC_E, FETCH }, // RLC E
	{ RLC_H, FETCH }, // RLC H
	{ RLC_L, FETCH }, // RLC L
	{ LD_HL_INTO_WZ, READ, WAIT, RLC_DB, WRITE_DB, WRITE, NOP, NOP, FETCH }, // RLC (HL)
	{ RLC_A, FETCH }, // RLC A
	{ RRC_B, FETCH }, // RRC B
	{ RRC_C, FETCH }, // RRC C
	{ RRC_D, FETCH }, // RRC D
	{ RRC_E, FETCH }, // RRC E
	{ RRC_H, FETCH }, // RRC H
	{ RRC_L, FETCH }, // RRC L
	{ LD_HL_INTO_WZ, READ, WAIT, RRC_DB, WRITE_DB, WRITE, NOP, NOP, FETCH }, // RRC (HL)
	{ RRC_A, FETCH }, // RRC A

	// 0x10 - 0x1F
	{ RL_B, FETCH }, // RL B
	{ RL_C, FETCH }, // RL C
	{ RL_D, FETCH }, // RL D
	{ RL_E, FETCH }, // RL E
	{ RL_H, FETCH }, // RL H
	{ RL_L, FETCH }, // RL L
	{ LD_HL_INTO_WZ, READ, WAIT, RL_DB, WRITE_DB, WRITE, NOP, NOP, FETCH }, // RL (HL)
	{ RL_A, FETCH }, // RL A
	{ RR_B, FETCH }, // RR B
	{ RR_C, FETCH }, // RR C
	{ RR_D, FETCH }, // RR D
	{ RR_E, FETCH }, // RR E
	{ RR_H, FETCH }, // RR H
	{ RR_L, FETCH }, // RR L
	{ LD_HL_INTO_WZ, READ, WAIT, RR_DB, WRITE_DB, WRITE, NOP, NOP, FETCH }, // RR (HL)
	{ RR_A, FETCH }, // RR A

	// 0x20 - 0x2F
	{ SLA_B, FETCH }, // SLA B
	{ SLA_C, FETCH }, // SLA C
	{ SLA_D, FETCH }, // SLA D
	{ SLA_E, FETCH }, // SLA E
	{ SLA_H, FETCH }, // SLA H
	{ SLA_L, FETCH }, // SLA L
	{ LD_HL_INTO_WZ, READ, WAIT, SLA_DB, WRITE_DB, WRITE, NOP, NOP, FETCH }, // SLA (HL)
	{ SLA_A, FETCH }, // SLA A
	{ SRA_B, FETCH }, // SRA B
	{ SRA_C, FETCH }, // SRA C
	{ SRA_D, FETCH }, // SRA D
	{ SRA_E, FETCH }, // SRA E
	{ SRA_H, FETCH }, // SRA H
	{ SRA_L, FETCH }, // SRA L
	{ LD_HL_INTO_WZ, READ, WAIT, SRA_DB, WRITE_DB, WRITE, NOP, NOP, FETCH }, // SRA (HL)
	{ SRA_A, FETCH }, // SRA A

	// 0x30 - 0x3F
	{ SLL_B, FETCH }, // SLL B
	{ SLL_C, FETCH }, // SLL C
	{ SLL_D, FETCH }, // SLL D
	{ SLL_E, FETCH }, // SLL E
	{ SLL_H, FETCH }, // SLL H
	{ SLL_L, FETCH }, // SLL L
	{ LD_HL_INTO_WZ, READ, WAIT, SLL_DB, WRITE_DB, WRITE, NOP, NOP, FETCH }, // SLL (HL)
	{ SLL_A, FETCH }, // SLL A
	{ SRL_B, FETCH }, // SRL B
	{ SRL_C, FETCH }, // SRL C
	{ SRL_D, FETCH }, // SRL D
	{ SRL_E, FETCH }, // SRL E
	{ SRL_H, FETCH }, // SRL H
	{ SRL_L, FETCH }, // SRL L
	{ LD_HL_INTO_WZ, READ, WAIT, SRL_DB, WRITE_DB, WRITE, NOP, NOP, FETCH }, // SRL (HL)
	{ SRL_A, FETCH }, // SRL A

	// 0x40 - 0x4F
	{ BIT0_B, FETCH }, // BIT 0,B
	{ BIT0_C, FETCH }, // BIT 0,C
	{ BIT0_D, FETCH }, // BIT 0,D
	{ BIT0_E, FETCH }, // BIT 0,E
	{ BIT0_H, FETCH }, // BIT 0,L
	{ BIT0_L, FETCH }, // BIT 0,H
	{ LD_HL_INTO_WZ, READ, WAIT, NOP, BIT0_DB, FETCH }, // BIT 0,(HL)
	{ BIT0_A, FETCH }, // BIT 0,A
	{ BIT1_B, FETCH }, // BIT 1,B
	{ BIT1_C, FETCH }, // BIT 1,C
	{ BIT1_D, FETCH }, // BIT 1,D
	{ BIT1_E, FETCH }, // BIT 1,E
	{ BIT1_H, FETCH }, // BIT 1,L
	{ BIT1_L, FETCH }, // BIT 1,H
	{ LD_HL_INTO_WZ, READ, WAIT, NOP, BIT1_DB, FETCH }, // BIT 1,(HL)
	{ BIT1_A, FETCH }, // BIT 1,A

	// 0x50 - 0x5F
	{ BIT2_B, FETCH }, // BIT 2,B
	{ BIT2_C, FETCH }, // BIT 2,C
	{ BIT2_D, FETCH }, // BIT 2,D
	{ BIT2_E, FETCH }, // BIT 2,E
	{ BIT2_H, FETCH }, // BIT 2,L
	{ BIT2_L, FETCH }, // BIT 2,H
	{ LD_HL_INTO_WZ, READ, WAIT, NOP, BIT2_DB, FETCH }, // BIT 2,(HL)
	{ BIT2_A, FETCH }, // BIT 2,A
	{ BIT3_B, FETCH }, // BIT 3,B
	{ BIT3_C, FETCH }, // BIT 3,C
	{ BIT3_D, FETCH }, // BIT 3,D
	{ BIT3_E, FETCH }, // BIT 3,E
	{ BIT3_H, FETCH }, // BIT 3,L
	{ BIT3_L, FETCH }, // BIT 3,H
	{ LD_HL_INTO_WZ, READ, WAIT, NOP, BIT3_DB, FETCH }, // BIT 3,(HL)
	{ BIT3_A, FETCH }, // BIT 3,A

	// 0x60 - 0x6F
	{ BIT4_B, FETCH }, // BIT 4,B
	{ BIT4_C, FETCH }, // BIT 4,C
	{ BIT4_D, FETCH }, // BIT 4,D
	{ BIT4_E, FETCH }, // BIT 4,E
	{ BIT4_H, FETCH }, // BIT 4,L
	{ BIT4_L, FETCH }, // BIT 4,H
	{ LD_HL_INTO_WZ, READ, WAIT, NOP, BIT4_DB, FETCH }, // BIT 4,(HL)
	{ BIT4_A, FETCH }, // BIT 4,A
	{ BIT5_B, FETCH }, // BIT 5,B
	{ BIT5_C, FETCH }, // BIT 5,C
	{ BIT5_D, FETCH }, // BIT 5,D
	{ BIT5_E, FETCH }, // BIT 5,E
	{ BIT5_H, FETCH }, // BIT 5,L
	{ BIT5_L, FETCH }, // BIT 5,H
	{ LD_HL_INTO_WZ, READ, WAIT, NOP, BIT5_DB, FETCH }, // BIT 5,(HL)
	{ BIT5_A, FETCH }, // BIT 5,A

	// 0x70 - 0x7F
	{ BIT6_B, FETCH }, // BIT 6,B
	{ BIT6_C, FETCH }, // BIT 6,C
	{ BIT6_D, FETCH }, // BIT 6,D
	{ BIT6_E, FETCH }, // BIT 6,E
	{ BIT6_H, FETCH }, // BIT 6,L
	{ BIT6_L, FETCH }, // BIT 6,H
	{ LD_HL_INTO_WZ, READ, WAIT, NOP, BIT6_DB, FETCH }, // BIT 6,(HL)
	{ BIT6_A, FETCH }, // BIT 6,A
	{ BIT7_B, FETCH }, // BIT 7,B
	{ BIT7_C, FETCH }, // BIT 7,C
	{ BIT7_D, FETCH }, // BIT 7,D
	{ BIT7_E, FETCH }, // BIT 7,E
	{ BIT7_H, FETCH }, // BIT 7,L
	{ BIT7_L, FETCH }, // BIT 7,H
	{ LD_HL_INTO_WZ, READ, WAIT, NOP, BIT7_DB, FETCH }, // BIT 7,(HL)
	{ BIT7_A, FETCH }, // BIT 7,A

	// 0x80 - 0x8F
	{ RES0_B, FETCH }, // RES 0,B
	{ RES0_C, FETCH }, // RES 0,C
	{ RES0_D, FETCH }, // RES 0,D
	{ RES0_E, FETCH }, // RES 0,E
	{ RES0_H, FETCH }, // RES 0,H
	{ RES0_L, FETCH }, // RES 0,L
	{ LD_HL_INTO_WZ, READ, WAIT, NOP, RES0_DB, WRITE_DB, WRITE, NOP, FETCH }, // RES 0,(HL)
	{ RES0_A, FETCH }, // RES 0,A
	{ RES1_B, FETCH }, // RES 1,B
	{ RES1_C, FETCH }, // RES 1,C
	{ RES1_D, FETCH }, // RES 1,D
	{ RES1_E, FETCH }, // RES 1,E
	{ RES1_H, FETCH }, // RES 1,H
	{ RES1_L, FETCH }, // RES 1,L
	{ LD_HL_INTO_WZ, READ, WAIT, NOP, RES1_DB, WRITE_DB, WRITE, NOP, FETCH }, // RES 1,(HL)
	{ RES1_A, FETCH }, // RES 1,A

	// 0x90 - 0x9F
	{ RES2_B, FETCH }, // RES 2,B
	{ RES2_C, FETCH }, // RES 2,C
	{ RES2_D, FETCH }, // RES 2,D
	{ RES2_E, FETCH }, // RES 2,E
	{ RES2_H, FETCH }, // RES 2,H
	{ RES2_L, FETCH }, // RES 2,L
	{ LD_HL_INTO_WZ, READ, WAIT, NOP, RES2_DB, WRITE_DB, WRITE, NOP, FETCH }, // RES 2,(HL)
	{ RES2_A, FETCH }, // RES 2,A
	{ RES3_B, FETCH }, // RES 3,B
	{ RES3_C, FETCH }, // RES 3,C
	{ RES3_D, FETCH }, // RES 3,D
	{ RES3_E, FETCH }, // RES 3,E
	{ RES3_H, FETCH }, // RES 3,H
	{ RES3_L, FETCH }, // RES 3,L
	{ LD_HL_INTO_WZ, READ, WAIT, NOP, RES3_DB, WRITE_DB, WRITE, NOP, FETCH }, // RES 3,(HL)
	{ RES3_A, FETCH }, // RES 3,A

	// 0xA0 - 0xAF
	{ RES4_B, FETCH }, // RES 4,B
	{ RES4_C, FETCH }, // RES 4,C
	{ RES4_D, FETCH }, // RES 4,D
	{ RES4_E, FETCH }, // RES 4,E
	{ RES4_H, FETCH }, // RES 4,H
	{ RES4_L, FETCH }, // RES 4,L
	{ LD_HL_INTO_WZ, READ, WAIT, NOP, RES4_DB, WRITE_DB, WRITE, NOP, FETCH }, // RES 4,(HL)
	{ RES4_A, FETCH }, // RES 4,A
	{ RES5_B, FETCH }, // RES 5,B
	{ RES5_C, FETCH }, // RES 5,C
	{ RES5_D, FETCH }, // RES 5,D
	{ RES5_E, FETCH }, // RES 5,E
	{ RES5_H, FETCH }, // RES 5,H
	{ RES5_L, FETCH }, // RES 5,L
	{ LD_HL_INTO_WZ, READ, WAIT, NOP, RES5_DB, WRITE_DB, WRITE, NOP, FETCH }, // RES 5,(HL)
	{ RES5_A, FETCH }, // RES 5,A

	// 0xB0 - 0xBF
	{ RES6_B, FETCH }, // RES 6,B
	{ RES6_C, FETCH }, // RES 6,C
	{ RES6_D, FETCH }, // RES 6,D
	{ RES6_E, FETCH }, // RES 6,E
	{ RES6_H, FETCH }, // RES 6,H
	{ RES6_L, FETCH }, // RES 6,L
	{ LD_HL_INTO_WZ, READ, WAIT, NOP, RES6_DB, WRITE_DB, WRITE, NOP, FETCH }, // RES 6,(HL)
	{ RES6_A, FETCH }, // RES 6,A
	{ RES7_B, FETCH }, // RES 7,B
	{ RES7_C, FETCH }, // RES 7,C
	{ RES7_D, FETCH }, // RES 7,D
	{ RES7_E, FETCH }, // RES 7,E
	{ RES7_H, FETCH }, // RES 7,H
	{ RES7_L, FETCH }, // RES 7,L
	{ LD_HL_INTO_WZ, READ, WAIT, NOP, RES7_DB, WRITE_DB, WRITE, NOP, FETCH }, // RES 7,(HL)
	{ RES7_A, FETCH }, // RES 7,A

	// 0xC0 - 0xCF
	{ SET0_B, FETCH }, // SET 0,B
	{ SET0_C, FETCH }, // SET 0,C
	{ SET0_D, FETCH }, // SET 0,D
	{ SET0_E, FETCH }, // SET 0,E
	{ SET0_H, FETCH }, // SET 0,H
	{ SET0_L, FETCH }, // SET 0,L
	{ LD_HL_INTO_WZ, READ, WAIT, NOP, SET0_DB, WRITE_DB, WRITE, NOP, FETCH }, // SET 0,(HL)
	{ SET0_A, FETCH }, // SET 0,A
	{ SET1_B, FETCH }, // SET 1,B
	{ SET1_C, FETCH }, // SET 1,C
	{ SET1_D, FETCH }, // SET 1,D
	{ SET1_E, FETCH }, // SET 1,E
	{ SET1_H, FETCH }, // SET 1,H
	{ SET1_L, FETCH }, // SET 1,L
	{ LD_HL_INTO_WZ, READ, WAIT, NOP, SET1_DB, WRITE_DB, WRITE, NOP, FETCH }, // SET 1,(HL)
	{ SET1_A, FETCH }, // SET 1,A

	// 0xD0 - 0xDF
	{ SET2_B, FETCH }, // SET 2,B
	{ SET2_C, FETCH }, // SET 2,C
	{ SET2_D, FETCH }, // SET 2,D
	{ SET2_E, FETCH }, // SET 2,E
	{ SET2_H, FETCH }, // SET 2,H
	{ SET2_L, FETCH }, // SET 2,L
	{ LD_HL_INTO_WZ, READ, WAIT, NOP, SET2_DB, WRITE_DB, WRITE, NOP, FETCH }, // SET 2,(HL)
	{ SET2_A, FETCH }, // SET 2,A
	{ SET3_B, FETCH }, // SET 3,B
	{ SET3_C, FETCH }, // SET 3,C
	{ SET3_D, FETCH }, // SET 3,D
	{ SET3_E, FETCH }, // SET 3,E
	{ SET3_H, FETCH }, // SET 3,H
	{ SET3_L, FETCH }, // SET 3,L
	{ LD_HL_INTO_WZ, READ, WAIT, NOP, SET3_DB, WRITE_DB, WRITE, NOP, FETCH }, // SET 3,(HL)
	{ SET3_A, FETCH }, // SET 3,A

	// 0xE0 - 0xEF
	{ SET4_B, FETCH }, // SET 4,B
	{ SET4_C, FETCH }, // SET 4,C
	{ SET4_D, FETCH }, // SET 4,D
	{ SET4_E, FETCH }, // SET 4,E
	{ SET4_H, FETCH }, // SET 4,H
	{ SET4_L, FETCH }, // SET 4,L
	{ LD_HL_INTO_WZ, READ, WAIT, NOP, SET4_DB, WRITE_DB, WRITE, NOP, FETCH }, // SET 4,(HL)
	{ SET4_A, FETCH }, // SET 4,A
	{ SET5_B, FETCH }, // SET 5,B
	{ SET5_C, FETCH }, // SET 5,C
	{ SET5_D, FETCH }, // SET 5,D
	{ SET5_E, FETCH }, // SET 5,E
	{ SET5_H, FETCH }, // SET 5,H
	{ SET5_L, FETCH }, // SET 5,L
	{ LD_HL_INTO_WZ, READ, WAIT, NOP, SET5_DB, WRITE_DB, WRITE, NOP, FETCH }, // SET 5,(HL)
	{ SET5_A, FETCH }, // SET 5,A

	// 0xF0 - 0xFF
	{ SET6_B, FETCH }, // SET 6,B
	{ SET6_C, FETCH }, // SET 6,C
	{ SET6_D, FETCH }, // SET 6,D
	{ SET6_E, FETCH }, // SET 6,E
	{ SET6_H, FETCH }, // SET 6,H
	{ SET6_L, FETCH }, // SET 6,L
	{ LD_HL_INTO_WZ, READ, WAIT, NOP, SET6_DB, WRITE_DB, WRITE, NOP, FETCH }, // SET 6,(HL)
	{ SET6_A, FETCH }, // SET 6,A
	{ SET7_B, FETCH }, // SET 7,B
	{ SET7_C, FETCH }, // SET 7,C
	{ SET7_D, FETCH }, // SET 7,D
	{ SET7_E, FETCH }, // SET 7,E
	{ SET7_H, FETCH }, // SET 7,H
	{ SET7_L, FETCH }, // SET 7,L
	{ LD_HL_INTO_WZ, READ, WAIT, NOP, SET7_DB, WRITE_DB, WRITE, NOP, FETCH }, // SET 7,(HL)
	{ SET7_A, FETCH }, // SET 7,A
};
