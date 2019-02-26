#pragma once
#include "../../hw_abstractions/Port.hpp"

namespace mega4809 {
	
	namespace {
		static inline constexpr ptr_t PortBaseAddress = 0x400;
		
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
		
		struct ports{
			struct A{
				static inline auto& value = PORTA;	
			};
			
			struct B{
				static inline auto& value = PORTB;
			};
			
			struct C{
				static inline auto& value = PORTC;
			};
			
			struct D{
				static inline auto& value = PORTD;
			};
			
			struct E{
				static inline auto& value = PORTE;
			};
			
			struct F{
				static inline auto& value = PORTF;
			};
																
		};
		
		template<typename P>
		using port = port::Port<P,PortRegisters>;
	}
}