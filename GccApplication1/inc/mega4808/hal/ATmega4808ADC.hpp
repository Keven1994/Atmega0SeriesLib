#pragma once
 #include "../../hw_abstractions/Port.hpp"
 #include "../../tools/meta.h"

namespace mega4808 {
    namespace adc_details {
        struct adcComponent {
            enum class CALIBMasks : mem_width {
                Dutycyc_duty50 = ADC_DUTYCYC_enum::ADC_DUTYCYC_DUTY50_gc,
                Dutycyc_duty25 = ADC_DUTYCYC_enum::ADC_DUTYCYC_DUTY25_gc
            };
        
            enum class COMMANDMasks : mem_width {
                Stconv = ADC_STCONV_bm
            };
        
            enum class CTRLAMasks : mem_width {
                Enable = ADC_ENABLE_bm,
                Freerun = ADC_FREERUN_bm,
                Runstby = ADC_RUNSTBY_bm,
                Ressel_10bit = ADC_RESSEL_enum::ADC_RESSEL_10BIT_gc,
                Ressel_8bit = ADC_RESSEL_enum::ADC_RESSEL_8BIT_gc
            };
        
            enum class CTRLBMasks : mem_width {
                Sampnum_acc1 = ADC_SAMPNUM_enum::ADC_SAMPNUM_ACC1_gc,
                Sampnum_acc2 = ADC_SAMPNUM_enum::ADC_SAMPNUM_ACC2_gc,
                Sampnum_acc4 = ADC_SAMPNUM_enum::ADC_SAMPNUM_ACC4_gc,
                Sampnum_acc8 = ADC_SAMPNUM_enum::ADC_SAMPNUM_ACC8_gc,
                Sampnum_acc16 = ADC_SAMPNUM_enum::ADC_SAMPNUM_ACC16_gc,
                Sampnum_acc32 = ADC_SAMPNUM_enum::ADC_SAMPNUM_ACC32_gc,
                Sampnum_acc64 = ADC_SAMPNUM_enum::ADC_SAMPNUM_ACC64_gc
            };
        
            enum class CTRLCMasks : mem_width {
                Sampcap = ADC_SAMPCAP_bm,
                Presc_div2 = ADC_PRESC_enum::ADC_PRESC_DIV2_gc,
                Presc_div4 = ADC_PRESC_enum::ADC_PRESC_DIV4_gc,
                Presc_div8 = ADC_PRESC_enum::ADC_PRESC_DIV8_gc,
                Presc_div16 = ADC_PRESC_enum::ADC_PRESC_DIV16_gc,
                Presc_div32 = ADC_PRESC_enum::ADC_PRESC_DIV32_gc,
                Presc_div64 = ADC_PRESC_enum::ADC_PRESC_DIV64_gc,
                Presc_div128 = ADC_PRESC_enum::ADC_PRESC_DIV128_gc,
                Presc_div256 = ADC_PRESC_enum::ADC_PRESC_DIV256_gc,
                Refsel_intref = ADC_REFSEL_enum::ADC_REFSEL_INTREF_gc,
                Refsel_vddref = ADC_REFSEL_enum::ADC_REFSEL_VDDREF_gc,
                Refsel_vrefa = ADC_REFSEL_enum::ADC_REFSEL_VREFA_gc
            };
        
            enum class CTRLDMasks : mem_width {
                Sampdly0 = ADC_SAMPDLY0_bm,
                Sampdly1 = ADC_SAMPDLY1_bm,
                Sampdly2 = ADC_SAMPDLY2_bm,
                Sampdly3 = ADC_SAMPDLY3_bm,
                Asdv_asvoff = ADC_ASDV_enum::ADC_ASDV_ASVOFF_gc,
                Asdv_asvon = ADC_ASDV_enum::ADC_ASDV_ASVON_gc,
                Initdly_dly0 = ADC_INITDLY_enum::ADC_INITDLY_DLY0_gc,
                Initdly_dly16 = ADC_INITDLY_enum::ADC_INITDLY_DLY16_gc,
                Initdly_dly32 = ADC_INITDLY_enum::ADC_INITDLY_DLY32_gc,
                Initdly_dly64 = ADC_INITDLY_enum::ADC_INITDLY_DLY64_gc,
                Initdly_dly128 = ADC_INITDLY_enum::ADC_INITDLY_DLY128_gc,
                Initdly_dly256 = ADC_INITDLY_enum::ADC_INITDLY_DLY256_gc
            };
        
