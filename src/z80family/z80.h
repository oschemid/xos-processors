#pragma once

#include "types.h"
#include "cpus.h"

#include <stdexcept>
#include <array>
#include <functional>

namespace xprocessors {
	typedef std::function<uint8_t(const uint8_t)> fnuint8_t;

	class Z80State {
	public:
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

		enum register8 {
			F = 0,
			A,
			C,
			B,
			E,
			D,
			L,
			H,
			IXl,
			IXh,
			IYl,
			IYh,
			R,
			I
		};

		enum register16 {
			AF = 0,
			BC,
			DE,
			HL,
			IX,
			IY,
			IR
		};

	protected:
		union {
			uint16_t word[11];	// AF, BC, DE, HL, IX, IY, AF', BC', DE', HL', IR
			uint8_t byte[22];
		} _pairs;
		uint16_t _sp;
		bool _alternative[4];

	public:
		uint16_t _pc;
		uint16_t pc() const { return _pc; }
		uint16_t& pc() { return _pc; }

		// Registers 8 bits
		uint8_t a() const { return (_alternative[0]) ? _pairs.byte[register8::A + 12] : _pairs.byte[register8::A]; }
		uint8_t& a() { return (_alternative[0]) ? _pairs.byte[register8::A + 12] : _pairs.byte[register8::A]; }
		uint8_t f() const { return (_alternative[0]) ? _pairs.byte[register8::F + 12] : _pairs.byte[register8::F]; }
		uint8_t& f() { return (_alternative[0]) ? _pairs.byte[register8::F + 12] : _pairs.byte[register8::F]; }
		uint8_t b() const { return (_alternative[1]) ? _pairs.byte[register8::B + 12] : _pairs.byte[register8::B]; }
		uint8_t& b() { return (_alternative[1]) ? _pairs.byte[register8::B + 12] : _pairs.byte[register8::B]; }
		uint8_t c() const { return (_alternative[1]) ? _pairs.byte[register8::C + 12] : _pairs.byte[register8::C]; }
		uint8_t& c() { return (_alternative[1]) ? _pairs.byte[register8::C + 12] : _pairs.byte[register8::C]; }
		uint8_t d() const { return (_alternative[2]) ? _pairs.byte[register8::D + 12] : _pairs.byte[register8::D]; }
		uint8_t& d() { return (_alternative[2]) ? _pairs.byte[register8::D + 12] : _pairs.byte[register8::D]; }
		uint8_t e() const { return (_alternative[2]) ? _pairs.byte[register8::E + 12] : _pairs.byte[register8::E]; }
		uint8_t& e() { return (_alternative[2]) ? _pairs.byte[register8::E + 12] : _pairs.byte[register8::E]; }
		uint8_t h() const { return (_alternative[3]) ? _pairs.byte[register8::H + 12] : _pairs.byte[register8::H]; }
		uint8_t& h() { return (_alternative[3]) ? _pairs.byte[register8::H + 12] : _pairs.byte[register8::H]; }
		uint8_t l() const { return (_alternative[3]) ? _pairs.byte[register8::L + 12] : _pairs.byte[register8::L]; }
		uint8_t& l() { return (_alternative[3]) ? _pairs.byte[register8::L + 12] : _pairs.byte[register8::L]; }
		uint8_t ixh() const { return _pairs.byte[register8::IXh]; }
		uint8_t& ixh() { return _pairs.byte[register8::IXh]; }
		uint8_t ixl() const { return _pairs.byte[register8::IXl]; }
		uint8_t& ixl() { return _pairs.byte[register8::IXl]; }
		uint8_t iyh() const { return _pairs.byte[register8::IYh]; }
		uint8_t& iyh() { return _pairs.byte[register8::IYh]; }
		uint8_t iyl() const { return _pairs.byte[register8::IYl]; }
		uint8_t& iyl() { return _pairs.byte[register8::IYl]; }
		uint8_t i() const { return _pairs.byte[register8::I]; }
		uint8_t& i() { return _pairs.byte[register8::I]; }
		uint8_t r() const { return _pairs.byte[register8::R]; }
		uint8_t& r() { return _pairs.byte[register8::R]; }

