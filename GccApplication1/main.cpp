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

#elif defined(MEGA4809)
#include "mega4809/Atmega4809.hpp"

#endif

using namespace AVR::port;

using PortA = Port<AVR::port::A>;
using PortC = Port<AVR::port::C>;

using led1 = Pin<PortA, 2>;
using led2 = Pin<PortA, 2>;
using ch0 = AVR::eventsystem::Channel<0>;
using Spi = typename AVR::spi::SPIMaster<AVR::spi::notBlocking,AVR::spi::Spis::spi0,true,false,true, AVR::spi::TransferMode::Mode0,false,false, AVR::spi:: Prescaler::Div16>;

static constexpr auto funcref = []() {return Spi::noneBlockReceive(); };

//testprogram will demonstrate how to safe cpu time
#include "hw_abstractions/TWI.hpp"
int main() {
	//every channel have its own generators
	using gens = typename ch0::generators;
	//set generator from channel 0 to port A pin 0
	//ch0::setGenerator<gens::template PortAGenerator<typename PortA::pins::pin0>>(); //use the nested pin type to ensure to not use unavailable pins
	//user port c listen to channel 0
	//ch0::registerListener<typename AVR::eventsystem::users<>::evportc>();
	
	//led1::setOutput();
	//led1::on();
	//Spi::init();
	uint8_t ctr = 0;
	//while(true){
		PORTA.DIR = 0xff;
		TWI0.CTRLA = 8;
		TWI0.MBAUD = 21;
		//TWI0.CTRLA = 3;
		TWI0.MCTRLA = 1;

		while(true){
			if(((TWI0.MSTATUS & TWI_BUSSTATE_enum::TWI_BUSSTATE_IDLE_gc) == TWI_BUSSTATE_enum::TWI_BUSSTATE_IDLE_gc) | 
			((TWI0.MSTATUS & TWI_BUSSTATE_enum::TWI_BUSSTATE_OWNER_gc) == TWI_BUSSTATE_enum::TWI_BUSSTATE_OWNER_gc)){
				TWI0.MADDR = 0x0f | (1<<7);
				while(! ((TWI0.MSTATUS & TWI_WIF_bm ) == TWI_WIF_bm));
				TWI0.MDATA = 42;
			//if((TWI0.MSTATUS & TWI_ACKACT_bm) == TWI_ACKACT_bm)
				//TWI0.MDATA = 42;
			//else {
				//TWI0.MCTRLB = TWI_FLUSH_bm;	
			//}
			} else {
				TWI0.MSTATUS = 0x1;
			}
			_delay_ms(200);
		//}
		/*
		if(ctr == 0)
		Spi::noneBlockSend(42);
		//blocks if flag if is set (if data was completely shifted out / in)
		auto msg = Spi::doIfTest < funcref >(Spi::InterruptFlagBits::Default_if);
		
		if (msg == 0) ctr++;

		if (ctr > 0 && msg != 0) {
			while(ctr-- > 0){
				led1::invert();
				_delay_ms(500);
			}
		}
		
		msg = 0;

		/*
		other code
		*/
	}
	
}

#endif