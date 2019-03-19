
/*
* SPI.hpp
*
* Created: 27.02.2019 10:21:27
*  Author: keven
*/

#include "Register.hpp"
#include "../MCUSelect.hpp"
namespace AVR {
	namespace spi {
		
		namespace details{
			template<typename SPIComponent, typename spiInf, typename bit_width>
			struct SPI {
				using ControlA = typename SPIComponent::registers::ctrla::type;
				using ControlB = typename SPIComponent::registers::ctrlb::type;
				using InterruptControl = typename SPIComponent::registers::intctrl::type;
				using Data = typename SPIComponent::registers::data::type;
				using InterruptFlags = typename SPIComponent::registers::intflags::type;
				using InterruptControlBits = typename InterruptControl::special_bit;
				
				static inline void nonBlockSend(bit_width data){
					auto& datareg = Data::getRegister(spiInf::value().DATA);
					datareg.raw() = data;
				}
				
				[[nodiscard]] static inline bit_width nonBlockReceive(){
					auto& datareg = Data::getRegister(spiInf::value().DATA);
					return datareg.raw();
				}
				
				template<typename... Args>
				requires(utils::sameTypes<InterruptControlBits,Args...>())
				static inline void enableInterrupt(Args... Bits) {
					InterruptControl::getRegister(spiInf::value().INTCTRL).set(Bits...);
				}
				
				[[nodiscard]] static inline bit_width singleTransmit(bit_width data) {
					auto& datareg = Data::getRegister(spiInf::value().DATA);
					auto& ints = InterruptFlags::getRegister(spiInf::value().INTFLAGS);
					datareg.raw() = data;
					while(! ints.areSet(InterruptFlags::special_bit::Default_if));
					return datareg.raw();
				}
				
				[[nodiscard]] static inline bit_width singleReceive() {
					auto& datareg = Data::getRegister(spiInf::value().DATA);
					auto& ints = InterruptFlags::getRegister(spiInf::value().INTFLAGS);

					while(! ints.areSet(InterruptFlags::special_bit::Default_if));
					return datareg.raw();
				}
				
				static inline void singleTransfer(bit_width data) {
					auto& datareg = Data::getRegister(spiInf::value().DATA);
					auto& ints = InterruptFlags::getRegister(spiInf::value().INTFLAGS);
					datareg.raw() = data;
					while(! ints.areSet(InterruptFlags::special_bit::Default_if));
				}
				
				[[nodiscard]] static inline bit_width* Transmit(bit_width* data, bit_width size) {
					for(bit_width i = 0; i < size; i++){
						data[i] = singleTransmit(data[i]);
					}
					return data;
				}
				
				[[nodiscard]] static inline bit_width* Receive(bit_width* data, bit_width size) {
					for(bit_width i = 0; i < size; i++){
						data[i] = singleReceive();
					}
					return data;
				}
				
				static inline void Transfer(bit_width* data, bit_width size) {
					for(bit_width i = 0; i < size; i++){
						singleTransmit(data[i]);
					}
				}
			};
			
			template<typename SPIComponent, typename spiInf, auto order,  auto clockDoubled, auto slaveSelectDisable,
			auto transferMode, auto buffered, auto waitForReceive, auto prescaler, typename bit_width = mem_width>
			struct SPIMaster : public details::SPI<SPIComponent, spiInf, bit_width> {

				NoConstructors(SPIMaster);
				
				[[gnu::always_inline]] static inline void init(){
					constexpr auto mBit = SPIComponent::CTRLAMasks::Master ;

					auto& ctra = SPIMaster::ControlA::getRegister(spiInf::value().CTRLA);
					auto& ctrb = SPIMaster::ControlB::getRegister(spiInf::value().CTRLB);
					
					AVR::port::PinsDirOut<typename spiInf::Spi::Mosi::pin0,typename spiInf::Spi::Sck::pin0>();
					//AVR::port::PinsDirIn<typename spiInf::Spi::Miso::pin0>();
					spiInf::Spi::Miso::pin0::setInput();
					//PORTA.DIR |= (1 <<4) | (1<<6);
					//PORTA.DIR &= ~((1 << 5));
					ctra.set(prescaler, clockDoubled, order, mBit);
					ctrb.set(transferMode, buffered, waitForReceive,slaveSelectDisable);
					ctra.on(SPIMaster::ControlA::special_bit::Enable);
				}
				
			};

			template<typename SPIComponent, typename spiInf, auto order,
			auto transferMode, auto buffered, auto waitForReceive, typename bit_width = mem_width>
			struct SPISlave : public details::SPI<SPIComponent, spiInf, bit_width> {

				NoConstructors(SPISlave);

				[[gnu::always_inline]] static inline void init(){

					auto& ctra = SPISlave::ControlA::getRegister(spiInf::value().CTRLA);
					auto& ctrb = SPISlave::ControlB::getRegister(spiInf::value().CTRLB);
					
					AVR::port::PinsDirIn<typename spiInf::Spi::Mosi::pin0,typename spiInf::Spi::Sck::pin0>();
					AVR::port::PinsDirOut<typename spiInf::Spi::Miso::pin0>();
					
					ctra.set(order);
					ctrb.set(transferMode, buffered, waitForReceive);
					ctra.on(SPISlave::ControlA::special_bit::Enable);
				}
				
			};
		}

		using TransferMode = DEFAULT_MCU::SPI::TransferMode;
		using Prescaler = DEFAULT_MCU::SPI::Prescaler;

		template<bool msb = true, bool clockDouble = true, bool slaveSelectDisable = true, TransferMode tmode = TransferMode::Mode0,
		bool buffered = false,bool waitForReceive = false, Prescaler prescaler = Prescaler::Div4, uint8_t alternative = 0, typename bit_width = mem_width>
		using SPIMaster = typename DEFAULT_MCU::SPI::template SPIMaster<msb,clockDouble,slaveSelectDisable,tmode,buffered,waitForReceive,prescaler,alternative,bit_width>;
		
		template<bool msb = true, TransferMode tmode = TransferMode::Mode0,  bool buffered = false,bool waitForReceive = false, uint8_t alternative = 0, typename bit_width = mem_width>
		using SPISlave = typename DEFAULT_MCU::SPI::template SPISlave<msb,tmode,buffered,waitForReceive,alternative,bit_width>;

	}
}