#pragma once
#include "xprocessors.h"
#include "cpu.h"


namespace xprocessors::cpu
{
	class sm83
	{
	public:
		static const uint16_t PIN_M1 = 1;
		//static const uint16_t PIN_MREQ = 2;
		//static const uint16_t PIN_IORQ = 4;
		static const uint16_t PIN_RD = 8;
		static const uint16_t PIN_WR = 16;
		//static const uint16_t PIN_HALT = 32;
		//static const uint16_t PIN_RFSH = 64;
		//static const uint16_t PIN_INT = 128;

		enum flags
		{
			ZF = 0x80,
			NF = 0x40,
			HF = 0x20,
			CF = 0x10
		};

	public:
		void init();
		void reset(const uint16_t = 0);

		void tick();

		uint16_t getAddressBus() const { return addressbus; }
		uint8_t getDataBus() const { return databus; }
		void setDataBus(const uint8_t d) { databus = d; }
		uint16_t getControlPins() const { return pins; }
		void interrupt(const uint8_t i) { _if |= i; }

	protected:
		// Registers
		union { struct { register8_t f; register8_t a; }; register16_t af; };
		union { struct { register8_t c; register8_t b; }; register16_t bc; };
		union { struct { register8_t e; register8_t d; }; register16_t de; };
		union { struct { register8_t l; register8_t h; }; register16_t hl; };

		union { struct { register8_t pcl; register8_t pch; }; register16_t pc; };
		union { struct { register8_t spl; register8_t sph; }; register16_t sp; };

		uint16_t ir{ 0 };

		union { struct { register8_t z; register8_t w; }; register16_t wz; };
		uint8_t _ime{ 0 };
		bool _halted{ false };
		uint8_t _ie{ 0 };
		uint8_t _if{ 0 };
		uint8_t _interrupt_in_process{ 0 };
		void interrupt_step1();

		// Pins
		uint16_t addressbus;
		uint8_t databus;
		uint16_t pins;

	protected:
		enum class addressbus_operation
		{
			NONE,
			PUSH_PC,
			PUSH_BC,
			PUSH_DE,
			PUSH_HL,
			PUSH_SP,
			PUSH_WZ,
			PUSH_Z,
			PUSH_C,
		};
		enum class databus_operation
		{
			NONE,
			POP_IR,
			POP_Z,
			POP_W,
			PUSH_A,
			PUSH_F,
			PUSH_B,
			PUSH_C,
			PUSH_D,
			PUSH_E,
			PUSH_H,
			PUSH_L,
			PUSH_Z,
			PUSH_SPL,
			PUSH_SPH,
			PUSH_PCH,
			PUSH_PCL,
		};
		enum class idu_operation
		{
			NONE,
			INC_PC,
			INC_BC,
			INC_DE,
			INC_HL,
			INC_SP,
			INC_WZ,
			DEC_BC,
			DEC_DE,
			DEC_HL,
			DEC_SP,
			ADD_Z_TO_PC,
			ADD_Z_TO_SP,
			HL_1_TO_PC,
			HL_TO_SP,
		};
		enum class alu_operation
		{
			NONE,

			A_TO_B,
			A_TO_C,
			A_TO_D,
			A_TO_E,
			A_TO_H,
			A_TO_L,

			B_TO_A,
			B_TO_C,
			B_TO_D,
			B_TO_E,
			B_TO_H,
			B_TO_L,

			C_TO_A,
			C_TO_B,
			C_TO_D,
			C_TO_E,
			C_TO_H,
			C_TO_L,

			D_TO_A,
			D_TO_B,
			D_TO_C,
			D_TO_E,
			D_TO_H,
			D_TO_L,

			E_TO_A,
			E_TO_B,
			E_TO_C,
			E_TO_D,
			E_TO_H,
			E_TO_L,

			H_TO_A,
			H_TO_B,
			H_TO_C,
			H_TO_D,
			H_TO_E,
			H_TO_L,

