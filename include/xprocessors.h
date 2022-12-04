#pragma once
#include <cstdint>
#include <string>
#include <functional>


namespace xprocessors {
	using std::uint8_t;
	using std::uint16_t;
	using std::uint64_t;
	using std::string;

	typedef std::function<void(const uint8_t, const uint8_t)> out_fn;
	typedef std::function<const uint8_t(const uint8_t)> in_fn;
	typedef std::function<void(const uint16_t, const uint8_t)> write_fn;
	typedef std::function<const uint8_t(const uint16_t)> read_fn;

	typedef uint8_t opcode_t;
	typedef uint8_t register8_t;
	typedef uint16_t register16_t;
	typedef union {
		register8_t byte[2];
		register16_t word;
	} pair16_t;

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
		uint64_t elapsed_cycles() const { return _elapsed_cycles; }
		uint64_t executed_instructions() const { return _executed_instructions; }

		virtual bool reset(const uint16_t = 0) = 0;
		virtual uint8_t executeOne() = 0;
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