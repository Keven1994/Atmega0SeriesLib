#pragma once
 #include "../../hw_abstractions/Port.hpp"
 #include "../../hw_abstractions/Bit.hpp"

namespace mega4808 {
    struct adcComponent {
        enum class CALIBMasks : mem_width {
            dutycyc_duty50 = ADC_DUTYCYC_enum::ADC_DUTYCYC_DUTY50_gc,
            dutycyc_duty25 = ADC_DUTYCYC_enum::ADC_DUTYCYC_DUTY25_gc
        };
    
        enum class COMMANDMasks : mem_width {
            Stconv = ADC_STCONV_bm
        };
    
        enum class CTRLAMasks : mem_width {
            Enable = ADC_ENABLE_bm,
            Freerun = ADC_FREERUN_bm,
            Runstby = ADC_RUNSTBY_bm,
            ressel_10bit = ADC_RESSEL_enum::ADC_RESSEL_10BIT_gc,
            ressel_8bit = ADC_RESSEL_enum::ADC_RESSEL_8BIT_gc
        };
    
        enum class CTRLBMasks : mem_width {
            sampnum_acc1 = ADC_SAMPNUM_enum::ADC_SAMPNUM_ACC1_gc,
            sampnum_acc2 = ADC_SAMPNUM_enum::ADC_SAMPNUM_ACC2_gc,
            sampnum_acc4 = ADC_SAMPNUM_enum::ADC_SAMPNUM_ACC4_gc,
            sampnum_acc8 = ADC_SAMPNUM_enum::ADC_SAMPNUM_ACC8_gc,
            sampnum_acc16 = ADC_SAMPNUM_enum::ADC_SAMPNUM_ACC16_gc,
            sampnum_acc32 = ADC_SAMPNUM_enum::ADC_SAMPNUM_ACC32_gc,
            sampnum_acc64 = ADC_SAMPNUM_enum::ADC_SAMPNUM_ACC64_gc
        };
    
        enum class CTRLCMasks : mem_width {
            Sampcap = ADC_SAMPCAP_bm,
            presc_div2 = ADC_PRESC_enum::ADC_PRESC_DIV2_gc,
            presc_div4 = ADC_PRESC_enum::ADC_PRESC_DIV4_gc,
            presc_div8 = ADC_PRESC_enum::ADC_PRESC_DIV8_gc,
            presc_div16 = ADC_PRESC_enum::ADC_PRESC_DIV16_gc,
            presc_div32 = ADC_PRESC_enum::ADC_PRESC_DIV32_gc,
            presc_div64 = ADC_PRESC_enum::ADC_PRESC_DIV64_gc,
            presc_div128 = ADC_PRESC_enum::ADC_PRESC_DIV128_gc,
            presc_div256 = ADC_PRESC_enum::ADC_PRESC_DIV256_gc,
            refsel_intref = ADC_REFSEL_enum::ADC_REFSEL_INTREF_gc,
            refsel_vddref = ADC_REFSEL_enum::ADC_REFSEL_VDDREF_gc,
            refsel_vrefa = ADC_REFSEL_enum::ADC_REFSEL_VREFA_gc
        };
    
        enum class CTRLDMasks : mem_width {
            Sampdly0 = ADC_SAMPDLY0_bm,
            Sampdly1 = ADC_SAMPDLY1_bm,
            Sampdly2 = ADC_SAMPDLY2_bm,
            Sampdly3 = ADC_SAMPDLY3_bm,
            asdv_asvoff = ADC_ASDV_enum::ADC_ASDV_ASVOFF_gc,
            asdv_asvon = ADC_ASDV_enum::ADC_ASDV_ASVON_gc,
            initdly_dly0 = ADC_INITDLY_enum::ADC_INITDLY_DLY0_gc,
            initdly_dly16 = ADC_INITDLY_enum::ADC_INITDLY_DLY16_gc,
            initdly_dly32 = ADC_INITDLY_enum::ADC_INITDLY_DLY32_gc,
            initdly_dly64 = ADC_INITDLY_enum::ADC_INITDLY_DLY64_gc,
            initdly_dly128 = ADC_INITDLY_enum::ADC_INITDLY_DLY128_gc,
            initdly_dly256 = ADC_INITDLY_enum::ADC_INITDLY_DLY256_gc
        };
    
