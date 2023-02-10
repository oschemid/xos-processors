#include "xprocessors.h"
#include "registry.h"


using namespace xprocessors;


Cpu::Ptr Cpu::create(const string& name)
{
	return CpuRegistry::instance().create(name);
}
