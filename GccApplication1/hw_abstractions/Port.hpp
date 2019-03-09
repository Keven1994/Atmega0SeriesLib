/*
* Port.h
*
* Created: 29.01.2019 11:45:45
*  Author: Keven
*/

#pragma once
#include "../hw_abstractions/Register.hpp"
#include "../tools/utils.h"

namespace port {
	
	namespace {
		
		template<typename P, mem_width number>
		class PortPin;
		
		template<typename P, typename regs>
		class Port {
			
			template<typename p, mem_width number>
			friend class PortPin;
			
			static inline auto& port = P::value;
			public:
			using pins = typename P::pins;
			using portPins = typename P::template portPins<Port>;
			using registers = regs;
			
			NoConstructors(Port);
			
			template<typename reg>
			static inline auto& get(){
				using reg_t = typename reg::type;
				return reg_t::getRegister(*((typename reg_t::regSize*)&port() + reg::value));
			}

			[[nodiscard]] static inline auto& getDir() {
				return reg::Register<>::getRegister(port().DIR);
			};

			[[nodiscard]] static inline auto& getInput() {
				return reg::Register<reg::accessmode::ReadOnly>::getRegister(port().IN);
			};

			[[nodiscard]] static inline auto& getOutput() {
				return reg::Register<>::getRegister(port().OUT);
			};
			
			template<typename... PINS>
			static inline void setInput(PINS... args) {
				if constexpr(sizeof...(PINS) == 0){
					getDir().raw() = static_cast<mem_width>(0x00);
					} else {
					getDir().raw() &= ~(static_cast<mem_width>(args) | ...);
				}
			}
			
			template<typename... PINS>
			static inline void setOutput(PINS... args) {
				if constexpr(sizeof...(PINS) == 0){
					getDir().raw() = static_cast<mem_width>(-1);
					} else {
					getDir().raw() |= (static_cast<mem_width>(args) | ...);
				}
			}
		};

		template<typename P, mem_width number>
		class PortPin{
			
			static inline auto& port = P::port;
			
			public:
			
			NoConstructors(PortPin);
			
			static inline constexpr mem_width pinValue = 1 << number;
			
			static inline void off(){
				port().OUT &= ~pinValue;
			}
			
			static inline void on(){
				port().OUT |= pinValue;
			}
			
			static inline void invert(){
				port().OUT ^= pinValue;
			}
			
			static inline void setOutput(){ //no option to write as template -> parse error in template argument list, bug?
				port().DIR |= pinValue;
			}
			
			static inline void setInput() {
				port().DIR &= ~pinValue;
			}
			
		};
	}
}
