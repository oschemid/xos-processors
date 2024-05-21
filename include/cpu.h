#pragma once

#include "xprocessors.h"

namespace xprocessors
{
	using opcode_t = uint8_t;
	using register8_t = uint8_t;
	using register16_t = uint16_t;
	using pair16_t = union { register8_t byte[2]; register16_t word; };
}
