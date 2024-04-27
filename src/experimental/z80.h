#pragma once
#include "xprocessors.h"
#include "interfaces.h"
#include "../cpu/cpu.h"


namespace xprocessors::cpu
{
	class Z80 : public interfaces::clockable
	{
		friend class Z80debugging;

	public:
		static const uint16_t PIN_M1 = 1;
		static const uint16_t PIN_MREQ = 2;
		static const uint16_t PIN_IORQ = 4;
		static const uint16_t PIN_RD = 8;
		static const uint16_t PIN_WR = 16;
		static const uint16_t PIN_HALT = 32;
		static const uint16_t PIN_RFSH = 64;
		static const uint16_t PIN_INT = 128;

		enum flags {
			SF = 0x80,
			ZF = 0x40,
			XF = 0x20,
			HF = 0x10,
			YF = 0x08,
			PF = 0x04,
			VF = 0x04,
			NF = 0x02,
			CF = 0x01
		};

	public:
		void init();
		void reset(const uint16_t = 0);

		void tick() override;

		uint16_t getAddressBus() const { return addressbus; }
		uint8_t getDataBus() const { return databus; }
		void setDataBus(const uint8_t d) { databus = d; }
		uint16_t getControlPins() const { return pins; }
		void interrupt() { pins |= PIN_INT; }
	public:
		// Registers
		union { struct { register8_t f; register8_t a; }; register16_t af; };
		union { struct { register8_t c; register8_t b; }; register16_t bc; };
		union { struct { register8_t e; register8_t d; }; register16_t de; };
		union { struct { register8_t l; register8_t h; }; register16_t hl; };
		union { struct { register8_t ixl; register8_t ixh; }; register16_t ix; };
		union { struct { register8_t iyl; register8_t iyh; }; register16_t iy; };
		register8_t i;
		register8_t r;

		register16_t afprime, bcprime, deprime, hlprime;
		union { struct { register8_t pcl; register8_t pch; }; register16_t pc; };
		union { struct { register8_t spl; register8_t sph; }; register16_t sp; };
		union { struct { register8_t z; register8_t w; }; register16_t wz; };

		bool iff1;
		bool iff1_requested;
		bool halted;

		enum class interrupt_mode {
			mode_0,
			mode_1,
			mode_2
		} im;

		// Process parameter
		enum { NORMAL = 0, CB, DD, FD, ED, DDCB } hli_idx;
		uint8_t opcode;

		// Pins
		uint16_t addressbus;
		uint8_t databus;
		uint16_t pins;

	protected:
		enum opcode_steps {
			FETCH = 0,
			FETCH_PREFIX,
			FETCHWAIT,
			DECODE,
			WAIT,
			HALT,
			PREFIX_CB,
			PREFIX_DD,
			PREFIX_DDCB,
			PREFIX_ED,
			PREFIX_FD,
			ERROR,
			NOP,
			DI,
			EI,
			IM1,
			IM2,

			// Copy 16bits register into another register
			LD_BC_INTO_WZ,
			LD_DE_INTO_WZ,
			LD_HL_INTO_WZ,
			LD_IX_INTO_WZ,
			LD_IY_INTO_WZ,
			LD_SP_INTO_WZ,
			LD_WZ_INTO_SP,
			LD_WZ_INTO_HL,
			LD_WZ_INTO_IX,
			LD_WZ_INTO_IY,

			// Copy 8bits register (or databus) into another register
			// Into A
			LD_IXH_INTO_A,
			LD_IXL_INTO_A,
			LD_IYH_INTO_A,
			LD_IYL_INTO_A,
			LD_DB_INTO_A,
			LD_DB_INTO_A_FLAGS,

			// Into F
			LD_DB_INTO_F,

			// Into B
			LD_A_INTO_B,
			LD_C_INTO_B,
			LD_D_INTO_B,
			LD_E_INTO_B,
			LD_H_INTO_B,
			LD_L_INTO_B,
			LD_IXH_INTO_B,
			LD_IXL_INTO_B,
			LD_IYH_INTO_B,
			LD_IYL_INTO_B,
			LD_DB_INTO_B,
			LD_DB_INTO_B_FLAGS,

