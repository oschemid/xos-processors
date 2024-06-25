#include "i8080.h"


using namespace xprocessors::cpu;


static inline constexpr bool parity(const uint8_t v)
{
	uint8_t t = v;
	t ^= t >> 4;
	t ^= t >> 2;
	return ((t ^= t >> 1) & 1) ? false : true;
}

void i8080::init()
{
	reset();
}

void i8080::reset(const uint16_t start)
{
	pc = start;
	af = bc = de = hl = 0;
	f = 0x02;
	_opcode = 0;
	_addressbus = 0;
	_databus = 0;
	_pins = 0;
	_current_step = FETCH;
	_current_steps_idx = -1;
	_halted = false;
	_intreq = false;
	_intgo = false;
	_haltint = false;
}

void i8080::tick()
{
	if (_pins & PIN_INT and _pins & PIN_INTE)
	{
		_intreq = true;
	}
	if (_intreq and _current_step == FETCH)
	{
		_intgo = true;
		_intreq = false;
		if (_halted)
		{
			_haltint = true;
			_halted = false;
		}
	}

	if (_halted)
		return;

	switch (_current_step) {
	case HALT1:
		_addressbus = pc;
		_databus = STATUS_HLTA | STATUS_MEMR | STATUS_WO;
		_pins |= PIN_SYNC;
		break;
	case HALT2:
		_halted = true;
		_pins &= ~PIN_SYNC;
		break;
	case FETCH:
		fetch();
		break;
	case FETCHREAD:
		fetchread();
		break;
	case READ:
		read();
		break;
	case READIO:
		readio();
		break;
	case READSP:
		readsp();
		break;
	case READINC:
		wz++;
		read();
		break;
	case READPC:
		readpc();
		break;
	case WAIT:
		wait();
		break;
	case WRITE:
		write();
		break;
	case WRITEIO:
		writeio();
		break;
	case WRITESP:
		writesp();
		break;
	case WRITEINC:
		_pins &= ~PIN_WR;
		wz++;
		break;
	case STOPWRITE:
		_pins &= ~PIN_WR;
		break;
	case NOP:
		break;
	case EI:
		setINTE(true);
		break;
	case DI:
		setINTE(false);
		break;
	case LD_SP_INTO_WZ:
		wz = sp;
		break;
	case LD_HL_INTO_WZ:
		wz = hl;
		break;
	case LD_BC_INTO_WZ:
		wz = bc;
		break;
	case LD_DE_INTO_WZ:
		wz = de;
		break;
	case LD_DB_INTO_W:
		_pins &= ~PIN_DBIN;
		w = _databus;
		break;
	case LD_DB_INTO_SAV:
		_pins &= ~PIN_DBIN;
		_dbsave = _databus;
		break;
	case LD_DB_INTO_Z:
		_pins &= ~PIN_DBIN;
		z = _databus;
		break;
	case LD_DB_INTO_SPL:
		_pins &= ~PIN_DBIN;
		spl = _databus;
		break;
	case LD_WZ_INTO_SP:
		sp = wz;
		break;
	case LD_WZ_INTO_HL:
		hl = wz;
		break;
	case LD_HL_INTO_PC:
		pc = hl;
		break;
	case LD_DB_INTO_SPH:
		_pins &= ~PIN_DBIN;
		sph = _databus;
		break;
	case LD_DB_INTO_PCH:
		_pins &= ~PIN_DBIN;
		pch = _databus;
		break;
	case LD_DB_INTO_PCL:
		_pins &= ~PIN_DBIN;
		pcl = _databus;
		break;
	case LD_DB_INTO_B:
		_pins &= ~PIN_DBIN;
		b = _databus;
		break;
	case LD_C_INTO_B:
		b = c;
		break;
	case LD_D_INTO_B:
		b = d;
		break;
	case LD_E_INTO_B:
		b = e;
		break;
	case LD_H_INTO_B:
		b = h;
		break;
	case LD_L_INTO_B:
		b = l;
		break;
	case LD_A_INTO_B:
		b = a;
		break;
	case LD_SAV_INTO_DB:
		writedb(_dbsave);
		break;
	case LD_A_INTO_DB:
		writedb(a);
		break;
	case LD_F_INTO_DB:
		writedb(f);
		break;
	case LD_B_INTO_DB:
		writedb(b);
		break;
	case LD_C_INTO_DB:
		writedb(c);
		break;
	case LD_D_INTO_DB:
		writedb(d);
		break;
	case LD_E_INTO_DB:
		writedb(e);
		break;
	case LD_H_INTO_DB:
		writedb(h);
		break;
	case LD_L_INTO_DB:
		writedb(l);
		break;
	case LD_W_INTO_DB:
		writedb(w);
		break;
	case LD_PCL_INTO_DB:
		writedb(pcl);
		break;
	case LD_PCH_INTO_DB:
		writedb(pch);
		break;
	case LD_DB_INTO_C:
		_pins &= ~PIN_DBIN;
		c = _databus;
		break;
	case LD_B_INTO_C:
		c = b;
		break;
	case LD_D_INTO_C:
		c = d;
		break;
	case LD_E_INTO_C:
		c = e;
		break;
	case LD_H_INTO_C:
		c = h;
		break;
	case LD_L_INTO_C:
		c = l;
		break;
	case LD_A_INTO_C:
		c = a;
		break;
	case LD_DB_INTO_D:
		_pins &= ~PIN_DBIN;
		d = _databus;
		break;
	case LD_B_INTO_D:
		d = b;
		break;
	case LD_C_INTO_D:
		d = c;
		break;
	case LD_E_INTO_D:
		d = e;
		break;
	case LD_H_INTO_D:
		d = h;
		break;
	case LD_L_INTO_D:
		d = l;
		break;
	case LD_A_INTO_D:
		d = a;
		break;
	case LD_DB_INTO_E:
		_pins &= ~PIN_DBIN;
		e = _databus;
		break;
	case LD_B_INTO_E:
		e = b;
		break;
	case LD_C_INTO_E:
		e = c;
		break;
	case LD_D_INTO_E:
		e = d;
		break;
	case LD_H_INTO_E:
		e = h;
		break;
	case LD_L_INTO_E:
		e = l;
		break;
	case LD_A_INTO_E:
		e = a;
		break;
	case LD_DB_INTO_L:
		_pins &= ~PIN_DBIN;
		l = _databus;
		break;
	case LD_B_INTO_L:
		l = b;
		break;
	case LD_C_INTO_L:
		l = c;
		break;
	case LD_D_INTO_L:
		l = d;
		break;
	case LD_E_INTO_L:
		l = e;
		break;
	case LD_H_INTO_L:
		l = h;
		break;
	case LD_A_INTO_L:
		l = a;
		break;
	case LD_DB_INTO_H:
		_pins &= ~PIN_DBIN;
		h = _databus;
		break;
	case LD_B_INTO_H:
		h = b;
		break;
	case LD_C_INTO_H:
		h = c;
		break;
	case LD_D_INTO_H:
		h = d;
		break;
	case LD_E_INTO_H:
		h = e;
		break;
	case LD_L_INTO_H:
		h = l;
		break;
	case LD_A_INTO_H:
		h = a;
		break;
	case LD_DB_INTO_A:
		_pins &= ~PIN_DBIN;
		a = _databus;
		break;
	case LD_DB_INTO_F:
		_pins &= ~PIN_DBIN;
		f = (_databus | 2) & 0xd7;
		break;
	case LD_B_INTO_A:
		a = b;
		break;
	case LD_C_INTO_A:
		a = c;
		break;
	case LD_D_INTO_A:
		a = d;
		break;
	case LD_E_INTO_A:
		a = e;
		break;
	case LD_L_INTO_A:
		a = l;
		break;
	case LD_H_INTO_A:
		a = h;
		break;
	case EX_DE_HL:
		wz = de;
		de = hl;
		hl = wz;
		break;
	case ADD_B_TO_A:
		add(b);
		break;
	case ADD_C_TO_A:
		add(c);
		break;
	case ADD_D_TO_A:
		add(d);
		break;
	case ADD_E_TO_A:
		add(e);
		break;
	case ADD_H_TO_A:
		add(h);
		break;
	case ADD_L_TO_A:
		add(l);
		break;
	case ADD_A_TO_A:
		add(a);
		break;
	case ADD_DB_TO_A:
		_pins &= ~PIN_DBIN;
		add(_databus);
		break;
	case ADC_B_TO_A:
		adc(b);
		break;
	case ADC_C_TO_A:
		adc(c);
		break;
	case ADC_D_TO_A:
		adc(d);
		break;
	case ADC_E_TO_A:
		adc(e);
		break;
	case ADC_H_TO_A:
		adc(h);
		break;
	case ADC_L_TO_A:
		adc(l);
		break;
	case ADC_A_TO_A:
		adc(a);
		break;
	case ADC_DB_TO_A:
		_pins &= ~PIN_DBIN;
		adc(_databus);
		break;
	case SUB_B_TO_A:
		sub(b);
		break;
	case SUB_C_TO_A:
		sub(c);
		break;
	case SUB_D_TO_A:
		sub(d);
		break;
	case SUB_E_TO_A:
		sub(e);
		break;
	case SUB_H_TO_A:
		sub(h);
		break;
	case SUB_L_TO_A:
		sub(l);
		break;
	case SUB_A_TO_A:
		sub(a);
		break;
	case SUB_DB_TO_A:
		_pins &= ~PIN_DBIN;
		sub(_databus);
		break;
	case SBB_B_TO_A:
		sbb(b);
		break;
	case SBB_C_TO_A:
		sbb(c);
		break;
	case SBB_D_TO_A:
		sbb(d);
		break;
	case SBB_E_TO_A:
		sbb(e);
		break;
	case SBB_H_TO_A:
		sbb(h);
		break;
	case SBB_L_TO_A:
		sbb(l);
		break;
	case SBB_A_TO_A:
		sbb(a);
		break;
	case SBB_DB_TO_A:
		_pins &= ~PIN_DBIN;
		sbb(_databus);
		break;
	case AND_A:
		ana(a);
		break;
	case AND_B:
		ana(b);
		break;
	case AND_C:
		ana(c);
		break;
	case AND_D:
		ana(d);
		break;
	case AND_E:
		ana(e);
		break;
	case AND_H:
		ana(h);
		break;
	case AND_L:
		ana(l);
		break;
	case AND_DB:
		_pins &= ~PIN_DBIN;
		ana(_databus);
		break;
	case OR_A:
		ora(a);
		break;
	case OR_B:
		ora(b);
		break;
	case OR_C:
		ora(c);
		break;
	case OR_D:
		ora(d);
		break;
	case OR_E:
		ora(e);
		break;
	case OR_H:
		ora(h);
		break;
	case OR_L:
		ora(l);
		break;
	case OR_DB:
		_pins &= ~PIN_DBIN;
		ora(_databus);
		break;
	case XOR_A:
		xra(a);
		break;
	case XOR_B:
		xra(b);
		break;
	case XOR_C:
		xra(c);
		break;
	case XOR_D:
		xra(d);
		break;
	case XOR_E:
		xra(e);
		break;
	case XOR_H:
		xra(h);
		break;
	case XOR_L:
		xra(l);
		break;
	case XOR_DB:
		_pins &= ~PIN_DBIN;
		xra(_databus);
		break;
	case CP_A:
		cp(a);
		break;
	case CP_B:
		cp(b);
		break;
	case CP_C:
		cp(c);
		break;
	case CP_D:
		cp(d);
		break;
	case CP_E:
		cp(e);
		break;
	case CP_H:
		cp(h);
		break;
	case CP_L:
		cp(l);
		break;
	case CP_DB:
		_pins &= ~PIN_DBIN;
		cp(_databus);
		break;
	case INC_A:
		a = inc(a);
		break;
	case INC_B:
		b = inc(b);
		break;
	case INC_C:
		c = inc(c);
		break;
	case INC_D:
		d = inc(d);
		break;
	case INC_E:
		e = inc(e);
		break;
	case INC_H:
		h = inc(h);
		break;
	case INC_L:
		l = inc(l);
		break;
	case INC_DB:
		_pins &= ~PIN_DBIN;
		_dbsave = inc(_databus);
		break;
	case DEC_A:
		a = dec(a);
		break;
	case DEC_B:
		b = dec(b);
		break;
	case DEC_C:
		c = dec(c);
		break;
	case DEC_D:
		d = dec(d);
		break;
	case DEC_E:
		e = dec(e);
		break;
	case DEC_H:
		h = dec(h);
		break;
	case DEC_L:
		l = dec(l);
		break;
	case DEC_DB:
		_pins &= ~PIN_DBIN;
		_dbsave = dec(_databus);
		break;
	case RLC:
		rlc();
		break;
	case RRC:
		rrc();
		break;
	case RAL:
		ral();
		break;
	case RAR:
		rar();
		break;
	case DAA:
		daa();
		break;
	case CMA:
		a = ~a;
		break;
	case STC:
		f |= flags::CF;
		break;
	case CMC:
		if (f & flags::CF)
			f &= ~flags::CF;
		else
			f |= flags::CF;
		break;
	case INC_BC:
		bc++;
		break;
	case INC_DE:
		de++;
		break;
	case INC_HL:
		hl++;
		break;
	case INC_SP:
		sp++;
		break;
	case DEC_BC:
		bc--;
		break;
	case DEC_DE:
		de--;
		break;
	case DEC_HL:
		hl--;
		break;
	case DEC_SP:
		sp--;
		break;
	case STOPZ:
		if (f & flags::ZF)
			next_fetch();
		break;
	case STOPNZ:
		if (!(f & flags::ZF))
			next_fetch();
		break;
	case STOPC:
		if (f & flags::CF)
			next_fetch();
		break;
	case STOPNC:
		if (!(f & flags::CF))
			next_fetch();
		break;
	case STOPPO:
		if (!(f & flags::PF))
			next_fetch();
		break;
	case STOPPE:
		if (f & flags::PF)
			next_fetch();
		break;
	case STOPP:
		if (!(f & flags::SF))
			next_fetch();
		break;
	case STOPM:
		if (f & flags::SF)
			next_fetch();
		break;

	case JP:
		jp();
		break;
	case JP_WZ:
		_pins &= ~(PIN_DBIN | PIN_WR);
		pc = wz;
		break;
	case JP_0:
		_pins &= ~(PIN_DBIN | PIN_WR);
		pc = 0;
		break;
	case JP_8:
		_pins &= ~(PIN_DBIN | PIN_WR);
		pc = 8;
		break;
	case JP_10:
		_pins &= ~(PIN_DBIN | PIN_WR);
		pc = 0x10;
		break;
	case JP_18:
		_pins &= ~(PIN_DBIN | PIN_WR);
		pc = 0x18;
		break;
	case JP_20:
		_pins &= ~(PIN_DBIN | PIN_WR);
		pc = 0x20;
		break;
	case JP_28:
		_pins &= ~(PIN_DBIN | PIN_WR);
		pc = 0x28;
		break;
	case JP_30:
		_pins &= ~(PIN_DBIN | PIN_WR);
		pc = 0x30;
		break;
	case JP_38:
		_pins &= ~(PIN_DBIN | PIN_WR);
		pc = 0x38;
		break;
	case JPNZ:
		_pins &= ~(PIN_DBIN | PIN_WR);
		if (!(f & flags::ZF))
			jp();
		break;
	case JPZ:
		_pins &= ~(PIN_DBIN | PIN_WR);
		if (f & flags::ZF)
			jp();
		break;
	case JPNC:
		_pins &= ~(PIN_DBIN | PIN_WR);
		if (!(f & flags::CF))
			jp();
		break;
	case JPC:
		_pins &= ~(PIN_DBIN | PIN_WR);
		if (f & flags::CF)
			jp();
		break;
	case JPPO:
		_pins &= ~(PIN_DBIN | PIN_WR);
		if (!(f & flags::PF))
			jp();
		break;
	case JPPE:
		_pins &= ~(PIN_DBIN | PIN_WR);
		if (f & flags::PF)
			jp();
		break;
	case JPP:
		_pins &= ~(PIN_DBIN | PIN_WR);
		if (!(f & flags::SF))
			jp();
		break;
	case JPM:
		_pins &= ~(PIN_DBIN | PIN_WR);
		if (f & flags::SF)
			jp();
		break;
	case DECODE:
		decode();
		break;
	case ADD_WZ_TO_HL:
		if (hl + wz > 0xffff)
			f |= flags::CF;
		else
			f &= ~flags::CF;
		hl += wz;
		break;
	default:
		throw "ERROR";
	}
	if (_current_steps_idx >= 0)
	{
		_current_step = (*_current_steps)[_current_steps_idx++];
	}
}

