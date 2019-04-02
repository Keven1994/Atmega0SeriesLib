
/*
* SPI.hpp
*
* Created: 27.02.2019 10:21:27
*  Author: keven
*/
#pragma once
#include "Register.hpp"
#include "../MCUSelect.hpp"
#include "RessourceController.hpp"

namespace AVR {
	namespace spi {
		
		struct blocking {}; struct notBlocking {};

		namespace details{
			
			template<typename accesstype, typename component, typename instance, typename bit_width>
			struct SPI {

				NoConstructors(SPI);
				
				protected:
				
				template<typename Reg>
				[[nodiscard,gnu::always_inline]] static inline auto& reg(){
					return AVR::port::details::getRegister<Reg,instance::value>();
				}
				
				using ControlA = typename component::registers::ctrla;
				using ControlB = typename component::registers::ctrlb;
				using InterruptControl = typename component::registers::intctrl;
				using Data = typename component::registers::data;
				using InterruptFlags = typename component::registers::intflags;
				using InterruptControlBits = typename InterruptControl::type::special_bit;
				using InterruptFlagBits = typename InterruptFlags::type::special_bit;
				
				public:

				template<typename block = accesstype, utils::enable_if_t<utils::isEqual<block,notBlocking>::value, int> = 0>
				static inline void tranfer(bit_width data)
				requires(utils::isEqual<notBlocking,accesstype>::value) {
					reg<Data>().raw() = data;
				}

				template<typename block = accesstype,utils::enable_if_t<utils::isEqual<block,notBlocking>::value, int> = 0>
				[[nodiscard]] static inline bit_width receive()
				requires(utils::isEqual<notBlocking,accesstype>::value) {
					return reg<Data>().raw();
				}
				
				template<typename... Args>
				requires(utils::sameTypes<InterruptControlBits,Args...>() && utils::isEqual<accesstype, notBlocking>::value)
				static inline void enableInterrupt(Args... Bits) {
					reg<InterruptControl>().set(Bits...);
				}

				template<auto& funcRef, typename... FlagsToTest>
				requires(utils::sameTypes<InterruptFlagBits, FlagsToTest...>())
				static inline auto doIfSet(FlagsToTest... flags) {
					using retType = decltype(funcRef());
					if (reg<InterruptFlags>().areSet(flags...))
					return funcRef();
					return retType{};
				}
				
				template<auto& funcRef, typename... FlagsToTest>
				requires(utils::sameTypes<InterruptFlagBits, FlagsToTest...>())
				static inline auto doIfAnySet(FlagsToTest... flags) {
					using retType = decltype(funcRef());
					if (reg<InterruptFlags>().anySet(flags...))
					return funcRef();
					return retType{};
				}

				template<typename block = accesstype, utils::enable_if_t<utils::isEqual<block,blocking>::value, int> = 0>
				[[nodiscard]] static inline bit_width transmit(bit_width data) {
					reg<Data>().raw() = data;
					while(! reg<InterruptFlags>().areSet(InterruptFlagBits::Default_if));
					return reg<Data>().raw();
				}

				template<typename block = accesstype, utils::enable_if_t<utils::isEqual<block,blocking>::value, int> = 0>
				[[nodiscard]] static inline bit_width receive() {
					while(! reg<InterruptFlags>().areSet(InterruptFlagBits::Default_if));
					return reg<Data>().raw();
				}

				template<typename block = accesstype, utils::enable_if_t<utils::isEqual<block,blocking>::value, int> = 0>
				static inline void transfer(bit_width data) {
					reg<Data>().raw() = data;
					while(! reg<InterruptFlags>().areSet(InterruptFlagBits::Default_if));
				}
				
				template<typename block = accesstype, utils::enable_if_t<utils::isEqual<block,blocking>::value, int> = 0>
				[[nodiscard]] static inline bit_width* transmit(bit_width* data, bit_width size) {
					for(bit_width i = 0; i < size; i++){
						data[i] = singleTransmit(data[i]);
					}
					return data;
				}
				
				[[nodiscard]] static inline bit_width* receive(bit_width* data, bit_width size) {
					for(bit_width i = 0; i < size; i++){
						data[i] = receive();
					}
					return data;
				}
				
