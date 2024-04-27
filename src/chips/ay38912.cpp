#include "ay38912.h"


using namespace xprocessors;


void ay38912::setControl(const uint8_t pins)
{
	_control = pins;
	process();
}

void ay38912::process()
{
	switch (_control)
	{
	case 0:
		break;
	case 1:
		if ((_register_selected = 14) && (r14_fn))
			_databus = r14_fn();
		else
			_databus = _registers[_register_selected];
		break;
	case 2:
		_registers[_register_selected] = _databus;
		break;
	case 3:
		_register_selected = _databus & 0x0f;
		break;
	default:
		throw "ERROR";
	}
}