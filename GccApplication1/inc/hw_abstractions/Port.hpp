/*
* Port.h
*
* Created: 29.01.2019 11:45:45
*  Author: Keven
*/

#pragma once

#include "Register.hpp"
#include <type_traits>
#include "../tools/utils.h"
#include "../MCUSelect.hpp"
#include "PortPin.hpp"

namespace AVR {
    namespace port {

        struct Out {
        };
        struct In {
        };

        namespace details {

            template<typename T, auto &inst>
            [[nodiscard, gnu::always_inline]] static inline auto &getRegister() {
                using reg_t = typename T::type;
                auto offset = (mem_width *) &inst() + T::value;
                return reg_t::getRegister(*((typename reg_t::reg_size*)offset));
            }


            template<typename P, typename regs>
            struct Port {

                static inline auto &port = P::value;

                using pins = typename P::Pin;
                using registers = typename regs::registers;

                NoConstructors(Port);

                /* Convenience method to get a specific register */
                template<typename reg>
                static inline auto &get() {
                    return getRegister<reg, port>();
                }

                //"hard" dir setting of a port
                template<typename Dir, typename... pins>
                static inline void setDir()
                requires(std::is_same_v<Dir, port::Out>
                || std::is_same_v<Dir, port::In>){
                    auto &reg = get<typename registers::dir>();

                    static_assert(sizeof...(pins) == 0 || (std::is_same_v<typename pins::port, Port> && ...),
                                  "pins are from an different port");

                    if constexpr(std::is_same_v<Dir, Out>) {
                        if constexpr(sizeof...(pins) > 0)
                            reg.raw() = static_cast<mem_width >((pins::pinValue | ...));
                        else reg.on();
                    } else {
                        if constexpr(sizeof...(pins) > 0)
                            reg.raw() = ~static_cast<mem_width >((pins::pinValue | ...));
                        else reg.off();
                    }
                }

                //"hard" out setting of a port
                template<typename... pins>
                static inline void setOut()
                requires(sizeof...(pins) > 0){
                    auto &reg = get<typename registers::out>();

                    static_assert(sizeof...(pins) == 0 || (std::is_same_v<typename pins::port, Port> && ...),
                                  "pins are from an different port");

                            reg.raw() = static_cast<mem_width >((pins::pinValue | ...));
                }

                static inline void setOutValue(typename registers::out::type::reg_size value){
                    get<typename registers::out>().raw() = value;
                }

                [[nodiscard]] static inline auto readValue() {
                    return get<typename registers::in>().raw();
                };

                template<typename... pins>
                static inline void InputAnySet() requires(sizeof...(pins) > 0) {
                    auto &reg = get<typename registers::outtgl>();

                    static_assert(sizeof...(pins) == 0 || (std::is_same_v<typename pins::port, Port> && ...),
                                  "pins are from an different port");

                    reg.anySet((pins::pinValue, ...));
                }

                template<typename... pins>
                static inline void InputAllSet() requires(sizeof...(pins) > 0) {
                    auto &reg = get<typename registers::outtgl>();

                    static_assert(sizeof...(pins) == 0 || (std::is_same_v<typename pins::port, Port> && ...),
                                  "pins are from an different port");

                    reg.allSet((pins::pinValue, ...));
                }

                template<typename... pins>
                static inline void toggleOut() {
                    auto &reg = get<typename registers::outtgl>();

                    static_assert(sizeof...(pins) == 0 || (std::is_same_v<typename pins::port, Port> && ...),
                                  "pins are from an different port");

                    if constexpr(sizeof...(pins) > 0)
                        reg.toggle((pins::pinValue, ...));
                    else reg.toggle();
                }

                template<typename... pins>
                static inline void toggleDir() {
                    auto &reg = get<typename registers::outtgl>();

                    static_assert(sizeof...(pins) == 0 || (std::is_same_v<typename pins::port, Port> && ...),
                                  "pins are from an different port");

                    if constexpr(sizeof...(pins) > 0)
                        reg.toggle((pins::pinValue, ...));
                    else reg.toggle();
                }

            };


        }

        struct A {
        };
        struct B {
        };
        struct C {
        };
        struct D {
        };
        struct E {
        };
        struct F {
        };

        template<typename p, typename DefaultMCU = DEFAULT_MCU>
        using Port = typename DefaultMCU::template Port<p>;
    }
}
