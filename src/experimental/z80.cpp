#include "z80.h"


using namespace xprocessors::cpu;

void Z80::init()
{
	reset();
}

void Z80::reset(const uint16_t start)
{
	pc = start;
	af = bc = de = hl = ix = iy = wz = sp = 0;
	hli_idx = NORMAL;
	opcode = 0;
	addressbus = 0;
	databus = 0;
	pins = 0;
	current_step = FETCH;
	current_steps_idx = -1;
	iff1 = iff1_requested = false;
	r = 0;
	halted = false;
}

void Z80::tick()
{
	pins &= PIN_INT;
	if (!halted) {
		switch (current_step) {
		case HALT:
			halted = true;
			break;
		case FETCH:
			hli_idx = NORMAL;
			fetch();
			break;
		case FETCH_PREFIX:
			fetch();
			break;
		case FETCHWAIT:
			if (iff1_requested) {
				iff1 = true;
				iff1_requested = false;
			}
			fetchwait(); break;
		case DECODE:
			decode(); break;
		case PREFIX_CB:
			hli_idx = CB;
			break;
		case PREFIX_DD:
			hli_idx = DD;
			break;
		case PREFIX_ED:
			hli_idx = ED;
			break;
		case PREFIX_FD:
			hli_idx = FD;
			break;
		case PREFIX_DDCB:
			hli_idx = DDCB;
			break;
		case WAIT:
			wait(); break;
		case ERROR:
			throw "ERROR";
			break;
		case NOP:
			break;
		case IM1:
			im = interrupt_mode::mode_1;
			break;
		case IM2:
			im = interrupt_mode::mode_2;
			break;
		case DI:
			iff1 = false;
			iff1_requested = false;
			break;
		case EI:
			iff1 = false;
			iff1_requested = true;
			break;

		case LD_BC_INTO_WZ:
			wz = bc;
			break;
		case LD_DE_INTO_WZ:
			wz = de;
			break;
		case LD_HL_INTO_WZ:
			wz = hl;
			break;
		case LD_IX_INTO_WZ:
			wz = ix;
			break;
		case LD_IY_INTO_WZ:
			wz = iy;
			break;
		case LD_SP_INTO_WZ:
			wz = sp;
			break;
		case LD_WZ_INTO_SP:
			sp = wz;
			break;
		case LD_WZ_INTO_HL:
			hl = wz;
			break;
		case LD_WZ_INTO_IX:
			ix = wz;
			break;
		case LD_WZ_INTO_IY:
			iy = wz;
			break;

		// Load databus
		case LD_DB_INTO_A:
			a = databus;
			break;
		case LD_DB_INTO_A_FLAGS:
			a = databus;
			f &= ~(flags::NF | flags::HF);
			setSZXY(a);
			setP(a);
			break;
		case LD_DB_INTO_F:
			f = databus;
			break;
		case LD_DB_INTO_B:
			b = databus;
			break;
		case LD_DB_INTO_B_FLAGS:
			b = databus;
			f &= ~(flags::NF | flags::HF);
			setSZXY(b);
			setP(b);
			break;
		case LD_DB_INTO_C:
			c = databus;
			break;
		case LD_DB_INTO_D:
			d = databus;
			break;
		case LD_DB_INTO_D_FLAGS:
			d = databus;
			f &= ~(flags::NF | flags::HF);
			setSZXY(d);
			setP(d);
			break;
		case LD_DB_INTO_E:
			e = databus;
			break;
		case LD_DB_INTO_H:
			h = databus;
			break;
		case LD_DB_INTO_H_FLAGS:
			h = databus;
			f &= ~(flags::NF | flags::HF);
			setSZXY(h);
			setP(h);
			break;
		case LD_DB_INTO_L:
			l = databus;
			break;
		case LD_DB_INTO_IXL:
			ixl = databus;
			break;
		case LD_DB_INTO_IXH:
			ixh = databus;
			break;
		case LD_DB_INTO_IYL:
			iyl = databus;
			break;
		case LD_DB_INTO_IYH:
			iyh = databus;
			break;
		case LD_DB_INTO_SPL:
			spl = databus;
			break;
		case LD_DB_INTO_SPH:
			sph = databus;
			break;
		case LD_DB_INTO_DB_FLAGS:
			f &= ~(flags::NF | flags::HF);
			setSZXY(databus);
			setP(databus);
			break;

		// Load register
		case LD_A_INTO_B:
			b = a;
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
		case LD_IXH_INTO_B:
			b = ixh;
			break;
		case LD_IXL_INTO_B:
			b = ixl;
			break;
		case LD_IYH_INTO_B:
			b = iyh;
			break;
		case LD_IYL_INTO_B:
			b = iyl;
			break;
		case LD_A_INTO_C:
			c = a;
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
		case LD_IXH_INTO_C:
			c = ixh;
			break;
		case LD_IXL_INTO_C:
			c = ixl;
			break;
		case LD_IYH_INTO_C:
			c = iyh;
			break;
		case LD_IYL_INTO_C:
			c = iyl;
			break;
		case LD_A_INTO_D:
			d = a;
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
		case LD_IXH_INTO_D:
			d = ixh;
			break;
		case LD_IXL_INTO_D:
			d = ixl;
			break;
		case LD_IYH_INTO_D:
			d = iyh;
			break;
		case LD_IYL_INTO_D:
			d = iyl;
			break;
		case LD_A_INTO_E:
			e = a;
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
		case LD_IXH_INTO_E:
			e = ixh;
			break;
		case LD_IXL_INTO_E:
			e = ixl;
			break;
		case LD_IYH_INTO_E:
			e = iyh;
			break;
		case LD_IYL_INTO_E:
			e = iyl;
			break;
		case LD_A_INTO_H:
			h = a;
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
		case LD_A_INTO_L:
			l = a;
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
		case LD_A_INTO_IXH:
			ixh = a;
			break;
		case LD_B_INTO_IXH:
			ixh = b;
			break;
		case LD_C_INTO_IXH:
			ixh = c;
			break;
		case LD_D_INTO_IXH:
			ixh = d;
			break;
		case LD_E_INTO_IXH:
			ixh = e;
			break;
		case LD_IXL_INTO_IXH:
			ixh = ixl;
			break;
		case LD_A_INTO_IXL:
			ixl = a;
			break;
		case LD_B_INTO_IXL:
			ixl = b;
			break;
		case LD_C_INTO_IXL:
			ixl = c;
			break;
		case LD_D_INTO_IXL:
			ixl = d;
			break;
		case LD_E_INTO_IXL:
			ixl = e;
			break;
		case LD_IXH_INTO_IXL:
			ixl = ixh;
			break;
		case LD_A_INTO_IYH:
			iyh = a;
			break;
		case LD_B_INTO_IYH:
			iyh = b;
			break;
		case LD_C_INTO_IYH:
			iyh = c;
			break;
		case LD_D_INTO_IYH:
			iyh = d;
			break;
		case LD_E_INTO_IYH:
			iyh = e;
			break;
		case LD_IYL_INTO_IYH:
			iyh = iyl;
			break;
		case LD_A_INTO_IYL:
			iyl = a;
			break;
		case LD_B_INTO_IYL:
			iyl = b;
			break;
		case LD_C_INTO_IYL:
			iyl = c;
			break;
		case LD_D_INTO_IYL:
			iyl = d;
			break;
		case LD_E_INTO_IYL:
			iyl = e;
			break;
		case LD_IYH_INTO_IYL:
			iyl = iyh;
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
		case LD_H_INTO_A:
			a = h;
			break;
		case LD_L_INTO_A:
			a = l;
			break;
		case LD_IXH_INTO_A:
			a = ixh;
			break;
		case LD_IXL_INTO_A:
			a = ixl;
			break;
		case LD_IYH_INTO_A:
			a = iyh;
			break;
		case LD_IYL_INTO_A:
			a = iyl;
			break;
		case LD_DB_INTO_WZL:
			z = databus;
			break;
		case LD_DB_INTO_WZH:
			w = databus;
			break;
		case LD_A_INTO_WZH:
			w = a;
			break;
		case LD_A_INTO_I:
			i = a;
			break;
		case LD_I_INTO_A:
			a = i;
			break;
		case LD_I_INTO_WZH:
			w = i;
		case LD_A_INTO_R:
			r = a;
			break;
		case LD_R_INTO_A:
			a = r;
			break;

		case READPC:
			readpc(); break;
		case READSP:
			readsp(); break;
		case READWR:
			readwr(); break;
		case READ:
			read(); break;
		case READIO:
			read(true); break;
		case WRITE_DB:
			write(databus);
			break;
		case WRITE_A:
			write(a);
			break;
		case WRITEF:
			write(f);
			break;
		case WRITEB:
			write(b);
			break;
		case WRITEC:
			write(c);
			break;
		case WRITED:
			write(d);
			break;
		case WRITEE:
			write(e);
			break;
		case WRITEH:
			write(h);
			break;
		case WRITEL:
			write(l);
			break;
		case WRITEIXH:
			write(ixh);
			break;
		case WRITEIXL:
			write(ixl);
			break;
		case WRITEIYH:
			write(iyh);
			break;
		case WRITEIYL:
			write(iyl);
			break;
		case WRITEPCL:
			write(pcl);
			break;
		case WRITEPCH:
			write(pch);
			break;
		case WRITESPL:
			write(spl);
			break;
		case WRITESPH:
			write(sph);
			break;
		case WRITE:
			write();
			break;
		case IOWRITE:
			iowrite();
			break;
		case PUSH_A:
			push(a);
			break;
		case PUSH_F:
			push(f);
			break;
		case PUSH_B:
			push(b);
			break;
		case PUSH_C:
			push(c);
			break;
		case PUSH_D:
			push(d);
			break;
		case PUSH_E:
			push(e);
			break;
		case PUSH_H:
			push(h);
			break;
		case PUSH_L:
			push(l);
			break;
		case PUSH_IXH:
			push(ixh);
			break;
		case PUSH_IXL:
			push(ixl);
			break;
		case PUSH_IYH:
			push(iyh);
			break;
		case PUSH_IYL:
			push(iyl);
			break;
		case PUSH_PCH:
			push(pch);
			break;
		case PUSH_PCL:
			push(pcl);
			break;
		case OUTA:
			write(a, true);
			break;
		case OUTC:
			write(c, true);
			break;
		case OUTD:
			write(d, true);
			break;
		case OUTE:
			write(e, true);
			break;
		case OUTH:
			write(h, true);
			break;
		case OUTL:
			write(l, true);
			break;
		case FILLPCH:
			pch = databus;
			break;
		case FILLPCL:
			pcl = databus;
			break;
		case CPAWRH:
			w = a;
			break;
		case JP_WZ:
			pc = wz;
			break;
		case JP_0:
			pc = 0;
			break;
		case JP_08:
			pc = 0x08;
			break;
		case JP_10:
			pc = 0x10;
			break;
		case JP_18:
			pc = 0x18;
			break;
		case JP_20:
			pc = 0x20;
			break;
		case JP_28:
			pc = 0x28;
			break;
		case JP_30:
			pc = 0x30;
			break;
		case JP_38:
			pc = 0x38;
			break;
		case JP:
			jp(); break;
		case JPZ:
			if (f & Z80::flags::ZF)
				jp();
			break;
		case JPNZ:
			if (!(f & Z80::flags::ZF))
				jp();
			break;
		case JPC:
			if (f & Z80::flags::CF)
				jp();
			break;
		case JPNC:
			if (!(f & Z80::flags::CF))
				jp();
			break;
		case JPPE:
			if (f & Z80::flags::PF)
				jp();
			break;
		case JPPO:
			if (!(f & Z80::flags::PF))
				jp();
			break;
		case JPM:
			if (f & Z80::flags::SF)
				jp();
			break;
		case JPP:
			if (!(f & Z80::flags::SF))
				jp();
			break;
		case JPHL:
			pc = hl;
			break;
		case JPIX:
			pc = ix;
			break;
		case JPIY:
			pc = iy;
			break;
		case JR:
			pc += static_cast<int8_t>(databus);
			break;

		case INC_BC:
			bc++;
			break;
		case DEC_BC:
			bc--;
			break;
		case INC_DE:
			de++;
			break;
		case DEC_DE:
			de--;
			break;
		case INC_HL:
			hl++;
			break;
		case DEC_HL:
			hl--;
			break;
		case DECPC:
			pc--;
			break;
		case INC_IX:
			ix++;
			break;
		case DEC_IX:
			ix--;
			break;
		case INC_IY:
			iy++;
			break;
		case DEC_IY:
			iy--;
			break;
		case INCSP:
			sp++;
			break;
		case DECSP:
			sp--;
			break;
		case INC_WZ:
			wz++;
			break;
		case DEC_WZ:
			wz--;
			break;

		case PUSH:
			wz = --sp;
			break;

		case INC_DB:
			databus = inc(databus);
			break;
		case DEC_DB:
			databus = dec(databus);
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
		case INC_IXH:
			ixh = inc(ixh);
			break;
		case INC_IXL:
			ixl = inc(ixl);
			break;
		case INC_IYH:
			ixh = inc(iyh);
			break;
		case INC_IYL:
			iyl = inc(iyl);
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
		case DEC_IXH:
			ixh = dec(ixh);
			break;
		case DEC_IXL:
			ixl = dec(ixl);
			break;
		case DEC_IYH:
			iyh = dec(iyh);
			break;
		case DEC_IYL:
			iyl = dec(iyl);
			break;
		case ADD_WZ_TO_HL:
			add16(hl, wz);
			break;
		case ADD_WZ_TO_IX:
			add16(ix, wz);
			break;
		case ADD_WZ_TO_IY:
			add16(iy, wz);
			break;
		case ADC_WZ_TO_HL:
			adc16(wz);
			break;
		case SBC_WZ_TO_HL:
			sbc16(wz);
			break;
		case ADD_DB_TO_WZ:
			wz += static_cast<int8_t>(databus);
			break;

		case ADD_DB_TO_A:
			add(databus);
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
		case ADD_IXH_TO_A:
			add(ixh);
			break;
		case ADD_IXL_TO_A:
			add(ixl);
			break;
		case ADD_IYH_TO_A:
			add(iyh);
			break;
		case ADD_IYL_TO_A:
			add(iyl);
			break;
		case ADD_A_TO_A:
			add(a);
			break;

		case ADC_DB_TO_A:
			adc(databus);
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
		case ADC_IXH_TO_A:
			adc(ixh);
			break;
		case ADC_IXL_TO_A:
			adc(ixl);
			break;
		case ADC_IYH_TO_A:
			adc(iyh);
			break;
		case ADC_IYL_TO_A:
			adc(iyl);
			break;
		case ADC_A_TO_A:
			adc(a);
			break;

		case SUB_DB_TO_A:
			sub(databus);
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
		case SUB_IXH_TO_A:
			sub(ixh);
			break;
		case SUB_IXL_TO_A:
			sub(ixl);
			break;
		case SUB_IYH_TO_A:
			sub(iyh);
			break;
		case SUB_IYL_TO_A:
			sub(iyl);
			break;
		case SUB_A_TO_A:
			sub(a);
			break;

		case SBC_DB_TO_A:
			sbc(databus);
			break;
		case SBC_B_TO_A:
			sbc(b);
			break;
		case SBC_C_TO_A:
			sbc(c);
			break;
		case SBC_D_TO_A:
			sbc(d);
			break;
		case SBC_E_TO_A:
			sbc(e);
			break;
		case SBC_H_TO_A:
			sbc(h);
			break;
		case SBC_L_TO_A:
			sbc(l);
			break;
		case SBC_IXH_TO_A:
			sbc(ixh);
			break;
		case SBC_IXL_TO_A:
			sbc(ixl);
			break;
		case SBC_IYH_TO_A:
			sbc(iyh);
			break;
		case SBC_IYL_TO_A:
			sbc(iyl);
			break;
		case SBC_A_TO_A:
			sbc(a);
			break;

		case AND_DB:
			ana(databus);
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
		case AND_IXH:
			ana(ixh);
			break;
		case AND_IXL:
			ana(ixl);
			break;
		case AND_IYH:
			ana(iyh);
			break;
		case AND_IYL:
			ana(iyl);
			break;
		case AND_A:
			ana(a);
			break;
		case OR_DB:
			ora(databus);
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
		case OR_IXH:
			ora(ixh);
			break;
		case OR_IXL:
			ora(ixl);
			break;
		case OR_IYH:
			ora(iyh);
			break;
		case OR_IYL:
			ora(iyl);
			break;
		case OR_A:
			ora(a);
			break;
		case XOR_DB:
			xra(databus);
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
		case XOR_IXH:
			xra(ixh);
			break;
		case XOR_IXL:
			xra(ixl);
			break;
		case XOR_IYH:
			xra(iyh);
			break;
		case XOR_IYL:
			xra(iyl);
			break;
		case XOR_A:
			xra(a);
			break;
		case CP_DB:
			cp(databus);
			break;
		case CP_DB_NOCARRY:
			cp(databus, true);
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
		case CP_IXH:
			cp(ixh);
			break;
		case CP_IXL:
			cp(ixl);
			break;
		case CP_IYH:
			cp(iyh);
			break;
		case CP_IYL:
			cp(iyl);
			break;
		case CP_A:
			cp(a);
			break;
		case RLC_A:
			a = rlc(a);
			break;
		case RLC_B:
			b = rlc(b);
			break;
		case RLC_C:
			c = rlc(c);
			break;
		case RLC_D:
			d = rlc(d);
			break;
		case RLC_E:
			e = rlc(e);
			break;
		case RLC_H:
			h = rlc(h);
			break;
		case RLC_L:
			l = rlc(l);
			break;
		case RLC_DB:
			databus = rlc(databus);
			break;
		case RL_A:
			a = rl(a);
			break;
		case RL_B:
			b = rl(b);
			break;
		case RL_C:
			c = rl(c);
			break;
		case RL_D:
			d = rl(d);
			break;
		case RL_E:
			e = rl(e);
			break;
		case RL_H:
			h = rl(h);
			break;
		case RL_L:
			l = rl(l);
			break;
		case RL_DB:
			databus = rl(databus);
			break;
		case RRC_A:
			a = rrc(a);
			break;
		case RRC_B:
			b = rrc(b);
			break;
		case RRC_C:
			c = rrc(c);
			break;
		case RRC_D:
			d = rrc(d);
			break;
		case RRC_E:
			e = rrc(e);
			break;
		case RRC_H:
			h = rrc(h);
			break;
		case RRC_L:
			l = rrc(l);
			break;
		case RRC_DB:
			databus = rrc(databus);
			break;
		case RR_A:
			a = rr(a);
			break;
		case RR_B:
			b = rr(b);
			break;
		case RR_C:
			c = rr(c);
			break;
		case RR_D:
			d = rr(d);
			break;
		case RR_E:
			e = rr(e);
			break;
		case RR_H:
			h = rr(h);
			break;
		case RR_L:
			l = rr(l);
			break;
		case RR_DB:
			databus = rr(databus);
			break;
		case SLA_A:
			a = sla(a);
			break;
		case SLA_B:
			b = sla(b);
			break;
		case SLA_C:
			c = sla(c);
			break;
		case SLA_D:
			d = sla(d);
			break;
		case SLA_E:
			e = sla(e);
			break;
		case SLA_H:
			h = sla(h);
			break;
		case SLA_L:
			l = sla(l);
			break;
		case SLA_DB:
			databus = sla(databus);
			break;
		case SLL_A:
			a = sll(a);
			break;
		case SLL_B:
			b = sll(b);
			break;
		case SLL_C:
			c = sll(c);
			break;
		case SLL_D:
			d = sll(d);
			break;
		case SLL_E:
			e = sll(e);
			break;
		case SLL_H:
			h = sll(h);
			break;
		case SLL_L:
			l = sll(l);
			break;
		case SLL_DB:
			databus = sll(databus);
			break;
		case SRL_A:
			a = srl(a);
			break;
		case SRL_B:
			b = srl(b);
			break;
		case SRL_C:
			c = srl(c);
			break;
		case SRL_D:
			d = srl(d);
			break;
		case SRL_E:
			e = srl(e);
			break;
		case SRL_H:
			h = srl(h);
			break;
		case SRL_L:
			l = srl(l);
			break;
		case SRL_DB:
			databus = srl(databus);
			break;
		case SRA_A:
			a = sra(a);
			break;
		case SRA_B:
			b = sra(b);
			break;
		case SRA_C:
			c = sra(c);
			break;
		case SRA_D:
			d = sra(d);
			break;
		case SRA_E:
			e = sra(e);
			break;
		case SRA_H:
			h = sra(h);
			break;
		case SRA_L:
			l = sra(l);
			break;
		case SRA_DB:
			databus = sra(databus);
			break;

		case RLCA:
			rla(true);
			break;
		case RRCA:
			rra(true);
			break;
		case RLA:
			rla(false);
			break;
		case RRA:
			rra(false);
			break;
		case DAA:
			daa();
			break;
		case CPL:
			cpl();
			break;
		case CCF:
			ccf();
			break;
		case SCF:
			scf();
			break;
		case NEG:
			neg();
			break;
		case RRD:
			rrd();
			break;
		case RLD:
			rld();
			break;
		case EXAF:
			exaf();
			break;
		case EX_DE_HL:
			exdehl();
			break;
		case EXX:
			exx();
			break;
		case LD_ITER:
			ld_iter();
			if (bc == 0)
				next_fetch();
			break;
		case CP_ITER:
			cp_iter();
			if ((bc == 0)||(f&flags::ZF))
				next_fetch();
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
		case STOPPE:
			if (f & flags::PF)
				next_fetch();
			break;
		case STOPPO:
			if (!(f & flags::PF))
				next_fetch();
			break;
		case STOPM:
			if (f & flags::SF)
				next_fetch();
			break;
		case STOPP:
			if (!(f & flags::SF))
				next_fetch();
			break;
		case BIT0_DB:
			bit(databus, 0);
			break;
		case BIT0_A:
			bit(a, 0);
			break;
		case BIT0_B:
			bit(b, 0);
			break;
		case BIT0_C:
			bit(c, 0);
			break;
		case BIT0_D:
			bit(d, 0);
			break;
		case BIT0_E:
			bit(e, 0);
			break;
		case BIT0_H:
			bit(h, 0);
			break;
		case BIT0_L:
			bit(l, 0);
			break;
		case BIT1_DB:
			bit(databus, 1);
			break;
		case BIT1_A:
			bit(a, 1);
			break;
		case BIT1_B:
			bit(b, 1);
			break;
		case BIT1_C:
			bit(c, 1);
			break;
		case BIT1_D:
			bit(d, 1);
			break;
		case BIT1_E:
			bit(e, 1);
			break;
		case BIT1_H:
			bit(h, 1);
			break;
		case BIT1_L:
			bit(l, 1);
			break;
		case BIT2_DB:
			bit(databus, 2);
			break;
		case BIT2_A:
			bit(a, 2);
			break;
		case BIT2_B:
			bit(b, 2);
			break;
		case BIT2_C:
			bit(c, 2);
			break;
		case BIT2_D:
			bit(d, 2);
			break;
		case BIT2_E:
			bit(e, 2);
			break;
		case BIT2_H:
			bit(h, 2);
			break;
		case BIT2_L:
			bit(l, 2);
			break;
		case BIT3_DB:
			bit(databus, 3);
			break;
		case BIT3_A:
			bit(a, 3);
			break;
		case BIT3_B:
			bit(b, 3);
			break;
		case BIT3_C:
			bit(c, 3);
			break;
		case BIT3_D:
			bit(d, 3);
			break;
		case BIT3_E:
			bit(e, 3);
			break;
		case BIT3_H:
			bit(h, 3);
			break;
		case BIT3_L:
			bit(l, 3);
			break;
		case BIT4_DB:
			bit(databus, 4);
			break;
		case BIT4_A:
			bit(a, 4);
			break;
		case BIT4_B:
			bit(b, 4);
			break;
		case BIT4_C:
			bit(c, 4);
			break;
		case BIT4_D:
			bit(d, 4);
			break;
		case BIT4_E:
			bit(e, 4);
			break;
		case BIT4_H:
			bit(h, 4);
			break;
		case BIT4_L:
			bit(l, 4);
			break;
		case BIT5_DB:
			bit(databus, 5);
			break;
		case BIT5_A:
			bit(a, 5);
			break;
		case BIT5_B:
			bit(b, 5);
			break;
		case BIT5_C:
			bit(c, 5);
			break;
		case BIT5_D:
			bit(d, 5);
			break;
		case BIT5_E:
			bit(e, 5);
			break;
		case BIT5_H:
			bit(h, 5);
			break;
		case BIT5_L:
			bit(l, 5);
			break;
		case BIT6_DB:
			bit(databus, 6);
			break;
		case BIT6_A:
			bit(a, 6);
			break;
		case BIT6_B:
			bit(b, 6);
			break;
		case BIT6_C:
			bit(c, 6);
			break;
		case BIT6_D:
			bit(d, 6);
			break;
		case BIT6_E:
			bit(e, 6);
			break;
		case BIT6_H:
			bit(h, 6);
			break;
		case BIT6_L:
			bit(l, 6);
			break;
		case BIT7_DB:
			bit(databus, 7);
			break;
		case BIT7_A:
			bit(a, 7);
			break;
		case BIT7_B:
			bit(b, 7);
			break;
		case BIT7_C:
			bit(c, 7);
			break;
		case BIT7_D:
			bit(d, 7);
			break;
		case BIT7_E:
			bit(e, 7);
			break;
		case BIT7_H:
			bit(h, 7);
			break;
		case BIT7_L:
			bit(l, 7);
			break;
		case SET0_DB:
			databus |= 1;
			break;
		case SET0_A:
			a |= 1;
			break;
		case SET0_B:
			b |= 1;
			break;
		case SET0_C:
			c |= 1;
			break;
		case SET0_D:
			d |= 1;
			break;
		case SET0_E:
			e |= 1;
			break;
		case SET0_H:
			h |= 1;
			break;
		case SET0_L:
			l |= 1;
			break;
		case SET1_DB:
			databus |= 2;
			break;
		case SET1_A:
			a |= 2;
			break;
		case SET1_B:
			b |= 2;
			break;
		case SET1_C:
			c |= 2;
			break;
		case SET1_D:
			d |= 2;
			break;
		case SET1_E:
			e |= 2;
			break;
		case SET1_H:
			h |= 2;
			break;
		case SET1_L:
			l |= 2;
			break;
		case SET2_DB:
			databus |= 4;
			break;
		case SET2_A:
			a |= 4;
			break;
		case SET2_B:
			b |= 4;
			break;
		case SET2_C:
			c |= 4;
			break;
		case SET2_D:
			d |= 4;
			break;
		case SET2_E:
			e |= 4;
			break;
		case SET2_H:
			h |= 4;
			break;
		case SET2_L:
			l |= 4;
			break;
		case SET3_DB:
			databus |= 8;
			break;
		case SET3_A:
			a |= 8;
			break;
		case SET3_B:
			b |= 8;
			break;
		case SET3_C:
			c |= 8;
			break;
		case SET3_D:
			d |= 8;
			break;
		case SET3_E:
			e |= 8;
			break;
		case SET3_H:
			h |= 8;
			break;
		case SET3_L:
			l |= 8;
			break;
		case SET4_DB:
			databus |= 16;
			break;
		case SET4_A:
			a |= 16;
			break;
		case SET4_B:
			b |= 16;
			break;
		case SET4_C:
			c |= 16;
			break;
		case SET4_D:
			d |= 16;
			break;
		case SET4_E:
			e |= 16;
			break;
		case SET4_H:
			h |= 16;
			break;
		case SET4_L:
			l |= 16;
			break;
		case SET5_DB:
			databus |= 32;
			break;
		case SET5_A:
			a |= 32;
			break;
		case SET5_B:
			b |= 32;
			break;
		case SET5_C:
			c |= 32;
			break;
		case SET5_D:
			d |= 32;
			break;
		case SET5_E:
			e |= 32;
			break;
		case SET5_H:
			h |= 32;
			break;
		case SET5_L:
			l |= 32;
			break;
		case SET6_DB:
			databus |= 64;
			break;
		case SET6_A:
			a |= 64;
			break;
		case SET6_B:
			b |= 64;
			break;
		case SET6_C:
			c |= 64;
			break;
		case SET6_D:
			d |= 64;
			break;
		case SET6_E:
			e |= 64;
			break;
		case SET6_H:
			h |= 64;
			break;
		case SET6_L:
			l |= 64;
			break;
		case SET7_DB:
			databus |= 128;
			break;
		case SET7_A:
			a |= 128;
			break;
		case SET7_B:
			b |= 128;
			break;
		case SET7_C:
			c |= 128;
			break;
		case SET7_D:
			d |= 128;
			break;
		case SET7_E:
			e |= 128;
			break;
		case SET7_H:
			h |= 128;
			break;
		case SET7_L:
			l |= 128;
			break;
		case RES0_DB:
			databus &= 0b11111110;
			break;
		case RES0_A:
			a &= 0b11111110;
			break;
		case RES0_B:
			b &= 0b11111110;
			break;
		case RES0_C:
			c &= 0b11111110;
			break;
		case RES0_D:
			d &= 0b11111110;
			break;
		case RES0_E:
			e &= 0b11111110;
			break;
		case RES0_H:
			h &= 0b11111110;
			break;
		case RES0_L:
			l &= 0b11111110;
			break;
		case RES1_DB:
			databus &= 0b11111101;
			break;
		case RES1_A:
			a &= 0b11111101;
			break;
		case RES1_B:
			b &= 0b11111101;
			break;
		case RES1_C:
			c &= 0b11111101;
			break;
		case RES1_D:
			d &= 0b11111101;
			break;
		case RES1_E:
			e &= 0b11111101;
			break;
		case RES1_H:
			h &= 0b11111101;
			break;
		case RES1_L:
			l &= 0b11111101;
			break;
		case RES2_DB:
			databus &= 0b11111011;
			break;
		case RES2_A:
			a &= 0b11111011;
			break;
		case RES2_B:
			b &= 0b11111011;
			break;
		case RES2_C:
			c &= 0b11111011;
			break;
		case RES2_D:
			d &= 0b11111011;
			break;
		case RES2_E:
			e &= 0b11111011;
			break;
		case RES2_H:
			h &= 0b11111011;
			break;
		case RES2_L:
			l &= 0b11111011;
			break;
		case RES3_DB:
			databus &= 0b11110111;
			break;
		case RES3_A:
			a &= 0b11110111;
			break;
		case RES3_B:
			b &= 0b11110111;
			break;
		case RES3_C:
			c &= 0b11110111;
			break;
		case RES3_D:
			d &= 0b11110111;
			break;
		case RES3_E:
			e &= 0b11110111;
			break;
		case RES3_H:
			h &= 0b11110111;
			break;
		case RES3_L:
			l &= 0b11110111;
			break;
		case RES4_DB:
			databus &= 0b11101111;
			break;
		case RES4_A:
			a &= 0b11101111;
			break;
		case RES4_B:
			b &= 0b11101111;
			break;
		case RES4_C:
			c &= 0b11101111;
			break;
		case RES4_D:
			d &= 0b11101111;
			break;
		case RES4_E:
			e &= 0b11101111;
			break;
		case RES4_H:
			h &= 0b11101111;
			break;
		case RES4_L:
			l &= 0b11101111;
			break;
		case RES5_DB:
			databus &= 0b11011111;
			break;
		case RES5_A:
			a &= 0b11011111;
			break;
		case RES5_B:
			b &= 0b11011111;
			break;
		case RES5_C:
			c &= 0b11011111;
			break;
		case RES5_D:
			d &= 0b11011111;
			break;
		case RES5_E:
			e &= 0b11011111;
			break;
		case RES5_H:
			h &= 0b11011111;
			break;
		case RES5_L:
			l &= 0b11011111;
			break;
		case RES6_DB:
			databus &= 0b10111111;
			break;
		case RES6_A:
			a &= 0b10111111;
			break;
		case RES6_B:
			b &= 0b10111111;
			break;
		case RES6_C:
			c &= 0b10111111;
			break;
		case RES6_D:
			d &= 0b10111111;
			break;
		case RES6_E:
			e &= 0b10111111;
			break;
		case RES6_H:
			h &= 0b10111111;
			break;
		case RES6_L:
			l &= 0b10111111;
			break;
		case RES7_DB:
			databus &= 0b01111111;
			break;
		case RES7_A:
			a &= 0b01111111;
			break;
		case RES7_B:
			b &= 0b01111111;
			break;
		case RES7_C:
			c &= 0b01111111;
			break;
		case RES7_D:
			d &= 0b01111111;
			break;
		case RES7_E:
			e &= 0b01111111;
			break;
		case RES7_H:
			h &= 0b01111111;
			break;
		case RES7_L:
			l &= 0b01111111;
			break;
		}
	}
	if (current_steps_idx >= 0)
	{
		if (halted)
			current_step = FETCH;
		else
			current_step = (*current_steps)[current_steps_idx++];
		if (current_step == FETCH) {
			if ((iff1) && (pins&PIN_INT)) {
				iff1 = false;
				pins &= ~PIN_INT;
				halted = false;
				switch (im)
				{
				case interrupt_mode::mode_1:
					current_steps = &(opcodes_timing[0x100]);
					break;
				case interrupt_mode::mode_2:
					current_steps = &(opcodes_timing[0x101]);
					break;
				default:
					break;
				}
				current_steps_idx = 0;
				current_step = (*current_steps)[current_steps_idx++];
			}
		}
	}
}

