//
// Created by keven on 12.04.2019.
//

#pragma once

#include "../MCUSelect.hpp"
#include "../hw_abstractions/USART.hpp"
#include "../hw_abstractions/RessourceController.hpp"

namespace AVR::details {

    template<typename res, auto size>
    struct _dbgout {

        NoConstructors(_dbgout);

        using usart = AVR::usart::USART<AVR::notBlocking<AVR::UseFifo<size>>, res, AVR::WriteOnly>;

        static inline void init() {
            usart::init();
        }

        static inline void put(char ch) {
            usart::put(ch);
        }

        static inline void put(const char *str) {
            while (*str) {
                usart::put(*str++);
            }
        }

        static inline bool isEmpty() {
            return usart::outputEmpty();
        }

        static inline void flush() {
            while (!usart::outputEmpty()) {
                usart::periodic();
            }
        }

    };
}