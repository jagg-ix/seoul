/**
 * Generic keyboard defines and scancode translations.
 * Documentation: PS2 hitrc chapter 11; Andries Brouwer Keyboard scancodes;
 *
 * Copyright (C) 2007-2008, Bernhard Kauer <bk@vmmon.org>
 *
 * This file is part of Vancouver.
 *
 * Vancouver is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Vancouver is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details.
 */
#pragma once

/**
 * We send set2 scancodes over the keycode bus including the following
 * flags.
 */
enum KBFLAGS {
  KBFLAG_RELEASE = 1 << 8,
  KBFLAG_EXTEND0 = 1 << 9,
  KBFLAG_EXTEND1 = 1 << 10,
  KBFLAG_NUM     = 1 << 11,
  KBFLAG_LSHIFT  = 1 << 12,
  KBFLAG_RSHIFT  = 1 << 13,
  KBFLAG_LALT    = 1 << 14,
  KBFLAG_RALT    = 1 << 15,
  KBFLAG_LCTRL   = 1 << 16,
  KBFLAG_RCTRL   = 1 << 17,
  KBFLAG_LWIN    = 1 << 18,
  KBFLAG_RWIN    = 1 << 19,
};



class GenericKeyboard
{

public:
  /**
   * Translate from scancode set2/3 to set1.
   */
  static unsigned char translate_sc2_to_sc1(unsigned char scancode)
  {
    static const unsigned char map[128] = {
      0xff, 0x43, 0x41, 0x3f, 0x3d, 0x3b, 0x3c, 0x58, 0x64, 0x44, 0x42, 0x40, 0x3e, 0x0f, 0x29, 0x59,
      0x65, 0x38, 0x2a, 0x70, 0x1d, 0x10, 0x02, 0x5a, 0x66, 0x71, 0x2c, 0x1f, 0x1e, 0x11, 0x03, 0x5b,
      0x67, 0x2e, 0x2d, 0x20, 0x12, 0x05, 0x04, 0x5c, 0x68, 0x39, 0x2f, 0x21, 0x14, 0x13, 0x06, 0x5d,
      0x69, 0x31, 0x30, 0x23, 0x22, 0x15, 0x07, 0x5e, 0x6a, 0x72, 0x32, 0x24, 0x16, 0x08, 0x09, 0x5f,
      0x6b, 0x33, 0x25, 0x17, 0x18, 0x0b, 0x0a, 0x60, 0x6c, 0x34, 0x35, 0x26, 0x27, 0x19, 0x0c, 0x61,
      0x6d, 0x73, 0x28, 0x74, 0x1a, 0x0d, 0x62, 0x6e, 0x3a, 0x36, 0x1c, 0x1b, 0x75, 0x2b, 0x63, 0x76,
      0x55, 0x56, 0x77, 0x78, 0x79, 0x7a, 0x0e, 0x7b, 0x7c, 0x4f, 0x7d, 0x4b, 0x47, 0x7e, 0x7f, 0x6f,
      0x52, 0x53, 0x50, 0x4c, 0x4d, 0x48, 0x01, 0x45, 0x57, 0x4e, 0x51, 0x4a, 0x37, 0x49, 0x46, 0x54,
    };
    switch (scancode)
    {
    case 0x00 ... 0x7f:
      return map[scancode];
    case 0x83: return 0x41; //F7
    case 0x84: return 0x54; //ALT+sysreq
    default:
      return scancode;
    }
  }

  /**
   * Translate from scancode set2 to set3.
   */
  static unsigned char translate_sc2_to_sc3(unsigned scancode)
  {
    static const unsigned char map[133] = {
      0x00, 0x47, 0x00, 0x27, 0x17, 0x07, 0x0f, 0x5e, 0x00, 0x4f, 0x3f, 0x2f, 0x1f, 0x0d, 0x0e, 0x00,
      0x00, 0x19, 0x12, 0x00, 0x11, 0x15, 0x16, 0x00, 0x00, 0x00, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x00,
      0x00, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x00, 0x00, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x00,
      0x00, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x00, 0x00, 0x00, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x00,
      0x00, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x00, 0x00, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x00,
      0x00, 0x00, 0x52, 0x00, 0x54, 0x55, 0x00, 0x00, 0x14, 0x59, 0x5a, 0x5b, 0x00, 0x5c, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x00, 0x00, 0x69, 0x00, 0x6b, 0x6c, 0x00, 0x00, 0x00,
      0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x08, 0x76, 0x56, 0x7c, 0x7a, 0x84, 0x7e, 0x7d, 0x5f, 0x00,
      0x00, 0x00, 0x00, 0x37, 0x57,
    };

    static const unsigned char emap[44] = {
      0x11, 0x39,
      0x14, 0x58,
      0x1f, 0x8b,
      0x27, 0x8c,
      0x2f, 0x8d,
      0x37, 0x00,
      0x3f, 0x7f,
      0x4a, 0x77,
      0x5a, 0x79,
      0x5e, 0x00,
      0x69, 0x65,
      0x6b, 0x61,
      0x6c, 0x6e,
      0x70, 0x67,
      0x71, 0x64,
      0x72, 0x60,
      0x74, 0x6a,
      0x75, 0x63,
      0x7a, 0x6d,
      0x7c, 0x57,
      0x7d, 0x6f,
      0x7e, 0x62,
    };

    unsigned char key = scancode;
    if (scancode & KBFLAG_EXTEND1 && key == 0x77)
      return 0x62;
    if (scancode & KBFLAG_EXTEND0)
      for (unsigned i=0; i<sizeof(emap); i+=2)
	if (emap[i*2] == key)
	  return emap[i*2+1];
    return (key < sizeof(map)) ? map[key] : 0x00;
  }


