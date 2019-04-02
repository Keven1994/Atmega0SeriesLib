/*
* Port.h
*
* Created: 29.01.2019 11:45:45
*  Author: Keven
*/

#pragma once
#include "Register.hpp"
#include "../tools/utils.h"
#include "../MCUSelect.hpp"

namespace AVR{
	namespace port {
		
		namespace details {
			
			template<typename T, auto& inst>
			[[nodiscard,gnu::always_inline]] static inline auto& getRegister(){
					using reg_t = typename T::type;
					return reg_t::getRegister(*((typename reg_t::regSize*)&inst() + T::value));
			}
			
			template<typename first,typename ...T>
			[[nodiscard]] constexpr bool samePorts() noexcept {
				if constexpr(sizeof...(T) == 0)
				return true;
				else {
					if constexpr (!utils::isEqual<typename first::port, typename utils::front<T...>::type::port>::value)
					return false;
					else
					return samePorts<T...>();
				}
				
			}
			
			template<typename P, typename regs>
			struct Port {
				
				static inline auto& port = P::value;
				
				using pins = typename P::Pin;
				using registers = typename regs::registers;
				
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
			
			template<typename... Pins>
			static inline void PinsOn();
			
			template<typename P, mem_width number>
			class PortPin{
				
				static inline auto& _port = P::port;

				public:

				using port = P;

				NoConstructors(PortPin);
				
				static inline constexpr auto pinValue = Bit(number);
				
				static inline void off(){
					_port().OUT &= ~pinValue;
				}
				
				static inline void on(){
					_port().OUT |= pinValue;
				}
				
				static inline void toggle(){
					_port().OUT ^= pinValue;
				}
				
				static inline void setOutput(){ //no option to write as template -> parse error in template argument list, bug?
					_port().DIR |= pinValue;
				}
				
				static inline void setInput() {
					_port().DIR &= ~pinValue;
				}
				
			};
		}
		
		template<typename... Pins>
		requires(details::samePorts<Pins...>())
		static inline void PinsOn() {
			using firstPin = typename utils::front<Pins...>::type;
			auto& Pval = firstPin::port::port().OUT;
			Pval |= (Pins::pinValue | ...);
		}
		
		template<typename... Pins>
		requires(details::samePorts<Pins...>())
		static inline void PinsDirIn() {
			using firstPin = typename utils::front<Pins...>::type;
			auto& Pval = firstPin::port::port().DIR;
			Pval &= ~(Pins::pinValue | ...);
		}
		
		template<typename... Pins>
		requires(details::samePorts<Pins...>())
		static inline void PinsDirOut() {
			using firstPin = typename utils::front<Pins...>::type;
			auto& Pval = firstPin::port::port().DIR;
			Pval |= (Pins::pinValue | ...);
		}
		
		template<typename... Pins>
		requires(details::samePorts<Pins...>())
		static inline void PinsToggle() {
			using firstPin = typename utils::front<Pins...>::type;
			auto& Pval = firstPin::port::port().OUT;
			Pval ^= (Pins::pinValue | ...);
		}
		
		template<typename... Pins>
		requires(details::samePorts<Pins...>())
		static inline void PinsOff() {
			using firstPin = typename utils::front<Pins...>::type;
			auto& Pval = firstPin::port::port().OUT;
			Pval &= ~(Pins::pinValue | ...);
		}

		struct A{};struct B{};struct C{};struct D{};struct E{};struct F{};
		
		template<typename p, typename DefaultMCU = DEFAULT_MCU>
		using Port = typename DefaultMCU::template Ports<p>::portX;
		
		template<typename DefaultMCU = DEFAULT_MCU>
		using Port_Registers = typename DefaultMCU::port_registers;
		
		template<typename p, mem_width number>
		using Pin = details::PortPin<p,number>;
	}
}
