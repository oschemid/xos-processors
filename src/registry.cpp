#include "registry.h"


using namespace xprocessors;


CpuRegistry::CpuRegistryItem::CpuRegistryItem(const string& name, cpufactory_fn factory) :
	_name(name),
	_factory(factory)
{}

CpuRegistry::CpuRegistry()
{}

CpuRegistry::~CpuRegistry()
{}

CpuRegistry& CpuRegistry::instance() {
	static CpuRegistry _registry;

	return _registry;
}

void CpuRegistry::add(const string& name, cpufactory_fn factory) {
	_entries.insert({ name, CpuRegistryItem(name, factory) });
}

Cpu* CpuRegistry::create(const string& name) {
	auto it = _entries.find(name);
	if (it != _entries.end())
		return it->second.factory()();
	return nullptr;
}
