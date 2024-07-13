#include "sm83.h"


using namespace xprocessors::cpu;


// 2 - operation
void sm83::process(const alu_operation operation)
{
	if (_tcycle == 2)
	{
		switch (operation)
		{
		case alu_operation::INC_A:
			a = inc(a);
			break;
		case alu_operation::INC_B:
			b = inc(b);
			break;
		case alu_operation::INC_C:
			c = inc(c);
			break;
		case alu_operation::INC_D:
			d = inc(d);
			break;
		case alu_operation::INC_E:
			e = inc(e);
			break;
		case alu_operation::INC_H:
			h = inc(h);
			break;
		case alu_operation::INC_L:
			l = inc(l);
			break;
		case alu_operation::INC_Z:
			z = inc(z);
			break;
		case alu_operation::DEC_A:
			a = dec(a);
			break;
		case alu_operation::DEC_B:
			b = dec(b);
			break;
		case alu_operation::DEC_C:
			c = dec(c);
			break;
		case alu_operation::DEC_D:
			d = dec(d);
			break;
		case alu_operation::DEC_E:
			e = dec(e);
			break;
		case alu_operation::DEC_H:
			h = dec(h);
			break;
		case alu_operation::DEC_L:
			l = dec(l);
			break;
		case alu_operation::DEC_Z:
			z = dec(z);
			break;
		case alu_operation::B_TO_A:
			a = b;
			break;
		case alu_operation::C_TO_A:
			a = c;
			break;
		case alu_operation::D_TO_A:
			a = d;
			break;
		case alu_operation::E_TO_A:
			a = e;
			break;
		case alu_operation::H_TO_A:
			a = h;
			break;
		case alu_operation::L_TO_A:
			a = l;
			break;
		case alu_operation::Z_TO_A:
			a = z;
			break;
		case alu_operation::A_TO_B:
			b = a;
			break;
		case alu_operation::C_TO_B:
			b = c;
			break;
		case alu_operation::D_TO_B:
			b = d;
			break;
		case alu_operation::E_TO_B:
			b = e;
			break;
		case alu_operation::H_TO_B:
			b = h;
			break;
		case alu_operation::L_TO_B:
			b = l;
			break;
		case alu_operation::Z_TO_B:
			b = z;
			break;
		case alu_operation::Z_TO_C:
			c = z;
			break;
		case alu_operation::A_TO_C:
			c = a;
			break;
		case alu_operation::B_TO_C:
			c = b;
			break;
		case alu_operation::D_TO_C:
			c = d;
			break;
		case alu_operation::E_TO_C:
			c = e;
			break;
		case alu_operation::H_TO_C:
			c = h;
			break;
		case alu_operation::L_TO_C:
			c = l;
			break;
		case alu_operation::Z_TO_D:
			d = z;
			break;
		case alu_operation::A_TO_D:
			d = a;
			break;
		case alu_operation::B_TO_D:
			d = b;
			break;
		case alu_operation::C_TO_D:
			d = c;
			break;
		case alu_operation::E_TO_D:
			d = e;
			break;
		case alu_operation::H_TO_D:
			d = h;
			break;
		case alu_operation::L_TO_D:
			d = l;
			break;
		case alu_operation::Z_TO_E:
			e = z;
			break;
		case alu_operation::A_TO_E:
			e = a;
			break;
		case alu_operation::B_TO_E:
			e = b;
			break;
		case alu_operation::C_TO_E:
			e = c;
			break;
		case alu_operation::D_TO_E:
			e = d;
			break;
		case alu_operation::H_TO_E:
			e = h;
			break;
		case alu_operation::L_TO_E:
			e = l;
			break;
		case alu_operation::Z_TO_H:
			h = z;
			break;
		case alu_operation::A_TO_H:
			h = a;
			break;
		case alu_operation::B_TO_H:
			h = b;
			break;
		case alu_operation::C_TO_H:
			h = c;
			break;
		case alu_operation::D_TO_H:
			h = d;
			break;
		case alu_operation::E_TO_H:
			h = e;
			break;
		case alu_operation::L_TO_H:
			h = l;
			break;
		case alu_operation::Z_TO_L:
			l = z;
			break;
		case alu_operation::A_TO_L:
			l = a;
			break;
		case alu_operation::B_TO_L:
			l = b;
			break;
		case alu_operation::C_TO_L:
			l = c;
			break;
		case alu_operation::D_TO_L:
			l = d;
			break;
		case alu_operation::E_TO_L:
			l = e;
			break;
		case alu_operation::H_TO_L:
			l = h;
			break;
		case alu_operation::RLCA:
			a = rlc(a);
			f &= ~flags::ZF;
			break;
		case alu_operation::RLC_A:
			a = rlc(a);
			break;
		case alu_operation::RLC_B:
			b = rlc(b);
			break;
		case alu_operation::RLC_C:
			c = rlc(c);
			break;
		case alu_operation::RLC_D:
			d = rlc(d);
			break;
		case alu_operation::RLC_E:
			e = rlc(e);
			break;
		case alu_operation::RLC_H:
			h = rlc(h);
			break;
		case alu_operation::RLC_L:
			l = rlc(l);
			break;
		case alu_operation::RLC_Z:
			z = rlc(z);
			break;
		case alu_operation::RLA:
			a = rl(a);
			f &= ~flags::ZF;
			break;
		case alu_operation::RL_A:
			a = rl(a);
			break;
		case alu_operation::RL_B:
			b = rl(b);
			break;
		case alu_operation::RL_C:
			c = rl(c);
			break;
		case alu_operation::RL_D:
			d = rl(d);
			break;
		case alu_operation::RL_E:
			e = rl(e);
			break;
		case alu_operation::RL_H:
			h = rl(h);
			break;
		case alu_operation::RL_L:
			l = rl(l);
			break;
		case alu_operation::RL_Z:
			z = rl(z);
			break;
		case alu_operation::RRA:
			a = rr(a);
			f &= ~flags::ZF;
			break;
		case alu_operation::RR_A:
			a = rr(a);
			break;
		case alu_operation::RR_B:
			b = rr(b);
			break;
		case alu_operation::RR_C:
			c = rr(c);
			break;
		case alu_operation::RR_D:
			d = rr(d);
			break;
		case alu_operation::RR_E:
			e = rr(e);
			break;
		case alu_operation::RR_H:
			h = rr(h);
			break;
		case alu_operation::RR_L:
			l = rr(l);
			break;
		case alu_operation::RR_Z:
			z = rr(z);
			break;
		case alu_operation::RRCA:
			a = rrc(a);
			f &= ~flags::ZF;
			break;
		case alu_operation::RRC_A:
			a = rrc(a);
			break;
		case alu_operation::RRC_B:
			b = rrc(b);
			break;
		case alu_operation::RRC_C:
			c = rrc(c);
			break;
		case alu_operation::RRC_D:
			d = rrc(d);
			break;
		case alu_operation::RRC_E:
			e = rrc(e);
			break;
		case alu_operation::RRC_H:
			h = rrc(h);
			break;
		case alu_operation::RRC_L:
			l = rrc(l);
			break;
		case alu_operation::RRC_Z:
			z = rrc(z);
			break;
		case alu_operation::SRL_A:
			a = srl(a);
			break;
		case alu_operation::SRL_B:
			b = srl(b);
			break;
		case alu_operation::SRL_C:
			c = srl(c);
			break;
		case alu_operation::SRL_D:
			d = srl(d);
			break;
		case alu_operation::SRL_E:
			e = srl(e);
			break;
		case alu_operation::SRL_H:
			h = srl(h);
			break;
		case alu_operation::SRL_L:
			l = srl(l);
			break;
		case alu_operation::SRL_Z:
			z = srl(z);
			break;
		case alu_operation::SLA_A:
			a = sla(a);
			break;
		case alu_operation::SLA_B:
			b = sla(b);
			break;
		case alu_operation::SLA_C:
			c = sla(c);
			break;
		case alu_operation::SLA_D:
			d = sla(d);
			break;
		case alu_operation::SLA_E:
			e = sla(e);
			break;
		case alu_operation::SLA_H:
			h = sla(h);
			break;
		case alu_operation::SLA_L:
			l = sla(l);
			break;
		case alu_operation::SLA_Z:
			z = sla(z);
			break;
		case alu_operation::SRA_A:
			a = sra(a);
			break;
		case alu_operation::SRA_B:
			b = sra(b);
			break;
		case alu_operation::SRA_C:
			c = sra(c);
			break;
		case alu_operation::SRA_D:
			d = sra(d);
			break;
		case alu_operation::SRA_E:
			e = sra(e);
			break;
		case alu_operation::SRA_H:
			h = sra(h);
			break;
		case alu_operation::SRA_L:
			l = sra(l);
			break;
		case alu_operation::SRA_Z:
			z = sra(z);
			break;
		case alu_operation::SWAP_A:
			a = swap(a);
			break;
		case alu_operation::SWAP_B:
			b = swap(b);
			break;
		case alu_operation::SWAP_C:
			c = swap(c);
			break;
		case alu_operation::SWAP_D:
			d = swap(d);
			break;
		case alu_operation::SWAP_E:
			e = swap(e);
			break;
		case alu_operation::SWAP_H:
			h = swap(h);
			break;
		case alu_operation::SWAP_L:
			l = swap(l);
			break;
		case alu_operation::SWAP_Z:
			z = swap(z);
			break;
		case alu_operation::ADD_B_TO_H:
			h = adc(h, b);
			break;
		case alu_operation::ADD_C_TO_L:
			l = add(l, c);
			break;
		case alu_operation::ADD_D_TO_H:
			h = adc(h, d);
			break;
		case alu_operation::ADD_E_TO_L:
			l = add(l, e);
			break;
		case alu_operation::ADD_H_TO_H:
			h = adc(h, h);
			break;
		case alu_operation::ADD_L_TO_L:
			l = add(l, l);
			break;
		case alu_operation::ADD_SPL_TO_L:
			l = add(spl, l);
			break;
		case alu_operation::ADD_SPH_TO_H:
			h = adc(sph, h);
			break;
		case alu_operation::ADD_B_TO_A:
			add(b);
			break;
		case alu_operation::ADD_C_TO_A:
			add(c);
			break;
		case alu_operation::ADD_D_TO_A:
			add(d);
			break;
		case alu_operation::ADD_E_TO_A:
			add(e);
			break;
		case alu_operation::ADD_H_TO_A:
			add(h);
			break;
		case alu_operation::ADD_L_TO_A:
			add(l);
			break;
		case alu_operation::ADD_A_TO_A:
			add(a);
			break;
		case alu_operation::ADD_Z_TO_A:
			add(z);
			break;
		case alu_operation::ADC_B_TO_A:
			adc(b);
			break;
		case alu_operation::ADC_C_TO_A:
			adc(c);
			break;
		case alu_operation::ADC_D_TO_A:
			adc(d);
			break;
		case alu_operation::ADC_E_TO_A:
			adc(e);
			break;
		case alu_operation::ADC_H_TO_A:
			adc(h);
			break;
		case alu_operation::ADC_L_TO_A:
			adc(l);
			break;
		case alu_operation::ADC_A_TO_A:
			adc(a);
			break;
		case alu_operation::ADC_Z_TO_A:
			adc(z);
			break;
		case alu_operation::SUB_B_TO_A:
			sub(b);
			break;
		case alu_operation::SUB_C_TO_A:
			sub(c);
			break;
		case alu_operation::SUB_D_TO_A:
			sub(d);
			break;
		case alu_operation::SUB_E_TO_A:
			sub(e);
			break;
		case alu_operation::SUB_H_TO_A:
			sub(h);
			break;
		case alu_operation::SUB_L_TO_A:
			sub(l);
			break;
		case alu_operation::SUB_A_TO_A:
			sub(a);
			break;
		case alu_operation::SUB_Z_TO_A:
			sub(z);
			break;
		case alu_operation::SBC_B_TO_A:
			sbc(b);
			break;
		case alu_operation::SBC_C_TO_A:
			sbc(c);
			break;
		case alu_operation::SBC_D_TO_A:
			sbc(d);
			break;
		case alu_operation::SBC_E_TO_A:
			sbc(e);
			break;
		case alu_operation::SBC_H_TO_A:
			sbc(h);
			break;
		case alu_operation::SBC_L_TO_A:
			sbc(l);
			break;
		case alu_operation::SBC_A_TO_A:
			sbc(a);
			break;
		case alu_operation::SBC_Z_TO_A:
			sbc(z);
			break;
		case alu_operation::AND_B:
			ana(b);
			break;
		case alu_operation::AND_C:
			ana(c);
			break;
		case alu_operation::AND_D:
			ana(d);
			break;
		case alu_operation::AND_E:
			ana(e);
			break;
		case alu_operation::AND_H:
			ana(h);
			break;
		case alu_operation::AND_L:
			ana(l);
			break;
		case alu_operation::AND_A:
			ana(a);
			break;
		case alu_operation::AND_Z:
			ana(z);
			break;
		case alu_operation::XOR_B:
			xra(b);
			break;
		case alu_operation::XOR_C:
			xra(c);
			break;
		case alu_operation::XOR_D:
			xra(d);
			break;
		case alu_operation::XOR_E:
			xra(e);
			break;
		case alu_operation::XOR_H:
			xra(h);
			break;
		case alu_operation::XOR_L:
			xra(l);
			break;
		case alu_operation::XOR_A:
			xra(a);
			break;
		case alu_operation::XOR_Z:
			xra(z);
			break;
		case alu_operation::OR_B:
			ora(b);
			break;
		case alu_operation::OR_C:
			ora(c);
			break;
		case alu_operation::OR_D:
			ora(d);
			break;
		case alu_operation::OR_E:
			ora(e);
			break;
		case alu_operation::OR_H:
			ora(h);
			break;
		case alu_operation::OR_L:
			ora(l);
			break;
		case alu_operation::OR_A:
			ora(a);
			break;
		case alu_operation::OR_Z:
			ora(z);
			break;
		case alu_operation::CP_B:
			cp(b);
			break;
		case alu_operation::CP_C:
			cp(c);
			break;
		case alu_operation::CP_D:
			cp(d);
			break;
		case alu_operation::CP_E:
			cp(e);
			break;
		case alu_operation::CP_H:
			cp(h);
			break;
		case alu_operation::CP_L:
			cp(l);
			break;
		case alu_operation::CP_A:
			cp(a);
			break;
		case alu_operation::CP_Z:
			cp(z);
			break;
		case alu_operation::NONE:
			break;
		case alu_operation::DAA:
			daa();
			break;
		case alu_operation::CPL:
			cpl();
			break;
		case alu_operation::CCF:
			ccf();
			break;
		case alu_operation::SCF:
			scf();
			break;
		case alu_operation::BIT_0_A:
			bit(a, 0);
			break;
		case alu_operation::BIT_0_B:
			bit(b, 0);
			break;
		case alu_operation::BIT_0_C:
			bit(c, 0);
			break;
		case alu_operation::BIT_0_D:
			bit(d, 0);
			break;
		case alu_operation::BIT_0_E:
			bit(e, 0);
			break;
		case alu_operation::BIT_0_H:
			bit(h, 0);
			break;
		case alu_operation::BIT_0_L:
			bit(l, 0);
			break;
		case alu_operation::BIT_0_Z:
			bit(z, 0);
			break;
		case alu_operation::BIT_1_A:
			bit(a, 1);
			break;
		case alu_operation::BIT_1_B:
			bit(b, 1);
			break;
		case alu_operation::BIT_1_C:
			bit(c, 1);
			break;
		case alu_operation::BIT_1_D:
			bit(d, 1);
			break;
		case alu_operation::BIT_1_E:
			bit(e, 1);
			break;
		case alu_operation::BIT_1_H:
			bit(h, 1);
			break;
		case alu_operation::BIT_1_L:
			bit(l, 1);
			break;
		case alu_operation::BIT_1_Z:
			bit(z, 1);
			break;
		case alu_operation::BIT_2_A:
			bit(a, 2);
			break;
		case alu_operation::BIT_2_B:
			bit(b, 2);
			break;
		case alu_operation::BIT_2_C:
			bit(c, 2);
			break;
		case alu_operation::BIT_2_D:
			bit(d, 2);
			break;
		case alu_operation::BIT_2_E:
			bit(e, 2);
			break;
		case alu_operation::BIT_2_H:
			bit(h, 2);
			break;
		case alu_operation::BIT_2_L:
			bit(l, 2);
			break;
		case alu_operation::BIT_2_Z:
			bit(z, 2);
			break;
		case alu_operation::BIT_3_A:
			bit(a, 3);
			break;
		case alu_operation::BIT_3_B:
			bit(b, 3);
			break;
		case alu_operation::BIT_3_C:
			bit(c, 3);
			break;
		case alu_operation::BIT_3_D:
			bit(d, 3);
			break;
		case alu_operation::BIT_3_E:
			bit(e, 3);
			break;
		case alu_operation::BIT_3_H:
			bit(h, 3);
			break;
		case alu_operation::BIT_3_L:
			bit(l, 3);
			break;
		case alu_operation::BIT_3_Z:
			bit(z, 3);
			break;
		case alu_operation::BIT_4_A:
			bit(a, 4);
			break;
		case alu_operation::BIT_4_B:
			bit(b, 4);
			break;
		case alu_operation::BIT_4_C:
			bit(c, 4);
			break;
		case alu_operation::BIT_4_D:
			bit(d, 4);
			break;
		case alu_operation::BIT_4_E:
			bit(e, 4);
			break;
		case alu_operation::BIT_4_H:
			bit(h, 4);
			break;
		case alu_operation::BIT_4_L:
			bit(l, 4);
			break;
		case alu_operation::BIT_4_Z:
			bit(z, 4);
			break;
		case alu_operation::BIT_5_A:
			bit(a, 5);
			break;
		case alu_operation::BIT_5_B:
			bit(b, 5);
			break;
		case alu_operation::BIT_5_C:
			bit(c, 5);
			break;
		case alu_operation::BIT_5_D:
			bit(d, 5);
			break;
		case alu_operation::BIT_5_E:
			bit(e, 5);
			break;
		case alu_operation::BIT_5_H:
			bit(h, 5);
			break;
		case alu_operation::BIT_5_L:
			bit(l, 5);
			break;
		case alu_operation::BIT_5_Z:
			bit(z, 5);
			break;
		case alu_operation::BIT_6_A:
			bit(a, 6);
			break;
		case alu_operation::BIT_6_B:
			bit(b, 6);
			break;
		case alu_operation::BIT_6_C:
			bit(c, 6);
			break;
		case alu_operation::BIT_6_D:
			bit(d, 6);
			break;
		case alu_operation::BIT_6_E:
			bit(e, 6);
			break;
		case alu_operation::BIT_6_H:
			bit(h, 6);
			break;
		case alu_operation::BIT_6_L:
			bit(l, 6);
			break;
		case alu_operation::BIT_6_Z:
			bit(z, 6);
			break;
		case alu_operation::BIT_7_A:
			bit(a, 7);
			break;
		case alu_operation::BIT_7_B:
			bit(b, 7);
			break;
		case alu_operation::BIT_7_C:
			bit(c, 7);
			break;
		case alu_operation::BIT_7_D:
			bit(d, 7);
			break;
		case alu_operation::BIT_7_E:
			bit(e, 7);
			break;
		case alu_operation::BIT_7_H:
			bit(h, 7);
			break;
		case alu_operation::BIT_7_L:
			bit(l, 7);
			break;
		case alu_operation::BIT_7_Z:
			bit(z, 7);
			break;
		case alu_operation::RES_0_A:
			a &= ~1;
			break;
		case alu_operation::RES_0_B:
			b &= ~1;
			break;
		case alu_operation::RES_0_C:
			c &= ~1;
			break;
		case alu_operation::RES_0_D:
			d &= ~1;
			break;
		case alu_operation::RES_0_E:
			e &= ~1;
			break;
		case alu_operation::RES_0_H:
			h &= ~1;
			break;
		case alu_operation::RES_0_L:
			l &= ~1;
			break;
		case alu_operation::RES_0_Z:
			z &= ~1;
			break;
		case alu_operation::RES_1_A:
			a &= ~2;
			break;
		case alu_operation::RES_1_B:
			b &= ~2;
			break;
		case alu_operation::RES_1_C:
			c &= ~2;
			break;
		case alu_operation::RES_1_D:
			d &= ~2;
			break;
		case alu_operation::RES_1_E:
			e &= ~2;
			break;
		case alu_operation::RES_1_H:
			h &= ~2;
			break;
		case alu_operation::RES_1_L:
			l &= ~2;
			break;
		case alu_operation::RES_1_Z:
			z &= ~2;
			break;
		case alu_operation::RES_2_A:
			a &= ~4;
			break;
		case alu_operation::RES_2_B:
			b &= ~4;
			break;
		case alu_operation::RES_2_C:
			c &= ~4;
			break;
		case alu_operation::RES_2_D:
			d &= ~4;
			break;
		case alu_operation::RES_2_E:
			e &= ~4;
			break;
		case alu_operation::RES_2_H:
			h &= ~4;
			break;
		case alu_operation::RES_2_L:
			l &= ~4;
			break;
		case alu_operation::RES_2_Z:
			z &= ~4;
			break;
		case alu_operation::RES_3_A:
			a &= ~8;
			break;
		case alu_operation::RES_3_B:
			b &= ~8;
			break;
		case alu_operation::RES_3_C:
			c &= ~8;
			break;
		case alu_operation::RES_3_D:
			d &= ~8;
			break;
		case alu_operation::RES_3_E:
			e &= ~8;
			break;
		case alu_operation::RES_3_H:
			h &= ~8;
			break;
		case alu_operation::RES_3_L:
			l &= ~8;
			break;
		case alu_operation::RES_3_Z:
			z &= ~8;
			break;
		case alu_operation::RES_4_A:
			a &= ~16;
			break;
		case alu_operation::RES_4_B:
			b &= ~16;
			break;
		case alu_operation::RES_4_C:
			c &= ~16;
			break;
		case alu_operation::RES_4_D:
			d &= ~16;
			break;
		case alu_operation::RES_4_E:
			e &= ~16;
			break;
		case alu_operation::RES_4_H:
			h &= ~16;
			break;
		case alu_operation::RES_4_L:
			l &= ~16;
			break;
		case alu_operation::RES_4_Z:
			z &= ~16;
			break;
		case alu_operation::RES_5_A:
			a &= ~32;
			break;
		case alu_operation::RES_5_B:
			b &= ~32;
			break;
		case alu_operation::RES_5_C:
			c &= ~32;
			break;
		case alu_operation::RES_5_D:
			d &= ~32;
			break;
		case alu_operation::RES_5_E:
			e &= ~32;
			break;
		case alu_operation::RES_5_H:
			h &= ~32;
			break;
		case alu_operation::RES_5_L:
			l &= ~32;
			break;
		case alu_operation::RES_5_Z:
			z &= ~32;
			break;
		case alu_operation::RES_6_A:
			a &= ~64;
			break;
		case alu_operation::RES_6_B:
			b &= ~64;
			break;
		case alu_operation::RES_6_C:
			c &= ~64;
			break;
		case alu_operation::RES_6_D:
			d &= ~64;
			break;
		case alu_operation::RES_6_E:
			e &= ~64;
			break;
		case alu_operation::RES_6_H:
			h &= ~64;
			break;
		case alu_operation::RES_6_L:
			l &= ~64;
			break;
		case alu_operation::RES_6_Z:
			z &= ~64;
			break;
		case alu_operation::RES_7_A:
			a &= ~128;
			break;
		case alu_operation::RES_7_B:
			b &= ~128;
			break;
		case alu_operation::RES_7_C:
			c &= ~128;
			break;
		case alu_operation::RES_7_D:
			d &= ~128;
			break;
		case alu_operation::RES_7_E:
			e &= ~128;
			break;
		case alu_operation::RES_7_H:
			h &= ~128;
			break;
		case alu_operation::RES_7_L:
			l &= ~128;
			break;
		case alu_operation::RES_7_Z:
			z &= ~128;
			break;
		case alu_operation::SET_0_A:
			a |= 1;
			break;
		case alu_operation::SET_0_B:
			b |= 1;
			break;
		case alu_operation::SET_0_C:
			c |= 1;
			break;
		case alu_operation::SET_0_D:
			d |= 1;
			break;
		case alu_operation::SET_0_E:
			e |= 1;
			break;
		case alu_operation::SET_0_H:
			h |= 1;
			break;
		case alu_operation::SET_0_L:
			l |= 1;
			break;
		case alu_operation::SET_0_Z:
			z |= 1;
			break;
		case alu_operation::SET_1_A:
			a |= 2;
			break;
		case alu_operation::SET_1_B:
			b |= 2;
			break;
		case alu_operation::SET_1_C:
			c |= 2;
			break;
		case alu_operation::SET_1_D:
			d |= 2;
			break;
		case alu_operation::SET_1_E:
			e |= 2;
			break;
		case alu_operation::SET_1_H:
			h |= 2;
			break;
		case alu_operation::SET_1_L:
			l |= 2;
			break;
		case alu_operation::SET_1_Z:
			z |= 2;
			break;
		case alu_operation::SET_2_A:
			a |= 4;
			break;
		case alu_operation::SET_2_B:
			b |= 4;
			break;
		case alu_operation::SET_2_C:
			c |= 4;
			break;
		case alu_operation::SET_2_D:
			d |= 4;
			break;
		case alu_operation::SET_2_E:
			e |= 4;
			break;
		case alu_operation::SET_2_H:
			h |= 4;
			break;
		case alu_operation::SET_2_L:
			l |= 4;
			break;
		case alu_operation::SET_2_Z:
			z |= 4;
			break;
		case alu_operation::SET_3_A:
			a |= 8;
			break;
		case alu_operation::SET_3_B:
			b |= 8;
			break;
		case alu_operation::SET_3_C:
			c |= 8;
			break;
		case alu_operation::SET_3_D:
			d |= 8;
			break;
		case alu_operation::SET_3_E:
			e |= 8;
			break;
		case alu_operation::SET_3_H:
			h |= 8;
			break;
		case alu_operation::SET_3_L:
			l |= 8;
			break;
		case alu_operation::SET_3_Z:
			z |= 8;
			break;
		case alu_operation::SET_4_A:
			a |= 16;
			break;
		case alu_operation::SET_4_B:
			b |= 16;
			break;
		case alu_operation::SET_4_C:
			c |= 16;
			break;
		case alu_operation::SET_4_D:
			d |= 16;
			break;
		case alu_operation::SET_4_E:
			e |= 16;
			break;
		case alu_operation::SET_4_H:
			h |= 16;
			break;
		case alu_operation::SET_4_L:
			l |= 16;
			break;
		case alu_operation::SET_4_Z:
			z |= 16;
			break;
		case alu_operation::SET_5_A:
			a |= 32;
			break;
		case alu_operation::SET_5_B:
			b |= 32;
			break;
		case alu_operation::SET_5_C:
			c |= 32;
			break;
		case alu_operation::SET_5_D:
			d |= 32;
			break;
		case alu_operation::SET_5_E:
			e |= 32;
			break;
		case alu_operation::SET_5_H:
			h |= 32;
			break;
		case alu_operation::SET_5_L:
			l |= 32;
			break;
		case alu_operation::SET_5_Z:
			z |= 32;
			break;
		case alu_operation::SET_6_A:
			a |= 64;
			break;
		case alu_operation::SET_6_B:
			b |= 64;
			break;
		case alu_operation::SET_6_C:
			c |= 64;
			break;
		case alu_operation::SET_6_D:
			d |= 64;
			break;
		case alu_operation::SET_6_E:
			e |= 64;
			break;
		case alu_operation::SET_6_H:
			h |= 64;
			break;
		case alu_operation::SET_6_L:
			l |= 64;
			break;
		case alu_operation::SET_6_Z:
			z |= 64;
			break;
		case alu_operation::SET_7_A:
			a |= 128;
			break;
		case alu_operation::SET_7_B:
			b |= 128;
			break;
		case alu_operation::SET_7_C:
			c |= 128;
			break;
		case alu_operation::SET_7_D:
			d |= 128;
			break;
		case alu_operation::SET_7_E:
			e |= 128;
			break;
		case alu_operation::SET_7_H:
			h |= 128;
			break;
		case alu_operation::SET_7_L:
			l |= 128;
			break;
		case alu_operation::SET_7_Z:
			z |= 128;
			break;
		default:
			throw "Not implemented";
		}
	}
}

