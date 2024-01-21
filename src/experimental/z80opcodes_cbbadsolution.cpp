#include "z80.h"


using namespace xprocessors::experimental;


// Opcodes table
std::function<void(Z80*)> Z80::decode_opcodes_cb[] =
{
	// 0x00 - 0x0f
	[](Z80* t) { if (t->hli_idx > 0) t->rlc_hl(); else t->rlc(t->b); },
	[](Z80* t) { if (t->hli_idx > 0) t->rlc_hl(); else t->rlc(t->c); },
	[](Z80* t) { if (t->hli_idx > 0) t->rlc_hl(); else t->rlc(t->d); },
	[](Z80* t) { if (t->hli_idx > 0) t->rlc_hl(); else t->rlc(t->e); },
	[](Z80* t) { if (t->hli_idx > 0) t->rlc_hl(); else t->rlc(t->h); },
	[](Z80* t) { if (t->hli_idx > 0) t->rlc_hl(); else t->rlc(t->l); },
	&Z80::rlc_hl,
	[](Z80* t) { if (t->hli_idx > 0) t->rlc_hl(); else t->rlc(t->a); },
	[](Z80* t) { if (t->hli_idx > 0) t->rrc_hl(); else t->rrc(t->b); },
	[](Z80* t) { if (t->hli_idx > 0) t->rrc_hl(); else t->rrc(t->c); },
	[](Z80* t) { if (t->hli_idx > 0) t->rrc_hl(); else t->rrc(t->d); },
	[](Z80* t) { if (t->hli_idx > 0) t->rrc_hl(); else t->rrc(t->e); },
	[](Z80* t) { if (t->hli_idx > 0) t->rrc_hl(); else t->rrc(t->h); },
	[](Z80* t) { if (t->hli_idx > 0) t->rrc_hl(); else t->rrc(t->l); },
	& Z80::rrc_hl,
	[](Z80* t) { if (t->hli_idx > 0) t->rrc_hl(); else t->rrc(t->a); },

	// 0x10 - 0x1f
	[](Z80* t) { if (t->hli_idx > 0) t->rl_hl(); else t->rl(t->b); },
	[](Z80* t) { if (t->hli_idx > 0) t->rl_hl(); else t->rl(t->c); },
	[](Z80* t) { if (t->hli_idx > 0) t->rl_hl(); else t->rl(t->d); },
	[](Z80* t) { if (t->hli_idx > 0) t->rl_hl(); else t->rl(t->e); },
	[](Z80* t) { if (t->hli_idx > 0) t->rl_hl(); else t->rl(t->h); },
	[](Z80* t) { if (t->hli_idx > 0) t->rl_hl(); else t->rl(t->l); },
	& Z80::rl_hl,
	[](Z80* t) { if (t->hli_idx > 0) t->rl_hl(); else t->rl(t->a); },
	[](Z80* t) { if (t->hli_idx > 0) t->rr_hl(); else t->rr(t->b); },
	[](Z80* t) { if (t->hli_idx > 0) t->rr_hl(); else t->rr(t->c); },
	[](Z80* t) { if (t->hli_idx > 0) t->rr_hl(); else t->rr(t->d); },
	[](Z80* t) { if (t->hli_idx > 0) t->rr_hl(); else t->rr(t->e); },
	[](Z80* t) { if (t->hli_idx > 0) t->rr_hl(); else t->rr(t->h); },
	[](Z80* t) { if (t->hli_idx > 0) t->rr_hl(); else t->rr(t->l); },
	& Z80::rr_hl,
	[](Z80* t) { if (t->hli_idx > 0) t->rr_hl(); else t->rr(t->a); },

	// 0x20 - 0x2f
	[](Z80* t) { if (t->hli_idx > 0) t->sla_hl(); else t->sla(t->b); },
	[](Z80* t) { if (t->hli_idx > 0) t->sla_hl(); else t->sla(t->c); },
	[](Z80* t) { if (t->hli_idx > 0) t->sla_hl(); else t->sla(t->d); },
	[](Z80* t) { if (t->hli_idx > 0) t->sla_hl(); else t->sla(t->e); },
	[](Z80* t) { if (t->hli_idx > 0) t->sla_hl(); else t->sla(t->h); },
	[](Z80* t) { if (t->hli_idx > 0) t->sla_hl(); else t->sla(t->l); },
	& Z80::sla_hl,
	[](Z80* t) { if (t->hli_idx > 0) t->sla_hl(); else t->sla(t->a); },
	[](Z80* t) { if (t->hli_idx > 0) t->sra_hl(); else t->sra(t->b); },
	[](Z80* t) { if (t->hli_idx > 0) t->sra_hl(); else t->sra(t->c); },
	[](Z80* t) { if (t->hli_idx > 0) t->sra_hl(); else t->sra(t->d); },
	[](Z80* t) { if (t->hli_idx > 0) t->sra_hl(); else t->sra(t->e); },
	[](Z80* t) { if (t->hli_idx > 0) t->sra_hl(); else t->sra(t->h); },
	[](Z80* t) { if (t->hli_idx > 0) t->sra_hl(); else t->sra(t->l); },
	& Z80::sra_hl,
	[](Z80* t) { if (t->hli_idx > 0) t->sra_hl(); else t->sra(t->a); },

	// Ox30 - 0x3f
	[](Z80* t) { if (t->hli_idx > 0) t->sll_hl(); else t->sll(t->b); },
	[](Z80* t) { if (t->hli_idx > 0) t->sll_hl(); else t->sll(t->c); },
	[](Z80* t) { if (t->hli_idx > 0) t->sll_hl(); else t->sll(t->d); },
	[](Z80* t) { if (t->hli_idx > 0) t->sll_hl(); else t->sll(t->e); },
	[](Z80* t) { if (t->hli_idx > 0) t->sll_hl(); else t->sll(t->h); },
	[](Z80* t) { if (t->hli_idx > 0) t->sll_hl(); else t->sll(t->l); },
	& Z80::sll_hl,
	[](Z80* t) { if (t->hli_idx > 0) t->sll_hl(); else t->sll(t->a); },
	[](Z80* t) { if (t->hli_idx > 0) t->srl_hl(); else t->srl(t->b); },
	[](Z80* t) { if (t->hli_idx > 0) t->srl_hl(); else t->srl(t->c); },
	[](Z80* t) { if (t->hli_idx > 0) t->srl_hl(); else t->srl(t->d); },
	[](Z80* t) { if (t->hli_idx > 0) t->srl_hl(); else t->srl(t->e); },
	[](Z80* t) { if (t->hli_idx > 0) t->srl_hl(); else t->srl(t->h); },
	[](Z80* t) { if (t->hli_idx > 0) t->srl_hl(); else t->srl(t->l); },
	& Z80::srl_hl,
	[](Z80* t) { if (t->hli_idx > 0) t->srl_hl(); else t->srl(t->a); },

	// 0x40 - 0x4f
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(0); else t->bit(t->b, 0); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(0); else t->bit(t->c, 0); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(0); else t->bit(t->d, 0); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(0); else t->bit(t->e, 0); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(0); else t->bit(t->h, 0); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(0); else t->bit(t->l, 0); },
	[](Z80* t) { t->bit_hl(0); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(0); else t->bit(t->a, 0); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(1); else t->bit(t->b, 1); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(1); else t->bit(t->c, 1); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(1); else t->bit(t->d, 1); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(1); else t->bit(t->e, 1); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(1); else t->bit(t->h, 1); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(1); else t->bit(t->l, 1); },
	[](Z80* t) { t->bit_hl(1); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(1); else t->bit(t->a, 1); },

	// 0x50 - 0x5f
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(2); else t->bit(t->b, 2); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(2); else t->bit(t->c, 2); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(2); else t->bit(t->d, 2); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(2); else t->bit(t->e, 2); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(2); else t->bit(t->h, 2); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(2); else t->bit(t->l, 2); },
	[](Z80* t) { t->bit_hl(2); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(2); else t->bit(t->a, 2); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(3); else t->bit(t->b, 3); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(3); else t->bit(t->c, 3); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(3); else t->bit(t->d, 3); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(3); else t->bit(t->e, 3); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(3); else t->bit(t->h, 3); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(3); else t->bit(t->l, 3); },
	[](Z80* t) { t->bit_hl(3); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(3); else t->bit(t->a, 3); },

	// 0x60 - 0x6f
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(4); else t->bit(t->b, 4); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(4); else t->bit(t->c, 4); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(4); else t->bit(t->d, 4); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(4); else t->bit(t->e, 4); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(4); else t->bit(t->h, 4); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(4); else t->bit(t->l, 4); },
	[](Z80* t) { t->bit_hl(4); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(4); else t->bit(t->a, 4); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(5); else t->bit(t->b, 5); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(5); else t->bit(t->c, 5); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(5); else t->bit(t->d, 5); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(5); else t->bit(t->e, 5); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(5); else t->bit(t->h, 5); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(5); else t->bit(t->l, 5); },
	[](Z80* t) { t->bit_hl(5); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(5); else t->bit(t->a, 5); },

	// 0x70 - 0x7f
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(6); else t->bit(t->b, 6); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(6); else t->bit(t->c, 6); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(6); else t->bit(t->d, 6); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(6); else t->bit(t->e, 6); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(6); else t->bit(t->h, 6); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(6); else t->bit(t->l, 6); },
	[](Z80* t) { t->bit_hl(6); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(6); else t->bit(t->a, 6); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(7); else t->bit(t->b, 7); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(7); else t->bit(t->c, 7); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(7); else t->bit(t->d, 7); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(7); else t->bit(t->e, 7); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(7); else t->bit(t->h, 7); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(7); else t->bit(t->l, 7); },
	[](Z80* t) { t->bit_hl(7); },
	[](Z80* t) { if (t->hli_idx > 0) t->bit_hl(7); else t->bit(t->a, 7); },

	// 0x80 - 0x8f
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(0); else t->res(t->b, 0); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(0); else t->res(t->c, 0); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(0); else t->res(t->d, 0); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(0); else t->res(t->e, 0); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(0); else t->res(t->h, 0); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(0); else t->res(t->l, 0); },
	[](Z80* t) { t->res_hl(0); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(0); else t->res(t->a, 0); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(1); else t->res(t->b, 1); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(1); else t->res(t->c, 1); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(1); else t->res(t->d, 1); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(1); else t->res(t->e, 1); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(1); else t->res(t->h, 1); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(1); else t->res(t->l, 1); },
	[](Z80* t) { t->res_hl(1); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(1); else t->res(t->a, 1); },

	// 0x90 - 0x9f
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(2); else t->res(t->b, 2); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(2); else t->res(t->c, 2); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(2); else t->res(t->d, 2); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(2); else t->res(t->e, 2); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(2); else t->res(t->h, 2); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(2); else t->res(t->l, 2); },
	[](Z80* t) { t->res_hl(2); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(2); else t->res(t->a, 2); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(3); else t->res(t->b, 3); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(3); else t->res(t->c, 3); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(3); else t->res(t->d, 3); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(3); else t->res(t->e, 3); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(3); else t->res(t->h, 3); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(3); else t->res(t->l, 3); },
	[](Z80* t) { t->res_hl(3); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(3); else t->res(t->a, 3); },

	// 0xa0 - 0xaf
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(4); else t->res(t->b, 4); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(4); else t->res(t->c, 4); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(4); else t->res(t->d, 4); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(4); else t->res(t->e, 4); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(4); else t->res(t->h, 4); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(4); else t->res(t->l, 4); },
	[](Z80* t) { t->res_hl(4); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(4); else t->res(t->a, 4); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(5); else t->res(t->b, 5); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(5); else t->res(t->c, 5); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(5); else t->res(t->d, 5); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(5); else t->res(t->e, 5); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(5); else t->res(t->h, 5); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(5); else t->res(t->l, 5); },
	[](Z80* t) { t->res_hl(5); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(5); else t->res(t->a, 5); },

	// 0xb0 - 0xbf
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(6); else t->res(t->b, 6); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(6); else t->res(t->c, 6); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(6); else t->res(t->d, 6); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(6); else t->res(t->e, 6); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(6); else t->res(t->h, 6); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(6); else t->res(t->l, 6); },
	[](Z80* t) { t->res_hl(6); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(6); else t->res(t->a, 6); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(7); else t->res(t->b, 7); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(7); else t->res(t->c, 7); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(7); else t->res(t->d, 7); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(7); else t->res(t->e, 7); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(7); else t->res(t->h, 7); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(7); else t->res(t->l, 7); },
	[](Z80* t) { t->res_hl(7); },
	[](Z80* t) { if (t->hli_idx > 0) t->res_hl(7); else t->res(t->a, 7); },

	// 0xc0 - 0xcf
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(0); else t->set(t->b, 0); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(0); else t->set(t->c, 0); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(0); else t->set(t->d, 0); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(0); else t->set(t->e, 0); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(0); else t->set(t->h, 0); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(0); else t->set(t->l, 0); },
	[](Z80* t) { t->set_hl(0); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(0); else t->set(t->a, 0); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(1); else t->set(t->b, 1); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(1); else t->set(t->c, 1); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(1); else t->set(t->d, 1); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(1); else t->set(t->e, 1); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(1); else t->set(t->h, 1); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(1); else t->set(t->l, 1); },
	[](Z80* t) { t->set_hl(1); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(1); else t->set(t->a, 1); },

	// 0xd0 - 0xdf
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(2); else t->set(t->b, 2); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(2); else t->set(t->c, 2); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(2); else t->set(t->d, 2); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(2); else t->set(t->e, 2); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(2); else t->set(t->h, 2); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(2); else t->set(t->l, 2); },
	[](Z80* t) { t->set_hl(2); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(2); else t->set(t->a, 2); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(3); else t->set(t->b, 3); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(3); else t->set(t->c, 3); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(3); else t->set(t->d, 3); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(3); else t->set(t->e, 3); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(3); else t->set(t->h, 3); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(3); else t->set(t->l, 3); },
	[](Z80* t) { t->set_hl(3); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(3); else t->set(t->a, 3); },

	// 0xe0 - 0xef
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(4); else t->set(t->b, 4); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(4); else t->set(t->c, 4); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(4); else t->set(t->d, 4); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(4); else t->set(t->e, 4); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(4); else t->set(t->h, 4); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(4); else t->set(t->l, 4); },
	[](Z80* t) { t->set_hl(4); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(4); else t->set(t->a, 4); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(5); else t->set(t->b, 5); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(5); else t->set(t->c, 5); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(5); else t->set(t->d, 5); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(5); else t->set(t->e, 5); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(5); else t->set(t->h, 5); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(5); else t->set(t->l, 5); },
	[](Z80* t) { t->set_hl(5); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(5); else t->set(t->a, 5); },

	// 0xf0 - 0xff
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(6); else t->set(t->b, 6); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(6); else t->set(t->c, 6); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(6); else t->set(t->d, 6); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(6); else t->set(t->e, 6); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(6); else t->set(t->h, 6); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(6); else t->set(t->l, 6); },
	[](Z80* t) { t->set_hl(6); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(6); else t->set(t->a, 6); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(7); else t->set(t->b, 7); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(7); else t->set(t->c, 7); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(7); else t->set(t->d, 7); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(7); else t->set(t->e, 7); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(7); else t->set(t->h, 7); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(7); else t->set(t->l, 7); },
	[](Z80* t) { t->set_hl(7); },
	[](Z80* t) { if (t->hli_idx > 0) t->set_hl(7); else t->set(t->a, 7); },
};

