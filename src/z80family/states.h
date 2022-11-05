#pragma once

#include "types.h"

namespace xprocessors {
	template<class F> class Z80FamilyState {
	protected:
		register8_t _a;
		register8_t _f;
		pair16_t _bc;
		pair16_t _de;
		pair16_t _hl;
		register16_t _pc;
		register16_t _sp;

	public:
		// Registers 8 bits
		uint8_t a() const { return _a; }
		uint8_t& a() { return _a; }
		uint8_t b() const { return _bc.byte[1]; }
		uint8_t& b() { return _bc.byte[1]; }
		uint8_t c() const { return _bc.byte[0]; }
		uint8_t& c() { return _bc.byte[0]; }
		uint8_t d() const { return _de.byte[1]; }
		uint8_t& d() { return _de.byte[1]; }
		uint8_t e() const { return _de.byte[0]; }
		uint8_t& e() { return _de.byte[0]; }
		uint8_t h() const { return _hl.byte[1]; }
		uint8_t& h() { return _hl.byte[1]; }
		uint8_t l() const { return _hl.byte[0]; }
		uint8_t& l() { return _hl.byte[0]; }

		// Registers 16 bits
		uint16_t af() const { return (_a << 8) | _f | F::MASK; }
		void af(const uint16_t value) { _a = value >> 8; _f = value & F::ALL; }
		uint16_t bc() const { return _bc.word; }
		uint16_t& bc() { return _bc.word; }
		uint16_t de() const { return _de.word; }
		uint16_t& de() { return _de.word; }
		uint16_t hl() const { return _hl.word; }
		uint16_t& hl() { return _hl.word; }
		uint16_t sp() const { return _sp; }
		uint16_t& sp() { return _sp; }
		uint16_t pc() const { return _pc; }
		uint16_t& pc() { return _pc; }

		// Flags
		bool parityFlag() const { return (_f & F::PF) ? true : false; }
		void setParityFlag(const bool v) { setFlag(F::PF, v); }
		bool carryFlag() const { return (_f & F::CF) ? true : false; }
		void setCarryFlag(const bool v) { setFlag(F::CF, v); }
		bool signFlag() const { return (_f & F::SF) ? true : false; }
		void setSignFlag(const bool v) { setFlag(F::SF, v); }
		bool zeroFlag() const { return (_f & F::ZF) ? true : false; }
		void setZeroFlag(const bool v) { setFlag(F::ZF, v); }
		bool halfCarryFlag() const { return (_f & F::HF) ? true : false; }
		void setHalfCarryFlag(const bool v) { setFlag(F::HF, v); }
		bool addSubFlag() const { return (_f & F::NF) ? true : false; }
		void setAddSubFlag(const bool v) { setFlag(F::NF, v); }

		void setFlag(const uint8_t flag, const bool value) {
			value ? _f |= flag : _f &= ~flag;
		}
		void adjustZ(const uint8_t value) {
			setFlag(F::ZF, value == 0);
		}
		void adjustSZ(const uint8_t value) {
			adjustZ(value);
			setFlag(F::SF, value >= 0x80);
		}
		void resetFlags(const uint8_t flags) { _f &= ~flags; }
		void setFlags(const uint8_t flags) { _f |= flags; }
		void setSZXY(const uint8_t value) {
			resetFlags(F::ZF | F::XF | F::SF | F::YF);
			if (value == 0)
				setFlags(F::ZF);
			else
				setFlags(value & (F::XF | F::SF | F::YF));
		}
		void setSZXY(const uint16_t value) {
			resetFlags(F::ZF | F::XF | F::SF | F::YF);
			if (value == 0)
				setFlags(F::ZF);
			else
				setFlags((value >> 8) & (F::XF | F::SF | F::YF));
		}
		void setP(const uint8_t value) {
			uint8_t t = value;
			t ^= t >> 4;
			t ^= t >> 2;
			if ((t ^= t >> 1) & 1)
				resetFlags(F::PF);
			else
				setFlags(F::PF);
		}

	public:
		Z80FamilyState() {
			reset();
		}
		void reset() {
			_a = 0;
			_f = 0;
			_bc.word = 0;
			_de.word = 0;
			_hl.word = 0;
			_sp = 0;
		}
	};
}
