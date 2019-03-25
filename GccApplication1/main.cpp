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
#include "hw_abstractions/Eventsystem.hpp"
#include "hw_abstractions/TWI.hpp"

#elif defined(MEGA4809)
#include "mega4809/Atmega4809.hpp"

#endif

using namespace AVR::port;

using PortA = Port<AVR::port::A>;
using PortC = Port<AVR::port::C>;

using led1 = Pin<PortA, 2>;
using led2 = Pin<PortA, 2>;
using ch0 = AVR::eventsystem::Channel<0>;
using Spi = AVR::spi::SPIMaster<AVR::spi::notBlocking,AVR::spi::Spis::spi0,AVR::spi::Spis::spi0::Spi,true,false,true, AVR::spi::TransferMode::Mode0,false,false, AVR::spi:: Prescaler::Div16>;
using twi = AVR::twi::TWIMaster<>;
static constexpr auto funcref = []() {return Spi::noneBlockReceive(); };

//testprogram will demonstrate how to safe cpu time
#include "hw_abstractions/TWI.hpp"

static constexpr auto lam = [](){twi::singleTransfer<true,0x0f>(42); return 42;};

int main() {
	twi::init();

		while(true){
			auto err = twi::doIfAnySet<lam>(twi::Mstatus::special_bit::Busstate_idle , twi::Mstatus::special_bit::Busstate_owner);
			if(err != 42){
				TWI0.MSTATUS = 0x1;
			}

			_delay_ms(200);
		
	}
	
}

#endif