uint8_t sm83::inc(const uint8_t value)
{
	const uint8_t result = value + 1;
	f &= ~(flags::NF | flags::HF | flags::ZF);
	if (result == 0)
		f |= flags::ZF;
	if ((result & 0x0f) == 0x00)
		f |= flags::HF;
	return result;
}

uint8_t sm83::dec(const uint8_t value)
{
	const uint8_t result = value - 1;
	f &= ~(flags::HF | flags::ZF);
	f |= flags::NF;
	if (result == 0)
		f |= flags::ZF;
	if ((result & 0x0f) == 0x0f)
		f |= flags::HF;
	return result;
}

uint8_t sm83::rlc(const uint8_t value)
{
	const uint8_t result = (value << 1) | (value >> 7);
	f = (value & 0x80) ? flags::CF : 0;
	if (result == 0)
		f |= flags::ZF;
	return result;
}

uint8_t sm83::rl(const uint8_t value)
{
	const uint8_t result = (value << 1) | ((f & flags::CF) ? 0x01 : 0);
	f = (value & 0x80) ? sm83::flags::CF : 0;
	if (result == 0)
		f |= flags::ZF;
	return result;
}

uint8_t sm83::rr(const uint8_t value)
{
	const uint8_t result = (value >> 1) | ((f & flags::CF) ? 0x80 : 0);
	f = (value & 0x01) ? flags::CF : 0;
	if (result == 0)
		f |= flags::ZF;
	return result;
}

