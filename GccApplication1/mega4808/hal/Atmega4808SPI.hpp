
/*
* Atmega4808SPI.hpp
*
* Created: 28.02.2019 00:15:49
*  Author: keven
*/

#include "../../hw_abstractions/SPI.hpp"
#include "../../hw_abstractions/Register.hpp"
#include "../../tools/utils.h"
#include "Atmega4808Port.hpp"

namespace mega4808 {
	
	struct spiComponent{
		
		enum class ControlAConf : mem_width {
			None = 0,
			LsbFirst = SPI_DORD_bm,
			Master = SPI_MASTER_bm,
			ClockDoubled = SPI_CLK2X_bm,
			Div4 = SPI_PRESC_DIV4_gc,
			Div16 = SPI_PRESC_DIV16_gc,
			Div64 = SPI_PRESC_DIV64_gc,
			Div128 = SPI_PRESC_DIV128_gc,
			SPIEnable = SPI_ENABLE_bm
		};
		
		enum class ControlBConf : mem_width {
			None = 0,
			BufferModeEnable = SPI_BUFEN_bm,
			BufferModeWaitforReceive = SPI_BUFWR_bm,
			SlaveSelectDisable = SPI_SSD_bm,
			SPIMode0 = SPI_MODE_0_gc,
			SPIMode1 = SPI_MODE_1_gc,
			SPIMode2 = SPI_MODE_2_gc,
			SPIMode3 = SPI_MODE_3_gc
		};
		
		enum class InterruptFlagBits : mem_width {
			None = 0,
			ReceiveComplete = SPI_IF_bm,
			WriteCollision = SPI_WRCOL_bm,
			TransferComplete = SPI_TXCIF_bm,
			DataRegisterEmpty = SPI_DREIF_bm,
			SlaveSelectTrigger = SPI_SSIF_bm,
			BufferOverFlow = SPI_BUFOVF_bm
		};
		
		enum class InterruptControlConfig : mem_width {
			None = 0,
			SlaveSelectTriggerInput = SPI_SSIE_bm,
			DataRegisterEmptyTrigger = SPI_DREIE_bm,
			TransferCompleteTrigger = SPI_TXCIE_bm,
			ReceiveCompleteTrigger = SPI_TXCIE_bm
		};
		
		using ControlA = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,ControlAConf>,0x00>;
		using ControlB = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,ControlBConf>,0x01>;
		using InterruptControl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control>,0x02>;
		using InterruptFlags = utils::Pair<reg::Register<reg::accessmode::ReadOnly,reg::specialization::Flag,InterruptFlagBits>,0x03>;
		using Data = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Data>,0x04>;
	};
	
	enum class TransferMode : mem_width {
		//Leading edge: Rising, sample - Trailing edge: Falling, setup
		Mode0 = static_cast<mem_width>(spiComponent::ControlBConf::SPIMode0),
		//Leading edge: Rising, setup - Trailing edge: Falling, sample
		Mode1 = static_cast<mem_width>(spiComponent::ControlBConf::SPIMode1),
		//Leading edge: Falling, sample - Trailing edge: Rising, setup
		Mode2 = static_cast<mem_width>(spiComponent::ControlBConf::SPIMode2),
		//Leading edge: Falling, setup - Trailing edge: Rising, sample
		Mode3 = static_cast<mem_width>(spiComponent::ControlBConf::SPIMode3)
	};
	
	enum class Prescaler : mem_width {
		Div4 = static_cast<mem_width>(spiComponent::ControlAConf::Div4),
		Div16 = static_cast<mem_width>(spiComponent::ControlAConf::Div16),
		Div64 = static_cast<mem_width>(spiComponent::ControlAConf::Div64),
		Div128 = static_cast<mem_width>(spiComponent::ControlAConf::Div128)
	};
	
	struct SPIs {
		NoConstructors(SPIs);
		
		template<uint8_t alternative>
		struct spi0 {
			NoConstructors(spi0);
			
			template<auto n0,auto n1,auto n2,auto n3>
			struct _pins {
				static inline constexpr Pin mosi{n0}, miso{n1}, sck{n2}, ss{n3};
			};

			using mport = typename utils::conditional<alternative == 0, port<ports::A>, port<ports::C>>::type;
			
			using pins = typename utils::conditional<alternative == 0, _pins<4,5,6,7>, _pins<0,1,2,3>>::type;
			
			[[gnu::always_inline]] static inline auto& value() { return SPI0; }
			
		};
	};
	
	template<bool msb,  bool clockDoubled, bool slaveSelectDisable,TransferMode transferMode, bool buffered, bool waitForReceive, Prescaler prescaler, uint8_t alternative>
	struct spiMaster{
		static_assert(! (!buffered && waitForReceive), "unbuffered mode combined with waitforreceive does not make sense, check spi configuration");

		using AConf = spiComponent::ControlAConf;
		using BConf = spiComponent::ControlBConf;
		
		static constexpr AConf Msb = msb ? AConf::None : AConf::LsbFirst;
		static constexpr AConf clkx2 = clockDoubled ? AConf::ClockDoubled : AConf::None;
		static constexpr AConf presc = static_cast<AConf>(prescaler);
		static constexpr BConf ssd = slaveSelectDisable ? BConf::SlaveSelectDisable : BConf::None;
		static constexpr BConf tmode = static_cast<BConf>(transferMode);
		static constexpr BConf buf = buffered ? BConf::BufferModeEnable : BConf::None;
		static constexpr BConf bufwait = buffered ? BConf::BufferModeWaitforReceive : BConf::None;

		using SPI = spi::SPIMaster<spiComponent, typename SPIs::spi0<alternative>, Msb,clkx2,ssd,tmode,buf,bufwait,presc>;
	};

	template<bool msb,TransferMode transferMode, bool buffered, bool waitForReceive, uint8_t alternative>
	struct spiSlave{
		static_assert(! (!buffered && waitForReceive), "unbuffered mode combined with waitforreceive does not make sense, check spi configuration");

		using AConf = spiComponent::ControlAConf;
		using BConf = spiComponent::ControlBConf;
		
		static constexpr AConf Msb = msb ? AConf::None : AConf::LsbFirst;
		static constexpr BConf tmode = static_cast<BConf>(transferMode);
		static constexpr BConf buf = buffered ? BConf::BufferModeEnable : BConf::None;
		static constexpr BConf bufwait = buffered ? BConf::BufferModeWaitforReceive : BConf::None;
		
		using SPI = spi::SPISlave<spiComponent, typename SPIs::spi0<alternative>, Msb,tmode,buf,bufwait>;
	};

}
