/*
 * Port.h
 *
 * Created: 29.01.2019 11:45:45
 *  Author: Keven
 */ 

#pragma once
#include "Register.hpp"
#include "Eventsystem.hpp"
#include "utils.h"

namespace port {

	namespace {
			
		template<ptr_t baseAddress,auto offset>
		struct Port {
			Port() = delete;
			Port(const Port&) = delete;
			Port(Port&&) = delete;

			static inline auto& port = *((PORT_t*)(baseAddress + static_cast<mem_width>(offset)));

			public:

			[[nodiscard]] static inline auto& getDir() {
				return Register<mem_width>::getRegister(port.DIR);
			};

			[[nodiscard]] static inline auto& getIn() {
				return Register<mem_width, ReadOnly>::getRegister(port.IN);
			};

			[[nodiscard]] static inline auto& getOut() {
				return Register<mem_width>::getRegister(port.OUT);
			};
				
		}__attribute__((packed));
	}
}
	
namespace {
	enum PortRegisters : mem_width {
		DIR = 0,  /* Data Direction */
		DIRSET = 1,  /* Data Direction Set */
		DIRCLR = 2,  /* Data Direction Clear */
		DIRTGL = 3,  /* Data Direction Toggle */
		OUT = 4,  /* Output Value */
		OUTSET = 5,  /* Output Value Set */
		OUTCLR = 6,  /* Output Value Clear */
		OUTTGL = 7,  /* Output Value Toggle */
		IN = 8,  /* Input Value */
		INTFLAGS = 9,  /* Interrupt Flags */
		PORTCTRL = 10,  /* Port Control */
		PIN0CTRL = 16,  /* Pin 0 Control */
		PIN1CTRL = 17,  /* Pin 1 Control */
		PIN2CTRL = 18,  /* Pin 2 Control */
		PIN3CTRL = 19,  /* Pin 3 Control */
		PIN4CTRL = 20,  /* Pin 4 Control */
		PIN5CTRL = 21,  /* Pin 5 Control */
		PIN6CTRL = 22,  /* Pin 6 Control */
		PIN7CTRL = 23  /* Pin 7 Control */
	};
}