            enum class CTRLEMasks : mem_width {
                Wincm_none = ADC_WINCM_enum::ADC_WINCM_NONE_gc,
                Wincm_below = ADC_WINCM_enum::ADC_WINCM_BELOW_gc,
                Wincm_above = ADC_WINCM_enum::ADC_WINCM_ABOVE_gc,
                Wincm_inside = ADC_WINCM_enum::ADC_WINCM_INSIDE_gc,
                Wincm_outside = ADC_WINCM_enum::ADC_WINCM_OUTSIDE_gc
            };
        
            enum class DBGCTRLMasks : mem_width {
                Dbgrun = ADC_DBGRUN_bm
            };
        
            enum class EVCTRLMasks : mem_width {
                Startei = ADC_STARTEI_bm
            };
        
            enum class INTCTRLMasks : mem_width {
                Resrdy = ADC_RESRDY_bm,
                Wcmp = ADC_WCMP_bm
            };
        
            enum class INTFLAGSMasks : mem_width {
                Resrdy = ADC_RESRDY_bm,
                Wcmp = ADC_WCMP_bm
            };
        
            enum class MUXPOSMasks : mem_width {
                Muxpos_ain0 = ADC_MUXPOS_enum::ADC_MUXPOS_AIN0_gc,
                Muxpos_ain1 = ADC_MUXPOS_enum::ADC_MUXPOS_AIN1_gc,
                Muxpos_ain2 = ADC_MUXPOS_enum::ADC_MUXPOS_AIN2_gc,
                Muxpos_ain3 = ADC_MUXPOS_enum::ADC_MUXPOS_AIN3_gc,
                Muxpos_ain4 = ADC_MUXPOS_enum::ADC_MUXPOS_AIN4_gc,
                Muxpos_ain5 = ADC_MUXPOS_enum::ADC_MUXPOS_AIN5_gc,
                Muxpos_ain6 = ADC_MUXPOS_enum::ADC_MUXPOS_AIN6_gc,
                Muxpos_ain7 = ADC_MUXPOS_enum::ADC_MUXPOS_AIN7_gc,
                Muxpos_ain8 = ADC_MUXPOS_enum::ADC_MUXPOS_AIN8_gc,
                Muxpos_ain9 = ADC_MUXPOS_enum::ADC_MUXPOS_AIN9_gc,
                Muxpos_ain10 = ADC_MUXPOS_enum::ADC_MUXPOS_AIN10_gc,
                Muxpos_ain11 = ADC_MUXPOS_enum::ADC_MUXPOS_AIN11_gc,
                Muxpos_ain12 = ADC_MUXPOS_enum::ADC_MUXPOS_AIN12_gc,
                Muxpos_ain13 = ADC_MUXPOS_enum::ADC_MUXPOS_AIN13_gc,
                Muxpos_ain14 = ADC_MUXPOS_enum::ADC_MUXPOS_AIN14_gc,
                Muxpos_ain15 = ADC_MUXPOS_enum::ADC_MUXPOS_AIN15_gc,
                Muxpos_dacref = ADC_MUXPOS_enum::ADC_MUXPOS_DACREF_gc,
                Muxpos_tempsense = ADC_MUXPOS_enum::ADC_MUXPOS_TEMPSENSE_gc,
                Muxpos_gnd = ADC_MUXPOS_enum::ADC_MUXPOS_GND_gc
            };
        
            enum class SAMPCTRLMasks : mem_width {
                Samplen0 = ADC_SAMPLEN0_bm,
                Samplen1 = ADC_SAMPLEN1_bm,
                Samplen2 = ADC_SAMPLEN2_bm,
                Samplen3 = ADC_SAMPLEN3_bm,
                Samplen4 = ADC_SAMPLEN4_bm
            };
        
