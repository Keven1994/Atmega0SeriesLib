
/*
* Atmega4808Port.hpp
*
* Created: 26.02.2019 22:16:26
*  Author: keven
*/

#pragma once
#include "../../hw_abstractions/Port.hpp"
#include "../../hw_abstractions/Pin.hpp"

namespace mega4808 {
	
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
		
		#define pp(number) using pin ## number = port::PortPin<P,number>
		
		struct ports{
			struct A{
				static inline auto& value = PORTA;
				struct pins {
					static inline constexpr Pin pin0{0}, pin1{1}, pin2{2}, pin3{3}, pin4{4}, pin5{5},pin6{6}, pin7{7};
				};
				template<typename P>
				struct portPins{
					pp(0); pp(1); pp(2); pp(3); pp(4); pp(5); pp(6); pp(7);
				};
			};
			
			struct C{
				static inline auto& value = PORTC;
				struct pins {
					static inline constexpr Pin pin0{0}, pin1{1}, pin2{2}, pin3{3};
				};
				template<typename P>
				struct portPins{
					pp(0); pp(1); pp(2); pp(3);
				};
			};
			
			struct D{
				static inline auto& value = PORTD;
				struct pins {
					static inline constexpr Pin pin0{0}, pin1{1}, pin2{2}, pin3{3}, pin4{4}, pin5{5},pin6{6}, pin7{7};
				};
				template<typename P>
				struct portPins{
					pp(0); pp(1); pp(2); pp(3); pp(4); pp(5); pp(6); pp(7);
				};
			};
			
			struct F{
				static inline auto& value = PORTF;
				struct pins {
					static inline constexpr Pin pin0{0}, pin1{1}, pin2{2}, pin3{3}, pin4{4}, pin5{5},pin6{6};
				};
				template<typename P>
				struct portPins{
					pp(0); pp(1); pp(2); pp(3); pp(4); pp(5); pp(6);
				};
			};
			
			#undef pp
		};
		
		template<typename P>
		using port = port::Port<P,PortRegisters>;
	}
}