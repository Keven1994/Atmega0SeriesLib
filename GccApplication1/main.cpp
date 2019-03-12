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

#define MEGA4808

#ifdef MEGA4808
	//fix unavailable ports
	#undef PORTB
	#undef PORTE
	///////////////////////
	#include "mega4808/Atmega4808.hpp"
	
	using portf = mega4808::Atmega4808::Ports::portf; 
	using porta = mega4808::Atmega4808::Ports::porta;
	using ch1 = mega4808::Atmega4808::EventSystem::ch1;
#elif defined(MEGA4809)
	#include "mega4809/Atmega4809.hpp"
	
	using namespace mega4809; //specify used mmcu
	using portf = Atmega4809::Ports::portf;
	using porta = Atmega4809::Ports::porta;
	using ch1 = Atmega4809::EventSystem::ch1;
#endif
	
using PortF = AVR::port::Port<AVR::port::F>;
using PortC = AVR::port::Port<AVR::port::C>;

using led1 = AVR::port::Pin<PortF, 1>;
using led2 = AVR::port::Pin<PortF, 2>;
	
int main( ) {
	PortF::get<AVR::port::Port_Registers<>::dir>().on();
	PortF::getOutput().on(led1::pinValue, led2::pinValue);
	
	while(true){
		led1::invert();
		_delay_ms(500);
		led2::invert();
		_delay_ms(500);
	}
}

#endif