
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
#include "../tools/fifo.h"
//#include "../tools/fifo.h"

namespace AVR {
	namespace spi {
		
	    template<auto N> struct useFifo{static constexpr auto value = N;}; struct noFifo{static constexpr auto value = 0;}; struct blocking {using fifo = noFifo;}; template<typename fifoUse = noFifo> struct notBlocking {using fifo = fifoUse;};
        struct ReadOnly{}; struct WriteOnly{}; struct ReadWrite{};

		namespace details{
			
			template<typename RW, typename accesstype, typename component, typename instance, typename bit_width>
			class SPI {

                using UseFifo = typename accesstype::fifo;
                static constexpr bool fifoEnabled = std::is_same_v<accesstype,notBlocking<useFifo<UseFifo::value>>>;
                static constexpr bool isBlocking = std::is_same_v<accesstype,blocking>;
                static constexpr bool isReadOnly = std::is_same_v<RW,ReadOnly>;
                static constexpr bool isWriteOnly = std::is_same_v<RW,WriteOnly>;

                using fifo_t = std::conditional_t<std::is_same_v<UseFifo,noFifo>,noFifo,etl::FiFo< bit_width,UseFifo::value>>;
                static inline std::conditional_t<isReadOnly,noFifo,fifo_t> fifoOut{};
                static inline std::conditional_t<isWriteOnly,noFifo,fifo_t> fifoIn{};

                template<bool dummy = true,typename T = std::enable_if_t<dummy && fifoEnabled && !isReadOnly>>
                static inline void transfer(){
                    bit_width item;
                    if(fifoOut.pop_front(item)){
                        reg<Data>().raw() = item;
                    }
                }

                template<bool dummy = true,typename T = std::enable_if_t<dummy && fifoEnabled && !isWriteOnly>>
                static inline bool receive() {
                    return fifoIn.push_back(reg<Data>().raw());
                }

                static inline constexpr auto txFunc = [](){ if constexpr(fifoEnabled && !isReadOnly) return transfer();};
                static inline constexpr auto rxFunc = [](){ if constexpr(fifoEnabled && !isWriteOnly) return receive();};

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

                NoConstructors(SPI);

                template<bool dummy = true,typename T = std::enable_if_t<dummy && !fifoEnabled && !isBlocking && !isReadOnly>>
				static inline void transfer(bit_width data)
				requires(std::is_same<notBlocking<>,accesstype>::value) {
					reg<Data>().raw() = data;
				}

				template<bool dummy = true,typename T = std::enable_if_t<dummy && !fifoEnabled && !isBlocking && !isWriteOnly>>
				[[nodiscard]] static inline bit_width receive()
				requires(std::is_same<notBlocking<>,accesstype>::value) {
					return reg<Data>().raw();
				}
				
				template<typename... Args>
				requires(utils::sameTypes<InterruptControlBits,Args...>() && std::is_same<accesstype, notBlocking<>>::value)
				static inline void enableInterrupt(Args... Bits) {
					reg<InterruptControl>().set(Bits...);
				}

				template<auto& funcRef, typename... FlagsToTest>
				requires(utils::sameTypes<InterruptFlagBits, FlagsToTest...>())
				static inline auto doIfSet(FlagsToTest... flags) {
					using retType = decltype(funcRef());
					if (reg<InterruptFlags>().areSet(flags...))
					return funcRef();
                    if constexpr (! std::is_same_v<retType,void>)
					return retType{};
				}
				
				template<auto& funcRef, typename... FlagsToTest>
				requires(utils::sameTypes<InterruptFlagBits, FlagsToTest...>())
				static inline auto doIfAnySet(FlagsToTest... flags) {
					using retType = decltype(funcRef());
					if (reg<InterruptFlags>().anySet(flags...))
					    return funcRef();
					if constexpr (! std::is_same_v<retType,void>)
					return retType{};
				}

				template<typename block = accesstype,typename T = std::enable_if_t<std::is_same<block,blocking>::value&& !(isWriteOnly || isReadOnly)>>
				[[nodiscard]] static inline bit_width transmit(bit_width data) {
					reg<Data>().raw() = data;
					while(! reg<InterruptFlags>().areSet(InterruptFlagBits::Default_if));
					return reg<Data>().raw();
				}

				template<typename block = accesstype, typename T = std::enable_if_t<std::is_same<block,blocking>::value&& !isWriteOnly>>
				[[nodiscard]] static inline bit_width receive() {
					while(! reg<InterruptFlags>().areSet(InterruptFlagBits::Default_if));
					return reg<Data>().raw();
				}

