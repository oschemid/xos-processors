#pragma once

#include "states.h"
#include "cpus.h"
#include <memory>


namespace xprocessors {
	struct Intel8080Flags {
		static const uint8_t MASK = 0x02;
		static const uint8_t SF = 0x80;
		static const uint8_t ZF = 0x40;
		static const uint8_t CF = 0x01;
		static const uint8_t PF = 0x04;
		static const uint8_t HF = 0x10;
		static const uint8_t ALL = SF | ZF | CF | PF | HF;
	};
	struct Intel8080Costs {
		static const uint8_t READ_OPCODE = 4;
		static const uint8_t READ_MEMORY8 = 3;
		static const uint8_t READ_MEMORY16 = 6;
		static const uint8_t WRITE_MEMORY8 = 3;
		static const uint8_t WRITE_MEMORY16 = 6;
		static const uint8_t READ_WRITE_R = 1;
		static const uint8_t READ_WRITE_RR = 1;
		static const uint8_t WRITE_PC = 0;
		static const uint8_t EXTRAPUSH = 1;
		static const uint8_t EXTRACALL = 1;
		static const uint8_t EXTRACALL2 = 0;
		static const uint8_t EXTRARET = 1;
		static const uint8_t EXTRAHALT = 3;
	};

	class Intel8080State : public Z80FamilyState<Intel8080Flags> {

	};

	class Intel8080 : public Z80FamilyCpu<Intel8080State, Intel8080Costs>
	{
	protected:
		uint8_t interrupt_enabled; // 0 ok, 1 wait one, 2 no
		uint8_t interrupt_request;

		void add(const uint8_t) override;
		void adc(const uint8_t) override;
		void sub(const uint8_t) override;
		void sbc(const uint8_t) override;
		void ora(const uint8_t) override;
		void xra(const uint8_t) override;
		void ana(const uint8_t) override;
		void cmp(const uint8_t) override;
		void add_hl(const uint16_t) override;

		uint8_t dec(const uint8_t) override;
		uint8_t inc(const uint8_t) override;

	public:

		uint8_t get_m() const;

		void daa();

		void decode_opcode(const uint8_t);

		Intel8080();

	public:
		bool reset(const uint16_t = 0) override;

		uint8_t executeOne() override;
		bool interrupt(const uint8_t) override;

		static inline constexpr bool parity(const uint8_t v) {
			uint8_t t = v;
			t ^= t >> 4;
			t ^= t >> 2;
			return ((t ^= t >> 1) & 1) ? false : true;
		}

		static std::unique_ptr<Cpu> create() { return std::make_unique<Intel8080>(Intel8080()); }
	};
}
