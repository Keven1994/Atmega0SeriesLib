//
// Created by keven on 24.04.2019.
//

#pragma once


namespace AVR::adc{

    struct Res_8_Bit{using reg = uint8_t;};
    struct Res_10_Bit {using reg = uint16_t;};
template<uint16_t val>
using SampleLengthUS = std::integral_constant<uint16_t, val>;
template<uint16_t val>
using SampleDelayUS = std::integral_constant<uint16_t, val>;

    template<typename... PINS>
    using ADC_Comp = typename DEFAULT_MCU::template ADC<false,PINS...>;

    using ADC_Comp_Unchecked = typename DEFAULT_MCU::template ADC<true>;

    using Accumulations = typename ADC_Comp<>::Accumulations;
    using Prescaler = typename ADC_Comp<>::Prescaler ;
    using WindowComparation = typename ADC_Comp<>::WindowComparation;

    template<typename instance, typename Resolution = Res_10_Bit, bool freerun = false, bool standby=false, Accumulations accumulations = Accumulations::Accs_1, Prescaler prescaler = Prescaler::Div2, WindowComparation window = WindowComparation::None, typename sampleLength = SampleLengthUS<0>, typename sampleDelay = SampleDelayUS<0>>
    class ADC {

        using component = typename ADC_Comp<>::Component_t;

        template<typename Reg>
        [[nodiscard,gnu::always_inline]] static inline auto& reg(){
            return AVR::port::details::getRegister<Reg,instance::value>();
        }

        using ControlA = typename component::registers::ctrla;
        using ControlB = typename component::registers::ctrlb;
        using ControlC = typename component::registers::ctrlc;
        using ControlD = typename component::registers::ctrld;
        using ControlE = typename component::registers::ctrle;
        using InterruptFlags = typename component::registers::intflags;
        using InterruptControl = typename component::registers::intctrl;

    public:

        using Setting = typename ADC_Comp<>::template ADCSetting<freerun,std::is_same_v<Resolution,Res_8_Bit>,standby,accumulations,prescaler,window,sampleDelay,sampleLength>;

        [[gnu::always_inline]] static inline void init(){


        }

        template<typename Pin>
        static inline void selectChannel(){

        }

        template<bool dummy = true, typename T = std::enable_if_t<dummy && !freerun>>
        static inline void startConversion(){

        }

        static inline typename Resolution::reg value(){

        }
    };

}