#pragma once
#include "xprocessors.h"
#include <map>
#include <memory>


namespace xprocessors {
	class CpuRegistry {
	public:
		using cpufactory_fn = std::function<UCpu ()>;

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
		static CpuRegistry& instance();

		void add(const string&, cpufactory_fn);
		UCpu create(const string&);
	};

	class CpuRegistryHandler {
	public:
		CpuRegistryHandler(const string& name, CpuRegistry::cpufactory_fn fn) {
			CpuRegistry::instance().add(name, fn);
		}
	};
}