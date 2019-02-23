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
#include "Register.hpp"
#include "Port.hpp"
#include "Eventsystem.hpp"
#include "Atmega4809EVSYS.hpp"
#include "utils.h"


namespace mega4809 {	
	struct Atmega4809 {
		
		class Ports {
			
			enum ports : mem_width {
				A = 0,
				B = 1*sizeof(PORT_t),
				C = 2*sizeof(PORT_t),
				D = 3*sizeof(PORT_t),
				E = 4*sizeof(PORT_t),
				F = 5*sizeof(PORT_t)
			};
			
			public:
			Ports() = delete;
			
			template<mem_width offset>
			using port = port::Port<0x0400,offset>;
			
			using porta = port<ports::A>;
			using portb = port<ports::B>;
			using portc = port<ports::C>;
			using portd = port<ports::D>;
			using porte = port<ports::E>;
			using portf = port<ports::F>;
		};
	
		struct EventSystem {
			
			template<mem_width number>
			using channel = eventsystem::Channel<0,mega4809::generatorChannel<number>,mega4809::users>;
			
			using ch0 = channel<0>;
			using ch1 = channel<1>;
			using ch2 = channel<2>;
			using ch3 = channel<3>;
			using ch4 = channel<4>;
			using ch5 = channel<5>;
			using ch6 = channel<6>;
			using ch7 = channel<7>;
		};
	}__attribute__((packed));
}
