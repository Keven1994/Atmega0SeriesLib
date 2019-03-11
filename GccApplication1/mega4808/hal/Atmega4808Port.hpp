
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
	
	struct portComponent {
		
		enum class intFlagMasks : mem_width { pinInterrupt = 0xff};
		enum class portCtrlMasks : mem_width { slewRateEnable = 0x1};
		enum class pinMasks : mem_width {
			InvertedIOEnable = PORT_INVEN_bm,
			InputSenseConfiguration = PORT_ISC_gm,
			ISCFallingEdge = PORT_ISC_enum::PORT_ISC_FALLING_gc,
			ISCRisingEdge = PORT_ISC_enum::PORT_ISC_RISING_gc,
			ISCBothEdges = PORT_ISC_enum::PORT_ISC_BOTHEDGES_gc,
			ISCInputDisable = PORT_ISC_enum::PORT_ISC_INPUT_DISABLE_gc,
			ISCInterruptDisable = PORT_ISC_enum::PORT_ISC_INTDISABLE_gc,
			Pullupenable = PORT_PULLUPEN_bm
		};
		
		struct registers{
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
			using intflags = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,intFlagMasks>,0x09> ;
			using portctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control, portCtrlMasks>,0x0A>;
		
			template<auto num>
			requires(num < 8 && num >= 0)
			using pinctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control, pinMasks>,0x10+num>;
		};
	};
	
	using PortRegisters = typename portComponent::registers;
	
	#define pp(number) using pin ## number = port::PortPin<P,number>
	
	struct ports{
		NoConstructors(ports);
		struct A{
			NoConstructors(A);
			[[gnu::always_inline]] static inline auto& value() { return PORTA; }
			struct pins {
				static inline constexpr Pin pin0{0}, pin1{1}, pin2{2}, pin3{3}, pin4{4}, pin5{5},pin6{6}, pin7{7};
			};
			template<typename P>
			struct portPins{
				pp(0); pp(1); pp(2); pp(3); pp(4); pp(5); pp(6); pp(7);
			};
		};
		
		struct C{
			NoConstructors(C);
			[[gnu::always_inline]] static inline auto& value() { return PORTC; }
			struct pins {
				static inline constexpr Pin pin0{0}, pin1{1}, pin2{2}, pin3{3};
			};
			template<typename P>
			struct portPins{
				pp(0); pp(1); pp(2); pp(3);
			};
		};
		
		struct D{
			NoConstructors(D);
			[[gnu::always_inline]] static inline auto& value() { return PORTD; }
			struct pins {
				static inline constexpr Pin pin0{0}, pin1{1}, pin2{2}, pin3{3}, pin4{4}, pin5{5},pin6{6}, pin7{7};
			};
			template<typename P>
			struct portPins{
				pp(0); pp(1); pp(2); pp(3); pp(4); pp(5); pp(6); pp(7);
			};
		};
		
		struct F{
			NoConstructors(F);
			[[gnu::always_inline]] static inline auto& value() { return PORTF; }
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
	
	using namespace port;
	template<typename P>
	using port = port::Port<P,portComponent>;
}