void Z80::fetch()
{
	// Request opcode
	addressbus = pc++;
	pins = PIN_MREQ | PIN_RD | PIN_M1 | (pins&PIN_INT);
	if ((r & 0x7f) == 0x7f)
		r = r & 0x80;
	else
		r++;
	current_steps_idx = -1;
	current_step = FETCHWAIT;
}
void Z80::fetchwait()
{
	current_step = DECODE;
}
void Z80::decode()
{	
	opcode = databus;
	pins = PIN_MREQ | PIN_RFSH | (pins&PIN_INT);
	switch (hli_idx) {
	case NORMAL:
		current_steps = &(opcodes_timing[opcode]);
		break;
	case CB:
		current_steps = &(opcodes_timing_cb[opcode]);
		break;
	case DD:
		current_steps = &(opcodes_timing_dd[opcode]);
		break;
	case ED:
		current_steps = &(opcodes_timing_ed[opcode]);
		break;
	case FD:
		current_steps = &(opcodes_timing_fd[opcode]);
		break;
	case DDCB:
		current_steps = &(opcodes_timing_ddcb[opcode]);
		break;
	}
	current_steps_idx = 0;
}
void Z80::wait()
{
}
void Z80::readpc()
{
	addressbus = pc++;
	pins = PIN_MREQ | PIN_RD | (pins&PIN_INT);
}
void Z80::readsp()
{
	addressbus = sp++;
	pins = PIN_MREQ | PIN_RD | (pins&PIN_INT);
}
void Z80::readwr()
{
	addressbus = wz++;
	pins = PIN_MREQ | PIN_RD | (pins&PIN_INT);
}
void Z80::write(const uint8_t r, const bool io)
{
	addressbus = wz;
	databus = r;
	pins = (pins&PIN_INT) | ((io)? PIN_IORQ : PIN_MREQ);
}
void Z80::push(const uint8_t r)
{
	addressbus = --sp;
	databus = r;
	pins = PIN_MREQ | (pins&PIN_INT);
}
void Z80::write()
{
	pins = PIN_MREQ | PIN_WR | (pins&PIN_INT);
}
void Z80::iowrite()
{
	pins = PIN_IORQ | PIN_WR | (pins&PIN_INT);
}
void Z80::read(const bool io)
{
	addressbus = wz;
	pins = (pins&PIN_INT) | (((io)? PIN_IORQ : PIN_MREQ)) | PIN_RD;
}
void Z80::next_fetch()
{
	current_step = FETCH;
	current_steps_idx = -1;
}
void Z80::jp()
{
	pc = z | (databus<<8);
}
void Z80::setSZXY(const uint8_t value) {
	f &= ~(flags::ZF | flags::XF | flags::SF | flags::YF);
	if (value == 0)
		f |= flags::ZF;
	else
		f |= value & (flags::XF | flags::SF | flags::YF);
}
void Z80::setSZXY(const uint16_t value) {
	f &= ~(flags::ZF | flags::XF | flags::SF | flags::YF);
	if (value == 0)
		f |= flags::ZF;
	else
		f |= (value >> 8) & (flags::XF | flags::SF | flags::YF);
}

