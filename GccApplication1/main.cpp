/*
 * GccApplication1.cpp
 *
 * Created: 26.01.2019 16:32:01
 * Author : Keven
 */ 
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
	#include "mega4808/Atmega4808.hpp"
	
	using namespace mega4808; //specify used mmcu
	using portf = Atmega4808::Ports::portf; 
	using porta = Atmega4808::Ports::porta;
	using ch1 = Atmega4808::EventSystem::ch1;
#elif defined(MEGA4809)
	#include "mega4809/Atmega4809.hpp"
	
	using namespace mega4809; //specify used mmcu
	using portf = Atmega4809::Ports::portf;
	using porta = Atmega4809::Ports::porta;
	using ch1 = Atmega4809::EventSystem::ch1;
#endif
	
	enum class testEnum : mem_width {
		ab = 1 << 1,
		ac = 1 << 3
		};
	
int main( ) {
	portf::get<portf::registers::out>().on();
	//PORTF.OUT = 0xff;
}

	//ch1::setGenerator<ch1::PortAGenerator<0>::portAPin>();
	//EVSYS.USEREVOUTF = 1 << 1; //user pf listen to ch 1 ->
	//eventsystem::EventSystem::listenToChannel<1,eventsystem::EventSystem::portf>();