        enum class CTRLEMasks : mem_width {
            wincm_none = ADC_WINCM_enum::ADC_WINCM_NONE_gc,
            wincm_below = ADC_WINCM_enum::ADC_WINCM_BELOW_gc,
            wincm_above = ADC_WINCM_enum::ADC_WINCM_ABOVE_gc,
            wincm_inside = ADC_WINCM_enum::ADC_WINCM_INSIDE_gc,
            wincm_outside = ADC_WINCM_enum::ADC_WINCM_OUTSIDE_gc
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
            muxpos_ain0 = ADC_MUXPOS_enum::ADC_MUXPOS_AIN0_gc,
            muxpos_ain1 = ADC_MUXPOS_enum::ADC_MUXPOS_AIN1_gc,
            muxpos_ain2 = ADC_MUXPOS_enum::ADC_MUXPOS_AIN2_gc,
            muxpos_ain3 = ADC_MUXPOS_enum::ADC_MUXPOS_AIN3_gc,
            muxpos_ain4 = ADC_MUXPOS_enum::ADC_MUXPOS_AIN4_gc,
            muxpos_ain5 = ADC_MUXPOS_enum::ADC_MUXPOS_AIN5_gc,
            muxpos_ain6 = ADC_MUXPOS_enum::ADC_MUXPOS_AIN6_gc,
            muxpos_ain7 = ADC_MUXPOS_enum::ADC_MUXPOS_AIN7_gc,
            muxpos_ain8 = ADC_MUXPOS_enum::ADC_MUXPOS_AIN8_gc,
            muxpos_ain9 = ADC_MUXPOS_enum::ADC_MUXPOS_AIN9_gc,
            muxpos_ain10 = ADC_MUXPOS_enum::ADC_MUXPOS_AIN10_gc,
            muxpos_ain11 = ADC_MUXPOS_enum::ADC_MUXPOS_AIN11_gc,
            muxpos_ain12 = ADC_MUXPOS_enum::ADC_MUXPOS_AIN12_gc,
            muxpos_ain13 = ADC_MUXPOS_enum::ADC_MUXPOS_AIN13_gc,
            muxpos_ain14 = ADC_MUXPOS_enum::ADC_MUXPOS_AIN14_gc,
            muxpos_ain15 = ADC_MUXPOS_enum::ADC_MUXPOS_AIN15_gc,
            muxpos_dacref = ADC_MUXPOS_enum::ADC_MUXPOS_DACREF_gc,
            muxpos_tempsense = ADC_MUXPOS_enum::ADC_MUXPOS_TEMPSENSE_gc,
            muxpos_gnd = ADC_MUXPOS_enum::ADC_MUXPOS_GND_gc
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
        };
    
    };

    struct adcs {
        struct adc0 {
            [[nodiscard,gnu::always_inline]] static inline auto& value()  { return ADC0;}
        
            struct Ain {
                using pin0 = AVR::port::details::PortPin<AVR::port::D,0>;
                using pin1 = AVR::port::details::PortPin<AVR::port::D,1>;
                using pin2 = AVR::port::details::PortPin<AVR::port::D,2>;
                using pin3 = AVR::port::details::PortPin<AVR::port::D,3>;
                using pin4 = AVR::port::details::PortPin<AVR::port::D,4>;
                using pin5 = AVR::port::details::PortPin<AVR::port::D,5>;
                using pin6 = AVR::port::details::PortPin<AVR::port::D,6>;
                using pin7 = AVR::port::details::PortPin<AVR::port::D,7>;
                using pin8 = AVR::port::details::PortPin<AVR::port::E,0>;
                using pin9 = AVR::port::details::PortPin<AVR::port::E,1>;
                using pin10 = AVR::port::details::PortPin<AVR::port::E,2>;
                using pin11 = AVR::port::details::PortPin<AVR::port::E,3>;
                using pin12 = AVR::port::details::PortPin<AVR::port::F,2>;
                using pin13 = AVR::port::details::PortPin<AVR::port::F,3>;
                using pin14 = AVR::port::details::PortPin<AVR::port::F,4>;
                using pin15 = AVR::port::details::PortPin<AVR::port::F,5>;
            };
        
        };
    
    };

}