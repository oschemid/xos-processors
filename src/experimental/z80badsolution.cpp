#include "z80.h"


using namespace xprocessors::experimental;


std::function<void(Z80*)> Z80::decode_opcodes[] =
{
	// 0x00 - 0x0f
	&Z80::nop, // NOP
	[](Z80* t) { t->ld_rr_nn(t->bc); }, // LD BC,nn
	[](Z80* t) { t->ld_rr_r(t->bc, t->a); }, // LD (BC),A
	[](Z80* t) { t->inc(t->bc); }, // INC BC
	[](Z80* t) { t->inc(t->b); }, // INC B
	[](Z80* t) { t->dec(t->b); }, // DEC B
	[](Z80* t) { t->ld_r_n(t->b); }, // LD B,n
	[](Z80* t) { t->rla(true); }, // RLCA
	&Z80::ex_af, // EX AF,AF'
	[](Z80* t) { t->add_hl(t->bc); }, // ADD HL,BC
	[](Z80* t) { t->ld_r_rr(t->a, t->bc); }, // LD A, (BC)
	[](Z80* t) { t->dec(t->bc); }, // DEC BC
	[](Z80* t) { t->inc(t->c); }, // INC C
	[](Z80* t) { t->dec(t->c); }, // DEC C
	[](Z80* t) { t->ld_r_n(t->c); }, // LD C,n
	[](Z80* t) { t->rra(true); }, // RRCA

	// 0x10 - 0x1f
	&Z80::djnz, // DJNZ
	[](Z80* t) { t->ld_rr_nn(t->de); }, // LD DE, nn
	[](Z80* t) { t->ld_rr_r(t->de, t->a); }, // LD (DE),A
	[](Z80* t) { t->inc(t->de); }, // INC DE
	[](Z80* t) { t->inc(t->d); }, // INC D
	[](Z80* t) { t->dec(t->d); }, // DEC D
	[](Z80* t) { t->ld_r_n(t->d); }, // LD D,n
	[](Z80* t) { t->rla(false); }, // RLA
	[](Z80* t) { t->jr_c(true); }, // JR n
	[](Z80* t) { t->add_hl(t->de); }, // ADD HL,DE
	[](Z80* t) { t->ld_r_rr(t->a, t->de); }, // LD A, (DE)
	[](Z80* t) { t->dec(t->de); }, // DEC DE
	[](Z80* t) { t->inc(t->e); }, // INC E
	[](Z80* t) { t->dec(t->e); }, // DEC E
	[](Z80* t) { t->ld_r_n(t->e); }, // LD E,n
	[](Z80* t) { t->rra(false); }, // RRA

	// 0x20 - 0x2f
	[](Z80* t) { t->jr_c((t->f & Z80::flags::ZF)? false : true); }, // JR NZ,n
	[](Z80* t) { t->ld_rr_nn(t->hli[t->hli_idx].hl); }, // LD HL,nn LD IX,nn LD IY,nn
	&Z80::ld_nn_hl, // LD (nn),HL LD (nn),IX LD (nn),IY
	[](Z80* t) { t->inc(t->hli[t->hli_idx].hl); }, // INC HL INC IX INC IY
	[](Z80* t) { t->inc(t->hli[t->hli_idx].h); }, // INC H INC IXh INX IYh
	[](Z80* t) { t->dec(t->hli[t->hli_idx].h); }, // DEC H DEC IXh DEC IYh
	[](Z80* t) { t->ld_r_n(t->hli[t->hli_idx].h); }, // LD H,n LD IXh,n LD IYh,n
	&Z80::daa, // DAA
	[](Z80* t) { t->jr_c(t->f & Z80::flags::ZF); }, // JR Z,n
	[](Z80* t) { t->add_hl(t->hli[t->hli_idx].hl); }, // ADD HL,HL ADD IX,IX ADD IY,IY
	&Z80::ld_hl_nn, // LD HL, (nn) LD IX,(nn) LD IY,(nn)
	[](Z80* t) { t->dec(t->hli[t->hli_idx].hl); }, // DEC HL DEC IX DEC IY
	[](Z80* t) { t->inc(t->hli[t->hli_idx].l); }, // INC L INC IXl INC IYl
	[](Z80* t) { t->dec(t->hli[t->hli_idx].l); }, // DEC L DEC IXl DEC IYl
	[](Z80* t) { t->ld_r_n(t->hli[t->hli_idx].l); }, // LD L,n LD IXl,n LD IYl,n
	&Z80::cpl, // CPL

	// 0x30 - 0x3f
	[](Z80* t) { t->jr_c((t->f & Z80::flags::CF) ? false : true); }, // JR NC,n
	[](Z80* t) { t->ld_rr_nn(t->sp); }, // LD SP, nn
	&Z80::ld_nn_a, // LD (nn),A
	[](Z80* t) { t->inc(t->sp); }, // INC SP
	&Z80::inc_hl, // INC (HL)
	&Z80::dec_hl, // DEC (HL)
	&Z80::ld_hl_n, // LD (HL),n
	&Z80::scf, // SCF
	[](Z80* t) { t->jr_c(t->f & Z80::flags::CF); }, // JR C,n
	[](Z80* t) { t->add_hl(t->sp); }, // ADD HL,SP
	&Z80::ld_a_nn, // LD A,(nn)
	[](Z80* t) { t->dec(t->sp); }, // DEC SP
	[](Z80* t) { t->inc(t->a); }, // INC A
	[](Z80* t) { t->dec(t->a); }, // DEC A
	[](Z80* t) { t->ld_r_n(t->a); }, // LD A,n
	&Z80::ccf, // CCF

	// 0x40 - 0x4f
[](Z80* t) { t->ld_r_r(t->b, t->b); }, // LD B,B
[](Z80* t) { t->ld_r_r(t->b, t->c); }, // LD B,C
[](Z80* t) { t->ld_r_r(t->b, t->d); }, // LD B,D
[](Z80* t) { t->ld_r_r(t->b, t->e); }, // LD B,E
[](Z80* t) { t->ld_r_r(t->b, t->hli[t->hli_idx].h); }, // LD B,H
[](Z80* t) { t->ld_r_r(t->b, t->hli[t->hli_idx].l); }, // LD B,L
[](Z80* t) { t->ld_r_hl(t->b); }, // LD B,(HL)
[](Z80* t) { t->ld_r_r(t->b, t->a); }, // LD B,A
[](Z80* t) { t->ld_r_r(t->c, t->b); }, // LD C,B
[](Z80* t) { t->ld_r_r(t->c, t->c); }, // LD C,C
[](Z80* t) { t->ld_r_r(t->c, t->d); }, // LD C,D
[](Z80* t) { t->ld_r_r(t->c, t->e); }, // LD C,E
[](Z80* t) { t->ld_r_r(t->c, t->hli[t->hli_idx].h); }, // LD C,H
[](Z80* t) { t->ld_r_r(t->c, t->hli[t->hli_idx].l); }, // LD C,L
[](Z80* t) { t->ld_r_hl(t->c); }, // LD C,(HL)
[](Z80* t) { t->ld_r_r(t->c, t->a); }, // LD C,A

// 0x50 - 0x5f
[](Z80* t) { t->ld_r_r(t->d, t->b); }, // LD D,B
[](Z80* t) { t->ld_r_r(t->d, t->c); }, // LD D,C
[](Z80* t) { t->ld_r_r(t->d, t->d); }, // LD D,D
[](Z80* t) { t->ld_r_r(t->d, t->e); }, // LD D,E
[](Z80* t) { t->ld_r_r(t->d, t->hli[t->hli_idx].h); }, // LD D,H
[](Z80* t) { t->ld_r_r(t->d, t->hli[t->hli_idx].l); }, // LD D,L
[](Z80* t) { t->ld_r_hl(t->d); }, // LD D,(HL)
[](Z80* t) { t->ld_r_r(t->d, t->a); }, // LD D,A
[](Z80* t) { t->ld_r_r(t->e, t->b); }, // LD E,B
[](Z80* t) { t->ld_r_r(t->e, t->c); }, // LD E,C
[](Z80* t) { t->ld_r_r(t->e, t->d); }, // LD E,D
[](Z80* t) { t->ld_r_r(t->e, t->e); }, // LD E,E
[](Z80* t) { t->ld_r_r(t->e, t->hli[t->hli_idx].h); }, // LD E,H
[](Z80* t) { t->ld_r_r(t->e, t->hli[t->hli_idx].l); }, // LD E,L
[](Z80* t) { t->ld_r_hl(t->e); }, // LD E,(HL)
[](Z80* t) { t->ld_r_r(t->e, t->a); }, // LD E,A

// 0x60 - 0x6f
[](Z80* t) { t->ld_r_r(t->hli[t->hli_idx].h, t->b); }, // LD H,B
[](Z80* t) { t->ld_r_r(t->hli[t->hli_idx].h, t->c); }, // LD H,C
[](Z80* t) { t->ld_r_r(t->hli[t->hli_idx].h, t->d); }, // LD H,D
[](Z80* t) { t->ld_r_r(t->hli[t->hli_idx].h, t->e); }, // LD H,E
[](Z80* t) { t->ld_r_r(t->hli[t->hli_idx].h, t->hli[t->hli_idx].h); }, // LD H,H
[](Z80* t) { t->ld_r_r(t->hli[t->hli_idx].h, t->hli[t->hli_idx].l); }, // LD H,L
[](Z80* t) { t->ld_r_hl(t->h); }, // LD H,(HL)
[](Z80* t) { t->ld_r_r(t->hli[t->hli_idx].h, t->a); }, // LD H,A
[](Z80* t) { t->ld_r_r(t->hli[t->hli_idx].l, t->b); }, // LD L,B
[](Z80* t) { t->ld_r_r(t->hli[t->hli_idx].l, t->c); }, // LD L,C
[](Z80* t) { t->ld_r_r(t->hli[t->hli_idx].l, t->d); }, // LD L,D
[](Z80* t) { t->ld_r_r(t->hli[t->hli_idx].l, t->e); }, // LD L,E
[](Z80* t) { t->ld_r_r(t->hli[t->hli_idx].l, t->hli[t->hli_idx].h); }, // LD L,H
[](Z80* t) { t->ld_r_r(t->hli[t->hli_idx].l, t->hli[t->hli_idx].l); }, // LD L,L
[](Z80* t) { t->ld_r_hl(t->l); }, // LD L,(HL)
[](Z80* t) { t->ld_r_r(t->hli[t->hli_idx].l, t->a); }, // LD L,A

// 0x70 - 0x7f
[](Z80* t) { t->ld_hl_r(t->b); }, // LD (HL),B
[](Z80* t) { t->ld_hl_r(t->c); }, // LD (HL),C
[](Z80* t) { t->ld_hl_r(t->d); }, // LD (HL),D
[](Z80* t) { t->ld_hl_r(t->e); }, // LD (HL),E
[](Z80* t) { t->ld_hl_r(t->h); }, // LD (HL),H
[](Z80* t) { t->ld_hl_r(t->l); }, // LD (HL),L
& Z80::halt, // HALT
[](Z80* t) { t->ld_hl_r(t->a); }, // LD (HL),A
[](Z80* t) { t->ld_r_r(t->a, t->b); }, // LD A,B
[](Z80* t) { t->ld_r_r(t->a, t->c); }, // LD A,C
[](Z80* t) { t->ld_r_r(t->a, t->d); }, // LD A,D
[](Z80* t) { t->ld_r_r(t->a, t->e); }, // LD A,E
[](Z80* t) { t->ld_r_r(t->a, t->hli[t->hli_idx].h); }, // LD A,H
[](Z80* t) { t->ld_r_r(t->a, t->hli[t->hli_idx].l); }, // LD A,L
[](Z80* t) { t->ld_r_hl(t->a); }, // LD A,(HL)
[](Z80* t) { t->ld_r_r(t->a, t->a); }, // LD A,A

// 0x80 - 0x8f
[](Z80* t) { t->add(t->b); }, // ADD B
[](Z80* t) { t->add(t->c); }, // ADD C
[](Z80* t) { t->add(t->d); }, // ADD D
[](Z80* t) { t->add(t->e); }, // ADD E
[](Z80* t) { t->add(t->hli[t->hli_idx].h); }, // ADD H
[](Z80* t) { t->add(t->hli[t->hli_idx].l); }, // ADD L
[](Z80* t) { t->read_hli([](Z80* t) {
	t->step = [](Z80* t) {
		t->read(t->buffer16, [](Z80* t) {
			t->add(t->databus);
			});
		};
	});
	}, // ADD (HL)
	[](Z80* t) { t->add(t->a); }, // ADD A
	[](Z80* t) { t->adc(t->b); }, // ADC B
	[](Z80* t) { t->adc(t->c); }, // ADC C
	[](Z80* t) { t->adc(t->d); }, // ADC D
	[](Z80* t) { t->adc(t->e); }, // ADC E
	[](Z80* t) { t->adc(t->hli[t->hli_idx].h); }, // ADC H
	[](Z80* t) { t->adc(t->hli[t->hli_idx].l); }, // ADC L
	[](Z80* t) { t->read_hli([](Z80* t) {
	t->step = [](Z80* t) {
		t->read(t->buffer16, [](Z80* t) {
			t->adc(t->databus);
			});
		};
		});
	}, // ADC (HL)
	[](Z80* t) { t->adc(t->a); }, // ADC A

	// 0x90 - 0x9f
	[](Z80* t) { t->sub(t->b); }, // SUB B
	[](Z80* t) { t->sub(t->c); }, // SUB C
	[](Z80* t) { t->sub(t->d); }, // SUB D
	[](Z80* t) { t->sub(t->e); }, // SUB E
	[](Z80* t) { t->sub(t->hli[t->hli_idx].h); }, // SUB H
	[](Z80* t) { t->sub(t->hli[t->hli_idx].l); }, // SUB L
	[](Z80* t) { t->read_hli([](Z80* t) {
	t->step = [](Z80* t) {
		t->read(t->buffer16, [](Z80* t) {
			t->sub(t->databus);
			});
		};
		});
	}, // SUB (HL)
	[](Z80* t) { t->sub(t->a); }, // SUB A
	[](Z80* t) { t->sbc(t->b); }, // SBC B
	[](Z80* t) { t->sbc(t->c); }, // SBC C
	[](Z80* t) { t->sbc(t->d); }, // SBC D
	[](Z80* t) { t->sbc(t->e); }, // SBC E
	[](Z80* t) { t->sbc(t->hli[t->hli_idx].h); }, // SBC H
	[](Z80* t) { t->sbc(t->hli[t->hli_idx].l); }, // SBC L
	[](Z80* t) { t->read_hli([](Z80* t) {
	t->step = [](Z80* t) {
		t->read(t->buffer16, [](Z80* t) {
			t->sbc(t->databus);
			});
		};
		});
	}, // SBC (HL)
	[](Z80* t) { t->sbc(t->a); }, // SBC A

	// 0xa0 - 0xaf
	[](Z80* t) { t->ana(t->b); }, // AND B
	[](Z80* t) { t->ana(t->c); }, // AND C
	[](Z80* t) { t->ana(t->d); }, // AND D
	[](Z80* t) { t->ana(t->e); }, // AND E
	[](Z80* t) { t->ana(t->hli[t->hli_idx].h); }, // AND H
	[](Z80* t) { t->ana(t->hli[t->hli_idx].l); }, // AND L
	[](Z80* t) { t->read_hli([](Z80* t) {
	t->step = [](Z80* t) {
		t->read(t->buffer16, [](Z80* t) {
			t->ana(t->databus); });
		};
		});
	}, // AND (HL)
	[](Z80* t) { t->ana(t->a); }, // AND A
	[](Z80* t) { t->xra(t->b); }, // XOR B
	[](Z80* t) { t->xra(t->c); }, // XOR C
	[](Z80* t) { t->xra(t->d); }, // XOR D
	[](Z80* t) { t->xra(t->e); }, // XOR E
	[](Z80* t) { t->xra(t->hli[t->hli_idx].h); }, // XOR H
	[](Z80* t) { t->xra(t->hli[t->hli_idx].l); }, // XOR L
	[](Z80* t) { t->read_hli([](Z80* t) {
	t->step = [](Z80* t) {
		t->read(t->buffer16, [](Z80* t) {
			t->xra(t->databus);
			}); };
		});
	}, // XOR (HL)
	[](Z80* t) { t->xra(t->a); }, // XOR A

	// 0xb0 - 0xbf
	[](Z80* t) { t->ora(t->b); }, // OR B
	[](Z80* t) { t->ora(t->c); }, // OR C
	[](Z80* t) { t->ora(t->d); }, // OR D
	[](Z80* t) { t->ora(t->e); }, // OR E
	[](Z80* t) { t->ora(t->hli[t->hli_idx].h); }, // OR H
	[](Z80* t) { t->ora(t->hli[t->hli_idx].l); }, // OR L
	[](Z80* t) { t->read_hli([](Z80* t) {
	t->step = [](Z80* t) {
		t->read(t->buffer16, [](Z80* t) {
			t->ora(t->databus);
			});
		};
		});
	}, // OR (HL)
	[](Z80* t) { t->ora(t->a); }, // OR A
	[](Z80* t) { t->cp(t->b); }, // CP B
	[](Z80* t) { t->cp(t->c); }, // CP C
	[](Z80* t) { t->cp(t->d); }, // CP D
	[](Z80* t) { t->cp(t->e); }, // CP E
	[](Z80* t) { t->cp(t->hli[t->hli_idx].h); }, // CP H
	[](Z80* t) { t->cp(t->hli[t->hli_idx].l); }, // CP L
	[](Z80* t) { t->read_hli([](Z80* t) {
		t->step = [](Z80* t) {
			t->read(t->buffer16, [](Z80* t) { t->cp(t->databus); });
			};
		});
		}, // CP (HL)
	[](Z80* t) { t->cp(t->a); }, // CP A

	// 0xc0 - 0xcf
	[](Z80* t) { t->ret_c((t->f & Z80::flags::ZF) ? false : true); }, // RET NZ
	[](Z80* t) { t->pop(t->bc); }, // POP BC
	[](Z80* t) { t->jp_c((t->f & Z80::flags::ZF)? false : true); }, // JP NZ,nn
	[](Z80* t) { t->jp_c(true); }, // JP nn
	[](Z80* t) { t->call((t->f & Z80::flags::ZF) ? false : true); }, // CALL NZ,nn
	[](Z80* t) { t->push(t->bc); }, // PUSH BC
	&Z80::add_n, // ADD A,n
	nullptr,
	[](Z80* t) { t->ret_c((t->f & Z80::flags::ZF) ? true : false); }, // RET Z
	&Z80::ret,
	[](Z80* t) { t->jp_c((t->f & Z80::flags::ZF)? true : false); }, // JP Z,nn
	&Z80::fetchCB,
	[](Z80* t) { t->call((t->f & Z80::flags::ZF) ? true : false); }, // CALL Z,nn
	[](Z80* t) { t->call(true); }, // CALL nn
	&Z80::adc_n, // ADC A,n
	nullptr,

	// 0xd0 - 0xdf
	[](Z80* t) { t->ret_c((t->f & Z80::flags::CF) ? false : true); }, // RET NC
	[](Z80* t) { t->pop(t->de); }, // POP DE
	[](Z80* t) { t->jp_c((t->f & Z80::flags::CF) ? false : true); }, // JP NC,nn
	&Z80::out_a,
	[](Z80* t) { t->call((t->f & Z80::flags::CF) ? false : true); }, // CALL NC,nn
	[](Z80* t) { t->push(t->de); }, // PUSH DE
	&Z80::sub_n, // SUB A,n
	nullptr,
	[](Z80* t) { t->ret_c((t->f & Z80::flags::CF) ? true : false); }, // RET C
	&Z80::exx, // EXX
	[](Z80* t) { t->jp_c((t->f & Z80::flags::CF) ? true : false); }, // JP C,nn
	nullptr,
	[](Z80* t) { t->call((t->f & Z80::flags::CF) ? true : false); }, // CALL C,nn
	&Z80::fetchDD,
	&Z80::sbc_n, // SBC A,n
	nullptr,

	// 0xe0 - 0xef
	[](Z80* t) { t->ret_c((t->f & Z80::flags::PF) ? false : true); }, // RET PO
	[](Z80* t) { t->pop(t->hli[t->hli_idx].hl); }, // POP HL IX IY
	[](Z80* t) { t->jp_c((t->f & Z80::flags::PF) ? false : true); }, // JP PO,nn
	&Z80::ex_sp_hl, // EX (SP),HL EX (SP),IX EX (SP),IY
	[](Z80* t) { t->call((t->f & Z80::flags::PF) ? false : true); }, // CALL PO,nn
	[](Z80* t) { t->push(t->hli[t->hli_idx].hl); }, // PUSH HL IX IY
	&Z80::and_n, // AND A,n
	nullptr,
	[](Z80* t) { t->ret_c((t->f & Z80::flags::PF) ? true : false); }, // RET PE
	&Z80::jp_hl, // JP (HL) JP(IX) JP(IY)
	[](Z80* t) { t->jp_c((t->f & Z80::flags::PF) ? true : false); }, // JP PE,nn
	&Z80::ex_de_hl, // EX DE,HL
	[](Z80* t) { t->call((t->f & Z80::flags::PF) ? true : false); }, // CALL PE,nn
	&Z80::fetchED,
	&Z80::xra_n, // XOR A,n
	nullptr,

	// 0xf0 - 0xff
	[](Z80* t) { t->ret_c((t->f & Z80::flags::SF) ? false : true); }, // RET P
	[](Z80* t) { t->pop(t->af); }, // POP AF
	[](Z80* t) { t->jp_c((t->f & Z80::flags::SF) ? false : true); }, // JP P,nn
	&Z80::di, // DI
	[](Z80* t) { t->call((t->f & Z80::flags::SF) ? false : true); }, // CALL P,nn
	[](Z80* t) { t->push(t->af); }, // PUSH AF
	&Z80::ora_n, // OR A,n
	nullptr,
	[](Z80* t) { t->ret_c((t->f & Z80::flags::SF) ? true : false); }, // RET M
	&Z80::ld_sp_hl, // LD SP,HL
	[](Z80* t) { t->jp_c((t->f & Z80::flags::SF) ? true : false); }, // JP M,nn
	&Z80::ei, // EI
	[](Z80* t) { t->call((t->f & Z80::flags::SF) ? true : false); }, // CALL M,nn
	&Z80::fetchFD,
	&Z80::cp_n,
	nullptr,
};


