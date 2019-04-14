
/*
* SPI.hpp
*
* Created: 27.02.2019 10:21:27
*  Author: keven
*/
#pragma once
#include "Register.hpp"
#include "Port.hpp"
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
    using Interrupts = typename USART_Comp::Interrupts;

        namespace details{

            template<typename RW, typename accesstype, typename component, typename instance,typename alt, typename setting, typename bit_width>
            class _USART : protected AVR::details::Communication<RW,accesstype, bit_width> {

                static_assert(setting::charsize != static_cast<typename setting::CConf>(CharacterSize::Bit9HighFirst) && setting::charsize != static_cast<typename setting::CConf>(CharacterSize::Bit9LowFirst),
                        "9 bit mode not supported");

                //////////////private fifo transfer/receive methods
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
                    return (((64.0 * fcpu) / (16 * baud)) +0.5) -1;
                }
                inline static constexpr uint16_t ubrrValue2(uint32_t fcpu, uint32_t baud) {
                    return (((64.0 * fcpu) / (8 * baud)) +0.5) -1;
                }

                inline static constexpr uint16_t ubrrValueSync(uint32_t fcpu, uint32_t baud) {
                    return (((64.0 * fcpu) / (2 * baud)) +0.5)-1;
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
                //using RxDataH = typename component::registers::rxdatah;
                using TxDataL = typename component::registers::txdatal;
                //using TxDataH = typename component::registers::txdatah;
                using InterruptFlags = typename component::registers::status;

                static auto constexpr txHelp = [](){if constexpr(! _USART::isReadOnly && _USART::fifoEnabled)transfer();};
                static auto constexpr rxHelp = [](){if constexpr(! _USART::isWriteOnly&& _USART::fifoEnabled)receive();};

            public:

                NoConstructors(_USART);

                template<bool dummy, typename T = std::enable_if_t<! _USART::isReadOnly>>
                [[nodiscard]] static inline bool outputEmpty(){
                    return _USART::fifoOut.empty();
                }

                template<bool dummy, typename T = std::enable_if_t<! _USART::isWriteOnly>>
                [[nodiscard]] static inline bool inputEmpty(){
                    return _USART::fifoOut.empty();
                }

                template<bool dummy, typename T = std::enable_if_t<! _USART::isReadOnly>>
                [[nodiscard]] static inline auto outputSize(){
                    return _USART::fifoOut.size();
                }

                template<bool dummy, typename T = std::enable_if_t<! _USART::isWriteOnly>>
                [[nodiscard]] static inline auto inputSize(){
                    return _USART::fifoOut.size();
                }

                using InterruptFlagBits = typename InterruptFlags::type::special_bit;

                template<uint32_t baud = 115200>
                [[gnu::always_inline]] static inline void init(){
                    static_assert(static_cast<mem_width >(setting::msb) == 0, "msb setting is a master spi option -> currently not supported");
                    static_assert(baud <= (DEFAULT_MCU::clockFrequency/8), "baud rate too high!");

                    if constexpr(! _USART::InterruptEnabled && (static_cast<mem_width >(setting::loopbackmode) | static_cast<mem_width >(setting::rsmode)) != 0)
                        reg<ControlA>().set(setting::loopbackmode, setting::rsmode);
                    else if constexpr(_USART::InterruptEnabled){
                        if constexpr(_USART::isReadOnly){
                            reg<ControlA>().set(Interrupts::RxIE, setting::loopbackmode, setting::rsmode);
                        } else if constexpr(_USART::isWriteOnly){
                            reg<ControlA>().set(Interrupts::TxIE, setting::loopbackmode, setting::rsmode);
                        } else if constexpr (!_USART::isReadOnly && !_USART::isWriteOnly){
                            reg<ControlA>().set(Interrupts::RxIE, Interrupts::TxIE, setting::loopbackmode, setting::rsmode);
                        }
                    }
                    using BConf = typename setting::BConf;

                    if constexpr((static_cast<mem_width >(setting::commode) | static_cast<mem_width >(setting::paritymode) | static_cast<mem_width >(setting::stopbitmode) | static_cast<mem_width >(setting::charsize)) != 0)
                        reg<ControlC >().set(setting::commode , setting::paritymode, setting::stopbitmode, setting::charsize);

                    constexpr auto recMode = (setting::receivermode != static_cast<BConf>(ReceiverMode::Normal)
                                             && setting::receivermode != static_cast<BConf>(ReceiverMode::Double) ?
                                             setting::receivermode : baud > 100000 ?
                                                                     static_cast<BConf>(ReceiverMode::Double) :
                                                                     static_cast<BConf>(ReceiverMode::Normal));

                    if constexpr (setting::receivermode == static_cast<BConf>(ReceiverMode::Normal)
                                  || setting::receivermode == static_cast<BConf>(ReceiverMode::Double)) {
                        if constexpr (baud > 100000) {
                            if constexpr(CommunicationMode::Synchronous != static_cast<CommunicationMode >(setting::commode)){
                                reg<Baud>().raw() = ubrrValue2(DEFAULT_MCU::clockFrequency, baud);
							}
                           else
                                reg<Baud>().raw() = ubrrValueSync(DEFAULT_MCU::clockFrequency, baud);
                        } else {

                            if constexpr(CommunicationMode::Synchronous != static_cast<CommunicationMode>(setting::commode))
                                reg<Baud>().raw() = ubrrValue(DEFAULT_MCU::clockFrequency, baud);
                            else
                                reg<Baud>().raw() = ubrrValueSync(DEFAULT_MCU::clockFrequency, baud);
                        }
                    }

                    if constexpr(_USART::isWriteOnly) {
                        port::PinsDirOut<typename alt::Txd::pin0>();
                        port::PinsOn<typename alt::Txd::pin0>();
                        reg<ControlB>().set(recMode,ControlB::type::special_bit::Txen, setting::opendrainmode, setting::startframedetection, setting::multiprocessor, setting::receivermode);
                    }
                    else if constexpr(_USART::isReadOnly) {
                        port::PinsDirOut<typename alt::Rxd::pin0>();
                        port::PinsOn<typename alt::Rxd::pin0>();
                        reg<ControlB>().set(recMode,ControlB::type::special_bit::Rxen, setting::opendrainmode, setting::startframedetection, setting::multiprocessor, setting::receivermode);
                    }
                    else {
                        port::PinsDirOut<typename alt::Txd::pin0,typename alt::Rxd::pin0>();
                        port::PinsOn<typename alt::Txd::pin0,typename alt::Rxd::pin0>();
                        reg<ControlB>().set(recMode,ControlB::type::special_bit::Txen, ControlB::type::special_bit::Rxen, setting::opendrainmode, setting::startframedetection, setting::multiprocessor, setting::receivermode);
                    }
                    if constexpr (_USART::InterruptEnabled){
                        sei();
                    }
               }

               ///////////////////Interrupthandlers
                template<bool dummy = true, typename T = std::enable_if_t<_USART::InterruptEnabled>>
                static inline void rxHandler(){
                    const auto c = reg<RxDataL>.raw();
                    if constexpr (_USART::FifoEnabled) {
                        static_assert(std::is_same_v<typename accesstype::Adapter, External::Hal::NullProtocollAdapter>, "recvQueue is used, no need for PA");
                        _USART::fifoIn.push_back(c);
                    }
                    else {
                        static_assert(_USART::fifoIn::value == 0);
                        if constexpr(!std::is_same_v<typename accesstype::Adapter, External::Hal::NullProtocollAdapter>) {
                            if (!accesstype::Adapter::process(c)) {
                                if constexpr(Debug) {
#ifdef DBGOUT
                                    dbgout::put("input not handled by protocol adapter!");
                                    dbgout::flush();
#endif
                                }
                            }
                        }
                        else {
                            static_assert(!dummy, "no recvQueue, no PA -> wrong configuration");
                        }
                    }
                }

                template<bool dummy = true, typename T = std::enable_if_t<dummy && _USART::InterruptEnabled && _USART::fifoEnabled>>
                static inline void txHandler(){
                    uint8_t item;
                    if (_USART::fifoOut.pop_front(item)) {
                        reg<TxDataL>().raw() = item;
                    }
                }

                template<bool dummy = true, typename T = std::enable_if_t<dummy && _USART::InterruptEnabled && !_USART::fifoEnabled>>
                static inline void txHandler(const uint8_t data){
                        reg<TxDataL>().raw() = data;
                }

                /////////////no fifo/not blocking transfer/receive methods
                template<bool dummy = true,typename T = std::enable_if_t<dummy && !_USART::fifoEnabled && !_USART::isBlocking && !_USART::isReadOnly>>
                static inline void transfer(bit_width data) {
                    reg<TxDataL>().raw() = data;
                }

                template<bool dummy = true,typename T = std::enable_if_t<dummy && !_USART::fifoEnabled && !_USART::isBlocking && !_USART::isWriteOnly>>
                [[nodiscard]] static inline bit_width receive() {
                    return reg<TxDataL>().raw();
                }

                //////////////blocking receive/transfer methods
                template<typename block = accesstype, typename T = std::enable_if_t< _USART::isBlocking && !_USART::isWriteOnly && ! _USART::fifoEnabled>>
                [[nodiscard]] static inline bit_width receive() {
                    while(! reg<InterruptFlags>().areSet(InterruptFlagBits::Rxcif));
                    return reg<RxDataL>().raw();
                }

                template<typename block = accesstype, typename T = std::enable_if_t<_USART::isBlocking && !_USART::isReadOnly && ! _USART::fifoEnabled>>
                static inline void transfer(bit_width data) {
                    reg<TxDataL>().raw() = data;
                    while(! reg<InterruptFlags>().areSet(InterruptFlagBits::Dreif));
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

                //////////public fifo receive/transfer methods
                template<bool dummy = true,typename T = std::enable_if_t<dummy && _USART::fifoEnabled && !_USART::isReadOnly>>
                static inline bool put(bit_width item) {
                    return _USART::fifoOut.push_back(item);
                }

                template<bool dummy = true,typename T = std::enable_if_t<dummy && _USART::fifoEnabled && !_USART::isWriteOnly>>
                static inline bool get(bit_width& item){
                    return _USART::fifoIn.pop_front(item);
                }

                template<bool dummy = true,typename T = std::enable_if_t<dummy && _USART::fifoEnabled>>
                static inline void periodic(){
                    if constexpr(! _USART::InterruptEnabled) {
                        if constexpr(!_USART::isWriteOnly) doIfSet<rxHelp>(InterruptFlagBits::Rxcif);
                        if constexpr(!_USART::isReadOnly) doIfSet<txHelp>(InterruptFlagBits::Dreif);
                    } else {
                        if constexpr(!_USART::isReadOnly) doIfSet<txHelp>(InterruptFlagBits::Dreif);
                    }
                }

                //convenience method for enabling interrupts
                template<typename... Args>
                requires(utils::sameTypes<Interrupts,Args...>() && _USART::InterruptEnabled)
                static inline void enableInterrupt(Args... Bits) {
                    reg<ControlA>().set(Bits...);
                }

                ///////test/execute methods
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
                StopBitMode stopBitMode = StopBitMode::OneBit, CharacterSize CharSize = CharacterSize::Bit8, bool msb = true,
                bool StartFrameDetection = false, bool OpenDrainMode = false, bool MultiProcessor =false, bool LoopBackMode=false, typename bit_width = mem_width>
        using USART = AVR::usart::details::_USART<
                RW,accesstype, USART_Comp::Component_t,typename instance::t1, typename instance::t2,
                USART_Comp::template USARTSetting<RSMode,receiverMode,ComMode,parityMode,stopBitMode,
                CharSize,msb,StartFrameDetection,OpenDrainMode,MultiProcessor,LoopBackMode>, bit_width>;

}