#pragma once
 #include "../../hw_abstractions/Port.hpp"
 #include "../../tools/meta.h"

namespace mega4808 {
    namespace usart_details {
        struct usartComponent {
            enum class CTRLAMasks : mem_width {
                Abeie = USART_ABEIE_bm,
                Dreie = USART_DREIE_bm,
                Lbme = USART_LBME_bm,
                Rxcie = USART_RXCIE_bm,
                Rxsie = USART_RXSIE_bm,
                Txcie = USART_TXCIE_bm,
                Rs485_off = USART_RS485_enum::USART_RS485_OFF_gc,
                Rs485_ext = USART_RS485_enum::USART_RS485_EXT_gc,
                Rs485_int = USART_RS485_enum::USART_RS485_INT_gc
            };
        
            enum class CTRLBMasks : mem_width {
                Mpcm = USART_MPCM_bm,
                Odme = USART_ODME_bm,
                Rxen = USART_RXEN_bm,
                Sfden = USART_SFDEN_bm,
                Txen = USART_TXEN_bm,
                Rxmode_normal = USART_RXMODE_enum::USART_RXMODE_NORMAL_gc,
                Rxmode_clk2x = USART_RXMODE_enum::USART_RXMODE_CLK2X_gc,
                Rxmode_genauto = USART_RXMODE_enum::USART_RXMODE_GENAUTO_gc,
                Rxmode_linauto = USART_RXMODE_enum::USART_RXMODE_LINAUTO_gc
            };
        
            enum class CTRLCMasks : mem_width {
                Ucpha = USART_UCPHA_bm,
                Udord = USART_UDORD_bm,
                Cmode_asynchronous = USART_CMODE_enum::USART_CMODE_ASYNCHRONOUS_gc,
                Cmode_synchronous = USART_CMODE_enum::USART_CMODE_SYNCHRONOUS_gc,
                Cmode_ircom = USART_CMODE_enum::USART_CMODE_IRCOM_gc,
                Cmode_mspi = USART_CMODE_enum::USART_CMODE_MSPI_gc,
                Chsize_5bit = USART_CHSIZE_enum::USART_CHSIZE_5BIT_gc,
                Chsize_6bit = USART_CHSIZE_enum::USART_CHSIZE_6BIT_gc,
                Chsize_7bit = USART_CHSIZE_enum::USART_CHSIZE_7BIT_gc,
                Chsize_8bit = USART_CHSIZE_enum::USART_CHSIZE_8BIT_gc,
                Chsize_9bitl = USART_CHSIZE_enum::USART_CHSIZE_9BITL_gc,
                Chsize_9bith = USART_CHSIZE_enum::USART_CHSIZE_9BITH_gc,
                Pmode_disabled = USART_PMODE_enum::USART_PMODE_DISABLED_gc,
                Pmode_even = USART_PMODE_enum::USART_PMODE_EVEN_gc,
                Pmode_odd = USART_PMODE_enum::USART_PMODE_ODD_gc,
                Sbmode_1bit = USART_SBMODE_enum::USART_SBMODE_1BIT_gc,
                Sbmode_2bit = USART_SBMODE_enum::USART_SBMODE_2BIT_gc
            };
        
            enum class DBGCTRLMasks : mem_width {
                Abmbp = USART_ABMBP_bm,
                Dbgrun = USART_DBGRUN_bm
            };
        
            enum class EVCTRLMasks : mem_width {
                Irei = USART_IREI_bm
            };
        
            enum class RXDATAHMasks : mem_width {
                Bufovf = USART_BUFOVF_bm,
                Data8 = USART_DATA8_bm,
                Ferr = USART_FERR_bm,
                Perr = USART_PERR_bm,
                Rxcif = USART_RXCIF_bm
            };
        
            enum class RXDATALMasks : mem_width {
                Data0 = USART_DATA0_bm,
                Data1 = USART_DATA1_bm,
                Data2 = USART_DATA2_bm,
                Data3 = USART_DATA3_bm,
                Data4 = USART_DATA4_bm,
                Data5 = USART_DATA5_bm,
                Data6 = USART_DATA6_bm,
                Data7 = USART_DATA7_bm
            };
        
            enum class RXPLCTRLMasks : mem_width {
                Rxpl0 = USART_RXPL0_bm,
                Rxpl1 = USART_RXPL1_bm,
                Rxpl2 = USART_RXPL2_bm,
                Rxpl3 = USART_RXPL3_bm,
                Rxpl4 = USART_RXPL4_bm,
                Rxpl5 = USART_RXPL5_bm,
                Rxpl6 = USART_RXPL6_bm
            };
        
            enum class STATUSMasks : mem_width {
                Bdf = USART_BDF_bm,
                Dreif = USART_DREIF_bm,
                Isfif = USART_ISFIF_bm,
                Rxcif = USART_RXCIF_bm,
                Rxsif = USART_RXSIF_bm,
                Txcif = USART_TXCIF_bm,
                Wfb = USART_WFB_bm
            };
        
            enum class TXDATAHMasks : mem_width {
                Data8 = USART_DATA8_bm
            };
        
            enum class TXDATALMasks : mem_width {
                Data0 = USART_DATA0_bm,
                Data1 = USART_DATA1_bm,
                Data2 = USART_DATA2_bm,
                Data3 = USART_DATA3_bm,
                Data4 = USART_DATA4_bm,
                Data5 = USART_DATA5_bm,
                Data6 = USART_DATA6_bm,
                Data7 = USART_DATA7_bm
            };
        
