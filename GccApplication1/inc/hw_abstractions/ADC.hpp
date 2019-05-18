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

    template<typename Comp, typename instance, typename Resolution = Res_10_Bit, bool freerun = false, bool standby=false, Accumulations accumulations = Accumulations::Accs_1, Prescaler prescaler = Prescaler::Div2, WindowComparation window = WindowComparation::None, typename sampleLength = SampleLengthUS<0>, typename sampleDelay = SampleDelayUS<0>>
    class ADC {

        using component = typename ADC_Comp<>::Component_t;
        using comps = ADC_Comp<>::Comps;

        template<typename Reg>
        [[nodiscard,gnu::always_inline]] static inline auto& reg(){
            static constexpr auto& val = comps::template inst<instance::Instance>::value;
            return AVR::port::details::getRegister<Reg, val>();
        }

        using ControlA = typename component::registers::ctrla;
        using ControlB = typename component::registers::ctrlb;
        using ControlC = typename component::registers::ctrlc;
        using ControlD = typename component::registers::ctrld;
        using ControlE = typename component::registers::ctrle;
        using Command = typename component::registers::command;
        using Result = typename component::registers::res;
        using Mux = typename component::registers::muxpos;
        using InterruptFlags = typename component::registers::intflags;
        using InterruptControl = typename component::registers::intctrl;

    public:

        using Setting = typename ADC_Comp<>::template ADCSetting<freerun,std::is_same_v<Resolution,Res_8_Bit>,standby,accumulations,prescaler,window,sampleDelay,sampleLength>;

        [[gnu::always_inline]] static inline void init(){
            reg<ControlB>().set(Setting::accumulations);
            reg<ControlC>().set(Setting::prescale);
            reg<ControlD>().set(Setting::sampledelay, Setting::samplelength);
            reg<ControlE>().set(Setting::windowcomp);
            reg<ControlA>().set(Setting::standby,Setting::freerun,Setting::resolution, ControlA::type::special_bit::Enable);
        }

        template<typename Pin>
        static inline void selectChannel(){
            reg<Mux>().set(Comp::template MuxResolve<typename comps::template inst<instance::Instance>::template alt<instance::Alternative>, Pin>::value);
        }

        template<bool dummy = true, typename T = std::enable_if_t<dummy && !freerun>>
        static inline void startConversion(){
            reg<Command>().set(Command::type::special_bit::Stconv);
        }

        static inline bool value(typename Resolution::reg& result){
            if(! reg<Command>().areSet(Command::type::special_bit::Stconv)){
                result = reg<Result>().raw();
                return true;
            }
            return false;
        }
    };

}