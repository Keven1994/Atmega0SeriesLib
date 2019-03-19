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
	
#elif defined(MEGA4809)
	#include "mega4809/Atmega4809.hpp"
	
#endif
	
using namespace AVR::port;

using PortF = Port<F>;
using PortC = Port<C>;

using led1 = Pin<PortF, 1>;
using led2 = Pin<PortF, 2>;
	
	
int main() {

	PortF::get<Port_Registers<>::dir>().on();
	PinsOn<led1,led2>();

	mega4808::Atmega4808::SPI::SpiMaster<>::init();

	while(true){
		led1::invert();
		_delay_ms(500);
		led2::invert();
		_delay_ms(500);
	}
	
}

#endif