				static inline void transfer(bit_width* data, bit_width size) {
					for(bit_width i = 0; i < size; i++){
						transfer(data[i]);
					}
				}
			};
			
			template<typename accesstype, typename component, typename instance,typename alt, typename Setting, typename bit_width>
			struct SPIMaster : public details::SPI<accesstype,component, instance, bit_width> {
				NoConstructors(SPIMaster);
				
				[[gnu::always_inline]] static inline void init(){
					constexpr auto mBit = component::CTRLAMasks::Master ;
					
					AVR::port::PinsDirOut<typename alt::Mosi::pin0,typename alt::Sck::pin0>();
					alt::Miso::pin0::setInput();
					SPIMaster::template reg<typename SPIMaster::ControlA>().set(Setting::presc, Setting::clkx2, Setting::Msb, mBit);
					SPIMaster::template reg<typename SPIMaster::ControlB>().set(Setting::tmode, Setting::buf, Setting::bufwait,Setting::ssd);
					SPIMaster::template reg<typename SPIMaster::ControlA>().on(SPIMaster::ControlA::type::special_bit::Enable);
				}
				
			};

			template<typename accesstype, typename component, typename instance,typename alt,typename Setting, typename bit_width>
			struct SPISlave : public details::SPI<accesstype,component, instance, bit_width> {

				NoConstructors(SPISlave);

				[[gnu::always_inline]] static inline void init(){
					AVR::port::PinsDirIn<typename alt::Mosi::pin0,typename instance::Spi::Sck::pin0>();
					AVR::port::PinsDirOut<typename alt::Miso::pin0>();
					
					SPISlave::template reg<typename SPISlave::ControlA>().set(Setting::Msb);
					SPISlave::template reg<typename SPISlave::ControlB>().set(Setting::tmode, Setting::buf, Setting::bufwait);
					SPISlave::template reg<typename SPISlave::ControlA>().on(SPISlave::ControlA::special_bit::Enable);
				}
				
			};
		}

		template<typename mcu = DEFAULT_MCU>
		using Basic_TransferMode =  typename mcu::SPI::TransferMode;
		using TransferMode = Basic_TransferMode<>;
		
		template<typename mcu = DEFAULT_MCU>
		using Basic_Prescaler = typename mcu::SPI::Prescaler;
		using Prescaler = Basic_Prescaler<>;
		
		template<typename mcu = DEFAULT_MCU>
		using Basic_SPI_Comp = typename mcu::SPI;
		using SPI_Comp = Basic_SPI_Comp<>;
		
		namespace details{
			using defComponent = AVR::rc::Instance<
			SPI_Comp, // using ressource SPI
			AVR::rc::Number<0>, //using instance '0'
			AVR::portmux::PortMux<0>>; // using portmux 0 alternative
			
			using defRC = rc::RessourceController<defComponent>;
			using defInst = typename defRC::getRessource<defComponent>::type;
		}

		template<typename accesstype = blocking,typename instance = details::defInst,bool msb = true, bool clockDouble = true, bool slaveSelectDisable = true, TransferMode tmode = TransferMode::Mode0,
		bool buffered = false,bool waitForReceive = false, Prescaler prescaler = Prescaler::Div4, typename bit_width = mem_width>
		using SPIMaster = AVR::spi::details::SPIMaster<accesstype, SPI_Comp::Component_t,typename instance::t1, typename instance::t2, SPI_Comp::template SPIMasterSetting<msb,clockDouble,slaveSelectDisable,tmode,buffered,waitForReceive,prescaler>, bit_width>;
		
		template<typename accesstype = blocking,typename instance = details::defInst, bool msb = true, TransferMode tmode = TransferMode::Mode0,  bool buffered = false,bool waitForReceive = false, typename bit_width = mem_width>
		using SPISlave = AVR::spi::details::SPISlave<accesstype,typename DEFAULT_MCU::SPI::Component_t,typename instance::t1, typename instance::t2,typename DEFAULT_MCU::SPI::template SPISlaveSetting<msb,tmode,buffered,waitForReceive>,bit_width>;

	}
}