				template<typename block = accesstype, typename T = std::enable_if_t<std::is_same<block,blocking>::value && !isReadOnly>>
				static inline void transfer(bit_width data) {
					reg<Data>().raw() = data;
					while(! reg<InterruptFlags>().areSet(InterruptFlagBits::Default_if));
				}
				
				template<typename block = accesstype, typename T = std::enable_if_t<std::is_same<block,blocking>::value&& !(isReadOnly || isWriteOnly)>>
				[[nodiscard]] static inline bit_width* transmit(bit_width* data, bit_width size) {
					for(bit_width i = 0; i < size; i++){
						data[i] = singleTransmit(data[i]);
					}
					return data;
				}

				template<bool dummy = true,typename T = std::enable_if_t<dummy && !fifoEnabled && !isWriteOnly>>
				[[nodiscard]] static inline bit_width* receive(bit_width* data, bit_width size) {
					for(bit_width i = 0; i < size; i++){
						data[i] = receive();
					}
					return data;
				}

                template<bool dummy = true,typename T = std::enable_if_t<dummy && !fifoEnabled && !isReadOnly>>
				static inline void transfer(bit_width* data, bit_width size) {
					for(bit_width i = 0; i < size; i++){
						transfer(data[i]);
					}
				}

                template<bool dummy = true,typename T = std::enable_if_t<dummy && fifoEnabled && !isReadOnly>>
				static inline bool put(bit_width item){
                    return fifoOut.push_back(item);
				}

                template<bool dummy = true,typename T = std::enable_if_t<dummy && fifoEnabled && !isWriteOnly>>
				static inline bool get(bit_width& item){
				    return fifoIn.pop_front(item);
				}

                template<bool dummy = true,typename T = std::enable_if_t<dummy && fifoEnabled>>
                static inline void periodic(){
                    if constexpr(! isReadOnly)
                    doIfAnySet<txFunc>(InterruptFlagBits::Txcif);
                    if constexpr(! isWriteOnly)
                    doIfAnySet<rxFunc>(InterruptFlagBits::Rxcif);
                }
			};

			template<typename RW, typename accesstype, typename component, typename instance,typename alt, typename Setting, typename bit_width>
			struct SPIMaster : public details::SPI<RW, accesstype,component, instance, bit_width> {
				NoConstructors(SPIMaster);
				
				[[gnu::always_inline]] static inline void init(){

                    constexpr auto mBit = component::CTRLAMasks::Master ;
					AVR::port::PinsDirOut<typename alt::Mosi::pin0,typename alt::Sck::pin0>();
					if constexpr(! std::is_same_v<WriteOnly,RW>)
					alt::Miso::pin0::setInput();
					SPIMaster::template reg<typename SPIMaster::ControlA>().set(Setting::presc, Setting::clkx2, Setting::Msb, mBit);
					SPIMaster::template reg<typename SPIMaster::ControlB>().set(Setting::tmode, Setting::buf, Setting::bufwait,Setting::ssd);
					SPIMaster::template reg<typename SPIMaster::ControlA>().on(SPIMaster::ControlA::type::special_bit::Enable);
				}
				
			};

			template<typename RW, typename accesstype, typename component, typename instance,typename alt,typename Setting, typename bit_width>
			struct SPISlave : public details::SPI<RW, accesstype,component, instance, bit_width> {

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

		template<typename accesstype = blocking,typename instance = details::defInst,typename RW = ReadWrite,bool msb = true, bool clockDouble = true, bool slaveSelectDisable = true, TransferMode tmode = TransferMode::Mode0,
		bool buffered = false,bool waitForReceive = false, Prescaler prescaler = Prescaler::Div4, typename bit_width = mem_width>
		using SPIMaster = AVR::spi::details::SPIMaster<RW,accesstype, SPI_Comp::Component_t,typename instance::t1, typename instance::t2, SPI_Comp::template SPIMasterSetting<msb,clockDouble,slaveSelectDisable,tmode,buffered,waitForReceive,prescaler>, bit_width>;
		
		template<typename accesstype = blocking,typename instance = details::defInst,typename RW = ReadWrite, bool msb = true, TransferMode tmode = TransferMode::Mode0,  bool buffered = false,bool waitForReceive = false, typename bit_width = mem_width>
		using SPISlave = AVR::spi::details::SPISlave<RW,accesstype,typename DEFAULT_MCU::SPI::Component_t,typename instance::t1, typename instance::t2,typename DEFAULT_MCU::SPI::template SPISlaveSetting<msb,tmode,buffered,waitForReceive>,bit_width>;

	}
}