void Z80::init()
{
	reset();
}

void Z80::reset(const uint16_t start)
{
	pc = start;
	af = bc = de = hl = ix = iy = 0;
	hli_idx = 0;
	opcode = 0;
	addressbus = 0;
	databus = 0;
	pins = 0;
	step = &Z80::fetch;
	iff1 = iff1_requested = false;
}

void Z80::tick()
{
	step(this);
}

void Z80::_fetch(std::function<void(Z80*)>* opcodes)
{
	// Request opcode
	addressbus = pc++;
	pins |= PIN_MREQ | PIN_RD | PIN_M1;
	pins &= ~PIN_RFSH;

	step = [opcodes](Z80* t) {
		t->opcode = t->databus;
		t->pins &= ~(PIN_M1 | PIN_RD);
		t->pins |= PIN_RFSH;
		if (t->iff1_requested) {
			t->iff1 = true;
			t->iff1_requested = false;
		}
		t->step = [opcodes](Z80* t) {
			t->step = [opcodes](Z80* t) {
				t->pins &= ~PIN_RFSH;
				opcodes[t->opcode](t);
				};
			};
		};
}

void Z80::fetch()
{
	hli_idx = 0;
	if (pins &= PIN_HALT)
		return;
	_fetch(decode_opcodes);
}
void Z80::fetchDD()
{
	hli_idx = 1;
	step = [](Z80* t) {
		t->_fetch(Z80::decode_opcodes);
		};
}
void Z80::fetchFD()
{
	hli_idx = 2;
	step = [](Z80* t) {
		t->_fetch(Z80::decode_opcodes);
		};
}
void Z80::readArgs(std::function<void(Z80*)> cb)
{
	read(pc, [cb](Z80* t) { t->pc++; cb(t); });
}
void Z80::readArgs16(std::function<void(Z80*)> cb)
{
	readArgs([cb](Z80* t) {
		t->buffer16 = t->databus;
		t->step = [cb](Z80* t) {
			t->readArgs([cb](Z80* t) {
				t->buffer16 |= t->databus << 8;
				cb(t); });
			};
		});
}
void Z80::read(const uint16_t address, std::function<void(Z80*)> cb)
{
	addressbus = address;
	pins |= PIN_MREQ | PIN_RD;
	step = [cb](Z80* t) {
		t->pins &= ~(PIN_RD | PIN_MREQ);
		t->step = cb;
		};
}
void Z80::write(const uint16_t address, const uint8_t v, std::function<void(Z80*)> cb)
{
	addressbus = address;
	databus = v;
	pins |= PIN_MREQ;
	step = [cb](Z80* t) {
		t->pins |= PIN_WR;
		t->step = [cb](Z80* t) {
			t->pins &= ~(PIN_WR | PIN_MREQ);
			cb(t);
			};
		};
}
void Z80::iowrite(const uint16_t address, const uint8_t data, std::function<void(Z80*)> cb)
{
	addressbus = address;
	databus = data;
	pins |= PIN_IORQ | PIN_WR;
	step = [cb](Z80* t) {
		t->step = [cb](Z80* t) {
			t->pins &= ~(PIN_IORQ | PIN_WR);
			t->step = [cb](Z80* t) {
				t->step = cb;
			};
		};
	};
}
void Z80::wait(const uint8_t waiting, std::function<void(Z80*)> cb)
{
	if (waiting == 1) {
		step = cb;
	}
	else {
		step = [waiting, cb](Z80* t) { t->wait(waiting - 1, cb); };
	}
}
void Z80::read_hli(std::function<void(Z80*)> cb)
{
	if (hli_idx == 0)
	{
		buffer16 = hl;
		cb(this);
	}
	else
	{
		step = [cb](Z80* t) { t->readArgs([cb](Z80* t) {
			t->buffer16 = t->hli[t->hli_idx].hl + static_cast<int8_t>(t->databus);
			t->wait(5, cb); });
			};
	}
}
void Z80::read16(const uint16_t address, std::function<void(Z80*)> cb)
{
	buffer16 = 0;
	read(address, [address, cb](Z80* t) {
		t->buffer16 = t->databus;
		t->step = [address, cb](Z80* t) { t->read(address + 1, [cb](Z80* t) {
			t->buffer16 |= t->databus << 8;
			cb(t);
			});
			};
		});
}

