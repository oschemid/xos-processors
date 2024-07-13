#include "sm83.h"

using namespace xprocessors::cpu;

#define FETCH { addressbus_operation::PUSH_PC, databus_operation::POP_IR, idu_operation::INC_PC }
#define FETCH_ALU(aluop) { addressbus_operation::PUSH_PC, databus_operation::POP_IR, idu_operation::INC_PC, alu_operation::aluop }
#define FETCH_MISC(miscop) { addressbus_operation::PUSH_PC, databus_operation::POP_IR, idu_operation::INC_PC, alu_operation::NONE, misc_operation::miscop }
#define READZ { addressbus_operation::PUSH_PC, databus_operation::POP_Z, idu_operation::INC_PC }
#define READZ_MISC(miscop) { addressbus_operation::PUSH_PC, databus_operation::POP_Z, idu_operation::INC_PC, alu_operation::NONE, misc_operation::miscop }
#define READW { addressbus_operation::PUSH_PC, databus_operation::POP_W, idu_operation::INC_PC }
#define READW_MISC(miscop) { addressbus_operation::PUSH_PC, databus_operation::POP_W, idu_operation::INC_PC, alu_operation::NONE, misc_operation::miscop }
#define OP_HL(aluop) { {addressbus_operation::PUSH_HL, databus_operation::POP_Z, idu_operation::NONE, alu_operation::aluop}, {addressbus_operation::PUSH_HL, databus_operation::PUSH_Z}, FETCH }