// Opcodes dispatch
void Z80::fetchCB()
{
	if (hli_idx == 0)
		step = [](Z80* t) {
			t->_fetch(Z80::decode_opcodes_cb);
		};
	else
		step = std::bind(&Z80::readArgs, this, [](Z80* t) {
			t->buffer16 = t->hli[t->hli_idx].hl + static_cast<int8_t>(t->databus);
			t->step = [](Z80* t) {
				t->_fetch(Z80::decode_opcodes_cb);
			};
		});
}

void Z80::bit(const uint8_t value, const uint8_t b)
{
	f &= Z80::flags::CF;
	f |= Z80::flags::HF;
	f |= (value & (1 << b)) ? Z80::flags::SF : Z80::flags::ZF | Z80::flags::VF;
	step = &Z80::fetch;
}
void Z80::bit_hl(const uint8_t b)
{
	if (hli_idx == 0)
		step = std::bind(&Z80::read, this, hl, [b](Z80* t) {
		t->wait(1, [b](Z80* t) { t->bit(t->databus, b); }); });
	else
		step = std::bind(&Z80::read, this, buffer16, [b](Z80* t) {
			t->wait(1, [b](Z80* t) { t->bit(t->databus, b); });
			});
}

void Z80::set(register8_t& r, const uint8_t b)
{
	r |= (1 << b);
	step = &Z80::fetch;
}
void Z80::set_hl(const uint8_t b)
{
	if (hli_idx == 0)
		step = std::bind(&Z80::read, this, hl, [b](Z80* t) {
		t->wait(1, [b](Z80* t) { t->step = std::bind(&Z80::write, t, t->hl, t->databus | (1 << b), &Z80::fetch); }); });
	else
		step = std::bind(&Z80::read, this, buffer16, [b](Z80* t) {
		t->wait(1, [b](Z80* t) { t->step = std::bind(&Z80::write, t, t->buffer16, t->databus | (1 << b), &Z80::fetch); }); });
}

