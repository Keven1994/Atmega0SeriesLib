
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
#include "Components.hpp"
#include "Basics.hpp"

namespace AVR::usart {


    using USART_Comp = typename DEFAULT_MCU::USART;

    using RS485Mode = typename USART_Comp::RS485Mode;
    using ReceiverMode = typename USART_Comp::ReceiverMode;
    using CommunicationMode = typename USART_Comp::CommunicationMode;
    using StopBitMode = typename USART_Comp::StopBitMode;
    using ParityMode = typename USART_Comp::ParityMode;
    using CharacterSize = typename USART_Comp::CharacterSize;

        namespace details{

            template<typename RW, typename accesstype, typename component, typename instance,typename alt, typename setting, typename bit_width>
            class _USART : protected AVR::details::Communication<RW,accesstype, bit_width> {

                template<bool dummy = true,typename T = std::enable_if_t<dummy && _USART::fifoEnabled && !_USART::isReadOnly>>
                static inline void transfer(){
                    bit_width item;
                    if(_USART::fifoOut.pop_front(item)){
                        reg<TxDataL>().raw() = item;
                    }
                }

                template<bool dummy = true,typename T = std::enable_if_t<dummy && _USART::fifoEnabled && !_USART::isWriteOnly>>
                static inline bool receive() {
                    return _USART::fifoIn.push_back(reg<RxDataL>().raw());
                }

                inline static constexpr uint16_t ubrrValue(uint32_t fcpu, uint32_t baud) {
                    return (((1.0 * fcpu) / (16 * baud)) + 0.5) - 1;
                }
                inline static constexpr uint16_t ubrrValue2(uint32_t fcpu, uint32_t baud) {
                    return (((1.0 * fcpu) / (8 * baud)) + 0.5) - 1;
                }

            protected:

                template<typename Reg>
                [[nodiscard,gnu::always_inline]] static inline auto& reg(){
                    return AVR::port::details::getRegister<Reg,instance::value>();
                }

                using ControlA = typename component::registers::ctrla;
                using ControlB = typename component::registers::ctrlb;
                using ControlC = typename component::registers::ctrlc;
                using Baud = typename component::registers::baud;
                using RxDataL = typename component::registers::rxdatal;
                using RxDataH = typename component::registers::rxdatah;
                using TxDataL = typename component::registers::txdatal;
                using TxDataH = typename component::registers::txdatah;
                using InterruptFlags = typename component::registers::status;

                template<uint32_t baud = 115200>
                [[gnu::always_inline]] static inline void init(){

                    if constexpr (setting::receivermode == ReceiverMode::Normal
                    || setting::receivermode == ReceiverMode::Double) {
                        if constexpr (baud > 100000) {
                            reg<ControlB>().on(ReceiverMode::Double);
                            reg<Baud>().raw() = ubrrValue2(DEFAULT_MCU::clockFrequency, baud);
                        } else {
                            constexpr auto ubrr = ubrrValue(DEFAULT_MCU::clockFrequency, baud);
                            reg<Baud>().raw() = ubrrValue(DEFAULT_MCU::clockFrequency, baud);
                        }
                    } else {
                        reg<ControlB>().on(setting::receivermode);
                    }
                    if constexpr(_USART::InterruptEnabled){
                        constexpr bool both = !_USART::isReadOnly && !_USART::isWriteOnly;
                        if constexpr(_USART::isReadOnly || both){
                           // reg<InterruptControl>().on(InterruptControl::special_bit::Rxcie);
                        }
                        if constexpr (_USART::isWriteOnly || both){
                            //reg<InterruptControl>().on(InterruptControl::special_bit::Txcie);
                        }
                    }
                }

                static auto constexpr txHelp = [](){transfer();};
                static auto constexpr rxHelp = [](){receive();};

            public:

                template<bool dummy = true, typename T = std::enable_if_t<_USART::InterruptEnabled>>
                static inline void rxHandler(){
                    const auto c = reg<RxDataL>.raw();
                    if constexpr (_USART::fifoIn::value > 0) {
                        static_assert(std::is_same_v<typename accesstype::Adapter, External::Hal::NullProtocollAdapter>, "recvQueue is used, no need for PA");
                        _USART::fifoIn.push_back(c);
                    }
                    else {
                        static_assert(_USART::fifoIn::value == 0);
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

                template<bool dummy = true, typename T = std::enable_if_t<_USART::InterruptEnabled>>
                static inline void txHandler(){
                    if (const auto c = _USART::fifoOut.pop_front()) {
                        reg<TxDataL>.raw() = *c;;
                    }
                    else {
                        if constexpr(_USART::InterruptEnabled) {
                            reg<ControlB>().off(ControlB::type::special_bit::Udrie);
                        }
                    }
                }

                using InterruptFlagBits = typename InterruptFlags::type::special_bit;
                NoConstructors(_USART);

                template<bool dummy = true,typename T = std::enable_if_t<dummy && !_USART::fifoEnabled && !_USART::isBlocking && !_USART::isReadOnly>>
                static inline void transfer(bit_width data) {
                    reg<RxDataL>().raw() = data;
                }

                template<bool dummy = true,typename T = std::enable_if_t<dummy && !_USART::fifoEnabled && !_USART::isBlocking && !_USART::isWriteOnly>>
                [[nodiscard]] static inline bit_width receive() {
                    return reg<TxDataL>().raw();
                }

                /*
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
                    }
                    if constexpr (! std::is_same_v<retType,void>)
                        return retType{};
                }*/


                template<typename block = accesstype, typename T = std::enable_if_t< _USART::isBlocking && !_USART::isWriteOnly>>
                [[nodiscard]] static inline bit_width receive() {
                    while(! reg<InterruptFlags>().areSet(InterruptFlagBits::If));
                    return reg<RxDataL>().raw();
                }

                template<typename block = accesstype, typename T = std::enable_if_t<_USART::isBlocking && !_USART::isReadOnly>>
                static inline void transfer(bit_width data) {
                    reg<TxDataL>().raw() = data;
                    while(! reg<InterruptFlags>().areSet(InterruptFlagBits::If));
                }


                template<bool dummy = true,typename T = std::enable_if_t<dummy && !_USART::fifoEnabled && !_USART::isWriteOnly>>
                [[nodiscard]] static inline bit_width* receive(bit_width* data, bit_width size) {
                    for(bit_width i = 0; i < size; i++){
                        data[i] = receive();
                    }
                    return data;
                }

                template<bool dummy = true,typename T = std::enable_if_t<dummy && !_USART::fifoEnabled && !_USART::isReadOnly>>
                static inline void transfer(bit_width* data, bit_width size) {
                    for(bit_width i = 0; i < size; i++){
                        transfer(data[i]);
                    }
                }

                template<bool dummy = true,typename T = std::enable_if_t<dummy && _USART::fifoEnabled && !_USART::isReadOnly>>
                static inline bool put(bit_width item){
                    return _USART::fifoOut.push_back(item);
                }

                template<bool dummy = true,typename T = std::enable_if_t<dummy && _USART::fifoEnabled && !_USART::isWriteOnly>>
                static inline bool get(bit_width& item){
                    return _USART::fifoIn.pop_front(item);
                }

                template<bool dummy = true,typename T = std::enable_if_t<dummy && _USART::fifoEnabled && !_USART::InterruptEnabled>>
                static inline void periodic(){
                    //if constexpr(! _USART::isReadOnly) doIfSet<rxHelp>(InterruptFlagBits::Txcif);
                    //if constexpr(! _USART::isWriteOnly) doIfSet<txHelp>(InterruptFlagBits::Rxcif);
                }
            };

        }

        namespace details{
            using defComponent = AVR::rc::Instance<
                    USART_Comp, // using ressource SPI
                    AVR::rc::Number<0>, //using instance '0'
                    AVR::portmux::PortMux<0>>; // using portmux 0 alternative

            using defRC = rc::RessourceController<defComponent>;
            using defInst = typename defRC::getRessource<defComponent>::type;
        }

        template<typename accesstype = notBlocking<UseFifo<42>>,typename instance = details::defInst,typename RW = ReadWrite, RS485Mode RSMode = RS485Mode::Disabled, ReceiverMode receiverMode = ReceiverMode::Normal,
                CommunicationMode ComMode = CommunicationMode::Asynchronous, ParityMode parityMode = ParityMode::Disabled ,
                StopBitMode stopBitMode = StopBitMode::OneBit, CharacterSize CharSize = CharacterSize::Bit8,
                bool StartFrameDetection = false, bool OpenDrainMode = false, bool MultiProcessor =false, bool LoopBackMode=false, typename bit_width = mem_width>
        using USART = AVR::usart::details::_USART<
                RW,accesstype, USART_Comp::Component_t,typename instance::t1, typename instance::t2,
                USART_Comp::template USARTSetting<RSMode,receiverMode,ComMode,parityMode,stopBitMode,
                CharSize,StartFrameDetection,OpenDrainMode,MultiProcessor,LoopBackMode>, bit_width>;

}