		// Registers 16 bits
		uint16_t af() const { return (_alternative[0]) ? _pairs.word[register16::AF + 6] : _pairs.word[register16::AF]; }
		void af(const uint16_t value) { if (_alternative[0]) _pairs.word[register16::AF + 6] = value; else _pairs.word[register16::AF] = value; }
		uint16_t bc() const { return (_alternative[1]) ? _pairs.word[register16::BC + 6] : _pairs.word[register16::BC]; }
		uint16_t& bc() { return (_alternative[1]) ? _pairs.word[register16::BC + 6] : _pairs.word[register16::BC]; }
		uint16_t de() const { return (_alternative[2]) ? _pairs.word[register16::DE + 6] : _pairs.word[register16::DE]; }
		uint16_t& de() { return (_alternative[2]) ? _pairs.word[register16::DE + 6] : _pairs.word[register16::DE]; }
		uint16_t hl() const { return (_alternative[3]) ? _pairs.word[register16::HL + 6] : _pairs.word[register16::HL]; }
		uint16_t& hl() { return (_alternative[3]) ? _pairs.word[register16::HL + 6] : _pairs.word[register16::HL]; }
		uint16_t sp() const { return _sp; }
		uint16_t& sp() { return _sp; }
		uint16_t ix() const { return _pairs.word[register16::IX]; }
		uint16_t& ix() { return _pairs.word[register16::IX]; }
		uint16_t iy() const { return _pairs.word[register16::IY]; }
		uint16_t& iy() { return _pairs.word[register16::IY]; }
		uint16_t ir() const { return _pairs.word[register16::IR]; }
		uint16_t& ir() { return _pairs.word[register16::IR]; }

		// Flags
		bool parityFlag() const { return (f() & flags::PF) ? true : false; }
		bool carryFlag() const { return (f() & flags::CF) ? true : false; }
		bool signFlag() const { return (f() & flags::SF) ? true : false; }
		bool zeroFlag() const { return (f() & flags::ZF) ? true : false; }
		bool halfCarryFlag() const { return (f() & flags::HF) ? true : false; }
		bool addSubFlag() const { return (f() & flags::NF) ? true : false; }

		void resetFlags(const uint8_t flags) { f() &= ~flags; }
		void setFlags(const uint8_t flags) { f() |= flags; }
		void setSZXY(const uint8_t value) {
			resetFlags(flags::ZF | flags::XF | flags::SF | flags::YF);
			if (value == 0)
				setFlags(flags::ZF);
			else
				setFlags(value & (flags::XF | flags::SF | flags::YF));
		}
		void setSZXY(const uint16_t value) {
			resetFlags(flags::ZF | flags::XF | flags::SF | flags::YF);
			if (value == 0)
				setFlags(flags::ZF);
			else
				setFlags((value >> 8) & (flags::XF | flags::SF | flags::YF));
		}
		void setP(const uint8_t value) {
			uint8_t t = value;
			t ^= t >> 4;
			t ^= t >> 2;
			if ((t ^= t >> 1) & 1)
				resetFlags(flags::PF);
			else
				setFlags(flags::PF);
		}

		// Alternative registers
		void exchange(const register16 r) { _alternative[r] = (_alternative[r]) ? false : true; }

