#include "z80debugging.h"


using namespace xprocessors::cpu;


uint8_t getMemory(std::function<uint8_t(const uint16_t)> memory_fn, const uint16_t address)
{
	return memory_fn(address);
}
unsigned int getN(std::function<uint8_t(const uint16_t)> memory_fn, const uint16_t address)
{
	return static_cast<unsigned int>(memory_fn(address));
}
unsigned int getNN(std::function<uint8_t(const uint16_t)> memory_fn, const uint16_t address)
{
	return static_cast<unsigned int>(memory_fn(address) | (memory_fn(address+1)<<8));
}

void Z80debugging::disassemblyopcode(const Z80* cpu, std::function<uint8_t(const uint16_t)> memory_fn)
{
	_disasm_fd << std::hex << cpu->pc << " ";
	switch (getMemory(memory_fn, cpu->pc))
	{
	case 0x00:
		_disasm_fd << "NOP";
		break;
	case 0x01:
		_disasm_fd << "LD BC," << getNN(memory_fn, cpu->pc+1);
		break;
	case 0x02:
		_disasm_fd << "LD (BC),A";
		break;
	case 0x03:
		_disasm_fd << "INC BC";
		break;
	case 0x04:
		_disasm_fd << "INC B";
		break;
	case 0x05:
		_disasm_fd << "DEC B";
		break;
	case 0x06:
		_disasm_fd << "LD B," << getN(memory_fn, cpu->pc + 1);
		break;
	case 0x07:
		_disasm_fd << "RLCA";
		break;
	case 0x08:
		_disasm_fd << "EX AF,AF'";
		break;
	case 0x09:
		_disasm_fd << "ADD HL,BC";
		break;
	case 0x0a:
		_disasm_fd << "LD A,(BC)";
		break;
	case 0x0b:
		_disasm_fd << "DEC BC";
		break;
	case 0x0c:
		_disasm_fd << "INC C";
		break;
	case 0x0d:
		_disasm_fd << "DEC C";
		break;
	case 0x0e:
		_disasm_fd << "LD C," << getN(memory_fn, cpu->pc + 1);
		break;
	case 0x0f:
		_disasm_fd << "RRCA";
		break;
	case 0x10:
		_disasm_fd << "DJNZ " << getN(memory_fn, cpu->pc + 1);
		break;
	case 0x11:
		_disasm_fd << "LD DE," << getNN(memory_fn, cpu->pc + 1);
		break;
	case 0x12:
		_disasm_fd << "LD (DE),A";
		break;
	case 0x13:
		_disasm_fd << "INC DE";
		break;
	case 0x14:
		_disasm_fd << "INC D";
		break;
	case 0x15:
		_disasm_fd << "DEC D";
		break;
	case 0x16:
		_disasm_fd << "LD D," << getN(memory_fn, cpu->pc + 1);
		break;
	case 0x17:
		_disasm_fd << "RLA";
		break;
	case 0x18:
		_disasm_fd << "JR " << getN(memory_fn, cpu->pc + 1);
		break;
	case 0x19:
		_disasm_fd << "ADD HL,DE";
		break;
	case 0x1a:
		_disasm_fd << "LD A,(DE)";
		break;
	case 0x1b:
		_disasm_fd << "DEC DE";
		break;
	case 0x1c:
		_disasm_fd << "INC E";
		break;
	case 0x1d:
		_disasm_fd << "DEC E";
		break;
	case 0x1e:
		_disasm_fd << "LD E," << getN(memory_fn, cpu->pc + 1);
		break;
	case 0x1f:
		_disasm_fd << "RRA";
		break;
	case 0x20:
		_disasm_fd << "JR NZ," << getN(memory_fn, cpu->pc + 1);
		break;
	case 0x21:
		_disasm_fd << "LD HL," << getNN(memory_fn, cpu->pc + 1);
		break;
	case 0x22:
		_disasm_fd << "LD (" << getNN(memory_fn, cpu->pc + 1) << "),HL";
		break;
	case 0x23:
		_disasm_fd << "INC HL";
		break;
	case 0x24:
		_disasm_fd << "INC H";
		break;
	case 0x25:
		_disasm_fd << "DEC H";
		break;
	case 0x26:
		_disasm_fd << "LD H," << getN(memory_fn, cpu->pc + 1);
		break;
	case 0x27:
		_disasm_fd << "DAA";
		break;
	case 0x28:
		_disasm_fd << "JR Z," << getN(memory_fn, cpu->pc + 1);
		break;
	case 0x29:
		_disasm_fd << "ADD HL,HL";
		break;
	case 0x2a:
		_disasm_fd << "LD HL,(" << getNN(memory_fn, cpu->pc + 1) << ")";
		break;
	case 0x2b:
		_disasm_fd << "DEC HL";
		break;
	case 0x2c:
		_disasm_fd << "INC L";
		break;
	case 0x2d:
		_disasm_fd << "DEC L";
		break;
	case 0x2e:
		_disasm_fd << "LD L," << getN(memory_fn, cpu->pc + 1);
		break;
	case 0x2f:
		_disasm_fd << "CPL";
		break;
	case 0x30:
		_disasm_fd << "JR NC," << getN(memory_fn, cpu->pc + 1);
		break;
	case 0x31:
		_disasm_fd << "LD SP," << getNN(memory_fn, cpu->pc + 1);
		break;
	case 0x32:
		_disasm_fd << "LD (" << getNN(memory_fn, cpu->pc + 1) <<"),A";
		break;
	case 0x33:
		_disasm_fd << "INC SP";
		break;
	case 0x34:
		_disasm_fd << "INC (HL)";
		break;
	case 0x35:
		_disasm_fd << "DEC (HL)";
		break;
	case 0x36:
		_disasm_fd << "LD (HL)," << getN(memory_fn, cpu->pc + 1);
		break;
	case 0x37:
		_disasm_fd << "SCF";
		break;
	case 0x38:
		_disasm_fd << "JR C," << getN(memory_fn, cpu->pc + 1);
		break;
	case 0x39:
		_disasm_fd << "ADD HL,SP";
		break;
	case 0x3a:
		_disasm_fd << "LD A,(" << getNN(memory_fn, cpu->pc + 1) << ")";
		break;
	case 0x3b:
		_disasm_fd << "DEC SP";
		break;
	case 0x3c:
		_disasm_fd << "INC A";
		break;
	case 0x3d:
		_disasm_fd << "DEC A";
		break;
	case 0x3e:
		_disasm_fd << "LD A," << getN(memory_fn, cpu->pc + 1);
		break;
	case 0x3f:
		_disasm_fd << "CCF";
		break;
	case 0x40:
		_disasm_fd << "LD B,B";
		break;
	case 0x41:
		_disasm_fd << "LD B,C";
		break;
	case 0x42:
		_disasm_fd << "LD B,D";
		break;
	case 0x43:
		_disasm_fd << "LD B,E";
		break;
	case 0x44:
		_disasm_fd << "LD B,H";
		break;
	case 0x45:
		_disasm_fd << "LD B,L";
		break;
	case 0x46:
		_disasm_fd << "LD B,(HL)";
		break;
	case 0x47:
		_disasm_fd << "LD B,A";
		break;
	case 0x48:
		_disasm_fd << "LD C,B";
		break;
	case 0x49:
		_disasm_fd << "LD C,C";
		break;
	case 0x4a:
		_disasm_fd << "LD C,D";
		break;
	case 0x4b:
		_disasm_fd << "LD C,E";
		break;
	case 0x4c:
		_disasm_fd << "LD C,H";
		break;
	case 0x4d:
		_disasm_fd << "LD C,L";
		break;
	case 0x4e:
		_disasm_fd << "LD C,(HL)";
		break;
	case 0x4f:
		_disasm_fd << "LD C,A";
		break;
	case 0x50:
		_disasm_fd << "LD D,B";
		break;
	case 0x51:
		_disasm_fd << "LD D,C";
		break;
	case 0x52:
		_disasm_fd << "LD D,D";
		break;
	case 0x53:
		_disasm_fd << "LD D,E";
		break;
	case 0x54:
		_disasm_fd << "LD D,H";
		break;
	case 0x55:
		_disasm_fd << "LD D,L";
		break;
	case 0x56:
		_disasm_fd << "LD D,(HL)";
		break;
	case 0x57:
		_disasm_fd << "LD D,A";
		break;
	case 0x58:
		_disasm_fd << "LD E,B";
		break;
	case 0x59:
		_disasm_fd << "LD E,C";
		break;
	case 0x5a:
		_disasm_fd << "LD E,D";
		break;
	case 0x5b:
		_disasm_fd << "LD E,E";
		break;
	case 0x5c:
		_disasm_fd << "LD E,H";
		break;
	case 0x5d:
		_disasm_fd << "LD E,L";
		break;
	case 0x5e:
		_disasm_fd << "LD E,(HL)";
		break;
	case 0x5f:
		_disasm_fd << "LD E,A";
		break;
	case 0x60:
		_disasm_fd << "LD H,B";
		break;
	case 0x61:
		_disasm_fd << "LD H,C";
		break;
	case 0x62:
		_disasm_fd << "LD H,D";
		break;
	case 0x63:
		_disasm_fd << "LD H,E";
		break;
	case 0x64:
		_disasm_fd << "LD H,H";
		break;
	case 0x65:
		_disasm_fd << "LD H,L";
		break;
	case 0x66:
		_disasm_fd << "LD H,(HL)";
		break;
	case 0x67:
		_disasm_fd << "LD H,A";
		break;
	case 0x68:
		_disasm_fd << "LD L,B";
		break;
	case 0x69:
		_disasm_fd << "LD L,C";
		break;
	case 0x6a:
		_disasm_fd << "LD L,D";
		break;
	case 0x6b:
		_disasm_fd << "LD L,E";
		break;
	case 0x6c:
		_disasm_fd << "LD L,H";
		break;
	case 0x6d:
		_disasm_fd << "LD L,L";
		break;
	case 0x6e:
		_disasm_fd << "LD L,(HL)";
		break;
	case 0x6f:
		_disasm_fd << "LD L,A";
		break;
	case 0x70:
		_disasm_fd << "LD (HL),B";
		break;
	case 0x71:
		_disasm_fd << "LD (HL),C";
		break;
	case 0x72:
		_disasm_fd << "LD (HL),D";
		break;
	case 0x73:
		_disasm_fd << "LD (HL),E";
		break;
	case 0x74:
		_disasm_fd << "LD (HL),H";
		break;
	case 0x75:
		_disasm_fd << "LD (HL),L";
		break;
	case 0x76:
		_disasm_fd << "HALT";
		break;
	case 0x77:
		_disasm_fd << "LD (HL),A";
		break;
	case 0x78:
		_disasm_fd << "LD A,B";
		break;
	case 0x79:
		_disasm_fd << "LD A,C";
		break;
	case 0x7a:
		_disasm_fd << "LD A,D";
		break;
	case 0x7b:
		_disasm_fd << "LD A,E";
		break;
	case 0x7c:
		_disasm_fd << "LD A,H";
		break;
	case 0x7d:
		_disasm_fd << "LD A,L";
		break;
	case 0x7e:
		_disasm_fd << "LD A,(HL)";
		break;
	case 0x7f:
		_disasm_fd << "LD A,A";
		break;
	case 0x80:
		_disasm_fd << "ADD A,B";
		break;
	case 0x81:
		_disasm_fd << "ADD A,C";
		break;
	case 0x82:
		_disasm_fd << "ADD A,D";
		break;
	case 0x83:
		_disasm_fd << "ADD A,E";
		break;
	case 0x84:
		_disasm_fd << "ADD A,H";
		break;
	case 0x85:
		_disasm_fd << "ADD A,L";
		break;
	case 0x86:
		_disasm_fd << "ADD A,(HL)";
		break;
	case 0x87:
		_disasm_fd << "ADD A,A";
		break;
	case 0x88:
		_disasm_fd << "ADC A,B";
		break;
	case 0x89:
		_disasm_fd << "ADC A,C";
		break;
	case 0x8a:
		_disasm_fd << "ADC A,D";
		break;
	case 0x8b:
		_disasm_fd << "ADC A,E";
		break;
	case 0x8c:
		_disasm_fd << "ADC A,H";
		break;
	case 0x8d:
		_disasm_fd << "ADC A,L";
		break;
	case 0x8e:
		_disasm_fd << "ADC A,(HL)";
		break;
	case 0x8f:
		_disasm_fd << "ADC A,A";
		break;
	case 0x90:
		_disasm_fd << "SUB A,B";
		break;
	case 0x91:
		_disasm_fd << "SUB A,C";
		break;
	case 0x92:
		_disasm_fd << "SUB A,D";
		break;
	case 0x93:
		_disasm_fd << "SUB A,E";
		break;
	case 0x94:
		_disasm_fd << "SUB A,H";
		break;
	case 0x95:
		_disasm_fd << "SUB A,L";
		break;
	case 0x96:
		_disasm_fd << "SUB A,(HL)";
		break;
	case 0x97:
		_disasm_fd << "SUB A,A";
		break;
	case 0x98:
		_disasm_fd << "SBC A,B";
		break;
	case 0x99:
		_disasm_fd << "SBC A,C";
		break;
	case 0x9a:
		_disasm_fd << "SBC A,D";
		break;
	case 0x9b:
		_disasm_fd << "SBC A,E";
		break;
	case 0x9c:
		_disasm_fd << "SBC A,H";
		break;
	case 0x9d:
		_disasm_fd << "SBC A,L";
		break;
	case 0x9e:
		_disasm_fd << "SBC A,(HL)";
		break;
	case 0x9f:
		_disasm_fd << "SBC A,A";
		break;
	case 0xa0:
		_disasm_fd << "AND B";
		break;
	case 0xa1:
		_disasm_fd << "AND C";
		break;
	case 0xa2:
		_disasm_fd << "AND D";
		break;
	case 0xa3:
		_disasm_fd << "AND E";
		break;
	case 0xa4:
		_disasm_fd << "AND H";
		break;
	case 0xa5:
		_disasm_fd << "AND L";
		break;
	case 0xa6:
		_disasm_fd << "AND (HL)";
		break;
	case 0xa7:
		_disasm_fd << "AND A";
		break;
	case 0xa8:
		_disasm_fd << "XOR B";
		break;
	case 0xa9:
		_disasm_fd << "XOR C";
		break;
	case 0xaa:
		_disasm_fd << "XOR D";
		break;
	case 0xab:
		_disasm_fd << "XOR E";
		break;
	case 0xac:
		_disasm_fd << "XOR H";
		break;
	case 0xad:
		_disasm_fd << "XOR L";
		break;
	case 0xae:
		_disasm_fd << "XOR (HL)";
		break;
	case 0xaf:
		_disasm_fd << "XOR A";
		break;
	case 0xb0:
		_disasm_fd << "OR B";
		break;
	case 0xb1:
		_disasm_fd << "OR C";
		break;
	case 0xb2:
		_disasm_fd << "OR D";
		break;
	case 0xb3:
		_disasm_fd << "OR E";
		break;
	case 0xb4:
		_disasm_fd << "OR H";
		break;
	case 0xb5:
		_disasm_fd << "OR L";
		break;
	case 0xb6:
		_disasm_fd << "OR (HL)";
		break;
	case 0xb7:
		_disasm_fd << "OR A";
		break;
	case 0xb8:
		_disasm_fd << "CP B";
		break;
	case 0xb9:
		_disasm_fd << "CP C";
		break;
	case 0xba:
		_disasm_fd << "CP D";
		break;
	case 0xbb:
		_disasm_fd << "CP E";
		break;
	case 0xbc:
		_disasm_fd << "CP H";
		break;
	case 0xbd:
		_disasm_fd << "CP L";
		break;
	case 0xbe:
		_disasm_fd << "CP (HL)";
		break;
	case 0xbf:
		_disasm_fd << "CP A";
		break;
	case 0xc0:
		_disasm_fd << "RET NZ";
		break;
	case 0xc1:
		_disasm_fd << "POP BC";
		break;
	case 0xc2:
		_disasm_fd << "JP NZ," << getNN(memory_fn, cpu->pc+1);
		break;
	case 0xc3:
		_disasm_fd << "JP " << getNN(memory_fn, cpu->pc + 1);
		break;
	case 0xc4:
		_disasm_fd << "CALL NZ," << getNN(memory_fn, cpu->pc + 1);
		break;
	case 0xc5:
		_disasm_fd << "PUSH BC";
		break;
	case 0xc6:
		_disasm_fd << "ADD A," << getN(memory_fn, cpu->pc + 1);
		break;
	case 0xc7:
		_disasm_fd << "RST 00";
		break;
	case 0xc8:
		_disasm_fd << "RET Z";
		break;
	case 0xc9:
		_disasm_fd << "RET";
		break;
	case 0xca:
		_disasm_fd << "JP Z," << getNN(memory_fn, cpu->pc + 1);
		break;
	case 0xcb:
		_disasm_fd << "CBBB";
		break;
	case 0xcc:
		_disasm_fd << "CALL Z," << getNN(memory_fn, cpu->pc + 1);
		break;
	case 0xcd:
		_disasm_fd << "CALL " << getNN(memory_fn, cpu->pc + 1);
		break;
	case 0xce:
		_disasm_fd << "ADC A," << getN(memory_fn, cpu->pc + 1);
		break;
	case 0xcf:
		_disasm_fd << "RST 08";
		break;
	case 0xd0:
		_disasm_fd << "RET NC";
		break;
	case 0xd1:
		_disasm_fd << "POP DE";
		break;
	case 0xd2:
		_disasm_fd << "JP NC," << getNN(memory_fn, cpu->pc + 1);
		break;
	case 0xd3:
		_disasm_fd << "OUT (" << getN(memory_fn, cpu->pc + 1) << "),A";
		break;
	case 0xd4:
		_disasm_fd << "CALL NC," << getNN(memory_fn, cpu->pc + 1);
		break;
	case 0xd5:
		_disasm_fd << "PUSH DE";
		break;
	case 0xd6:
		_disasm_fd << "SUB " << getN(memory_fn, cpu->pc + 1);
		break;
	case 0xd7:
		_disasm_fd << "RST 10";
		break;
	case 0xd8:
		_disasm_fd << "RET C";
		break;
	case 0xd9:
		_disasm_fd << "EXX";
		break;
	case 0xda:
		_disasm_fd << "JP C," << getNN(memory_fn, cpu->pc + 1);
		break;
	case 0xdb:
		_disasm_fd << "IN A,(" << getN(memory_fn, cpu->pc + 1) << ")";
		break;
	case 0xdc:
		_disasm_fd << "CALL C," << getNN(memory_fn, cpu->pc + 1);
		break;
	case 0xdd:
		disassemblyopcode_dd(cpu, memory_fn);
		break;
	case 0xde:
		_disasm_fd << "SBC A," << getN(memory_fn, cpu->pc + 1);
		break;
	case 0xdf:
		_disasm_fd << "RST 18";
		break;
	case 0xe0:
		_disasm_fd << "RET PO";
		break;
	case 0xe1:
		_disasm_fd << "POP HL";
		break;
	case 0xe2:
		_disasm_fd << "JP PO," << getNN(memory_fn, cpu->pc + 1);
		break;
	case 0xe3:
		_disasm_fd << "EX (SP),HL";
		break;
	case 0xe4:
		_disasm_fd << "CALL PO," << getNN(memory_fn, cpu->pc + 1);
		break;
	case 0xe5:
		_disasm_fd << "PUSH HL";
		break;
	case 0xe6:
		_disasm_fd << "AND " << getN(memory_fn, cpu->pc + 1);
		break;
	case 0xe7:
		_disasm_fd << "RST 20";
		break;
	case 0xe8:
		_disasm_fd << "RET PE";
		break;
	case 0xe9:
		_disasm_fd << "JP (HL)";
		break;
	case 0xea:
		_disasm_fd << "JP PE," << getNN(memory_fn, cpu->pc + 1);
		break;
	case 0xeb:
		_disasm_fd << "EX DE,HL";
		break;
	case 0xec:
		_disasm_fd << "CALL PE," << getNN(memory_fn, cpu->pc + 1);
		break;
	case 0xed:
		disassemblyopcode_ed(cpu, memory_fn);
		break;
	case 0xee:
		_disasm_fd << "XOR " << getN(memory_fn, cpu->pc + 1);
		break;
	case 0xef:
		_disasm_fd << "RST 28";
		break;
	case 0xf0:
		_disasm_fd << "RET P";
		break;
	case 0xf1:
		_disasm_fd << "POP AF";
		break;
	case 0xf2:
		_disasm_fd << "JP P," << getNN(memory_fn, cpu->pc + 1);
		break;
	case 0xf3:
		_disasm_fd << "DI";
		break;
	case 0xf4:
		_disasm_fd << "CALL P," << getNN(memory_fn, cpu->pc + 1);
		break;
	case 0xf5:
		_disasm_fd << "PUSH AF";
		break;
	case 0xf6:
		_disasm_fd << "OR " << getN(memory_fn, cpu->pc + 1);
		break;
	case 0xf7:
		_disasm_fd << "RST 30";
		break;
	case 0xf8:
		_disasm_fd << "RET M";
		break;
	case 0xf9:
		_disasm_fd << "LD SP,HL";
		break;
	case 0xfa:
		_disasm_fd << "JP M," << getNN(memory_fn, cpu->pc + 1);
		break;
	case 0xfb:
		_disasm_fd << "EI";
		break;
	case 0xfc:
		_disasm_fd << "CALL M," << getNN(memory_fn, cpu->pc + 1);
		break;
	case 0xfd:
		disassemblyopcode_fd(cpu, memory_fn);
		break;
	case 0xfe:
		_disasm_fd << "CP " << getN(memory_fn, cpu->pc + 1);
		break;
	case 0xff:
		_disasm_fd << "RST 38";
		break;
	}
	_disasm_fd << std::endl;
}