void Z80::setP(const uint8_t value) {
	uint8_t t = value;
	t ^= t >> 4;
	t ^= t >> 2;
	if ((t ^= t >> 1) & 1)
		f &= ~(flags::PF);
	else
		f |= flags::PF;
}

uint8_t Z80::inc(const uint8_t value) {
	uint8_t result = value + 1;
	f &= ~(flags::NF | flags::HF | flags::VF);
	setSZXY(result);
	if (result == 0x80)
		f |= flags::VF;
	if ((result & 0x0f) == 0x00)
		f |= flags::HF;
	return result;
}

uint8_t Z80::dec(const uint8_t value) {
	uint8_t result = value - 1;
	f &= ~(flags::HF | flags::VF);
	f |= flags::NF;
	setSZXY(result);
	if (result == 0x7f)
		f |= flags::VF;
	if ((result & 0x0f) == 0x0f)
		f |= flags::HF;
	return result;
}

void Z80::rla(const bool c) {
	const bool carry = (c) ? a & 0x80 : f & flags::CF;

	f &= ~(flags::NF | flags::HF | flags::CF);
	if (a & 0x80)
		f |= flags::CF;
	a <<= 1;
	a |= (carry) ? 1 : 0;
}
void Z80::rra(const bool c) {
	const bool carry = (c) ? a & 0x01 : f & flags::CF;

	f &= ~(flags::NF | flags::HF | flags::CF);
	if (a & 0x01)
		f |= flags::CF;
	a >>= 1;
	a |= (carry) ? 0x80 : 0;
}
void Z80::daa() {
	uint8_t carry = 0;
	uint8_t diff = 0;
	if ((f & flags::CF) || (a > 0x99)) {
		diff += 0x60;
		carry = flags::CF;
	}
	if ((f & flags::HF) || ((a & 0xf) > 9))
		diff += 6;

	if (f & flags::NF) {
		if ((a & 0xf) > 5)
			f &= ~flags::HF;
		a -= diff;
	}
	else {
		if ((a & 0xf) > 9)
			f |= flags::HF;
		else
			f &= ~flags::HF;
		a += diff;
	}
	setSZXY(a);
	setP(a);
	f |= carry;
}

