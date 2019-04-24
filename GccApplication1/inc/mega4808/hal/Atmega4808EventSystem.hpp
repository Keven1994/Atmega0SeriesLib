#pragma once
#include "../../tools/utils.h"
#include "../../hw_abstractions/Eventsystem.hpp"

namespace mega4808 {
	namespace details {
		struct users {
            struct evporta {
                [[nodiscard, gnu::always_inline]] static inline auto& listener() { return EVSYS.USEREVOUTA;}               };

            struct evportb {
                [[nodiscard, gnu::always_inline]] static inline auto& listener() { return EVSYS.USEREVOUTB;}               };

            struct evportc {
                [[nodiscard, gnu::always_inline]] static inline auto& listener() { return EVSYS.USEREVOUTC;}               };

            struct evportd {
                [[nodiscard, gnu::always_inline]] static inline auto& listener() { return EVSYS.USEREVOUTD;}               };

            struct evporte {
                [[nodiscard, gnu::always_inline]] static inline auto& listener() { return EVSYS.USEREVOUTE;}               };

            struct evportf {
                [[nodiscard, gnu::always_inline]] static inline auto& listener() { return EVSYS.USEREVOUTF;}               };

            struct evtca0 {
                [[nodiscard, gnu::always_inline]] static inline auto& listener() { return EVSYS.USERTCA0;}               };

            struct evtcb0 {
                [[nodiscard, gnu::always_inline]] static inline auto& listener() { return EVSYS.USERTCB0;}               };

            struct evtcb1 {
                [[nodiscard, gnu::always_inline]] static inline auto& listener() { return EVSYS.USERTCB1;}               };

            struct evtcb2 {
                [[nodiscard, gnu::always_inline]] static inline auto& listener() { return EVSYS.USERTCB2;}               };

            struct evtcb3 {
                [[nodiscard, gnu::always_inline]] static inline auto& listener() { return EVSYS.USERTCB3;}               };

            struct evusart0 {
                [[nodiscard, gnu::always_inline]] static inline auto& listener() { return EVSYS.USERUSART0;}               };

            struct evusart1 {
                [[nodiscard, gnu::always_inline]] static inline auto& listener() { return EVSYS.USERUSART1;}               };

            struct evusart2 {
                [[nodiscard, gnu::always_inline]] static inline auto& listener() { return EVSYS.USERUSART2;}               };

            struct evusart3 {
                [[nodiscard, gnu::always_inline]] static inline auto& listener() { return EVSYS.USERUSART3;}               };

            struct evccllut0a {
                [[nodiscard, gnu::always_inline]] static inline auto& listener() { return EVSYS.USERCCLLUT0A;}               };

            struct evccllut0b {
                [[nodiscard, gnu::always_inline]] static inline auto& listener() { return EVSYS.USERCCLLUT0B;}               };

            struct evccllut1a {
                [[nodiscard, gnu::always_inline]] static inline auto& listener() { return EVSYS.USERCCLLUT1A;}               };

            struct evccllut1b {
                [[nodiscard, gnu::always_inline]] static inline auto& listener() { return EVSYS.USERCCLLUT1B;}               };

            struct evccllut2a {
                [[nodiscard, gnu::always_inline]] static inline auto& listener() { return EVSYS.USERCCLLUT2A;}               };

            struct evccllut2b {
                [[nodiscard, gnu::always_inline]] static inline auto& listener() { return EVSYS.USERCCLLUT2B;}               };

            struct evccllut3a {
                [[nodiscard, gnu::always_inline]] static inline auto& listener() { return EVSYS.USERCCLLUT3A;}               };

            struct evccllut3b {
                [[nodiscard, gnu::always_inline]] static inline auto& listener() { return EVSYS.USERCCLLUT3B;}               };

            struct evadc0 {
                [[nodiscard, gnu::always_inline]] static inline auto& listener() { return EVSYS.USERADC0;}               };
			
		};
		
		struct generalGenerators {

			#define ic(x) typename utils::integralConstant<mem_width,x>
			
