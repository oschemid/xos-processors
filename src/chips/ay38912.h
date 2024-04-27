#pragma once
#include "xprocessors.h"


namespace xprocessors
{
	class ay38912
	{
	public:
		ay38912() {}

		uint8_t getDatabus() const { return _databus; }
		void setDatabus(const uint8_t value) { _databus = value; process(); }
		void setControl(const uint8_t);

		void handler(std::function<uint8_t()> fn) { r14_fn = fn; }
	protected:
		uint8_t _databus{ 0 };
		uint8_t _registers[14]{ 0 };
		uint8_t _register_selected{ 0 };
		uint8_t _control{ 0 };

		std::function<uint8_t()> r14_fn{ nullptr };
		void process();
	};
}
