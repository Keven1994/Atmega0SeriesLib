/*
 * Atmega4809.h
 *
 * Created: 29.01.2019 11:37:00
 *  Author: Keven
 */ 


#pragma once
#include "Register.hpp"
#include "utils.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsubobject-linkage"
namespace mega4809 {
	using mem_width = uint8_t;
	using reg_t = volatile Register<mem_width>; 
	namespace port {
				enum Ports {
					A,B,C,D,E,F
				};
				
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
					PIN7CTRL = 23,  /* Pin 7 Control */
				};

		namespace {
			template<port::Ports P>
			struct Port {
				Port() = delete;
				Port(const Port&) = delete;
				Port(Port&&) = delete;

				static inline volatile auto& port = *(&PORTA + P);
				
				template<typename prot = RW>
				[[nodiscard]] static inline auto& getRegister(port::PortRegisters reg) {
					return reinterpret_cast<reg_t&>(*(&port.DIR+static_cast<mem_width>(reg)));
				};

				public:

				[[nodiscard]] static inline auto& getDir() {
					return getRegister<>(PortRegisters::DIR);
				};

				[[nodiscard]] static inline auto& getIn() {
					return getRegister<ReadOnly>(PortRegisters::IN);
				};

				[[nodiscard]] static inline auto& getOut() {
					return getRegister<>(PortRegisters::OUT);
				};
				
				template<typename Channel>
				static inline void RegisterOnChannel() {
					*(&EVSYS.USEREVOUTA + P) = 1 << Channel::channelNumber; 
				}
				
			}__attribute__((packed));
		}
	}
	
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
					
					template<typename generator>
					static inline void setGenerator(){
						Register<mem_width>::getRegister(*(&EVSYS.CHANNEL0+channelNumber)).raw() = static_cast<mem_width>(generator::value);
					}
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
			
			template<uint8_t number>
			[[nodiscard]] static inline auto& getChannel(){
				static_assert(number < 8, "wrong channel number [0,7]");
				return reinterpret_cast<reg_t&>(*(&EVSYS + number));
			};

			[[nodiscard]] static inline auto& getStrobe(){
				return reg_t::getRegister(EVSYS.STROBE);
			};
			
			
		};
	}
	struct Atmega4809 {
		
		struct Ports {
			Ports() = delete;
			using porta = port::Port<port::A>;
			using portb = port::Port<port::B>;
			using portc = port::Port<port::C>;
			using portd = port::Port<port::D>;
			using porte = port::Port<port::E>;
			using portf = port::Port<port::F>;
		};
	
	}__attribute__((packed));
}

#pragma GCC diagnostic pop
