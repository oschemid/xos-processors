#pragma once

#include "states.h"
#include "cpus.h"


namespace xprocessors {
	struct LR35902Flags {
		static const uint8_t MASK = 0x00;
		static const uint8_t SF = 0x80;
		static const uint8_t ZF = 0x40;
		static const uint8_t CF = 0x01;
		static const uint8_t PF = 0x00;
		static const uint8_t ALL = SF | CF | ZF;
	};
	struct LR35902Costs {
		static const uint8_t READ_OPCODE = 4;
		static const uint8_t READ_MEMORY8 = 4;
		static const uint8_t READ_MEMORY16 = 8;
		static const uint8_t WRITE_MEMORY8 = 4;
		static const uint8_t WRITE_MEMORY16 = 8;
		static const uint8_t READ_WRITE_R = 0;
		static const uint8_t WRITE_PC = 4;
		static const uint8_t EXTRAPUSH = 0;
		static const uint8_t EXTRARETCALL = 0;
		static const uint8_t EXTRAHALT = 0;
	};


	class LR35902State : public Z80FamilyState<LR35902Flags> {

	};

	class LR35902 : public Z80FamilyCpu<LR35902State, LR35902Costs>
	{
	protected:
		uint8_t interrupt_enabled; // 0 ok, 1 wait one, 2 no
		uint8_t interrupt_request;

		uint8_t auxCarryBit : 1; // auxiliary carry bit
		uint8_t parityBit : 1; // parity bit

		void add(const uint8_t) override;
		void adc(const uint8_t) override;
		void sub(const uint8_t) override;
		void sbc(const uint8_t) override;
		void ana(const uint8_t) override;
		void ora(const uint8_t) override;
		void xra(const uint8_t) override;
		void cmp(const uint8_t) override;

		uint8_t dec(const uint8_t) override;
		uint8_t inc(const uint8_t) override;

	public:

		uint8_t get_m() const;

		void daa();
		void dad(const uint16_t);

		//void pushToStack(const uint16_t);
		//const uint16_t popOfStack();

		void decode_opcode(const uint8_t);

		LR35902();

	public:
		bool reset(const uint16_t = 0) override;

		const uint8_t executeOne() override;
		bool interrupt(const uint8_t) override;

		static inline constexpr bool parity(const uint8_t v) {
			uint8_t t = v;
			t ^= t >> 4;
			t ^= t >> 2;
			return ((t ^= t >> 1) & 1) ? false : true;
		}

		static Cpu* create() { return new LR35902(); }
	};
}