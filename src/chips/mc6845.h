#pragma once
#include "xprocessors.h"
#include "interfaces.h"


namespace xprocessors
{
	class mc6845 : public interfaces::clockable
	{
	public:
		mc6845();

		void tick() override;
		void tickio(const uint8_t, const uint8_t);
		bool de() const { return _display_enable; }
		bool hsync() const { return _hsync; }
		bool vsync() const { return _vsync; }
		uint16_t get_vma() const { return (_vma & 0x07ff) | ((_registers[12] & 0x30) << 10) | (_counter9b & 0x07) << 11; }
	protected:
		uint8_t _selected_register{ 0 };
		uint8_t _registers[18]{ 0 };
		uint8_t _counter0{ 0 };
		uint8_t _counter4{ 0 };
		uint8_t _counter9{ 0 };
		uint8_t _counter9b{ 0 };

		bool _display_enable{ false };
		bool _hsync{ false };
		bool _vsync{ false };
		uint16_t _vma{ 0 };
		uint16_t _vmap{ 0 };
	};
}