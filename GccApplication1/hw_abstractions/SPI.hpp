
/*
* SPI.hpp
*
* Created: 27.02.2019 10:21:27
*  Author: keven
*/
#pragma once
#include "Register.hpp"
#include "../MCUSelect.hpp"
namespace AVR {
	namespace spi {
		
		struct blocking {}; struct notBlocking {};

		namespace details{
			template<typename accesstype, typename component, typename instance, typename bit_width>
			struct SPI {
				
				NoConstructors(SPI);
				
				using ControlA = typename component::registers::ctrla::type;
				using ControlB = typename component::registers::ctrlb::type;
				using InterruptControl = typename component::registers::intctrl::type;
				using Data = typename component::registers::data::type;
				using InterruptFlags = typename component::registers::intflags::type;
				using InterruptControlBits = typename InterruptControl::special_bit;
				using InterruptFlagBits = typename InterruptFlags::special_bit;
				
				static inline void noneBlockSend(bit_width data)		
				requires(utils::isEqual<notBlocking,accesstype>::value)		
				{
					auto& datareg = Data::getRegister(instance::value().DATA);
					datareg.raw() = data;
				}
				
				[[nodiscard]] static inline bit_width noneBlockReceive() 
				requires(utils::isEqual<notBlocking,accesstype>::value)
				{
					auto& datareg = Data::getRegister(instance::value().DATA);
					return datareg.raw();
				}
				
				template<typename... Args>
				requires(utils::sameTypes<InterruptControlBits,Args...>() && utils::isEqual<accesstype, notBlocking>::value)
				static inline void enableInterrupt(Args... Bits) {
					InterruptControl::getRegister(instance::value().INTCTRL).set(Bits...);
				}

				template<auto& funcRef, typename... FlagsToTest>
				requires(utils::sameTypes<InterruptFlagBits, FlagsToTest...>())
				static inline decltype(funcRef()) doIfTest(FlagsToTest... flags) {
					using retType = decltype(funcRef());
					if (InterruptFlags::getRegister(instance::value().INTFLAGS).areSet(flags...))
						return funcRef();
					return retType{};
				}
				
				[[nodiscard]] static inline bit_width singleTransmit(bit_width data) {
					auto& datareg = Data::getRegister(instance::value().DATA);
					auto& ints = InterruptFlags::getRegister(instance::value().INTFLAGS);
					datareg.raw() = data;
					while(! ints.areSet(InterruptFlags::special_bit::Default_if));
					return datareg.raw();
				}
				
				[[nodiscard]] static inline bit_width singleReceive() {
					auto& datareg = Data::getRegister(instance::value().DATA);
					auto& ints = InterruptFlags::getRegister(instance::value().INTFLAGS);

					while(! ints.areSet(InterruptFlags::special_bit::Default_if));
					return datareg.raw();
				}
				
				static inline void singleTransfer(bit_width data) {
					auto& datareg = Data::getRegister(instance::value().DATA);
					auto& ints = InterruptFlags::getRegister(instance::value().INTFLAGS);
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
			
			template<typename accesstype, typename component, typename instance, typename Setting, typename bit_width>
			struct SPIMaster : public details::SPI<accesstype,component, instance, bit_width> {

				NoConstructors(SPIMaster);
				
				[[gnu::always_inline]] static inline void init(){
					constexpr auto mBit = component::CTRLAMasks::Master ;

					auto& ctra = SPIMaster::ControlA::getRegister(instance::value().CTRLA);
					auto& ctrb = SPIMaster::ControlB::getRegister(instance::value().CTRLB);
					
					AVR::port::PinsDirOut<typename instance::Spi::Mosi::pin0,typename instance::Spi::Sck::pin0>();
					instance::Spi::Miso::pin0::setInput();
					ctra.set(Setting::presc, Setting::clkx2, Setting::Msb, mBit);
					ctrb.set(Setting::tmode, Setting::buf, Setting::bufwait,Setting::ssd);
					ctra.on(SPIMaster::ControlA::special_bit::Enable);
				}
				
			};

			template<typename accesstype, typename component, typename instance,typename Setting, typename bit_width>
			struct SPISlave : public details::SPI<accesstype,component, instance, bit_width> {

				NoConstructors(SPISlave);

				[[gnu::always_inline]] static inline void init(){

					auto& ctra = SPISlave::ControlA::getRegister(instance::value().CTRLA);
					auto& ctrb = SPISlave::ControlB::getRegister(instance::value().CTRLB);
					
					AVR::port::PinsDirIn<typename instance::Spi::Mosi::pin0,typename instance::Spi::Sck::pin0>();
					AVR::port::PinsDirOut<typename instance::Spi::Miso::pin0>();
					
					ctra.set(Setting::Msb);
					ctrb.set(Setting::tmode, Setting::buf, Setting::bufwait);
					ctra.on(SPISlave::ControlA::special_bit::Enable);
				}
				
			};
		}

		using TransferMode =  typename DEFAULT_MCU::SPI::TransferMode;
		using Prescaler = typename DEFAULT_MCU::SPI::Prescaler;
		using Spis = typename DEFAULT_MCU::SPI::Components;

		template<typename accesstype = blocking,typename instance = Spis::spi0,bool msb = true, bool clockDouble = true, bool slaveSelectDisable = true, TransferMode tmode = TransferMode::Mode0,
		bool buffered = false,bool waitForReceive = false, Prescaler prescaler = Prescaler::Div16, typename bit_width = mem_width>
		using SPIMaster = AVR::spi::details::SPIMaster<accesstype,typename DEFAULT_MCU::SPI::Component,instance, typename DEFAULT_MCU::SPI::template SPIMasterSetting<msb,clockDouble,slaveSelectDisable,tmode,buffered,waitForReceive,prescaler>, bit_width>;
		
		template<typename accesstype = blocking,typename instance = Spis::spi0, bool msb = true, TransferMode tmode = TransferMode::Mode0,  bool buffered = false,bool waitForReceive = false, typename bit_width = mem_width>
		using SPISlave = AVR::spi::details::SPISlave<accesstype,typename DEFAULT_MCU::SPI::Component,instance,typename DEFAULT_MCU::SPI::template SPISlaveSetting<msb,tmode,buffered,waitForReceive>,bit_width>;

	}
}