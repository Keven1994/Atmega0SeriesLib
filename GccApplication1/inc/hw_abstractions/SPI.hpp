
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
#include "../tools/concepts.h"
#include "Components.hpp"

namespace AVR {
	namespace spi {

		namespace details{

        template<typename RW, typename accesstype, typename component, typename instance, typename bit_width>
		class _SPI : protected AVR::details::Communication<RW,accesstype, bit_width> {

                template<bool dummy = true,typename T = std::enable_if_t<dummy && _SPI::fifoEnabled && !_SPI::isReadOnly>>
                static inline void transfer(){
                    bit_width item;
                    if(_SPI::fifoOut.pop_front(item)){
                        reg<Data>().raw() = item;
                    }
                }

                template<bool dummy = true,typename T = std::enable_if_t<dummy && _SPI::fifoEnabled && !_SPI::isWriteOnly>>
                static inline bool receive() {
                    return _SPI::fifoIn.push_back(reg<Data>().raw());
                }

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

                static inline void init(){
                    if constexpr(_SPI::InterruptEnabled){
                        constexpr bool both = !_SPI::isReadOnly && !_SPI::isWriteOnly;
                        if constexpr(_SPI::isReadOnly || both){
                            reg<InterruptControl>().on(InterruptControl::special_bit::Rxcie);
                        }
                        if constexpr (_SPI::isWriteOnly || both){
                            //reg<InterruptControl>().on(InterruptControl::special_bit::Txcie);
                        }
                    }
                }



            static auto constexpr txHelp = [](){if constexpr(! _SPI::isReadOnly) transfer();};
            static auto constexpr rxHelp = [](){if constexpr(! _SPI::isWriteOnly) receive();};

            public:

            template<bool dummy = true, typename T = std::enable_if_t<_SPI::InterruptEnabled>>
            static inline void rxHandler(){
                const auto c = reg<Data>.raw();
                if constexpr (_SPI::fifoIn::value > 0) {
                    static_assert(std::is_same_v<typename accesstype::Adapter, External::Hal::NullProtocollAdapter>, "recvQueue is used, no need for PA");
                    _SPI::fifoIn.push_back(c);
                }
                else {
                    static_assert(_SPI::fifoIn::value == 0);
                    if constexpr(!std::is_same_v<typename accesstype::Adapter, External::Hal::NullProtocollAdapter>) {
                        if (!accesstype::Adapter::process(c)) {
#ifdef DEBUG
                            assert("input not handled by protocoll adapter");
#endif
                        }
                    }
                    else {
                        static_assert(!dummy, "no recvQueue, no PA -> wrong configuration");
                    }
                }
            }

            template<bool dummy = true, typename T = std::enable_if_t<_SPI::InterruptEnabled>>
            static inline void txHandler(){
                if (const auto c = _SPI::fifoOut.pop_front()) {
                    reg<Data>.raw() = *c;;
                }
                else {
                    if constexpr(_SPI::InterruptEnabled) {
                        reg<ControlB>().off(ControlB::type::special_bit::Udrie);
                    }
                }
            }

                using InterruptControlBits = typename InterruptControl::type::special_bit;
                using InterruptFlagBits = typename InterruptFlags::type::special_bit;
                NoConstructors(_SPI);

                template<bool dummy = true,typename T = std::enable_if_t<dummy && !_SPI::fifoEnabled && !_SPI::isBlocking && !_SPI::isReadOnly>>
				static inline void transfer(bit_width data) {
					reg<Data>().raw() = data;
				}

				template<bool dummy = true,typename T = std::enable_if_t<dummy && !_SPI::fifoEnabled && !_SPI::isBlocking && !_SPI::isWriteOnly>>
				[[nodiscard]] static inline bit_width receive() {
					return reg<Data>().raw();
				}
				
				template<typename... Args>
				requires(utils::sameTypes<InterruptControlBits,Args...>() && std::is_same<accesstype, notBlocking<>>::value)
				static inline void enableInterrupt(Args... Bits) {
					reg<InterruptControl>().set(Bits...);
				}

				template<auto& funcRef, typename... FlagsToTest>
				requires(utils::sameTypes<InterruptFlagBits, FlagsToTest...>() && etl::Concepts::Callable<decltype(funcRef)>)
				static inline auto doIfSet(FlagsToTest... flags) {
					using retType = decltype(funcRef());
					if (reg<InterruptFlags>().areSet(flags...)) {
                        if constexpr (! std::is_same_v<retType,void>)
                            return funcRef();
							else funcRef();
                    }
                    if constexpr (! std::is_same_v<retType,void>)
					    return retType{};
				}
				
				template<auto& funcRef, typename... FlagsToTest>
				requires(utils::sameTypes<InterruptFlagBits, FlagsToTest...>() && etl::Concepts::Callable<decltype(funcRef)>)
				static inline auto doIfAnySet(FlagsToTest... flags) {
					using retType = decltype(funcRef());
					if (reg<InterruptFlags>().anySet(flags...)) {
                        if constexpr (! std::is_same_v<retType,void>)
                            return funcRef();
							else funcRef();
                    }
					if constexpr (! std::is_same_v<retType,void>)
					    return retType{};
				}

				template<typename block = accesstype,typename T = std::enable_if_t< _SPI::isBlocking && !(_SPI::isWriteOnly || _SPI::isReadOnly)>>
				[[nodiscard]] static inline bit_width transmit(bit_width data) {
					reg<Data>().raw() = data;
					while(! reg<InterruptFlags>().areSet(InterruptFlagBits::If));
					return reg<Data>().raw();
				}

