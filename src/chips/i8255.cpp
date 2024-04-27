#include "i8255.h"


using namespace xprocessors;


i8255::i8255()
{

}

void i8255::write(const uint8_t control, const uint8_t data)
{
	switch (control)
	{
	case 0:
		if (_porta_is_out) {
			_porta = data;
			if (_porta_out)
				_porta_out(data);
		}
		break;
	case 1:
		if (_portb_is_out) {
			_portb = data;
			if (_portb_out)
				_portb_out(data);
		}
		break;
	case 2:
		_portc = data;
		if (_portc_out)
			_portc_out(data);
		break;
	case 3:
		if (data & 0x80)
		{
			_porta_is_out = (data & 0x10) ? false : true;
			_portb_is_out = (data & 0x02) ? false : true;
		}
		break;
	}
}

uint8_t i8255::read(const uint8_t control)
{
	switch (control)
	{
	case 0:
		if (_porta_in)
			_porta = _porta_in();
		return _porta;
	case 1:
		if (_portb_in)
			_portb = _portb_in();
		return _portb;
	case 2:
		if (_portc_in)
			_portc = _portc_in();
		return _portc;
	default:
		return 0;
	}
}