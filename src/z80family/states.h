#pragma once

#include "types.h"

namespace xprocessors {
	template<class F> class Z80FamilyState {
	protected:
		pair16_t _af;
		pair16_t _bc;
		pair16_t _de;
		pair16_t _hl;
		register16_t _pc;
		register16_t _sp;

	public:
		// Registers 8 bits
		uint8_t a() const { return _af.byte[1]; }
		uint8_t& a() { return _af.byte[1]; }
		uint8_t f() const { return _af.byte[0]; }
		uint8_t& f() { return _af.byte[0]; }
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
		uint16_t af() const { return _af.word; }
		uint16_t& af() { return _af.word; }
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
		bool parityFlag() const { return (f() & F::PF) ? true : false; }
		void setParityFlag(const bool v) { v ? f() |= F::PF : f() &= ~F::PF; }
		bool carryFlag() const { return (f() & static_cast<uint8_t>(F::CF)) ? true : false; }
		void setCarryFlag(const bool v) { v ? f() |= F::CF : f() &= ~F::CF; }
		bool signFlag() const { return (f() & static_cast<uint8_t>(F::SF)) ? true : false; }
		void setSignFlag(const bool v) { v ? f() |= F::SF : f() &= ~F::SF; }
		bool zeroFlag() const { return (f() & static_cast<uint8_t>(F::ZF)) ? true : false; }
		void setZeroFlag(const bool v) { v ? f() |= F::ZF : f() &= ~F::ZF; }
		bool halfCarryFlag() const { return (f() & F::HF) ? true : false; }
		bool addSubFlag() const { return (f() & F::NF) ? true : false; }

		void setFlag(const F flag, const bool value) {
			value ? f() |= static_cast<uint8_t>(flag) : f() &= ~static_cast<uint8_t>(flag);
		}
		void adjustSZ(const uint8_t value) {
			setFlag(F::ZF, value == 0);
			setFlag(F::SF, value >= 0x80);
		}
		void resetFlags(const F flags) { f() &= ~static_cast<uint8_t>(flags); }
		void setFlags(const F flags) { f() |= static_cast<uint8_t>(flags); }
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
			af() = 0;
			bc() = 0;
			de() = 0;
			hl() = 0;
			sp() = 0;
		}
	};
}