            enum class TEMPMasks : mem_width {
                Temp0 = ADC_TEMP0_bm,
                Temp1 = ADC_TEMP1_bm,
                Temp2 = ADC_TEMP2_bm,
                Temp3 = ADC_TEMP3_bm,
                Temp4 = ADC_TEMP4_bm,
                Temp5 = ADC_TEMP5_bm,
                Temp6 = ADC_TEMP6_bm,
                Temp7 = ADC_TEMP7_bm
            };
        
            struct registers {
                using calib = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,CALIBMasks>,0x16>;
                using command = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,COMMANDMasks>,0x08>;
                using ctrla = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,CTRLAMasks>,0x00>;
                using ctrlb = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,CTRLBMasks>,0x01>;
                using ctrlc = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,CTRLCMasks>,0x02>;
                using ctrld = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,CTRLDMasks>,0x03>;
                using ctrle = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,CTRLEMasks>,0x04>;
                using dbgctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,DBGCTRLMasks>,0x0C>;
                using evctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,EVCTRLMasks>,0x09>;
                using intctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,INTCTRLMasks>,0x0A>;
                using intflags = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Flag,INTFLAGSMasks>,0x0B>;
                using muxpos = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,MUXPOSMasks>,0x06>;
                using res = utils::Pair<reg::Register<reg::accessmode::ReadOnly,reg::specialization::Data,void,uint16_t>,0x10>;
                using sampctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,SAMPCTRLMasks>,0x05>;
                using temp = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,TEMPMasks>,0x0D>;
                using winht = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Data,void,uint16_t>,0x14>;
                using winlt = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Data,void,uint16_t>,0x12>;
                ctrla::type Ctrla;
                ctrlb::type Ctrlb;
                ctrlc::type Ctrlc;
                ctrld::type Ctrld;
                ctrle::type Ctrle;
                sampctrl::type Sampctrl;
                muxpos::type Muxpos;
                volatile mem_width reserved7;
                command::type Command;
                evctrl::type Evctrl;
                intctrl::type Intctrl;
                intflags::type Intflags;
                dbgctrl::type Dbgctrl;
                temp::type Temp;
                volatile mem_width reserved14;
                volatile mem_width reserved15;
                res::type Res;
                volatile mem_width reserved17;
                winlt::type Winlt;
                volatile mem_width reserved19;
                winht::type Winht;
                volatile mem_width reserved21;
                calib::type Calib;
            };
        
        };
    
        struct adcs {
            template<auto N ,bool dummy = true>
            struct inst;
        
            template<bool dummy>
            struct inst<0, dummy>
             {
                template<auto N ,bool dummy1 = true>
                struct alt;
            
                [[nodiscard,gnu::always_inline]] static inline auto& value()  { return ADC0;}
            
                template<bool dummy1>
                struct alt<0, dummy1>
                 {
                    struct Ain {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::portd>,0>;
                        using pin1 = AVR::port::details::PortPin<port_details::port<port_details::ports::portd>,1>;
                        using pin2 = AVR::port::details::PortPin<port_details::port<port_details::ports::portd>,2>;
                        using pin3 = AVR::port::details::PortPin<port_details::port<port_details::ports::portd>,3>;
                        using pin4 = AVR::port::details::PortPin<port_details::port<port_details::ports::portd>,4>;
                        using pin5 = AVR::port::details::PortPin<port_details::port<port_details::ports::portd>,5>;
                        using pin6 = AVR::port::details::PortPin<port_details::port<port_details::ports::portd>,6>;
                        using pin7 = AVR::port::details::PortPin<port_details::port<port_details::ports::portd>,7>;
                        using pin8 = AVR::port::details::PortPin<port_details::port<port_details::ports::portf>,2>;
                        using pin9 = AVR::port::details::PortPin<port_details::port<port_details::ports::portf>,3>;
                        using pin10 = AVR::port::details::PortPin<port_details::port<port_details::ports::portf>,4>;
                        using pin11 = AVR::port::details::PortPin<port_details::port<port_details::ports::portf>,5>;
                    };
                
                    using list = Meta::List<typename Ain::pin0, typename Ain::pin1, typename Ain::pin2, typename Ain::pin3, typename Ain::pin4, typename Ain::pin5, typename Ain::pin6, typename Ain::pin7, typename Ain::pin8, typename Ain::pin9, typename Ain::pin10, typename Ain::pin11>;
                };
            
            };
        
        };
    
    }

}