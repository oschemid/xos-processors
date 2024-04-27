#include "am40010.h"


using namespace xprocessors;


static uint32_t cpc_colors[32] = {
	0xff6B7D6E,         // #40 white
	0xff6D7D6E,         // #41 white
	0xff6BF300,         // #42 sea green
	0xff6DF3F3,         // #43 pastel yellow
	0xff6B0200,         // #44 blue
	0xff6802F0,         // #45 purple
	0xff687800,         // #46 cyan
	0xff6B7DF3,         // #47 pink
	0xff6802F3,         // #48 purple
	0xff6BF3F3,         // #49 pastel yellow
	0xff0DF3F3,         // #4A bright yellow
	0xffF9F3FF,         // #4B bright white
	0xff0605F3,         // #4C bright red
	0xffF402F3,         // #4D bright magenta
	0xff0D7DF3,         // #4E orange
	0xffF980FA,         // #4F pastel magenta
	0xff680200,         // #50 blue
	0xff6BF302,         // #51 sea green
	0xff01F002,         // #52 bright green
	0xffF2F30F,         // #53 bright cyan
	0xff010200,         // #54 black
	0xffF4020C,         // #55 bright blue
	0xff017802,         // #56 green
	0xffF47B0C,         // #57 sky blue
	0xff680269,         // #58 magenta
	0xff6BF371,         // #59 pastel green
	0xff04F571,         // #5A lime
	0xffF4F371,         // #5B pastel cyan
	0xff01026C,         // #5C red
	0xffF2026C,         // #5D mauve
	0xff017B6E,         // #5E yellow
	0xffF67B6E,         // #5F pastel blue
};

am40010::am40010()
{
}

void am40010::init(xprocessors::mc6845* crtc, uint8_t* videoram)
{
	_crtc = crtc;
	_videoram = videoram;
}

void am40010::tick()
{
	if (_crtc->vsync())
	{
		_step = 0;
		_beam.hsync = _crtc->hsync();
		_beam.vsync = _crtc->vsync();
	}
	else
	{
		uint8_t col = 4; // border
		switch (_step)
		{
		case 0:
			_val = _videoram[_crtc->get_vma()];
			if (_mode == 1)
				col = (((_val >> 3) & 1) << 1) | ((_val >> 7) & 1);
			else
				col = (((_val >> 1) & 1) << 3) | (((_val >> 5) & 1) << 2) | (((_val >> 3) & 1) << 1) | ((_val >> 7) & 1);
			break;
		case 1:
			if (_mode==1)
				col = (((_val >> 2) & 1)<<1) | ((_val >> 6) & 1);
			else
				col = (((_val >> 1) & 1) << 3) | (((_val >> 5) & 1) << 2) | (((_val >> 3) & 1) << 1) | ((_val >> 7) & 1);
			break;
		case 2:
			if (_mode==1)
				col = (((_val >> 1) & 1)<<1) | ((_val >> 5) & 1);
			else
				col = ((_val & 1)<<3) | (((_val >> 4) & 1) << 2) | (((_val >> 2) & 1) << 1) | ((_val >> 6) & 1);
			break;
		case 3:
			if (_mode==1)
				col = (((_val)&1)<<1) | ((_val >> 4) & 1);
			else
				col = ((_val & 1) << 3) | (((_val >> 4) & 1) << 2) | (((_val >> 2) & 1) << 1) | ((_val >> 6) & 1);
			break;
		case 4:
			_val = _videoram[_crtc->get_vma()+1];
			if (_mode==1)
				col = (((_val >> 3) & 1) << 1) | (((_val >> 4) >> 3) & 1);
			else
				col = (((_val >> 1) & 1) << 3) | (((_val >> 5) & 1) << 2) | (((_val >> 3) & 1) << 1) | ((_val >> 7) & 1);
			break;
		case 5:
			if (_mode == 1)
				col = (((_val >> 2) & 1) << 1) | (((_val >> 4) >> 2) & 1);
			else
				col = (((_val >> 1) & 1) << 3) | (((_val >> 5) & 1) << 2) | (((_val >> 3) & 1) << 1) | ((_val >> 7) & 1);
			break;
		case 6:
			if (_mode == 1)
				col = (((_val >> 1) & 1) << 1) | (((_val >> 4) >> 1) & 1);
			else
				col = ((_val & 1) << 3) | (((_val >> 4) & 1) << 2) | (((_val >> 2) & 1) << 1) | ((_val >> 6) & 1);
			break;
		case 7:
			if (_mode == 1)
				col = (((_val) & 1) << 1) | (((_val >> 4)) & 1);
			else
				col = ((_val & 1) << 3) | (((_val >> 4) & 1) << 2) | (((_val >> 2) & 1) << 1) | ((_val >> 6) & 1);
			break;
		}
		uint32_t coul = cpc_colors[_color_pen[col]];
		if (!_crtc->de())
			coul = cpc_colors[_color_border];
		_beam.color = coul;
		_beam.hsync = _crtc->hsync();
		_beam.vsync = _crtc->vsync();
		_step = (_step + 1) & 7;
	}
}

void am40010::tickio(const uint8_t data)
{
    switch (data & 0xc0)
    {
    case 0:
        _selected_pen = data & 0x1f;
        break;
    case 0x40:
        if ((_selected_pen & 0x10) == 0x10)
            _color_border = data & 0x1f;
        else
            _color_pen[_selected_pen] = data & 0x1f;
        break;
    case 0x80:
        _mode = data & 0x03;
        _callback_memory((data & 0x04) ? false : true, (data & 0x08) ? false : true);
        break;
    }
}

