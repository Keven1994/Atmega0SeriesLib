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

            template<typename P, typename regs>
            struct Port;

            template<typename P, mem_width number>
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
                    return _port().IN & pinValue;
                }

                static inline void on() {
                    _port().OUT |= pinValue;
                }

                static inline void toggle() {
                    _port().OUT ^= pinValue;
                }

                static inline void setOutput() { //no option to write as template -> parse error in template argument list, bug?
                    _port().DIR |= pinValue;
                }

                static inline void setInput() {
                    _port().DIR &= ~pinValue;
                }

            };
        }

        template<typename... Pins>
        requires (details::samePorts<Pins...>())
        static inline void PinsOn() {
            using firstPin = typename utils::front<Pins...>::type;
            auto &Pval = firstPin::port::port().OUT;
            Pval |= (Pins::pinValue | ...);
        }

        template<typename... Pins>
        requires (details::samePorts<Pins...>())
        static inline void PinsDirIn() {
            using firstPin = typename utils::front<Pins...>::type;
            auto &Pval = firstPin::port::port().DIR;
            Pval &= ~(Pins::pinValue | ...);
        }

        template<typename... Pins>
        requires (details::samePorts<Pins...>())
        static inline void PinsDirOut() {
            using firstPin = typename utils::front<Pins...>::type;
            auto &Pval = firstPin::port::port().DIR;
            Pval |= (static_cast<mem_width >(Pins::pinValue) | ...);
        }

        template<typename... Pins>
        requires (details::samePorts<Pins...>())
        static inline void PinsDirToggle() {
            using firstPin = typename utils::front<Pins...>::type;
            auto &Pval = firstPin::port::port().DIRTGL;
            Pval = static_cast<mem_width >((Pins::pinValue | ...));
        }

        template<typename... Pins>
        requires (details::samePorts<Pins...>())
        static inline void PinsOutToggle() {
            using firstPin = typename utils::front<Pins...>::type;
            auto &Pval = firstPin::port::port().OUTTGL;
            Pval = static_cast<mem_width >((Pins::pinValue | ...));
        }

        template<typename... Pins>
        requires (details::samePorts<Pins...>())

        static inline void PinsOff() {
            using firstPin = typename utils::front<Pins...>::type;
            auto &Pval = firstPin::port::port().OUT;
            Pval &= ~(Pins::pinValue | ...);
        }

        template<typename P, mem_width num>
        using Pin = details::PortPin<P,num>;
    }

}
