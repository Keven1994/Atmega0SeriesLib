/*
 * Eventsystem.h
 *
 * Created: 22.02.2019 17:18:46
 *  Author: keven
 */ 


#pragma once
#include "Register.hpp"
#include "../tools/utils.h"
#include "../tools/AVRConcepts.hpp"


	namespace eventsystem {
				
			template<uint8_t channelNumber,typename Generators, typename Users>
			struct Channel {
				static inline constexpr uint8_t chNumber = channelNumber;
				
				using generators = Generators;
				using users = Users;
				
				Channel() = delete;
				Channel(const Channel&) = delete;
				Channel(Channel&&) = delete;
				
				template<typename generator>
				static inline void setGenerator(){
					Register<mem_width>::getRegister(*(&EVSYS.CHANNEL0+channelNumber)).raw() = static_cast<mem_width>(generator::value);
				}
				
				template<typename user>
				static inline void registerListener(){
					static_assert(isEventSystemUser<user,mem_width>(),"this type does not meet requirements of eventsystemusable");
					user::listener = 1 << channelNumber;
				}
				
			};
		
}
