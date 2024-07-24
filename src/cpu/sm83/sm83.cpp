#include "sm83.h"


using namespace xprocessors::cpu;

void sm83::init()
{
	reset();
}

void sm83::reset(const uint16_t start)
{
	pc = start;
	a = 1;
	f = 0xb0;
	b = 0;
	c = 0x13;
	d = 0;
	e = 0xd8;
	h = 1;
	l = 0x4d;
	wz = 0; 
	sp = 0xfffe;
	ir = 0;
	_tcycle = 0;
	_ie = _if = 0;
	addressbus = 0;
	databus = 0;
	pins = 0;
}

void sm83::interrupt_step1()
{
	uint8_t interrupts = _if & _ie;
	if (interrupts)
	{
		_halted = false;
		_interrupt_in_process = 1;
		while (interrupts)
		{
			if (interrupts & 1)
			{
				interrupts = 0;
			}
			else
			{
				interrupts >>= 1;
				_interrupt_in_process<<=1;
			}
		}
		_if &= ~_interrupt_in_process;
		addressbus = 0xff0f;
		databus = _if;
		pins |= PIN_WR;
		_ime++;
	}
}
void sm83::tick()
{
	if ((_tstep == 0) && (_tcycle == 0))
	{
		if (_ime)
		{
			switch (_ime)
			{
			case 1:
				// Check
				interrupt_step1();
				break;
			case 2:
				// Nop
				pins &= ~PIN_WR;
				_ime++;
				pc--;
				if (ir > 255)
					pc--;
				break;
			case 3:
				sp--;
				addressbus = sp;
				databus = pch;
				pins |= PIN_WR;
				_ime++;
				break;
			case 4:
				sp--;
				addressbus = sp;
				databus = pcl;
				pins |= PIN_WR;
				_ime++;
				break;
			case 5:
				pins &= ~PIN_WR;
				ir = 0;
				switch (_interrupt_in_process)
				{
				case 1:
					pc = 0x40;
					break;
				case 2:
					pc = 0x48;
					break;
				case 4:
					pc = 0x50;
					break;
				case 8:
					pc = 0x58;
					break;
				case 16:
					pc = 0x60;
					break;
				default:
					throw "Erreur";
				}
				_ime = 0;
				break;
			}
			if (_ime != 1)
				return;
		}
		else
		{
			if ((_halted) && (_if & _ie))
				_halted = false;
		}
	}
	if (_tcycle == 0)
	{
		if (_halted)
			return;
		_current_step = opcodes_steps[ir][_tstep];
	}
	process(_current_step.addressbus);
	process(_current_step.databus);
	process(_current_step.idu);
	process(_current_step.alu);
	process(_current_step.misc);
	_tcycle++;
	if (_tcycle == 4)
	{
		_tcycle = 0;
		if (_skip)
		{
			_skip = false;
			_tstep = opcodes_steps[ir].size() - 1;
		}
		else
		{
			_tstep++;
		}
	}
}

// 0 - SET AB
void sm83::process(const addressbus_operation operation)
{
	switch (operation)
	{
	case addressbus_operation::PUSH_PC:
		if (_tcycle == 0)
			addressbus = pc;
		break;
	case addressbus_operation::PUSH_BC:
		if (_tcycle == 0)
			addressbus = bc;
		break;
	case addressbus_operation::PUSH_DE:
		if (_tcycle == 0)
			addressbus = de;
		break;
	case addressbus_operation::PUSH_HL:
		if (_tcycle == 0)
			addressbus = hl;
		break;
	case addressbus_operation::PUSH_SP:
		if (_tcycle == 0)
			addressbus = sp;
		break;
	case addressbus_operation::PUSH_WZ:
		if (_tcycle == 0)
			addressbus = wz;
		break;
	case addressbus_operation::PUSH_Z:
		if (_tcycle == 0)
			addressbus = 0xff00 | z;
		break;
	case addressbus_operation::PUSH_C:
		if (_tcycle == 0)
			addressbus = 0xff00 | c;
		break;
	}
}