void Z80debugging::disassemblyopcode_dd(const Z80* cpu, std::function<uint8_t(const uint16_t)> memory_fn)
{
	switch (getMemory(memory_fn, cpu->pc+1))
	{
	case 0x04:
		_disasm_fd << "INC B";
		break;
	case 0x05:
		_disasm_fd << "DEC B";
		break;
	case 0x06:
		_disasm_fd << "LD B," << getN(memory_fn, cpu->pc + 2);
		break;
	case 0x09:
		_disasm_fd << "ADD IX,BC";
		break;
	case 0x0c:
		_disasm_fd << "INC C";
		break;
	case 0x0d:
		_disasm_fd << "DEC C";
		break;
	case 0x0e:
		_disasm_fd << "LD C," << getN(memory_fn, cpu->pc + 2);
		break;
	case 0x14:
		_disasm_fd << "INC D";
		break;
	case 0x15:
		_disasm_fd << "DEC D";
		break;
	case 0x16:
		_disasm_fd << "LD D," << getN(memory_fn, cpu->pc + 2);
		break;
	case 0x19:
		_disasm_fd << "ADD IX,DE";
		break;
	case 0x1c:
		_disasm_fd << "INC E";
		break;
	case 0x1d:
		_disasm_fd << "DEC E";
		break;
	case 0x1e:
		_disasm_fd << "LD E," << getN(memory_fn, cpu->pc + 2);
		break;
	case 0x21:
		_disasm_fd << "LD IX," << getNN(memory_fn, cpu->pc + 2);
		break;
	case 0x22:
		_disasm_fd << "LD (" << getNN(memory_fn, cpu->pc + 2) << "),IX";
		break;
	case 0x23:
		_disasm_fd << "INC IX";
		break;
	case 0x24:
		_disasm_fd << "INC IXh";
		break;
	case 0x25:
		_disasm_fd << "DEC IXh";
		break;
	case 0x26:
		_disasm_fd << "LD IXh," << getN(memory_fn, cpu->pc + 2);
		break;
	case 0x29:
		_disasm_fd << "ADD IX,IX";
		break;
	case 0x2a:
		_disasm_fd << "LD IX,(" << getNN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0x2b:
		_disasm_fd << "DEC IX";
		break;
	case 0x2c:
		_disasm_fd << "INC IXl";
		break;
	case 0x2d:
		_disasm_fd << "DEC IXl";
		break;
	case 0x2e:
		_disasm_fd << "LD IXl," << getN(memory_fn, cpu->pc + 2);
		break;
	case 0x34:
		_disasm_fd << "INC (IX+" << getN(memory_fn, cpu->pc+2) << ")";
		break;
	case 0x35:
		_disasm_fd << "DEC (IX+" << getN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0x36:
		_disasm_fd << "LD (IX+" << getN(memory_fn, cpu->pc + 2) << ")," << getN(memory_fn, cpu->pc + 3);
		break;
	case 0x39:
		_disasm_fd << "ADD IX,SP";
		break;
	case 0x3c:
		_disasm_fd << "INC A";
		break;
	case 0x3d:
		_disasm_fd << "DEC A";
		break;
	case 0x3e:
		_disasm_fd << "LD A," << getN(memory_fn, cpu->pc + 1);
		break;
	case 0x40:
		_disasm_fd << "LD B,B";
		break;
	case 0x41:
		_disasm_fd << "LD B,C";
		break;
	case 0x42:
		_disasm_fd << "LD B,D";
		break;
	case 0x43:
		_disasm_fd << "LD B,E";
		break;
	case 0x44:
		_disasm_fd << "LD B,IXh";
		break;
	case 0x45:
		_disasm_fd << "LD B,IXl";
		break;
	case 0x46:
		_disasm_fd << "LD B,(IX+" << getN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0x47:
		_disasm_fd << "LD B,A";
		break;
	case 0x48:
		_disasm_fd << "LD C,B";
		break;
	case 0x49:
		_disasm_fd << "LD C,C";
		break;
	case 0x4a:
		_disasm_fd << "LD C,D";
		break;
	case 0x4b:
		_disasm_fd << "LD C,E";
		break;
	case 0x4c:
		_disasm_fd << "LD C,IXh";
		break;
	case 0x4d:
		_disasm_fd << "LD C,IXl";
		break;
	case 0x4e:
		_disasm_fd << "LD C,(IX+" << getN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0x4f:
		_disasm_fd << "LD C,A";
		break;
	case 0x50:
		_disasm_fd << "LD D,B";
		break;
	case 0x51:
		_disasm_fd << "LD D,C";
		break;
	case 0x52:
		_disasm_fd << "LD D,D";
		break;
	case 0x53:
		_disasm_fd << "LD D,E";
		break;
	case 0x54:
		_disasm_fd << "LD D,IXh";
		break;
	case 0x55:
		_disasm_fd << "LD D,IXl";
		break;
	case 0x56:
		_disasm_fd << "LD D,(IX+" << getN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0x57:
		_disasm_fd << "LD D,A";
		break;
	case 0x58:
		_disasm_fd << "LD E,B";
		break;
	case 0x59:
		_disasm_fd << "LD E,C";
		break;
	case 0x5a:
		_disasm_fd << "LD E,D";
		break;
	case 0x5b:
		_disasm_fd << "LD E,E";
		break;
	case 0x5c:
		_disasm_fd << "LD E,IXh";
		break;
	case 0x5d:
		_disasm_fd << "LD E,IXl";
		break;
	case 0x5e:
		_disasm_fd << "LD E,(IX+" << getN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0x5f:
		_disasm_fd << "LD E,A";
		break;
	case 0x60:
		_disasm_fd << "LD IXh,B";
		break;
	case 0x61:
		_disasm_fd << "LD IXh,C";
		break;
	case 0x62:
		_disasm_fd << "LD IXh,D";
		break;
	case 0x63:
		_disasm_fd << "LD IXh,E";
		break;
	case 0x64:
		_disasm_fd << "LD IXh,IXh";
		break;
	case 0x65:
		_disasm_fd << "LD IXh,IXl";
		break;
	case 0x66:
		_disasm_fd << "LD H,(IX+" << getN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0x67:
		_disasm_fd << "LD IXh,A";
		break;
	case 0x68:
		_disasm_fd << "LD IXl,B";
		break;
	case 0x69:
		_disasm_fd << "LD IXl,C";
		break;
	case 0x6a:
		_disasm_fd << "LD IXl,D";
		break;
	case 0x6b:
		_disasm_fd << "LD IXl,E";
		break;
	case 0x6c:
		_disasm_fd << "LD IXl,IXh";
		break;
	case 0x6d:
		_disasm_fd << "LD IXl,IXl";
		break;
	case 0x6e:
		_disasm_fd << "LD L,(IX+" << getN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0x6f:
		_disasm_fd << "LD IXl,A";
		break;
	case 0x70:
		_disasm_fd << "LD (IX+" << getN(memory_fn, cpu->pc + 2) << "),B";
		break;
	case 0x71:
		_disasm_fd << "LD (IX+" << getN(memory_fn, cpu->pc + 2) << "),C";
		break;
	case 0x72:
		_disasm_fd << "LD (IX+" << getN(memory_fn, cpu->pc + 2) << "),D";
		break;
	case 0x73:
		_disasm_fd << "LD (IX+" << getN(memory_fn, cpu->pc + 2) << "),E";
		break;
	case 0x74:
		_disasm_fd << "LD (IX+" << getN(memory_fn, cpu->pc + 2) << "),H";
		break;
	case 0x75:
		_disasm_fd << "LD (IX+" << getN(memory_fn, cpu->pc + 2) << "),L";
		break;
	case 0x77:
		_disasm_fd << "LD (IX+" << getN(memory_fn, cpu->pc + 2) << "),A";
		break;
	case 0x78:
		_disasm_fd << "LD A,B";
		break;
	case 0x79:
		_disasm_fd << "LD A,C";
		break;
	case 0x7a:
		_disasm_fd << "LD A,D";
		break;
	case 0x7b:
		_disasm_fd << "LD A,E";
		break;
	case 0x7c:
		_disasm_fd << "LD A,IXh";
		break;
	case 0x7d:
		_disasm_fd << "LD A,IXl";
		break;
	case 0x7e:
		_disasm_fd << "LD A,(IX+" << getN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0x7f:
		_disasm_fd << "LD A,A";
		break;
	case 0x80:
		_disasm_fd << "ADD A,B";
		break;
	case 0x81:
		_disasm_fd << "ADD A,C";
		break;
	case 0x82:
		_disasm_fd << "ADD A,D";
		break;
	case 0x83:
		_disasm_fd << "ADD A,E";
		break;
	case 0x84:
		_disasm_fd << "ADD A,IXh";
		break;
	case 0x85:
		_disasm_fd << "ADD A,IXl";
		break;
	case 0x86:
		_disasm_fd << "ADD A,(IX+" << getN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0x87:
		_disasm_fd << "ADD A,A";
		break;
	case 0x88:
		_disasm_fd << "ADC A,B";
		break;
	case 0x89:
		_disasm_fd << "ADC A,C";
		break;
	case 0x8a:
		_disasm_fd << "ADC A,D";
		break;
	case 0x8b:
		_disasm_fd << "ADC A,E";
		break;
	case 0x8c:
		_disasm_fd << "ADC A,IXh";
		break;
	case 0x8d:
		_disasm_fd << "ADC A,IXl";
		break;
	case 0x8e:
		_disasm_fd << "ADC A,(IX+" << getN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0x8f:
		_disasm_fd << "ADC A,A";
		break;
	case 0x90:
		_disasm_fd << "SUB A,B";
		break;
	case 0x91:
		_disasm_fd << "SUB A,C";
		break;
	case 0x92:
		_disasm_fd << "SUB A,D";
		break;
	case 0x93:
		_disasm_fd << "SUB A,E";
		break;
	case 0x94:
		_disasm_fd << "SUB A,IXh";
		break;
	case 0x95:
		_disasm_fd << "SUB A,IXl";
		break;
	case 0x96:
		_disasm_fd << "SUB A,(IX+" << getN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0x97:
		_disasm_fd << "SUB A,A";
		break;
	case 0x98:
		_disasm_fd << "SBC A,B";
		break;
	case 0x99:
		_disasm_fd << "SBC A,C";
		break;
	case 0x9a:
		_disasm_fd << "SBC A,D";
		break;
	case 0x9b:
		_disasm_fd << "SBC A,E";
		break;
	case 0x9c:
		_disasm_fd << "SBC A,IXh";
		break;
	case 0x9d:
		_disasm_fd << "SBC A,IXl";
		break;
	case 0x9e:
		_disasm_fd << "SBC A,(IX+" << getN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0x9f:
		_disasm_fd << "SBC A,A";
		break;
	case 0xa0:
		_disasm_fd << "AND B";
		break;
	case 0xa1:
		_disasm_fd << "AND C";
		break;
	case 0xa2:
		_disasm_fd << "AND D";
		break;
	case 0xa3:
		_disasm_fd << "AND E";
		break;
	case 0xa4:
		_disasm_fd << "AND IXh";
		break;
	case 0xa5:
		_disasm_fd << "AND IXl";
		break;
	case 0xa6:
		_disasm_fd << "AND (IX+" << getN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0xa7:
		_disasm_fd << "AND A";
		break;
	case 0xa8:
		_disasm_fd << "XOR B";
		break;
	case 0xa9:
		_disasm_fd << "XOR C";
		break;
	case 0xaa:
		_disasm_fd << "XOR D";
		break;
	case 0xab:
		_disasm_fd << "XOR E";
		break;
	case 0xac:
		_disasm_fd << "XOR IXh";
		break;
	case 0xad:
		_disasm_fd << "XOR IXl";
		break;
	case 0xae:
		_disasm_fd << "XOR (IX+" << getN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0xaf:
		_disasm_fd << "XOR A";
		break;
	case 0xb0:
		_disasm_fd << "OR B";
		break;
	case 0xb1:
		_disasm_fd << "OR C";
		break;
	case 0xb2:
		_disasm_fd << "OR D";
		break;
	case 0xb3:
		_disasm_fd << "OR E";
		break;
	case 0xb4:
		_disasm_fd << "OR IXh";
		break;
	case 0xb5:
		_disasm_fd << "OR IXl";
		break;
	case 0xb6:
		_disasm_fd << "OR (IX+" << getN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0xb7:
		_disasm_fd << "OR A";
		break;
	case 0xb8:
		_disasm_fd << "CP B";
		break;
	case 0xb9:
		_disasm_fd << "CP C";
		break;
	case 0xba:
		_disasm_fd << "CP D";
		break;
	case 0xbb:
		_disasm_fd << "CP E";
		break;
	case 0xbc:
		_disasm_fd << "CP IXh";
		break;
	case 0xbd:
		_disasm_fd << "CP IXl";
		break;
	case 0xbe:
		_disasm_fd << "CP (IX+" << getN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0xbf:
		_disasm_fd << "CP A";
		break;
	case 0xcb:
		_disasm_fd << "DDCBBB";
		break;
	case 0xe1:
		_disasm_fd << "POP IX";
		break;
	case 0xe3:
		_disasm_fd << "EX (SP),IX";
		break;
	case 0xe5:
		_disasm_fd << "PUSH IX";
		break;
	case 0xe9:
		_disasm_fd << "JP (IX)";
		break;
	case 0xf9:
		_disasm_fd << "LD SP,IX";
		break;
	default:
		_disasm_fd << "Unknown Instruction" << getN(memory_fn, cpu->pc + 1);
		break;
	}
}

