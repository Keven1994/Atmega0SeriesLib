#pragma once
#include "../../hw_abstractions/Port.hpp"
#include "../../hw_abstractions/SPI.hpp"

namespace mega4808 {
struct spiComponent {
	enum class CTRLAMasks : mem_width {
		Clk2x = SPI_CLK2X_bm,
		Dord = SPI_DORD_bm,
		Enable = SPI_ENABLE_bm,
		Master = SPI_MASTER_bm,
		presc_div4 = SPI_PRESC_enum::SPI_PRESC_DIV4_gc,
		presc_div16 = SPI_PRESC_enum::SPI_PRESC_DIV16_gc,
		presc_div64 = SPI_PRESC_enum::SPI_PRESC_DIV64_gc,
		presc_div128 = SPI_PRESC_enum::SPI_PRESC_DIV128_gc
	};
	
	enum class CTRLBMasks : mem_width {
		Bufen = SPI_BUFEN_bm,
		Bufwr = SPI_BUFWR_bm,
		Ssd = SPI_SSD_bm,
		mode_0 = SPI_MODE_enum::SPI_MODE_0_gc,
		mode_1 = SPI_MODE_enum::SPI_MODE_1_gc,
		mode_2 = SPI_MODE_enum::SPI_MODE_2_gc,
		mode_3 = SPI_MODE_enum::SPI_MODE_3_gc
	};
	
	enum class INTCTRLMasks : mem_width {
		Dreie = SPI_DREIE_bm,
		Ie = SPI_IE_bm,
		Rxcie = SPI_RXCIE_bm,
		Ssie = SPI_SSIE_bm,
		Txcie = SPI_TXCIE_bm
	};
	
	enum class INTFLAGSMasks : mem_width {
		Buffered_bufovf = SPI_BUFOVF_bm,
		Buffered_dreif = SPI_DREIF_bm,
		Buffered_rxcif = SPI_RXCIF_bm,
		Buffered_ssif = SPI_SSIF_bm,
		Buffered_txcif = SPI_TXCIF_bm,
		Default_if = SPI_IF_bm,
		Default_wrcol = SPI_WRCOL_bm
	};
	
	struct registers {
		using ctrla = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,CTRLAMasks>,0x0>;
		using ctrlb = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,CTRLBMasks>,0x1>;
		using data = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Data>,0x4>;
		using intctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,INTCTRLMasks>,0x2>;
		using intflags = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,INTFLAGSMasks>,0x3>;
	};
	
};

struct spis {
	struct spi0 {
		[[nodiscard,gnu::always_inline]] static inline auto& value()  { return SPI0;}
		
		struct Spi_alt2 {
			struct Miso {
				//using pin0 = AVR::port::Pin<AVR::port::Port<AVR::port::E>,1>; ? -> not available
			};
			
			struct Mosi {
				//using pin0 = AVR::port::Pin<AVR::port::Port<AVR::port::E>,0>;
			};
			
			struct Sck {
				//using pin0 = AVR::port::Pin<AVR::port::Port<AVR::port::E>,2>;
			};
			
			struct Ss {
				//using pin0 = AVR::port::Pin<AVR::port::Port<AVR::port::E>,3>;
			};
			
		};
            struct Spi_alt {
	            struct Miso {
		            using pin0 = AVR::port::details::PortPin<port<ports::portc>,1>;
	            };
	            
	            struct Mosi {
		            using pin0 = AVR::port::details::PortPin<port<ports::portc>,0>;
	            };
	            
	            struct Sck {
		            using pin0 = AVR::port::details::PortPin<port<ports::portc>,2>;
	            };
	            
	            struct Ss {
		            using pin0 = AVR::port::details::PortPin<port<ports::portc>,3>;
	            };
	            
            };
            
            struct Spi {
	            struct Miso {
		            using pin0 = AVR::port::details::PortPin<port<ports::porta>,5>;
	            };
	            
	            struct Mosi {
		            using pin0 = AVR::port::details::PortPin<port<ports::porta>,4>;
	            };
	            
	            struct Sck {
		            using pin0 = AVR::port::details::PortPin<port<ports::porta>,6>;
	            };
	            
	            struct Ss {
		            using pin0 = AVR::port::details::PortPin<port<ports::porta>,7>;
	            };
	            
            };
		
	};
	
};	
			
			enum class TransferMode : mem_width {
				//Leading edge: Rising, sample - Trailing edge: Falling, setup
				Mode0 = static_cast<mem_width>(spiComponent::CTRLBMasks::mode_0),
				//Leading edge: Rising, setup - Trailing edge: Falling, sample
				Mode1 = static_cast<mem_width>(spiComponent::CTRLBMasks::mode_1),
				//Leading edge: Falling, sample - Trailing edge: Rising, setup
				Mode2 = static_cast<mem_width>(spiComponent::CTRLBMasks::mode_2),
				//Leading edge: Falling, setup - Trailing edge: Rising, sample
				Mode3 = static_cast<mem_width>(spiComponent::CTRLBMasks::mode_3)
			};
			
			enum class Prescaler : mem_width {
				Div4 = static_cast<mem_width>(spiComponent::CTRLAMasks::presc_div4),
				Div16 = static_cast<mem_width>(spiComponent::CTRLAMasks::presc_div16),
				Div64 = static_cast<mem_width>(spiComponent::CTRLAMasks::presc_div64),
				Div128 = static_cast<mem_width>(spiComponent::CTRLAMasks::presc_div128)
			};
			
			template<bool msb,  bool clockDoubled, bool slaveSelectDisable,TransferMode transferMode, bool buffered, bool waitForReceive, Prescaler prescaler, uint8_t alternative>
			struct spiMaster{
				static_assert(! (!buffered && waitForReceive), "unbuffered mode combined with waitforreceive does not make sense, check spi configuration");

				using AConf = spiComponent::CTRLAMasks;
				using BConf = spiComponent::CTRLBMasks;
				
				static constexpr AConf Msb = msb ? static_cast<AConf>(0) : AConf::Dord;
				static constexpr AConf clkx2 = clockDoubled ? AConf::Clk2x :  static_cast<AConf>(0);
				static constexpr AConf presc = static_cast<AConf>(prescaler);
				static constexpr BConf ssd = slaveSelectDisable ? BConf::Ssd :  static_cast<BConf>(0);
				static constexpr BConf tmode = static_cast<BConf>(transferMode);
				static constexpr BConf buf = buffered ? BConf::Bufen : static_cast<BConf>(0);
				static constexpr BConf bufwait = buffered ? BConf::Bufwr : static_cast<BConf>(0);

				using SPI = spi::SPIMaster<spiComponent, typename spis::spi0, Msb,clkx2,ssd,tmode,buf,bufwait,presc>;
			};

			template<bool msb,TransferMode transferMode, bool buffered, bool waitForReceive, uint8_t alternative>
			struct spiSlave{
				static_assert(! (!buffered && waitForReceive), "unbuffered mode combined with waitforreceive does not make sense, check spi configuration");

				using AConf = spiComponent::CTRLAMasks;
				using BConf = spiComponent::CTRLBMasks;
				
				static constexpr AConf Msb = msb ? static_cast<AConf>(0) : AConf::Dord;
				static constexpr BConf tmode = static_cast<BConf>(transferMode);
				static constexpr BConf buf = buffered ? BConf::Bufen : static_cast<BConf>(0);
				static constexpr BConf bufwait = buffered ? BConf::Bufwr : static_cast<BConf>(0);
				
				using SPI = spi::SPISlave<spiComponent, typename spis::spi0, Msb,tmode,buf,bufwait>;
			};
			
}