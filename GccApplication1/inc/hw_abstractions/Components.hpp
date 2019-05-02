//
// Created by keven on 09.04.2019.
//

#pragma once

#include <type_traits>
#include "../MCUSelect.hpp"
#include "../tools/concepts.h"
#include "../tools/fifo.h"
#include "Basics.hpp"


namespace AVR {

    namespace details {
        template<typename RW, typename accesstype, typename bit_width>
        struct Communication {
            using Use_Fifo = typename accesstype::fifo;
            static constexpr bool fifoEnabled = Use_Fifo::value > 0;
            static constexpr bool InterruptEnabled = accesstype::intEnabled;
            static constexpr bool isBlocking = std::is_same_v<accesstype,blocking>;
            static constexpr bool isReadOnly = std::is_same_v<RW,ReadOnly>;
            static constexpr bool isWriteOnly = std::is_same_v<RW,WriteOnly>;
            static_assert(! (fifoEnabled && isBlocking), "fifo combined with blocking mode is not allowed");
            using fifo_t = std::conditional_t<InterruptEnabled && fifoEnabled,
                    volatile etl::FiFo< bit_width,Use_Fifo::value> ,
                    std::conditional_t<! fifoEnabled,NoFifo,etl::FiFo< bit_width,Use_Fifo::value>>>;
            static inline std::conditional_t<isReadOnly,NoFifo,fifo_t> fifoOut{};
            static inline std::conditional_t<isWriteOnly,NoFifo,fifo_t> fifoIn{};
        };
    }

}