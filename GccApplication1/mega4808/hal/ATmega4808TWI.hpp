#pragma once
#include "../../hw_abstractions/Port.hpp"
#include <util/twi.h>

namespace mega4808 {
	struct twiComponent {
		enum class BRIDGECTRLMasks : mem_width {
			Default_enable = TWI_ENABLE_bm,
			Default_fmpen = TWI_FMPEN_bm
		};
		
		enum class CTRLAMasks : mem_width {
			Default_fmpen = TWI_FMPEN_bm
		};
		
		enum class DBGCTRLMasks : mem_width {
			Default_dbgrun = TWI_DBGRUN_bm
		};
		
		enum class MCTRLAMasks : mem_width {
			Enable = TWI_ENABLE_bm,
			Qcen = TWI_QCEN_bm,
			Rien = TWI_RIEN_bm,
			Smen = TWI_SMEN_bm,
			Wien = TWI_WIEN_bm,
			timeout_disabled = TWI_TIMEOUT_enum::TWI_TIMEOUT_DISABLED_gc,
			timeout_50us = TWI_TIMEOUT_enum::TWI_TIMEOUT_50US_gc,
			timeout_100us = TWI_TIMEOUT_enum::TWI_TIMEOUT_100US_gc,
			timeout_200us = TWI_TIMEOUT_enum::TWI_TIMEOUT_200US_gc
		};
		
		enum class MCTRLBMasks : mem_width {
			Flush = TWI_FLUSH_bm,
			ackact_ack = TWI_ACKACT_enum::TWI_ACKACT_ACK_gc,
			ackact_nack = TWI_ACKACT_enum::TWI_ACKACT_NACK_gc,
			mcmd_noact = TWI_MCMD_enum::TWI_MCMD_NOACT_gc,
			mcmd_repstart = TWI_MCMD_enum::TWI_MCMD_REPSTART_gc,
			mcmd_recvtrans = TWI_MCMD_enum::TWI_MCMD_RECVTRANS_gc,
			mcmd_stop = TWI_MCMD_enum::TWI_MCMD_STOP_gc
		};
		
		enum class MSTATUSMasks : mem_width {
			Arblost = TWI_ARBLOST_bm,
			Buserr = TWI_BUSERR_bm,
			Clkhold = TWI_CLKHOLD_bm,
			Rif = TWI_RIF_bm,
			Rxack = TWI_RXACK_bm,
			Wif = TWI_WIF_bm,
			busstate_unknown = TWI_BUSSTATE_enum::TWI_BUSSTATE_UNKNOWN_gc,
			busstate_idle = TWI_BUSSTATE_enum::TWI_BUSSTATE_IDLE_gc,
			busstate_owner = TWI_BUSSTATE_enum::TWI_BUSSTATE_OWNER_gc,
			busstate_busy = TWI_BUSSTATE_enum::TWI_BUSSTATE_BUSY_gc
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
			ackact_ack = TWI_ACKACT_enum::TWI_ACKACT_ACK_gc,
			ackact_nack = TWI_ACKACT_enum::TWI_ACKACT_NACK_gc,
			scmd_noact = TWI_SCMD_enum::TWI_SCMD_NOACT_gc,
			scmd_comptrans = TWI_SCMD_enum::TWI_SCMD_COMPTRANS_gc,
			scmd_response = TWI_SCMD_enum::TWI_SCMD_RESPONSE_gc
		};
		
		enum class SSTATUSMasks : mem_width {
			Apif = TWI_APIF_bm,
			Buserr = TWI_BUSERR_bm,
			Clkhold = TWI_CLKHOLD_bm,
			Coll = TWI_COLL_bm,
			Dif = TWI_DIF_bm,
			Dir = TWI_DIR_bm,
			Rxack = TWI_RXACK_bm,
			ap_stop = TWI_AP_enum::TWI_AP_STOP_gc,
			ap_adr = TWI_AP_enum::TWI_AP_ADR_gc
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
		};
		
	};

	struct twis {
		struct twi0 {
			[[nodiscard,gnu::always_inline]] static inline auto& value()  { return TWI0;}
			
			struct I2c_alt2 {
				struct Scl {
					using pin0 = AVR::port::details::PortPin<port<ports::portf>,3>;
				};
				
				struct Sda {
					using pin0 = AVR::port::details::PortPin<port<ports::portf>,2>;
				};
				
			};
			
			struct I2c_alt {
				struct Scl {
					using pin0 = AVR::port::details::PortPin<port<ports::portc>,3>;
				};
				
				struct Sda {
					using pin0 = AVR::port::details::PortPin<port<ports::portc>,2>;
				};
				
			};
			
			struct I2c {
				struct Scl {
					using pin0 = AVR::port::details::PortPin<port<ports::porta>,3>;
				};
				
				struct Sda {
					using pin0 = AVR::port::details::PortPin<port<ports::porta>,2>;
				};
				
			};
			
		};
		
	};

}