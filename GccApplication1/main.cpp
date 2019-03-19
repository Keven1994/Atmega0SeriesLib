/*
* GccApplication1.cpp
*
* Created: 26.01.2019 16:32:01
* Author : Keven
*/
#include "test.hpp"
#ifndef TEST

#define F_CPU 4000000UL

#include <stddef.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define MEGA4808

#ifdef MEGA4808
//fix unavailable ports
#undef PORTB
#undef PORTE
///////////////////////
#include "mega4808/Atmega4808.hpp"
#include "hw_abstractions/SPI.hpp"

#elif defined(MEGA4809)
#include "mega4809/Atmega4809.hpp"

#endif

using namespace AVR::port;

using PortA = Port<A>;
using PortC = Port<C>;

using led1 = Pin<PortA, 2>;
using led2 = Pin<PortA, 2>;

static constexpr auto bufS = 20;
volatile static inline uint8_t dataBuf[bufS];
volatile static inline uint8_t ptr = 0;

using Spi = typename AVR::spi::SPIMaster<>;
ISR(SPI0_INT_vect){
	if(ptr < bufS) dataBuf[ptr++] = Spi::nonBlockReceive();
	SPI0.INTFLAGS = 0xff;
}

int main() {
	led1::setOutput();
	led1::on();
	led1::invert();

	Spi::init();
	Spi::enableInterrupt(Spi::InterruptControlBits::Ie);
	sei();

	volatile uint8_t it = 0;
	while(true){
		Spi::nonBlockSend(42);
		if(dataBuf[it++] != 0 )
			led1::invert();
		if(it >= bufS) it = 0;
			_delay_ms(500);
	}
	
}

#endif