			// Into C
			LD_A_INTO_C,
			LD_B_INTO_C,
			LD_D_INTO_C,
			LD_E_INTO_C,
			LD_H_INTO_C,
			LD_L_INTO_C,
			LD_IXH_INTO_C,
			LD_IXL_INTO_C,
			LD_IYH_INTO_C,
			LD_IYL_INTO_C,
			LD_DB_INTO_C,

			// Into D
			LD_A_INTO_D,
			LD_B_INTO_D,
			LD_C_INTO_D,
			LD_E_INTO_D,
			LD_H_INTO_D,
			LD_L_INTO_D,
			LD_IXH_INTO_D,
			LD_IXL_INTO_D,
			LD_IYH_INTO_D,
			LD_IYL_INTO_D,
			LD_DB_INTO_D,
			LD_DB_INTO_D_FLAGS,

			// Into E
			LD_A_INTO_E,
			LD_B_INTO_E,
			LD_C_INTO_E,
			LD_D_INTO_E,
			LD_H_INTO_E,
			LD_L_INTO_E,
			LD_IXH_INTO_E,
			LD_IXL_INTO_E,
			LD_IYH_INTO_E,
			LD_IYL_INTO_E,
			LD_DB_INTO_E,

			// Into H
			LD_A_INTO_H,
			LD_B_INTO_H,
			LD_C_INTO_H,
			LD_D_INTO_H,
			LD_E_INTO_H,
			LD_L_INTO_H,
			LD_DB_INTO_H,
			LD_DB_INTO_H_FLAGS,

			// Into L
			LD_A_INTO_L,
			LD_B_INTO_L,
			LD_C_INTO_L,
			LD_D_INTO_L,
			LD_E_INTO_L,
			LD_H_INTO_L,
			LD_DB_INTO_L,

			// Into IXH
			LD_A_INTO_IXH,
			LD_B_INTO_IXH,
			LD_C_INTO_IXH,
			LD_D_INTO_IXH,
			LD_E_INTO_IXH,
			LD_IXL_INTO_IXH,
			LD_DB_INTO_IXH,

			// Into IXL
			LD_A_INTO_IXL,
			LD_B_INTO_IXL,
			LD_C_INTO_IXL,
			LD_D_INTO_IXL,
			LD_E_INTO_IXL,
			LD_IXH_INTO_IXL,
			LD_DB_INTO_IXL,

			// Into IYH
			LD_A_INTO_IYH,
			LD_B_INTO_IYH,
			LD_C_INTO_IYH,
			LD_D_INTO_IYH,
			LD_E_INTO_IYH,
			LD_IYL_INTO_IYH,
			LD_DB_INTO_IYH,

			// Into IXL
			LD_A_INTO_IYL,
			LD_B_INTO_IYL,
			LD_C_INTO_IYL,
			LD_D_INTO_IYL,
			LD_E_INTO_IYL,
			LD_IYH_INTO_IYL,
			LD_DB_INTO_IYL,

			// Into another
			LD_DB_INTO_SPH,
			LD_DB_INTO_SPL,
			LD_DB_INTO_WZH,
			LD_DB_INTO_WZL,
			LD_A_INTO_WZH,
			LD_DB_INTO_DB_FLAGS,
			LD_A_INTO_I,
			LD_I_INTO_A,
			LD_A_INTO_R,
			LD_R_INTO_A,
			LD_I_INTO_WZH,

			READPC,
			READSP,
			READWR,
			READ,

			WRITE_DB,
			WRITE_A,
			WRITEF,
			WRITEB,
			WRITEC,
			WRITED,
			WRITEE,
			WRITEH,
			WRITEL,
			WRITEPCL,
			WRITEPCH,
			WRITEIXL,
			WRITEIXH,
			WRITEIYL,
			WRITEIYH,
			WRITESPL,
			WRITESPH,
			WRITE,
			PUSH_A,
			PUSH_F,
			PUSH_B,
			PUSH_C,
			PUSH_D,
			PUSH_E,
			PUSH_H,
			PUSH_L,
			PUSH_IXH,
			PUSH_IXL,
			PUSH_IYH,
			PUSH_IYL,
			PUSH_PCL,
			PUSH_PCH,
			IOWRITE,
			FILLPCL,
			FILLPCH,
			CPAWRH,