		// Reset state
		Z80State() {
			reset();
		}
		void reset() {
			for (int i = 0; i < 12; _pairs.word[i++] = 0);
			for (int i = 0; i < 4; _alternative[i++] = false);
			_sp = 0;
		}
	};
	struct Z80Costs {
		static const uint8_t READ_OPCODE = 4;
		static const uint8_t READ_MEMORY8 = 3;
		static const uint8_t READ_MEMORY16 = 6;
		static const uint8_t WRITE_MEMORY8 = 3;
		static const uint8_t WRITE_MEMORY16 = 6;
		static const uint8_t WRITE_PC = 0;
		static const uint8_t READ_WRITE_R = 0;
		static const uint8_t READ_WRITE_RR = 2;
		static const uint8_t EXTRAPUSH = 1;
		static const uint8_t EXTRARET = 1;
		static const uint8_t EXTRACALL = 0;
		static const uint8_t EXTRAHALT = 0;
	};
	class Z80 : public Z80FamilyCpu<Z80State, Z80Costs>
	{
	protected:
		enum prefix {
			NO = 0,
			DD,
			FD
		};
		prefix current_prefix;

		const uint8_t decodeR(const opcode_t opcode) override {
			switch ((opcode & 0x07) | (current_prefix << 4)) {
			case 0x00:
			case 0x10:
			case 0x20:
				return _state.b();
			case 0x01:
			case 0x11:
			case 0x21:
				return _state.c();
			case 0x02:
			case 0x12:
			case 0x22:
				return _state.d();
			case 0x03:
			case 0x13:
			case 0x23:
				return _state.e();
			case 0x04:
				return _state.h();
			case 0x14:
				return _state.ixh();
			case 0x24:
				return _state.iyh();
			case 0x05:
				return _state.l();
			case 0x15:
				return _state.ixl();
			case 0x25:
				return _state.iyl();
			case 0x06:
				return read8(_state.hl());
			case 0x16:
				return read8(_state.ix());
			case 0x26:
				return read8(_state.iy());
			case 0x07:
			case 0x17:
			case 0x27:
				return _state.a();
			}
			throw std::runtime_error("Unexpected opcode in decode8 " + opcode);
		}
		void decodeR(const opcode_t opcode, const uint8_t value) override {
			switch ((opcode & 0x07) | (current_prefix << 4)) {
			case 0x00:
			case 0x10:
			case 0X20:
				_state.b() = value;
				return;
			case 0x01:
			case 0x11:
			case 0x21:
				_state.c() = value;
				return;
			case 0x02:
			case 0x12:
			case 0x22:
				_state.d() = value;
				return;
			case 0x03:
			case 0x13:
			case 0x23:
				_state.e() = value;
				return;
			case 0x04:
				_state.h() = value;
				return;
			case 0x14:
				_state.ixh() = value;
				return;
			case 0x24:
				_state.iyh() = value;
				return;
			case 0x05:
				_state.l() = value;
				return;
			case 0x15:
				_state.ixl() = value;
				return;
			case 0x25:
				_state.iyl() = value;
				return;
			case 0x06:
				write8(_state.hl(), value);
				return;
			case 0x16:
				write8(_state.ix(), value);
				return;
			case 0x26:
				write8(_state.iy(), value);
				return;
			case 0x07:
			case 0x17:
			case 0x27:
				_state.a() = value;
				return;
			}
		}
		const uint16_t decodeRR(const opcode_t opcode) const override {
			switch ((opcode & 0x30) | current_prefix) {
			case 0x00:
			case 0x01:
			case 0x02:
				return _state.bc();
			case 0x10:
			case 0x11:
			case 0x12:
				return _state.de();
			case 0x20:
				return _state.hl();
			case 0x21:
				return _state.ix();
			case 0x22:
				return _state.iy();
			case 0x30:
			case 0x31:
			case 0x32:
				return _state.sp();
			}
		}
		void decodeRR(const opcode_t opcode, const uint16_t value) override {
			switch ((opcode & 0x30) | current_prefix) {
			case 0x00:
			case 0x01:
			case 0x02:
				_state.bc() = value;
				return;
			case 0x10:
			case 0x11:
			case 0x12:
				_state.de() = value;
				return;
			case 0x20:
				_state.hl() = value;
				return;
			case 0x21:
				_state.ix() = value;
				return;
			case 0x22:
				_state.iy() = value;
				return;
			case 0x30:
			case 0x31:
			case 0x32:
				_state.sp() = value;
				return;
			}
		}
		const uint16_t decodePush(const opcode_t opcode) const override {
			switch ((opcode & 0x30) | current_prefix) {
			case 0x00:
			case 0x01:
			case 0x02:
				return _state.bc();
			case 0x10:
			case 0x11:
			case 0x12:
				return _state.de();
			case 0x20:
				return _state.hl();
			case 0x21:
				return _state.ix();
			case 0x22:
				return _state.iy();
			case 0x30:
			case 0x31:
			case 0x32:
				return _state.af();
			}
		}
		void decodePop(const opcode_t opcode, const uint16_t value) override {
			switch ((opcode & 0x30) | current_prefix) {
			case 0x00:
			case 0x01:
			case 0x02:
				_state.bc() = value;
				return;
			case 0x10:
			case 0x11:
			case 0x12:
				_state.de() = value;
				return;
			case 0x20:
				_state.hl() = value;
				return;
			case 0x21:
				_state.ix() = value;
				return;
			case 0x22:
				_state.iy() = value;
				return;
			case 0x30:
			case 0x31:
			case 0x32:
				_state.af(value);
				return;
			}
		}

