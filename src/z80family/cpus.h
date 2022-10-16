#pragma once

#include "cpu.h"
#include <stdexcept>


namespace xprocessors {
	template <class S> class Z80FamilyCpu : public Cpu {
	public:
		// Temporary access
		uint8_t c() const { return _state.c(); }
		uint8_t e() const { return _state.e(); }
		uint16_t de() const { return _state.de(); }

	protected:
		// state
		S _state;

	protected:
		// opcode decoding
		const opcode_t readOpcode() {
			_elapsed_cycles += 4;
			return _handlerRead(_state.pc()++);
		}
		const uint8_t readArgument8() {
			_elapsed_cycles += 4;
			return _handlerRead(_state.pc()++);
		}
		const uint16_t readArgument16() {
			_elapsed_cycles += 6;
			return _handlerRead(_state.pc()++) | (_handlerRead(_state.pc()++) << 8);
		}

		const uint8_t decodeR(const opcode_t opcode) {
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
			}
			throw std::runtime_error("Unexpected opcode in decode8 " + opcode);
		}
		void decodeR(const opcode_t opcode, const uint8_t value) {
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

	protected:
		// Memory access
		const uint8_t read8(const uint16_t address) {
			_elapsed_cycles += 4;
			return _handlerRead(address);
		}
		void write8(const uint16_t address, const uint8_t value) {
			_elapsed_cycles += 3;
			_handlerWrite(address, value);
		}
	};
}
