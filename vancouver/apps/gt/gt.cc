/*
 * Graphic test.
 *
 * Copyright (C) 2009, Bernhard Kauer <bk@vmmon.org>
 *
 * This file is part of Vancouver.nova.
 *
 * Vancouver.nova is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation.
 *
 * Vancouver.nova is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details.
 */

#include "sys/console.h"
#include "driver/logging.h"
#include "vmm/message.h"
#include "vmm/timer.h"
#include <sigma0.h>

using namespace Nova;

class Gt : public ProgramConsole
{

  char *  _vesa_console;
  VgaRegs _vesaregs;
  ConsoleModeInfo _modeinfo;
  unsigned _frame;

  void animate()
  {
    _frame++;
    for (unsigned y = 0; y < _modeinfo.resolution[1]; y++)
      {
	unsigned char *v = reinterpret_cast<unsigned char *>(_vesa_console + y * _modeinfo.bytes_per_scanline);
	for (unsigned x = 0; x < _modeinfo.resolution[0]; x++)
	  {
	    unsigned value = (x+(_frame/8)) | (( (x % 128) + 128 + _frame) << 8) | ((y + (((_frame%256)*(_frame%256)/128))) << 16);
	    *v++ = value;
	    *v++ = value >> 8;
	    if (_modeinfo.bpp >= 24) *v++ = value >> 16;
	    if (_modeinfo.bpp >= 32) *v++ = value >> 24;
	  }
      }
  };


public:
  static void exit(unsigned long status)
  {
    Logging::printf("%s(%lx)\n", __func__, status);
  }

  int run(Hip *hip)
  {
    console_init("GT");
    Logging::printf("GT up and running\n");

    unsigned size = 0;
    unsigned mode = ~0;
    ConsoleModeInfo m;
    MessageConsole msg(0, &m);
    while (Sigma0Base::console(msg))      {
      Logging::printf("GT: %x %dx%d-%d sc %x\n", 
		      msg.index, m.resolution[0], m.resolution[1], m.bpp, m.bytes_per_scanline);
	// we like to have the 24/32bit mode with 1024
	if (!m.textmode && m.bpp >= 16 && m.resolution[0] == 1024)
	  {
	    size = m.resolution[1] * m.bytes_per_scanline;
	    mode = msg.index;
	    _modeinfo = m;
	  }
	msg.index++;
      }

    if (mode == ~0u) Logging::panic("have not found any 32bit graphic mode");

    _vesa_console = reinterpret_cast<char *>(memalign(0x1000, size));
    Logging::printf("GT: use %x %dx%d-%d %p size %x sc %x\n", 
		    mode, _modeinfo.resolution[0], _modeinfo.resolution[1], _modeinfo.bpp, _vesa_console, size, _modeinfo.bytes_per_scanline);

    MessageConsole msg2("GT2", _vesa_console, size, &_vesaregs);
    check(!Sigma0Base::console(msg2));
    _vesaregs.mode = mode;


    Logging::printf("request timer\n");
    TimerConsumer *timerconsumer = new TimerConsumer(hip->pre + hip->gsi);
    Sigma0Base::request_timer_attach(timerconsumer, timerconsumer->sm());
    Clock * clock = new Clock(hip->tsc_freq_khz*1000);

    // switch to the graphic console
    msg.type = MessageConsole::TYPE_SWITCH_VIEW;
    msg.view = 1;
    Sigma0Base::console(msg);

    Logging::printf("start animation\n");
    while (1)
      {
	MessageTimer msg3(0, clock->abstime(1, 50));
	Sigma0Base::timer(msg3);
	TimerItem *item = timerconsumer->get_buffer();
	timerconsumer->free_buffer();
	animate();
      }
  }
};

ASMFUNCS(Gt);