		// Decode registers from opcode
		bool checkCondition2(const opcode_t) const;

		// Decode opcode
		void decode_opcode(const uint8_t);
		void decode_opcode_cb(const prefix);
		void decode_opcode_ed();

		// (HL) <- fn((HL))
		void apply_hl(const fnuint8_t, const prefix, const int8_t = 0);
		// r <- fn(r)
		void apply_r(const fnuint8_t, const opcode_t, const prefix);
		// (IXY+d) <- r <- fn((IXY+d))
		void apply_ixy_r(const fnuint8_t, const opcode_t, const prefix, const int8_t = 0);

	protected:
		enum flags {
			zeroFlag = 0x40,
			parityFlag = 0x04,
		};
		bool iff1_waiting;
		bool iff1;
		bool iff2;

		bool interrupt_waiting;
		uint8_t interrupt_request;
	public:
		enum class interrupt_mode {
			mode_0,
			mode_1,
			mode_2
		} im;

	protected:
		void add(const uint8_t) override;
		void adc(const uint8_t) override;
		void sub(const uint8_t) override;
		void sbc(const uint8_t) override;
		void ora(const uint8_t) override;
		void xra(const uint8_t) override;
		void ana(const uint8_t) override;
		void cmp(const uint8_t) override;
		void add_hl(const uint16_t) override;

		uint8_t inc(const uint8_t) override;
		uint8_t dec(const uint8_t) override;

		/* opcodes */
		void rla(const bool);
		void rra(const bool);
		void daa();
		void exchange_de_hl();
		uint16_t exchange_sp(const uint16_t);

		/* opcodes CB */
		uint8_t rlc(const uint8_t);
		uint8_t rl(const uint8_t);
		uint8_t rrc(const uint8_t);
		uint8_t rr(const uint8_t);
		uint8_t sla(const uint8_t);
		uint8_t sll(const uint8_t);
		uint8_t sra(const uint8_t);
		uint8_t srl(const uint8_t);
		void bit(const uint8_t, const uint8_t);

		/* opcodes ED */
		void rrd();
		void rld();
		void adc_ss(const opcode_t);
		void sbc_ss(const opcode_t);
		void neg();
		void cpd();
		void cpi();
		void ldd();
		void ldi();
		void in_c(const opcode_t);
		void ini();
		void ind();
		void out_c(const opcode_t);
		void outi();
		void outd();

		void unimplemented();
		void illegal();

		Z80();

	public:
		bool reset(const uint16_t = 0) override;

		const uint8_t executeOne() override;
		bool interrupt(const uint8_t) override;

		static Cpu* create() { return new Z80(); }
	};
}
