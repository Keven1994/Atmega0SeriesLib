
/*
* Atmega4808SPI.hpp
*
* Created: 28.02.2019 00:15:49
*  Author: keven
*/

#include "../../hw_abstractions/SPI.hpp"
#include "Atmega4808Port.hpp"

namespace mega4808 {
	
	namespace {
		
		template<bool msb, bool clockDouble, bool slaveSelectDisable, spi::TransferMode tmode, spi::BufferMode bmode, spi::Prescaler prescaler>
		struct spiMaster{
			using SPI = spi::SPIMaster<port<ports::A>, port<ports::A>::pins::pin7,port<ports::A>::pins::pin4, port<ports::A>::pins::pin5, port<ports::A>::pins::pin6,msb,clockDouble,slaveSelectDisable,tmode,bmode,prescaler>;
		};

		template<bool msb, spi::TransferMode tmode, spi::BufferMode bmode>
		struct spiSlave{
			using SPI = spi::SPISlave<port<ports::A>, port<ports::A>::pins::pin7,port<ports::A>::pins::pin4, port<ports::A>::pins::pin5, port<ports::A>::pins::pin6,msb,tmode,bmode>;
		};
	}
}