// 0 - SET RD (DB if WR)
// 1 - READ DB
void sm83::process(const databus_operation operation)
{
	switch (operation)
	{
	case databus_operation::NONE:
		break;
	case databus_operation::POP_IR:
		if (_tcycle == 0)
		{
			pins |= PIN_RD;
			if (ir != 0xcb)
				pins |= PIN_M1;
		}
		if (_tcycle == 1)
		{
			ir = databus;
			pins &= ~PIN_RD;
		}
		if (_tcycle == 3)
		{
			_tstep = -1;
			pins &= ~PIN_M1;
		}
		break;
	case databus_operation::POP_Z:
		if (_tcycle == 0)
		{
			pins |= (PIN_RD);
		}
		if (_tcycle == 1)
		{
			z = databus;
			pins &= ~PIN_RD;
		}
		break;
	case databus_operation::POP_W:
		if (_tcycle == 0)
		{
			pins |= (PIN_RD);
		}
		if (_tcycle == 1)
		{
			w = databus;
			pins &= ~PIN_RD;
		}
		break;
	case databus_operation::PUSH_A:
		if (_tcycle == 0)
		{
			databus = a;
			pins |= (PIN_WR);
		}
		if (_tcycle == 1)
		{
			pins &= ~PIN_WR;
		}
		break;
	case databus_operation::PUSH_Z:
		if (_tcycle == 0)
		{
			databus = z;
			pins |= (PIN_WR);
		}
		if (_tcycle == 1)
		{
			pins &= ~PIN_WR;
		}
		break;
	case databus_operation::PUSH_F:
		if (_tcycle == 0)
		{
			databus = f;
			pins |= (PIN_WR);
		}
		if (_tcycle == 1)
		{
			pins &= ~PIN_WR;
		}
		break;
	case databus_operation::PUSH_B:
		if (_tcycle == 0)
		{
			databus = b;
			pins |= (PIN_WR);
		}
		if (_tcycle == 1)
		{
			pins &= ~PIN_WR;
		}
		break;
	case databus_operation::PUSH_C:
		if (_tcycle == 0)
		{
			databus = c;
			pins |= (PIN_WR);
		}
		if (_tcycle == 1)
		{
			pins &= ~PIN_WR;
		}
		break;
	case databus_operation::PUSH_D:
		if (_tcycle == 0)
		{
			databus = d;
			pins |= (PIN_WR);
		}
		if (_tcycle == 1)
		{
			pins &= ~PIN_WR;
		}
		break;
	case databus_operation::PUSH_E:
		if (_tcycle == 0)
		{
			databus = e;
			pins |= (PIN_WR);
		}
		if (_tcycle == 1)
		{
			pins &= ~PIN_WR;
		}
		break;
	case databus_operation::PUSH_H:
		if (_tcycle == 0)
		{
			databus = h;
			pins |= (PIN_WR);
		}
		if (_tcycle == 1)
		{
			pins &= ~PIN_WR;
		}
		break;
	case databus_operation::PUSH_L:
		if (_tcycle == 0)
		{
			databus = l;
			pins |= (PIN_WR);
		}
		if (_tcycle == 1)
		{
			pins &= ~PIN_WR;
		}
		break;
	case databus_operation::PUSH_SPL:
		if (_tcycle == 0)
		{
			databus = spl;
			pins |= (PIN_WR);
		}
		if (_tcycle == 1)
		{
			pins &= ~PIN_WR;
		}
		break;
	case databus_operation::PUSH_SPH:
		if (_tcycle == 0)
		{
			databus = sph;
			pins |= (PIN_WR);
		}
		if (_tcycle == 1)
		{
			pins &= ~PIN_WR;
		}
		break;
	case databus_operation::PUSH_PCH:
		if (_tcycle == 0)
		{
			databus = pch;
			pins |= (PIN_WR);
		}
		if (_tcycle == 1)
		{
			pins &= ~PIN_WR;
		}
		break;
	case databus_operation::PUSH_PCL:
		if (_tcycle == 0)
		{
			databus = pcl;
			pins |= (PIN_WR);
		}
		if (_tcycle == 1)
		{
			pins &= ~PIN_WR;
		}
		break;
	default:
		throw "unimplemented";
	}
	if (pins & PIN_WR)
	{
		switch (addressbus)
		{
		case 0xff0f:
			_if = databus;
			break;
		case 0xffff:
			_ie = databus;
			break;
		}
	}
}

