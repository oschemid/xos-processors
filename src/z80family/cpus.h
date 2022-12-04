#pragma once

#include "xprocessors.h"
#include <stdexcept>


namespace xprocessors {
	template <class S, class C> class Z80FamilyCpu : public Cpu {

		typedef C Cost;
	public:
		// Temporary access
		uint8_t c() const { return _state.c(); }
		uint8_t e() const { return _state.e(); }
		uint16_t de() const { return _state.de(); }
		uint16_t pc() const { return _state.pc(); }

	protected:
		// state
		S _state;
		bool _halted;

	protected:
		// opcode decoding
		const opcode_t readOpcode() {
			_elapsed_cycles += Cost::READ_OPCODE;
			_executed_instructions++;
			return _handlerRead(_state.pc()++);
		}
		const uint8_t readArgument8() {
			_elapsed_cycles += Cost::READ_MEMORY8;
			return _handlerRead(_state.pc()++);
		}
		const uint16_t readArgument16() {
			_elapsed_cycles += Cost::READ_MEMORY16;
			return _handlerRead(_state.pc()++) | (_handlerRead(_state.pc()++) << 8);
		}

		// common opcodes decoding
		void unimplemented(const opcode_t opcode) const {
			throw std::runtime_error("Opcode not implemented : " + opcode);
		}
		void jump(const uint16_t address, const bool condition = true) {
			if (condition) {
				_state.pc() = address;
				_elapsed_cycles += Cost::WRITE_PC;
			}
		}
		void call(const uint16_t address, const bool condition = true) {
			if (condition) {
				push(_state.pc());
				_state.pc() = address;
				_elapsed_cycles += Cost::WRITE_PC;
				_elapsed_cycles -= Cost::EXTRACALL2;
			}
			else
				_elapsed_cycles += Cost::EXTRACALL;
		}
		void ret(const bool condition = true) {
			if (condition) {
				_state.pc() = pop();
				_elapsed_cycles += Cost::WRITE_PC;
			}
		}

		virtual void add(const uint8_t) = 0;
		virtual void adc(const uint8_t) = 0;
		virtual void sub(const uint8_t) = 0;
		virtual void sbc(const uint8_t) = 0;
		virtual void ora(const uint8_t) = 0;
		virtual void xra(const uint8_t) = 0;
		virtual void ana(const uint8_t) = 0;
		virtual void cmp(const uint8_t) = 0;
		virtual void add_hl(const uint16_t) = 0;

		virtual uint8_t inc(const uint8_t) = 0;
		virtual uint8_t dec(const uint8_t) = 0;

		virtual const uint8_t decodeR(const opcode_t opcode) {
			switch (opcode & 0x07) {
			case 0x00:
				return _state.b();
			case 0x01:
				return _state.c();
			case 0x02:
				return _state.d();
			case 0x03:
				return _state.e();
			case 0x04:
				return _state.h();
			case 0x05:
				return _state.l();
			case 0x06:
				return read8(_state.hl());
			case 0x07:
				return _state.a();
			default:
				return 0;
			}
		}
		virtual void decodeR(const opcode_t opcode, const uint8_t value) {
			switch (opcode & 0x07) {
			case 0x00:
				_state.b() = value;
				return;
			case 0x01:
				_state.c() = value;
				return;
			case 0x02:
				_state.d() = value;
				return;
			case 0x03:
				_state.e() = value;
				return;
			case 0x04:
				_state.h() = value;
				return;
			case 0x05:
				_state.l() = value;
				return;
			case 0x06:
				write8(_state.hl(), value);
				return;
			case 0x07:
				_state.a() = value;
				return;
			}
			throw std::runtime_error("Unexpected opcode in decode8 " + opcode);
		}
		virtual const uint16_t decodeRR(const opcode_t opcode) const {
			switch (opcode & 0x30) {
			case 0x00:
				return _state.bc();
			case 0x10:
				return _state.de();
			case 0x20:
				return _state.hl();
			case 0x30:
				return _state.sp();
			default:
				return 0;
			}
		}
		virtual void decodeRR(const opcode_t opcode, const uint16_t value) {
			switch (opcode & 0x30) {
			case 0x00:
				_state.bc() = value;
				return;
			case 0x10:
				_state.de() = value;
				return;
			case 0x20:
				_state.hl() = value;
				return;
			case 0x30:
				_state.sp() = value;
				return;
			}
		}
		virtual const uint16_t decodePush(const opcode_t opcode) const {
			switch (opcode & 0x30) {
			case 0x00:
				return _state.bc();
			case 0x10:
				return _state.de();
			case 0x20:
				return _state.hl();
			case 0x30:
				return _state.af();
			default:
				return 0;
			}
		}
		virtual void decodePop(const opcode_t opcode, const uint16_t value) {
			switch (opcode & 0x30) {
			case 0x00:
				_state.bc() = value;
				return;
			case 0x10:
				_state.de() = value;
				return;
			case 0x20:
				_state.hl() = value;
				return;
			case 0x30:
				_state.af(value);
				return;
			}
		}

		virtual bool checkCC(const opcode_t opcode) const
		{
			switch (opcode & 0b00111000) {
			case 0b00000000:
				return !_state.zeroFlag();
			case 0b00001000:
				return _state.zeroFlag();
			case 0b00010000:
				return !_state.carryFlag();
			case 0b00011000:
				return _state.carryFlag();
			case 0b00100000:
				return !_state.parityFlag();
			case 0b00101000:
				return _state.parityFlag();
			case 0b00110000:
				return !_state.signFlag();
			case 0b00111000:
				return _state.signFlag();
			default:
				return 0;
			}
		}

	protected:
		Z80FamilyCpu() {
			reset();
		}
		virtual bool reset(const uint16_t = 0) override {
			_halted = false;
			_executed_instructions = 0;
			_state.reset();
			return true;
		}

		// Memory access
		const uint8_t read8(const uint16_t address) {
			_elapsed_cycles += Cost::READ_MEMORY8;
			return _handlerRead(address);
		}
		void write8(const uint16_t address, const uint8_t value) {
			_elapsed_cycles += Cost::WRITE_MEMORY8;
			_handlerWrite(address, value);
		}
		const uint16_t read16(const uint16_t address) {
			_elapsed_cycles += Cost::READ_MEMORY16;
			return _handlerRead(address) | (_handlerRead(address + 1) << 8);
		}
		void write16(const uint16_t address, const uint16_t value) {
			_elapsed_cycles += Cost::WRITE_MEMORY16;
			_handlerWrite(address, value & 0xff);
			_handlerWrite(address + 1, value >> 8);
		}

		// Stack
		const uint16_t pop() {
			uint16_t tmp = read16(_state.sp());
			_state.sp() += 2;
			return tmp;
		}
		void push(const uint16_t value) {
			_state.sp() -= 2;
			write16(_state.sp(), value);
			_elapsed_cycles += Cost::EXTRAPUSH;
		}
	};
}
