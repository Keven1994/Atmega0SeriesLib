/*
* IncFile1.h
*
* Created: 30.03.2019 11:27:10
*  Author: keven
*/


#pragma once
#include "../tools/meta.h"
namespace AVR {
	
	namespace rc {
		
		namespace details{
			
			template<typename Alias>
			struct resolveComponent {
				using alt = typename Alias::super::template comps<Alias>::alt;
				using inst = typename Alias::super::template comps<Alias>::inst;
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
					using alt  =typename  comstr::template inst<Alias::Instance>::template alt<Alias::Alternative>;
					using inst  =typename  comstr::template inst<Alias::Instance>;
				};
				
			};
		}
		
		template<typename MCU,typename FIRST,typename... PINS>
		class ResController {
			
			template<typename Alias>
			using resolveAlt = typename details::resolveComponent<Alias>::alt;
			template<typename Alias>
			using resolveInst = typename details::resolveComponent<Alias>::inst;
			
			template<auto N, typename First, typename... pins>
			struct getRessourceHelp {
				using alt = typename utils::conditional<N == 0, resolveAlt<First>, typename getRessourceHelp<N-1, pins...>::alt>::type;
				using inst = typename utils::conditional<N == 0,resolveInst<First>, typename getRessourceHelp<N-1, pins...>::inst>::type;
			};
			
			template<auto N, typename First>
			struct getRessourceHelp<N,First> {
				using alt = typename utils::conditional<N == 0, resolveAlt<First>, void>::type;
				using inst = typename utils::conditional<N == 0, resolveInst<First>, void>::type;
			};

			template<auto N ,typename _first,typename _second, typename... pins>
			static constexpr bool checkRessource_helper(){
				using first = typename resolveAlt<_first>::list;
				using second = typename resolveAlt<_second>::list;
				
				if constexpr((sizeof...(pins)-N) > 1)
				return !Meta::contains_any<first, second>::value & checkRessource_helper<N+1,typename Meta::concat_t<first,second>, pins...>();
				else {
					return !Meta::contains_any<first,second>::value;
				}
			}
			
			template<typename first,typename... pins>
			static constexpr bool checkRessource() {
				if constexpr(sizeof...(pins) == 0)
				return true;
				else return checkRessource_helper<0,first,pins...>();
			}
			
			static_assert(checkRessource<FIRST,PINS...>(), "I/O Pins conflicting");
			
			public:
			
			template<auto N>
			struct getRessource {
				static_assert(N >= 0, "invalid number, only positives allowed");
				using alt = typename getRessourceHelp<N, FIRST, PINS...>::alt;
				using inst = typename getRessourceHelp<N, FIRST, PINS...>::inst;
			};

		};
		
		template<typename FIRST,typename... PINS>
		using RessourceController = ResController<DEFAULT_MCU,FIRST,PINS...>;
	}
	
}