void i8080::fetch()
{
	_addressbus = pc;
	if (_intgo)
	{
		_databus = STATUS_WO | STATUS_M1 | STATUS_INTA;
		if (_haltint)
		{
			_databus |= STATUS_HLTA;
			_haltint = false;
		}
		_pins &= ~PIN_INTE;
	}
	else
	{
		_databus = STATUS_WO | STATUS_M1 | STATUS_MEMR;
	}
	_pins |= PIN_SYNC;
	_current_steps_idx = -1;
	_current_step = FETCHREAD;
}
void i8080::fetchread()
{
	_pins &= ~PIN_SYNC;
	_pins |= PIN_DBIN;
	if (!(_pins & PIN_READY))
	{
		_pins |= PIN_WAIT;
	}
	else
	{
		_pins &= ~PIN_WAIT;
		if (_intgo)
		{
			_intgo = false;
		}
		else
		{
			pc++;
		}
		_current_step = DECODE;
	}
}
void i8080::decode()
{
	_pins &= ~PIN_DBIN;
	_opcode = _databus;
	_current_steps = &(opcodes_timing[_opcode]);
	_current_steps_idx = 0;
}
void i8080::next_fetch()
{
	_current_step = FETCH;
	_current_steps_idx = -1;
}

void i8080::read()
{
	_addressbus = wz;
	_databus = STATUS_WO | STATUS_MEMR;
	_pins |= PIN_SYNC;
}
void i8080::readio()
{
	_addressbus = (z << 8) | z;
	_databus = STATUS_WO | STATUS_INP;
	_pins |= PIN_SYNC;
}

