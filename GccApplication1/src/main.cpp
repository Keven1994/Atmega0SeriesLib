/*
* GccApplication1.cpp
*
* Created: 26.01.2019 16:32:01
* Author : Keven
*/

#ifndef TEST

///////////////////////
#include "MCUSelect.hpp"
#include "../inc/Boards/CuriosityNanoIOT.hpp"
#include "../inc/hw_abstractions/SPI.hpp"
#include "../inc/hw_abstractions/Port.hpp"
#include "../inc/hw_abstractions/Eventsystem.hpp"
#include "../inc/hw_abstractions/TWI.hpp"
#include "../inc/hw_abstractions/USART.hpp"
#include "../hw_abstractions/Delay.hpp"
#include "../hw_abstractions/ADC.hpp"


using namespace AVR::port;

using PortA = Port<AVR::port::A>;
using PortC = Port<AVR::port::C>;
using PortD = Port<AVR::port::D>;

using spiRessource = AVR::rc::Instance<
AVR::spi::SPI, // using ressource SPI
AVR::rc::Number<0>, //using instance '0'
AVR::portmux::PortMux<0>>; // using portmux 0 alternative

using twiRessource = AVR::rc::Instance<
        AVR::twi::TWI, // using ressource SPI
        AVR::rc::Number<0>, //using instance '0'
        AVR::portmux::PortMux<0>>; // using portmux 0 alternative
using test = typename mega4808::Atmega4808<>::template ADC<false,mega4808::port_details::ports::portd::Pin::pin7,mega4808::port_details::ports::portd::Pin::pin6>;
using adcRessource = AVR::rc::Instance<
       test, // using ressource SPI
        AVR::rc::Number<0>, //using instance '0'
        AVR::portmux::PortMux<0>>; // using portmux 0 alternative

using usartRessource = AVR::rc::Instance<
        AVR::usart::USART_Comp, // using ressource SPI
        AVR::rc::Number<2>, //using instance '0'
        AVR::portmux::PortMux<0>>; // using portmux 0 alternative


        struct testPA {

            static inline bool process(uint8_t data){
                return true;
            }

        };

using led1 = Pin<PortA, 2>;
using led2 = Pin<PortA, 2>;
using led3 = Pin<PortD, 6>;

using RC = AVR::rc::RessourceController<spiRessource,twiRessource,usartRessource,adcRessource >; //acquire ressource
using res = RC::getRessource_t<spiRessource>; //get the ressource
using twires = RC::getRessource_t<twiRessource>;
using usartres = RC::getRessource_t<usartRessource>;
using spi = AVR::spi::SPIMaster<AVR::notBlocking<AVR::UseFifo<42> ,AVR::Interrupts<> >,res, AVR::ReadWrite>; // put spi ressource in
using usart =AVR::usart::USART<AVR::notBlocking<AVR::UseFifo<42>, AVR::Interrupts<>>,usartres, AVR::WriteOnly>;
using usart1 =AVR::usart::USART<AVR::notBlocking<AVR::NoFifo , AVR::Interrupts<testPA>>,usartres, AVR::ReadWrite>;
using usart2 =AVR::usart::USART<AVR::blocking,usartres, AVR::ReadWrite>;
using twi = AVR::twi::TWIMaster<AVR::notBlocking<AVR::UseFifo<42>,AVR::Interrupts<>>,twires , AVR::ReadWrite>;

using ch0 = AVR::eventsystem::Channel<0>;


ISR(TWI0_TWIM_vect){
    twi::intHandler();
  //  PORTA.OUTTGL = 1 <<5;
}
volatile bool wasread = false;
static inline void Callback (){
    wasread=true;
    uint8_t item;
    while(twi::getInputFifo().pop_front(item))
        usart2::transfer(item);
}

int main() {
    usart2::init();
    PORTC.DIR = 0xff;
    twi::init();

        while(true){
            twi::get<42,Callback>(12);
            while(!wasread)
                ;


            AVR::delay<AVR::ms,200>();

            wasread = false;
	}
	
}

#endif