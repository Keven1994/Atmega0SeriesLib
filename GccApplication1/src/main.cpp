/*
* GccApplication1.cpp
*
* Created: 26.01.2019 16:32:01
* Author : Keven
*/

#ifndef TEST
#define TWITEST
#ifdef TWITEST
///////////////////////
#include "MCUSelect.hpp"
#include "../inc/Boards/CuriosityNanoIOT.hpp"
#include "../inc/hw_abstractions/SPI.hpp"
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



using twi = AVR::twi::TWIMaster<AVR::notBlocking<AVR::NoFifo,AVR::NoInterrupts>,twires , AVR::ReadOnly>;

volatile bool wasread = false;
static inline void Callback (){
    wasread=true;
    uint8_t item;
    //while(twi::getInputFifo().pop_front(item))
        //AVR::dbgout::put(item);
}
static constexpr uint8_t size = 12;
static uint8_t arr[size];

int main() {
    twi::init();

    while(true){

        uint8_t tmp = 0;
        auto rd = twi::scopedRead<42>();
        uint8_t data;
        while(tmp != 11){
            if(rd.receive(data))
                arr[tmp++] = data;
        }

        while(!rd.receiveLast(data));
        arr[tmp] = data;

        AVR::delay<AVR::ms,200>();

    }

}
#else

#include "MCUSelect.hpp"
#include "hw_abstractions/Delay.hpp"
#include "hw_abstractions/Eventsystem.hpp"

using portA = AVR::port::Port<AVR::port::A>;
using portC = AVR::port::Port<AVR::port::C>;
using led1 = AVR::port::Pin<portA,2>;
using led2 = typename portC::pins::pin3;
using led3 = AVR::port::Pin<portA,3>;
using led4 = AVR::port::Pin<portA,4>;
using led5 = AVR::port::Pin<portA,5>;

using ch0 = AVR::eventsystem::Channel<0>;
using ch0_gen = typename ch0::generators::generals::spi0_sck;
using user0 = typename ch0::users::evtca0;
using user1 = typename ch0::users::evtcb0;

int main() {
    ch0::template registerListener<user0,user1>();
    ch0::template setGenerator<ch0_gen >();
    ch0::softwareEvent();

    portA::setDir<AVR::port::Out>();

    led2::setOutput();
    while(true){
        led1::toggle();
        if(led2::isOn())
            led3::toggle();
        led2::toggle();
        AVR::port::pinsOutToggle<led4,led5>();
        AVR::delay<AVR::ms,500>();
    }
}
#endif


#endif