				template<typename block = accesstype, typename T = std::enable_if_t< _SPI::isBlocking && !_SPI::isWriteOnly>>
				[[nodiscard]] static inline bit_width receive() {
					while(! reg<InterruptFlags>().areSet(InterruptFlagBits::If));
					return reg<Data>().raw();
				}

				template<typename block = accesstype, typename T = std::enable_if_t<_SPI::isBlocking && !_SPI::isReadOnly>>
				static inline void transfer(bit_width data) {
					reg<Data>().raw() = data;
					while(! reg<InterruptFlags>().areSet(InterruptFlagBits::If));
				}
				
				template<typename block = accesstype, typename T = std::enable_if_t< _SPI::isBlocking && !(_SPI::isReadOnly || _SPI::isWriteOnly)>>
				[[nodiscard]] static inline bit_width* transmit(bit_width* data, bit_width size) {
					for(bit_width i = 0; i < size; i++){
						data[i] = singleTransmit(data[i]);
					}
					return data;
				}

				template<bool dummy = true,typename T = std::enable_if_t<dummy && !_SPI::fifoEnabled && !_SPI::isWriteOnly>>
				[[nodiscard]] static inline bit_width* receive(bit_width* data, bit_width size) {
					for(bit_width i = 0; i < size; i++){
						data[i] = receive();
					}
					return data;
				}

                template<bool dummy = true,typename T = std::enable_if_t<dummy && !_SPI::fifoEnabled && !_SPI::isReadOnly>>
				static inline void transfer(bit_width* data, bit_width size) {
					for(bit_width i = 0; i < size; i++){
						transfer(data[i]);
					}
				}

                template<bool dummy = true,typename T = std::enable_if_t<dummy && _SPI::fifoEnabled && !_SPI::isReadOnly>>
				static inline bool put(bit_width item){
                    return _SPI::fifoOut.push_back(item);
				}

                template<bool dummy = true,typename T = std::enable_if_t<dummy && _SPI::fifoEnabled && !_SPI::isWriteOnly>>
				static inline bool get(bit_width& item){
				    return _SPI::fifoIn.pop_front(item);
				}

                template<bool dummy = true,typename T = std::enable_if_t<dummy && _SPI::fifoEnabled && !_SPI::InterruptEnabled>>
                static inline void periodic(){
                    if constexpr(! _SPI::isWriteOnly) doIfSet<rxHelp>(InterruptFlagBits::Txcif);
                    if constexpr(! _SPI::isReadOnly) doIfSet<txHelp>(InterruptFlagBits::Rxcif);
                }
			};

			template<typename RW, typename accesstype, typename component, typename instance,typename alt, typename Setting, typename bit_width>
			struct SPIMaster final : public details::_SPI<RW, accesstype,component, instance, bit_width> {
				NoConstructors(SPIMaster);

				//init and enable
				[[gnu::always_inline]] static inline void init(){

                    details::_SPI<RW, accesstype,component, instance, bit_width>::init();

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
			struct SPISlave final : public details::_SPI<RW, accesstype,component, instance, bit_width> {

				NoConstructors(SPISlave);

                //init and enable
				[[gnu::always_inline]] static inline void init(){

                    details::_SPI<RW, accesstype,component, instance, bit_width>::init();

					AVR::port::PinsDirIn<typename alt::Mosi::pin0,typename alt::Sck::pin0>();
                    alt::Miso::pin0::setOutput();
					
					SPISlave::template reg<typename SPISlave::ControlA>().set(Setting::Msb);
					SPISlave::template reg<typename SPISlave::ControlB>().set(Setting::tmode, Setting::buf, Setting::bufwait);
					SPISlave::template reg<typename SPISlave::ControlA>().on(SPISlave::ControlA::type::special_bit::Enable);
				}
				
			};
		}

        using SPI = typename DEFAULT_MCU::SPI;

		using TransferMode = typename SPI::TransferMode;

		using Prescaler = typename SPI::Prescaler;
		
		namespace details{
			using defComponent = AVR::rc::Instance<
			        SPI, // using ressource SPI
			AVR::rc::Number<0>, //using instance '0'
			AVR::portmux::PortMux<0>>; // using portmux 0 alternative
			
			using defRC = rc::RessourceController<defComponent>;
			using defInst = defRC::getRessource_t<defComponent>;
		}

		template<typename accesstype = blocking,typename instance = details::defInst,typename RW = ReadWrite,bool msb = true, bool clockDouble = true, bool slaveSelectDisable = true, TransferMode tmode = TransferMode::Mode0,
		bool buffered = false,bool waitForReceive = false, Prescaler prescaler = Prescaler::Div4, typename bit_width = mem_width>
		using SPIMaster = AVR::spi::details::SPIMaster<RW,accesstype, typename SPI::Component_t,typename instance::t1, typename instance::t2, SPI::template SPIMasterSetting<msb,clockDouble,slaveSelectDisable,tmode,buffered,waitForReceive,prescaler>, bit_width>;
		
		template<typename accesstype = blocking,typename instance = details::defInst,typename RW = ReadWrite, bool msb = true, TransferMode tmode = TransferMode::Mode0,  bool buffered = false,bool waitForReceive = false, typename bit_width = mem_width>
		using SPISlave = AVR::spi::details::SPISlave<RW,accesstype,typename SPI::Component_t,typename instance::t1, typename instance::t2, SPI::template SPISlaveSetting<msb,tmode,buffered,waitForReceive>,bit_width>;

	}
}
