#include "z80.h"
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
	string Z80dbg::disassembly(uint16_t& address) const
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
			return "EX AF,AF'";
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
			disasm << "DJNZ " << std::hex << getN(_memoryfn, address);
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
			disasm << "LD (" << std::hex << getNN(_memoryfn, address) << "),HL";
			return disasm.str();
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
			disasm << "LD HL,(" << std::hex << getNN(_memoryfn, address) << ")";
			return disasm.str();
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
			disasm << "LD (" << std::hex << getNN(_memoryfn, address) << "),A";
			return disasm.str();
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
			disasm << "LD A,(" << std::hex << getNN(_memoryfn, address) << ")";
			return disasm.str();
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
			disasm << "OUT (" << std::hex << getN(_memoryfn, address) << "),A";
			return disasm.str();
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
			disasm << "IN A,(" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
		case 0xdc:
			disasm << "CALL C," << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xdd:
			return disassembly_dd(address);
		case 0xde:
			disasm << "SBC A," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0xdf:
			return "RST 18";
		case 0xe0:
			return "RET PO";
		case 0xe1:
			return "POP HL";
		case 0xe2:
			disasm << "JP PO," << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xe3:
			return "EX (SP),HL";
		case 0xe4:
			disasm << "CALL PO," << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xe5:
			return "PUSH HL";
		case 0xe6:
			disasm << "AND " << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0xe7:
			return "RST 20";
		case 0xe8:
			return "RET PE";
		case 0xe9:
			return "JP (HL)";
		case 0xea:
			disasm << "JP PE," << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xeb:
			return "EX DE,HL";
		case 0xec:
			disasm << "CALL PE," << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xed:
			return disassembly_ed(address);
		case 0xee:
			disasm << "XOR " << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0xef:
			return "RST 28";
		case 0xf0:
			return "RET P";
		case 0xf1:
			return "POP AF";
		case 0xf2:
			disasm << "JP P," << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xf3:
			return "DI";
		case 0xf4:
			disasm << "CALL P," << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xf5:
			return "PUSH AF";
		case 0xf6:
			disasm << "OR " << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0xf7:
			return "RST 30";
		case 0xf8:
			return "RET M";
		case 0xf9:
			return "LD SP,HL";
		case 0xfa:
			disasm << "JP M," << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xfb:
			return "EI";
		case 0xfc:
			disasm << "CALL M," << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xfd:
			return disassembly_fd(address);
		case 0xfe:
			disasm << "CP " << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0xff:
			return "RST 38";
		}
	}

	string Z80dbg::disassembly_dd(uint16_t& address) const
	{
		std::stringstream disasm;
		switch (_memoryfn(address++))
		{
		case 0x04:
			return "INC B";
		case 0x05:
			return "DEC B";
		case 0x06:
			disasm << "LD B," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x09:
			return "ADD IX,BC";
		case 0x0c:
			return "INC C";
		case 0x0d:
			return "DEC C";
		case 0x0e:
			disasm << "LD C," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x14:
			return "INC D";
		case 0x15:
			return "DEC D";
		case 0x16:
			disasm << "LD D," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x19:
			return "ADD IX,DE";
		case 0x1c:
			return "INC E";
		case 0x1d:
			return "DEC E";
		case 0x1e:
			disasm << "LD E," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x21:
			disasm << "LD IX," << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0x22:
			disasm << "LD (" << std::hex << getNN(_memoryfn, address) << "),IX";
			return disasm.str();
		case 0x23:
			return "INC IX";
		case 0x24:
			return "INC IXh";
		case 0x25:
			return "DEC IXh";
		case 0x26:
			disasm << "LD IXh," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x29:
			return "ADD IX,IX";
		case 0x2a:
			disasm << "LD IX,(" << std::hex << getNN(_memoryfn, address) << ")";
			return disasm.str();
		case 0x2b:
			return "DEC IX";
		case 0x2c:
			return "INC IXl";
		case 0x2d:
			return "DEC IXl";
		case 0x2e:
			disasm << "LD IXl," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x34:
			disasm << "INC (IX+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
		case 0x35:
			disasm << "DEC (IX+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
		case 0x36:
			disasm << "LD (IX+" << std::hex << getN(_memoryfn, address) << ")," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x39:
			return "ADD IX,SP";
		case 0x3c:
			return "INC A";
		case 0x3d:
			return "DEC A";
		case 0x3e:
			disasm << "LD A," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x40:
			return "LD B,B";
		case 0x41:
			return "LD B,C";
		case 0x42:
			return "LD B,D";
		case 0x43:
			return "LD B,E";
		case 0x44:
			return "LD B,IXh";
		case 0x45:
			return "LD B,IXl";
		case 0x46:
			disasm << "LD B,(IX+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
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
			return "LD C,IXh";
		case 0x4d:
			return "LD C,IXl";
		case 0x4e:
			disasm << "LD C,(IX+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
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
			return "LD D,IXh";
		case 0x55:
			return "LD D,IXl";
		case 0x56:
			disasm << "LD D,(IX+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
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
			return "LD E,IXh";
		case 0x5d:
			return "LD E,IXl";
		case 0x5e:
			disasm << "LD E,(IX+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
		case 0x5f:
			return "LD E,A";
		case 0x60:
			return "LD IXh,B";
		case 0x61:
			return "LD IXh,C";
		case 0x62:
			return "LD IXh,D";
		case 0x63:
			return "LD IXh,E";
		case 0x64:
			return "LD IXh,IXh";
		case 0x65:
			return "LD IXh,IXl";
		case 0x66:
			disasm << "LD H,(IX+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
		case 0x67:
			return "LD IXh,A";
		case 0x68:
			return "LD IXl,B";
		case 0x69:
			return "LD IXl,C";
		case 0x6a:
			return "LD IXl,D";
		case 0x6b:
			return "LD IXl,E";
		case 0x6c:
			return "LD IXl,IXh";
		case 0x6d:
			return "LD IXl,IXl";
		case 0x6e:
			disasm << "LD L,(IX+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
		case 0x6f:
			return "LD IXl,A";
		case 0x70:
			disasm << "LD (IX+" << std::hex << getN(_memoryfn, address) << "),B";
			return disasm.str();
		case 0x71:
			disasm << "LD (IX+" << std::hex << getN(_memoryfn, address) << "),C";
			return disasm.str();
		case 0x72:
			disasm << "LD (IX+" << std::hex << getN(_memoryfn, address) << "),D";
			return disasm.str();
		case 0x73:
			disasm << "LD (IX+" << std::hex << getN(_memoryfn, address) << "),E";
			return disasm.str();
		case 0x74:
			disasm << "LD (IX+" << std::hex << getN(_memoryfn, address) << "),H";
			return disasm.str();
		case 0x75:
			disasm << "LD (IX+" << std::hex << getN(_memoryfn, address) << "),L";
			return disasm.str();
		case 0x77:
			disasm << "LD (IX+" << std::hex << getN(_memoryfn, address) << "),A";
			return disasm.str();
		case 0x78:
			return "LD A,B";
		case 0x79:
			return "LD A,C";
		case 0x7a:
			return "LD A,D";
		case 0x7b:
			return "LD A,E";
		case 0x7c:
			return "LD A,IXh";
		case 0x7d:
			return "LD A,IXl";
		case 0x7e:
			disasm << "LD A,(IX+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
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
			return "ADD A,IXh";
		case 0x85:
			return "ADD A,IXl";
		case 0x86:
			disasm << "ADD A,(IX+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
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
			return "ADC A,IXh";
		case 0x8d:
			return "ADC A,IXl";
		case 0x8e:
			disasm << "ADC A,(IX+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
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
			return "SUB A,IXh";
		case 0x95:
			return "SUB A,IXl";
		case 0x96:
			disasm << "SUB A,(IX+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
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
			return "SBC A,IXh";
		case 0x9d:
			return "SBC A,IXl";
		case 0x9e:
			disasm << "SBC A,(IX+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
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
			return "AND IXh";
		case 0xa5:
			return "AND IXl";
		case 0xa6:
			disasm << "AND (IX+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
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
			return "XOR IXh";
		case 0xad:
			return "XOR IXl";
		case 0xae:
			disasm << "XOR (IX+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
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
			return "OR IXh";
		case 0xb5:
			return "OR IXl";
		case 0xb6:
			disasm << "OR (IX+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
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
			return "CP IXh";
		case 0xbd:
			return "CP IXl";
		case 0xbe:
			disasm << "CP (IX+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
		case 0xbf:
			return "CP A";
		case 0xcb:
			return "DDCBBB";
		case 0xe1:
			return "POP IX";
		case 0xe3:
			return "EX (SP),IX";
		case 0xe5:
			return "PUSH IX";
		case 0xe9:
			return "JP (IX)";
		case 0xf9:
			return "LD SP,IX";
		default:
			disasm << "Unknown Instruction" << std::hex << getN(_memoryfn, address);
			return disasm.str();
		}
	}

	string Z80dbg::disassembly_ed(uint16_t& address) const
	{
		std::stringstream disasm;
		switch (_memoryfn(address++))
		{
		case 0x40:
			return "IN B,(C)";
		case 0x41:
			return "OUT (C),B";
		case 0x42:
			return "SBC HL,BC";
		case 0x43:
			disasm << "LD (" << std::hex << getNN(_memoryfn, address) << "),BC";
			return disasm.str();
		case 0x44:
			return "NEG";
		case 0x45:
			return "RETN";
		case 0x46:
			return "IM 0";
		case 0x47:
			return "LD I,A";
		case 0x48:
			return "IN C,(C)";
		case 0x49:
			return "OUT (C),C";
		case 0x4a:
			return "ADC HL,BC";
		case 0x4b:
			disasm << "LD BC,(" << std::hex << getNN(_memoryfn, address) << ")";
			return disasm.str();
		case 0x4d:
			return "RETI";
		case 0x4f:
			return "LD R,A";
		case 0x50:
			return "IN D,(C)";
		case 0x51:
			return "OUT (C),D";
		case 0x52:
			return "SBC HL,DE";
		case 0x53:
			disasm << "LD (" << std::hex << getNN(_memoryfn, address) << "),DE";
			return disasm.str();
		case 0x56:
			return "IM 1";
		case 0x57:
			return "LD A,I";
		case 0x58:
			return "IN E,(C)";
		case 0x59:
			return "OUT (C),E";
		case 0x5a:
			return "ADC HL,DE";
		case 0x5b:
			disasm << "LD DE,(" << std::hex << getNN(_memoryfn, address) << ")";
			return disasm.str();
		case 0x5e:
			return "IM 2";
		case 0x5f:
			return "LD A,R";
		case 0x60:
			return "IN H,(C)";
		case 0x61:
			return "OUT (C),H";
		case 0x62:
			return "SBC HL,HL";
		case 0x63:
			disasm << "LD (" << std::hex << getNN(_memoryfn, address) << "),HL";
			return disasm.str();
		case 0x67:
			return "RRD";
		case 0x68:
			return "IN L,(C)";
		case 0x69:
			return "OUT (C),L";
		case 0x6a:
			return "ADC HL,HL";
		case 0x6b:
			disasm << "LD HL,(" << std::hex << getNN(_memoryfn, address) << ")";
			return disasm.str();
		case 0x6f:
			return "RLD";
		case 0x70:
			return "IN (C)";
		case 0x71:
			return "OUT (C),0";
		case 0x72:
			return "SBC HL,SP";
		case 0x73:
			disasm << "LD (" << std::hex << getNN(_memoryfn, address) << "),SP";
			return disasm.str();
		case 0x78:
			return "IN A,(C)";
		case 0x79:
			return "OUT (C),A";
		case 0x7a:
			return "ADC HL,SP";
		case 0x7b:
			disasm << "LD SP,(" << std::hex << getNN(_memoryfn, address) << ")";
			return disasm.str();
		case 0xa0:
			return "LDI";
		case 0xa1:
			return "CPI";
		case 0xa2:
			return "INI";
		case 0xa3:
			return "OUTI";
		case 0xa8:
			return "LDD";
		case 0xa9:
			return "CPD";
		case 0xaa:
			return "IND";
		case 0xab:
			return "OUTD";
		case 0xb0:
			return "LDIR";
		case 0xb1:
			return "CPIR";
		case 0xb2:
			return "INIR";
		case 0xb3:
			return "OTIR";
		case 0xb8:
			return "LDDR";
		case 0xb9:
			return "CPDR";
		case 0xba:
			return "INDR";
		case 0xbb:
			return "OTDR";
		default:
			disasm << "Unknown Instruction" << std::hex << getN(_memoryfn, address);
			return disasm.str();
		}
	}

	string Z80dbg::disassembly_fd(uint16_t& address) const
	{
		std::stringstream disasm;
		switch (_memoryfn(address++))
		{
		case 0x04:
			return "INC B";
		case 0x05:
			return "DEC B";
		case 0x06:
			disasm << "LD B," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x09:
			return "ADD IY,BC";
		case 0x0c:
			return "INC C";
		case 0x0d:
			return "DEC C";
		case 0x0e:
			disasm << "LD C," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x14:
			return "INC D";
		case 0x15:
			return "DEC D";
		case 0x16:
			disasm << "LD D," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x19:
			return "ADD IY,DE";
		case 0x1c:
			return "INC E";
		case 0x1d:
			return "DEC E";
		case 0x1e:
			disasm << "LD E," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x21:
			disasm << "LD IY," << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0x22:
			disasm << "LD (" << std::hex << getNN(_memoryfn, address) << "),IY";
			return disasm.str();
		case 0x23:
			return "INC IY";
		case 0x24:
			return "INC IYh";
		case 0x25:
			return "DEC IYh";
		case 0x26:
			disasm << "LD IYh," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x29:
			return "ADD IY,IY";
		case 0x2a:
			disasm << "LD IY,(" << std::hex << getNN(_memoryfn, address) << ")";
			return disasm.str();
		case 0x2b:
			return "DEC IY";
		case 0x2c:
			return "INC IYl";
		case 0x2d:
			return "DEC IYl";
		case 0x2e:
			disasm << "LD IYl," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x34:
			disasm << "INC (IY+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
		case 0x35:
			disasm << "DEC (IY+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
		case 0x36:
			disasm << "LD (IY+" << std::hex << getN(_memoryfn, address) << ")," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x39:
			return "ADD IY,SP";
		case 0x3c:
			return "INC A";
		case 0x3d:
			return "DEC A";
		case 0x3e:
			disasm << "LD A," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x40:
			return "LD B,B";
		case 0x41:
			return "LD B,C";
		case 0x42:
			return "LD B,D";
		case 0x43:
			return "LD B,E";
		case 0x44:
			return "LD B,IYh";
		case 0x45:
			return "LD B,IYl";
		case 0x46:
			disasm << "LD B,(IY+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
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
			return "LD C,IYh";
		case 0x4d:
			return "LD C,IYl";
		case 0x4e:
			disasm << "LD C,(IY+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
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
			return "LD D,IYh";
		case 0x55:
			return "LD D,IYl";
		case 0x56:
			disasm << "LD D,(IY+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
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
			return "LD E,IYh";
		case 0x5d:
			return "LD E,IYl";
		case 0x5e:
			disasm << "LD E,(IY+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
		case 0x5f:
			return "LD E,A";
		case 0x60:
			return "LD IYh,B";
		case 0x61:
			return "LD IYh,C";
		case 0x62:
			return "LD IYh,D";
		case 0x63:
			return "LD IYh,E";
		case 0x64:
			return "LD IYh,IYh";
		case 0x65:
			return "LD IYh,IYl";
		case 0x66:
			disasm << "LD H,(IY+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
		case 0x67:
			return "LD IYh,A";
		case 0x68:
			return "LD IYl,B";
		case 0x69:
			return "LD IYl,C";
		case 0x6a:
			return "LD IYl,D";
		case 0x6b:
			return "LD IYl,E";
		case 0x6c:
			return "LD IYl,IYh";
		case 0x6d:
			return "LD IYl,IYl";
		case 0x6e:
			disasm << "LD L,(IY+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
		case 0x6f:
			return "LD IYl,A";
		case 0x70:
			disasm << "LD (IY+" << std::hex << getN(_memoryfn, address) << "),B";
			return disasm.str();
		case 0x71:
			disasm << "LD (IY+" << std::hex << getN(_memoryfn, address) << "),C";
			return disasm.str();
		case 0x72:
			disasm << "LD (IY+" << std::hex << getN(_memoryfn, address) << "),D";
			return disasm.str();
		case 0x73:
			disasm << "LD (IY+" << std::hex << getN(_memoryfn, address) << "),E";
			return disasm.str();
		case 0x74:
			disasm << "LD (IY+" << std::hex << getN(_memoryfn, address) << "),H";
			return disasm.str();
		case 0x75:
			disasm << "LD (IY+" << std::hex << getN(_memoryfn, address) << "),L";
			return disasm.str();
		case 0x77:
			disasm << "LD (IY+" << std::hex << getN(_memoryfn, address) << "),A";
			return disasm.str();
		case 0x78:
			return "LD A,B";
		case 0x79:
			return "LD A,C";
		case 0x7a:
			return "LD A,D";
		case 0x7b:
			return "LD A,E";
		case 0x7c:
			return "LD A,IYh";
		case 0x7d:
			return "LD A,IYl";
		case 0x7e:
			disasm << "LD A,(IY+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
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
			return "ADD A,IYh";
		case 0x85:
			return "ADD A,IYl";
		case 0x86:
			disasm << "ADD A,(IY+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
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
			return "ADC A,IYh";
		case 0x8d:
			return "ADC A,IYl";
		case 0x8e:
			disasm << "ADC A,(IY+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
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
			return "SUB A,IYh";
		case 0x95:
			return "SUB A,IYl";
		case 0x96:
			disasm << "SUB A,(IY+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
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
			return "SBC A,IYh";
		case 0x9d:
			return "SBC A,IYl";
		case 0x9e:
			disasm << "SBC A,(IY+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
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
			return "AND IYh";
		case 0xa5:
			return "AND IYl";
		case 0xa6:
			disasm << "AND (IY+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
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
			return "XOR IYh";
		case 0xad:
			return "XOR IYl";
		case 0xae:
			disasm << "XOR (IY+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
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
			return "OR IYh";
		case 0xb5:
			return "OR IYl";
		case 0xb6:
			disasm << "OR (IY+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
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
			return "CP IYh";
		case 0xbd:
			return "CP IYl";
		case 0xbe:
			disasm << "CP (IY+" << std::hex << getN(_memoryfn, address) << ")";
			return disasm.str();
		case 0xbf:
			return "CP A";
		case 0xcb:
			return "FDCBBB";
		case 0xe1:
			return "POP IY";
		case 0xe3:
			return "EX (SP),IY";
		case 0xe5:
			return "PUSH IY";
		case 0xe9:
			return "JP (IY)";
		case 0xf9:
			return "LD SP,IY";
		default:
			disasm << "Unknown Instruction" << std::hex << getN(_memoryfn, address);
			return disasm.str();
		}
	}
}
