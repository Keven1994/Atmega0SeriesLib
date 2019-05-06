//
// Created by keven on 06.04.2019.
//

#pragma once


namespace AVR {

    namespace port {

        namespace details{

            template<typename first, typename ...T>
            [[nodiscard]] constexpr bool samePorts() noexcept {
                if constexpr(sizeof...(T) == 0)
                    return true;
                else {
                    if constexpr (!std::is_same<typename first::port, typename utils::front<T...>::type::port>::value)
                        return false;
                    else
                        return samePorts<T...>();
                }

            }

            template<typename val>
            static constexpr auto pinCount(){
                return sizeof(val)*8;
            }

            template<typename P, typename regs>
            struct Port;

            template<typename P, mem_width number>
            requires(number < pinCount<mem_width>())
            class PortPin {

                static inline auto &_port = P::port;

            public:

                using port = P;

                NoConstructors(PortPin);

                static inline constexpr auto pinValue = Bit(number);

                static inline void off() {
                    _port().OUT &= ~pinValue;
                }

                [[nodiscard]] static inline bool isOn(){
                    auto& reg = P::template get<typename P::registers::in>();
                    return reg.raw() & static_cast<mem_width >(pinValue);
                }

                static inline void on() {
                    auto& reg = P::template get<typename P::registers::out>();
                    reg.raw() |= static_cast<mem_width >(pinValue);
                }

                static inline void toggle() {
                    auto& reg = P::template get<typename P::registers::outtgl>();
                    reg.raw() = static_cast<mem_width >(pinValue);
                }

                static inline void setOutput() { //no option to write as template -> parse error in template argument list, bug?
                    auto& reg = P::template get<typename P::registers::dir>();
                    reg.raw() |= static_cast<mem_width >(pinValue);
                }

                static inline void setInput() {
                    auto& reg = P::template get<typename P::registers::dir>();
                    reg.raw() &= ~static_cast<mem_width >(pinValue);
                }

            };
        }

        template<typename... Pins>
        requires (details::samePorts<Pins...>())
        static inline void pinsOn() {
            using firstPin = typename utils::front<Pins...>::type;
            auto &Pval = firstPin::port::port().OUT;
            Pval |= (Pins::pinValue | ...);
        }

        template<typename... Pins>
        requires (details::samePorts<Pins...>())
        static inline void pinsDirIn() {
            using firstPin = typename utils::front<Pins...>::type;
            auto &Pval = firstPin::port::port().DIR;
            Pval &= ~(Pins::pinValue | ...);
        }

        template<typename... Pins>
        requires (details::samePorts<Pins...>())
        static inline void pinsDirOut() {
            using firstPin = typename utils::front<Pins...>::type;
            auto &Pval = firstPin::port::port().DIR;
            Pval |= (static_cast<mem_width >(Pins::pinValue) | ...);
        }

        template<typename... Pins>
        requires (details::samePorts<Pins...>())
        static inline void pinsDirToggle() {
            using firstPin = typename utils::front<Pins...>::type;
            auto &Pval = firstPin::port::port().DIRTGL;
            Pval = static_cast<mem_width >((Pins::pinValue | ...));
        }

        template<typename... Pins>
        requires (details::samePorts<Pins...>())
        static inline void pinsOutToggle() {
            using firstPin = typename utils::front<Pins...>::type;
            auto &Pval = firstPin::port::port().OUTTGL;
            Pval = static_cast<mem_width >((Pins::pinValue | ...));
        }

        template<typename... Pins>
        requires (details::samePorts<Pins...>())
        static inline void pinsOff() {
            using firstPin = typename utils::front<Pins...>::type;
            auto &Pval = firstPin::port::port().OUT;
            Pval &= ~(Pins::pinValue | ...);
        }

        template<typename P, mem_width num>
        using Pin = details::PortPin<P,num>;

    }

}