void i8080::readsp()
{
	_addressbus = sp++;
	_databus = STATUS_WO | STATUS_STACK | STATUS_MEMR;
	_pins |= PIN_SYNC;
}
void i8080::readpc()
{
	_addressbus = pc++;
	_databus = STATUS_WO | STATUS_MEMR;
	_pins |= PIN_SYNC;
}
void i8080::wait()
{
	_pins &= ~PIN_SYNC;
	_pins |= PIN_DBIN;
	if (!(_pins & PIN_READY))
	{
		_pins |= PIN_WAIT;
		_current_steps_idx--;
	}
	else
	{
		_pins &= ~PIN_WAIT;
	}
}
void i8080::write()
{
	_addressbus = wz;
	_databus = 0;
	_pins |= PIN_SYNC;
}
void i8080::writeio()
{
	_addressbus = (z<<8) | z;
	_databus = 16;
	_pins |= PIN_SYNC;
}
void i8080::writesp()
{
	_addressbus = --sp;
	_databus = STATUS_STACK;
	_pins |= PIN_SYNC;
}
void i8080::writedb(const uint8_t value)
{
	_databus = value;
	_pins &= ~PIN_SYNC;
	_pins |= PIN_WR;
	if (!(_pins & PIN_READY))
	{
		_pins |= PIN_WAIT;
		_current_steps_idx--;
	}
	else
	{
		_pins &= ~PIN_WAIT;
	}
}
void i8080::stopwrite()
{
	_pins &= ~PIN_WR;
}
void i8080::setSZP(const uint8_t value) {
	f &= ~(flags::ZF | flags::SF | flags::PF);
	if (value == 0)
		f |= flags::ZF;
	else
		f |= value & flags::SF;
	if (parity(value))
		f |= flags::PF;
}

