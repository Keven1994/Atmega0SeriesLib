/*
* main2.cpp
*
* Created: 11.03.2019 10:56:27
*  Author: keven
*/
#include "../test.hpp"
#if defined(TEST) && defined(SPIMASTER)

#define F_CPU 4000000UL

#include <stddef.h>
#include <util/delay.h>
#include <avr/io.h>

#define MEGA4808

#ifdef MEGA4808
//fix unavailable ports
#undef PORTB
#undef PORTE
///////////////////////
#include "../mega4808/Atmega4808.hpp"

using namespace mega4808; //specify used mmcu
using portf = Atmega4808::Ports::portf;
using porta = Atmega4808::Ports::porta;
using ch1 = Atmega4808::EventSystem::ch1;
#elif defined(MEGA4809)
#include "../mega4809/Atmega4809.hpp"

using namespace mega4809; //specify used mmcu
using portf = Atmega4809::Ports::portf;
using porta = Atmega4809::Ports::porta;
using ch1 = Atmega4809::EventSystem::ch1;
#endif

//working, tested: 11.03.2019 with logic analyzer

int main( ) {
	using _spi = Atmega4808::SPI::SpiMaster<>;

	_spi::init();
	
	while(true){
		_delay_ms(100);
		_spi::singleTransfer(42);
		_delay_ms(100);
		_spi::singleTransfer(21);
	}
}

#endif
