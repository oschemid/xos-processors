#pragma once
#include "types.h"
#include "cpu.h"
#include <map>


namespace xprocessors {
	class CpuRegistry {
	public:
		typedef std::function<Cpu* ()> cpufactory_fn;

	protected:

		class CpuRegistryItem {
		protected:
			string _name;
			cpufactory_fn _factory;

		public:
			CpuRegistryItem(const string&, const cpufactory_fn);
			cpufactory_fn factory() const { return _factory; }
		};

		std::map<const string, const CpuRegistryItem> _entries;
	protected:
		CpuRegistry();
		~CpuRegistry();

	public:
		typedef std::function<Cpu* ()> cpufactory_fn;
		static CpuRegistry& instance();

		void add(const string&, cpufactory_fn);
		Cpu* create(const string&);
	};

	class CpuRegistryHandler {
	public:
		CpuRegistryHandler(const string& name, CpuRegistry::cpufactory_fn fn) {
			CpuRegistry::instance().add(name, fn);
		}
	};
}