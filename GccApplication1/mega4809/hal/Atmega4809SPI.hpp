
/*
 * Atmega4809SPI.hpp
 *
 * Created: 01.03.2019 14:45:33
 *  Author: keven
 */ 

#include "../../hw_abstractions/SPI.hpp"
#include "Atmega4809Port.hpp"

namespace mega4808 {
	
	namespace {
		
		template<bool master = true>
		struct spiSelect{
			using SPI = spi::SPIMaster<port<ports::A>, port<ports::A>::pins::pin7,port<ports::A>::pins::pin4, port<ports::A>::pins::pin5, port<ports::A>::pins::pin6>;
		};

		template<>
		struct spiSelect<false>{
			using SPI = spi::SPISlave<port<ports::A>, port<ports::A>::pins::pin7,port<ports::A>::pins::pin4, port<ports::A>::pins::pin5, port<ports::A>::pins::pin6>;
		};
		
	}
	
	template<bool master = true>
	using _SPI = typename spiSelect<master>::SPI;
}