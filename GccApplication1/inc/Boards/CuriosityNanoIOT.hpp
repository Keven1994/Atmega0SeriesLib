//
// Created by keven on 11.04.2019.
//




#pragma once

#include "../mega4808/Atmega4808.hpp"

#include "../hw_abstractions/USART.hpp"
#include "../hw_abstractions/RessourceController.hpp"


namespace AVR{
struct dbgout {

    NoConstructors(dbgout);

    using usartRessource = AVR::rc::Instance<
    AVR::usart::USART_Comp, // using ressource SPI
    AVR::rc::Number<2>, //using instance '0'
    AVR::portmux::PortMux<0>>; // using portmux 0 alternative
    using RC = AVR::rc::RessourceController<usartRessource>;
    using usartres = RC::getRessource_t<usartRessource>;
    using usart = AVR::usart::USART<AVR::notBlocking<AVR::UseFifo<42>>,usartres, AVR::WriteOnly>;

    static inline void init(){
        usart::init();
    }

    static inline void put(char ch){
        usart::put(ch);
    }

    static inline void put(const char* str){
        while (*str) {
            usart::put(*str++);
        }
    }

    static inline bool  isEmpty(){
        return usart::outputEmpty();
    }

    static inline void flush(){
        while(! usart::outputEmpty()) {
            usart::periodic();
        }
    }

};
}