// M1/T4 execution
void Z80::nop()
{
	step = &Z80::fetch;
}
void Z80::halt()
{
	pins |= PIN_HALT;
	step = &Z80::fetch;
}
void Z80::ld_r_rr(register8_t& r, const uint16_t rr)
{
	step = [&r, rr](Z80* t) { t->read(rr, [&r](Z80* t) {
		r = t->databus;
		t->step = &Z80::fetch; });
		};
}
void Z80::ei()
{
	iff1 = false;
	iff1_requested = true;
	step = &Z80::fetch;
}
void Z80::di()
{
	iff1 = false;
	step = &Z80::fetch;
}
void Z80::ld_rr_nn(register16_t& rr)
{
	step = [&rr](Z80* t) { t->readArgs16([&rr](Z80* t) { rr = t->buffer16; t->step = &Z80::fetch; }); };
}
void Z80::ld_a_nn()
{
	step = [](Z80* t) { t->readArgs16([](Z80* t) {
		t->step = std::bind(&Z80::read, t, t->buffer16, [](Z80* t) {
			t->a = t->databus;
			t->step = &Z80::fetch;
			});
		});
		};
}
void Z80::ld_hl_nn()
{
	step = [](Z80* t) { t->readArgs16([](Z80* t) {
		t->step = std::bind(&Z80::read, t, t->buffer16, [](Z80* t) {
			t->hli[t->hli_idx].l = t->databus;
			t->step = std::bind(&Z80::read, t, t->buffer16 + 1, [](Z80* t) {
				t->hli[t->hli_idx].h = t->databus;
				t->step = &Z80::fetch;
				});
			});
		});
	};
}
void Z80::inc(register8_t& r)
{
	r = increment(r);
	step = &Z80::fetch;
}
void Z80::inc(register16_t& rr)
{
	step = [&rr](Z80* t) { rr++; t->step = [](Z80* t) { t->step = &Z80::fetch; }; };
}
void Z80::inc_hl()
{
	read_hli([](Z80* t) { t->step = [](Z80* t) { t->read(t->buffer16, [](Z80* t) {
		auto tmp = t->increment(t->databus);
		t->step = [tmp](Z80* t) {
			t->step = [tmp](Z80* t) {
				t->write(t->buffer16, tmp, [](Z80* t) {
					t->step = &Z80::fetch;
					});
				};
			};
		});
		};
	});
}
void Z80::dec_hl()
{
	read_hli([](Z80* t) { t->step = std::bind(&Z80::read, t, t->buffer16, [](Z80* t) {
		auto tmp = t->decrement(t->databus);
		t->step = [tmp](Z80* t) { t->step = std::bind(&Z80::write, t, t->buffer16, tmp, [](Z80* t) { t->step = &Z80::fetch; }); }; }); });
}
void Z80::dec(register8_t& r)
{
	r = decrement(r);
	step = &Z80::fetch;
}
void Z80::dec(register16_t& rr)
{
	step = [&rr](Z80* t) { rr--; t->step = [](Z80* t) { t->step = &Z80::fetch; }; };
}
void Z80::add(const register8_t value)
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
	step = &Z80::fetch;
}
void Z80::add_n()
{
	step = std::bind(&Z80::readArgs, this, [](Z80* t) { t->add(t->databus); });
}
void Z80::add_hl(const register16_t rr)
{
	step = [rr](Z80* t) {
		t->step = [rr](Z80* t) {
			t->step = [rr](Z80* t) {
				t->step = [rr](Z80* t) {
					t->step = [rr](Z80* t) {
						t->step = [rr](Z80* t) {
							t->step = [rr](Z80* t) {
								const uint16_t hl = t->hli[t->hli_idx].hl;
								t->hli[t->hli_idx].hl += rr;
								t->f &= ~(Z80::flags::NF | Z80::flags::CF | Z80::flags::HF);
								if (rr > 0xffff - hl)
									t->f |= Z80::flags::CF;
								if (((t->hli[t->hli_idx].hl ^ hl ^ rr) >> 11) & 0x1)
									t->f |= Z80::flags::HF;
								t->step = &Z80::fetch;
								};
							};
						};
					};
				};
			};
		};
}

