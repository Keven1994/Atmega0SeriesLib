
/*
* TWI.hpp
*
* Created: 22.03.2019 19:50:16
*  Author: keven
*/

#include "Port.hpp"
#include "../MCUSelect.hpp"
#include <util/twi.h>

namespace AVR {
	
	namespace twi {
		
		struct blocking{}; struct notBlocking{};
		
		namespace details {
			template<typename accesstype, typename component, typename instance, typename bit_width>
			struct TWI {
				
			NoConstructors(TWI);
			
			
						using Bridgectrl = typename component::registers::bridgectrl;
						using Ctrla =  typename component::registers::ctrla;
						using Dbgctrl = typename component::registers::dbgctrl;
						using Maddr =  typename component::registers::maddr;
						using Mbaud =  typename component::registers::mbaud;
						using Mctrla =  typename component::registers::mctrla;
						using Mctrlb = typename component::registers::mctrlb;
						using Mdata =  typename component::registers::mdata;
						using Mstatus =  typename component::registers::mstatus;
						using Saddr =  typename component::registers::saddr;
						using Saddrmask =  typename component::registers::saddrmask;
						using Sctrla =  typename component::registers::sctrla;
						using Sctrlb =  typename component::registers::sctrlb;
						using Sdata =  typename component::registers::sdata;
						using Sstatus =  typename component::registers::sstatus;
				
				static inline void singleTransfer(bit_width data){
					
				}
				
				static inline void Transfer(bit_width* data, uint8_t n){
					while(--n >= 0)
					singleTransfer(data[n]);
				}
				
				[[nodiscard]] static inline bit_width singleReceive(){
					
					return 0;
				}
				
				[[nodiscard]] static inline bit_width* Receive(bit_width* data, uint8_t n){
					for(uint8_t i = 0; i < n; i++)
					data[i] = singleReceive();
					
					return data;
				}
				
				
			};
		}
		
		
		template<typename accesstype,  typename component, typename instance, typename Setting, typename bit_width = mem_width>
		struct SDAMaster : public details::TWI<accesstype,  component, instance, Setting> {
			
			template<auto& func,typename... Flags>
			requires(utils::sameTypes<SDAMaster::Mstatus::special_bit,Flags...>())
			static inline decltype(func()) testIfSet(Flags... flags){
				using ret_type = decltype(func());
				if(SDAMaster::Mstatus::getRegister(SDAMaster::instance::value().MSTATUS).areSet(flags...))
				return func();
				return ret_type{};
			}
			
			static inline void init(){
				TWI0.MCTRLB = TWI_FLUSH_bm;
			}
			
			static inline void stop(){
				
			}
		};
		
		template<typename accesstype,  typename component, typename instance, typename Setting, typename bit_width = mem_width>
		struct SDASlave : public details::TWI<accesstype,  component, instance, Setting>  {
			
			template<auto& func,typename... Flags>
			requires(utils::sameTypes<SDASlave::Mstatus::special_bit,Flags...>())
			static inline decltype(func()) doIfSet(Flags... flags){
				using ret_type = decltype(func());
				if(SDASlave::Mstatus::getRegister(SDASlave::instance::value().SSTATUS).areSet(flags...))
				return func();
				return ret_type{};
			}
			
			static inline void init(){
				
			}
		};
		
		
		
	}
	
}
