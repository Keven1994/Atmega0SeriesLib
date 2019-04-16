#pragma once
 #include "../../hw_abstractions/Port.hpp"
 #include "../../tools/meta.h"

namespace mega4808 {
    namespace spi_details {
        struct spiComponent {
            enum class CTRLAMasks : mem_width {
                Clk2x = SPI_CLK2X_bm,
                Dord = SPI_DORD_bm,
                Enable = SPI_ENABLE_bm,
                Master = SPI_MASTER_bm,
                Presc_div4 = SPI_PRESC_enum::SPI_PRESC_DIV4_gc,
                Presc_div16 = SPI_PRESC_enum::SPI_PRESC_DIV16_gc,
                Presc_div64 = SPI_PRESC_enum::SPI_PRESC_DIV64_gc,
                Presc_div128 = SPI_PRESC_enum::SPI_PRESC_DIV128_gc
            };
        
            enum class CTRLBMasks : mem_width {
                Bufen = SPI_BUFEN_bm,
                Bufwr = SPI_BUFWR_bm,
                Ssd = SPI_SSD_bm,
                Mode_0 = SPI_MODE_enum::SPI_MODE_0_gc,
                Mode_1 = SPI_MODE_enum::SPI_MODE_1_gc,
                Mode_2 = SPI_MODE_enum::SPI_MODE_2_gc,
                Mode_3 = SPI_MODE_enum::SPI_MODE_3_gc
            };
        
            enum class INTCTRLMasks : mem_width {
                Dreie = SPI_DREIE_bm,
                Ie = SPI_IE_bm,
                Rxcie = SPI_RXCIE_bm,
                Ssie = SPI_SSIE_bm,
                Txcie = SPI_TXCIE_bm
            };
        
            enum class INTFLAGSMasks : mem_width {
                Bufovf = SPI_BUFOVF_bm,
                Dreif = SPI_DREIF_bm,
                Rxcif = SPI_RXCIF_bm,
                Ssif = SPI_SSIF_bm,
                Txcif = SPI_TXCIF_bm,
                If = SPI_IF_bm,
                Wrcol = SPI_WRCOL_bm
            };
        
            struct registers {
                using ctrla = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,CTRLAMasks>,0x0>;
                using ctrlb = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,CTRLBMasks>,0x1>;
                using data = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Data>,0x4>;
                using intctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,INTCTRLMasks>,0x2>;
                using intflags = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,INTFLAGSMasks>,0x3>;
                ctrla::type Ctrla;
                ctrlb::type Ctrlb;
                intctrl::type Intctrl;
                intflags::type Intflags;
                data::type Data;
            };
        
        };
    
        struct spis {
            template<auto N ,bool dummy = true>
            struct inst;
        
            template<bool dummy>
            struct inst<0, dummy>
             {
                template<auto N ,bool dummy1 = true>
                struct alt;
            
                [[nodiscard,gnu::always_inline]] static inline auto& value()  { return SPI0;}
            
                template<bool dummy1>
                struct alt<1, dummy1>
                 {
                    struct Miso {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::portc>,1>;
                    };
                
                    struct Mosi {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::portc>,0>;
                    };
                
                    struct Sck {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::portc>,2>;
                    };
                
                    struct Ss {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::portc>,3>;
                    };
                
                    using list = Meta::List<typename Miso::pin0, typename Mosi::pin0, typename Sck::pin0, typename Ss::pin0>;
                };
            
                template<bool dummy1>
                struct alt<0, dummy1>
                 {
                    struct Miso {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::porta>,5>;
                    };
                
                    struct Mosi {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::porta>,4>;
                    };
                
                    struct Sck {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::porta>,6>;
                    };
                
                    struct Ss {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::porta>,7>;
                    };
                
                    using list = Meta::List<typename Miso::pin0, typename Mosi::pin0, typename Sck::pin0, typename Ss::pin0>;
                };
            
            };
        
        };
    
    }

}