void Z80debugging::disassemblyopcode_ed(const Z80* cpu, std::function<uint8_t(const uint16_t)> memory_fn)
{
	switch (getMemory(memory_fn, cpu->pc + 1))
	{
	case 0x40:
		_disasm_fd << "IN B,(C)";
		break;
	case 0x41:
		_disasm_fd << "OUT (C),B";
		break;
	case 0x42:
		_disasm_fd << "SBC HL,BC";
		break;
	case 0x43:
		_disasm_fd << "LD (" << getNN(memory_fn, cpu->pc + 2) << "),BC";
		break;
	case 0x44:
		_disasm_fd << "NEG";
		break;
	case 0x45:
		_disasm_fd << "RETN";
		break;
	case 0x46:
		_disasm_fd << "IM 0";
		break;
	case 0x47:
		_disasm_fd << "LD I,A";
		break;
	case 0x48:
		_disasm_fd << "IN C,(C)";
		break;
	case 0x49:
		_disasm_fd << "OUT (C),C";
		break;
	case 0x4a:
		_disasm_fd << "ADC HL,BC";
		break;
	case 0x4b:
		_disasm_fd << "LD BC,(" << getNN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0x4d:
		_disasm_fd << "RETI";
		break;
	case 0x4f:
		_disasm_fd << "LD R,A";
		break;
	case 0x50:
		_disasm_fd << "IN D,(C)";
		break;
	case 0x51:
		_disasm_fd << "OUT (C),D";
		break;
	case 0x52:
		_disasm_fd << "SBC HL,DE";
		break;
	case 0x53:
		_disasm_fd << "LD (" << getNN(memory_fn, cpu->pc + 2) << "),DE";
		break;
	case 0x56:
		_disasm_fd << "IM 1";
		break;
	case 0x57:
		_disasm_fd << "LD A,I";
		break;
	case 0x58:
		_disasm_fd << "IN E,(C)";
		break;
	case 0x59:
		_disasm_fd << "OUT (C),E";
		break;
	case 0x5a:
		_disasm_fd << "ADC HL,DE";
		break;
	case 0x5b:
		_disasm_fd << "LD DE,(" << getNN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0x5e:
		_disasm_fd << "IM 2";
		break;
	case 0x5f:
		_disasm_fd << "LD A,R";
		break;
	case 0x60:
		_disasm_fd << "IN H,(C)";
		break;
	case 0x61:
		_disasm_fd << "OUT (C),H";
		break;
	case 0x62:
		_disasm_fd << "SBC HL,HL";
		break;
	case 0x63:
		_disasm_fd << "LD (" << getNN(memory_fn, cpu->pc + 2) << "),HL";
		break;
	case 0x67:
		_disasm_fd << "RRD";
		break;
	case 0x68:
		_disasm_fd << "IN L,(C)";
		break;
	case 0x69:
		_disasm_fd << "OUT (C),L";
		break;
	case 0x6a:
		_disasm_fd << "ADC HL,HL";
		break;
	case 0x6b:
		_disasm_fd << "LD HL,(" << getNN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0x6f:
		_disasm_fd << "RLD";
		break;
	case 0x70:
		_disasm_fd << "IN (C)";
		break;
	case 0x71:
		_disasm_fd << "OUT (C),0";
		break;
	case 0x72:
		_disasm_fd << "SBC HL,SP";
		break;
	case 0x73:
		_disasm_fd << "LD (" << getNN(memory_fn, cpu->pc + 2) << "),SP";
		break;
	case 0x78:
		_disasm_fd << "IN A,(C)";
		break;
	case 0x79:
		_disasm_fd << "OUT (C),A";
		break;
	case 0x7a:
		_disasm_fd << "ADC HL,SP";
		break;
	case 0x7b:
		_disasm_fd << "LD SP,(" << getNN(memory_fn, cpu->pc+2) << ")";
		break;
	case 0xa0:
		_disasm_fd << "LDI";
		break;
	case 0xa1:
		_disasm_fd << "CPI";
		break;
	case 0xa2:
		_disasm_fd << "INI";
		break;
	case 0xa3:
		_disasm_fd << "OUTI";
		break;
	case 0xa8:
		_disasm_fd << "LDD";
		break;
	case 0xa9:
		_disasm_fd << "CPD";
		break;
	case 0xaa:
		_disasm_fd << "IND";
		break;
	case 0xab:
		_disasm_fd << "OUTD";
		break;
	case 0xb0:
		_disasm_fd << "LDIR";
		break;
	case 0xb1:
		_disasm_fd << "CPIR";
		break;
	case 0xb2:
		_disasm_fd << "INIR";
		break;
	case 0xb3:
		_disasm_fd << "OTIR";
		break;
	case 0xb8:
		_disasm_fd << "LDDR";
		break;
	case 0xb9:
		_disasm_fd << "CPDR";
		break;
	case 0xba:
		_disasm_fd << "INDR";
		break;
	case 0xbb:
		_disasm_fd << "OTDR";
		break;
	default:
		_disasm_fd << "Unknown Instruction" << getN(memory_fn, cpu->pc + 1);
		break;
	}
}