void Z80::adc(const register8_t value)
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
	step = &Z80::fetch;
}
void Z80::adc_n()
{
	step = std::bind(&Z80::readArgs, this, [](Z80* t) { t->adc(t->databus); });
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

	step = &Z80::fetch;
}
void Z80::sub_n()
{
	step = std::bind(&Z80::readArgs, this, [](Z80* t) { t->sub(t->databus); });
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

	step = &Z80::fetch;
}
void Z80::sbc_n()
{
	step = std::bind(&Z80::readArgs, this, [](Z80* t) { t->sbc(t->databus); });
}

void Z80::cp(const register8_t value, std::function<void(Z80*)> cb)
{
	const uint16_t sum = a - value;
	const uint16_t carryIns = (sum ^ a ^ value);

	f = flags::NF;
	if (sum == 0)
		f |= flags::ZF;
	else
		f |= sum & flags::SF;
	f |= value & (flags::XF | flags::YF);

	if (carryIns & 0x10)
		f |= flags::HF;
	if (((carryIns >> 7) & 0x1) ^ ((carryIns >> 8) & 0x1))
		f |= flags::PF;
	if ((carryIns >> 8) & 0x1)
		f |= flags::CF;

	step = cb;
}
void Z80::cp_n()
{
	step = std::bind(&Z80::readArgs, this, [](Z80* t) { t->cp(t->databus); });
}
void Z80::and_n() {
	step = std::bind(&Z80::readArgs, this, [](Z80* t) { t->ana(t->databus); });
}
void Z80::ana(const register8_t value) {
	a &= value;

	f = 0;
	setSZXY(a);
	setP(a);
	f |= flags::HF;

	step = &Z80::fetch;
}
void Z80::ora_n() {
	step = std::bind(&Z80::readArgs, this, [](Z80* t) { t->ora(t->databus); });
}
void Z80::ora(const register8_t value) {
	a |= value;

	f = 0;
	setSZXY(a);
	setP(a);

	step = &Z80::fetch;
}
void Z80::xra_n() {
	step = std::bind(&Z80::readArgs, this, [](Z80* t) { t->xra(t->databus); });
}
void Z80::xra(const uint8_t value) {
	a ^= value;

	f = 0;
	setSZXY(a);
	setP(a);

	step = &Z80::fetch;
}
void Z80::rla(const bool c) {
	const bool carry = (c) ? a & 0x80 : f & flags::CF;

	f &= ~(flags::NF | flags::HF | flags::CF);
	if (a & 0x80)
		f |= flags::CF;
	a <<= 1;
	a |= (carry) ? 1 : 0;

	step = &Z80::fetch;
}
void Z80::rra(const bool c) {
	const bool carry = (c) ? a & 0x01 : f & flags::CF;

	f &= ~(flags::NF | flags::HF | flags::CF);
	if (a & 0x01)
		f |= flags::CF;
	a >>= 1;
	a |= (carry) ? 0x80 : 0;

	step = &Z80::fetch;
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
			f = ~flags::HF;
		a += diff;
	}
	setSZXY(a);
	setP(a);
	f |= carry;

	step = &Z80::fetch;
}

