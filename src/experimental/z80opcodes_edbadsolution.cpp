#include "z80.h"


using namespace xprocessors::experimental;


// Opcodes table
std::function<void(Z80*)> Z80::decode_opcodes_ed[] =
{
	// 0x00 - 0x0f
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	// 0x10 - 0x1f
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	// 0x20 - 0x2f
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	// Ox30 - 0x3f
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	// 0x40 - 0x4f
	nullptr,
	nullptr,
	[](Z80* t) { t->sbc(t->bc); },
	[](Z80* t) { t->ld_nnnn_rr(t->bc); },
	&Z80::neg,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	[](Z80* t) { t->adc(t->bc); },
	[](Z80* t) { t->ld_rr_nnnn(t->bc); },
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	// 0x50 - 0x5f
	nullptr,
	nullptr,
	[](Z80* t) { t->sbc(t->de); },
	[](Z80* t) { t->ld_nnnn_rr(t->de); },
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	[](Z80* t) { t->adc(t->de); },
	[](Z80* t) { t->ld_rr_nnnn(t->de); },
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	// 0x60 - 0x6f
	nullptr,
	nullptr,
	[](Z80* t) { t->sbc(t->hl); },
	[](Z80* t) { t->ld_nnnn_rr(t->hl); },
	nullptr,
	nullptr,
	nullptr,
	&Z80::rrd,
	nullptr,
	nullptr,
	[](Z80* t) { t->adc(t->hl); },
	[](Z80* t) { t->ld_rr_nnnn(t->hl); },
	nullptr,
	nullptr,
	nullptr,
	&Z80::rld,

	// 0x70 - 0x7f
	nullptr,
	nullptr,
	[](Z80* t) { t->sbc(t->sp); },
	[](Z80* t) { t->ld_nnnn_rr(t->sp); },
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	[](Z80* t) { t->adc(t->sp); },
	[](Z80* t) { t->ld_rr_nnnn(t->sp); },
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	// 0x80 - 0x8f
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	// 0x90 - 0x9f
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	// 0xa0 - 0xaf
	[](Z80* t) { t->ldi(); },
	[](Z80* t) { t->cpi(); },
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	[](Z80* t) { t->ldd(); },
	[](Z80* t) { t->cpd(); },
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	// 0xb0 - 0xbf
	&Z80::ldir,
	&Z80::cpir,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	&Z80::lddr,
	&Z80::cpdr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	// 0xc0 - 0xcf
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	// 0xd0 - 0xdf
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	// 0xe0 - 0xef
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,

	// 0xf0 - 0xff
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
};

// Opcodes dispatch
void Z80::fetchED()
{
	step = [](Z80* t) {
		t->_fetch(Z80::decode_opcodes_ed);
		};
}

void Z80::sbc(const register16_t rr) {
	const uint32_t s = hl - rr - ((f & Z80::flags::CF) ? 1 : 0);
	const uint32_t c = (s ^ hl ^ rr) & 0x18000;
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
	step = std::bind(&Z80::wait, this, 6, &Z80::fetch);
}

void Z80::adc(const register16_t rr) {
	const uint32_t s = hl + rr + ((f & Z80::flags::CF) ? 1 : 0);
	const uint32_t c = (s ^ hl ^ rr);
	hl = static_cast<uint16_t>(s);
	f &= ~(Z80::flags::NF | Z80::flags::CF | Z80::flags::HF | Z80::flags::PF);
	if (s > 0xffff)
		f |= Z80::flags::CF;
	if (c & 0x1000)
		f |= Z80::flags::HF;
	if (((c >> 15) & 0x1) ^ ((c >> 16) & 0x1))
		f |= Z80::flags::PF;
	setSZXY(hl);
	step = std::bind(&Z80::wait, this, 6, &Z80::fetch);
}

