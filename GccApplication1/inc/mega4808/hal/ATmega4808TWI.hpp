#pragma once
#include "../../hw_abstractions/Port.hpp"
#include <util/twi.h>

namespace mega4808 {
	namespace twi_details {
        struct twiComponent {
            enum class BRIDGECTRLMasks : mem_width {
                Enable = TWI_ENABLE_bm,
                Fmpen = TWI_FMPEN_bm,
                Sdahold_off = TWI_SDAHOLD_enum::TWI_SDAHOLD_OFF_gc,
                Sdahold_50ns = TWI_SDAHOLD_enum::TWI_SDAHOLD_50NS_gc,
                Sdahold_300ns = TWI_SDAHOLD_enum::TWI_SDAHOLD_300NS_gc,
                Sdahold_500ns = TWI_SDAHOLD_enum::TWI_SDAHOLD_500NS_gc
            };

            enum class CTRLAMasks : mem_width {
                Fmpen = TWI_FMPEN_bm,
                Sdahold_off = TWI_SDAHOLD_enum::TWI_SDAHOLD_OFF_gc,
                Sdahold_50ns = TWI_SDAHOLD_enum::TWI_SDAHOLD_50NS_gc,
                Sdahold_300ns = TWI_SDAHOLD_enum::TWI_SDAHOLD_300NS_gc,
                Sdahold_500ns = TWI_SDAHOLD_enum::TWI_SDAHOLD_500NS_gc,
                Sdasetup_4cyc = TWI_SDASETUP_enum::TWI_SDASETUP_4CYC_gc,
                Sdasetup_8cyc = TWI_SDASETUP_enum::TWI_SDASETUP_8CYC_gc
            };

            enum class DBGCTRLMasks : mem_width {
                Dbgrun = TWI_DBGRUN_bm
            };

            enum class MCTRLAMasks : mem_width {
                Enable = TWI_ENABLE_bm,
                Qcen = TWI_QCEN_bm,
                Rien = TWI_RIEN_bm,
                Smen = TWI_SMEN_bm,
                Wien = TWI_WIEN_bm,
                Timeout_disabled = TWI_TIMEOUT_enum::TWI_TIMEOUT_DISABLED_gc,
                Timeout_50us = TWI_TIMEOUT_enum::TWI_TIMEOUT_50US_gc,
                Timeout_100us = TWI_TIMEOUT_enum::TWI_TIMEOUT_100US_gc,
                Timeout_200us = TWI_TIMEOUT_enum::TWI_TIMEOUT_200US_gc
            };

            enum class MCTRLBMasks : mem_width {
                Flush = TWI_FLUSH_bm,
                Ackact_ack = TWI_ACKACT_enum::TWI_ACKACT_ACK_gc,
                Ackact_nack = TWI_ACKACT_enum::TWI_ACKACT_NACK_gc,
                Mcmd_noact = TWI_MCMD_enum::TWI_MCMD_NOACT_gc,
                Mcmd_repstart = TWI_MCMD_enum::TWI_MCMD_REPSTART_gc,
                Mcmd_recvtrans = TWI_MCMD_enum::TWI_MCMD_RECVTRANS_gc,
                Mcmd_stop = TWI_MCMD_enum::TWI_MCMD_STOP_gc
            };

            enum class MSTATUSMasks : mem_width {
                Arblost = TWI_ARBLOST_bm,
                Buserr = TWI_BUSERR_bm,
                Clkhold = TWI_CLKHOLD_bm,
                Rif = TWI_RIF_bm,
                Rxack = TWI_RXACK_bm,
                Wif = TWI_WIF_bm,
                Busstate_unknown = TWI_BUSSTATE_enum::TWI_BUSSTATE_UNKNOWN_gc,
                Busstate_idle = TWI_BUSSTATE_enum::TWI_BUSSTATE_IDLE_gc,
                Busstate_owner = TWI_BUSSTATE_enum::TWI_BUSSTATE_OWNER_gc,
                Busstate_busy = TWI_BUSSTATE_enum::TWI_BUSSTATE_BUSY_gc
            };

            enum class SADDRMASKMasks : mem_width {
                Addren = TWI_ADDREN_bm
            };

            enum class SCTRLAMasks : mem_width {
                Apien = TWI_APIEN_bm,
                Dien = TWI_DIEN_bm,
                Enable = TWI_ENABLE_bm,
                Pien = TWI_PIEN_bm,
                Pmen = TWI_PMEN_bm,
                Smen = TWI_SMEN_bm
            };

