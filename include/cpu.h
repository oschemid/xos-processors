#pragma once
#include "types.h"


namespace xprocessors {
	class Cpu {
	protected:
		uint64_t _elapsed_cycles;
		uint64_t _executed_instructions;

		out_fn _handlerOut;
		in_fn _handlerIn;
		read_fn _handlerRead;
		write_fn _handlerWrite;

		Cpu() : _elapsed_cycles(0), _executed_instructions(0) {};

	public:
		const uint64_t elapsed_cycles() const { return _elapsed_cycles; }
		const uint64_t executed_instructions() const { return _executed_instructions; }

		virtual bool reset(const uint16_t = 0) = 0;
		virtual const uint8_t executeOne() = 0;
		virtual bool interrupt(const uint8_t) = 0;

		// Handlers
		bool in(const in_fn fn) noexcept { _handlerIn = fn; return true; }
		bool out(const out_fn fn) noexcept { _handlerOut = fn; return true; }
		bool read(const read_fn fn) noexcept { _handlerRead = fn; return true; }
		bool write(const write_fn fn) noexcept { _handlerWrite = fn; return true; }

		// Factory
		static Cpu* create(const string&);
	};
}