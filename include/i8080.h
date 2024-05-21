#pragma once
#include "cpu.h"


namespace xprocessors::cpu
{
	class i8080
	{
	public:
		static const uint8_t STATUS_INTA = 1;
		static const uint8_t STATUS_WO = 2;
		static const uint8_t STATUS_STACK = 4;
		static const uint8_t STATUS_HLTA = 8;
		static const uint8_t STATUS_OUT = 16;
		static const uint8_t STATUS_M1 = 32;
		static const uint8_t STATUS_INP = 64;
		static const uint8_t STATUS_MEMR = 128;

		static const uint8_t PIN_SYNC = 1;
		static const uint8_t PIN_READY = 2;
		static const uint8_t PIN_WAIT = 4;
		static const uint8_t PIN_DBIN = 8;
		static const uint8_t PIN_WR = 16;
		static const uint16_t PIN_INTE = 32;
		static const uint16_t PIN_INT = 64;

		enum flags
		{
			SF = 0x80,
			ZF = 0x40,
			HF = 0x10,
			PF = 0x04,
			CF = 0x01
		};

	public:
		void init();
		void reset(const uint16_t = 0);

		void tick();

		uint16_t getAddressBus() const { return _addressbus; }
		uint8_t getDataBus() const { return _databus; }
		uint8_t getControlPins() const { return _pins; }
		void setDataBus(const uint8_t d) { _databus = d; }
		void setReady() { _pins |= PIN_READY; }
		void resetReady() { _pins &= ~PIN_READY; }

	protected:
		uint16_t _addressbus;
		uint8_t _databus;
		uint8_t _pins;

		union { struct { register8_t pcl; register8_t pch; }; register16_t pc; };
		union { struct { register8_t f; register8_t a; }; register16_t af; };
		union { struct { register8_t c; register8_t b; }; register16_t bc; };
		union { struct { register8_t e; register8_t d; }; register16_t de; };
		union { struct { register8_t l; register8_t h; }; register16_t hl; };
		union { struct { register8_t spl; register8_t sph; }; register16_t sp; };
		union { struct { register8_t z; register8_t w; }; register16_t wz; };
		uint8_t _dbsave;

	protected:
		enum opcode_steps
		{
			FETCH,
			FETCHREAD,
			DECODE,
			READ,
			READIO,
			READINC,
			READPC,
			READSP,
			WAIT,
			WRITE,
			WRITESP,
			WRITEINC,
			WRITEIO,
			STOPWRITE,
			NOP,
			EI,
			DI,
			HALT1,
			HALT2,
			LD_DB_INTO_SAV,
			LD_SAV_INTO_DB,
			LD_SP_INTO_WZ,
			LD_HL_INTO_WZ,
			LD_BC_INTO_WZ,
			LD_DE_INTO_WZ,
			LD_DB_INTO_B,
			LD_C_INTO_B,
			LD_D_INTO_B,
			LD_E_INTO_B,
			LD_H_INTO_B,
			LD_L_INTO_B,
			LD_A_INTO_B,
			LD_DB_INTO_C,
			LD_B_INTO_C,
			LD_D_INTO_C,
			LD_E_INTO_C,
			LD_H_INTO_C,
			LD_L_INTO_C,
			LD_A_INTO_C,
			LD_DB_INTO_D,
			LD_B_INTO_D,
			LD_C_INTO_D,
			LD_E_INTO_D,
			LD_H_INTO_D,
			LD_L_INTO_D,
			LD_A_INTO_D,
			LD_DB_INTO_E,
			LD_B_INTO_E,
			LD_C_INTO_E,
			LD_D_INTO_E,
			LD_H_INTO_E,
			LD_L_INTO_E,
			LD_A_INTO_E,
			LD_DB_INTO_H,
			LD_B_INTO_H,
			LD_C_INTO_H,
			LD_D_INTO_H,
			LD_E_INTO_H,
			LD_L_INTO_H,
			LD_A_INTO_H,
			LD_DB_INTO_L,
			LD_B_INTO_L,
			LD_C_INTO_L,
			LD_D_INTO_L,
			LD_E_INTO_L,
			LD_H_INTO_L,
			LD_A_INTO_L,
			LD_DB_INTO_A,
			LD_DB_INTO_F,
			LD_B_INTO_A,
			LD_C_INTO_A,
			LD_D_INTO_A,
			LD_E_INTO_A,
			LD_H_INTO_A,
			LD_L_INTO_A,
			LD_B_INTO_DB,
			LD_C_INTO_DB,
			LD_D_INTO_DB,
			LD_E_INTO_DB,
			LD_H_INTO_DB,
			LD_L_INTO_DB,
			LD_A_INTO_DB,
			LD_F_INTO_DB,
			LD_W_INTO_DB,
			LD_DB_INTO_W,
			LD_DB_INTO_Z,
			LD_DB_INTO_SPL,
			LD_DB_INTO_SPH,
			LD_DB_INTO_PCL,
			LD_DB_INTO_PCH,
			LD_PCL_INTO_DB,
			LD_PCH_INTO_DB,
			LD_WZ_INTO_SP,
			LD_WZ_INTO_HL,
			LD_HL_INTO_PC,
			EX_DE_HL,

