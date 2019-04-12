//
// Created by keven on 13.04.2019.
//

#pragma once

#include <type_traits>
#include "../tools/scoped.h"
#include <avr/interrupt.h>
#include <util/delay.h>

namespace AVR {
    struct us {
    };
    struct ms {
    };
    struct s {
    };

    template<typename T = ms>
            requires ( std::is_same_v<T, us>
    || std::is_same_v<T, ms> || std::is_same_v<T, s>)
    static inline void safeDelay(double n) {
        etl::Scoped<etl::DisbaleInterrupt<etl::RestoreState>, true> Guard;
        if constexpr(std::is_same_v<T, us>) {
            _delay_us(n);
        } else if constexpr (std::is_same_v<T, ms>) {
            _delay_ms(n);
        } else {
            _delay_ms(1000 * n);
        }
    }

    template<typename T, long long n>
    requires ( std::is_same_v<T, us>
    || std::is_same_v<T, ms> || std::is_same_v<T, s>)
    static inline void safeDelay() {
        etl::Scoped<etl::DisbaleInterrupt<etl::RestoreState>, true> Guard;
        if constexpr(std::is_same_v<T, us>) {
            _delay_us(n);
        } else if constexpr (std::is_same_v<T, ms>) {
            _delay_ms(n);
        } else {
            constexpr auto tmp = n * 1000;
            _delay_ms(tmp);
        }
    }

    template<typename T = ms>
            requires ( std::is_same_v<T, us>
    || std::is_same_v<T, ms> || std::is_same_v<T, s>)
    static inline void delay(double n) {
        if constexpr(std::is_same_v<T, us>) {
            _delay_us(n);
        } else if constexpr (std::is_same_v<T, ms>) {
            _delay_ms(n);
        } else {
            _delay_ms(1000 * n);
        }
    }

    template<typename T, long long n>
    requires ( std::is_same_v<T, us>
    || std::is_same_v<T, ms> || std::is_same_v<T, s>)
    static inline void delay() {
        if constexpr(std::is_same_v<T, us>) {
            _delay_us(n);
        } else if constexpr (std::is_same_v<T, ms>) {
            _delay_ms(n);
        } else {
            constexpr auto tmp = n * 1000;
            _delay_ms(tmp);
        }
    }
}