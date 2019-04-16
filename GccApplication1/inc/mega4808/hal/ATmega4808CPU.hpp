#pragma once
 #include "../../hw_abstractions/Port.hpp"
 #include "../../tools/meta.h"

namespace mega4808 {
    namespace cpu_details {
        struct cpuComponent {
            enum class CCPMasks : mem_width {
                Ccp_spm = CCP_SPM_gc,
                Ccp_ioreg = CCP_IOREG_gc
            };
        
            enum class SREGMasks : mem_width {
                C = CPU_C_bm,
                H = CPU_H_bm,
                I = CPU_I_bm,
                N = CPU_N_bm,
                S = CPU_S_bm,
                T = CPU_T_bm,
                V = CPU_V_bm,
                Z = CPU_Z_bm
            };
        
            struct registers {
                using ccp = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,CCPMasks>,0x4>;
                using rampz = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Data>,0xB>;
                using sph = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Data>,0xE>;
                using spl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Data>,0xD>;
                using sreg = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Flag,SREGMasks>,0xF>;
                volatile mem_width reserved0;
                volatile mem_width reserved1;
                volatile mem_width reserved2;
                volatile mem_width reserved3;
                ccp::type Ccp;
                volatile mem_width reserved5;
                volatile mem_width reserved6;
                volatile mem_width reserved7;
                volatile mem_width reserved8;
                volatile mem_width reserved9;
                volatile mem_width reserved10;
                rampz::type Rampz;
                volatile mem_width reserved12;
                spl::type Spl;
                sph::type Sph;
                sreg::type Sreg;
            };
        
        };
    
        struct cpus {
        
        };
    
    }

}