
/*
* Atmega4808.hpp
*
* Created: 26.02.2019 22:15:24
*  Author: keven
*/

#pragma once

//type informations for the templates in the hw includes
using mem_width = uint8_t;
using ptr_t = uintptr_t;

//hw includes
#include "hal/Atmega4808Port.hpp"
#include "hal/Atmega4808EVSYS.hpp"
#include "hal/Atmega4808SPI.hpp"
#include "hal/ATmega4808TWI.hpp"


namespace mega4808 {
	

	template<typename frequenzy>
	#ifndef Intelli
	requires(utils::isEqual<MHZ4,frequenzy>::value || utils::isEqual<MHZ12,frequenzy>::value || utils::isEqual<MHZ20,frequenzy>::value)
	#endif
	struct Atmega4808 {
		
		static constexpr auto clockFrequenzy = frequenzy::value;
		
		NoConstructors(Atmega4808);
		
		using port_registers = typename mega4808::portComponent::registers;
		
		template<typename p>
		struct Ports {

			using portX = typename utils::condEqual<AVR::port::A,p, port<ports::porta>,
			typename utils::condEqual<AVR::port::C,p, port<ports::portc>,
			typename utils::condEqual<AVR::port::D,p, port<ports::portd>,
			typename utils::condEqual<AVR::port::F,p, port<ports::portf>,void
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
		
		struct SPI {
			using TransferMode = mega4808::TransferMode;
			using Prescaler = mega4808::Prescaler;
			using Components = mega4808::spis;
			using Component = mega4808::spiComponent;

			template<bool msb,  bool clockDoubled, bool slaveSelectDisable,TransferMode transferMode, bool buffered,
			bool waitForReceive, Prescaler prescaler>
			requires(!(!buffered && waitForReceive))
			struct SPIMasterSetting{

				using AConf = spiComponent::CTRLAMasks;
				using BConf = spiComponent::CTRLBMasks;
				
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

				using AConf = spiComponent::CTRLAMasks;
				using BConf = spiComponent::CTRLBMasks;
				
				static constexpr AConf Msb = msb ? static_cast<AConf>(0) : AConf::Dord;
				static constexpr BConf tmode = static_cast<BConf>(transferMode);
				static constexpr BConf buf = buffered ? BConf::Bufen : static_cast<BConf>(0);
				static constexpr BConf bufwait = buffered ? BConf::Bufwr : static_cast<BConf>(0);
			};
		};
	};
}