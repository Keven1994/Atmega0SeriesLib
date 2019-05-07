/*
* Eventsystem.h
*
* Created: 22.02.2019 17:18:46
*  Author: keven
*/


#pragma once
#include "Register.hpp"
#include "../MCUSelect.hpp"
#include "../tools/AVRConcepts.hpp"

namespace AVR {
	namespace eventsystem {


		template<mem_width number, typename defaultMCU = DEFAULT_MCU>
		struct Channel {


            using users = typename defaultMCU::EventSystem::users;
			using generators = typename defaultMCU::EventSystem::template generators<number>;
			
			NoConstructors(Channel);
			
			template<typename generator>
			static inline void setGenerator(){
				generators::value() = static_cast<mem_width>(generator::value);
			}
			
			template<typename... user>
			static inline void registerListener(){
                ((user::listener() |= 1 << number),...);
			}

			static inline void softwareEvent(){
			    EVSYS.STROBE = 1 << number;
			}
			
		};
	}
}