uint8_t sm83::rrc(const uint8_t value)
{
	const uint8_t result = (value >> 1) | ((value & 1) ? 0x80 : 0);
	f = (value & 0x01) ? flags::CF : 0;
	if (result == 0)
		f |= flags::ZF;
	return result;
}

uint8_t sm83::srl(const uint8_t value)
{
	const uint8_t result = value >> 1;
	f = (value & 0x01) ? sm83::flags::CF : 0;
	if (result == 0)
		f |= flags::ZF;
	return result;
}

uint8_t sm83::sla(const uint8_t value)
{
	const uint8_t result = value << 1;
	f = (value & 0x80) ? sm83::flags::CF : 0;
	if (result == 0)
		f |= flags::ZF;
	return result;
}

uint8_t sm83::sra(const uint8_t value)
{
	const uint8_t result = value >> 1 | (value & 0x80);
	f = (value & 0x01) ? sm83::flags::CF : 0;
	if (result == 0)
		f |= flags::ZF;
	return result;
}

uint8_t sm83::add(const uint8_t value, const uint8_t value2)
{
	const uint16_t sum = value2 + value;
	const uint16_t carryIns = sum ^ value2 ^ value;

	f &= ~(flags::NF | flags::HF | flags::CF);
	if (sum > 0xff)
		f |= flags::CF;
	if (carryIns & 0x10)
		f |= flags::HF;
	return sum & 0xff;
}

