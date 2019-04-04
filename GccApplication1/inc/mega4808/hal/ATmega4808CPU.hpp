#pragma once
 #include "../../hw_abstractions/Port.hpp"

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
            };
        
        };

    }

}