            enum class SCTRLBMasks : mem_width {
                Ackact_ack = TWI_ACKACT_enum::TWI_ACKACT_ACK_gc,
                Ackact_nack = TWI_ACKACT_enum::TWI_ACKACT_NACK_gc,
                Scmd_noact = TWI_SCMD_enum::TWI_SCMD_NOACT_gc,
                Scmd_comptrans = TWI_SCMD_enum::TWI_SCMD_COMPTRANS_gc,
                Scmd_response = TWI_SCMD_enum::TWI_SCMD_RESPONSE_gc
            };

            enum class SSTATUSMasks : mem_width {
                Apif = TWI_APIF_bm,
                Buserr = TWI_BUSERR_bm,
                Clkhold = TWI_CLKHOLD_bm,
                Coll = TWI_COLL_bm,
                Dif = TWI_DIF_bm,
                Dir = TWI_DIR_bm,
                Rxack = TWI_RXACK_bm,
                Ap_stop = TWI_AP_enum::TWI_AP_STOP_gc,
                Ap_adr = TWI_AP_enum::TWI_AP_ADR_gc
            };

            struct registers {
                using bridgectrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,BRIDGECTRLMasks>,0x1>;
                using ctrla = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,CTRLAMasks>,0x0>;
                using dbgctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,DBGCTRLMasks>,0x2>;
                using maddr = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Data>,0x7>;
                using mbaud = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Data>,0x6>;
                using mctrla = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,MCTRLAMasks>,0x3>;
                using mctrlb = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,MCTRLBMasks>,0x4>;
                using mdata = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Data>,0x8>;
                using mstatus = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Flag,MSTATUSMasks>,0x5>;
                using saddr = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Data>,0xC>;
                using saddrmask = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,SADDRMASKMasks>,0xE>;
                using sctrla = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,SCTRLAMasks>,0x9>;
                using sctrlb = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,SCTRLBMasks>,0xA>;
                using sdata = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Data>,0xD>;
                using sstatus = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Flag,SSTATUSMasks>,0xB>;
                ctrla::type Ctrla;
                bridgectrl::type Bridgectrl;
                dbgctrl::type Dbgctrl;
                mctrla::type Mctrla;
                mctrlb::type Mctrlb;
                mstatus::type Mstatus;
                mbaud::type Mbaud;
                maddr::type Maddr;
                mdata::type Mdata;
                sctrla::type Sctrla;
                sctrlb::type Sctrlb;
                sstatus::type Sstatus;
                saddr::type Saddr;
                sdata::type Sdata;
                saddrmask::type Saddrmask;
            };

        };

        struct twis {
            template<auto N ,bool dummy = true>
            struct inst;

            template<bool dummy>
            struct inst<0, dummy>
            {
                template<auto N ,bool dummy1 = true>
                struct alt;

                [[nodiscard,gnu::always_inline]] static inline auto& value()  { return TWI0;}

                template<bool dummy1>
                struct alt<2, dummy1>
                {
                    struct Scl {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::portf>,3>;
                    };

                    struct Sda {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::portf>,2>;
                    };

                    using list = Meta::List<typename Scl::pin0, typename Sda::pin0>;
                };

                template<bool dummy1>
                struct alt<1, dummy1>
                {
                    struct Scl {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::portc>,3>;
                    };

                    struct Sda {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::portc>,2>;
                    };

                    using list = Meta::List<typename Scl::pin0, typename Sda::pin0>;
                };

                template<bool dummy1>
                struct alt<0, dummy1>
                {
                    struct Scl {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::porta>,3>;
                    };

                    struct Sda {
                        using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::porta>,2>;
                    };

                    using list = Meta::List<typename Scl::pin0, typename Sda::pin0>;
                };

            };

        };


		enum class SDASetup: mem_width {
			SDASetup_off = static_cast<mem_width>(twiComponent::CTRLAMasks::Sdahold_off),
			SDASetup_50ns = static_cast<mem_width>(twiComponent::CTRLAMasks::Sdahold_50ns) ,
			SDASetup_300ns = static_cast<mem_width>(twiComponent::CTRLAMasks::Sdahold_300ns) ,
			SDASetup_500ns = static_cast<mem_width>(twiComponent::CTRLAMasks::Sdahold_500ns) ,
		};
		
		enum class SDAHold: mem_width {
			Setup4Cycles = static_cast<mem_width>(twiComponent::CTRLAMasks::Sdasetup_4cyc),
			Setup8Cycles = static_cast<mem_width>(twiComponent::CTRLAMasks::Sdasetup_8cyc)
		};

		enum class MasterTimeout: mem_width {
			Disabled = static_cast<mem_width>(twiComponent::MCTRLAMasks::Timeout_disabled),
			T_50us = static_cast<mem_width>(twiComponent::MCTRLAMasks::Timeout_50us),
			T_100us = static_cast<mem_width>(twiComponent::MCTRLAMasks::Timeout_100us),
			T_200us = static_cast<mem_width>(twiComponent::MCTRLAMasks::Timeout_200us)
		};
	}
}