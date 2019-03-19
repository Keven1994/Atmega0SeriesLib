
/*
* SPI.hpp
*
* Created: 27.02.2019 10:21:27
*  Author: keven
*/

#include "Register.hpp"

namespace spi {
	
	template<typename SPIComponent, typename spiInf, auto order,  auto clockDoubled, auto slaveSelectDisable,
	auto transferMode, auto buffered, auto waitForReceive, auto prescaler>
	struct SPIMaster {

		NoConstructors(SPIMaster);
		using ControlA = typename SPIComponent::registers::ctrla::type;
		using ControlB = typename SPIComponent::registers::ctrlb::type;
		using Data = typename SPIComponent::registers::data;
		using InterruptFlags = typename SPIComponent::registers::intflags;
		
		[[gnu::always_inline]] static inline void init(){
			constexpr auto mBit = SPIComponent::CTRLAMasks::Master ;

			auto& ctra = ControlA::getRegister(spiInf::value().CTRLA);
			auto& ctrb = ControlB::getRegister(spiInf::value().CTRLB);

						AVR::port::PinsOn<typename spiInf::Spi::Mosi::pin0,typename spiInf::Spi::Sck::pin0>();
						//spiInf::mport::getDir().on(spiInf::spi::mosi::pin0,spiInf::spi::Sck::pin0);
						//spiInf::mport::getDir().off(spiInf::pins::miso);
						AVR::port::PinsOff<typename spiInf::Spi::Miso::pin0>();

			ctra.set(prescaler, clockDoubled, order, mBit);
			ctrb.set(transferMode, buffered, waitForReceive,slaveSelectDisable);
			ctra.on(SPIComponent::CTRLAMasks::Enable);
		}
		
		[[nodiscard]] static inline mem_width singleTransmit(mem_width data) {
			auto& datareg = Data::getRegister(spiInf::value().DATA);
			auto& ints = InterruptFlags::getRegister(spiInf::value().INTFLAGS);
			datareg.raw() = data;
			while(! ints.areSet(InterruptFlags::special_bit::ReceiveComplete));
			return datareg.raw();
		}
		
		[[nodiscard]] static inline mem_width singleReceive() {
			auto& datareg = Data::getRegister(spiInf::value().DATA);
			auto& ints = InterruptFlags::getRegister(spiInf::value().INTFLAGS);

			while(! ints.areSet(InterruptFlags::special_bit::ReceiveComplete));
			return datareg.raw();
		}
		
		static inline void singleTransfer(mem_width data) {
			auto& datareg = Data::getRegister(spiInf::value().DATA);
			auto& ints = InterruptFlags::getRegister(spiInf::value().INTFLAGS);
			datareg.raw() = data;
			while(! ints.areSet(InterruptFlags::special_bit::ReceiveComplete));
		}
		
		[[nodiscard]] static inline mem_width* Transmit(mem_width* data, mem_width size) {
			for(mem_width i = 0; i < size; i++){
				data[i] = singleTransmit(data[i]);
			}
			return data;
		}
		
		[[nodiscard]] static inline mem_width* Receive(mem_width* data, mem_width size) {
			for(mem_width i = 0; i < size; i++){
				data[i] = singleReceive();
			}
			return data;
		}
		
		static inline void Transfer(mem_width* data, mem_width size) {
			for(mem_width i = 0; i < size; i++){
				singleTransmit(data[i]);
			}
		}
		
	};

	template<typename SPIComponent, typename spiInf, auto order,
	auto transferMode, auto buffered, auto waitForReceive>
	struct SPISlave {
		using ControlA = typename SPIComponent::CTRLAMasks;
		using ControlB = typename SPIComponent::CTRLBMasks;
		using Data = typename SPIComponent::Data::type;
		using InterruptFlags = typename SPIComponent::InterruptFlags::type;
		NoConstructors(SPISlave);

		static inline void init(){

			auto& ctra = reg::Register<>::getRegister(spiInf::value().CTRLA);
			auto& ctrb = reg::Register<>::getRegister(spiInf::value().CTRLB);
			
			AVR::port::PinsOn<spiInf::spi::mosi::pin0,spiInf::spi::Sck::pin0>();
			//spiInf::mport::getDir().on(spiInf::spi::mosi::pin0,spiInf::spi::Sck::pin0);
			//spiInf::mport::getDir().off(spiInf::pins::miso);
			AVR::port::PinsOff<spiInf::spi::miso::pin0>();
			ctra.set(order);
			ctrb.set(transferMode, buffered, waitForReceive);
			ctra.on(ControlA::Enable);
		}

		[[nodiscard]] static inline mem_width singleTransmit(mem_width data) {
			auto& datareg = Data::getRegister(spiInf::value().DATA);
			auto& ints = InterruptFlags::getRegister(spiInf::value().INTFLAGS);
			datareg.raw() = data;
			while(! ints.areSet(InterruptFlags::special_bit::ReceiveComplete));
			return datareg.raw();
		}

		static inline void singleTransfer(mem_width data) {
			auto& datareg = Data::getRegister(spiInf::value().DATA);
			auto& ints = InterruptFlags::getRegister(spiInf::value().INTFLAGS);
			datareg.raw() = data;
			while(! ints.areSet(InterruptFlags::special_bit::ReceiveComplete));
		}
		
		[[nodiscard]] static inline mem_width singleReceive() {
			auto& datareg = Data::getRegister(spiInf::value().DATA);
			auto& ints = InterruptFlags::getRegister(spiInf::value().INTFLAGS);

			while(! ints.areSet(InterruptFlags::special_bit::ReceiveComplete));
			return datareg.raw();
		}

		[[nodiscard]] static inline mem_width* Receive(mem_width* data, mem_width size) {
			for(mem_width i = 0; i < size; i++){
				data[i] = singleReceive();
			}
			return data;
		}

		[[nodiscard]] static inline mem_width* Transmit(mem_width* data, mem_width size) {
			for(mem_width i = 0; i < size; i++){
				data[i] = singleTransmit(data[i]);
			}
			return data;
		}

		static inline void Transfer(mem_width* data, mem_width size) {
			for(mem_width i = 0; i < size; i++){
				singleTransmit(data[i]);
			}
		}
	};
}