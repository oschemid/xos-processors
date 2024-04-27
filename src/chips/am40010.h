#pragma once
#include "xprocessors.h"
#include "interfaces.h"
#include "./mc6845.h"


namespace xprocessors
{
	class am40010 : public interfaces::clockable
	{
	public:
		struct beam_signal
		{
			uint32_t color;
			bool hsync;
			bool vsync;
		};
	public:
		am40010();

		void init(xprocessors::mc6845*, uint8_t*);
		void tick() override;
		void tickio(const uint8_t);

		void memory_notifier(std::function<void(const bool, const bool)> fn) { _callback_memory = fn; }
		beam_signal get_beam() const { return _beam; }
	protected:
		uint8_t _selected_pen;
		uint8_t _color_border;
		uint8_t _color_pen[16];
		uint8_t _mode;
		std::function<void(const bool, const bool)> _callback_memory;
		xprocessors::mc6845* _crtc;
		uint8_t* _videoram;

		uint8_t _step{ 0 };
		uint8_t _val{ 0 };
		beam_signal _beam;
	};
}