void sm83::add(const uint8_t value)
{
	a = add(value, a);
	f &= ~flags::ZF;
	if (a == 0)
		f |= flags::ZF;
}

uint8_t sm83::adc(const uint8_t value, const uint8_t value2)
{
	const uint8_t flag = (f & flags::CF) ? 1 : 0;
	const uint16_t sum = value2 + value + flag;
	const uint16_t carryIns = sum ^ value2 ^ value;

	f &= ~(flags::NF | flags::HF | flags::CF);
	if (sum > 0xff)
		f |= flags::CF;
	if (carryIns & 0x10)
		f |= flags::HF;
	return sum & 0xff;
}

void sm83::adc(const uint8_t value)
{
	a = adc(value, a);
	f &= ~flags::ZF;
	if (a == 0)
		f |= flags::ZF;
}

void sm83::sub(const uint8_t value)
{
	const uint16_t sum = a - value;
	const uint16_t carryIns = (sum ^ a ^ value);

	a = sum & 0xff;
	f &= ~(flags::HF | flags::CF | flags::ZF);
	if (a == 0)
		f |= flags::ZF;
	f |= flags::NF;
	if (carryIns & 0x10)
		f |= flags::HF;
	if ((carryIns >> 8) & 0x1)
		f |= flags::CF;
}
void sm83::sbc(const uint8_t value)
{
	const uint8_t flag = (f & flags::CF) ? 1 : 0;
	const uint16_t sum = a - value - flag;
	const uint16_t carryIns = sum ^ a ^ value;

	a = sum & 0xff;

	f &= ~(flags::HF | flags::CF | flags::ZF);
	if (a == 0)
		f |= flags::ZF;
	f |= flags::NF;
	if (carryIns & 0x10)
		f |= flags::HF;
	if ((carryIns >> 8) & 0x1)
		f |= flags::CF;
}
void sm83::ana(const uint8_t value)
{
	a &= value;

	f = 0;
	if (a == 0)
		f |= flags::ZF;
	f |= flags::HF;
}
void sm83::xra(const uint8_t value)
{
	a ^= value;

	f = 0;
	if (a == 0)
		f |= flags::ZF;
}
void sm83::ora(const uint8_t value)
{
	a |= value;

	f = 0;
	if (a == 0)
		f |= flags::ZF;
}
void sm83::cp(const uint8_t value)
{
	const uint16_t sum = a - value;
	const uint16_t carryIns = (sum ^ a ^ value);

	f = flags::NF;
	if ((sum&0xff) == 0)
		f |= flags::ZF;

	if (carryIns & 0x10)
		f |= flags::HF;
	if ((carryIns >> 8) & 0x1)
		f |= flags::CF;
}