void Z80::cpl() {
	a = ~a;
	f |= (flags::HF | flags::NF);
	step = &Z80::fetch;
}
void Z80::scf() {
	f |= flags::CF;
	f &= ~(flags::NF | flags::XF | flags::YF | flags::HF);
	f |= a & (flags::XF | flags::YF);

	step = &Z80::fetch;
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

	step = &Z80::fetch;
}

void Z80::djnz() {
	step = [](Z80* t) {
		if (--t->b == 0) {
			t->step = std::bind(&Z80::read, t, t->pc++, [](Z80* t) { t->step = &Z80::fetch; });
		}
		else
		{
			t->jr_c(true);
		}
		};
}
void Z80::call(const bool flag)
{
	step = std::bind(&Z80::readArgs16, this, [flag](Z80* t) {
		if (flag) {
			t->push(t->pc);
			t->pc = t->buffer16;
		}
		else {
			t->step = &Z80::fetch;
		}
		});
}
void Z80::ret()
{
	pop(pc);
}
void Z80::ret_c(const bool flag)
{
	step = [flag](Z80* t) { if (flag) t->pop(t->pc); else t->step = &Z80::fetch; };
}
void Z80::out_a()
{
	step = std::bind(&Z80::readArgs, this, [](Z80* t) {
		t->step = std::bind(&Z80::iowrite, t, (t->a << 8) | t->databus, t->databus, &Z80::fetch);
	});
}
void Z80::ex_af()
{
	const register16_t tmp = af;
	af = afprime;
	afprime = tmp;
	step = &Z80::fetch;
}
void Z80::ex_de_hl()
{
	const register16_t tmp = de;
	de = hl;
	hl = tmp;
	step = &Z80::fetch;
}
void Z80::ex_sp_hl()
{
	step = std::bind(&Z80::read16, this, sp, [](Z80* t) {
		t->step = [](Z80* t) {
			t->step = std::bind(&Z80::write, t, t->buffer16, t->hli[t->hli_idx].l, [](Z80* t) {
				t->step = std::bind(&Z80::write, t, t->buffer16 + 1, t->hli[t->hli_idx].h, [](Z80* t) {
					t->step = [](Z80* t) {
						t->step = [](Z80* t) {
							t->hli[t->hli_idx].hl = t->buffer16;
							t->step = &Z80::fetch;
							};
						};
					});
				});
			};
		});
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
	step = &Z80::fetch;
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

uint8_t Z80::increment(const uint8_t value) {
	uint8_t result = value + 1;
	f &= ~(flags::NF | flags::HF | flags::VF);
	setSZXY(result);
	if (result == 0x80)
		f |= flags::VF;
	if ((result & 0x0f) == 0x00)
		f |= flags::HF;
	return result;
}

uint8_t Z80::decrement(const uint8_t value) {
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

