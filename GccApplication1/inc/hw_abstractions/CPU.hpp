//
// Created by keven on 04.04.2019.
//

#pragma once

#include "../MCUSelect.hpp"

namespace AVR::cpu {

    template<typename default_mcu = DEFAULT_MCU>
    class CPU {
        using sreg = typename default_mcu::Status::Component_t::registers::sreg::type;
    public:
        using status_bits = typename sreg::special_bit;
        static inline auto &getStatusRegister() {
            return sreg::getRegister(SREG);
        }
    };
}