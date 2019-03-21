/*
* GccApplication1.cpp
*
* Created: 26.01.2019 16:32:01
* Author : Keven
*/
#include "test.hpp"
#define F_CPU 4000000
#ifndef TEST

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
#include "hw_abstractions/Port.hpp"

#elif defined(MEGA4809)
#include "mega4809/Atmega4809.hpp"

#endif

using namespace AVR::port;

using PortA = Port<AVR::port::A>;
using PortC = Port<AVR::port::C>;

using led1 = Pin<PortA, 2>;
using led2 = Pin<PortA, 2>;

using Spi = typename AVR::spi::SPIMaster<AVR::spi::notBlocking,true,false,true, AVR::spi::TransferMode::Mode0,false,false, AVR::spi:: Prescaler::Div16>;

static inline auto funcref = []() {return Spi::singleReceive(); };

//testprogram will demonstrate how to safe cpu time

int main() {
	led1::setOutput();
	led1::on();

	Spi::init();
	uint8_t ctr = 0;
	while(true){
		if(ctr == 0)
		Spi::nonBlockSend(42);
		//blocks if flag if is set (if data was completely shifted out / in)
		auto msg = Spi::doIfTest < funcref >(Spi::InterruptFlagBits::Default_if);
		
		if (msg == 0) ctr++;

		if (ctr > 0 && msg != 0) {
			for(uint8_t i = 0; i < ctr; i++){
			led1::invert();
			_delay_ms(500);
			}
			ctr = 0;

		}
		
		msg = 0;

		/*
		other code
		*/
	}
	
}

#endif