  /**
   * Translate from scancode set1 to set2.
   */
  static unsigned char translate_sc1_to_sc2(unsigned char scancode)
  {
    static const unsigned char map[128] = {
      0x00, 0x76, 0x16, 0x1e, 0x26, 0x25, 0x2e, 0x36, 0x3d, 0x3e, 0x46, 0x45, 0x4e, 0x55, 0x66, 0x0d,
      0x15, 0x1d, 0x24, 0x2d, 0x2c, 0x35, 0x3c, 0x43, 0x44, 0x4d, 0x54, 0x5b, 0x5a, 0x14, 0x1c, 0x1b,
      0x23, 0x2b, 0x34, 0x33, 0x3b, 0x42, 0x4b, 0x4c, 0x52, 0x0e, 0x12, 0x5d, 0x1a, 0x22, 0x21, 0x2a,
      0x32, 0x31, 0x3a, 0x41, 0x49, 0x4a, 0x59, 0x7c, 0x11, 0x29, 0x58, 0x05, 0x06, 0x04, 0x0c, 0x03,
      0x0b, 0x83, 0x0a, 0x01, 0x09, 0x77, 0x7e, 0x6c, 0x75, 0x7d, 0x7b, 0x6b, 0x73, 0x74, 0x79, 0x69,
      0x72, 0x7a, 0x70, 0x71, 0x84, 0x60, 0x61, 0x78, 0x07, 0x0f, 0x17, 0x1f, 0x27, 0x2f, 0x37, 0x3f,
      0x47, 0x4f, 0x56, 0x5e, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x57, 0x6f,
      0x13, 0x19, 0x39, 0x51, 0x53, 0x5c, 0x5f, 0x62, 0x63, 0x64, 0x65, 0x67, 0x68, 0x6a, 0x6d, 0x6e,
    };
    switch (scancode)
      {
      case 0xE1:
      case 0xE0:
	return scancode;
      default:
	return map[scancode & 0x7f];
      }
  }

  /**
   * ANSI map, escape sequences to set2 keycodes.
   */
  struct ansi_map{
    char escape[5];
    unsigned keycode;
  };

  static struct ansi_map *get_ansi_map()
  {
    static struct ansi_map map [] = {
      {"[P",   KBFLAG_EXTEND1 | 0x77}, // pause
      {"[A",   KBFLAG_EXTEND0 | 0x75}, // up
      {"[B",   KBFLAG_EXTEND0 | 0x72}, // down
      {"[C",   KBFLAG_EXTEND0 | 0x74}, // right
      {"[D",   KBFLAG_EXTEND0 | 0x6b}, // left
      {"OP",   0x05}, // F1
      {"OQ",   0x06}, // F2
      {"OR",   0x04}, // F3
      {"OS",   0x0c}, // F4
      {"[15~", 0x03}, // F5
      {"[17~", 0x0b}, // F6
      {"[18~", 0x83}, // F7
      {"[19~", 0x0a}, // F8
      {"[20~", 0x01}, // F9
      {"[21~", 0x09}, // F10
      {"[23~", 0x78}, // F11
      {"[24~", 0x07}, // F12
      {"[1~",  KBFLAG_EXTEND0 | 0x6c}, // home
      {"[2~",  KBFLAG_EXTEND0 | 0x70}, // insert
      {"[3~",  KBFLAG_EXTEND0 | 0x71}, // delete
      {"[4~",  KBFLAG_EXTEND0 | 0x69}, // end
      {"[5~",  KBFLAG_EXTEND0 | 0x7d}, // pgup
      {"[6~",  KBFLAG_EXTEND0 | 0x7a}, // pgdown
      {"\x1b", 0x76},                  // esc-esc
      {"", 0}, // table end indicator
    };
    return map;
  }

  static unsigned *get_ascii_map()
  {
    /**
     * ASCII chars to set2 keycodes.
     */
#define S(X) (X | KBFLAG_LSHIFT)
    static unsigned ascii_map [128] = {
      0,       0,       0,       0,       0,       0,       0,       0,       0,       0x0d,    0,       0,       0,       0x5a,    0,       0,
      0,       0,       0,       0,       0,       0,       0,       0,       0,       0,       0,       0,       0,       0,       0,       0,
      0x29,    S(0x16), S(0x52), S(0x26), S(0x25), S(0x2e), S(0x3d), 0x52,    S(0x46), S(0x45), S(0x3e), S(0x55), 0x41,    0x4e,    0x49,    0x4a,
      0x45,    0x16,    0x1e,    0x26,    0x25,    0x2e,    0x36,    0x3d,    0x3e,    0x46,    S(0x4c), 0x4c,    S(0x41), 0x55,    S(0x49), S(0x4a),
      S(0x1e), S(0x1c), S(0x32), S(0x21), S(0x23), S(0x24), S(0x2b), S(0x34), S(0x33), S(0x43), S(0x3b), S(0x42), S(0x4b), S(0x3a), S(0x31), S(0x44),
      S(0x4d), S(0x15), S(0x2d), S(0x1b), S(0x2c), S(0x3c), S(0x2a), S(0x1d), S(0x22), S(0x35), S(0x1a), 0x54,    0x5d,    0x5b,    S(0x36), S(0x4e),
      0x0e,    0x1c,    0x32,    0x21,    0x23,    0x24,    0x2b,    0x34,    0x33,    0x43,    0x3b,    0x42,    0x4b,    0x3a,    0x31,    0x44,
      0x4d,    0x15,    0x2d,    0x1b,    0x2c,    0x3c,    0x2a,    0x1d,    0x22,    0x35,    0x1a,    S(0x54), S(0x5d), S(0x5b), S(0x0e), 0x66,
    };
#undef S
    return ascii_map;
  }
};
