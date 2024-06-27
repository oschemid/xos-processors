#include "sm83.h"
#include <sstream>


using namespace xprocessors::cpu;

static unsigned int getN(std::function<uint8_t(const uint16_t)> memory_fn, uint16_t& address)
{
	return static_cast<unsigned int>(memory_fn(address++));
}
static unsigned int getNN(std::function<uint8_t(const uint16_t)> memory_fn, uint16_t& address)
{
	address += 2;
	return static_cast<unsigned int>(memory_fn(address - 2) | (memory_fn(address - 1) << 8));
}

namespace xprocessors::cpu
{
	string sm83dbg::disassembly(uint16_t& address) const
	{
		std::stringstream disasm;
		switch (_memoryfn(address++))
		{
		case 0x00:
			return "NOP";
		case 0x01:
			disasm << "LD BC," << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0x02:
			return "LD (BC),A";
		case 0x03:
			return "INC BC";
		case 0x04:
			return "INC B";
		case 0x05:
			return "DEC B";
		case 0x06:
			disasm << "LD B," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x07:
			return "RLCA";
		case 0x08:
			disasm << "LD (" << std::hex << getNN(_memoryfn, address) <<"),SP";
			return disasm.str();
		case 0x09:
			return "ADD HL,BC";
		case 0x0a:
			return "LD A,(BC)";
		case 0x0b:
			return "DEC BC";
		case 0x0c:
			return "INC C";
		case 0x0d:
			return "DEC C";
		case 0x0e:
			disasm << "LD C," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x0f:
			return "RRCA";
		case 0x10:
			disasm << "STOP " << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x11:
			disasm << "LD DE," << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0x12:
			return "LD (DE),A";
		case 0x13:
			return "INC DE";
		case 0x14:
			return "INC D";
		case 0x15:
			return "DEC D";
		case 0x16:
			disasm << "LD D," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x17:
			return "RLA";
		case 0x18:
			disasm << "JR " << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x19:
			return "ADD HL,DE";
		case 0x1a:
			return "LD A,(DE)";
		case 0x1b:
			return "DEC DE";
		case 0x1c:
			return "INC E";
		case 0x1d:
			return "DEC E";
		case 0x1e:
			disasm << "LD E," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x1f:
			return "RRA";
		case 0x20:
			disasm << "JR NZ," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x21:
			disasm << "LD HL," << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0x22:
			return "LD (HL+),A";
		case 0x23:
			return "INC HL";
		case 0x24:
			return "INC H";
		case 0x25:
			return "DEC H";
		case 0x26:
			disasm << "LD H," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x27:
			return "DAA";
		case 0x28:
			disasm << "JR Z," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x29:
			return "ADD HL,HL";
		case 0x2a:
			return "LD A,(HL+)";
		case 0x2b:
			return "DEC HL";
		case 0x2c:
			return "INC L";
		case 0x2d:
			return "DEC L";
		case 0x2e:
			disasm << "LD L," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x2f:
			return "CPL";
		case 0x30:
			disasm << "JR NC," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x31:
			disasm << "LD SP," << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0x32:
			return "LD (HL-),A";
		case 0x33:
			return "INC SP";
		case 0x34:
			return "INC (HL)";
		case 0x35:
			return "DEC (HL)";
		case 0x36:
			disasm << "LD (HL)," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x37:
			return "SCF";
		case 0x38:
			disasm << "JR C," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x39:
			return "ADD HL,SP";
		case 0x3a:
			return "LD A,(HL-)";
		case 0x3b:
			return "DEC SP";
		case 0x3c:
			return "INC A";
		case 0x3d:
			return "DEC A";
		case 0x3e:
			disasm << "LD A," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x3f:
			return "CCF";
		case 0x40:
			return "LD B,B";
		case 0x41:
			return "LD B,C";
		case 0x42:
			return "LD B,D";
		case 0x43:
			return "LD B,E";
		case 0x44:
			return "LD B,H";
		case 0x45:
			return "LD B,L";
		case 0x46:
			return "LD B,(HL)";
		case 0x47:
			return "LD B,A";
		case 0x48:
			return "LD C,B";
		case 0x49:
			return "LD C,C";
		case 0x4a:
			return "LD C,D";
		case 0x4b:
			return "LD C,E";
		case 0x4c:
			return "LD C,H";
		case 0x4d:
			return "LD C,L";
		case 0x4e:
			return "LD C,(HL)";
		case 0x4f:
			return "LD C,A";
		case 0x50:
			return "LD D,B";
		case 0x51:
			return "LD D,C";
		case 0x52:
			return "LD D,D";
		case 0x53:
			return "LD D,E";
		case 0x54:
			return "LD D,H";
		case 0x55:
			return "LD D,L";
		case 0x56:
			return "LD D,(HL)";
		case 0x57:
			return "LD D,A";
		case 0x58:
			return "LD E,B";
		case 0x59:
			return "LD E,C";
		case 0x5a:
			return "LD E,D";
		case 0x5b:
			return "LD E,E";
		case 0x5c:
			return "LD E,H";
		case 0x5d:
			return "LD E,L";
		case 0x5e:
			return "LD E,(HL)";
		case 0x5f:
			return "LD E,A";
		case 0x60:
			return "LD H,B";
		case 0x61:
			return "LD H,C";
		case 0x62:
			return "LD H,D";
		case 0x63:
			return "LD H,E";
		case 0x64:
			return "LD H,H";
		case 0x65:
			return "LD H,L";
		case 0x66:
			return "LD H,(HL)";
		case 0x67:
			return "LD H,A";
		case 0x68:
			return "LD L,B";
		case 0x69:
			return "LD L,C";
		case 0x6a:
			return "LD L,D";
		case 0x6b:
			return "LD L,E";
		case 0x6c:
			return "LD L,H";
		case 0x6d:
			return "LD L,L";
		case 0x6e:
			return "LD L,(HL)";
		case 0x6f:
			return "LD L,A";
		case 0x70:
			return "LD (HL),B";
		case 0x71:
			return "LD (HL),C";
		case 0x72:
			return "LD (HL),D";
		case 0x73:
			return "LD (HL),E";
		case 0x74:
			return "LD (HL),H";
		case 0x75:
			return "LD (HL),L";
		case 0x76:
			return "HALT";
		case 0x77:
			return "LD (HL),A";
		case 0x78:
			return "LD A,B";
		case 0x79:
			return "LD A,C";
		case 0x7a:
			return "LD A,D";
		case 0x7b:
			return "LD A,E";
		case 0x7c:
			return "LD A,H";
		case 0x7d:
			return "LD A,L";
		case 0x7e:
			return "LD A,(HL)";
		case 0x7f:
			return "LD A,A";
		case 0x80:
			return "ADD A,B";
		case 0x81:
			return "ADD A,C";
		case 0x82:
			return "ADD A,D";
		case 0x83:
			return "ADD A,E";
		case 0x84:
			return "ADD A,H";
		case 0x85:
			return "ADD A,L";
		case 0x86:
			return "ADD A,(HL)";
		case 0x87:
			return "ADD A,A";
		case 0x88:
			return "ADC A,B";
		case 0x89:
			return "ADC A,C";
		case 0x8a:
			return "ADC A,D";
		case 0x8b:
			return "ADC A,E";
		case 0x8c:
			return "ADC A,H";
		case 0x8d:
			return "ADC A,L";
		case 0x8e:
			return "ADC A,(HL)";
		case 0x8f:
			return "ADC A,A";
		case 0x90:
			return "SUB A,B";
		case 0x91:
			return "SUB A,C";
		case 0x92:
			return "SUB A,D";
		case 0x93:
			return "SUB A,E";
		case 0x94:
			return "SUB A,H";
		case 0x95:
			return "SUB A,L";
		case 0x96:
			return "SUB A,(HL)";
		case 0x97:
			return "SUB A,A";
		case 0x98:
			return "SBC A,B";
		case 0x99:
			return "SBC A,C";
		case 0x9a:
			return "SBC A,D";
		case 0x9b:
			return "SBC A,E";
		case 0x9c:
			return "SBC A,H";
		case 0x9d:
			return "SBC A,L";
		case 0x9e:
			return "SBC A,(HL)";
		case 0x9f:
			return "SBC A,A";
		case 0xa0:
			return "AND B";
		case 0xa1:
			return "AND C";
		case 0xa2:
			return "AND D";
		case 0xa3:
			return "AND E";
		case 0xa4:
			return "AND H";
		case 0xa5:
			return "AND L";
		case 0xa6:
			return "AND (HL)";
		case 0xa7:
			return "AND A";
		case 0xa8:
			return "XOR B";
		case 0xa9:
			return "XOR C";
		case 0xaa:
			return "XOR D";
		case 0xab:
			return "XOR E";
		case 0xac:
			return "XOR H";
		case 0xad:
			return "XOR L";
		case 0xae:
			return "XOR (HL)";
		case 0xaf:
			return "XOR A";
		case 0xb0:
			return "OR B";
		case 0xb1:
			return "OR C";
		case 0xb2:
			return "OR D";
		case 0xb3:
			return "OR E";
		case 0xb4:
			return "OR H";
		case 0xb5:
			return "OR L";
		case 0xb6:
			return "OR (HL)";
		case 0xb7:
			return "OR A";
		case 0xb8:
			return "CP B";
		case 0xb9:
			return "CP C";
		case 0xba:
			return "CP D";
		case 0xbb:
			return "CP E";
		case 0xbc:
			return "CP H";
		case 0xbd:
			return "CP L";
		case 0xbe:
			return "CP (HL)";
		case 0xbf:
			return "CP A";
		case 0xc0:
			return "RET NZ";
		case 0xc1:
			return "POP BC";
		case 0xc2:
			disasm << "JP NZ," << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xc3:
			disasm << "JP " << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xc4:
			disasm << "CALL NZ," << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xc5:
			return "PUSH BC";
		case 0xc6:
			disasm << "ADD A," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0xc7:
			return "RST 00";
		case 0xc8:
			return "RET Z";
		case 0xc9:
			return "RET";
		case 0xca:
			disasm << "JP Z," << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xcb:
			return "CBBB";
		case 0xcc:
			disasm << "CALL Z," << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xcd:
			disasm << "CALL " << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xce:
			disasm << "ADC A," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0xcf:
			return "RST 08";
		case 0xd0:
			return "RET NC";
		case 0xd1:
			return "POP DE";
		case 0xd2:
			disasm << "JP NC," << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xd3:
			return "NOOPCODE";
		case 0xd4:
			disasm << "CALL NC," << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xd5:
			return "PUSH DE";
		case 0xd6:
			disasm << "SUB " << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0xd7:
			return "RST 10";
		case 0xd8:
			return "RET C";
		case 0xd9:
			return "EXX";
		case 0xda:
			disasm << "JP C," << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xdb:
			return "NOOPCODE";
		case 0xdc:
			disasm << "CALL C," << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xdd:
			return "NOOPCODE";
		case 0xde:
			disasm << "SBC A," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0xdf:
			return "RST 18";
		case 0xe0:
			disasm << "LDH (" << std::hex << getN(_memoryfn, address) << "),A";
			return disasm.str();
		case 0xe1:
			return "POP HL";
		case 0xe2:
			return "LD (C),A";
		case 0xe3:
			return "NOOPCODE";
		case 0xe4:
			return "NOOPCODE";
		case 0xe5:
			return "PUSH HL";
		case 0xe6:
			disasm << "AND " << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0xe7:
			return "RST 20";
		case 0xe8:
			disasm << "ADD SP,"<< std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0xe9:
			return "JP (HL)";
		case 0xea:
			disasm << "LD (" << std::hex << getNN(_memoryfn, address) << "),A";
			return disasm.str();
		case 0xeb:
			return "NOOPCODE";
		case 0xec:
			return "NOOPCODE";
		case 0xed:
			return "NOOPCODE";
		case 0xee:
			disasm << "XOR " << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0xef:
			return "RST 28";
		case 0xf0:
			disasm << "LDH A,("<< std::hex << getN(_memoryfn,address)<<")";
			return disasm.str();
		case 0xf1:
			return "POP AF";
		case 0xf2:
			return "LD A,(C)";
		case 0xf3:
			return "DI";
		case 0xf4:
			return "NOOPCODE";
		case 0xf5:
			return "PUSH AF";
		case 0xf6:
			disasm << "OR " << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0xf7:
			return "RST 30";
		case 0xf8:
			disasm << "LD HL,SP+"<<std::hex<<getN(_memoryfn,address);
			return disasm.str();
		case 0xf9:
			return "LD SP,HL";
		case 0xfa:
			disasm << "LD A,(" << std::hex << getNN(_memoryfn, address) << ")";
			return disasm.str();
		case 0xfb:
			return "EI";
		case 0xfc:
			return "NOOPCODE";
		case 0xfd:
			return "NOOPCODE";
		case 0xfe:
			disasm << "CP " << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0xff:
			return "RST 38";
		}
	}
}
