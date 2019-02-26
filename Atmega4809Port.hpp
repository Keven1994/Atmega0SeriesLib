#pragma once
	
namespace mega4809 {
			constexpr ptr_t PortBaseAddress = 0x400;
	namespace {
		// can be used to get e specific port register
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
						
		//offsets to the base
		enum ports : mem_width {
			A = 0,
			B = 1*sizeof(PORT_t),
			C = 2*sizeof(PORT_t),
			D = 3*sizeof(PORT_t),
			E = 4*sizeof(PORT_t),
			F = 5*sizeof(PORT_t)
		};
	}
}