#pragma once
#include "xprocessors.h"
#include <functional>


namespace xprocessors
{
	class i8255
	{
	public:
		using ohandler = std::function<void(const uint8_t)>;
		using ihandler = std::function<const uint8_t()>;

		i8255();
		void porta_handlers(ihandler in_fn, ohandler out_fn) { _porta_in = in_fn; _porta_out = out_fn; }
		void portb_handlers(ihandler in_fn, ohandler out_fn) { _portb_in = in_fn; _portb_out = out_fn; }
		void portc_handlers(ihandler in_fn, ohandler out_fn) { _portc_in = in_fn; _portc_out = out_fn; }

		void write(const uint8_t, const uint8_t); // A0-1 D0-7
		uint8_t read(const uint8_t); // A0-1

	protected:
		uint8_t _porta;
		ohandler _porta_out{ nullptr };
		ihandler _porta_in{ nullptr };
		uint8_t _portb;
		ohandler _portb_out{ nullptr };
		ihandler _portb_in{ nullptr };
		uint8_t _portc;
		ohandler _portc_out{ nullptr };
		ihandler _portc_in{ nullptr };

		bool _porta_is_out;
		bool _portb_is_out;
	};
}