const std::vector<sm83::step> sm83::opcodes_steps[] = {
	{ FETCH }, // NOP
	{ READZ, READW, FETCH_MISC(WZ_TO_BC) }, // LD BC,nn
	{ { addressbus_operation::PUSH_BC, databus_operation::PUSH_A }, FETCH }, // LD (BC),A
	{ {.idu = idu_operation::INC_BC }, FETCH }, // INC BC
	{ FETCH_ALU(INC_B) }, // INC B
	{ FETCH_ALU(DEC_B) }, // DEC B
	{ READZ, FETCH_ALU(Z_TO_B) }, // LD B,n
	{ FETCH_ALU(RLCA) }, // RLCA
	{ READZ, READW, { addressbus_operation::PUSH_WZ, databus_operation::PUSH_SPL, idu_operation::INC_WZ },  { addressbus_operation::PUSH_WZ, databus_operation::PUSH_SPH }, FETCH }, // LD (nn),SP
	{ { .alu = alu_operation::ADD_C_TO_L }, FETCH_ALU(ADD_B_TO_H) }, // ADD HL,BC
	{ { addressbus_operation::PUSH_BC, databus_operation::POP_Z }, FETCH_ALU(Z_TO_A)}, // LD A,(BC)
	{ {.idu = idu_operation::DEC_BC }, FETCH }, // DEC BC
	{ FETCH_ALU(INC_C) }, // INC C
	{ FETCH_ALU(DEC_C) }, // DEC C
	{ READZ, FETCH_ALU(Z_TO_C) }, // LD C,n
	{ FETCH_ALU(RRCA) }, // RRCA

	{ READZ, FETCH }, // STOP 0
	{ READZ, READW, FETCH_MISC(WZ_TO_DE) }, // LD DE,nn
	{ { addressbus_operation::PUSH_DE, databus_operation::PUSH_A }, FETCH }, // LD (DE),A
	{ {.idu = idu_operation::INC_DE }, FETCH }, // INC DE
	{ FETCH_ALU(INC_D) }, // INC D
	{ FETCH_ALU(DEC_D) }, // DEC D
	{ READZ, FETCH_ALU(Z_TO_D) }, // LD D,n
	{ FETCH_ALU(RLA) }, // RLA
	{ READZ, { .idu=idu_operation::ADD_Z_TO_PC }, FETCH }, // JR n
	{ {.alu = alu_operation::ADD_E_TO_L }, FETCH_ALU(ADD_D_TO_H) }, // ADD HL,DE
	{ { addressbus_operation::PUSH_DE, databus_operation::POP_Z }, FETCH_ALU(Z_TO_A)}, // LD A,(DE)
	{ {.idu = idu_operation::DEC_DE }, FETCH }, // DEC DE
	{ FETCH_ALU(INC_E) }, // INC E
	{ FETCH_ALU(DEC_E) }, // DEC E
	{ READZ, FETCH_ALU(Z_TO_E) }, // LD E,n
	{ FETCH_ALU(RRA) }, // RRA

	{ READZ_MISC(CHECK_NZ), {.idu = idu_operation::ADD_Z_TO_PC}, FETCH}, // JR NZ,n
	{ READZ, READW, FETCH_MISC(WZ_TO_HL) }, // LD HL,nn
	{ {addressbus_operation::PUSH_HL, databus_operation::PUSH_A, idu_operation::INC_HL }, FETCH }, // LD (HL+),A
	{ {.idu = idu_operation::INC_HL }, FETCH }, // INC HL
	{ FETCH_ALU(INC_H) }, // INC H
	{ FETCH_ALU(DEC_H) }, // DEC H
	{ READZ, FETCH_ALU(Z_TO_H) }, // LD H,n
	{ FETCH_ALU(DAA) }, // DAA
	{ READZ_MISC(CHECK_Z), {.idu = idu_operation::ADD_Z_TO_PC}, FETCH}, // JR Z,n
	{ {.alu = alu_operation::ADD_L_TO_L }, FETCH_ALU(ADD_H_TO_H) }, // ADD HL,HL
	{ { addressbus_operation::PUSH_HL, databus_operation::POP_Z, idu_operation::INC_HL }, FETCH_ALU(Z_TO_A) }, // LD A,(HL+)
	{ {.idu = idu_operation::DEC_HL }, FETCH }, // DEC HL
	{ FETCH_ALU(INC_L) }, // INC L
	{ FETCH_ALU(DEC_L) }, // DEC L
	{ READZ, FETCH_ALU(Z_TO_L) }, // LD L,n
	{ FETCH_ALU(CPL) }, // CPL

	{ READZ_MISC(CHECK_NC), {.idu = idu_operation::ADD_Z_TO_PC}, FETCH }, // JR NC,n
	{ READZ, READW, FETCH_MISC(WZ_TO_SP) }, // LD SP,nn
	{ {addressbus_operation::PUSH_HL, databus_operation::PUSH_A, idu_operation::DEC_HL }, FETCH }, // LD (HL-),A
	{ {.idu = idu_operation::INC_SP }, FETCH }, // INC SP
	OP_HL(INC_Z), // INC (HL)
	OP_HL(DEC_Z), // DEC (HL)
	{ READZ, {addressbus_operation::PUSH_HL, databus_operation::PUSH_Z }, FETCH }, // LD (HL),n
	{ FETCH_ALU(SCF) }, // SCF
	{ READZ_MISC(CHECK_C), {.idu = idu_operation::ADD_Z_TO_PC}, FETCH }, // JR C,n
	{ {.alu = alu_operation::ADD_SPL_TO_L }, FETCH_ALU(ADD_SPH_TO_H) }, // ADD HL,SP
	{ { addressbus_operation::PUSH_HL, databus_operation::POP_Z, idu_operation::DEC_HL }, FETCH_ALU(Z_TO_A) }, // LD A,(HL-)
	{ {.idu = idu_operation::DEC_SP }, FETCH }, // DEC SP
	{ FETCH_ALU(INC_A) }, // INC A
	{ FETCH_ALU(DEC_A) }, // DEC A
	{ READZ, FETCH_ALU(Z_TO_A) }, // LD A,n
	{ FETCH_ALU(CCF) }, // CCF

	{ FETCH }, // LD B,B
	{ FETCH_ALU(C_TO_B) }, // LD B,C
	{ FETCH_ALU(D_TO_B) }, // LD B,D
	{ FETCH_ALU(E_TO_B) }, // LD B,E
	{ FETCH_ALU(H_TO_B) }, // LD B,H
	{ FETCH_ALU(L_TO_B) }, // LD B,L
	{ {addressbus_operation::PUSH_HL, databus_operation::POP_Z}, FETCH_ALU(Z_TO_B) }, // LD B,(HL)
	{ FETCH_ALU(A_TO_B) }, // LD B,A
	{ FETCH_ALU(B_TO_C) }, // LD C,B
	{ FETCH }, // LD C,C
	{ FETCH_ALU(D_TO_C) }, // LD C,D
	{ FETCH_ALU(E_TO_C) }, // LD C,E
	{ FETCH_ALU(H_TO_C) }, // LD C,H
	{ FETCH_ALU(L_TO_C) }, // LD C,L
	{ {addressbus_operation::PUSH_HL, databus_operation::POP_Z}, FETCH_ALU(Z_TO_C)}, // LD C,(HL)
	{ FETCH_ALU(A_TO_C) }, // LD C,A

	{ FETCH_ALU(B_TO_D) }, // LD D,B
	{ FETCH_ALU(C_TO_D) }, // LD D,C
	{ FETCH }, // LD D,D
	{ FETCH_ALU(E_TO_D) }, // LD D,E
	{ FETCH_ALU(H_TO_D) }, // LD D,H
	{ FETCH_ALU(L_TO_D) }, // LD D,L
	{ {addressbus_operation::PUSH_HL, databus_operation::POP_Z}, FETCH_ALU(Z_TO_D)}, // LD D,(HL)
	{ FETCH_ALU(A_TO_D) }, // LD D,A
	{ FETCH_ALU(B_TO_E) }, // LD E,B
	{ FETCH_ALU(C_TO_E) }, // LD E,C
	{ FETCH_ALU(D_TO_E) }, // LD E,D
	{ FETCH }, // LD E,E
	{ FETCH_ALU(H_TO_E) }, // LD E,H
	{ FETCH_ALU(L_TO_E) }, // LD E,L
	{ {addressbus_operation::PUSH_HL, databus_operation::POP_Z}, FETCH_ALU(Z_TO_E)}, // LD E,(HL)
	{ FETCH_ALU(A_TO_E) }, // LD E,A

	{ FETCH_ALU(B_TO_H) }, // LD H,B
	{ FETCH_ALU(C_TO_H) }, // LD H,C
	{ FETCH_ALU(D_TO_H) }, // LD H,D
	{ FETCH_ALU(E_TO_H) }, // LD H,E
	{ FETCH }, // LD H,H
	{ FETCH_ALU(L_TO_H) }, // LD H,L
	{ {addressbus_operation::PUSH_HL, databus_operation::POP_Z}, FETCH_ALU(Z_TO_H) }, // LD H,(HL)
	{ FETCH_ALU(A_TO_H) }, // LD H,A
	{ FETCH_ALU(B_TO_L) }, // LD L,B
	{ FETCH_ALU(C_TO_L) }, // LD L,C
	{ FETCH_ALU(D_TO_L) }, // LD L,D
	{ FETCH_ALU(E_TO_L) }, // LD L,E
	{ FETCH_ALU(H_TO_L) }, // LD L,H
	{ FETCH }, // LD L,L
	{ {addressbus_operation::PUSH_HL, databus_operation::POP_Z}, FETCH_ALU(Z_TO_L) }, // LD L,(HL)
	{ FETCH_ALU(A_TO_L) }, // LD L,A

	{ {addressbus_operation::PUSH_HL, databus_operation::PUSH_B}, FETCH }, // LD (HL),B
	{ {addressbus_operation::PUSH_HL, databus_operation::PUSH_C}, FETCH }, // LD (HL),C
	{ {addressbus_operation::PUSH_HL, databus_operation::PUSH_D}, FETCH }, // LD (HL),D
	{ {addressbus_operation::PUSH_HL, databus_operation::PUSH_E}, FETCH }, // LD (HL),E
	{ {addressbus_operation::PUSH_HL, databus_operation::PUSH_H}, FETCH }, // LD (HL),H
	{ {addressbus_operation::PUSH_HL, databus_operation::PUSH_L}, FETCH }, // LD (HL),L
	{ FETCH_MISC(HALT) }, // HALT
	{ {addressbus_operation::PUSH_HL, databus_operation::PUSH_A}, FETCH }, // LD (HL),A
	{ FETCH_ALU(B_TO_A) }, // LD A,B
	{ FETCH_ALU(C_TO_A) }, // LD A,C
	{ FETCH_ALU(D_TO_A) }, // LD A,D
	{ FETCH_ALU(E_TO_A) }, // LD A,E
	{ FETCH_ALU(H_TO_A) }, // LD A,H
	{ FETCH_ALU(L_TO_A) }, // LD A,L
	{ {addressbus_operation::PUSH_HL, databus_operation::POP_Z}, FETCH_ALU(Z_TO_A) }, // LD A,(HL)
	{ FETCH }, // LD A,A

	{ FETCH_ALU(ADD_B_TO_A) }, // ADD B
	{ FETCH_ALU(ADD_C_TO_A) }, // ADD C
	{ FETCH_ALU(ADD_D_TO_A) }, // ADD D
	{ FETCH_ALU(ADD_E_TO_A) }, // ADD E
	{ FETCH_ALU(ADD_H_TO_A) }, // ADD H
	{ FETCH_ALU(ADD_L_TO_A) }, // ADD L
	{ {addressbus_operation::PUSH_HL, databus_operation::POP_Z}, FETCH_ALU(ADD_Z_TO_A) }, // ADD (HL)
	{ FETCH_ALU(ADD_A_TO_A) }, // ADD A
	{ FETCH_ALU(ADC_B_TO_A) }, // ADC B
	{ FETCH_ALU(ADC_C_TO_A) }, // ADC C
	{ FETCH_ALU(ADC_D_TO_A) }, // ADC D
	{ FETCH_ALU(ADC_E_TO_A) }, // ADC E
	{ FETCH_ALU(ADC_H_TO_A) }, // ADC H
	{ FETCH_ALU(ADC_L_TO_A) }, // ADC L
	{ {addressbus_operation::PUSH_HL, databus_operation::POP_Z}, FETCH_ALU(ADC_Z_TO_A) }, // ADC (HL)
	{ FETCH_ALU(ADC_A_TO_A) }, // ADC A

	{ FETCH_ALU(SUB_B_TO_A) }, // SUB B
	{ FETCH_ALU(SUB_C_TO_A) }, // SUB C
	{ FETCH_ALU(SUB_D_TO_A) }, // SUB D
	{ FETCH_ALU(SUB_E_TO_A) }, // SUB E
	{ FETCH_ALU(SUB_H_TO_A) }, // SUB H
	{ FETCH_ALU(SUB_L_TO_A) }, // SUB L
	{ {addressbus_operation::PUSH_HL, databus_operation::POP_Z}, FETCH_ALU(SUB_Z_TO_A) }, // SUB (HL)
	{ FETCH_ALU(SUB_A_TO_A) }, // SUB A
	{ FETCH_ALU(SBC_B_TO_A) }, // SBC B
	{ FETCH_ALU(SBC_C_TO_A) }, // SBC C
	{ FETCH_ALU(SBC_D_TO_A) }, // SBC D
	{ FETCH_ALU(SBC_E_TO_A) }, // SBC E
	{ FETCH_ALU(SBC_H_TO_A) }, // SBC H
	{ FETCH_ALU(SBC_L_TO_A) }, // SBC L
	{ {addressbus_operation::PUSH_HL, databus_operation::POP_Z}, FETCH_ALU(SBC_Z_TO_A) }, // SBC (HL)
	{ FETCH_ALU(SBC_A_TO_A) }, // SBC A

	{ FETCH_ALU(AND_B) }, // AND B
	{ FETCH_ALU(AND_C) }, // AND C
	{ FETCH_ALU(AND_D) }, // AND D
	{ FETCH_ALU(AND_E) }, // AND E
	{ FETCH_ALU(AND_H) }, // AND H
	{ FETCH_ALU(AND_L) }, // AND L
	{ { addressbus_operation::PUSH_HL, databus_operation::POP_Z }, FETCH_ALU(AND_Z) }, // AND (HL)
	{ FETCH_ALU(AND_A) }, // AND A
	{ FETCH_ALU(XOR_B) }, // XOR B
	{ FETCH_ALU(XOR_C) }, // XOR C
	{ FETCH_ALU(XOR_D) }, // XOR D
	{ FETCH_ALU(XOR_E) }, // XOR E
	{ FETCH_ALU(XOR_H) }, // XOR H
	{ FETCH_ALU(XOR_L) }, // XOR L
	{ { addressbus_operation::PUSH_HL, databus_operation::POP_Z }, FETCH_ALU(XOR_Z) }, // XOR (HL)
	{ FETCH_ALU(XOR_A) }, // XOR A

	{ FETCH_ALU(OR_B) }, // OR B
	{ FETCH_ALU(OR_C) }, // OR C
	{ FETCH_ALU(OR_D) }, // OR D
	{ FETCH_ALU(OR_E) }, // OR E
	{ FETCH_ALU(OR_H) }, // OR H
	{ FETCH_ALU(OR_L) }, // OR L
	{ { addressbus_operation::PUSH_HL, databus_operation::POP_Z }, FETCH_ALU(OR_Z) }, // OR (HL)
	{ FETCH_ALU(OR_A) }, // OR A
	{ FETCH_ALU(CP_B) }, // CP B
	{ FETCH_ALU(CP_C) }, // CP C
	{ FETCH_ALU(CP_D) }, // CP D
	{ FETCH_ALU(CP_E) }, // CP E
	{ FETCH_ALU(CP_H) }, // CP H
	{ FETCH_ALU(CP_L) }, // CP L
	{ { addressbus_operation::PUSH_HL, databus_operation::POP_Z }, FETCH_ALU(CP_Z) }, // CP (HL)
	{ FETCH_ALU(CP_A) }, // CP A

	{ {.misc = misc_operation::CHECK_NZ }, { addressbus_operation::PUSH_SP, databus_operation::POP_Z, idu_operation::INC_SP }, { addressbus_operation::PUSH_SP, databus_operation::POP_W, idu_operation::INC_SP }, {.misc = misc_operation::WZ_TO_PC }, FETCH }, // RET NZ
	{ { addressbus_operation::PUSH_SP, databus_operation::POP_Z, idu_operation::INC_SP }, { addressbus_operation::PUSH_SP, databus_operation::POP_W, idu_operation::INC_SP}, FETCH_MISC(WZ_TO_BC) }, // POP BC
	{ READZ, READW_MISC(CHECK_NZ), {.misc = misc_operation::WZ_TO_PC }, FETCH }, // JP NZ,nn
	{ READZ, READW, { .misc = misc_operation::WZ_TO_PC }, FETCH }, // JP nn
	{ READZ, READW_MISC(CHECK_NZ), {.idu = idu_operation::DEC_SP}, {addressbus_operation::PUSH_SP, databus_operation::PUSH_PCH, idu_operation::DEC_SP}, {addressbus_operation::PUSH_SP, databus_operation::PUSH_PCL, idu_operation::NONE, alu_operation::NONE, misc_operation::WZ_TO_PC}, FETCH}, // CALL NZ,nn
	{ {.idu = idu_operation::DEC_SP }, { addressbus_operation::PUSH_SP, databus_operation::PUSH_B, idu_operation::DEC_SP }, { addressbus_operation::PUSH_SP, databus_operation::PUSH_C }, FETCH }, // PUSH BC
	{ READZ, FETCH_ALU(ADD_Z_TO_A) }, // ADD A,n
	{ {.idu = idu_operation::DEC_SP}, {addressbus_operation::PUSH_SP, databus_operation::PUSH_PCH, idu_operation::DEC_SP}, {addressbus_operation::PUSH_SP, databus_operation::PUSH_PCL, idu_operation::NONE, alu_operation::NONE, misc_operation::RST0_TO_PC}, FETCH }, // RST 0
	{ {.misc = misc_operation::CHECK_Z }, { addressbus_operation::PUSH_SP, databus_operation::POP_Z, idu_operation::INC_SP }, { addressbus_operation::PUSH_SP, databus_operation::POP_W, idu_operation::INC_SP }, {.misc = misc_operation::WZ_TO_PC }, FETCH }, // RET Z
	{ { addressbus_operation::PUSH_SP, databus_operation::POP_Z, idu_operation::INC_SP }, { addressbus_operation::PUSH_SP, databus_operation::POP_W, idu_operation::INC_SP }, {.misc = misc_operation::WZ_TO_PC }, FETCH }, // RET
	{ READZ, READW_MISC(CHECK_Z), {.misc = misc_operation::WZ_TO_PC }, FETCH }, // JP Z,nn
	{ FETCH_MISC(CB) }, // CB prefix
	{ READZ, READW_MISC(CHECK_Z), {.idu = idu_operation::DEC_SP}, {addressbus_operation::PUSH_SP, databus_operation::PUSH_PCH, idu_operation::DEC_SP}, {addressbus_operation::PUSH_SP, databus_operation::PUSH_PCL, idu_operation::NONE, alu_operation::NONE, misc_operation::WZ_TO_PC}, FETCH }, // CALL Z,nn
	{ READZ, READW, {.idu = idu_operation::DEC_SP }, { addressbus_operation::PUSH_SP, databus_operation::PUSH_PCH, idu_operation::DEC_SP }, { addressbus_operation::PUSH_SP, databus_operation::PUSH_PCL, idu_operation::NONE, alu_operation::NONE, misc_operation::WZ_TO_PC }, FETCH }, // CALL nn
	{ READZ, FETCH_ALU(ADC_Z_TO_A) }, // ADC A,n
	{ {.idu = idu_operation::DEC_SP}, {addressbus_operation::PUSH_SP, databus_operation::PUSH_PCH, idu_operation::DEC_SP}, {addressbus_operation::PUSH_SP, databus_operation::PUSH_PCL, idu_operation::NONE, alu_operation::NONE, misc_operation::RST8_TO_PC}, FETCH }, // RST 08

	{ {.misc = misc_operation::CHECK_NC }, { addressbus_operation::PUSH_SP, databus_operation::POP_Z, idu_operation::INC_SP }, { addressbus_operation::PUSH_SP, databus_operation::POP_W, idu_operation::INC_SP }, {.misc = misc_operation::WZ_TO_PC }, FETCH }, // RET NC
	{ { addressbus_operation::PUSH_SP, databus_operation::POP_Z, idu_operation::INC_SP }, { addressbus_operation::PUSH_SP, databus_operation::POP_W, idu_operation::INC_SP}, FETCH_MISC(WZ_TO_DE) }, // POP DE
	{ READZ, READW_MISC(CHECK_NC), {.misc = misc_operation::WZ_TO_PC }, FETCH }, // JP NC,nn
	{ },
	{ READZ, READW_MISC(CHECK_NC), {.idu = idu_operation::DEC_SP}, {addressbus_operation::PUSH_SP, databus_operation::PUSH_PCH, idu_operation::DEC_SP}, {addressbus_operation::PUSH_SP, databus_operation::PUSH_PCL, idu_operation::NONE, alu_operation::NONE, misc_operation::WZ_TO_PC}, FETCH }, // CALL NC,nn
	{ {.idu = idu_operation::DEC_SP }, { addressbus_operation::PUSH_SP, databus_operation::PUSH_D, idu_operation::DEC_SP }, { addressbus_operation::PUSH_SP, databus_operation::PUSH_E }, FETCH }, // PUSH DE
	{ READZ, FETCH_ALU(SUB_Z_TO_A) }, // SUB n
	{ {.idu = idu_operation::DEC_SP}, {addressbus_operation::PUSH_SP, databus_operation::PUSH_PCH, idu_operation::DEC_SP}, {addressbus_operation::PUSH_SP, databus_operation::PUSH_PCL, idu_operation::NONE, alu_operation::NONE, misc_operation::RST10_TO_PC}, FETCH }, // RST 10
	{ {.misc = misc_operation::CHECK_C }, { addressbus_operation::PUSH_SP, databus_operation::POP_Z, idu_operation::INC_SP }, { addressbus_operation::PUSH_SP, databus_operation::POP_W, idu_operation::INC_SP }, {.misc = misc_operation::WZ_TO_PC }, FETCH }, // RET C
	{ { addressbus_operation::PUSH_SP, databus_operation::POP_Z, idu_operation::INC_SP }, { addressbus_operation::PUSH_SP, databus_operation::POP_W, idu_operation::INC_SP }, {.misc = misc_operation::WZ_TO_PC }, FETCH_MISC(EI) }, // RETI
	{ READZ, READW_MISC(CHECK_C), {.misc = misc_operation::WZ_TO_PC }, FETCH }, // JP C,nn
	{ },
	{ READZ, READW_MISC(CHECK_C), {.idu = idu_operation::DEC_SP}, {addressbus_operation::PUSH_SP, databus_operation::PUSH_PCH, idu_operation::DEC_SP}, {addressbus_operation::PUSH_SP, databus_operation::PUSH_PCL, idu_operation::NONE, alu_operation::NONE, misc_operation::WZ_TO_PC}, FETCH }, // CALL C,nn
	{ },
	{ READZ, FETCH_ALU(SBC_Z_TO_A) }, // SBC n
	{ {.idu = idu_operation::DEC_SP}, {addressbus_operation::PUSH_SP, databus_operation::PUSH_PCH, idu_operation::DEC_SP}, {addressbus_operation::PUSH_SP, databus_operation::PUSH_PCL, idu_operation::NONE, alu_operation::NONE, misc_operation::RST18_TO_PC}, FETCH }, // RST 18

	{ READZ, { addressbus_operation::PUSH_Z, databus_operation::PUSH_A }, FETCH }, // LDH (n),A
	{ { addressbus_operation::PUSH_SP, databus_operation::POP_Z, idu_operation::INC_SP }, { addressbus_operation::PUSH_SP, databus_operation::POP_W, idu_operation::INC_SP}, FETCH_MISC(WZ_TO_HL) }, // POP HL
	{ {addressbus_operation::PUSH_C, databus_operation::PUSH_A}, FETCH }, // LD (C),A
	{ },
	{ },
	{ {.idu = idu_operation::DEC_SP }, { addressbus_operation::PUSH_SP, databus_operation::PUSH_H, idu_operation::DEC_SP }, { addressbus_operation::PUSH_SP, databus_operation::PUSH_L }, FETCH }, // PUSH HL
	{ READZ, FETCH_ALU(AND_Z) }, // AND n
	{ {.idu = idu_operation::DEC_SP}, {addressbus_operation::PUSH_SP, databus_operation::PUSH_PCH, idu_operation::DEC_SP}, {addressbus_operation::PUSH_SP, databus_operation::PUSH_PCL, idu_operation::NONE, alu_operation::NONE, misc_operation::RST20_TO_PC}, FETCH }, // RST 20
	{ READZ, {.idu=idu_operation::ADD_Z_TO_SP }, {.misc=misc_operation::WZ_TO_SP}, FETCH }, // ADD SP,n
	{ {addressbus_operation::PUSH_HL, databus_operation::POP_IR, idu_operation::HL_1_TO_PC} }, // JP (HL)
	{ READZ, READW, { addressbus_operation::PUSH_WZ, databus_operation::PUSH_A }, FETCH }, // LD (nn),A
	{ },
	{ },
	{ },
	{ READZ, FETCH_ALU(XOR_Z) }, // XOR n
	{ {.idu = idu_operation::DEC_SP}, {addressbus_operation::PUSH_SP, databus_operation::PUSH_PCH, idu_operation::DEC_SP}, {addressbus_operation::PUSH_SP, databus_operation::PUSH_PCL, idu_operation::NONE, alu_operation::NONE, misc_operation::RST28_TO_PC}, FETCH }, // RST 28

	{ READZ, { addressbus_operation::PUSH_Z, databus_operation::POP_Z }, FETCH_ALU(Z_TO_A) }, // LDH A,(n)
	{ { addressbus_operation::PUSH_SP, databus_operation::POP_Z, idu_operation::INC_SP }, { addressbus_operation::PUSH_SP, databus_operation::POP_W, idu_operation::INC_SP}, FETCH_MISC(WZ_TO_AF) }, // POP AF
	{ {addressbus_operation::PUSH_C, databus_operation::POP_Z}, FETCH_ALU(Z_TO_A) }, // LD A,(C)
	{ FETCH_MISC(DI) }, // DI
	{ },
	{ {.idu = idu_operation::DEC_SP }, { addressbus_operation::PUSH_SP, databus_operation::PUSH_A, idu_operation::DEC_SP }, { addressbus_operation::PUSH_SP, databus_operation::PUSH_F }, FETCH }, // PUSH AF
	{ READZ, FETCH_ALU(OR_Z) }, // OR n
	{ {.idu = idu_operation::DEC_SP}, {addressbus_operation::PUSH_SP, databus_operation::PUSH_PCH, idu_operation::DEC_SP}, {addressbus_operation::PUSH_SP, databus_operation::PUSH_PCL, idu_operation::NONE, alu_operation::NONE, misc_operation::RST30_TO_PC}, FETCH }, // RST 30
	{ READZ, {.idu=idu_operation::ADD_Z_TO_SP}, FETCH_MISC(WZ_TO_HL) }, // LD HL,SP+n
	{ {.idu=idu_operation::HL_TO_SP}, FETCH }, // LD SP,HL
	{ READZ, READW, { addressbus_operation::PUSH_WZ, databus_operation::POP_Z }, FETCH_ALU(Z_TO_A) }, // LD A,(nn)
	{ FETCH_MISC(EI) }, // EI
	{ },
	{ },
	{ READZ, FETCH_ALU(CP_Z) }, // CP n
	{ {.idu = idu_operation::DEC_SP}, {addressbus_operation::PUSH_SP, databus_operation::PUSH_PCH, idu_operation::DEC_SP}, {addressbus_operation::PUSH_SP, databus_operation::PUSH_PCL, idu_operation::NONE, alu_operation::NONE, misc_operation::RST38_TO_PC}, FETCH }, // RST 38

	// CB
	{ FETCH_ALU(RLC_B) }, // RLC B
	{ FETCH_ALU(RLC_C) }, // RLC C
	{ FETCH_ALU(RLC_D) }, // RLC D
	{ FETCH_ALU(RLC_E) }, // RLC E
	{ FETCH_ALU(RLC_H) }, // RLC H
	{ FETCH_ALU(RLC_L) }, // RLC L
	OP_HL(RLC_Z), // RLC (HL)
	{ FETCH_ALU(RLC_A) }, // RLC A
	{ FETCH_ALU(RRC_B) }, // RRC B
	{ FETCH_ALU(RRC_C) }, // RRC C
	{ FETCH_ALU(RRC_D) }, // RRC D
	{ FETCH_ALU(RRC_E) }, // RRC E
	{ FETCH_ALU(RRC_H) }, // RRC H
	{ FETCH_ALU(RRC_L) }, // RRC L
	OP_HL(RRC_Z), // RRC (HL)
	{ FETCH_ALU(RRC_A) }, // RRC A

	{ FETCH_ALU(RL_B) }, // RL B
	{ FETCH_ALU(RL_C) }, // RL C
	{ FETCH_ALU(RL_D) }, // RL D
	{ FETCH_ALU(RL_E) }, // RL E
	{ FETCH_ALU(RL_H) }, // RL H
	{ FETCH_ALU(RL_L) }, // RL L
	OP_HL(RL_Z), // RL (HL)
	{ FETCH_ALU(RL_A) }, // RL A
	{ FETCH_ALU(RR_B) }, // RR B
	{ FETCH_ALU(RR_C) }, // RR C
	{ FETCH_ALU(RR_D) }, // RR D
	{ FETCH_ALU(RR_E) }, // RR E
	{ FETCH_ALU(RR_H) }, // RR H
	{ FETCH_ALU(RR_L) }, // RR L
	OP_HL(RR_Z), // RR (HL)
	{ FETCH_ALU(RR_A) }, // RR A

	{ FETCH_ALU(SLA_B) }, // SLA B
	{ FETCH_ALU(SLA_C) }, // SLA C
	{ FETCH_ALU(SLA_D) }, // SLA D
	{ FETCH_ALU(SLA_E) }, // SLA E
	{ FETCH_ALU(SLA_H) }, // SLA H
	{ FETCH_ALU(SLA_L) }, // SLA L
	OP_HL(SLA_Z), // SLA (HL)
	{ FETCH_ALU(SLA_A) }, // SLA A
	{ FETCH_ALU(SRA_B) }, // SRA B
	{ FETCH_ALU(SRA_C) }, // SRA C
	{ FETCH_ALU(SRA_D) }, // SRA D
	{ FETCH_ALU(SRA_E) }, // SRA E
	{ FETCH_ALU(SRA_H) }, // SRA H
	{ FETCH_ALU(SRA_L) }, // SRA L
	OP_HL(SRA_Z), // SRA (HL)
	{ FETCH_ALU(SRA_A) }, // SRA A

	{ FETCH_ALU(SWAP_B) }, // SWAP B
	{ FETCH_ALU(SWAP_C) }, // SWAP C
	{ FETCH_ALU(SWAP_D) }, // SWAP D
	{ FETCH_ALU(SWAP_E) }, // SWAP E
	{ FETCH_ALU(SWAP_H) }, // SWAP H
	{ FETCH_ALU(SWAP_L) }, // SWAP L
	OP_HL(SWAP_Z), // SWAP (HL)
	{ FETCH_ALU(SWAP_A) }, // SWAP A
	{ FETCH_ALU(SRL_B) }, // SRL B
	{ FETCH_ALU(SRL_C) }, // SRL C
	{ FETCH_ALU(SRL_D) }, // SRL D
	{ FETCH_ALU(SRL_E) }, // SRL E
	{ FETCH_ALU(SRL_H) }, // SRL H
	{ FETCH_ALU(SRL_L) }, // SRL L
	OP_HL(SRL_Z), // SRL (HL)
	{ FETCH_ALU(SRL_A) }, // SRL A

	{ FETCH_ALU(BIT_0_B) }, // BIT 0,B
	{ FETCH_ALU(BIT_0_C) }, // BIT 0,C
	{ FETCH_ALU(BIT_0_D) }, // BIT 0,D
	{ FETCH_ALU(BIT_0_E) }, // BIT 0,E
	{ FETCH_ALU(BIT_0_H) }, // BIT 0,H
	{ FETCH_ALU(BIT_0_L) }, // BIT 0,L
	OP_HL(BIT_0_Z), // BIT 0,(HL)
	{ FETCH_ALU(BIT_0_A) }, // BIT 0,A
	{ FETCH_ALU(BIT_1_B) }, // BIT 1,B
	{ FETCH_ALU(BIT_1_C) }, // BIT 1,C
	{ FETCH_ALU(BIT_1_D) }, // BIT 1,D
	{ FETCH_ALU(BIT_1_E) }, // BIT 1,E
	{ FETCH_ALU(BIT_1_H) }, // BIT 1,H
	{ FETCH_ALU(BIT_1_L) }, // BIT 1,L
	OP_HL(BIT_1_Z), // BIT 1,(HL)
	{ FETCH_ALU(BIT_1_A) }, // BIT 1,A

	{ FETCH_ALU(BIT_2_B) }, // BIT 2,B
	{ FETCH_ALU(BIT_2_C) }, // BIT 2,C
	{ FETCH_ALU(BIT_2_D) }, // BIT 2,D
	{ FETCH_ALU(BIT_2_E) }, // BIT 2,E
	{ FETCH_ALU(BIT_2_H) }, // BIT 2,H
	{ FETCH_ALU(BIT_2_L) }, // BIT 2,L
	OP_HL(BIT_2_Z), // BIT 2,(HL)
	{ FETCH_ALU(BIT_2_A) }, // BIT 2,A
	{ FETCH_ALU(BIT_3_B) }, // BIT 3,B
	{ FETCH_ALU(BIT_3_C) }, // BIT 3,C
	{ FETCH_ALU(BIT_3_D) }, // BIT 3,D
	{ FETCH_ALU(BIT_3_E) }, // BIT 3,E
	{ FETCH_ALU(BIT_3_H) }, // BIT 3,H
	{ FETCH_ALU(BIT_3_L) }, // BIT 3,L
	OP_HL(BIT_3_Z), // BIT 3,(HL)
	{ FETCH_ALU(BIT_3_A) }, // BIT 3,A

	{ FETCH_ALU(BIT_4_B) }, // BIT 4,B
	{ FETCH_ALU(BIT_4_C) }, // BIT 4,C
	{ FETCH_ALU(BIT_4_D) }, // BIT 4,D
	{ FETCH_ALU(BIT_4_E) }, // BIT 4,E
	{ FETCH_ALU(BIT_4_H) }, // BIT 4,H
	{ FETCH_ALU(BIT_4_L) }, // BIT 4,L
	OP_HL(BIT_4_Z), // BIT 4,(HL)
	{ FETCH_ALU(BIT_4_A) }, // BIT 4,A
	{ FETCH_ALU(BIT_5_B) }, // BIT 5,B
	{ FETCH_ALU(BIT_5_C) }, // BIT 5,C
	{ FETCH_ALU(BIT_5_D) }, // BIT 5,D
	{ FETCH_ALU(BIT_5_E) }, // BIT 5,E
	{ FETCH_ALU(BIT_5_H) }, // BIT 5,H
	{ FETCH_ALU(BIT_5_L) }, // BIT 5,L
	OP_HL(BIT_5_Z), // BIT 5,(HL)
	{ FETCH_ALU(BIT_5_A) }, // BIT 5,A

	{ FETCH_ALU(BIT_6_B) }, // BIT 6,B
	{ FETCH_ALU(BIT_6_C) }, // BIT 6,C
	{ FETCH_ALU(BIT_6_D) }, // BIT 6,D
	{ FETCH_ALU(BIT_6_E) }, // BIT 6,E
	{ FETCH_ALU(BIT_6_H) }, // BIT 6,H
	{ FETCH_ALU(BIT_6_L) }, // BIT 6,L
	OP_HL(BIT_6_Z), // BIT 6,(HL)
	{ FETCH_ALU(BIT_6_A) }, // BIT 6,A
	{ FETCH_ALU(BIT_7_B) }, // BIT 7,B
	{ FETCH_ALU(BIT_7_C) }, // BIT 7,C
	{ FETCH_ALU(BIT_7_D) }, // BIT 7,D
	{ FETCH_ALU(BIT_7_E) }, // BIT 7,E
	{ FETCH_ALU(BIT_7_H) }, // BIT 7,H
	{ FETCH_ALU(BIT_7_L) }, // BIT 7,L
	OP_HL(BIT_7_Z), // BIT 7,(HL)
	{ FETCH_ALU(BIT_7_A) }, // BIT 7,A

	{ FETCH_ALU(RES_0_B) }, // RES 0,B
	{ FETCH_ALU(RES_0_C) }, // RES 0,C
	{ FETCH_ALU(RES_0_D) }, // RES 0,D
	{ FETCH_ALU(RES_0_E) }, // RES 0,E
	{ FETCH_ALU(RES_0_H) }, // RES 0,H
	{ FETCH_ALU(RES_0_L) }, // RES 0,L
	OP_HL(RES_0_Z), // RES 0,(HL)
	{ FETCH_ALU(RES_0_A) }, // RES 0,A
	{ FETCH_ALU(RES_1_B) }, // RES 1,B
	{ FETCH_ALU(RES_1_C) }, // RES 1,C
	{ FETCH_ALU(RES_1_D) }, // RES 1,D
	{ FETCH_ALU(RES_1_E) }, // RES 1,E
	{ FETCH_ALU(RES_1_H) }, // RES 1,H
	{ FETCH_ALU(RES_1_L) }, // RES 1,L
	OP_HL(RES_1_Z), // RES 1,(HL)
	{ FETCH_ALU(RES_1_A) }, // RES 1,A

	{ FETCH_ALU(RES_2_B) }, // RES 2,B
	{ FETCH_ALU(RES_2_C) }, // RES 2,C
	{ FETCH_ALU(RES_2_D) }, // RES 2,D
	{ FETCH_ALU(RES_2_E) }, // RES 2,E
	{ FETCH_ALU(RES_2_H) }, // RES 2,H
	{ FETCH_ALU(RES_2_L) }, // RES 2,L
	OP_HL(RES_2_Z), // RES 2,(HL)
	{ FETCH_ALU(RES_2_A) }, // RES 2,A
	{ FETCH_ALU(RES_3_B) }, // RES 3,B
	{ FETCH_ALU(RES_3_C) }, // RES 3,C
	{ FETCH_ALU(RES_3_D) }, // RES 3,D
	{ FETCH_ALU(RES_3_E) }, // RES 3,E
	{ FETCH_ALU(RES_3_H) }, // RES 3,H
	{ FETCH_ALU(RES_3_L) }, // RES 3,L
	OP_HL(RES_3_Z), // RES 3,(HL)
	{ FETCH_ALU(RES_3_A) }, // RES 3,A

	{ FETCH_ALU(RES_4_B) }, // RES 4,B
	{ FETCH_ALU(RES_4_C) }, // RES 4,C
	{ FETCH_ALU(RES_4_D) }, // RES 4,D
	{ FETCH_ALU(RES_4_E) }, // RES 4,E
	{ FETCH_ALU(RES_4_H) }, // RES 4,H
	{ FETCH_ALU(RES_4_L) }, // RES 4,L
	OP_HL(RES_4_Z), // RES 4,(HL)
	{ FETCH_ALU(RES_4_A) }, // RES 4,A
	{ FETCH_ALU(RES_5_B) }, // RES 5,B
	{ FETCH_ALU(RES_5_C) }, // RES 5,C
	{ FETCH_ALU(RES_5_D) }, // RES 5,D
	{ FETCH_ALU(RES_5_E) }, // RES 5,E
	{ FETCH_ALU(RES_5_H) }, // RES 5,H
	{ FETCH_ALU(RES_5_L) }, // RES 5,L
	OP_HL(RES_5_Z), // RES 5,(HL)
	{ FETCH_ALU(RES_5_A) }, // RES 5,A

	{ FETCH_ALU(RES_6_B) }, // RES 6,B
	{ FETCH_ALU(RES_6_C) }, // RES 6,C
	{ FETCH_ALU(RES_6_D) }, // RES 6,D
	{ FETCH_ALU(RES_6_E) }, // RES 6,E
	{ FETCH_ALU(RES_6_H) }, // RES 6,H
	{ FETCH_ALU(RES_6_L) }, // RES 6,L
	OP_HL(RES_6_Z), // RES 6,(HL)
	{ FETCH_ALU(RES_6_A) }, // RES 6,A
	{ FETCH_ALU(RES_7_B) }, // RES 7,B
	{ FETCH_ALU(RES_7_C) }, // RES 7,C
	{ FETCH_ALU(RES_7_D) }, // RES 7,D
	{ FETCH_ALU(RES_7_E) }, // RES 7,E
	{ FETCH_ALU(RES_7_H) }, // RES 7,H
	{ FETCH_ALU(RES_7_L) }, // RES 7,L
	OP_HL(RES_7_Z), // RES 7,(HL)
	{ FETCH_ALU(RES_7_A) }, // RES 7,A

	{ FETCH_ALU(SET_0_B) }, // SET 0,B
	{ FETCH_ALU(SET_0_C) }, // SET 0,C
	{ FETCH_ALU(SET_0_D) }, // SET 0,D
	{ FETCH_ALU(SET_0_E) }, // SET 0,E
	{ FETCH_ALU(SET_0_H) }, // SET 0,H
	{ FETCH_ALU(SET_0_L) }, // SET 0,L
	OP_HL(SET_0_Z), // SET 0,(HL)
	{ FETCH_ALU(SET_0_A) }, // SET 0,A
	{ FETCH_ALU(SET_1_B) }, // SET 1,B
	{ FETCH_ALU(SET_1_C) }, // SET 1,C
	{ FETCH_ALU(SET_1_D) }, // SET 1,D
	{ FETCH_ALU(SET_1_E) }, // SET 1,E
	{ FETCH_ALU(SET_1_H) }, // SET 1,H
	{ FETCH_ALU(SET_1_L) }, // SET 1,L
	OP_HL(SET_1_Z), // SET 1,(HL)
	{ FETCH_ALU(SET_1_A) }, // SET 1,A

	{ FETCH_ALU(SET_2_B) }, // SET 2,B
	{ FETCH_ALU(SET_2_C) }, // SET 2,C
	{ FETCH_ALU(SET_2_D) }, // SET 2,D
	{ FETCH_ALU(SET_2_E) }, // SET 2,E
	{ FETCH_ALU(SET_2_H) }, // SET 2,H
	{ FETCH_ALU(SET_2_L) }, // SET 2,L
	OP_HL(SET_2_Z), // SET 2,(HL)
	{ FETCH_ALU(SET_2_A) }, // SET 2,A
	{ FETCH_ALU(SET_3_B) }, // SET 3,B
	{ FETCH_ALU(SET_3_C) }, // SET 3,C
	{ FETCH_ALU(SET_3_D) }, // SET 3,D
	{ FETCH_ALU(SET_3_E) }, // SET 3,E
	{ FETCH_ALU(SET_3_H) }, // SET 3,H
	{ FETCH_ALU(SET_3_L) }, // SET 3,L
	OP_HL(SET_3_Z), // SET 3,(HL)
	{ FETCH_ALU(SET_3_A) }, // SET 3,A

	{ FETCH_ALU(SET_4_B) }, // SET 4,B
	{ FETCH_ALU(SET_4_C) }, // SET 4,C
	{ FETCH_ALU(SET_4_D) }, // SET 4,D
	{ FETCH_ALU(SET_4_E) }, // SET 4,E
	{ FETCH_ALU(SET_4_H) }, // SET 4,H
	{ FETCH_ALU(SET_4_L) }, // SET 4,L
	OP_HL(SET_4_Z), // SET 4,(HL)
	{ FETCH_ALU(SET_4_A) }, // SET 4,A
	{ FETCH_ALU(SET_5_B) }, // SET 5,B
	{ FETCH_ALU(SET_5_C) }, // SET 5,C
	{ FETCH_ALU(SET_5_D) }, // SET 5,D
	{ FETCH_ALU(SET_5_E) }, // SET 5,E
	{ FETCH_ALU(SET_5_H) }, // SET 5,H
	{ FETCH_ALU(SET_5_L) }, // SET 5,L
	OP_HL(SET_5_Z), // SET 5,(HL)
	{ FETCH_ALU(SET_5_A) }, // SET 5,A

	{ FETCH_ALU(SET_6_B) }, // SET 6,B
	{ FETCH_ALU(SET_6_C) }, // SET 6,C
	{ FETCH_ALU(SET_6_D) }, // SET 6,D
	{ FETCH_ALU(SET_6_E) }, // SET 6,E
	{ FETCH_ALU(SET_6_H) }, // SET 6,H
	{ FETCH_ALU(SET_6_L) }, // SET 6,L
	OP_HL(SET_6_Z), // SET 6,(HL)
	{ FETCH_ALU(SET_6_A) }, // SET 6,A
	{ FETCH_ALU(SET_7_B) }, // SET 7,B
	{ FETCH_ALU(SET_7_C) }, // SET 7,C
	{ FETCH_ALU(SET_7_D) }, // SET 7,D
	{ FETCH_ALU(SET_7_E) }, // SET 7,E
	{ FETCH_ALU(SET_7_H) }, // SET 7,H
	{ FETCH_ALU(SET_7_L) }, // SET 7,L
	OP_HL(SET_7_Z), // SET 7,(HL)
	{ FETCH_ALU(SET_7_A) }, // SET 7,A
};
