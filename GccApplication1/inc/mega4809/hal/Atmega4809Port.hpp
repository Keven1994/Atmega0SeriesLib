#pragma once
#include "../../hw_abstractions/Port.hpp"
#include "../../hw_abstractions/Pin.hpp"

namespace mega4809 {
	
	namespace {
		struct portpairs {
					
			enum intFlagMasks : mem_width { pinInterrupt = 0xff};
			enum portCtrlMasks : mem_width { slewRateEnable = 0x1};
			enum pinMasks : mem_width {
				InvertedIOEnable = PORT_INVEN_bm,
				InputSenseConfiguration = PORT_ISC_gm,
				ISCFallingEdge = PORT_ISC_enum::PORT_ISC_FALLING_gc,
				ISCRisingEdge = PORT_ISC_enum::PORT_ISC_RISING_gc,
				ISCBothEdges = PORT_ISC_enum::PORT_ISC_BOTHEDGES_gc,
				ISCInputDisable = PORT_ISC_enum::PORT_ISC_INPUT_DISABLE_gc,
				ISCInterruptDisable = PORT_ISC_enum::PORT_ISC_INTDISABLE_gc,
				Pullupenable = PORT_PULLUPEN_bm
			};
					
			using dir = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Data>,0x00>;
			using dirset = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Data>,0x01>;
			using dirclear = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Data>,0x02> ;
			using dirtoggle = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Toggle>,0x03>;
			using out = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Data>,0x04>;
			using outset = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Data>,0x05>;
			using outclear = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Data>,0x06>;
			using outtoggle = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Toggle>,0x07>;
			//Input port declared as RW ?
			using in = utils::Pair<reg::Register<reg::accessmode::ReadOnly,reg::specialization::Data>,0x08>;
			using intflags = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control, intFlagMasks>,0x09> ;
			using portctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control, portCtrlMasks>,0x0A>;
					
			template<auto num>
			requires(num < 8 && num >= 0)
			using pinctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control, pinMasks>,0x10+num>;
		};
		
		//legacy
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
				[[gnu::always_inline]] static inline auto& value(){ return PORTA;}
				struct pins {
					static inline constexpr Pin pin0{0}, pin1{1}, pin2{2}, pin3{3}, pin4{4}, pin5{5},pin6{6}, pin7{7};
				};
				template<typename P>
				struct portPins{
					pp(0); pp(1); pp(2); pp(3); pp(4); pp(5); pp(6); pp(7);
				};
			};
			
			struct B{
				[[gnu::always_inline]] static inline auto& value(){ return  PORTB; }
				struct pins {
					static inline constexpr Pin pin0{0}, pin1{1}, pin2{2}, pin3{3}, pin4{4}, pin5{5},pin6{6}, pin7{7};
				};
				template<typename P>
				struct portPins{
					pp(0); pp(1); pp(2); pp(3); pp(4); pp(5); pp(6); pp(7);
				};
			};
			
			struct C{
				[[gnu::always_inline]] static inline auto& value(){ return PORTC; }
				struct pins {
					static inline constexpr Pin pin0{0}, pin1{1}, pin2{2}, pin3{3}, pin4{4}, pin5{5},pin6{6}, pin7{7};
				};
				template<typename P>
				struct portPins{
					pp(0); pp(1); pp(2); pp(3); pp(4); pp(5); pp(6); pp(7);
				};
			};
			
			struct D{
				[[gnu::always_inline]] static inline auto& value(){ return PORTD; }
				struct pins {
					static inline constexpr Pin pin0{0}, pin1{1}, pin2{2}, pin3{3}, pin4{4}, pin5{5},pin6{6}, pin7{7};
				};
				template<typename P>
				struct portPins{
					pp(0); pp(1); pp(2); pp(3); pp(4); pp(5); pp(6); pp(7);
				};
			};
			
			struct E{
				[[gnu::always_inline]] static inline auto& value(){ return PORTE; }
				struct pins {
					static inline constexpr Pin pin0{0}, pin1{1}, pin2{2}, pin3{3}, pin4{4}, pin5{5},pin6{6}, pin7{7};
				};
				template<typename P>
				struct portPins{
					pp(0); pp(1); pp(2); pp(3); pp(4); pp(5); pp(6); pp(7);
				};
			};
			
			struct F{
				[[gnu::always_inline]] static inline auto& value(){ return PORTF; }
				struct pins {
					static inline constexpr Pin pin0{0}, pin1{1}, pin2{2}, pin3{3}, pin4{4}, pin5{5},pin6{6}, pin7{7};
				};
				template<typename P>
				struct portPins{
					pp(0); pp(1); pp(2); pp(3); pp(4); pp(5); pp(6); pp(7);
				};
			};
			
			#undef pp
			
		};
		
		template<typename P>
		using port = port::Port<P,portpairs>;
	}
}