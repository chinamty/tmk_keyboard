/*
Copyright 2013 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef ACTION_MACRO_H
#define ACTION_MACRO_H
#include <stdint.h>
#include "progmem.h"


#define MACRO_NONE      0
#define MACRO(...)      ({ static const macro_t __m[] PROGMEM = { __VA_ARGS__ }; &__m[0]; })
#define MACRO_GET(p)    pgm_read_byte(p)
#define MACRO_GET16(p)    pgm_read_word(p)

typedef uint8_t macro_t;


#ifndef NO_ACTION_MACRO
void action_macro_play(const macro_t *macro_p);
#else
#define action_macro_play(macro)
#endif



/* Macro commands
 *   code(0x04-73)                      // key down(1byte)
 *   code(0x04-73) | 0x80               // key up(1byte)
 *   { KEY_DOWN, code(0x04-0xff) }      // key down(2bytes)
 *   { KEY_UP,   code(0x04-0xff) }      // key up(2bytes)
 *   WAIT                               // wait milli-seconds
 *   INTERVAL                           // set interval between macro commands
 *   END                                // stop macro execution
 *
 * Ideas(Not implemented):
 *   modifiers
 *   system usage
 *   consumer usage
 *   unicode usage
 *   function call
 *   conditionals
 *   loop
 */
enum macro_command_id{
    /* 0x00 - 0x03 */
    END                 = 0x00,
    KEY_DOWN,
    KEY_UP,
    /* 0x04 - 0x73(F24) (reserved for keycode tap) */

    /* 0x74 - 0x83 */
    WAIT                = 0x74,
    INTERVAL,
    MOD_STORE,
    MOD_RESTORE,
    MOD_CLEAR,
    UNICODE,
    EMOJI               = 0x80,

    ACTION_DOWN         = 0x81,
    ACTION_UP,
    ACTION_TAP,
    /* 0x84 - 0xf3 (reserved for keycode tap with left shift) */

    /* 0xf4 - 0xff */
};


/* TODO: keycode:0x04-0x73 can be handled by 1byte command  else 2bytes are needed
 * if keycode between 0x04 and 0x73
 *      keycode / (keycode|0x80)
 * else
 *      {KEY_DOWN, keycode} / {KEY_UP, keycode}
*/
#define DOWN(key)       KEY_DOWN, (key)
#define UP(key)         KEY_UP, (key)
#define TYPE(key)       DOWN(key), UP(key)
#define WAIT(ms)        WAIT, (ms/2)
#define INTERVAL(ms)    INTERVAL, (ms)
#define STORE()         MOD_STORE
#define RESTORE()       MOD_RESTORE
#define CLEAR()         MOD_CLEAR
#define UNICODE(code)   UNICODE, (code&0xff), (code>>8)
#define EMOJI(code)     EMOJI,   (code&0xff), (code>>8)
#define ACTION_DOWN(code)  ACTION_DOWN,((code)&0xff), ((code)>>8)
#define ACTION_UP(code)    ACTION_UP,  ((code)&0xff), ((code)>>8)
#define ACTION_TAP(code)   ACTION_TAP, ((code)&0xff), ((code)>>8)

/* key down */
#define D(key)          DOWN(KC_##key)
#define DA(code)        ACTION_DOWN(code)
/* key up */
#define U(key)          UP(KC_##key)
#define UA(code)        ACTION_UP(code)
/* key type */
//#define T(key)          TYPE(KC_##key)
#define T(key)          KC_##key //Only for key(0x04 - 0x73, A - F24), not mods
#define TS(key)         KC_##key | 0x80 //Only for key(0x04 - 0x73, A - F24), not mods
#define TA(code)        ACTION_TAP(code) //Only for key(0x04 - 0x73, A - F24), not mods
/* wait */
#define W(ms)           WAIT(ms)
/* interval */
#define I(ms)           INTERVAL(ms)
/* store modifier(s) */
#define SM()            STORE()
/* restore modifier(s) */
#define RM()            RESTORE()
/* clear modifier(s) */
#define CM()            CLEAR()

#define UC(code)        UNICODE(code)
#define EM(code)        EMOJI(code)

/* for backward comaptibility */
#define MD(key)         DOWN(KC_##key)
#define MU(key)         UP(KC_##key)


#endif /* ACTION_MACRO_H */
