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
}