			OUTA,
			OUTC,
			OUTD,
			OUTE,
			OUTH,
			OUTL,
			READIO,

			LD_B_INTO_A,
			LD_C_INTO_A,
			LD_D_INTO_A,
			LD_E_INTO_A,
			LD_H_INTO_A,
			LD_L_INTO_A,

			JP_WZ,
			JP_0,
			JP_08,
			JP_10,
			JP_18,
			JP_20,
			JP_28,
			JP_30,
			JP_38,
			JP,
			JPZ,
			JPNZ,
			JPC,
			JPNC,
			JPPE,
			JPPO,
			JPM,
			JPP,
			JPHL,
			JPIX,
			JPIY,
			JR,

			// Increment / Decrement register 16bits
			INC_BC,
			DEC_BC,
			INC_DE,
			DEC_DE,
			INC_HL,

			DEC_HL,
			INC_IX,
			DEC_IX,
			INC_IY,
			DEC_IY,
			INCSP,
			DECSP,
			DECPC,
			INC_WZ,
			DEC_WZ,

			PUSH,

			INC_DB,
			DEC_DB,
			INC_A,
			DEC_A,
			INC_B,
			DEC_B,
			INC_C,
			DEC_C,
			INC_D,
			DEC_D,
			INC_E,
			DEC_E,
			INC_H,
			DEC_H,
			INC_L,
			DEC_L,
			INC_IXH,
			DEC_IXH,
			INC_IXL,
			DEC_IXL,
			INC_IYH,
			DEC_IYH,
			INC_IYL,
			DEC_IYL,

			ADD_DB_TO_WZ,
			ADD_WZ_TO_HL,
			ADD_WZ_TO_IX,
			ADD_WZ_TO_IY,
			ADC_WZ_TO_HL,
			SBC_WZ_TO_HL,

			ADD_A_TO_A,
			ADD_DB_TO_A,
			ADD_B_TO_A,
			ADD_C_TO_A,
			ADD_D_TO_A,
			ADD_E_TO_A,
			ADD_H_TO_A,
			ADD_L_TO_A,
			ADD_IXH_TO_A,
			ADD_IXL_TO_A,
			ADD_IYH_TO_A,
			ADD_IYL_TO_A,

			ADC_DB_TO_A,
			ADC_B_TO_A,
			ADC_C_TO_A,
			ADC_D_TO_A,
			ADC_E_TO_A,
			ADC_H_TO_A,
			ADC_L_TO_A,
			ADC_A_TO_A,
			ADC_IXH_TO_A,
			ADC_IXL_TO_A,
			ADC_IYH_TO_A,
			ADC_IYL_TO_A,

			SUB_DB_TO_A,
			SUB_A_TO_A,
			SUB_B_TO_A,
			SUB_C_TO_A,
			SUB_D_TO_A,
			SUB_E_TO_A,
			SUB_H_TO_A,
			SUB_L_TO_A,
			SUB_IXH_TO_A,
			SUB_IXL_TO_A,
			SUB_IYH_TO_A,
			SUB_IYL_TO_A,

			SBC_DB_TO_A,
			SBC_A_TO_A,
			SBC_B_TO_A,
			SBC_C_TO_A,
			SBC_D_TO_A,
			SBC_E_TO_A,
			SBC_H_TO_A,
			SBC_L_TO_A,
			SBC_IXH_TO_A,
			SBC_IXL_TO_A,
			SBC_IYH_TO_A,
			SBC_IYL_TO_A,

			AND_DB,
			AND_B,
			AND_C,
			AND_D,
			AND_E,
			AND_H,
			AND_L,
			AND_IXH,
			AND_IXL,
			AND_IYH,
			AND_IYL,
			AND_A,
			OR_DB,
			OR_B,
			OR_C,
			OR_D,
			OR_E,
			OR_H,
			OR_L,
			OR_IXH,
			OR_IXL,
			OR_IYH,
			OR_IYL,
			OR_A,
			XOR_DB,
			XOR_B,
			XOR_C,
			XOR_D,
			XOR_E,
			XOR_H,
			XOR_L,
			XOR_IXH,
			XOR_IXL,
			XOR_IYH,
			XOR_IYL,
			XOR_A,
			CP_DB,
			CP_DB_NOCARRY,
			CP_B,
			CP_C,
			CP_D,
			CP_E,
			CP_H,
			CP_L,
			CP_IXH,
			CP_IXL,
			CP_IYH,
			CP_IYL,
			CP_A,