void Z80::cpl() {
	a = ~a;
	f |= (flags::HF | flags::NF);
}
void Z80::scf() {
	f |= flags::CF;
	f &= ~(flags::NF | flags::XF | flags::YF | flags::HF);
	f |= a & (flags::XF | flags::YF);
}
void Z80::ccf() {
	if (f & flags::CF) {
		f &= ~(flags::NF | flags::CF | flags::XF | flags::YF);
		f |= flags::HF;
	}
	else {
		f &= ~(flags::NF | flags::HF | flags::XF | flags::YF);
		f |= flags::CF;
	}
	f |= a & (flags::XF | flags::YF);
}

void Z80::add(const uint8_t value)
{
	const uint16_t sum = a + value;
	const uint16_t carryIns = sum ^ a ^ value;

	a = sum & 0xff;

	setSZXY(a);
	f &= ~(flags::NF | flags::HF | flags::PF | flags::CF);
	if (sum > 0xff)
		f |= flags::CF;
	if (carryIns & 0x10)
		f |= flags::HF;
	if (((carryIns >> 7) & 0x1) ^ ((carryIns >> 8) & 0x1))
		f |= flags::PF;
}
void Z80::adc(const uint8_t value)
{
	const uint8_t flag = (f & flags::CF) ? 1 : 0;
	const uint16_t sum = a + value + flag;
	const uint16_t carryIns = sum ^ a ^ value;

	a = sum & 0xff;

	setSZXY(a);
	f &= ~(flags::NF | flags::HF | flags::PF | flags::CF);
	if (sum > 0xff)
		f |= flags::CF;
	if (carryIns & 0x10)
		f |= flags::HF;
	if (((carryIns >> 7) & 0x1) ^ ((carryIns >> 8) & 0x1))
		f |= flags::PF;
}
void Z80::sub(const uint8_t value)
{
	const uint16_t sum = a - value;
	const uint16_t carryIns = (sum ^ a ^ value);

	a = sum & 0xff;

	setSZXY(a);
	f &= ~(flags::HF | flags::PF | flags::CF);
	f |= flags::NF;
	if (carryIns & 0x10)
		f |= flags::HF;
	if (((carryIns >> 7) & 0x1) ^ ((carryIns >> 8) & 0x1))
		f |= flags::PF;
	if ((carryIns >> 8) & 0x1)
		f |= flags::CF;
}
void Z80::sbc(const uint8_t value)
{
	const uint8_t flag = (f & flags::CF) ? 1 : 0;
	const uint16_t sum = a - value - flag;
	const uint16_t carryIns = (sum ^ a ^ value);

	a = sum & 0xff;

	setSZXY(a);
	f &= ~(flags::HF | flags::PF | flags::CF);
	f |= flags::NF;
	if (carryIns & 0x10)
		f |= flags::HF;
	if (((carryIns >> 7) & 0x1) ^ ((carryIns >> 8) & 0x1))
		f |= flags::PF;
	if ((carryIns >> 8) & 0x1)
		f |= flags::CF;
}
void Z80::ana(const uint8_t value) {
	a &= value;

	f = 0;
	setSZXY(a);
	setP(a);
	f |= flags::HF;
}
void Z80::ora(const uint8_t value) {
	a |= value;

	f = 0;
	setSZXY(a);
	setP(a);
}
void Z80::xra(const uint8_t value)
{
	a ^= value;

	f = 0;
	setSZXY(a);
	setP(a);
}

