/*
Copyright 2011 Jun Wako <wakojun@gmail.com>

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

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "timer_avr.h"
#include "timer.h"


// counter resolution 1ms
// NOTE: union { uint32_t timer32; struct { uint16_t dummy; uint16_t timer16; }}
volatile uint32_t timer_count = 0;

void timer_init(void)
{
#ifdef ATMEGA32
    TCCR0 = (1<<WGM01);
#if TIMER_PRESCALER == 1
    TCCR0 |= (1<<CS00);
#elif TIMER_PRESCALER == 8
    TCCR0 |= (1<<CS01);
#elif TIMER_PRESCALER == 64
    TCCR0 |= (1<<CS00) | (1<<CS01);
#elif TIMER_PRESCALER == 256
    TCCR0 |= (1<<CS02);
#elif TIMER_PRESCALER == 1024
    TCCR0 |= (1<<CS00) | (1<<CS02);
#else
#   error "Timer prescaler value is NOT vaild."
#endif

    OCR0 = TIMER_RAW_TOP;
    TIMSK = (1<<OCIE0); // Allow compare interrupt
#else
    // Timer0 CTC mode
    TCCR0A = 0x02;

#if TIMER_PRESCALER == 1
    TCCR0B = 0x01;
#elif TIMER_PRESCALER == 8
    TCCR0B = 0x02;
#elif TIMER_PRESCALER == 64
    TCCR0B = 0x03;
#elif TIMER_PRESCALER == 256
    TCCR0B = 0x04;
#elif TIMER_PRESCALER == 1024
    TCCR0B = 0x05;
#else
#   error "Timer prescaler value is NOT vaild."
#endif

    OCR0A = TIMER_RAW_TOP;
#ifdef TIMSK0
    TIMSK0 = (1<<OCIE0A);
#else
    TIMSK = (1<<OCIE0A);
#endif
}

inline
void timer_clear(void)
{
    uint8_t sreg = SREG;
    cli();
    timer_count = 0;
    SREG = sreg;
}

inline
uint16_t timer_read(void)
{
    uint32_t t;

    uint8_t sreg = SREG;
    cli();
    t = timer_count;
    SREG = sreg;

    return (t & 0xFFFF);
}

inline
uint32_t timer_read32(void)
{
    uint32_t t;

    uint8_t sreg = SREG;
    cli();
    t = timer_count;
    SREG = sreg;

    return t;
}

inline
uint16_t timer_elapsed(uint16_t last)
{
    uint32_t t;

    uint8_t sreg = SREG;
    cli();
    t = timer_count;
    SREG = sreg;

    return TIMER_DIFF_16((t & 0xFFFF), last);
}

inline
uint32_t timer_elapsed32(uint32_t last)
{
    uint32_t t;

    uint8_t sreg = SREG;
    cli();
    t = timer_count;
    SREG = sreg;

    return TIMER_DIFF_32(t, last);
}

// excecuted once per 1ms.(excess for just timer count?)
//#ifndef __AVR_ATmega32A__
#ifndef ATMEGA32
#define TIMER_INTERRUPT_VECTOR TIMER0_COMPA_vect
#else
#define TIMER_INTERRUPT_VECTOR TIMER0_COMP_vect
#endif
ISR(TIMER_INTERRUPT_VECTOR, ISR_NOBLOCK)
{
    timer_count++;
}