			RLCA,
			RRCA,
			RLA,
			RRA,
			DAA,
			CPL,
			SCF,
			CCF,
			NEG,
			RRD,
			RLD,

			EXAF,
			EXX,
			EX_DE_HL,

			LD_ITER,
			CP_ITER,
			STOPZ,
			STOPNZ,
			STOPC,
			STOPNC,
			STOPPO,
			STOPPE,
			STOPM,
			STOPP,

			// BIT
			BIT0_A,
			BIT0_B,
			BIT0_C,
			BIT0_D,
			BIT0_E,
			BIT0_H,
			BIT0_L,
			BIT0_DB,
			BIT1_A,
			BIT1_B,
			BIT1_C,
			BIT1_D,
			BIT1_E,
			BIT1_H,
			BIT1_L,
			BIT1_DB,
			BIT2_A,
			BIT2_B,
			BIT2_C,
			BIT2_D,
			BIT2_E,
			BIT2_H,
			BIT2_L,
			BIT2_DB,
			BIT3_A,
			BIT3_B,
			BIT3_C,
			BIT3_D,
			BIT3_E,
			BIT3_H,
			BIT3_L,
			BIT3_DB,
			BIT4_A,
			BIT4_B,
			BIT4_C,
			BIT4_D,
			BIT4_E,
			BIT4_H,
			BIT4_L,
			BIT4_DB,
			BIT5_A,
			BIT5_B,
			BIT5_C,
			BIT5_D,
			BIT5_E,
			BIT5_H,
			BIT5_L,
			BIT5_DB,
			BIT6_A,
			BIT6_B,
			BIT6_C,
			BIT6_D,
			BIT6_E,
			BIT6_H,
			BIT6_L,
			BIT6_DB,
			BIT7_A,
			BIT7_B,
			BIT7_C,
			BIT7_D,
			BIT7_E,
			BIT7_H,
			BIT7_L,
			BIT7_DB,

			//SET
			SET0_A,
			SET0_B,
			SET0_C,
			SET0_D,
			SET0_E,
			SET0_H,
			SET0_L,
			SET0_DB,
			SET1_A,
			SET1_B,
			SET1_C,
			SET1_D,
			SET1_E,
			SET1_H,
			SET1_L,
			SET1_DB,
			SET2_A,
			SET2_B,
			SET2_C,
			SET2_D,
			SET2_E,
			SET2_H,
			SET2_L,
			SET2_DB,
			SET3_A,
			SET3_B,
			SET3_C,
			SET3_D,
			SET3_E,
			SET3_H,
			SET3_L,
			SET3_DB,
			SET4_A,
			SET4_B,
			SET4_C,
			SET4_D,
			SET4_E,
			SET4_H,
			SET4_L,
			SET4_DB,
			SET5_A,
			SET5_B,
			SET5_C,
			SET5_D,
			SET5_E,
			SET5_H,
			SET5_L,
			SET5_DB,
			SET6_A,
			SET6_B,
			SET6_C,
			SET6_D,
			SET6_E,
			SET6_H,
			SET6_L,
			SET6_DB,
			SET7_A,
			SET7_B,
			SET7_C,
			SET7_D,
			SET7_E,
			SET7_H,
			SET7_L,
			SET7_DB,