void Z80::cp(const uint8_t value, const bool nocarry)
{
	const uint16_t sum = a - value;
	const uint16_t carryIns = (sum ^ a ^ value);

	f = (nocarry)? (f&flags::CF)|flags::NF : flags::NF;
	if (sum == 0)
		f |= flags::ZF;
	else
		f |= sum & flags::SF;
	f |= value & (flags::XF | flags::YF);

	if (carryIns & 0x10)
		f |= flags::HF;
	if (((carryIns >> 7) & 0x1) ^ ((carryIns >> 8) & 0x1))
		f |= flags::PF;
	if ((!nocarry) && ((carryIns >> 8) & 0x1))
		f |= flags::CF;
}

void Z80::exaf()
{
	const register16_t tmp = af;
	af = afprime;
	afprime = tmp;
}
void Z80::exx()
{
	register16_t tmp = bc;
	bc = bcprime;
	bcprime = tmp;
	tmp = de;
	de = deprime;
	deprime = tmp;
	tmp = hl;
	hl = hlprime;
	hlprime = tmp;
}
void Z80::exdehl()
{
	const register16_t tmp = de;
	de = hl;
	hl = tmp;
}
void Z80::ld_iter() {
	--bc;
	f &= ~(Z80::flags::HF | Z80::flags::NF | Z80::flags::PF);
	if (bc != 0)
		f |= Z80::flags::PF;
}
void Z80::cp_iter() {
	--bc;
	f &= ~(Z80::flags::PF);
	f |= Z80::flags::NF;
	if (bc != 0)
		f |= Z80::flags::PF;
}

