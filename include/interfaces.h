#pragma once

namespace xprocessors
{
	namespace interfaces
	{
		class clockable
		{
		public:
			virtual void tick() = 0;
		};
	}
}
