
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
		};
	};
}