			L_TO_A,
			L_TO_B,
			L_TO_C,
			L_TO_D,
			L_TO_E,
			L_TO_H,

			Z_TO_A,
			Z_TO_B,
			Z_TO_C,
			Z_TO_D,
			Z_TO_E,
			Z_TO_H,
			Z_TO_L,

			INC_B,
			INC_C,
			INC_D,
			INC_E,
			INC_H,
			INC_L,
			INC_A,
			INC_Z,
			DEC_B,
			DEC_C,
			DEC_D,
			DEC_E,
			DEC_H,
			DEC_L,
			DEC_A,
			DEC_Z,
			RLCA,
			RLC_A,
			RLC_B,
			RLC_C,
			RLC_D,
			RLC_E,
			RLC_H,
			RLC_L,
			RLC_Z,
			RLA,
			RL_A,
			RL_B,
			RL_C,
			RL_D,
			RL_E,
			RL_H,
			RL_L,
			RL_Z,
			RRA,
			RR_A,
			RR_B,
			RR_C,
			RR_D,
			RR_E,
			RR_H,
			RR_L,
			RR_Z,
			RRCA,
			RRC_A,
			RRC_B,
			RRC_C,
			RRC_D,
			RRC_E,
			RRC_H,
			RRC_L,
			RRC_Z,
			SRL_A,
			SRL_B,
			SRL_C,
			SRL_D,
			SRL_E,
			SRL_H,
			SRL_L,
			SRL_Z,
			SLA_A,
			SLA_B,
			SLA_C,
			SLA_D,
			SLA_E,
			SLA_H,
			SLA_L,
			SLA_Z,
			SRA_A,
			SRA_B,
			SRA_C,
			SRA_D,
			SRA_E,
			SRA_H,
			SRA_L,
			SRA_Z,
			SWAP_A,
			SWAP_B,
			SWAP_C,
			SWAP_D,
			SWAP_E,
			SWAP_H,
			SWAP_L,
			SWAP_Z,

			ADD_B_TO_H,
			ADD_C_TO_L,
			ADD_D_TO_H,
			ADD_E_TO_L,
			ADD_L_TO_L,
			ADD_H_TO_H,
			ADD_SPL_TO_L,
			ADD_SPH_TO_H,
			ADD_B_TO_A,
			ADD_C_TO_A,
			ADD_D_TO_A,
			ADD_E_TO_A,
			ADD_H_TO_A,
			ADD_L_TO_A,
			ADD_A_TO_A,
			ADD_Z_TO_A,
			ADC_B_TO_A,
			ADC_C_TO_A,
			ADC_D_TO_A,
			ADC_E_TO_A,
			ADC_H_TO_A,
			ADC_L_TO_A,
			ADC_A_TO_A,
			ADC_Z_TO_A,
			SUB_B_TO_A,
			SUB_C_TO_A,
			SUB_D_TO_A,
			SUB_E_TO_A,
			SUB_H_TO_A,
			SUB_L_TO_A,
			SUB_A_TO_A,
			SUB_Z_TO_A,
			SBC_B_TO_A,
			SBC_C_TO_A,
			SBC_D_TO_A,
			SBC_E_TO_A,
			SBC_H_TO_A,
			SBC_L_TO_A,
			SBC_A_TO_A,
			SBC_Z_TO_A,

			AND_B,
			AND_C,
			AND_D,
			AND_E,
			AND_H,
			AND_L,
			AND_A,
			AND_Z,
			OR_B,
			OR_C,
			OR_D,
			OR_E,
			OR_H,
			OR_L,
			OR_A,
			OR_Z,
			XOR_B,
			XOR_C,
			XOR_D,
			XOR_E,
			XOR_H,
			XOR_L,
			XOR_A,
			XOR_Z,
			CP_B,
			CP_C,
			CP_D,
			CP_E,
			CP_H,
			CP_L,
			CP_A,
			CP_Z,

