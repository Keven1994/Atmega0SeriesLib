
/*
* TWI.hpp
*
* Created: 22.03.2019 19:50:16
*  Author: keven
*/
#pragma once
#include "Port.hpp"
#include "../MCUSelect.hpp"
#include <util/twi.h>

namespace AVR {
	
	namespace twi {
		
		struct blocking{}; struct notBlocking{};
		
		namespace details {
			
			template<typename component, typename instance>
			struct TWI {
				protected:
				template<typename Reg>
				[[nodiscard,gnu::always_inline]] static inline auto& reg(){
					return AVR::port::details::getRegister<Reg,instance::value>();
				}
			};
			
			template<typename accesstype,  typename component, typename instance, typename alt, typename Setting, typename bit_width = mem_width>
			class TWIMaster : protected TWI<component, instance> {
				using Bridgectrl = typename component::registers::bridgectrl;
				using Ctrla =  typename component::registers::ctrla;
				using Dbgctrl = typename component::registers::dbgctrl;
				using Maddr =  typename component::registers::maddr;
				using Mbaud =  typename component::registers::mbaud;
				using Mctrla =  typename component::registers::mctrla;
				using Mctrlb = typename component::registers::mctrlb;
				using Mdata =  typename component::registers::mdata;
				using Mstatus =  typename component::registers::mstatus;
				
				public:
				using status_bits = typename TWIMaster::Mstatus::type::special_bit;
				
				template<bool pinSetting = true, uint8_t address>
				requires(address < (1 << 7))
				static inline void singleTransfer(uint8_t data){
					constexpr mem_width addr = ~(TWI_ADDREN_bm) & address;
					auto& status = TWIMaster::template reg<Mstatus>();
					
					if constexpr(pinSetting){
						alt::Sda::pin0::setOutput();
					}
					
					TWIMaster::template reg<Maddr>().set(addr);
					while(! (status.areSet(status_bits::Wif)));
					TWIMaster::template reg<Mdata>().set(data);
					while(! (status.areSet(status_bits::Wif)));
				}
				
				static inline void Transfer(bit_width* data, uint8_t n){
					while(--n >= 0)
					singleTransfer(data[n]);
				}
				
				template<bool pinSetting = true, uint8_t address>
				[[nodiscard]] static inline bit_width singleReceive(){
					static constexpr uint8_t addr = TWI_ADDREN_bm | (address << 1);
					if constexpr(pinSetting){
						alt::Sda::pin0::setInput();
					}
					TWIMaster::template reg<Maddr>().set(addr);
					while(! (TWIMaster::template reg<Mstatus>().areSet(Mstatus::special_bit::Rif)));
					return TWIMaster::template reg<Mdata>().raw();
				}
				
				template<bool pinSetting = true>
				[[nodiscard]] static inline bit_width* Receive(bit_width* data, uint8_t n){
					for(uint8_t i = 0; i < n; i++)
					data[i] = singleReceive<pinSetting>();
					
					return data;
				}
				
				template<auto& func,typename... Flags>
				requires(utils::sameTypes<status_bits,Flags...>())
				static inline auto doIfSet(Flags... flags){
					using ret_type = decltype(func());
					if(TWIMaster::template reg<Mstatus>().areSet(flags...))
					return func();
					return ret_type{};
				}
				
				template<auto& func,typename... Flags>
				requires(utils::sameTypes<status_bits,Flags...>())
				static inline auto doIfAnySet(Flags... flags){
					using ret_type = decltype(func());
					if(TWIMaster::template reg<Mstatus>().anySet(flags...))
					return func();
					return ret_type{};
				}
				
				static inline void init(){
					alt::Scl::pin0::setOutput();
					TWIMaster::template reg<Ctrla>().set(Setting::fastmode,Setting::holdtime,Setting::setuptime);
					TWIMaster::template reg<Mbaud>().set(Setting::baud);
					TWIMaster::template reg<Mctrla>().set(Setting::quickcommand,Setting::smartmode,Setting::timeout,Mctrla::type::special_bit::Enable);
					busStateIdle();
				}
				