void sm83::daa()
{
	if (f & flags::NF)
	{
		if (f & flags::CF)
			a -= 0x60;
		if (f & flags::HF)
			a -= 0x06;
	}
	else
	{
		if ((f & flags::CF) || (a > 0x99))
		{
			a += 0x60;
			f |= flags::CF;
		}
		if ((f & flags::HF) || ((a & 0xf) > 9))
			a += 6;
	}
	f &= ~flags::HF;
	if (a == 0)
		f |= flags::ZF;
	else
		f &= ~flags::ZF;
}

uint8_t sm83::swap(const uint8_t value)
{
	const uint8_t result = (value << 4) | (value >> 4);
	f = (result) ? 0 : flags::ZF;
	return result;
}

void sm83::cpl()
{
	a = ~a;
	f |= (flags::HF | flags::NF);
}

void sm83::scf()
{
	f |= flags::CF;
	f &= ~(flags::NF | flags::HF);
}
void sm83::ccf()
{
	f &= ~(flags::NF | flags::HF);
	if (f & flags::CF)
	{
		f &= ~flags::CF;
	}
	else
	{
		f |= flags::CF;
	}
}

void sm83::bit(const uint8_t value, const uint8_t b)
{
	f &= sm83::flags::CF;
	f |= sm83::flags::HF;
	f |= (value & (1 << b)) ? 0 : sm83::flags::ZF;
}
