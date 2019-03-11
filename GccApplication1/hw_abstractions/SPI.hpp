
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
		using ControlA = typename SPIComponent::ControlA::type;
		using ControlB = typename SPIComponent::ControlB::type;
		using Data = typename SPIComponent::Data::type;
		using InterruptFlags = typename SPIComponent::InterruptFlags::type;
		
		[[gnu::always_inline]] static inline void init(){
			constexpr typename ControlA::regSize mBit = 1 << 5;

			auto& ctra = ControlA::getRegister(spiInf::value().CTRLA);
			auto& ctrb = ControlB::getRegister(spiInf::value().CTRLB);

			spiInf::mport::getDir().on(spiInf::pins::mosi,spiInf::pins::sck);
			spiInf::mport::getDir().off(spiInf::pins::miso);

			ctra.set(prescaler, clockDoubled, order, mBit);
			ctrb.set(transferMode, buffered, waitForReceive,slaveSelectDisable);
			ctra.on(ControlA::special_bit::SPIEnable);
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
		using ControlA = typename SPIComponent::ControlA::type;
		using ControlB = typename SPIComponent::ControlB::type;
		using Data = typename SPIComponent::Data::type;
		using InterruptFlags = typename SPIComponent::InterruptFlags::type;
		NoConstructors(SPISlave);

		static inline void init(){

			auto& ctra = reg::Register<>::getRegister(spiInf::value().CTRLA);
			auto& ctrb = reg::Register<>::getRegister(spiInf::value().CTRLB);
			
			spiInf::mport::getDir().on(spiInf::pins::mosi,spiInf::pins::sck);
			spiInf::mport::getDir().off(spiInf::pins::miso);
			ctra.set(order);
			ctrb.set(transferMode, buffered, waitForReceive);
			ctra.on(ControlA::special_bit::SPIEnable);
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