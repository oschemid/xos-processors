#include "z80.h"


using namespace xprocessors::cpu;


const std::vector<Z80::opcode_steps> Z80::opcodes_timing_ed[] =
{
	// 0x00 - 0x0F
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },

	// 0x10 - 0x1F
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },

	// 0x20 - 0x2F
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },

	// 0x30 - 0x3F
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },

	// 0x40 - 0x4F
	{ LD_BC_INTO_WZ, NOP, READIO, WAIT, LD_DB_INTO_B_FLAGS, FETCH }, // IN B,(C)
	{ },
	{ LD_BC_INTO_WZ, SBC_WZ_TO_HL, NOP, NOP, NOP, NOP, NOP, NOP, FETCH }, // SBC HL,BC
	{ NOP, READPC, WAIT, LD_DB_INTO_WZL, READPC, WAIT, LD_DB_INTO_WZH, WRITEC, WRITE, INC_WZ, WRITEB, WRITE, NOP, FETCH }, // LD (nn),BC
	{ NEG, FETCH }, // NEG
	{ },
	{ },
	{ },
	{ },
	{ },
	{ LD_BC_INTO_WZ, ADC_WZ_TO_HL, NOP, NOP, NOP, NOP, NOP, NOP, FETCH }, // ADC HL,BC
	{ NOP, READPC, WAIT, LD_DB_INTO_WZL, READPC, WAIT, LD_DB_INTO_WZH, READWR, WAIT, LD_DB_INTO_C, READWR, WAIT, LD_DB_INTO_B, FETCH }, // LD BC,(nn)
	{ ERROR },
	{ },
	{ ERROR },
	{ },

	// 0x50 - 0x5F
	{ LD_BC_INTO_WZ, NOP, READIO, WAIT, LD_DB_INTO_D_FLAGS, FETCH }, // IN D,(C)
	{ },
	{ LD_DE_INTO_WZ, SBC_WZ_TO_HL, NOP, NOP, NOP, NOP, NOP, NOP, FETCH }, // SBC HL,DE
	{ NOP, READPC, WAIT, LD_DB_INTO_WZL, READPC, WAIT, LD_DB_INTO_WZH, WRITEE, WRITE, INC_WZ, WRITED, WRITE, NOP, FETCH }, // LD (nn),DE
	{ ERROR },
	{ ERROR },
	{ },
	{ },
	{ },
	{ },
	{ LD_DE_INTO_WZ, ADC_WZ_TO_HL, NOP, NOP, NOP, NOP, NOP, NOP, FETCH }, // ADC HL,DE
	{ NOP, READPC, WAIT, LD_DB_INTO_WZL, READPC, WAIT, LD_DB_INTO_WZH, READWR, WAIT, LD_DB_INTO_E, READWR, WAIT, LD_DB_INTO_D, FETCH }, // LD DE,(nn)
	{ ERROR },
	{ ERROR },
	{ },
	{ },

	// 0x60 - 0x6F
	{ LD_BC_INTO_WZ, NOP, READIO, WAIT, LD_DB_INTO_H_FLAGS, FETCH }, // IN H,(C)
	{ },
	{ LD_HL_INTO_WZ, SBC_WZ_TO_HL, NOP, NOP, NOP, NOP, NOP, NOP, FETCH }, // SBC HL,HL
	{ NOP, READPC, WAIT, LD_DB_INTO_WZL, READPC, WAIT, LD_DB_INTO_WZH, WRITEL, WRITE, INC_WZ, WRITEH, WRITE, NOP, FETCH }, // LD (nn),HL
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ LD_HL_INTO_WZ, READ, WAIT, NOP, RRD, NOP, NOP, NOP, WRITE_DB, WRITE, NOP, FETCH }, // RRD
	{ },
	{ },
	{ LD_HL_INTO_WZ, ADC_WZ_TO_HL, NOP, NOP, NOP, NOP, NOP, NOP, FETCH }, // ADC HL,HL
	{ NOP, READPC, WAIT, LD_DB_INTO_WZL, READPC, WAIT, LD_DB_INTO_WZH, READWR, WAIT, LD_DB_INTO_L, READWR, WAIT, LD_DB_INTO_H, FETCH }, // LD HL,(nn)
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ LD_HL_INTO_WZ, READ, WAIT, NOP, RLD, NOP, NOP, NOP, WRITE_DB, WRITE, NOP, FETCH }, // RLD

	// 0x70 - 0x7F
	{ LD_BC_INTO_WZ, NOP, READIO, WAIT, LD_DB_INTO_DB_FLAGS, FETCH }, // IN (C)
	{ },
	{ LD_SP_INTO_WZ, SBC_WZ_TO_HL, NOP, NOP, NOP, NOP, NOP, NOP, FETCH }, // SBC HL,SP
	{ NOP, READPC, WAIT, LD_DB_INTO_WZL, READPC, WAIT, LD_DB_INTO_WZH, WRITESPL, WRITE, INC_WZ, WRITESPH, WRITE, NOP, FETCH }, // LD (nn),SP
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ },
	{ },
	{ LD_SP_INTO_WZ, ADC_WZ_TO_HL, NOP, NOP, NOP, NOP, NOP, NOP, FETCH }, // ADC HL,SP
	{ NOP, READPC, WAIT, LD_DB_INTO_WZL, READPC, WAIT, LD_DB_INTO_WZH, READWR, WAIT, LD_DB_INTO_SPL, READWR, WAIT, LD_DB_INTO_SPH, FETCH }, // LD SP,(nn)
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },

	// 0x80 - 0x8F
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },

	// 0x90 - 0x9F
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },

	// 0xA0 - 0xAF
	{ LD_HL_INTO_WZ, READ, WAIT, LD_DE_INTO_WZ, WRITE_DB, WRITE, INC_HL, INC_DE, LD_ITER, FETCH }, // LDI
	{ LD_HL_INTO_WZ, READ, WAIT, NOP, CP_DB_NOCARRY, INC_HL, NOP, NOP, CP_ITER, FETCH }, // CPI
	{ },
	{ },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ LD_HL_INTO_WZ, READ, WAIT, LD_DE_INTO_WZ, WRITE_DB, WRITE, DEC_HL, DEC_DE, LD_ITER, FETCH }, // LDD
	{ LD_HL_INTO_WZ, READ, WAIT, NOP, CP_DB_NOCARRY, DEC_HL, NOP, NOP, CP_ITER, FETCH }, // CPD
	{ },
	{ },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },

	// 0xB0 - 0xBF
	{ LD_HL_INTO_WZ, READ, WAIT, LD_DE_INTO_WZ, WRITE_DB, WRITE, INC_HL, INC_DE, LD_ITER, DECPC, DECPC, NOP, NOP, NOP, FETCH }, // LDIR
	{ LD_HL_INTO_WZ, READ, WAIT, NOP, CP_DB_NOCARRY, INC_HL, NOP, NOP, CP_ITER, DECPC, DECPC, NOP, NOP, NOP, FETCH }, // CPIR
	{ },
	{ },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ LD_HL_INTO_WZ, READ, WAIT, LD_DE_INTO_WZ, WRITE_DB, WRITE, DEC_HL, DEC_DE, LD_ITER, DECPC, DECPC, NOP, NOP, NOP, FETCH }, // LDDR
	{ LD_HL_INTO_WZ, READ, WAIT, NOP, CP_DB_NOCARRY, DEC_HL, NOP, NOP, CP_ITER, DECPC, DECPC, NOP, NOP, NOP, FETCH }, // CPDR
	{ },
	{ },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },

	// 0xC0 - 0xCF
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },

	// 0xD0 - 0xDF
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },

	// 0xE0 - 0xEF
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },

	// 0xF0 - 0xFF
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
	{ ERROR },
};
