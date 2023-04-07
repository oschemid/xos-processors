#include "cpu.h"
#include "../registry.h"


using namespace xprocessors;


Cpu::Ptr Cpu::create(const string& name)
{
	return Registry<Cpu::Ptr>::instance().create(name);
}
