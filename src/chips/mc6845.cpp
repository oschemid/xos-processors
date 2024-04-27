#include "mc6845.h"


using namespace xprocessors;


mc6845::mc6845()
{
}

#include <iostream>

void mc6845::tick()
{
	_counter0++;
	_vma += 2;

	if (_counter0 == _registers[0])
	{
		_counter0 = 0;

		_hsync = false;

		if (_counter4 < _registers[6])
			_display_enable = true;

		if (_counter4 == 0)
			_vsync = false;

		if ((_counter9 == 0) && (_counter4 == 0)) {
			_vmap = (_registers[12] & 0x30) << 10 | ((_registers[12] & 0x03) << 9) | (_registers[13] << 1);
		}
		_vma = _vmap;
		_counter9b = _counter9;
		if (_counter9 == _registers[9])
		{
			_counter9 = 0;
			if (_counter4 == _registers[7])
				_vsync = true;
			if (_counter4 == _registers[4])
			{
				if (_registers[5] == 0)
				{
					_counter4 = 0;
				}
				else
				{
					_counter4++;
					_counter4 = 0; // TMP
				}
			}
			else
			{
				_counter4++;
			}
		}
		else
		{
			_counter9++;
		}
	}
	else if (_counter0 == _registers[1])
	{
		_display_enable = false;
		if (_counter9b == _registers[9]) {
			_vmap = _vma;
		}
	}
	else if (_counter0 == _registers[2])
	{
		_hsync = true;
	}
}

void mc6845::tickio(const uint8_t addr, const uint8_t data)
{
	switch (addr & 0x03) {
	case 0:
		_selected_register = data & 0x1f;
		break;
	case 1:
		_registers[_selected_register] = data;
		break;
	}
}
