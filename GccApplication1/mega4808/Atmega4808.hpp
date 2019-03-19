
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
#include "hal/Atmega4808EVSYS.hpp"
#include "hal/Atmega4808Port.hpp"
#include "hal/Atmega4808SPI.hpp"


namespace mega4808 {
using namespace utils;
	struct Atmega4808 {
		
		static constexpr auto clockFrequenzy = 4000000UL;
		
		NoConstructors(Atmega4808);
		
		struct Ports {

			NoConstructors(Ports);
			
			using porta = port<ports::porta>;
			using portc = port<ports::portc>;
			using portd = port<ports::portd>;
			using portf = port<ports::portf>;
		};
		
		using port_registers = typename mega4808::portComponent::registers;
		
		template<typename p>
		struct testports {
			using portX = typename condEqual<AVR::port::A,p, port<ports::porta>, 
							typename condEqual<AVR::port::C,p, port<ports::portc>,
								typename condEqual<AVR::port::D,p, port<ports::portd>,
									typename condEqual<AVR::port::F,p, port<ports::portf>,void
									>::type
								>::type
							>::type
						 >::type; 	
						 
			static_assert(! isEqual<portX,void>::value,"port not available");
		};
		
		struct EventSystem {
			
			using ch0 = channel<0>;
			using ch1 = channel<1>;
			using ch2 = channel<2>;
			using ch3 = channel<3>;
			using ch4 = channel<4>;
			using ch5 = channel<5>;
		};
		
		struct SPI {
			using TransferMode = mega4808::TransferMode;
			using Prescaler = mega4808::Prescaler;
			//template<bool msb,  bool clockDoubled, bool slaveSelectDisable,TransferMode transferMode, bool buffered, bool waitForReceive, Prescaler prescaler>
			template<bool msb = true, bool clockDouble = true, bool slaveSelectDisable = true, TransferMode tmode = TransferMode::Mode0,  bool buffered = false,bool waitForReceive = false, Prescaler prescaler = Prescaler::Div4, uint8_t alternative = 0>
			using SpiMaster = typename spiMaster<msb,clockDouble,slaveSelectDisable,tmode,buffered,waitForReceive,prescaler,alternative>::SPI;
			
			template<bool msb = true, TransferMode tmode = TransferMode::Mode0,  bool buffered = false,bool waitForReceive = false, uint8_t alternative = 0>
			using SPISlave = typename spiSlave<msb,tmode,buffered,waitForReceive,alternative>::SPI;
		};
	};
}