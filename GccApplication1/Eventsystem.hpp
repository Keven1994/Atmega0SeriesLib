/*
 * Eventsystem.h
 *
 * Created: 22.02.2019 17:18:46
 *  Author: keven
 */ 


#pragma once
#include "Register.hpp"
#include "utils.h"

	namespace eventsystem {
		namespace {
			struct generalGenerators {
				#define ic(x) typename utils::integralConstant<mem_width,x>
				using pdi = ic(0x01);
				using rtc_ovf = ic(0x06);
				using rtc_cmp = ic(0x07);
				using ccl_lut0 = ic(0x10); using ccl_lut1 = ic(0x11); using ccl_lut2 = ic(0x12); using ccl_lut3 = ic(0x13);
				using ac0_out = ic(0x20);
				using adc0_comp = ic(0x24);
				using usart0_xck0 = ic(0x60); using usart0_xck1 = ic(0x61); using usart1_xck0 = ic(0x62); using usart1_xck1 = ic(0x63);
				using usart2_xck0 = ic(0x64); using usart2_xck1 = ic(0x65); using usart3_xck0 = ic(0x66); using usart3_xck1 = ic(0x67);
				using spi0_sck = ic(0x68);
				using tca0_ovf = ic(0x80);
				using tca0_err = ic(0x81);
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
			
		}
		
		
		namespace {
			template<uint8_t channelNumber>
			struct _channel {
				using general = generalGenerators;
				_channel() = delete;
				
				template<typename generator>
				static inline void setGenerator(){
					Register<mem_width>::getRegister(*(&EVSYS.CHANNEL0+channelNumber)).raw() = static_cast<mem_width>(generator::value);
				}
				
				~_channel() = delete;
			};
		}
		
		template<uint8_t number>
		struct Channel;
		
		template<>
		struct Channel<0> : public _channel<0> {
			static inline constexpr uint8_t channelNumber = 0;
			
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
		struct Channel<1> : public _channel<1> {
			static inline constexpr uint8_t channelNumber = 1;
			
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
		struct Channel<2> : public _channel<2> {
			static inline constexpr uint8_t channelNumber = 2;
			
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
		struct Channel<3> : public _channel<3> {
			static inline constexpr uint8_t channelNumber = 3;
			
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
		struct Channel<4> : public _channel<4> {
			static inline constexpr uint8_t channelNumber = 4;
			
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
		struct Channel<5> : public _channel<5> {
			static inline constexpr uint8_t channelNumber = 5;
			
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
		struct Channel<6> : public _channel<6> {
			static inline constexpr uint8_t channelNumber = 6;
			using general = generalGenerators;
			
			using RTCDivGenerator = type1RTC;
			
			template<typename generator>
			static inline void setGenerator(){
				Register<mem_width>::getRegister(EVSYS.CHANNEL6).raw() = static_cast<mem_width>(generator::value);
			}
		};
		
		template<>
		struct Channel<7> : public _channel<7> {
			static inline constexpr uint8_t channelNumber = 7;
			
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
		
		
		struct EventSystem {
			
			struct channels {
				using ch0 = Channel<0>;
				using ch1 = Channel<1>;
				using ch2 = Channel<2>;
				using ch3 = Channel<3>;
				using ch4 = Channel<4>;
				using ch5 = Channel<5>;
				using ch6 = Channel<6>;
				using ch7 = Channel<7>;
			};
			
			enum users {
				porta, portb , portc, portd, porte, portf,
				ccl_lut0A, ccl_lut0B, ccl_luta1A, ccl_lut1B, ccl_lut2A, ccl_lut2B, ccl_lut3A, ccl_lut3B,
				strobe, adc0, 
				tca0, tcb0, tcb1,tcb2,tcb3,
				usart0, usart1, usart2, usart3
			};
			
			template<mem_width channelNumber, users user>
			static inline void listenToChannel() {
				if constexpr (user == porta) {
					EVSYS.USEREVOUTA = 1 << channelNumber;
				} else if constexpr (user == portb) {
					EVSYS.USEREVOUTB= 1 << channelNumber;
				} else if constexpr (user == portc) {
					EVSYS.USEREVOUTC = 1 << channelNumber;
				} else if constexpr (user == portd) {
					EVSYS.USEREVOUTD = 1 << channelNumber;
				} else if constexpr (user == porte) {
					EVSYS.USEREVOUTE = 1 << channelNumber;
				} else if constexpr (user == portf) {
					EVSYS.USEREVOUTF = 1 << channelNumber;
				} else if constexpr (user == porta) {
					EVSYS.USEREVOUTA = 1 << channelNumber;
				} else if constexpr (user == porta) {
					EVSYS.USEREVOUTA = 1 << channelNumber;
				} else if constexpr (user == porta) {
					EVSYS.USEREVOUTA = 1 << channelNumber;
				} else if constexpr (user == porta) {
					EVSYS.USEREVOUTA = 1 << channelNumber;
				} else if constexpr (user == porta) {
					EVSYS.USEREVOUTA = 1 << channelNumber;
				} else if constexpr (user == porta) {
					EVSYS.USEREVOUTA = 1 << channelNumber;
				} else if constexpr (user == porta) {
					EVSYS.USEREVOUTA = 1 << channelNumber;
				} else if constexpr (user == porta) {
					EVSYS.USEREVOUTA = 1 << channelNumber;
				} else if constexpr (user == porta) {
					EVSYS.USEREVOUTA = 1 << channelNumber;
				} else if constexpr (user == porta) {
					EVSYS.USEREVOUTA = 1 << channelNumber;
				} else if constexpr (user == porta) {
					EVSYS.USEREVOUTA = 1 << channelNumber;
				} else if constexpr (user == porta) {
					EVSYS.USEREVOUTA = 1 << channelNumber;
				} else if constexpr (user == porta) {
					EVSYS.USEREVOUTA = 1 << channelNumber;
				} else if constexpr (user == porta) {
					EVSYS.USEREVOUTA = 1 << channelNumber;
				} else if constexpr (user == porta) {
					EVSYS.USEREVOUTA = 1 << channelNumber;
				} else if constexpr (user == porta) {
					EVSYS.USEREVOUTA = 1 << channelNumber;
				} else if constexpr (user == porta) {
					EVSYS.USEREVOUTA = 1 << channelNumber;
				}		
			}
		};
}