/*
* IncFile1.h
*
* Created: 30.03.2019 11:27:10
*  Author: keven
*/


#pragma once
#include "../tools/meta.h"
#include "../tools/utils.h"
#include "PortMux.hpp"
#include "../MCUSelect.hpp"
#include <type_traits>

namespace AVR {
	
	namespace rc {
		
		namespace details{
			
			template<typename Alias>
			struct resolveComponent {
				using inst = typename Alias::super::template comps<Alias>::inst;
				using alt = typename Alias::super::template comps<Alias>::alt;
			};
			
			template<typename Comp, auto instance, auto alternative>
			struct Component {
				using super = Comp;
				static constexpr auto Instance = instance;
				static constexpr auto Alternative = alternative;
			};
			
			template<typename comstr>
			class RCComponent {
				template<typename Alias>
				friend struct AVR::rc::details::resolveComponent;
				template<typename Alias>
				struct comps{
					using inst  =typename  comstr::template inst<Alias::Instance>;
					using alt  =typename  inst::template alt<Alias::Alternative>;
				};
				
			};
		}
		
		template<typename MCU,typename FIRST,typename... PINS>
		class ResController {
			
			template<typename Alias>
			using resolveAlt = typename details::resolveComponent<Alias>::alt;
			template<typename Alias>
			using resolveInst = typename details::resolveComponent<Alias>::inst;
			
			template<typename N, typename First, typename... pins>
			struct get_ressource_help {
				using alt = typename utils::conditional<utils::isEqual<N,First>::value, resolveAlt<First>, typename get_ressource_help<N, pins...>::alt>::type;
				using inst = typename utils::conditional<utils::isEqual<N,First>::value,resolveInst<First>, typename get_ressource_help<N, pins...>::inst>::type;
			};
			
			template<typename N, typename First>
			struct get_ressource_help<N,First> {
				using alt = typename utils::conditional<utils::isEqual<N,First>::value, resolveAlt<First>, void>::type;
				using inst = typename utils::conditional<utils::isEqual<N,First>::value, resolveInst<First>, void>::type;
			};

			template<Meta::concepts::List first,typename _second, typename... pins>
			static constexpr bool checkRessourceHelper(){
				
				using second = typename resolveAlt<_second>::list;
				
				if constexpr(sizeof...(pins) > 0){
					constexpr bool current = !Meta::contains_any<first, second>::value;
					constexpr bool next =  checkRessourceHelper<Meta::concat_t<first,second>, pins...>();
					return  current && next ;
				}
				else {
					return !Meta::contains_any<first,second>::value;
				}
			}
			
			template<Meta::concepts::List first,typename _second, typename... pins>
			static constexpr bool checkInstanceHelper(){
				using second = Meta::List<resolveInst<_second>>;
				
				if constexpr(sizeof...(pins) > 0)
					return !Meta::contains_any<first,second>::value && checkInstanceHelper<typename Meta::concat_t<first,second>, pins...>();
				else {
					return !Meta::contains_any<first,second>::value;
				}
			}
			
			template<typename _first,typename... pins>
			static constexpr bool checkRessource() {
				using first = typename resolveAlt<_first>::list;
								
				if constexpr(sizeof...(pins) == 0)
				return true;
				else return checkRessourceHelper<first,pins...>();
			}
			
			template<typename _first,typename... pins>
			static constexpr bool checkInstance() {
				using first = resolveInst<_first>;
				
				if constexpr(sizeof...(pins) == 0)
				return true;
				else return checkInstanceHelper<Meta::List<first>,pins...>();
			}
			
				
				//
			public:
			
			template<typename N>
			struct getRessource {
				using type = utils::tuple<typename get_ressource_help<N, FIRST, PINS...>::inst,typename get_ressource_help<N, FIRST, PINS...>::alt>;
				static_assert(!std::is_same<typename type::t2,void>::value , "portmux not found");
				static_assert(checkRessource<FIRST,PINS...>(), "I/O Pins conflicting");
				static_assert(checkInstance<FIRST,PINS...>(), "only 1 alternative from a single instance permitted");
			};

			template<typename N>
			using getRessource_t = typename getRessource<N>::type;
		};
		
		template<auto N>
		using Number = utils::autoConstant<N>;
		
		template<typename Comp, typename instance, typename  alternative>
		using Instance = details::Component<Comp,instance::value,alternative::value>;
		
		template<typename FIRST,typename... PINS>
		using RessourceController = ResController<DEFAULT_MCU,FIRST,PINS...>;
	}
	
}