			using pdi = ic(0x01);
			using rtc_ovf = ic(0x06); using rtc_cmp = ic(0x07);
			using ccl_lut0 = ic(0x10); using ccl_lut1 = ic(0x11); using ccl_lut2 = ic(0x12); using ccl_lut3 = ic(0x13);
			using ac0_out = ic(0x20); using adc0_comp = ic(0x24);
			using usart0_xck0 = ic(0x60); using usart0_xck1 = ic(0x61); using usart1_xck0 = ic(0x62); using usart1_xck1 = ic(0x63);
			using usart2_xck0 = ic(0x64); using usart2_xck1 = ic(0x65);
			using spi0_sck = ic(0x68);
			using tca0_ovf = ic(0x80); using tca0_err = ic(0x81);
			using tca0_cmp0 = ic(0x84); using tca0_cmp1 = ic(0x85); using tca0_cmp2 = ic(0x86);
			using tcb0_cmp = ic(0xA0); using tcb0_cmph = ic(0xA1); using tcb0_cmpl = ic(0xA2);
			using tcb1_cmp = ic(0xA4); using tcb1_cmph = ic(0xA5); using tcb1_cmpl = ic(0xA6);
			using tcb2_cmp = ic(0xA8); using tcb2_cmph = ic(0xA9); using tcb2_cmpl = ic(0xAA);
			
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
		
		template<mem_width number>
		struct generatorChannel;
		
		template<>
		struct generatorChannel<0> {
			
			[[nodiscard, gnu::always_inline]] static inline auto& value() { return EVSYS.CHANNEL0;}
			using generals = generalGenerators;
			using RTCDivGenerator = type1RTC;
			
			template<typename pin>
			struct PortAGenerator {
				static inline constexpr mem_width value = 0x40+pin::pinValue;
			};

		};
		
		template<>
		struct generatorChannel<1> {
			
			[[nodiscard, gnu::always_inline]] static inline auto& value() { return EVSYS.CHANNEL1;}
			using generals = generalGenerators;
			
			using RTCDivGenerator = type2RTC;
			
			template<typename pin>
			struct PortAGenerator {
				
				static inline constexpr mem_width value = 0x40+pin::pinValue;
			};

		};
		
		template<>
		struct generatorChannel<2> {
			
			[[nodiscard, gnu::always_inline]] static inline auto& value() { return EVSYS.CHANNEL2;}
			using generals = generalGenerators;
			using RTCDivGenerator = type1RTC;
			
			template<typename pin>
			struct PortCGenerator {
				
				static inline constexpr mem_width value = 0x40+pin::pinValue;
			};
			
			template<typename pin>
			struct PortDGenerator {
				
				static inline constexpr mem_width value = 0x48+pin::pinValue;
			};

		};
		
		template<>
		struct generatorChannel<3> {
			
			[[nodiscard, gnu::always_inline]] static inline auto& value() { return EVSYS.CHANNEL3;}
			using generals = generalGenerators;
			
			using RTCDivGenerator = type2RTC;
			
			template<typename pin>
			struct PortCGenerator {
				
				static inline constexpr mem_width value = 0x40+pin::pinValue;
			};
			
			template<typename pin>
			struct PortDGenerator {
				
				static inline constexpr mem_width value = 0x48+pin::pinValue;
			};
		};

		template<>
		struct generatorChannel<4> {
			
			[[nodiscard, gnu::always_inline]] static inline auto& value() { return EVSYS.CHANNEL4;}
			using generals = generalGenerators;
			
			using RTCDivGenerator = type1RTC;
			
			template<typename pin>
			struct PortFGenerator {
				
				static inline constexpr mem_width value = 0x48+pin::pinValue;
			};
		};
		
		template<>
		struct generatorChannel<5> {
			
			[[nodiscard, gnu::always_inline]] static inline auto& value() { return EVSYS.CHANNEL5;}
			using generals = generalGenerators;
			
			using RTCDivGenerator = type2RTC;
			
			template<typename pin>
			struct PortFGenerator {
				
				static inline constexpr mem_width value = 0x48+pin::pinValue;
			};
		};
		
	}
}