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
			struct getRessourceHelp {
				using alt = typename utils::conditional<utils::isEqual<N,First>::value, resolveAlt<First>, typename getRessourceHelp<N, pins...>::alt>::type;
				using inst = typename utils::conditional<utils::isEqual<N,First>::value,resolveInst<First>, typename getRessourceHelp<N, pins...>::inst>::type;
			};
			
			template<typename N, typename First>
			struct getRessourceHelp<N,First> {
				using alt = typename utils::conditional<utils::isEqual<N,First>::value, resolveAlt<First>, void>::type;
				using inst = typename utils::conditional<utils::isEqual<N,First>::value, resolveInst<First>, void>::type;
			};

			template<auto N ,typename _first,typename _second, typename... pins>
			static constexpr bool checkRessource_helper(){
				using first = typename resolveAlt<_first>::list;
				using second = typename resolveAlt<_second>::list;
				
				if constexpr((sizeof...(pins)-N) > 1)
				return !Meta::contains_any<first, second>::value && checkRessource_helper<N+1,typename Meta::concat_t<first,second>, pins...>();
				else {
					return !Meta::contains_any<first,second>::value;
				}
			}
			
			template<auto N ,typename _first,typename _second, typename... pins>
			static constexpr bool checkInstance_helper(){
				using first = resolveInst<_first>;
				using second = resolveInst<_second>;
				
				if constexpr((sizeof...(pins)-N) > 1)
				return !utils::isEqual<first,second>::value && checkRessource_helper<N+1,typename Meta::concat_t<first,second>, pins...>();
				else {
					return !utils::isEqual<first,second>::value;
				}
			}
			
			template<typename first,typename... pins>
			static constexpr bool checkRessource() {
				if constexpr(sizeof...(pins) == 0)
				return true;
				else return checkRessource_helper<0,first,pins...>();
			}
			
			template<typename first,typename... pins>
			static constexpr bool checkInstance() {
				if constexpr(sizeof...(pins) == 0)
				return true;
				else return checkInstance_helper<0,first,pins...>();
			}
			
			static_assert(checkRessource<FIRST,PINS...>(), "I/O Pins conflicting");
			static_assert(checkInstance<FIRST,PINS...>(), "only multiple alternatives from one instance");
			public:
			
			template<typename N>
			struct getRessource {
				using ressource = utils::tuple<typename getRessourceHelp<N, FIRST, PINS...>::inst,typename getRessourceHelp<N, FIRST, PINS...>::alt>;
			};

		};
		
		template<auto N>
		using Number = utils::autoConstant<N>;
		
		template<typename Comp, typename instance, typename  alternative>
		using Instance = details::Component<Comp,instance::value,alternative::value>;
		
		template<typename FIRST,typename... PINS>
		using RessourceController = ResController<DEFAULT_MCU,FIRST,PINS...>;
	}
	
}