/*
* GccApplication1.cpp
*
* Created: 26.01.2019 16:32:01
* Author : Keven
*/
#include "test.hpp"
#ifndef TEST

#define F_CPU 4000000

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

using RC = AVR::rc::RessourceController<
	AVR::spi::Component<0,1>, AVR::spi::Component<0,1>>; //getting first instance here: spi0 and second alternative (portmux) here alt1

using led1 = Pin<PortA, 2>;
using led2 = Pin<PortA, 2>;
using ch0 = AVR::eventsystem::Channel<0>;
using spi = AVR::spi::SPIMaster<AVR::spi::notBlocking,RC::getRessource<0>::inst,RC::getRessource<0>::alt>;
using twi = AVR::twi::TWIMaster<>;

static constexpr auto funcref = []() {return spi::receive(); };

#include "hw_abstractions/TWI.hpp"

enum class error : mem_width {
	Busy = 0, 
	notBusy = 42	
};

static constexpr auto lam = [](){twi::startTransaction<0x0f,AVR::twi::direction::output>(); twi::singleTransfer(42); twi::stopTransaction(); return static_cast<mem_width>(error::notBusy);};

int main() {
	twi::init();

		while(true){
			auto err = twi::doIfAnySet<lam>(twi::status_bits::Busstate_idle , twi::status_bits::Busstate_owner);

			if( err != static_cast<mem_width>(error::notBusy)){
				/*
				if bus is busy...
				*/
			}

			_delay_ms(200);
		
	}
	
}

#endif