void Z80::ldi(std::function<void(Z80*)> cb)
{
	step = std::bind(&Z80::read, this, hl++, [cb](Z80* t) {
		t->step = std::bind(&Z80::write, t, t->de++, t->databus, [cb](Z80* t) {
			t->bc--;
			t->step = [cb](Z80* t) {
				t->f &= ~(Z80::flags::HF | Z80::flags::NF | Z80::flags::PF);
				if (t->bc != 0)
					t->f |= Z80::flags::PF;
				t->step = cb; };
			});
		});
}
void Z80::ldir()
{
	ldi([](Z80* t) {
		if (t->bc != 0) {
			t->step = std::bind(&Z80::wait, t, 5, [](Z80* t) { t->pc -= 2; t->step = &Z80::fetch; });
		}
		else {
			t->fetch();
		}
		});
}
void Z80::ldd(std::function<void(Z80*)> cb)
{
	step = std::bind(&Z80::read, this, hl--, [cb](Z80* t) {
		t->step = std::bind(&Z80::write, t, t->de--, t->databus, [cb](Z80* t) {
			t->bc--;
			t->step = [cb](Z80* t) {
				t->f &= ~(Z80::flags::HF | Z80::flags::NF | Z80::flags::PF);
				if (t->bc != 0)
					t->f |= Z80::flags::PF;
				t->step = cb; };
			});
		});
}
void Z80::lddr()
{
	ldd([](Z80* t) {
		if (t->bc != 0) {
			t->step = std::bind(&Z80::wait, t, 5, [](Z80* t) { t->pc -= 2; t->step = &Z80::fetch; });
		}
		else {
			t->fetch();
		}
		});
}


void Z80::cpd(std::function<void(Z80*)> cb)
{
	bool carryFlag = f & Z80::flags::CF;
	step = std::bind(&Z80::read, this, hl, [carryFlag, cb](Z80* t) {
		--t->hl;
		t->cp(t->databus, std::bind(&Z80::wait, t, 5, [cb](Z80* t) {
			if (--t->bc != 0)
				t->f |= Z80::flags::PF;
			else
				t->f &= ~Z80::flags::PF;
			t->step = cb; }));
		if (carryFlag)
			t->f |= Z80::flags::CF;
		else
			t->f &= ~Z80::flags::CF;
		});
}
void Z80::cpdr()
{
	cpd([](Z80* t) {
		if ((t->bc != 0)&&((t->f&Z80::flags::ZF)==0)) {
			t->step = std::bind(&Z80::wait, t, 5, [](Z80* t) { t->pc -= 2; t->step = &Z80::fetch; });
		}
		else {
			t->fetch();
		}
		});
}
void Z80::cpi(std::function<void(Z80*)> cb)
{
	step = std::bind(&Z80::read, this, hl, [cb](Z80* t) {
		bool carryFlag = t->f & Z80::flags::CF;
		++t->hl;
		t->cp(t->databus, std::bind(&Z80::wait, t, 5, [cb](Z80* t) {
			if (--t->bc != 0)
				t->f |= Z80::flags::PF;
			else
				t->f &= ~Z80::flags::PF;
			t->step = cb; }));
		if (carryFlag)
			t->f |= Z80::flags::CF;
		else
			t->f &= ~Z80::flags::CF;
		});
}
void Z80::cpir()
{
	cpi([](Z80* t) {
		if ((t->bc != 0) && ((t->f & Z80::flags::ZF) == 0)) {
			t->step = std::bind(&Z80::wait, t, 5, [](Z80* t) { t->pc -= 2; t->step = &Z80::fetch; });
		}
		else {
			t->fetch();
		}
		});
}
void Z80::neg()
{
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
	step = &Z80::fetch;
}
void Z80::rrd() {
	step = std::bind(&Z80::read, this, hl, [](Z80* t) {
		const uint8_t tmp = ((t->a & 0x0f) << 4) | (t->databus >> 4);
		t->a = (t->a & 0xf0) | (t->databus & 0x0f);
		t->step = std::bind(&Z80::wait, t, 4, [tmp](Z80* t) {
			t->step = std::bind(&Z80::write, t, t->hl, tmp, [](Z80* t) {
				t->f &= Z80::flags::CF;
				t->setSZXY(t->a);
				t->setP(t->a);
				t->step = &Z80::fetch;
				});
			});
		});
}
void Z80::rld() {
	step = std::bind(&Z80::read, this, hl, [](Z80* t) {
		const uint8_t tmp = (t->a & 0x0f) | (t->databus << 4);
		t->a = (t->a & 0xf0) | (t->databus >> 4);
		t->step = std::bind(&Z80::wait, t, 4, [tmp](Z80* t) {
			t->step = std::bind(&Z80::write, t, t->hl, tmp, [](Z80* t) {
				t->f &= Z80::flags::CF;
				t->setSZXY(t->a);
				t->setP(t->a);
				t->step = &Z80::fetch;
				});
			});
		});
}