void Z80::res(register8_t& r, const uint8_t b)
{
	r &= ~(1 << b);
	step = &Z80::fetch;
}
void Z80::res_hl(const uint8_t b)
{
	if (hli_idx == 0)
		step = std::bind(&Z80::read, this, hl, [b](Z80* t) {
		t->wait(1, [b](Z80* t) { t->step = std::bind(&Z80::write, t, t->hl, t->databus & ~(1 << b), &Z80::fetch); }); });
	else
		step = std::bind(&Z80::read, this, buffer16, [b](Z80* t) {
		t->wait(1, [b](Z80* t) { t->step = std::bind(&Z80::write, t, t->buffer16, t->databus & ~(1 << b), &Z80::fetch); }); });
}


void Z80::rlc(register8_t& r, std::function<void(Z80*)> cb) {
	const uint8_t result = (r << 1) | (r >> 7);
	f = (r & 0x80) ? Z80::flags::CF : 0;
	r = result;
	setSZXY(r);
	setP(r);
	step = cb;
}
void Z80::rlc_hl() {
	if (hli_idx == 0)
		step = std::bind(&Z80::read, this, hl, [](Z80* t) {
		t->wait(1, [](Z80* t) { t->rlc(t->databus, [](Z80* t) {
			t->step = std::bind(&Z80::write, t, t->hl, t->databus, &Z80::fetch);
			}); }); });
	else
		step = std::bind(&Z80::read, this, buffer16, [](Z80* t) {
		t->wait(1, [](Z80* t) { t->rlc(t->databus, [](Z80* t) {
			t->step = std::bind(&Z80::write, t, t->buffer16, t->databus, &Z80::fetch);
			}); }); });
}