void Z80::sbc16(const uint16_t b) {
	const uint32_t s = hl - b - ((f&Z80::flags::CF) ? 1 : 0);
	const uint32_t c = (s ^ hl ^ b) & 0x18000;
	hl = static_cast<uint16_t>(s);
	f &= ~(Z80::flags::CF | Z80::flags::HF | Z80::flags::PF);
	f |= Z80::flags::NF;
	if (s > 0xffff)
		f |= Z80::flags::CF;
	if (c & 0x1000)
		f |= Z80::flags::HF;
	if (((c >> 15) & 0x1) ^ ((c >> 16) & 0x1))
		f |= Z80::flags::PF;
	setSZXY(hl);
}

void Z80::add16(uint16_t& r1, const uint16_t r2)
{
	const uint16_t previous_r1 = r1;
	r1 += r2;
	f &= ~(Z80::flags::NF | Z80::flags::CF | Z80::flags::HF);
	if (r2 > 0xffff - previous_r1)
		f |= Z80::flags::CF;
	if (((previous_r1 ^ r1 ^ r2) >> 11) & 0x1)
		f |= Z80::flags::HF;
}

void Z80::adc16(const uint16_t b) {
	const uint32_t s = hl + b + ((f&Z80::flags::CF) ? 1 : 0);
	const uint32_t c = s ^ hl ^ b;
	hl = static_cast<uint16_t>(s);
	f &= ~(Z80::flags::NF | Z80::flags::CF | Z80::flags::HF | Z80::flags::PF);
	if (s > 0xffff)
		f |= Z80::flags::CF;
	if (c & 0x1000)
		f |= Z80::flags::HF;
	if (((c >> 15) & 0x1) ^ ((c >> 16) & 0x1))
		f |= Z80::flags::PF;
	setSZXY(hl);
}

