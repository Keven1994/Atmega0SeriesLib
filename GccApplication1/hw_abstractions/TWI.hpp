
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
			template<typename accesstype, typename component, typename instance, typename alt, typename bit_width>
			struct TWI {
				
				NoConstructors(TWI);
				
				
				using Bridgectrl = typename component::registers::bridgectrl::type;
				using Ctrla =  typename component::registers::ctrla::type;
				using Dbgctrl = typename component::registers::dbgctrl::type;
				using Maddr =  typename component::registers::maddr::type;
				using Mbaud =  typename component::registers::mbaud::type;
				using Mctrla =  typename component::registers::mctrla::type;
				using Mctrlb = typename component::registers::mctrlb::type;
				using Mdata =  typename component::registers::mdata::type;
				using Mstatus =  typename component::registers::mstatus::type;
				using Saddr =  typename component::registers::saddr::type;
				using Saddrmask =  typename component::registers::saddrmask::type;
				using Sctrla =  typename component::registers::sctrla::type;
				using Sctrlb =  typename component::registers::sctrlb::type;
				using Sdata =  typename component::registers::sdata::type;
				using Sstatus =  typename component::registers::sstatus::type;
				
				static inline void singleTransfer(bit_width data){
					while(! (Mstatus::getRegister(instance::value().MSTATUS).areSet(Mstatus::special_bit::Wif)));
				}
				
				static inline void Transfer(bit_width* data, uint8_t n){
					while(--n >= 0)
					singleTransfer(data[n]);
				}
				
				template<bool pinSetting = true>
				[[nodiscard]] static inline bit_width singleReceive(){
					if constexpr(pinSetting){
						alt::Sda::pin0::setInput();
					}
					while(! (Mstatus::getRegister(instance::value().MSTATUS).areSet(Mstatus::special_bit::Rif)));
					return bit_width{};
				}
				
				template<bool pinSetting = true>
				[[nodiscard]] static inline bit_width* Receive(bit_width* data, uint8_t n){
					for(uint8_t i = 0; i < n; i++)
					data[i] = singleReceive<pinSetting>();
					
					return data;
				}
				
				
			};
			
			
			template<typename accesstype,  typename component, typename instance, typename alt, typename Setting, typename bit_width = mem_width>
			struct TWIMaster : public details::TWI<accesstype,  component, instance,alt, Setting> {
				
				template<auto& func,typename... Flags>
				requires(utils::sameTypes<typename TWIMaster::Mstatus::special_bit,Flags...>())
				static inline decltype(func()) testIfSet(Flags... flags){
					using ret_type = decltype(func());
					if(TWIMaster::Mstatus::getRegister(instance::value().MSTATUS).areSet(flags...))
					return func();
					return ret_type{};
				}
				
				static inline void init(){
					alt::Scl::pin0::setOutput();
					TWIMaster::Ctrla::getRegister(instance::value().CTRLA).set(Setting::fastmode,Setting::holdtime,Setting::setuptime);
					TWIMaster::Mbaud::getRegister(instance::value().MBAUD).set(Setting::baud);
					TWIMaster::Mctrla::getRegister(instance::value().MCTRLA).set(Setting::quickcommand,Setting::smartmode,Setting::timeout,TWIMaster::Mctrla::special_bit::Enable);
				}
				
				static inline void stop(){
					TWIMaster::Mctrlb::getRegister(instance::value().MCTRLB).on(TWIMaster::Mctrlb::special_bit::Mcmd_stop);
				}
				
				private:
				static inline void restart() {
					TWIMaster::Mctrlb::getRegister(instance::value().MCTRLB).on(TWIMaster::Mctrlb::special_bit::Mcmd_repstart);
				}
			};
			
			template<typename accesstype,  typename component, typename instance, typename alt, typename Setting, typename bit_width = mem_width>
			struct TWISlave : public details::TWI<accesstype,  component, instance, alt, Setting>  {
				
				template<auto& func,typename... Flags>
				requires(utils::sameTypes<TWISlave::Mstatus::special_bit,Flags...>())
				static inline decltype(func()) doIfSet(Flags... flags){
					using ret_type = decltype(func());
					if(TWISlave::Mstatus::getRegister(TWISlave::instance::value().SSTATUS).areSet(flags...))
					return func();
					return ret_type{};
				}
				
				static inline void init(){
					alt::Scl::pin0::setInput();
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