			//RES
			RES0_A,
			RES0_B,
			RES0_C,
			RES0_D,
			RES0_E,
			RES0_H,
			RES0_L,
			RES0_DB,
			RES1_A,
			RES1_B,
			RES1_C,
			RES1_D,
			RES1_E,
			RES1_H,
			RES1_L,
			RES1_DB,
			RES2_A,
			RES2_B,
			RES2_C,
			RES2_D,
			RES2_E,
			RES2_H,
			RES2_L,
			RES2_DB,
			RES3_A,
			RES3_B,
			RES3_C,
			RES3_D,
			RES3_E,
			RES3_H,
			RES3_L,
			RES3_DB,
			RES4_A,
			RES4_B,
			RES4_C,
			RES4_D,
			RES4_E,
			RES4_H,
			RES4_L,
			RES4_DB,
			RES5_A,
			RES5_B,
			RES5_C,
			RES5_D,
			RES5_E,
			RES5_H,
			RES5_L,
			RES5_DB,
			RES6_A,
			RES6_B,
			RES6_C,
			RES6_D,
			RES6_E,
			RES6_H,
			RES6_L,
			RES6_DB,
			RES7_A,
			RES7_B,
			RES7_C,
			RES7_D,
			RES7_E,
			RES7_H,
			RES7_L,
			RES7_DB,

			RLC_A,
			RLC_B,
			RLC_C,
			RLC_D,
			RLC_E,
			RLC_H,
			RLC_L,
			RLC_DB,
			RL_A,
			RL_B,
			RL_C,
			RL_D,
			RL_E,
			RL_H,
			RL_L,
			RL_DB,
			RRC_A,
			RRC_B,
			RRC_C,
			RRC_D,
			RRC_E,
			RRC_H,
			RRC_L,
			RRC_DB,
			RR_A,
			RR_B,
			RR_C,
			RR_D,
			RR_E,
			RR_H,
			RR_L,
			RR_DB,
			SLA_A,
			SLA_B,
			SLA_C,
			SLA_D,
			SLA_E,
			SLA_H,
			SLA_L,
			SLA_DB,
			SLL_A,
			SLL_B,
			SLL_C,
			SLL_D,
			SLL_E,
			SLL_H,
			SLL_L,
			SLL_DB,
			SRA_A,
			SRA_B,
			SRA_C,
			SRA_D,
			SRA_E,
			SRA_H,
			SRA_L,
			SRA_DB,
			SRL_A,
			SRL_B,
			SRL_C,
			SRL_D,
			SRL_E,
			SRL_H,
			SRL_L,
			SRL_DB,
		} ;

		static const std::vector<opcode_steps> opcodes_timing[];
		static const std::vector<opcode_steps> opcodes_timing_cb[];
		static const std::vector<opcode_steps> opcodes_timing_dd[];
		static const std::vector<opcode_steps> opcodes_timing_ddcb[];
		static const std::vector<opcode_steps> opcodes_timing_ed[];
		static const std::vector<opcode_steps> opcodes_timing_fd[];

		void fetch();
		void fetchwait();
		void decode();
		void wait();
		void readpc();
		void readsp();
		void readwr();
		void read(const bool = false);
		void push(const uint8_t);
		void write(const uint8_t, const bool = false);
		void write();
		void iowrite();
		void next_fetch();
		void jp();

		std::vector<opcode_steps> const* current_steps;
		int8_t current_steps_idx;
		opcode_steps current_step;

		void setSZXY(const uint8_t);
		void setSZXY(const uint16_t);
		void setP(const uint8_t);

		uint8_t inc(const uint8_t);
		uint8_t dec(const uint8_t);
		void rla(const bool);
		void rra(const bool);
		void daa();
		void cpl();
		void scf();
		void ccf();
		void neg();
		void rrd();
		void rld();
		uint8_t rrc(const uint8_t);
		uint8_t rr(const uint8_t);
		uint8_t rlc(const uint8_t);
		uint8_t rl(const uint8_t);
		uint8_t srl(const uint8_t);
		uint8_t sra(const uint8_t);
		uint8_t sla(const uint8_t);
		uint8_t sll(const uint8_t);
		void add(const uint8_t);
		void add16(uint16_t&, const uint16_t);
		void adc(const uint8_t);
		void adc16(const uint16_t);
		void sub(const uint8_t);
		void sbc(const uint8_t);
		void sbc16(const uint16_t);
		void ana(const uint8_t);
		void ora(const uint8_t);
		void xra(const uint8_t);
		void cp(const uint8_t, const bool = false);
		void exaf();
		void exx();
		void exdehl();
		void ld_iter();
		void cp_iter();
		void cpd();
		void bit(const uint8_t, const uint8_t);
	};
}