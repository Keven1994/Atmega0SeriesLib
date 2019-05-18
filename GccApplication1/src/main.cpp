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
using led1 = Pin<PortD, 4>;
using led2 = Pin<PortD, 6>;

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

        using ADC_Comp = AVR::adc::ADC_Comp<led1,led2>;
using adcRessource = AVR::rc::Instance<
        ADC_Comp , // using ressource SPI
        AVR::rc::Number<0>, //using instance '0'
        AVR::portmux::PortMux<0>>; // using portmux 0 alternative

        struct testPA {

            static inline bool process(uint8_t data){
                return true;
            }

        };



using led3 = Pin<PortD, 6>;


using RC = AVR::rc::RessourceController<twiRessource,usartRessource,adcRessource,spiRessource >; //acquire ressource
using twires = RC::getRessource<twiRessource>;
using res = RC::getRessource<spiRessource >;
using usartres = RC::getRessource<usartRessource>;
using adcres = RC::getRessource<adcRessource >;


using ADC1 = AVR::adc::ADC<ADC_Comp, adcres >;

using spi = AVR::spi::SPIMaster<AVR::notBlocking<AVR::NoFifo ,AVR::Interrupts<testPA> >,res, AVR::WriteOnly>;

using _twi = AVR::twi::TWIMaster<AVR::blocking, twires, AVR::ReadWrite>;

static inline auto handler = [](){
    while (!_twi::endTransaction());
    while(!_twi::startTransaction<42,AVR::twi::access::Read>());
};

using twi = AVR::twi::TWIMaster<AVR::blocking, twires, AVR::ReadWrite, handler>;

ISR(SPI0_INT_vect){
//    spi::intHandler(42);
}

int main() {
    AVR::dbgout::init();

 ADC1::init();
 ADC1::selectChannel<led1>();
 ADC1::startConversion();
 bool b = false;
 uint16_t result;
    while(true){
        if(ADC1::value(result)){
            b = true;
        }
        if(b){
            AVR::dbgout::put(utils::toString(result));
            AVR::dbgout::put('\n');
            AVR::dbgout::flush();
            ADC1::startConversion();
            b=false;
        }
        AVR::delay<AVR::ms,500>();
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