void Z80::bit(const uint8_t value, const uint8_t b) {
	f &= Z80::flags::CF;
	f |= Z80::flags::HF;
	f |= (value & (1 << b)) ? Z80::flags::SF : Z80::flags::ZF | Z80::flags::VF;
}

void Z80::neg() {
	const uint8_t ap = a;
	a = -a;

	setSZXY(a);
	f &= ~(Z80::flags::PF | Z80::flags::CF | Z80::flags::HF);
	if (ap != 0)
		f |= Z80::flags::CF;
	if (ap == 0x80)
		f |= Z80::flags::PF;
	f |= Z80::flags::NF;
	if ((a ^ ap) & 0x10)
		f |= Z80::flags::HF;
}


void Z80::rrd() {
	const uint8_t tmp = a & 0x0f;
	a = (a & 0xf0) | (databus & 0x0f);
	databus = static_cast<uint8_t>((databus >> 4) | (tmp << 4));

	f &= Z80::flags::CF;
	setSZXY(a);
	setP(a);
}

void Z80::rld() {
	const uint8_t tmp = a & 0x0f;
	a = (a & 0xf0) | (databus >> 4);
	databus = static_cast<uint8_t>((databus << 4) | tmp);

	f &= Z80::flags::CF;
	setSZXY(a);
	setP(a);
}