void Z80::rrc(register8_t& r, std::function<void(Z80*)> cb) {
	const uint8_t result = (r >> 1) | (r << 7);
	f = (r & 0x01) ? Z80::flags::CF : 0;
	r = result;
	setSZXY(r);
	setP(r);
	step = cb;
}
void Z80::rrc_hl() {
	if (hli_idx == 0)
		step = std::bind(&Z80::read, this, hl, [](Z80* t) {
		t->wait(1, [](Z80* t) { t->rrc(t->databus, [](Z80* t) {
			t->step = std::bind(&Z80::write, t, t->hl, t->databus, &Z80::fetch);
			}); }); });
	else
		step = std::bind(&Z80::read, this, buffer16, [](Z80* t) {
		t->wait(1, [](Z80* t) { t->rrc(t->databus, [](Z80* t) {
			t->step = std::bind(&Z80::write, t, t->buffer16, t->databus, &Z80::fetch);
			}); }); });
}
void Z80::rl(register8_t& r, std::function<void(Z80*)> cb) {
	const uint8_t result = (r << 1) | ((f & Z80::flags::CF)? 0x01 : 0);
	f = (r & 0x80) ? Z80::flags::CF : 0;
	r = result;
	setSZXY(r);
	setP(r);
	step = cb;
}
void Z80::rl_hl() {
	if (hli_idx == 0)
		step = std::bind(&Z80::read, this, hl, [](Z80* t) {
		t->wait(1, [](Z80* t) { t->rl(t->databus, [](Z80* t) {
			t->step = std::bind(&Z80::write, t, t->hl, t->databus, &Z80::fetch);
			}); }); });
	else
		step = std::bind(&Z80::read, this, buffer16, [](Z80* t) {
		t->wait(1, [](Z80* t) { t->rl(t->databus, [](Z80* t) {
			t->step = std::bind(&Z80::write, t, t->buffer16, t->databus, &Z80::fetch);
			}); }); });
}
void Z80::rr(register8_t& r, std::function<void(Z80*)> cb) {
	const uint8_t result = (r >> 1) | ((f & Z80::flags::CF)? 0x80: 0);
	f = (r & 0x01) ? Z80::flags::CF : 0;
	r = result;
	setSZXY(r);
	setP(r);
	step = cb;
}
void Z80::rr_hl() {
	if (hli_idx == 0)
		step = std::bind(&Z80::read, this, hl, [](Z80* t) {
		t->wait(1, [](Z80* t) { t->rr(t->databus, [](Z80* t) {
			t->step = std::bind(&Z80::write, t, t->hl, t->databus, &Z80::fetch);
			}); }); });
	else
		step = std::bind(&Z80::read, this, buffer16, [](Z80* t) {
		t->wait(1, [](Z80* t) { t->rr(t->databus, [](Z80* t) {
			t->step = std::bind(&Z80::write, t, t->buffer16, t->databus, &Z80::fetch);
			}); }); });
}

