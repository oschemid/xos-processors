#pragma once
#include <cstdint>
#include <string>
#include <functional>
#include <memory>


namespace xprocessors
{
	using uint8_t = std::uint8_t;
	using uint16_t = std::uint16_t;
	using uint64_t = std::uint64_t;
	using string = std::string;

	using out_fn = std::function<void(const uint8_t, const uint8_t)>;
	using in_fn = std::function<const uint8_t(const uint8_t)>;
	using write_fn = std::function<void(const uint16_t, const uint8_t)>;
	using read_fn = std::function<const uint8_t(const uint16_t)>;

	using opcode_t = uint8_t;
	using register8_t = uint8_t;
	using register16_t = uint16_t;
	using pair16_t = union { register8_t byte[2]; register16_t word; };

	class Device
	{
	protected:
		out_fn _handlerOut;
		in_fn _handlerIn;
		read_fn _handlerRead;
		write_fn _handlerWrite;

	public:
		Device() :
			_handlerIn{nullptr},
			_handlerOut{nullptr},
			_handlerRead{nullptr},
			_handlerWrite{nullptr}
		{}

		// Handlers
		void in(const in_fn fn) noexcept { _handlerIn = fn; }
		void out(const out_fn fn) noexcept { _handlerOut = fn; }
		void read(const read_fn fn) noexcept { _handlerRead = fn; }
		void write(const write_fn fn) noexcept { _handlerWrite = fn; }
	};

	class Cpu : public Device {
	protected:
		uint64_t _elapsed_cycles;
		uint64_t _executed_instructions;

		Cpu() : Device{}, _elapsed_cycles{ 0 }, _executed_instructions{ 0 } {};

	public:
		using Ptr = std::unique_ptr<Cpu>;

		uint64_t elapsed_cycles() const { return _elapsed_cycles; }
		uint64_t executed_instructions() const { return _executed_instructions; }

		virtual bool reset(const uint16_t = 0) = 0;
		virtual uint8_t executeOne() = 0;
		virtual bool interrupt(const uint8_t) = 0;

		// Factory
		static Ptr create(const string&);
	};
}