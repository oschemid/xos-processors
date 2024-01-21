#pragma once
#include "xprocessors.h"
#include "../cpu/cpu.h"


namespace xprocessors::experimental
{

	class Z80
	{
	public:
		static const uint16_t PIN_M1 = 1;
		static const uint16_t PIN_MREQ = 2;
		static const uint16_t PIN_IORQ = 4;
		static const uint16_t PIN_RD = 8;
		static const uint16_t PIN_WR = 16;
		static const uint16_t PIN_HALT = 32;
		static const uint16_t PIN_RFSH = 64;

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

		void tick();

		uint16_t getAddressBus() const { return addressbus; }
		uint8_t getDataBus() const { return databus; }
		void setDataBus(const uint8_t d) { databus = d; }
		uint16_t getControlPins() const { return pins; }
	public:
		// Registers
		union { struct { register8_t f; register8_t a; }; register16_t af; };
		union { struct { register8_t c; register8_t b; }; register16_t bc; };
		union { struct { register8_t e; register8_t d; }; register16_t de; };
		union {
			struct {
				union { struct { register8_t l; register8_t h; }; register16_t hl; };
				union { struct { register8_t ixl; register8_t ixh; }; register16_t ix; };
				union { struct { register8_t iyl; register8_t iyh; }; register16_t iy; };
			};
			struct { union { struct { register8_t l; register8_t h; }; register16_t hl; }; } hli[3];
		};
		register16_t afprime, bcprime, deprime, hlprime;
		register16_t pc;
		register16_t sp;
		bool iff1;
		bool iff1_requested;

		// Process parameter
		std::function<void(Z80*)> step;
		uint8_t hli_idx;
		uint8_t opcode;
		uint16_t buffer16;

		// Pins
		uint16_t addressbus;
		uint8_t databus;
		uint16_t pins;

	protected:
		static std::function<void(Z80*)> decode_opcodes[];
		static std::function<void(Z80*)> decode_opcodes_ed[];
		static std::function<void(Z80*)> decode_opcodes_cb[];

		void _fetch(std::function<void(Z80*)>*);
		void fetch();
		void fetchDD();
		void fetchFD();

		void readArgs(std::function<void(Z80*)>);
		void readArgs16(std::function<void(Z80*)>);
		void read(const uint16_t, std::function<void(Z80*)>);
		void write(const uint16_t, const uint8_t, std::function<void(Z80*)>);
		void read16(const uint16_t, std::function<void(Z80*)>);
		void iowrite(const uint16_t, const uint8_t, std::function<void(Z80*)>);
		void wait(const uint8_t, std::function<void(Z80*)>);
		void read_hli(std::function<void(Z80*)>);

		uint8_t increment(const uint8_t);
		uint8_t decrement(const uint8_t);
		void setSZXY(const uint8_t);
		void setSZXY(const uint16_t);
		void setP(const uint8_t);

		// CB opcodes
		void fetchCB();
		void bit(const uint8_t, const uint8_t);
		void bit_hl(const uint8_t);
		void set(register8_t&, const uint8_t);
		void set_hl(const uint8_t);
		void res(register8_t&, const uint8_t);
		void res_hl(const uint8_t);
		void rlc(register8_t&, std::function<void(Z80*)> = &Z80::fetch);
		void rlc_hl();
		void rl(register8_t&, std::function<void(Z80*)> = &Z80::fetch);
		void rl_hl();
		void rrc(register8_t&, std::function<void(Z80*)> = &Z80::fetch);
		void rrc_hl();
		void rr(register8_t&, std::function<void(Z80*)> = &Z80::fetch);
		void rr_hl();
		void sla(register8_t&, std::function<void(Z80*)> = &Z80::fetch);
		void sla_hl();
		void sra(register8_t&, std::function<void(Z80*)> = &Z80::fetch);
		void sra_hl();
		void sll(register8_t&, std::function<void(Z80*)> = &Z80::fetch);
		void sll_hl();
		void srl(register8_t&, std::function<void(Z80*)> = &Z80::fetch);
		void srl_hl();

		// ED opcodes
		void fetchED();
		void sbc(const register16_t);
		void adc(const register16_t);
		void neg();
		void rrd();
		void rld();
		void ldi(std::function<void(Z80*)> = &Z80::fetch);
		void ldir();
		void ldd(std::function<void(Z80*)> = &Z80::fetch);
		void lddr();
		void cpd(std::function<void(Z80*)> = &Z80::fetch);
		void cpdr();
		void cpi(std::function<void(Z80*)> = &Z80::fetch);
		void cpir();

		void nop();
		void halt();
		void ei();
		void di();
		void ld_r_r(register8_t&, const register8_t);
		void ld_r_n(register8_t&);
		void ld_a_nn();
		void ld_nn_a();
		void ld_r_rr(register8_t&, const uint16_t);
		void ld_rr_nn(register16_t&);
		void ld_rr_r(const uint16_t, const register8_t);
		void ld_hl_r(const register8_t);
		void ld_nn_hl();
		void ld_nnnn_rr(const register16_t);
		void ld_rr_nnnn(register16_t&);
		void ld_hl_nn();
		void ld_hl_n();
		void ld_sp_hl();
		void ld_r_hl(register8_t&);
		void inc(register8_t&);
		void inc(register16_t&);
		void inc_hl();
		void dec(register8_t&);
		void dec(register16_t&);
		void dec_hl();
		void add(const register8_t);
		void add_n();
		void add_hl(const register16_t);
		void adc(const register8_t);
		void adc_n();
		void sub(const register8_t);
		void sub_n();
		void sbc(const register8_t);
		void sbc_n();

		void cp(const register8_t, std::function<void(Z80*)> = &Z80::fetch);
		void cp_n();
		void and_n();
		void ana(const register8_t);
		void ora(const register8_t);
		void ora_n();
		void xra(const register8_t);
		void xra_n();
		void rla(const bool);
		void rra(const bool);
		void daa();
		void cpl();
		void scf();
		void ccf();
		void djnz();
		void call(const bool);
		void ret();
		void ret_c(const bool);
		void ex_af();
		void ex_de_hl();
		void ex_sp_hl();
		void exx();

		void push(const register16_t);
		void pop(register16_t&);

		void jr_c(const bool);
		void jp_c(const bool);
		void jp_hl();

		void out_a();

	};
}