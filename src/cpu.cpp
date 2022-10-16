#include "cpu.h"
#include "registry.h"


using namespace xprocessors;


Cpu* Cpu::create(const string& name) {
	return CpuRegistry::instance().create(name);
}