void Z80::sra(register8_t& r, std::function<void(Z80*)> cb) {
	const uint8_t result = (r >> 1) | (r & 0x80);
	f = (r & 0x01) ? Z80::flags::CF : 0;
	r = result;
	setSZXY(r);
	setP(r);
	step = cb;
}
void Z80::sra_hl() {
	if (hli_idx == 0)
		step = std::bind(&Z80::read, this, hl, [](Z80* t) {
		t->wait(1, [](Z80* t) { t->sra(t->databus, [](Z80* t) {
			t->step = std::bind(&Z80::write, t, t->hl, t->databus, &Z80::fetch);
			}); }); });
	else
		step = std::bind(&Z80::read, this, buffer16, [](Z80* t) {
		t->wait(1, [](Z80* t) { t->sra(t->databus, [](Z80* t) {
			t->step = std::bind(&Z80::write, t, t->buffer16, t->databus, &Z80::fetch);
			}); }); });
}

void Z80::sla(register8_t& r, std::function<void(Z80*)> cb) {
	const uint8_t result = (r << 1);
	f = (r & 0x80) ? Z80::flags::CF : 0;
	r = result;
	setSZXY(r);
	setP(r);
	step = cb;
}
void Z80::sla_hl() {
	if (hli_idx == 0)
		step = std::bind(&Z80::read, this, hl, [](Z80* t) {
		t->wait(1, [](Z80* t) { t->sla(t->databus, [](Z80* t) {
			t->step = std::bind(&Z80::write, t, t->hl, t->databus, &Z80::fetch);
			}); }); });
	else
		step = std::bind(&Z80::read, this, buffer16, [](Z80* t) {
		t->wait(1, [](Z80* t) { t->sla(t->databus, [](Z80* t) {
			t->step = std::bind(&Z80::write, t, t->buffer16, t->databus, &Z80::fetch);
			}); }); });
}