				static inline void busStateIdle(){
					TWIMaster::template reg<Mstatus>().toggle(status_bits::Busstate_idle);
				}
				
				static inline void stop(){
					TWIMaster::template reg<Mctrlb>().on(TWIMaster::Mctrlb::special_bit::Mcmd_stop);
				}
				
				private:
				static inline void restart() {
					TWIMaster::template reg<Mctrlb>().on(TWIMaster::Mctrlb::special_bit::Mcmd_repstart);
				}
			};
			
			template<typename accesstype,  typename component, typename instance, typename alt, typename Setting, typename bit_width = mem_width>
			class TWISlave : public TWI<component,instance>  {
				
				using Bridgectrl = typename component::registers::bridgectrl;
				using Ctrla =  typename component::registers::ctrla;
				using Dbgctrl = typename component::registers::dbgctrl;
				using Saddr =  typename component::registers::saddr;
				using Saddrmask =  typename component::registers::saddrmask;
				using Sctrla =  typename component::registers::sctrla;
				using Sctrlb =  typename component::registers::sctrlb;
				using Sdata =  typename component::registers::sdata;
				using Sstatus =  typename component::registers::sstatus;
				using status_bits = typename component::registers::sstatus::type::special_bit;
				
				public:
				
				template<auto& func,typename... Flags>
				requires(utils::sameTypes<Sstatus::special_bit,Flags...>())
				static inline auto doIfSet(Flags... flags){
					using ret_type = decltype(func());
					if(TWISlave::template reg<Sstatus>().areSet(flags...))
					return func();
					return ret_type{};
				}
				
				template<uint8_t address>
				requires(address < (1<<7))
				static inline void init(){
					alt::Scl::pin0::setInput();
					TWISlave::template reg<Ctrla>().set(Setting::fastmode,Setting::holdtime,Setting::setuptime);
					TWISlave::template reg<Saddr>().set(address);
				}
				
				template<bool pinSetting = true, uint8_t address>
				[[nodiscard]] static inline bit_width singleReceive(){
					auto& statusreg = TWISlave::template reg<Sstatus>();
					if constexpr(pinSetting){
						alt::Sda::pin0::setInput();
					}
					while(!(statusreg.areSet(status_bits::Dif) && !statusreg.areSet(status_bits::Dir)));
					return TWISlave::template reg<Sdata>.raw();
				}
			};
		}
		
		using SDAHold =  typename DEFAULT_MCU::TWI::SDAHold;
		using SDASetup = typename DEFAULT_MCU::TWI::SDASetup;
		using MasterTimeout = typename DEFAULT_MCU::TWI::MasterTimeout;
		using Twis = typename DEFAULT_MCU::TWI::Components;

		template<typename accesstype = blocking,typename instance = Twis::twi0, typename alternative = typename instance::I2c, bool fastModePlus = false, SDAHold holdTime = SDAHold::Setup4Cycles, SDASetup sdaSetup = SDASetup::SDASetup_300ns,
		bool quickCommand = true, bool smartMode = true, MasterTimeout timeOut = MasterTimeout::Disabled, typename bit_width = mem_width>
		using TWIMaster = AVR::twi::details::TWIMaster<accesstype,typename DEFAULT_MCU::TWI::Component,instance,alternative, typename DEFAULT_MCU::TWI::template TWIMasterSetting<fastModePlus,holdTime,sdaSetup,quickCommand,smartMode,timeOut,21>, bit_width>;
		
		template<typename accesstype = blocking,typename instance = Twis::twi0,typename alternative = typename instance::I2c, bool fastModePlus = false, SDAHold holdTime = SDAHold::Setup4Cycles, SDASetup sdaSetup = SDASetup::SDASetup_300ns,typename bit_width = mem_width>
		using TWISlave = AVR::twi::details::TWISlave<accesstype,typename DEFAULT_MCU::TWI::Component,instance,alternative,typename DEFAULT_MCU::TWI::template TWISlaveSetting<fastModePlus,holdTime,sdaSetup>,bit_width>;

		
		
	}
	
}