            enum class TXPLCTRLMasks : mem_width {
                Txpl0 = USART_TXPL0_bm,
                Txpl1 = USART_TXPL1_bm,
                Txpl2 = USART_TXPL2_bm,
                Txpl3 = USART_TXPL3_bm,
                Txpl4 = USART_TXPL4_bm,
                Txpl5 = USART_TXPL5_bm,
                Txpl6 = USART_TXPL6_bm,
                Txpl7 = USART_TXPL7_bm
            };
        
            struct registers {
                using baud = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Data,void,uint16_t>,0x8>;
                using ctrla = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,CTRLAMasks>,0x5>;
                using ctrlb = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,CTRLBMasks>,0x6>;
                using ctrlc = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,CTRLCMasks>,0x7>;
                using ctrld = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Data>,0xA>;
                using dbgctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,DBGCTRLMasks>,0xB>;
                using evctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,EVCTRLMasks>,0xC>;
                using rxdatah = utils::Pair<reg::Register<reg::accessmode::ReadOnly,reg::specialization::Flag,RXDATAHMasks>,0x1>;
                using rxdatal = utils::Pair<reg::Register<reg::accessmode::ReadOnly,reg::specialization::Flag,RXDATALMasks>,0x0>;
                using rxplctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,RXPLCTRLMasks>,0xE>;
                using status = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Flag,STATUSMasks>,0x4>;
                using txdatah = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,TXDATAHMasks>,0x3>;
                using txdatal = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,TXDATALMasks>,0x2>;
                using txplctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,TXPLCTRLMasks>,0xD>;
                rxdatal::type Rxdatal;
                rxdatah::type Rxdatah;
                txdatal::type Txdatal;
                txdatah::type Txdatah;
                status::type Status;
                ctrla::type Ctrla;
                ctrlb::type Ctrlb;
                ctrlc::type Ctrlc;
                baud::type Baud;
                volatile mem_width reserved9;
                ctrld::type Ctrld;
                dbgctrl::type Dbgctrl;
                evctrl::type Evctrl;
                txplctrl::type Txplctrl;
                rxplctrl::type Rxplctrl;
            };
        
        };
    
        struct usarts {
            template<auto N ,bool dummy = true>
            struct inst;
        
            template<bool dummy>
            struct inst<0, dummy>
             {
                template<auto N ,bool dummy1 = true>
                struct alt;
            
                [[nodiscard,gnu::always_inline]] static inline auto& value()  { return USART0;}
            
                template<bool dummy1>
                struct alt<1, dummy1>
                 {
                    struct Rxd {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::porta>,5>;
                    };
                
                    struct Txd {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::porta>,4>;
                    };
                
                    struct Xck {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::porta>,6>;
                    };
                
                    struct Xdir {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::porta>,7>;
                    };
                
                    using list = Meta::List<typename Rxd::pin0, typename Txd::pin0, typename Xck::pin0, typename Xdir::pin0>;
                };
            
                template<bool dummy1>
                struct alt<0, dummy1>
                 {
                    struct Rxd {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::porta>,1>;
                    };
                
                    struct Txd {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::porta>,0>;
                    };
                
                    struct Xck {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::porta>,2>;
                    };
                
                    struct Xdir {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::porta>,3>;
                    };
                
                    using list = Meta::List<typename Rxd::pin0, typename Txd::pin0, typename Xck::pin0, typename Xdir::pin0>;
                };
            
            };
        
            template<bool dummy>
            struct inst<1, dummy>
             {
                template<auto N ,bool dummy1 = true>
                struct alt;
            
                [[nodiscard,gnu::always_inline]] static inline auto& value()  { return USART1;}
            
                template<bool dummy1>
                struct alt<0, dummy1>
                 {
                    struct Rxd {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::portc>,1>;
                    };
                
                    struct Txd {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::portc>,0>;
                    };
                
                    struct Xck {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::portc>,2>;
                    };
                
                    struct Xdir {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::portc>,3>;
                    };
                
                    using list = Meta::List<typename Rxd::pin0, typename Txd::pin0, typename Xck::pin0, typename Xdir::pin0>;
                };
            
            };
        
            template<bool dummy>
            struct inst<2, dummy>
             {
                template<auto N ,bool dummy1 = true>
                struct alt;
            
                [[nodiscard,gnu::always_inline]] static inline auto& value()  { return USART2;}
            
                template<bool dummy1>
                struct alt<1, dummy1>
                 {
                    struct Rxd {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::portf>,5>;
                    };
                
                    struct Txd {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::portf>,4>;
                    };
                
                    struct Xck {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::portf>,6>;
                    };
                
                    using list = Meta::List<typename Rxd::pin0, typename Txd::pin0, typename Xck::pin0>;
                };
            
                template<bool dummy1>
                struct alt<0, dummy1>
                 {
                    struct Rxd {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::portf>,1>;
                    };
                
                    struct Txd {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::portf>,0>;
                    };
                
                    struct Xck {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::portf>,2>;
                    };
                
                    struct Xdir {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::portf>,3>;
                    };
                
                    using list = Meta::List<typename Rxd::pin0, typename Txd::pin0, typename Xck::pin0, typename Xdir::pin0>;
                };
            
            };
        
        };
    
    }

}