void Z80debugging::disassemblyopcode_fd(const Z80* cpu, std::function<uint8_t(const uint16_t)> memory_fn)
{
	switch (getMemory(memory_fn, cpu->pc + 1))
	{
	case 0x04:
		_disasm_fd << "INC B";
		break;
	case 0x05:
		_disasm_fd << "DEC B";
		break;
	case 0x06:
		_disasm_fd << "LD B," << getN(memory_fn, cpu->pc + 2);
		break;
	case 0x09:
		_disasm_fd << "ADD IY,BC";
		break;
	case 0x0c:
		_disasm_fd << "INC C";
		break;
	case 0x0d:
		_disasm_fd << "DEC C";
		break;
	case 0x0e:
		_disasm_fd << "LD C," << getN(memory_fn, cpu->pc + 2);
		break;
	case 0x14:
		_disasm_fd << "INC D";
		break;
	case 0x15:
		_disasm_fd << "DEC D";
		break;
	case 0x16:
		_disasm_fd << "LD D," << getN(memory_fn, cpu->pc + 2);
		break;
	case 0x19:
		_disasm_fd << "ADD IY,DE";
		break;
	case 0x1c:
		_disasm_fd << "INC E";
		break;
	case 0x1d:
		_disasm_fd << "DEC E";
		break;
	case 0x1e:
		_disasm_fd << "LD E," << getN(memory_fn, cpu->pc + 2);
		break;
	case 0x21:
		_disasm_fd << "LD IY," << getNN(memory_fn, cpu->pc + 2);
		break;
	case 0x22:
		_disasm_fd << "LD (" << getNN(memory_fn, cpu->pc + 2) << "),IY";
		break;
	case 0x23:
		_disasm_fd << "INC IY";
		break;
	case 0x24:
		_disasm_fd << "INC IYh";
		break;
	case 0x25:
		_disasm_fd << "DEC IYh";
		break;
	case 0x26:
		_disasm_fd << "LD IYh," << getN(memory_fn, cpu->pc + 2);
		break;
	case 0x29:
		_disasm_fd << "ADD IY,IY";
		break;
	case 0x2a:
		_disasm_fd << "LD IY,(" << getNN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0x2b:
		_disasm_fd << "DEC IY";
		break;
	case 0x2c:
		_disasm_fd << "INC IYl";
		break;
	case 0x2d:
		_disasm_fd << "DEC IYl";
		break;
	case 0x2e:
		_disasm_fd << "LD IYl," << getN(memory_fn, cpu->pc + 2);
		break;
	case 0x34:
		_disasm_fd << "INC (IY+" << getN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0x35:
		_disasm_fd << "DEC (IY+" << getN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0x36:
		_disasm_fd << "LD (IY+" << getN(memory_fn, cpu->pc + 2) << ")," << getN(memory_fn, cpu->pc + 3);
		break;
	case 0x39:
		_disasm_fd << "ADD IY,SP";
		break;
	case 0x3c:
		_disasm_fd << "INC A";
		break;
	case 0x3d:
		_disasm_fd << "DEC A";
		break;
	case 0x3e:
		_disasm_fd << "LD A," << getN(memory_fn, cpu->pc + 1);
		break;
	case 0x40:
		_disasm_fd << "LD B,B";
		break;
	case 0x41:
		_disasm_fd << "LD B,C";
		break;
	case 0x42:
		_disasm_fd << "LD B,D";
		break;
	case 0x43:
		_disasm_fd << "LD B,E";
		break;
	case 0x44:
		_disasm_fd << "LD B,IYh";
		break;
	case 0x45:
		_disasm_fd << "LD B,IYl";
		break;
	case 0x46:
		_disasm_fd << "LD B,(IY+" << getN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0x47:
		_disasm_fd << "LD B,A";
		break;
	case 0x48:
		_disasm_fd << "LD C,B";
		break;
	case 0x49:
		_disasm_fd << "LD C,C";
		break;
	case 0x4a:
		_disasm_fd << "LD C,D";
		break;
	case 0x4b:
		_disasm_fd << "LD C,E";
		break;
	case 0x4c:
		_disasm_fd << "LD C,IYh";
		break;
	case 0x4d:
		_disasm_fd << "LD C,IYl";
		break;
	case 0x4e:
		_disasm_fd << "LD C,(IY+" << getN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0x4f:
		_disasm_fd << "LD C,A";
		break;
	case 0x50:
		_disasm_fd << "LD D,B";
		break;
	case 0x51:
		_disasm_fd << "LD D,C";
		break;
	case 0x52:
		_disasm_fd << "LD D,D";
		break;
	case 0x53:
		_disasm_fd << "LD D,E";
		break;
	case 0x54:
		_disasm_fd << "LD D,IYh";
		break;
	case 0x55:
		_disasm_fd << "LD D,IYl";
		break;
	case 0x56:
		_disasm_fd << "LD D,(IY+" << getN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0x57:
		_disasm_fd << "LD D,A";
		break;
	case 0x58:
		_disasm_fd << "LD E,B";
		break;
	case 0x59:
		_disasm_fd << "LD E,C";
		break;
	case 0x5a:
		_disasm_fd << "LD E,D";
		break;
	case 0x5b:
		_disasm_fd << "LD E,E";
		break;
	case 0x5c:
		_disasm_fd << "LD E,IYh";
		break;
	case 0x5d:
		_disasm_fd << "LD E,IYl";
		break;
	case 0x5e:
		_disasm_fd << "LD E,(IY+" << getN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0x5f:
		_disasm_fd << "LD E,A";
		break;
	case 0x60:
		_disasm_fd << "LD IYh,B";
		break;
	case 0x61:
		_disasm_fd << "LD IYh,C";
		break;
	case 0x62:
		_disasm_fd << "LD IYh,D";
		break;
	case 0x63:
		_disasm_fd << "LD IYh,E";
		break;
	case 0x64:
		_disasm_fd << "LD IYh,IYh";
		break;
	case 0x65:
		_disasm_fd << "LD IYh,IYl";
		break;
	case 0x66:
		_disasm_fd << "LD H,(IY+" << getN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0x67:
		_disasm_fd << "LD IYh,A";
		break;
	case 0x68:
		_disasm_fd << "LD IYl,B";
		break;
	case 0x69:
		_disasm_fd << "LD IYl,C";
		break;
	case 0x6a:
		_disasm_fd << "LD IYl,D";
		break;
	case 0x6b:
		_disasm_fd << "LD IYl,E";
		break;
	case 0x6c:
		_disasm_fd << "LD IYl,IYh";
		break;
	case 0x6d:
		_disasm_fd << "LD IYl,IYl";
		break;
	case 0x6e:
		_disasm_fd << "LD L,(IY+" << getN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0x6f:
		_disasm_fd << "LD IYl,A";
		break;
	case 0x70:
		_disasm_fd << "LD (IY+" << getN(memory_fn, cpu->pc + 2) << "),B";
		break;
	case 0x71:
		_disasm_fd << "LD (IY+" << getN(memory_fn, cpu->pc + 2) << "),C";
		break;
	case 0x72:
		_disasm_fd << "LD (IY+" << getN(memory_fn, cpu->pc + 2) << "),D";
		break;
	case 0x73:
		_disasm_fd << "LD (IY+" << getN(memory_fn, cpu->pc + 2) << "),E";
		break;
	case 0x74:
		_disasm_fd << "LD (IY+" << getN(memory_fn, cpu->pc + 2) << "),H";
		break;
	case 0x75:
		_disasm_fd << "LD (IY+" << getN(memory_fn, cpu->pc + 2) << "),L";
		break;
	case 0x77:
		_disasm_fd << "LD (IY+" << getN(memory_fn, cpu->pc + 2) << "),A";
		break;
	case 0x78:
		_disasm_fd << "LD A,B";
		break;
	case 0x79:
		_disasm_fd << "LD A,C";
		break;
	case 0x7a:
		_disasm_fd << "LD A,D";
		break;
	case 0x7b:
		_disasm_fd << "LD A,E";
		break;
	case 0x7c:
		_disasm_fd << "LD A,IYh";
		break;
	case 0x7d:
		_disasm_fd << "LD A,IYl";
		break;
	case 0x7e:
		_disasm_fd << "LD A,(IY+" << getN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0x7f:
		_disasm_fd << "LD A,A";
		break;
	case 0x80:
		_disasm_fd << "ADD A,B";
		break;
	case 0x81:
		_disasm_fd << "ADD A,C";
		break;
	case 0x82:
		_disasm_fd << "ADD A,D";
		break;
	case 0x83:
		_disasm_fd << "ADD A,E";
		break;
	case 0x84:
		_disasm_fd << "ADD A,IYh";
		break;
	case 0x85:
		_disasm_fd << "ADD A,IYl";
		break;
	case 0x86:
		_disasm_fd << "ADD A,(IY+" << getN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0x87:
		_disasm_fd << "ADD A,A";
		break;
	case 0x88:
		_disasm_fd << "ADC A,B";
		break;
	case 0x89:
		_disasm_fd << "ADC A,C";
		break;
	case 0x8a:
		_disasm_fd << "ADC A,D";
		break;
	case 0x8b:
		_disasm_fd << "ADC A,E";
		break;
	case 0x8c:
		_disasm_fd << "ADC A,IYh";
		break;
	case 0x8d:
		_disasm_fd << "ADC A,IYl";
		break;
	case 0x8e:
		_disasm_fd << "ADC A,(IY+" << getN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0x8f:
		_disasm_fd << "ADC A,A";
		break;
	case 0x90:
		_disasm_fd << "SUB A,B";
		break;
	case 0x91:
		_disasm_fd << "SUB A,C";
		break;
	case 0x92:
		_disasm_fd << "SUB A,D";
		break;
	case 0x93:
		_disasm_fd << "SUB A,E";
		break;
	case 0x94:
		_disasm_fd << "SUB A,IYh";
		break;
	case 0x95:
		_disasm_fd << "SUB A,IYl";
		break;
	case 0x96:
		_disasm_fd << "SUB A,(IY+" << getN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0x97:
		_disasm_fd << "SUB A,A";
		break;
	case 0x98:
		_disasm_fd << "SBC A,B";
		break;
	case 0x99:
		_disasm_fd << "SBC A,C";
		break;
	case 0x9a:
		_disasm_fd << "SBC A,D";
		break;
	case 0x9b:
		_disasm_fd << "SBC A,E";
		break;
	case 0x9c:
		_disasm_fd << "SBC A,IYh";
		break;
	case 0x9d:
		_disasm_fd << "SBC A,IYl";
		break;
	case 0x9e:
		_disasm_fd << "SBC A,(IY+" << getN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0x9f:
		_disasm_fd << "SBC A,A";
		break;
	case 0xa0:
		_disasm_fd << "AND B";
		break;
	case 0xa1:
		_disasm_fd << "AND C";
		break;
	case 0xa2:
		_disasm_fd << "AND D";
		break;
	case 0xa3:
		_disasm_fd << "AND E";
		break;
	case 0xa4:
		_disasm_fd << "AND IYh";
		break;
	case 0xa5:
		_disasm_fd << "AND IYl";
		break;
	case 0xa6:
		_disasm_fd << "AND (IY+" << getN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0xa7:
		_disasm_fd << "AND A";
		break;
	case 0xa8:
		_disasm_fd << "XOR B";
		break;
	case 0xa9:
		_disasm_fd << "XOR C";
		break;
	case 0xaa:
		_disasm_fd << "XOR D";
		break;
	case 0xab:
		_disasm_fd << "XOR E";
		break;
	case 0xac:
		_disasm_fd << "XOR IYh";
		break;
	case 0xad:
		_disasm_fd << "XOR IYl";
		break;
	case 0xae:
		_disasm_fd << "XOR (IY+" << getN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0xaf:
		_disasm_fd << "XOR A";
		break;
	case 0xb0:
		_disasm_fd << "OR B";
		break;
	case 0xb1:
		_disasm_fd << "OR C";
		break;
	case 0xb2:
		_disasm_fd << "OR D";
		break;
	case 0xb3:
		_disasm_fd << "OR E";
		break;
	case 0xb4:
		_disasm_fd << "OR IYh";
		break;
	case 0xb5:
		_disasm_fd << "OR IYl";
		break;
	case 0xb6:
		_disasm_fd << "OR (IY+" << getN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0xb7:
		_disasm_fd << "OR A";
		break;
	case 0xb8:
		_disasm_fd << "CP B";
		break;
	case 0xb9:
		_disasm_fd << "CP C";
		break;
	case 0xba:
		_disasm_fd << "CP D";
		break;
	case 0xbb:
		_disasm_fd << "CP E";
		break;
	case 0xbc:
		_disasm_fd << "CP IYh";
		break;
	case 0xbd:
		_disasm_fd << "CP IYl";
		break;
	case 0xbe:
		_disasm_fd << "CP (IY+" << getN(memory_fn, cpu->pc + 2) << ")";
		break;
	case 0xbf:
		_disasm_fd << "CP A";
		break;
	case 0xcb:
		_disasm_fd << "FDCBBB";
		break;
	case 0xe1:
		_disasm_fd << "POP IY";
		break;
	case 0xe3:
		_disasm_fd << "EX (SP),IY";
		break;
	case 0xe5:
		_disasm_fd << "PUSH IY";
		break;
	case 0xe9:
		_disasm_fd << "JP (IY)";
		break;
	case 0xf9:
		_disasm_fd << "LD SP,IY";
		break;
	default:
		_disasm_fd << "Unknown Instruction" << getN(memory_fn, cpu->pc + 1);
		break;
	}
}