			DAA,
			CPL,
			CCF,
			SCF,
			BIT_0_B,
			BIT_0_C,
			BIT_0_D,
			BIT_0_E,
			BIT_0_H,
			BIT_0_L,
			BIT_0_A,
			BIT_0_Z,
			BIT_1_B,
			BIT_1_C,
			BIT_1_D,
			BIT_1_E,
			BIT_1_H,
			BIT_1_L,
			BIT_1_A,
			BIT_1_Z,
			BIT_2_B,
			BIT_2_C,
			BIT_2_D,
			BIT_2_E,
			BIT_2_H,
			BIT_2_L,
			BIT_2_A,
			BIT_2_Z,
			BIT_3_B,
			BIT_3_C,
			BIT_3_D,
			BIT_3_E,
			BIT_3_H,
			BIT_3_L,
			BIT_3_A,
			BIT_3_Z,
			BIT_4_B,
			BIT_4_C,
			BIT_4_D,
			BIT_4_E,
			BIT_4_H,
			BIT_4_L,
			BIT_4_A,
			BIT_4_Z,
			BIT_5_B,
			BIT_5_C,
			BIT_5_D,
			BIT_5_E,
			BIT_5_H,
			BIT_5_L,
			BIT_5_A,
			BIT_5_Z,
			BIT_6_B,
			BIT_6_C,
			BIT_6_D,
			BIT_6_E,
			BIT_6_H,
			BIT_6_L,
			BIT_6_A,
			BIT_6_Z,
			BIT_7_B,
			BIT_7_C,
			BIT_7_D,
			BIT_7_E,
			BIT_7_H,
			BIT_7_L,
			BIT_7_A,
			BIT_7_Z,
			RES_0_B,
			RES_0_C,
			RES_0_D,
			RES_0_E,
			RES_0_H,
			RES_0_L,
			RES_0_A,
			RES_0_Z,
			RES_1_B,
			RES_1_C,
			RES_1_D,
			RES_1_E,
			RES_1_H,
			RES_1_L,
			RES_1_A,
			RES_1_Z,
			RES_2_B,
			RES_2_C,
			RES_2_D,
			RES_2_E,
			RES_2_H,
			RES_2_L,
			RES_2_A,
			RES_2_Z,
			RES_3_B,
			RES_3_C,
			RES_3_D,
			RES_3_E,
			RES_3_H,
			RES_3_L,
			RES_3_A,
			RES_3_Z,
			RES_4_B,
			RES_4_C,
			RES_4_D,
			RES_4_E,
			RES_4_H,
			RES_4_L,
			RES_4_A,
			RES_4_Z,
			RES_5_B,
			RES_5_C,
			RES_5_D,
			RES_5_E,
			RES_5_H,
			RES_5_L,
			RES_5_A,
			RES_5_Z,
			RES_6_B,
			RES_6_C,
			RES_6_D,
			RES_6_E,
			RES_6_H,
			RES_6_L,
			RES_6_A,
			RES_6_Z,
			RES_7_B,
			RES_7_C,
			RES_7_D,
			RES_7_E,
			RES_7_H,
			RES_7_L,
			RES_7_A,
			RES_7_Z,
			SET_0_B,
			SET_0_C,
			SET_0_D,
			SET_0_E,
			SET_0_H,
			SET_0_L,
			SET_0_A,
			SET_0_Z,
			SET_1_B,
			SET_1_C,
			SET_1_D,
			SET_1_E,
			SET_1_H,
			SET_1_L,
			SET_1_A,
			SET_1_Z,
			SET_2_B,
			SET_2_C,
			SET_2_D,
			SET_2_E,
			SET_2_H,
			SET_2_L,
			SET_2_A,
			SET_2_Z,
			SET_3_B,
			SET_3_C,
			SET_3_D,
			SET_3_E,
			SET_3_H,
			SET_3_L,
			SET_3_A,
			SET_3_Z,
			SET_4_B,
			SET_4_C,
			SET_4_D,
			SET_4_E,
			SET_4_H,
			SET_4_L,
			SET_4_A,
			SET_4_Z,
			SET_5_B,
			SET_5_C,
			SET_5_D,
			SET_5_E,
			SET_5_H,
			SET_5_L,
			SET_5_A,
			SET_5_Z,
			SET_6_B,
			SET_6_C,
			SET_6_D,
			SET_6_E,
			SET_6_H,
			SET_6_L,
			SET_6_A,
			SET_6_Z,
			SET_7_B,
			SET_7_C,
			SET_7_D,
			SET_7_E,
			SET_7_H,
			SET_7_L,
			SET_7_A,
			SET_7_Z,
		};
		enum class misc_operation
		{
			NONE,
			CB,
			DI,
			EI,
			HALT,
			STOP,

