#pragma once

#include "states.h"
#include "cpus.h"


namespace xprocessors {
	enum class Intel8080Flags {
		SF = 0x80,
		ZF = 0x40,
		CF = 0x01
	};

	class Intel8080State : public Z80FamilyState<Intel8080Flags> {

	};

	class Intel8080 : public Z80FamilyCpu<Intel8080State>
	{
	protected:
		uint8_t interrupt_enabled; // 0 ok, 1 wait one, 2 no
		uint8_t interrupt_request;

		uint8_t auxCarryBit : 1; // auxiliary carry bit
		uint8_t parityBit : 1; // parity bit

	public:

		uint8_t get_m() const;

		uint8_t dcr(const uint8_t);
		uint8_t inr(const uint8_t);
		void xra(const uint8_t);
		void ana(const uint8_t);
		void ora(const uint8_t);
		void sub(const uint8_t, const uint8_t = 0);
		void sbb(const uint8_t);
		void add(const uint8_t, const uint8_t = 0);
		void adc(const uint8_t);
		void cmp(const uint8_t);
		void daa();
		void dad(const uint16_t);

		void unimplemented();
		void illegal();

		void pushToStack(const uint16_t);
		const uint16_t popOfStack();

		void write(const uint16_t address, const uint16_t value) {
			_handlerWrite(address, value & 0xFF);
			_handlerWrite(address + 1, value >> 8);
		}

		void decode_opcode(const uint8_t);

		Intel8080();

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

		static Cpu* create() { return new Intel8080(); }
	};
}
