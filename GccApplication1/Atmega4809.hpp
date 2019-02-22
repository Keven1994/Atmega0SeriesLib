/*
 * Atmega4809.h
 *
 * Created: 29.01.2019 11:37:00
 *  Author: Keven
 */ 


#pragma once

#include "utils.h"


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsubobject-linkage"
namespace mega4809 {
	//type informations for the templates in the hw includes
	using mem_width = uint8_t;
	using reg_t = volatile Register<mem_width>; 
	using ptr_t = uintptr_t;
	//hw includes
	#include "Register.hpp"
	#include "Port.hpp"
	#include "Eventsystem.hpp"

	
	
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
	
	}__attribute__((packed));
}

#pragma GCC diagnostic pop