uint8_t Z80::rlc(const uint8_t value) {
	const uint8_t result = (value << 1) | (value >> 7);
	f = (value & 0x80) ? Z80::flags::CF : 0;
	setSZXY(result);
	setP(result);
	return result;
}
uint8_t Z80::rrc(const uint8_t value) {
	const uint8_t result = (value >> 1) | (value << 7);
	f = (value & 0x01) ? Z80::flags::CF : 0;
	setSZXY(result);
	setP(result);
	return result;
}
uint8_t Z80::rr(const uint8_t value) {
	const uint8_t result = (value >> 1) | ((f&flags::CF) ? 0x80 : 0);
	f = (value & 0x01) ? Z80::flags::CF : 0;
	setSZXY(result);
	setP(result);
	return result;
}
uint8_t Z80::rl(const uint8_t value) {
	const uint8_t result = (value << 1) | ((f&flags::CF) ? 0x01 : 0);
	f = (value & 0x80) ? Z80::flags::CF : 0;
	setSZXY(result);
	setP(result);
	return result;
}
uint8_t Z80::srl(const uint8_t value) {
	const uint8_t result = value >> 1;
	f = (value & 0x01) ? Z80::flags::CF : 0;
	setSZXY(result);
	setP(result);
	return result;
}
uint8_t Z80::sra(const uint8_t value) {
	const uint8_t result = value >> 1 | (value & 0x80);
	f = (value & 0x01) ? Z80::flags::CF : 0;
	setSZXY(result);
	setP(result);
	return result;
}
uint8_t Z80::sla(const uint8_t value) {
	const uint8_t result = value << 1;
	f = (value & 0x80) ? Z80::flags::CF : 0;
	setSZXY(result);
	setP(result);
	return result;
}
uint8_t Z80::sll(const uint8_t value) {
	const uint8_t result = (value << 1) | 1;
	f = (value & 0x80) ? Z80::flags::CF : 0;
	setSZXY(result);
	setP(result);
	return result;
}
