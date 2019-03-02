
/*
* Atmega4809SPI.hpp
*
* Created: 01.03.2019 14:45:33
*  Author: keven
*/

#include "../../hw_abstractions/SPI.hpp"
#include "Atmega4809Port.hpp"

namespace mega4809 {
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