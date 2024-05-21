#include "i8080.h"
#include <sstream>

using std::stringstream;


unsigned int getN(std::function<uint8_t(const uint16_t)> memory_fn, uint16_t& address)
{
	return static_cast<unsigned int>(memory_fn(address++));
}
unsigned int getNN(std::function<uint8_t(const uint16_t)> memory_fn, uint16_t& address)
{
	address += 2;
	return static_cast<unsigned int>(memory_fn(address-2) | (memory_fn(address - 1) << 8));
}

namespace xprocessors::cpu
{
	string i8080dbg::disassembly(uint16_t& address) const
	{
		stringstream disasm;
		switch (_memoryfn(address++))
		{
		case 0x00:
			return "NOP";
		case 0x01:
			disasm << "LXI B," << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0x02:
			return "STAX B";
		case 0x03:
			return "INX B";
		case 0x04:
			return "INR B";
		case 0x05:
			return "DCR B";
		case 0x06:
			disasm << "MVI B," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x07:
			return "RLC";
		case 0x08:
			return "NOP";
		case 0x09:
			return "DAD B";
		case 0x0a:
			return "LDAX B";
		case 0x0b:
			return "DCX B";
		case 0x0c:
			return "INR C";
		case 0x0d:
			return "DCR C";
		case 0x0e:
			disasm << "MVI C," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x0f:
			return "RRC";
		case 0x10:
			return "NOP";
		case 0x11:
			disasm << "LXI D," << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0x12:
			return "STAX D";
		case 0x13:
			return "INX D";
		case 0x14:
			return "INR D";
		case 0x15:
			return "DCR D";
		case 0x16:
			disasm << "MVI D," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x17:
			return "RAL";
		case 0x18:
			return "NOP";
		case 0x19:
			return "DAD D";
		case 0x1a:
			return "LDAX D";
		case 0x1b:
			return "DCX D";
		case 0x1c:
			return "INR E";
		case 0x1d:
			return "DCR E";
		case 0x1e:
			disasm << "MVI E," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x1f:
			return "RAR";
		case 0x20:
			return "NOP";
		case 0x21:
			disasm << "LXI H," << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0x22:
			disasm << "SHLD " << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0x23:
			return "INX H";
		case 0x24:
			return "INR H";
		case 0x25:
			return "DCR H";
		case 0x26:
			disasm << "MVI H," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x27:
			return "DAA";
		case 0x28:
			return "NOP";
		case 0x29:
			return "DAD H";
		case 0x2a:
			disasm << "LHLD " << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0x2b:
			return "DCX H";
		case 0x2c:
			return "INR L";
		case 0x2d:
			return "DCR L";
		case 0x2e:
			disasm << "MVI L," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x2f:
			return "CMA";
		case 0x30:
			return "NOP";
		case 0x31:
			disasm << "LXI SP," << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0x32:
			disasm << "STA " << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0x33:
			return "INX SP";
		case 0x34:
			return "INR M";
		case 0x35:
			return "DCR M";
		case 0x36:
			disasm << "MVI M," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x37:
			return "STC";
		case 0x38:
			return "NOP";
		case 0x39:
			return "DAD SP";
		case 0x3a:
			disasm << "LDA " << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0x3b:
			return "DCX SP";
		case 0x3c:
			return "INR A";
		case 0x3d:
			return "DCR A";
		case 0x3e:
			disasm << "MVI A," << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0x3f:
			return "CMC";
		case 0x40:
			return "MOV B,B";
		case 0x41:
			return "MOV B,C";
		case 0x42:
			return "MOV B,D";
		case 0x43:
			return "MOV B,E";
		case 0x44:
			return "MOV B,H";
		case 0x45:
			return "MOV B,L";
		case 0x46:
			return "MOV B,M";
		case 0x47:
			return "MOV B,A";
		case 0x48:
			return "MOV C,B";
		case 0x49:
			return "MOV C,C";
		case 0x4a:
			return "MOV C,D";
		case 0x4b:
			return "MOV C,E";
		case 0x4c:
			return "MOV C,H";
		case 0x4d:
			return "MOV C,L";
		case 0x4e:
			return "MOV C,M";
		case 0x4f:
			return "MOV C,A";
		case 0x50:
			return "MOV D,B";
		case 0x51:
			return "MOV D,C";
		case 0x52:
			return "MOV D,D";
		case 0x53:
			return "MOV D,E";
		case 0x54:
			return "MOV D,H";
		case 0x55:
			return "MOV D,L";
		case 0x56:
			return "MOV D,M";
		case 0x57:
			return "MOV D,A";
		case 0x58:
			return "MOV E,B";
		case 0x59:
			return "MOV E,C";
		case 0x5a:
			return "MOV E,D";
		case 0x5b:
			return "MOV E,E";
		case 0x5c:
			return "MOV E,H";
		case 0x5d:
			return "MOV E,L";
		case 0x5e:
			return "MOV E,M";
		case 0x5f:
			return "MOV E,A";
		case 0x60:
			return "MOV H,B";
		case 0x61:
			return "MOV H,C";
		case 0x62:
			return "MOV H,D";
		case 0x63:
			return "MOV H,E";
		case 0x64:
			return "MOV H,H";
		case 0x65:
			return "MOV H,L";
		case 0x66:
			return "MOV H,M";
		case 0x67:
			return "MOV H,A";
		case 0x68:
			return "MOV L,B";
		case 0x69:
			return "MOV L,C";
		case 0x6a:
			return "MOV L,D";
		case 0x6b:
			return "MOV L,E";
		case 0x6c:
			return "MOV L,H";
		case 0x6d:
			return "MOV L,L";
		case 0x6e:
			return "MOV L,M";
		case 0x6f:
			return "MOV L,A";
		case 0x70:
			return "MOV M,B";
		case 0x71:
			return "MOV M,C";
		case 0x72:
			return "MOV M,D";
		case 0x73:
			return "MOV M,E";
		case 0x74:
			return "MOV M,H";
		case 0x75:
			return "MOV M,L";
		case 0x76:
			return "HALT";
		case 0x77:
			return "MOV M,A";
		case 0x78:
			return "MOV A,B";
		case 0x79:
			return "MOV A,C";
		case 0x7a:
			return "MOV A,D";
		case 0x7b:
			return "MOV A,E";
		case 0x7c:
			return "MOV A,H";
		case 0x7d:
			return "MOV A,L";
		case 0x7e:
			return "MOV A,M";
		case 0x7f:
			return "MOV A,A";
		case 0x80:
			return "ADD B";
		case 0x81:
			return "ADD C";
		case 0x82:
			return "ADD D";
		case 0x83:
			return "ADD E";
		case 0x84:
			return "ADD H";
		case 0x85:
			return "ADD L";
		case 0x86:
			return "ADD M";
		case 0x87:
			return "ADD A";
		case 0x88:
			return "ADC B";
		case 0x89:
			return "ADC C";
		case 0x8a:
			return "ADC D";
		case 0x8b:
			return "ADC E";
		case 0x8c:
			return "ADC H";
		case 0x8d:
			return "ADC L";
		case 0x8e:
			return "ADC M";
		case 0x8f:
			return "ADC A";
		case 0x90:
			return "SUB B";
		case 0x91:
			return "SUB C";
		case 0x92:
			return "SUB D";
		case 0x93:
			return "SUB E";
		case 0x94:
			return "SUB H";
		case 0x95:
			return "SUB L";
		case 0x96:
			return "SUB M";
		case 0x97:
			return "SUB A";
		case 0x98:
			return "SBB B";
		case 0x99:
			return "SBB C";
		case 0x9a:
			return "SBB D";
		case 0x9b:
			return "SBB E";
		case 0x9c:
			return "SBB H";
		case 0x9d:
			return "SBB L";
		case 0x9e:
			return "SBB M";
		case 0x9f:
			return "SBB A";
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
			return "AND M";
		case 0xa7:
			return "AND A";
		case 0xa8:
			return "XRA B";
		case 0xa9:
			return "XRA C";
		case 0xaa:
			return "XRA D";
		case 0xab:
			return "XRA E";
		case 0xac:
			return "XRA H";
		case 0xad:
			return "XRA L";
		case 0xae:
			return "XRA M";
		case 0xaf:
			return "XRA A";
		case 0xb0:
			return "ORA B";
		case 0xb1:
			return "ORA C";
		case 0xb2:
			return "ORA D";
		case 0xb3:
			return "ORA E";
		case 0xb4:
			return "ORA H";
		case 0xb5:
			return "ORA L";
		case 0xb6:
			return "ORA M";
		case 0xb7:
			return "ORA A";
		case 0xb8:
			return "CMP B";
		case 0xb9:
			return "CMP C";
		case 0xba:
			return "CMP D";
		case 0xbb:
			return "CMP E";
		case 0xbc:
			return "CMP H";
		case 0xbd:
			return "CMP L";
		case 0xbe:
			return "CMP M";
		case 0xbf:
			return "CMP A";
		case 0xc0:
			return "RNZ";
		case 0xc1:
			return "POP B";
		case 0xc2:
			disasm << "JNZ " << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xc3:
			disasm << "JMP " << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xc4:
			disasm << "CNZ " << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xc5:
			return "PUSH B";
		case 0xc6:
			disasm << "ADI " << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0xc7:
			return "RST 0";
		case 0xc8:
			return "RZ";
		case 0xc9:
			return "RET";
		case 0xca:
			disasm << "JZ " << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xcb:
			disasm << "JMP " << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xcc:
			disasm << "CZ " << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xcd:
			disasm << "CALL " << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xce:
			disasm << "ACI " << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0xcf:
			return "RST 1";
		case 0xd0:
			return "RNC";
		case 0xd1:
			return "POP D";
		case 0xd2:
			disasm << "JNC " << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xd3:
			disasm << "OUT " << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0xd4:
			disasm << "CNC " << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xd5:
			return "PUSH D";
		case 0xd6:
			disasm << "SUI " << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0xd7:
			return "RST 2";
		case 0xd8:
			return "RC";
		case 0xd9:
			return "RET";
		case 0xda:
			disasm << "JC " << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xdb:
			disasm << "IN " << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0xdc:
			disasm << "CC " << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xdd:
			disasm << "CALL " << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xde:
			disasm << "SBI " << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0xdf:
			return "RST 3";
		case 0xe0:
			return "RPO";
		case 0xe1:
			return "POP H";
		case 0xe2:
			disasm << "JPO " << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xe3:
			return "XTHL";
		case 0xe4:
			disasm << "CPO " << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xe5:
			return "PUSH H";
		case 0xe6:
			disasm << "ANI " << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0xe7:
			return "RST 4";
		case 0xe8:
			return "RPE";
		case 0xe9:
			return "PCHL";
		case 0xea:
			disasm << "JPE " << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xeb:
			return "XCHG";
		case 0xec:
			disasm << "CPE " << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xed:
			disasm << "CALL " << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xee:
			disasm << "XRI " << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0xef:
			return "RST 5";
		case 0xf0:
			return "RP";
		case 0xf1:
			return "POP PSW";
		case 0xf2:
			disasm << "JP " << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xf3:
			return "DI";
		case 0xf4:
			disasm << "CP " << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xf5:
			return "PUSH PSW";
		case 0xf6:
			disasm << "ORI " << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0xf7:
			return "RST 6";
		case 0xf8:
			return "RM";
		case 0xf9:
			return "SPHL";
		case 0xfa:
			disasm << "JM " << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xfb:
			return "EI";
		case 0xfc:
			disasm << "CM " << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xfd:
			disasm << "CALL " << std::hex << getNN(_memoryfn, address);
			return disasm.str();
		case 0xfe:
			disasm << "CPI " << std::hex << getN(_memoryfn, address);
			return disasm.str();
		case 0xff:
			return "RST 7";
		default:
			return "Impossible";
		}
	}
}