void i8080::rlc()
{
	if (a >> 7)
		f |= flags::CF;
	else
		f &= ~flags::CF;
	a = ((f&flags::CF) ? 1 : 0) | (a << 1);
}
void i8080::rrc()
{
	if (a & 1)
		f |= flags::CF;
	else
		f &= ~flags::CF;
	a = ((f & flags::CF) ? 0x80 : 0) | (a >> 1);
}
void i8080::ral()
{
	const uint8_t flag = (f&flags::CF) ? 1 : 0;
	if (a >> 7)
		f |= flags::CF;
	else
		f &= ~flags::CF;

	a = (a << 1) | flag;
}
void i8080::rar()
{
	const uint8_t flag = (f & flags::CF) ? 1 : 0;
	if (a & 1)
		f |= flags::CF;
	else
		f &= ~flags::CF;
	a = (a >> 1) | (flag << 7);
}
void i8080::daa()
{
	uint16_t temp = a;
	if (((a & 0xf) > 9) || (f&flags::HF))
	{
		temp += 6;
		if ((a & 0x0f) > 9)
			f |= flags::HF;
		else
			f &= ~flags::HF;
	}
	if ((temp >> 4 > 9) || (f&flags::CF))
	{
		temp += 0x60;
		f |= flags::CF;
	}
	a = temp & 0xff;
	setSZP(a);
}
void i8080::add(const uint8_t value)
{
	const uint16_t sum = a + value;

	f &= ~(flags::HF | flags::PF | flags::CF);
	if (((value & 0x0f) + (a & 0x0f)) > 0x0f)
		f |= flags::HF;

	a = sum & 0xff;
	setSZP(a);
	if (sum > 0xff)
		f |= flags::CF;
}
void i8080::adc(const uint8_t value)
{
	const uint8_t flag = (f & flags::CF) ? 1 : 0;
	const uint16_t sum = a + value + flag;

	f &= ~(flags::HF | flags::CF);
	if (((value & 0x0f) + (a & 0x0f) + flag) > 0x0f)
		f |= flags::HF;

	a = sum & 0xff;
	setSZP(a);
	if (sum > 0xff)
		f |= flags::CF;
}
void i8080::sub(const uint8_t value)
{
	f &= ~(flags::HF | flags::CF);
	if (value > a)
		f |= flags::CF;
	if (((a & 0x0f) - (value & 0x0f)) >= 0)
		f |= flags::HF;

	a -= value;
	setSZP(a);
}
void i8080::sbb(const uint8_t value)
{
	const uint8_t flag = (f & flags::CF) ? 1 : 0;
	f &= ~(flags::HF | flags::CF);
	if (value + flag > a)
		f |= flags::CF;
	if (((a & 0x0f) - (value & 0x0f) - flag) >= 0)
		f |= flags::HF;

	a -= value + flag;
	setSZP(a);
}

