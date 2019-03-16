
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
	    enum class INTFLAGSMasks : mem_width {
		    Int0 = PORT_INT0_bm,
		    Int1 = PORT_INT1_bm,
		    Int2 = PORT_INT2_bm,
		    Int3 = PORT_INT3_bm,
		    Int4 = PORT_INT4_bm,
		    Int5 = PORT_INT5_bm,
		    Int6 = PORT_INT6_bm,
		    Int7 = PORT_INT7_bm
	    };
	    
	    enum class PINCTRLMasks : mem_width {
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
		    using pin0ctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,PINCTRLMasks>,0x10>;
		    using pin1ctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,PINCTRLMasks>,0x11>;
		    using pin2ctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,PINCTRLMasks>,0x12>;
		    using pin3ctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,PINCTRLMasks>,0x13>;
		    using pin4ctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,PINCTRLMasks>,0x14>;
		    using pin5ctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,PINCTRLMasks>,0x15>;
		    using pin6ctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,PINCTRLMasks>,0x16>;
		    using pin7ctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,PINCTRLMasks>,0x17>;
		    using portctrl = utils::Pair<reg::Register<reg::accessmode::RW,reg::specialization::Control,PORTCTRLMasks>,0x0A>;
	    };
	    
    };

    struct ports {
	    struct porta {
		    [[nodiscard,gnu::always_inline]] static inline auto& value()  { return PORTA;}
		    
		    struct Pin {
			    using pin0 = AVR::port::details::PortPin<AVR::port::A,0>;
			    using pin1 = AVR::port::details::PortPin<AVR::port::A,1>;
			    using pin2 = AVR::port::details::PortPin<AVR::port::A,2>;
			    using pin3 = AVR::port::details::PortPin<AVR::port::A,3>;
			    using pin4 = AVR::port::details::PortPin<AVR::port::A,4>;
			    using pin5 = AVR::port::details::PortPin<AVR::port::A,5>;
			    using pin6 = AVR::port::details::PortPin<AVR::port::A,6>;
			    using pin7 = AVR::port::details::PortPin<AVR::port::A,7>;
		    };
		    
	    };
	    
	    struct portc {
		    [[nodiscard,gnu::always_inline]] static inline auto& value()  { return PORTC;}
		    
		    struct Pin {
			    using pin0 = AVR::port::details::PortPin<AVR::port::C,0>;
			    using pin1 = AVR::port::details::PortPin<AVR::port::C,1>;
			    using pin2 = AVR::port::details::PortPin<AVR::port::C,2>;
			    using pin3 = AVR::port::details::PortPin<AVR::port::C,3>;
		    };
		    
	    };
	    
	    struct portd {
		    [[nodiscard,gnu::always_inline]] static inline auto& value()  { return PORTD;}
		    
		    struct Pin {
			    using pin0 = AVR::port::details::PortPin<AVR::port::D,0>;
			    using pin1 = AVR::port::details::PortPin<AVR::port::D,1>;
			    using pin2 = AVR::port::details::PortPin<AVR::port::D,2>;
			    using pin3 = AVR::port::details::PortPin<AVR::port::D,3>;
			    using pin4 = AVR::port::details::PortPin<AVR::port::D,4>;
			    using pin5 = AVR::port::details::PortPin<AVR::port::D,5>;
			    using pin6 = AVR::port::details::PortPin<AVR::port::D,6>;
			    using pin7 = AVR::port::details::PortPin<AVR::port::D,7>;
		    };
		    
	    };
	    
	    struct portf {
		    [[nodiscard,gnu::always_inline]] static inline auto& value()  { return PORTF;}
		    
		    struct Pin {
			    using pin0 = AVR::port::details::PortPin<AVR::port::F,0>;
			    using pin1 = AVR::port::details::PortPin<AVR::port::F,1>;
			    using pin2 = AVR::port::details::PortPin<AVR::port::F,2>;
			    using pin3 = AVR::port::details::PortPin<AVR::port::F,3>;
			    using pin4 = AVR::port::details::PortPin<AVR::port::F,4>;
			    using pin5 = AVR::port::details::PortPin<AVR::port::F,5>;
			    using pin6 = AVR::port::details::PortPin<AVR::port::F,6>;
		    };
		    
	    };
	    
    };
	
	template<typename P>
	using port = AVR::port::details::Port<P,portComponent>;
}
