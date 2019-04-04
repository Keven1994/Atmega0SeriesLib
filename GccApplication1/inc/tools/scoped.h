/*
 * WMuCpp - Bare Metal C++ 
 * Copyright (C) 2016, 2017, 2018 Wilhelm Meier <wilhelm.wm.meier@googlemail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

namespace AVR {
    template<typename T>
    auto getBaseAddr();
}

#include <type_traits>
#include <memory>
#include "../hw_abstractions/CPU.hpp"
#include "type_traits.h"

namespace etl {

    struct RestoreState {
    };
    struct ForceOn {
    };
    struct NoDisableEnable {
    };

    template<typename T = RestoreState>
    struct EnableInterrupt {
        typedef T type;
    };
    template<typename T = RestoreState>
    struct DisbaleInterrupt {
        typedef T type;
    };

    struct Transaction {
    };

    namespace details {

        template<typename T, bool Active, typename MCU, typename F1, typename F2>
        class _Scoped;

        template<typename T, bool Active, typename MCU, typename F1, typename F2>
        class Z_Scoped;


        template<bool Active, typename MCU, typename F1, typename F2>
        class _Scoped<EnableInterrupt<ForceOn>, Active, MCU, F1, F2> final {
        public:
            inline _Scoped() {
                if constexpr(Active) {
                    sei();
                }
            }

            inline ~_Scoped() {
                if constexpr(Active) {
                    cli();
                }
            }
        };

        template<bool Active, typename MCU, typename F1, typename F2>
        class _Scoped<EnableInterrupt<RestoreState>, Active, MCU, F1, F2> final {
            inline static constexpr auto status = AVR::getBaseAddr<typename MCU::Status>;
        public:
            inline _Scoped() {
                if constexpr(Active) {
                    v = status()->value.value();
                    sei();
                }
            }

            inline ~_Scoped() {
                if constexpr(Active) {
                    if (!etl::toBool(MCU::Status::Bits::globalIntEnable & v)) {
                        cli();
                    }
                }
            }

        private:
            typename MCU::Status::Bits v{0};
        };

        template<bool Active, typename MCU, typename F1, typename F2>
        class Z_Scoped<EnableInterrupt<RestoreState>, Active, MCU, F1, F2> final {
            inline static auto& status = AVR::cpu::CPU<>::getStatusRegister();
        public:
            inline Z_Scoped() {
                if constexpr(Active) {
                    sei();
                }
            }

            inline ~Z_Scoped() {
                if constexpr(Active) {
                    if (status.areSet(AVR::cpu::CPU<>::status_bits::I)) {
                        cli();
                    }
                }
            }

        };

        template<bool Active, typename MCU, typename F1, typename F2>
        class _Scoped<DisbaleInterrupt<ForceOn>, Active, MCU, F1, F2> final {
        public:
            inline _Scoped() {
                if constexpr(Active) {
                    cli();
                }
            }

            inline ~_Scoped() {
                if constexpr(Active) {
                    sei();
                }
            }
        };

        template<bool Active, typename MCU, typename F1, typename F2>
        class _Scoped<DisbaleInterrupt<NoDisableEnable>, Active, MCU, F1, F2> final {
        };

        template<bool Active, typename MCU, typename F1, typename F2>
        class _Scoped<DisbaleInterrupt<RestoreState>, Active, MCU, F1, F2> final {
            inline static constexpr auto status = AVR::getBaseAddr<typename MCU::Status>;
        public:
            inline _Scoped() {
                if constexpr(Active) {
                    v = status()->value.value();
                    cli();
                }
            }

            inline ~_Scoped() {
                if constexpr(Active) {
                    if (etl::toBool(MCU::Status::Bits::globalIntEnable & v)) {
                        sei();
                    }
                }
            }

        private:
            typename MCU::Status::Bits v{0};
        };

        template<bool Active, typename MCU, typename F1, typename F2>
        class Z_Scoped<DisbaleInterrupt<RestoreState>, Active, MCU, F1, F2> final {
            inline static auto& status = AVR::cpu::CPU<>::getStatusRegister();
        public:
            inline Z_Scoped() {
                if constexpr(Active) {
                    cli();
                }
            }

            inline ~Z_Scoped() {
                if constexpr(Active) {
                    if (status.areSet(AVR::cpu::CPU<>::status_bits::I)) {
                        sei();
                    }
                }
            }

        };

        template<typename MCU, typename F1, typename F2>
        class _Scoped<Transaction, true, MCU, F1, F2> final {
        public:
            inline _Scoped(F1 f1, F2 f2) : f2(std::move(f2)) {
                f1();
            }

            _Scoped(const _Scoped &) = delete;

            _Scoped(_Scoped &&) = delete;

            _Scoped &operator=(const _Scoped &) = delete;

            _Scoped &operator=(_Scoped &&) = delete;

            inline ~_Scoped() {
                f2();
            }

        private:
            F2 f2;
        };

        template<typename MCU, typename F1, typename F2>
        class Z_Scoped<Transaction, true, MCU, F1, F2> final {
        public:
            inline Z_Scoped(F1 f1, F2 f2) : f2(std::move(f2)) {
                f1();
            }

            Z_Scoped(const Z_Scoped &) = delete;

            Z_Scoped(Z_Scoped &&) = delete;

            Z_Scoped &operator=(const Z_Scoped &) = delete;

            Z_Scoped &operator=(Z_Scoped &&) = delete;

            inline ~Z_Scoped() {
                f2();
            }

        private:
            F2 f2;
        };

        // explicit deduction guide
        template<typename MCU, typename F1, typename F2>
        _Scoped(F1 f1, F2 f2) -> _Scoped<Transaction, true, MCU, F1, F2>;
    }

    template<typename T = Transaction, bool Active = true, typename MCU = DefaultMcuType, typename F1 = void, typename F2 = void>
    using Scoped = std::conditional_t<etl::Concepts::ZeroAVR<MCU> &&
                                      !std::is_same_v<DisbaleInterrupt<ForceOn>,T> && !std::is_same_v<DisbaleInterrupt<NoDisableEnable>,T>, details::Z_Scoped<T, Active, MCU, F1, F2>, details::_Scoped<T, Active, MCU, F1, F2>>;

}