// 3 - operation
void sm83::process(const idu_operation operation)
{
	if (_tcycle != 3)
		return;
	switch (operation)
	{
	case idu_operation::NONE:
		break;
	case idu_operation::INC_PC:
		pc++;
		break;
	case idu_operation::INC_BC:
		bc++;
		break;
	case idu_operation::INC_DE:
		de++;
		break;
	case idu_operation::INC_HL:
		hl++;
		break;
	case idu_operation::INC_SP:
		sp++;
		break;
	case idu_operation::INC_WZ:
		wz++;
		break;
	case idu_operation::DEC_BC:
		bc--;
		break;
	case idu_operation::DEC_DE:
		de--;
		break;
	case idu_operation::DEC_HL:
		hl--;
		break;
	case idu_operation::DEC_SP:
		sp--;
		break;
	case idu_operation::ADD_Z_TO_PC:
		pc += static_cast<int8_t>(z);
		break;
	case idu_operation::ADD_Z_TO_SP:
		wz = add_sp(z);
		break;
	case idu_operation::HL_1_TO_PC:
		pc = hl + 1;
		break;
	case idu_operation::HL_TO_SP:
		sp = hl;
		break;
	default:
		throw "unimplemented";
	}
}

void sm83::process(const misc_operation operation)
{
	if (_tcycle == 3)
	{
		switch (operation)
		{
		case misc_operation::DI:
			_ime = 0;
			break;
		case misc_operation::EI:
			_ime = 1;
			break;
		case misc_operation::HALT:
			_halted = true;
			break;
		case misc_operation::STOP:
			_halted = true;
			_ime = 1;
			break;
		case misc_operation::CB:
			ir += 256;
			break;
		case misc_operation::WZ_TO_AF:
			af = wz & 0xfff0;
			break;
		case misc_operation::WZ_TO_BC:
			bc = wz;
			break;
		case misc_operation::WZ_TO_DE:
			de = wz;
			break;
		case misc_operation::WZ_TO_HL:
			hl = wz;
			break;
		case misc_operation::WZ_TO_SP:
			sp = wz;
			break;
		case misc_operation::WZ_TO_PC:
			pc = wz;
			break;
		case misc_operation::RST0_TO_PC:
			pc = 0;
			break;
		case misc_operation::RST8_TO_PC:
			pc = 8;
			break;
		case misc_operation::RST10_TO_PC:
			pc = 0x10;
			break;
		case misc_operation::RST18_TO_PC:
			pc = 0x18;
			break;
		case misc_operation::RST20_TO_PC:
			pc = 0x20;
			break;
		case misc_operation::RST28_TO_PC:
			pc = 0x28;
			break;
		case misc_operation::RST30_TO_PC:
			pc = 0x30;
			break;
		case misc_operation::RST38_TO_PC:
			pc = 0x38;
			break;
		case misc_operation::CHECK_Z:
			if (!(f & flags::ZF))
				_skip = true;
			break;
		case misc_operation::CHECK_NZ:
			if (f & flags::ZF)
				_skip = true;
			break;
		case misc_operation::CHECK_C:
			if (!(f & flags::CF))
				_skip = true;
			break;
		case misc_operation::CHECK_NC:
			if (f & flags::CF)
				_skip = true;
			break;
		}
	}
}

uint16_t sm83::add_sp(const uint8_t value)
{
	const uint32_t res = sp + static_cast<signed char>(value);
	f &= ~(flags::NF | flags::ZF | flags::HF | flags::CF);
	if ((sp & 0xf) + (value & 0xf) > 0xf)
		f |= flags::HF;
	if ((sp & 0xff) + value > 0xff)
		f |= flags::CF;
	return res & 0xffff;
}