void Z80::sll(register8_t& r, std::function<void(Z80*)> cb) {
	const uint8_t result = (r << 1) | 1;
	f = (r & 0x80) ? Z80::flags::CF : 0;
	r = result;
	setSZXY(r);
	setP(r);
	step = cb;
}
void Z80::sll_hl() {
	if (hli_idx == 0)
		step = std::bind(&Z80::read, this, hl, [](Z80* t) {
		t->wait(1, [](Z80* t) { t->sll(t->databus, [](Z80* t) {
			t->step = std::bind(&Z80::write, t, t->hl, t->databus, &Z80::fetch);
			}); }); });
	else
		step = std::bind(&Z80::read, this, buffer16, [](Z80* t) {
		t->wait(1, [](Z80* t) { t->sll(t->databus, [](Z80* t) {
			t->step = std::bind(&Z80::write, t, t->buffer16, t->databus, &Z80::fetch);
			}); }); });
}

void Z80::srl(register8_t& r, std::function<void(Z80*)> cb) {
	const uint8_t result = (r >> 1);
	f = (r & 0x01) ? Z80::flags::CF : 0;
	r = result;
	setSZXY(r);
	setP(r);
	step = cb;
}
void Z80::srl_hl() {
	if (hli_idx == 0)
		step = std::bind(&Z80::read, this, hl, [](Z80* t) {
		t->wait(1, [](Z80* t) { t->srl(t->databus, [](Z80* t) {
			t->step = std::bind(&Z80::write, t, t->hl, t->databus, &Z80::fetch);
			}); }); });
	else
		step = std::bind(&Z80::read, this, buffer16, [](Z80* t) {
		t->wait(1, [](Z80* t) { t->srl(t->databus, [](Z80* t) {
			t->step = std::bind(&Z80::write, t, t->buffer16, t->databus, &Z80::fetch);
			}); }); });
}
