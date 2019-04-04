
/*
* Atmega4808.hpp
*
* Created: 26.02.2019 22:15:24
*  Author: keven
*/

#pragma once
#include <avr/io.h>
//type informations for the templates in the hw includes
using mem_width = uint8_t;
using ptr_t = uintptr_t;

//hw includes
#include "hal/Atmega4808Port.hpp"
#include "hal/Atmega4808EVSYS.hpp"
#include "hal/Atmega4808SPI.hpp"
#include "hal/ATmega4808TWI.hpp"
#include "hal/ATmega4808CPU.hpp"
#include "hw_abstractions/CPU.hpp"
#include "../hw_abstractions/RessourceController.hpp"

namespace mega4808 {
	

	template<typename frequenzy>
	requires(utils::isEqual<MHZ4,frequenzy>::value || utils::isEqual<MHZ12,frequenzy>::value || utils::isEqual<MHZ20,frequenzy>::value)
	class Atmega4808 {

		template<typename Alias>
		friend struct AVR::rc::details::resolveComponent;
		
		template<typename MCU,typename FIRST,typename... PINS>
		friend class AVR::rc::ResController;
		
		public:

	    struct isZero{};

		static constexpr auto clockFrequenzy = frequenzy::value;
		
		NoConstructors(Atmega4808);
		
		template<typename T>
		static inline constexpr bool is_atomic(){
		    if constexpr (std::is_same<T, uint8_t>::value) return true;
		    else return false;
		}

		template<typename p>
		struct Ports {

			using portX = typename utils::condEqual<AVR::port::A,p, port_details::port<port_details::ports::porta>,
			typename utils::condEqual<AVR::port::C,p, port_details::port<port_details::ports::portc>,
			typename utils::condEqual<AVR::port::D,p, port_details::port<port_details::ports::portd>,
			typename utils::condEqual<AVR::port::F,p, port_details::port<port_details::ports::portf>,void
			>::type
			>::type
			>::type
			>::type;
			
			static_assert(! utils::isEqual<portX,void>::value,"port not available");
		};
		
		struct EventSystem {

			template<mem_width number>
			using generators = mega4808::details::generatorChannel<number>;
			
			using users = mega4808::details::users;
		};

		struct SPI : public AVR::rc::details::RCComponent<spi_details::spis> {
				
			using TransferMode = mega4808::spi_details::TransferMode;
			using Prescaler = mega4808::spi_details::Prescaler;
			using Component_t = mega4808::spi_details::spiComponent;

			template<bool msb,  bool clockDoubled, bool slaveSelectDisable,TransferMode transferMode, bool buffered,
			bool waitForReceive, Prescaler prescaler>
			requires(!(!buffered && waitForReceive))
			struct SPIMasterSetting{

				using AConf = spi_details::spiComponent::CTRLAMasks;
				using BConf = spi_details::spiComponent::CTRLBMasks;
				
				static constexpr AConf Msb = msb ? static_cast<AConf>(0) : AConf::Dord;
				static constexpr AConf clkx2 = clockDoubled ? AConf::Clk2x :  static_cast<AConf>(0);
				static constexpr AConf presc = static_cast<AConf>(prescaler);
				static constexpr BConf ssd = slaveSelectDisable ? BConf::Ssd :  static_cast<BConf>(0);
				static constexpr BConf tmode = static_cast<BConf>(transferMode);
				static constexpr BConf buf = buffered ? BConf::Bufen : static_cast<BConf>(0);
				static constexpr BConf bufwait = buffered ? BConf::Bufwr : static_cast<BConf>(0);
			};

			template<bool msb,TransferMode transferMode, bool buffered, bool waitForReceive>
			requires(!(!buffered && waitForReceive))
			struct SPISlaveSetting{

				using AConf = spi_details::spiComponent::CTRLAMasks;
				using BConf = spi_details::spiComponent::CTRLBMasks;
				
				static constexpr AConf Msb = msb ? static_cast<AConf>(0) : AConf::Dord;
				static constexpr BConf tmode = static_cast<BConf>(transferMode);
				static constexpr BConf buf = buffered ? BConf::Bufen : static_cast<BConf>(0);
				static constexpr BConf bufwait = buffered ? BConf::Bufwr : static_cast<BConf>(0);
			};
		};
		
		struct TWI : public AVR::rc::details::RCComponent<twi_details::twis>  {
			
			using SDAHold = twi_details::SDAHold;
			using SDASetup = twi_details::SDASetup;
			using MasterTimeout = twi_details::MasterTimeout;
            using Component_t = mega4808::twi_details::twiComponent;

			template<bool fastModePlus, SDAHold holdTime, SDASetup sdaSetup, bool quickCommand, bool smartMode, MasterTimeout timeOut, mem_width baudRate>
			struct TWIMasterSetting{

				using AConf = twi_details::twiComponent::CTRLAMasks;
				using MAConf = twi_details::twiComponent::MCTRLAMasks;
				
				static constexpr AConf fastmode = fastModePlus ? AConf::Fmpen : static_cast<AConf>(0);
				static constexpr AConf holdtime = static_cast<AConf>(holdTime);
				static constexpr AConf setuptime = static_cast<AConf>(sdaSetup);
				static constexpr MAConf quickcommand = quickCommand ? MAConf::Qcen : static_cast<MAConf>(0);
				static constexpr MAConf smartmode = smartMode ? MAConf::Smen : static_cast<MAConf>(0);
				static constexpr MAConf timeout = static_cast<MAConf>(timeOut);
				static constexpr mem_width baud = baudRate;
			};

			template<bool fastModePlus, SDAHold holdTime, SDASetup sdaSetup>
			struct TWISlaveSetting{

				using AConf = twi_details::twiComponent::CTRLAMasks;
				using SAConf = twi_details::twiComponent::SCTRLAMasks;
				using SBConf = twi_details::twiComponent::SCTRLBMasks;
				
				static constexpr AConf fastmode = fastModePlus ? AConf::Fmpen : static_cast<AConf>(0);
				static constexpr AConf holdtime = static_cast<AConf>(holdTime);
				static constexpr AConf setuptime = static_cast<AConf>(sdaSetup);
			};
		};

		class Status {

		    public:
            using Component_t = cpu_details::cpuComponent;
		};
		
	};
}