			WZ_TO_AF,
			WZ_TO_BC,
			WZ_TO_DE,
			WZ_TO_HL,
			WZ_TO_SP,
			WZ_TO_PC,
			RST0_TO_PC,
			RST8_TO_PC,
			RST10_TO_PC,
			RST18_TO_PC,
			RST20_TO_PC,
			RST28_TO_PC,
			RST30_TO_PC,
			RST38_TO_PC,

			CHECK_Z,
			CHECK_NZ,
			CHECK_C,
			CHECK_NC,
		};
		struct step
		{
			addressbus_operation addressbus{ addressbus_operation::NONE };
			databus_operation databus{ databus_operation::NONE };
			idu_operation idu{ idu_operation::NONE };
			alu_operation alu{ alu_operation::NONE };
			misc_operation misc{ misc_operation::NONE };
		};
		step _current_step;
		int8_t _tcycle{ 0 };
		int8_t _tstep{ 0 };
		bool _skip{ false };
		void process(const addressbus_operation);
		void process(const databus_operation);
		void process(const idu_operation);
		void process(const alu_operation);
		void process(const misc_operation);

		static const std::vector<step> opcodes_steps[];

		// IDU
		uint16_t add_sp(const uint8_t);

		// ALU
		uint8_t inc(const uint8_t);
		uint8_t dec(const uint8_t);
		uint8_t rlc(const uint8_t);
		uint8_t rl(const uint8_t);
		uint8_t rr(const uint8_t);
		uint8_t rrc(const uint8_t);
		uint8_t srl(const uint8_t);
		uint8_t sla(const uint8_t);
		uint8_t sra(const uint8_t);
		uint8_t swap(const uint8_t);
		uint8_t add(const uint8_t, const uint8_t);
		void add(const uint8_t);
		uint8_t adc(const uint8_t, const uint8_t);
		void adc(const uint8_t);
		void sub(const uint8_t);
		void sbc(const uint8_t);
		void ana(const uint8_t);
		void xra(const uint8_t);
		void ora(const uint8_t);
		void cp(const uint8_t);
		void daa();
		void cpl();
		void scf();
		void ccf();
		void bit(const uint8_t, const uint8_t);
	};

	class sm83dbg : public sm83
	{
	public:
		sm83dbg();
		void setMemoryAccessor(std::function<uint8_t(const uint16_t)>);

		[[nodiscard]] bool isrunning();

		void pause() { _running = false; }
		void run() { _step = false; _running = true; }
		void runStep() { _step = true; _step1 = 0; _running = true; }

		string opcode() const;
		uint8_t getc() const { return c; }
		uint8_t gete() const { return e; }
		uint16_t getde() const { return de; }
		uint16_t getpc() const { return pc; }
	protected:
		std::function<uint8_t(const uint16_t)> _memoryfn{ nullptr };
		bool _running{ true };
		bool _step{ false };
		uint8_t _step1{ 0 };

		string disassembly(uint16_t&) const;
	};
}