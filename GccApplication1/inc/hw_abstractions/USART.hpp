
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

namespace AVR::usart {

        namespace details{

            template<typename RW, typename accesstype, typename component, typename instance,typename setting, typename bit_width>
            class USART : protected AVR::details::Communication<RW,accesstype, bit_width> {

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

            static inline constexpr auto txFunc = [](){ if constexpr(_SPI::fifoEnabled && !_SPI::isReadOnly) return transfer();};
            static inline constexpr auto rxFunc = [](){ if constexpr(_SPI::fifoEnabled && !_SPI::isWriteOnly) return receive();};

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


            public:
            using InterruptControlBits = typename InterruptControl::type::special_bit;
            using InterruptFlagBits = typename InterruptFlags::type::special_bit;
            NoConstructors(USART);

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

            template<bool dummy = true,typename T = std::enable_if_t<dummy && _SPI::fifoEnabled>>
            static inline void periodic(){
                if constexpr(! _SPI::isReadOnly) txFunc();
                if constexpr(! _SPI::isWriteOnly) rxFunc();
            }
        };

        }

        using USART = typename DEFAULT_MCU::USART;



        namespace details{
            using defComponent = AVR::rc::Instance<
                    USART_Comp, // using ressource SPI
                    AVR::rc::Number<0>, //using instance '0'
                    AVR::portmux::PortMux<0>>; // using portmux 0 alternative

            using defRC = rc::RessourceController<defComponent>;
            using defInst = typename defRC::getRessource<defComponent>::type;
        }

        template<typename accesstype = blocking,typename instance = details::defInst,typename RW = ReadWrite,bool msb = true, bool clockDouble = true, bool slaveSelectDisable = true, TransferMode tmode = TransferMode::Mode0,
                bool buffered = false,bool waitForReceive = false, Prescaler prescaler = Prescaler::Div4, typename bit_width = mem_width>
        using USART = AVR::usart::details::USART<RW,accesstype, USART_Comp::Component_t,typename instance::t1, typename instance::t2, SPI_Comp::template SPIMasterSetting<msb,clockDouble,slaveSelectDisable,tmode,buffered,waitForReceive,prescaler>, bit_width>;

}