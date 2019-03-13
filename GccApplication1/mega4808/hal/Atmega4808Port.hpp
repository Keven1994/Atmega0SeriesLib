
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
			using in = utils::Pair<reg::Register<reg::accessmode::ReadOnly,reg::specialization::Data>,0x08>;
			using intflags = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,intFlagMasks>,0x09> ;
			using portctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control, portCtrlMasks>,0x0A>;
			
		
			template<auto num>
			requires(num < 8 && num >= 0)
			using pinctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control, pinMasks>,0x10+num>;
		};
	};
	
	#define pp(number) using pin ## number = AVR::port::details::PortPin<P,number>
	
	struct ports{
		NoConstructors(ports);
		struct A{
			NoConstructors(A);
			[[gnu::always_inline,nodiscard]] static inline auto& value() { return PORTA; }
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
			[[gnu::always_inline,nodiscard]] static inline auto& value() { return PORTC; }
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
			[[gnu::always_inline,nodiscard]] static inline auto& value() { return PORTD; }
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
			[[gnu::always_inline,nodiscard]] static inline auto& value() { return PORTF; }
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
	using port = AVR::port::details::Port<P,portComponent>;
}

struct portComponent {
	enum class INTFLAGSMasks : mem_width {
		Int = PORT_INT0_bm
	};
	
	enum class PIN0CTRLMasks : mem_width {
		Inven = PORT_INVEN_bm,
		Pullupen = PORT_PULLUPEN_bm,
		isc_intdisable = PORT_ISC_enum::PORT_ISC_INTDISABLE_gc,
		isc_bothedges = PORT_ISC_enum::PORT_ISC_BOTHEDGES_gc,
		isc_rising = PORT_ISC_enum::PORT_ISC_RISING_gc,
		isc_falling = PORT_ISC_enum::PORT_ISC_FALLING_gc,
		isc_input_disable = PORT_ISC_enum::PORT_ISC_INPUT_DISABLE_gc,
		isc_level = PORT_ISC_enum::PORT_ISC_LEVEL_gc
	};
	
	enum class PIN1CTRLMasks : mem_width {
		Inven = PORT_INVEN_bm,
		Pullupen = PORT_PULLUPEN_bm,
		isc_intdisable = PORT_ISC_enum::PORT_ISC_INTDISABLE_gc,
		isc_bothedges = PORT_ISC_enum::PORT_ISC_BOTHEDGES_gc,
		isc_rising = PORT_ISC_enum::PORT_ISC_RISING_gc,
		isc_falling = PORT_ISC_enum::PORT_ISC_FALLING_gc,
		isc_input_disable = PORT_ISC_enum::PORT_ISC_INPUT_DISABLE_gc,
		isc_level = PORT_ISC_enum::PORT_ISC_LEVEL_gc
	};
	
	enum class PIN2CTRLMasks : mem_width {
		Inven = PORT_INVEN_bm,
		Pullupen = PORT_PULLUPEN_bm,
		isc_intdisable = PORT_ISC_enum::PORT_ISC_INTDISABLE_gc,
		isc_bothedges = PORT_ISC_enum::PORT_ISC_BOTHEDGES_gc,
		isc_rising = PORT_ISC_enum::PORT_ISC_RISING_gc,
		isc_falling = PORT_ISC_enum::PORT_ISC_FALLING_gc,
		isc_input_disable = PORT_ISC_enum::PORT_ISC_INPUT_DISABLE_gc,
		isc_level = PORT_ISC_enum::PORT_ISC_LEVEL_gc
	};
	
	enum class PIN3CTRLMasks : mem_width {
		Inven = PORT_INVEN_bm,
		Pullupen = PORT_PULLUPEN_bm,
		isc_intdisable = PORT_ISC_enum::PORT_ISC_INTDISABLE_gc,
		isc_bothedges = PORT_ISC_enum::PORT_ISC_BOTHEDGES_gc,
		isc_rising = PORT_ISC_enum::PORT_ISC_RISING_gc,
		isc_falling = PORT_ISC_enum::PORT_ISC_FALLING_gc,
		isc_input_disable = PORT_ISC_enum::PORT_ISC_INPUT_DISABLE_gc,
		isc_level = PORT_ISC_enum::PORT_ISC_LEVEL_gc
	};
	
	enum class PIN4CTRLMasks : mem_width {
		Inven = PORT_INVEN_bm,
		Pullupen = PORT_PULLUPEN_bm,
		isc_intdisable = PORT_ISC_enum::PORT_ISC_INTDISABLE_gc,
		isc_bothedges = PORT_ISC_enum::PORT_ISC_BOTHEDGES_gc,
		isc_rising = PORT_ISC_enum::PORT_ISC_RISING_gc,
		isc_falling = PORT_ISC_enum::PORT_ISC_FALLING_gc,
		isc_input_disable = PORT_ISC_enum::PORT_ISC_INPUT_DISABLE_gc,
		isc_level = PORT_ISC_enum::PORT_ISC_LEVEL_gc
	};
	
	enum class PIN5CTRLMasks : mem_width {
		Inven = PORT_INVEN_bm,
		Pullupen = PORT_PULLUPEN_bm,
		isc_intdisable = PORT_ISC_enum::PORT_ISC_INTDISABLE_gc,
		isc_bothedges = PORT_ISC_enum::PORT_ISC_BOTHEDGES_gc,
		isc_rising = PORT_ISC_enum::PORT_ISC_RISING_gc,
		isc_falling = PORT_ISC_enum::PORT_ISC_FALLING_gc,
		isc_input_disable = PORT_ISC_enum::PORT_ISC_INPUT_DISABLE_gc,
		isc_level = PORT_ISC_enum::PORT_ISC_LEVEL_gc
	};
	
	enum class PIN6CTRLMasks : mem_width {
		Inven = PORT_INVEN_bm,
		Pullupen = PORT_PULLUPEN_bm,
		isc_intdisable = PORT_ISC_enum::PORT_ISC_INTDISABLE_gc,
		isc_bothedges = PORT_ISC_enum::PORT_ISC_BOTHEDGES_gc,
		isc_rising = PORT_ISC_enum::PORT_ISC_RISING_gc,
		isc_falling = PORT_ISC_enum::PORT_ISC_FALLING_gc,
		isc_input_disable = PORT_ISC_enum::PORT_ISC_INPUT_DISABLE_gc,
		isc_level = PORT_ISC_enum::PORT_ISC_LEVEL_gc
	};
	
	enum class PIN7CTRLMasks : mem_width {
		Inven = PORT_INVEN_bm,
		Pullupen = PORT_PULLUPEN_bm,
		isc_intdisable = PORT_ISC_enum::PORT_ISC_INTDISABLE_gc,
		isc_bothedges = PORT_ISC_enum::PORT_ISC_BOTHEDGES_gc,
		isc_rising = PORT_ISC_enum::PORT_ISC_RISING_gc,
		isc_falling = PORT_ISC_enum::PORT_ISC_FALLING_gc,
		isc_input_disable = PORT_ISC_enum::PORT_ISC_INPUT_DISABLE_gc,
		isc_level = PORT_ISC_enum::PORT_ISC_LEVEL_gc
	};
	
	enum class PORTCTRLMasks : mem_width {
		Srl = PORT_SRL_bm
	};
	
	struct registers {
		using dir = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Data>,0x00>;
		
		using dirclr = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Data>,0x02>;
		
		using dirset = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Data>,0x01>;
		
		using dirtgl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Toggle>,0x03>;
		
		using in = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Data>,0x08>;
		
		using intflags = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Flag,INTFLAGSMasks>,0x09>;
		
		using out = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Data>,0x04>;
		
		using outclr = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Data>,0x06>;
		
		using outset = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Data>,0x05>;
		
		using outtgl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Toggle>,0x07>;
		
		using pin0ctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,PIN0CTRLMasks>,0x10>;
		
		using pin1ctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,PIN1CTRLMasks>,0x11>;
		
		using pin2ctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,PIN2CTRLMasks>,0x12>;
		
		using pin3ctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,PIN3CTRLMasks>,0x13>;
		
		using pin4ctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,PIN4CTRLMasks>,0x14>;
		
		using pin5ctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,PIN5CTRLMasks>,0x15>;
		
		using pin6ctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,PIN6CTRLMasks>,0x16>;
		
		using pin7ctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,PIN7CTRLMasks>,0x17>;
		
		using portctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,PORTCTRLMasks>,0x0A>;
		
	};
	
};

struct ports {
	
};