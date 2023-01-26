#include "xprocessors.h"
#include "registry.h"


using namespace xprocessors;


UCpu Cpu::create(const string& name)
{
	return CpuRegistry::instance().create(name);
}
