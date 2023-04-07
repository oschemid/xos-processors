#pragma once
#include "xprocessors.h"


namespace xprocessors
{
	class MB14241
	{
	public:
		using Ptr = std::unique_ptr<MB14241>;

		static Ptr create() { return std::make_unique<MB14241>(MB14241()); }

		void writeOffset(const uint8_t o) {
			_shift = ~o & 0x07;
		}
		void writeValue(const uint8_t v) {
			_data = (_data >> 8) | (static_cast<uint16_t>(v) << 7);
		}
		const uint8_t readValue() { return static_cast<uint8_t>((_data >> _shift) & 0x00ff); }

	protected:
		MB14241() {}

		uint16_t _data{ 0 };
		uint8_t _shift{ 0 };
	};
}