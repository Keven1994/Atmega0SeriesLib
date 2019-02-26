#pragma once
#include "../../tools/utils.h"
#include "../../hw_abstractions/Register.hpp"
#include "../../hw_abstractions/Eventsystem.hpp"

namespace mega4809 {
	
struct users {	
	struct evporta {
		static inline auto& listener = EVSYS.USEREVOUTA;
	};
		
	struct evportb {
		static inline auto& listener = EVSYS.USEREVOUTB;
	};
		
	struct evportc {
		static inline auto& listener = EVSYS.USEREVOUTC;
	};
		
	struct evportd {
		static inline auto& listener = EVSYS.USEREVOUTD;
	};
		
	struct evporte {
		static inline auto& listener = EVSYS.USEREVOUTE;
	};
		
	struct evportf {
		static inline auto& listener = EVSYS.USEREVOUTF;
	};
	
	struct evtca0 {
		static inline auto& listener = EVSYS.USERTCA0;
	};
		
	struct evtcb0 {
		static inline auto& listener = EVSYS.USERTCB0;
	};
		
	struct evtcb1 {
		static inline auto& listener = EVSYS.USERTCB1;
	};
		
	struct evtcb2 {
		static inline auto& listener = EVSYS.USERTCB2;
	};
		
	struct evtcb3 {
		static inline auto& listener = EVSYS.USERTCB3;
	};
		
	struct evusart0 {
		static inline auto& listener = EVSYS.USERUSART0;
	};
		
	struct evusart1 {
		static inline auto& listener = EVSYS.USERUSART1;
	};
		
	struct evusart2 {
		static inline auto& listener = EVSYS.USERUSART2;
	};
		
	struct evusart3 {
		static inline auto& listener = EVSYS.USERUSART3;
	};
		
	struct evccllut0a {
		static inline auto& listener = EVSYS.USERCCLLUT0A;
	};
		
	struct evccllut0b {
		static inline auto& listener = EVSYS.USERCCLLUT0B;
	};
		
	struct evccllut1a {
		static inline auto& listener = EVSYS.USERCCLLUT1A;
	};
		
	struct evccllut1b {
		static inline auto& listener = EVSYS.USERCCLLUT1B;
	};
		
	struct evccllut2a {
		static inline auto& listener = EVSYS.USERCCLLUT2A;
	};
		
	struct evccllut2b {
		static inline auto& listener = EVSYS.USERCCLLUT2B;
	};
		
	struct evccllut3a {
		static inline auto& listener = EVSYS.USERCCLLUT3A;
	};
		
	struct evccllut3b {
		static inline auto& listener = EVSYS.USERCCLLUT3B;
	};
		
	struct evadc0 {
		static inline auto& listener = EVSYS.USERADC0;
	};
		
};
	
namespace {
	struct generalGenerators {
		
		#define ic(x) typename utils::integralConstant<mem_width,x>
		
		using pdi = ic(0x01);
		using rtc_ovf = ic(0x06); using rtc_cmp = ic(0x07);
		using ccl_lut0 = ic(0x10); using ccl_lut1 = ic(0x11); using ccl_lut2 = ic(0x12); using ccl_lut3 = ic(0x13);
		using ac0_out = ic(0x20); using adc0_comp = ic(0x24);
		using usart0_xck0 = ic(0x60); using usart0_xck1 = ic(0x61); using usart1_xck0 = ic(0x62); using usart1_xck1 = ic(0x63);
		using usart2_xck0 = ic(0x64); using usart2_xck1 = ic(0x65); using usart3_xck0 = ic(0x66); using usart3_xck1 = ic(0x67);
		using spi0_sck = ic(0x68);
		using tca0_ovf = ic(0x80); using tca0_err = ic(0x81);
		using tca0_cmp0 = ic(0x84); using tca0_cmp1 = ic(0x85); using tca0_cmp2 = ic(0x86);
		using tcb0_cmp = ic(0xA0); using tcb0_cmph = ic(0xA1); using tcb0_cmpl = ic(0xA2);
		using tcb1_cmp = ic(0xA4); using tcb1_cmph = ic(0xA5); using tcb1_cmpl = ic(0xA6);
		using tcb2_cmp = ic(0xA8); using tcb2_cmph = ic(0xA9); using tcb2_cmpl = ic(0xAA);
		using tcb3_cmp = ic(0xAC); using tcb3_cmph = ic(0xAD); using tcb3_cmpl = ic(0xAE);
		
		#undef ic
	};
	
	struct type1RTC {
		using div64 = utils::integralConstant<mem_width,0x08>;
		using div128 = utils::integralConstant<mem_width,0x09>;
		using div256 = utils::integralConstant<mem_width,0x0A>;
		using div512 = utils::integralConstant<mem_width,0x0B>;
	};
	
	struct type2RTC {
		using div1024 = utils::integralConstant<mem_width,0x08>;
		using div2048 = utils::integralConstant<mem_width,0x09>;
		using div4096 = utils::integralConstant<mem_width,0x0A>;
		using div8192 = utils::integralConstant<mem_width,0x0B>;
	};
	
