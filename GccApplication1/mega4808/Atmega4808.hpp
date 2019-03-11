
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

	struct Atmega4808 {
		
		Atmega4808() = delete;
		Atmega4808(const Atmega4808&) = delete;
		Atmega4808(Atmega4808&&) = delete;
		
		struct Ports {

			Ports() = delete;
			Ports(const Ports&) = delete;
			Ports(Ports&&) = delete;
			
			using porta = port<ports::A>;
			using portc = port<ports::C>;
			using portd = port<ports::D>;
			using portf = port<ports::F>;
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