			ADD_WZ_TO_HL,
			ADD_B_TO_A,
			ADD_C_TO_A,
			ADD_D_TO_A,
			ADD_E_TO_A,
			ADD_H_TO_A,
			ADD_L_TO_A,
			ADD_A_TO_A,
			ADD_DB_TO_A,
			ADC_B_TO_A,
			ADC_C_TO_A,
			ADC_D_TO_A,
			ADC_E_TO_A,
			ADC_H_TO_A,
			ADC_L_TO_A,
			ADC_A_TO_A,
			ADC_DB_TO_A,
			SUB_B_TO_A,
			SUB_C_TO_A,
			SUB_D_TO_A,
			SUB_E_TO_A,
			SUB_H_TO_A,
			SUB_L_TO_A,
			SUB_A_TO_A,
			SUB_DB_TO_A,
			SBB_B_TO_A,
			SBB_C_TO_A,
			SBB_D_TO_A,
			SBB_E_TO_A,
			SBB_H_TO_A,
			SBB_L_TO_A,
			SBB_A_TO_A,
			SBB_DB_TO_A,

			AND_B,
			AND_C,
			AND_D,
			AND_E,
			AND_H,
			AND_L,
			AND_A,
			AND_DB,
			OR_B,
			OR_C,
			OR_D,
			OR_E,
			OR_H,
			OR_L,
			OR_A,
			OR_DB,
			XOR_B,
			XOR_C,
			XOR_D,
			XOR_E,
			XOR_H,
			XOR_L,
			XOR_A,
			XOR_DB,
			CP_B,
			CP_C,
			CP_D,
			CP_E,
			CP_H,
			CP_L,
			CP_A,
			CP_DB,

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
			INC_DB,
			DEC_DB,
			INC_BC,
			DEC_BC,
			INC_DE,
			DEC_DE,
			INC_HL,
			DEC_HL,
			INC_SP,
			DEC_SP,

			RLC,
			RRC,
			RAL,
			RAR,
			DAA,
			CMA,
			STC,
			CMC,

			STOPZ,
			STOPNZ,
			STOPC,
			STOPNC,
			STOPPE,
			STOPPO,
			STOPP,
			STOPM,

			JP,
			JP_WZ,
			JP_0,
			JP_8,
			JP_10,
			JP_18,
			JP_20,
			JP_28,
			JP_30,
			JP_38,
			JPNZ,
			JPZ,
			JPNC,
			JPC,
			JPPE,
			JPPO,
			JPP,
			JPM,
		};

		static const std::vector<opcode_steps> opcodes_timing[];
		std::vector<opcode_steps> const* _current_steps;
		opcode_steps _current_step;
		int8_t _current_steps_idx;
		uint8_t _opcode;

		bool _halted;
		bool _intreq;
		bool _intgo;

		void setINTE(const bool flag) { if (flag) _pins |= PIN_INTE; else _pins &= ~PIN_INTE; }

		void setSZP(const uint8_t);

		void fetch();
		void next_fetch();
		void fetchread();
		void decode();
		void read();
		void readio();
		void readsp();
		void readpc();
		void wait();
		void write();
		void writeio();
		void writesp();
		void writedb(const uint8_t);
		void stopwrite();

		void jp();

		void add(const uint8_t);
		void adc(const uint8_t);
		void sub(const uint8_t);
		void sbb(const uint8_t);
		void ana(const uint8_t);
		void ora(const uint8_t);
		void xra(const uint8_t);
		void cp(const uint8_t);
		uint8_t inc(const uint8_t);
		uint8_t dec(const uint8_t);

		void rlc();
		void rrc();
		void ral();
		void rar();
		void daa();
	};

	class i8080dbg : public i8080
	{
	public:
		i8080dbg();
		void setMemoryAccessor(std::function<uint8_t(const uint16_t)>);

		[[nodiscard]] bool isrunning();

		void pause() { _running = false; }
		void run() { _step = false; _running = true; }
		void runStep() { _step = true; _step1 = false; _running = true; }

		string opcode() const;
		uint8_t getc() const { return c; }
		uint8_t gete() const { return e; }
		uint16_t getde() const { return de; }
	protected:
		std::function<uint8_t(const uint16_t)> _memoryfn{ nullptr };
		bool _running{ true };
		bool _step{ false };
		bool _step1{ false };

		string disassembly(uint16_t&) const;
	};
}