	template<uint8_t number>
	struct generatorChannel;
	
	template<>
	struct generatorChannel<0> {
	
		using generals = generalGenerators;
		using RTCDivGenerator = type1RTC;
		
		template<uint8_t pinNumber>
		struct PortAGenerator {
			static_assert(pinNumber < 8, "only pins [0,7] allowed");
			using portAPin = utils::integralConstant<mem_width,(0x40+pinNumber)>;
		};
		
		template<uint8_t pinNumber>
		struct PortBGenerator {
			static_assert(pinNumber < 8, "only pins [0,7] allowed");
			using portBPin = utils::integralConstant<mem_width,(0x48+pinNumber)>;
		};


	};
	
	template<>
	struct generatorChannel<1> {
	
		using generals = generalGenerators;
		
		using RTCDivGenerator = type2RTC;
		
		template<uint8_t pinNumber>
		struct PortAGenerator {
			static_assert(pinNumber < 8, "only pins [0,7] allowed");
			using portAPin = utils::integralConstant<mem_width,(0x40+pinNumber)>;
		};
		
		template<uint8_t pinNumber>
		struct PortBGenerator {
			static_assert(pinNumber < 8, "only pins [0,7] allowed");
			using portBPin = utils::integralConstant<mem_width,(0x48+pinNumber)>;
		};
		
	};
	
	template<>
	struct generatorChannel<2> {
	
		using generals = generalGenerators;
		using RTCDivGenerator = type1RTC;
		
		template<uint8_t pinNumber>
		struct PortCGenerator {
			static_assert(pinNumber < 8, "only pins [0,7] allowed");
			using portAPin = utils::integralConstant<mem_width,(0x40+pinNumber)>;
		};
		
		template<uint8_t pinNumber>
		struct PortDGenerator {
			static_assert(pinNumber < 8, "only pins [0,7] allowed");
			using portBPin = utils::integralConstant<mem_width,(0x48+pinNumber)>;
		};

	};
	
	template<>
	struct generatorChannel<3> {
	
		using generals = generalGenerators;
		
		using RTCDivGenerator = type2RTC;
		
		template<uint8_t pinNumber>
		struct PortCGenerator {
			static_assert(pinNumber < 8, "only pins [0,7] allowed");
			using portAPin = utils::integralConstant<mem_width,(0x40+pinNumber)>;
		};
		
		template<uint8_t pinNumber>
		struct PortDGenerator {
			static_assert(pinNumber < 8, "only pins [0,7] allowed");
			using portBPin = utils::integralConstant<mem_width,(0x48+pinNumber)>;
		};
	};

	template<>
	struct generatorChannel<4> {
	
		using generals = generalGenerators;
		
		using RTCDivGenerator = type1RTC;
		
		template<uint8_t pinNumber>
		struct PortEGenerator {
			static_assert(pinNumber < 8, "only pins [0,7] allowed");
			using portAPin = utils::integralConstant<mem_width,(0x40+pinNumber)>;
		};
		
		template<uint8_t pinNumber>
		struct PortFGenerator {
			static_assert(pinNumber < 8, "only pins [0,7] allowed");
			using portBPin = utils::integralConstant<mem_width,(0x48+pinNumber)>;
		};
	};
	
	template<>
	struct generatorChannel<5> {
	
		using generals = generalGenerators;
		
		using RTCDivGenerator = type2RTC;
		
		template<uint8_t pinNumber>
		struct PortEGenerator {
			static_assert(pinNumber < 8, "only pins [0,7] allowed");
			using portAPin = utils::integralConstant<mem_width,(0x40+pinNumber)>;
		};
		
		template<uint8_t pinNumber>
		struct PortFGenerator {
			static_assert(pinNumber < 8, "only pins [0,7] allowed");
			using portBPin = utils::integralConstant<mem_width,(0x48+pinNumber)>;
		};
	};
	
	template<>
	struct generatorChannel<6> {
	
		using generals = generalGenerators;
		
		using RTCDivGenerator = type1RTC;
		
		template<typename generator>
		static inline void setGenerator(){
			Register<mem_width>::getRegister(EVSYS.CHANNEL6).raw() = static_cast<mem_width>(generator::value);
		}
	};
	
	template<>
	struct generatorChannel<7> {
	
		using generals = generalGenerators;
		
		using RTCDivGenerator = type2RTC;
		
		template<uint8_t pinNumber>
		struct PortCGenerator {
			static_assert(pinNumber < 8, "only pins [0,7] allowed");
			using portAPin = utils::integralConstant<mem_width,(0x40+pinNumber)>;
		};
		
		template<uint8_t pinNumber>
		struct PortDGenerator {
			static_assert(pinNumber < 8, "only pins [0,7] allowed");
			using portBPin = utils::integralConstant<mem_width,(0x48+pinNumber)>;
		};
	};

	template<mem_width number>
	using channel = eventsystem::Channel<0,mega4809::generatorChannel<number>,mega4809::users>;
	
	}
}