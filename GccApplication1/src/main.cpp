/*
* GccApplication1.cpp
*
* Created: 26.01.2019 16:32:01
* Author : Keven
*/

#ifndef TEST

#define MEGA4808
//CMake define AVR Makro ???????????????????????????? wtf
#undef AVR
#undef NDEBUG
#ifdef MEGA4808
//fix unavailable ports
#undef PORTB
#undef PORTE
///////////////////////
#include "../inc/mega4808/Atmega4808.hpp"
#include "../inc/Boards/CuriosityNanoIOT.hpp"
#include "../inc/hw_abstractions/SPI.hpp"
#include "../inc/hw_abstractions/Port.hpp"
#include "../inc/hw_abstractions/Eventsystem.hpp"
#include "../inc/hw_abstractions/TWI.hpp"
#include "../inc/hw_abstractions/USART.hpp"
#include "hw_abstractions/Delay.hpp"
#elif defined(MEGA4809)
#include "../inc/mega4809/Atmega4809.hpp"

#endif


using namespace AVR::port;

using PortA = Port<AVR::port::A>;
using PortC = Port<AVR::port::C>;

using spiRessource = AVR::rc::Instance<
AVR::spi::SPI, // using ressource SPI
AVR::rc::Number<0>, //using instance '0'
AVR::portmux::PortMux<0>>; // using portmux 0 alternative

using twiRessource = AVR::rc::Instance<
        AVR::twi::TWI, // using ressource SPI
        AVR::rc::Number<0>, //using instance '0'
        AVR::portmux::PortMux<0>>; // using portmux 0 alternative

using usartRessource = AVR::rc::Instance<
        AVR::usart::USART_Comp, // using ressource SPI
        AVR::rc::Number<2>, //using instance '0'
        AVR::portmux::PortMux<0>>; // using portmux 0 alternative



using RC = AVR::rc::RessourceController<spiRessource,twiRessource,usartRessource>; //acquire ressource
using res = RC::getRessource_t<spiRessource>; //get the ressource
using twires = RC::getRessource_t<twiRessource>;
using usartres = RC::getRessource_t<usartRessource>;
//using spi = AVR::spi::SPIMaster<AVR::spi::notBlocking<AVR::spi::useFifo<42>>,res, AVR::spi::WriteOnly>; // put spi ressource in
using spi = AVR::spi::SPISlave<AVR::notBlocking<AVR::UseFifo<42> ,AVR::NoInterrupts >,res, AVR::ReadWrite>; // put spi ressource in
using usart =AVR::usart::USART<AVR::notBlocking<AVR::UseFifo<42>, AVR::Interrupts<>>,usartres, AVR::WriteOnly>;
using twi = AVR::twi::TWIMaster<AVR::notBlocking<>,twires>;

using led1 = Pin<PortA, 2>;
using led2 = Pin<PortA, 2>;
using ch0 = AVR::eventsystem::Channel<0>;


enum class error : mem_width {
	Busy = 0, 
	notBusy = 42	
};

ISR(USART2_TXC_vect){
		usart::txHandler();
}


int main() {
    usart::init();

        while(true){

			usart::put('h');
			usart::put('e');
			usart::put('l');
			usart::put('l');
			usart::put('o');

            usart::periodic();

            AVR::safeDelay<AVR::ms,200>();
	}
	
}

#endif