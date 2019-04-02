
/*
 * PortMux.hpp
 *
 * Created: 01.04.2019 13:42:24
 *  Author: keven
 */ 
#pragma once

namespace AVR {
	
	namespace portmux{
		
		template<auto N>
		struct PortMux{
			static constexpr auto value = N;	
		};
		
	}
	
}