void i8080::ana(const uint8_t value)
{
	f &= ~(flags::HF | flags::CF);
	if (((a | value) & 0x08) != 0)
		f |= flags::HF;
	a &= value;
	setSZP(a);
}
void i8080::ora(const uint8_t value)
{
	a |= value;
	f &= ~(flags::CF | flags::HF);
	setSZP(a);
}
void i8080::xra(const uint8_t value)
{
	a ^= value;
	f &= ~(flags::CF | flags::HF);
	setSZP(a);
}
void i8080::cp(const uint8_t value)
{
	f &= ~(flags::CF | flags::HF);

	if (value > a)
		f |= flags::CF;
	if ((value & 0x0f) <= (a & 0x0f))
		f |= flags::HF;
	setSZP(a - value);
}
uint8_t i8080::inc(const uint8_t value)
{
	uint8_t result = value + 1;
	setSZP(result);
	f &= ~flags::HF;
	if ((result & 0x0f) == 0x00)
		f |= flags::HF;
	return result;
}

uint8_t i8080::dec(const uint8_t value)
{
	uint8_t result = value - 1;
	setSZP(result);
	f &= ~flags::HF;
	if (!((result & 0x0f) == 0x0f))
		f |= flags::HF;
	return result;
}
void i8080::jp()
{
	_pins &= ~(PIN_DBIN|PIN_WR);
	pc = z | (_databus << 8);
}
