#include "z80.h"


using namespace xprocessors::experimental;


// LOADING OPCODES

// The content of register rr is loaded into register r
void Z80::ld_r_r(register8_t& r, const register8_t rr)
{
	r = rr;
	step = &Z80::fetch;
}
// Loading of argument into register r
void Z80::ld_r_n(register8_t& r)
{
//	step = [](Z80* t) { t->readArgs([](Z80* t) { t->b = t->databus; t->step = &Z80::fetch; }); };
	step = std::bind(&Z80::readArgs, this, [&r](Z80* t) { r = t->databus; t->step = &Z80::fetch; });
}
// Loading of a into (nn)
void Z80::ld_nn_a()
{
	step = std::bind(&Z80::readArgs16, this, [](Z80* t) {
		t->step = std::bind(&Z80::write, t, t->buffer16, t->a, [](Z80* t) { t->step = &Z80::fetch; });
		});
}
// Loading hp into sp
void Z80::ld_sp_hl()
{
	step = [](Z80* t) {
		t->sp = t->hl;
		t->step = [](Z80* t) { t->step = &Z80::fetch; };
		};
}

// Loading of argument into (HL)
void Z80::ld_hl_n()
{
	if (hli_idx == 0) {
		step = std::bind(&Z80::readArgs, this, [](Z80* t) {
			t->step = std::bind(&Z80::write, t, t->hl, t->databus, [](Z80* t) { t->step = &Z80::fetch; });
			});
	}
	else {
		step = std::bind(&Z80::readArgs, this, [](Z80* t) {
			t->buffer16 = t->hli[t->hli_idx].hl + static_cast<int8_t>(t->databus);
			t->step = std::bind(&Z80::readArgs, t, [](Z80* t) {
				t->step = std::bind(&Z80::wait, t, 2, [](Z80* t) { t->write(t->buffer16, t->databus, [](Z80* t) { t->step = &Z80::fetch; }); });
				});
			});
	}
}

// Loading of hl into (nn)
void Z80::ld_nn_hl()
{
	step = std::bind(&Z80::readArgs16, this, [](Z80* t) {
		t->step = std::bind(&Z80::write, t, t->buffer16, t->hli[t->hli_idx].l, [](Z80* t) {
			t->step = std::bind(&Z80::write, t, t->buffer16 + 1, t->hli[t->hli_idx].h, [](Z80* t) { t->step = &Z80::fetch; });
			});
		});
}

// Loading of rr into (nn)
void Z80::ld_nnnn_rr(const register16_t rr)
{
	step = std::bind(&Z80::readArgs16, this, [rr](Z80* t) {
		t->step = std::bind(&Z80::write, t, t->buffer16, rr & 0xff, [rr](Z80* t) {
			t->step = std::bind(&Z80::write, t, t->buffer16 + 1, rr >> 8, [](Z80* t) { t->step = &Z80::fetch; });
			});
		});
}
// Loading of (nn) into rr
void Z80::ld_rr_nnnn(register16_t& rr)
{
	step = std::bind(&Z80::readArgs16, this, [&rr](Z80* t) {
		t->step = std::bind(&Z80::read, t, t->buffer16, [&rr](Z80* t) {
			rr = (rr & 0xff00) | t->databus;
			t->step = std::bind(&Z80::read, t, t->buffer16 + 1, [&rr](Z80* t) {
				rr = (rr & 0x00ff) | (t->databus << 8);
				t->step = &Z80::fetch; });
			});
		});
}

// Loading of (hl) into register
void Z80::ld_r_hl(register8_t& r)
{
	read_hli([&r](Z80* t) {
		t->step = std::bind(&Z80::read, t, t->buffer16, [&r](Z80* t) {
			r = t->databus;
			t->step = &Z80::fetch;
			});
		});
}

// Loading r into (a)
void Z80::ld_rr_r(const uint16_t a, const register8_t r)
{
	step = std::bind(&Z80::write, this, a, r, [](Z80* t) { t->step = &Z80::fetch; });
}

// Loading r into (hl)
void Z80::ld_hl_r(const register8_t r)
{
	read_hli([r](Z80* t) {
		t->step = std::bind(&Z80::write, t, t->buffer16, r, [](Z80* t) { t->step = &Z80::fetch; });
		});
}

// OTHER OPCODES

void Z80::push(const register16_t rr)
{
	step = [rr](Z80* t) { t->step = std::bind(&Z80::write, t, --t->sp, rr >> 8, [rr](Z80* t) {
		t->step = std::bind(&Z80::write, t, --t->sp, rr & 0xff, [](Z80* t) { t->step = &Z80::fetch; });
		});
		};
}
void Z80::pop(register16_t& rr)
{
	step = std::bind(&Z80::read16, this, sp, [&rr](Z80* t) {
		rr = t->buffer16;
		t->sp += 2;
		t->step = &Z80::fetch;
	});
}

void Z80::jp_c(const bool flag) {
	step = std::bind(&Z80::readArgs16, this, [flag](Z80* t) {
		if (flag) {
			t->pc = t->buffer16;
		}
		t->step = &Z80::fetch;
	});
}
void Z80::jp_hl() {
	pc = hli[hli_idx].hl;
	step = &Z80::fetch;
}
void Z80::jr_c(const bool flag) {
	step = std::bind(&Z80::read, this, pc++, [flag](Z80* t) {
		if (!flag)
			t->step = &Z80::fetch;
		else {
			t->pc += static_cast<signed char>(t->databus);
			t->step = std::bind(&Z80::wait, t, 5, &Z80::fetch);
		}
	});
}
