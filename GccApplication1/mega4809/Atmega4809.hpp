/*
 * Atmega4809.h
 *
 * Created: 29.01.2019 11:37:00
 *  Author: Keven
 */ 


#pragma once

//type informations for the templates in the hw includes
using mem_width = uint8_t;
using ptr_t = uintptr_t;
//hw includes
#include "hal/Atmega4809EVSYS.hpp"
#include "hal/Atmega4809Port.hpp"
#include "hal/Atmega4809SPI.hpp"


namespace mega4809 {	
	struct Atmega4809 {
		
		struct Ports {

			Ports() = delete;
			Ports(const Ports&) = delete;
			Ports(Ports&&) = delete;
			
			using porta = port<ports::A>;
			using portb = port<ports::B>;
			using portc = port<ports::C>;
			using portd = port<ports::D>;
			using porte = port<ports::E>;
			using portf = port<ports::F>;
		};
	
		struct EventSystem {
			
			using ch0 = channel<0>;
			using ch1 = channel<1>;
			using ch2 = channel<2>;
			using ch3 = channel<3>;
			using ch4 = channel<4>;
			using ch5 = channel<5>;
			using ch6 = channel<6>;
			using ch7 = channel<7>;
		};
		
		struct SPI {
			using TransferMode = spi::TransferMode;
			using BufferMode = spi::BufferMode;
			using Prescaler = spi::Prescaler;
			
			template<bool msb = true, bool clockDouble = true, bool slaveSelectDisable = true, TransferMode tmode = TransferMode::Mode0, BufferMode bmode = BufferMode::unbuffered, Prescaler prescaler = Prescaler::Div4>
			using SpiMaster = typename spiMaster<msb,clockDouble, slaveSelectDisable,tmode,bmode,prescaler>::SPI;
			
			template<bool msb = true, bool clockDouble = true, bool slaveSelectDisable = true, TransferMode tmode = TransferMode::Mode0, BufferMode bmode = BufferMode::unbuffered, Prescaler prescaler = Prescaler::Div4>
			using SpiSlave = typename spiSlave<msb,tmode,bmode>::SPI;
		};
	};
}
