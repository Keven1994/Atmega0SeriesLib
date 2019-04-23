
/*
* TWI.hpp
*
* Created: 22.03.2019 19:50:16
*  Author: keven
*/
#pragma once

#include "Port.hpp"
#include "../MCUSelect.hpp"
#include "RessourceController.hpp"
#include <util/twi.h>
#include "Components.hpp"

namespace AVR {

    namespace twi {

        enum readWrite {
            read, write
        };

        struct blocking {
        };
        struct notBlocking {
        };
        enum access {
            Read, Write
        };

        namespace details {

            template<bool singlemode = false>
            struct Command {
                uint8_t address = 0;
                uint8_t bytes = 0;
                readWrite access = write;

                void operator=(const volatile Command &other) volatile {
                    address = other.address;
                    bytes = other.bytes;
                    access = other.access;
                }
            };

            template<>
            struct Command<true> {
                uint8_t address = 0;
                uint8_t bytes = 0;

                void operator=(const volatile Command &other) volatile {
                    address = other.address;
                    bytes = other.bytes;
                }
            };

            struct NoCommand{};

            template<typename RW, typename accesstype, typename component, typename instance, typename bit_width, bool master>
            struct _TWI : protected AVR::details::Communication<RW, accesstype, bit_width> {
            protected:
                using command = std::conditional_t<!_TWI::fifoEnabled, NoCommand,
                        std::conditional_t<
                        _TWI::isReadOnly || _TWI::isWriteOnly, Command<true>, Command<false>>>;
                static inline command current{};
                using Dbgctrl = typename component::registers::dbgctrl;
                using Bridgectrl = typename component::registers::bridgectrl;
                using Ctrla =  typename component::registers::ctrla;
                using Addr =  std::conditional_t<master, typename component::registers::maddr, typename component::registers::saddrmask>;
                using Baud =  std::conditional_t<master, typename component::registers::mbaud, void>;
                using _Ctrla =  std::conditional_t<master, typename component::registers::mctrla, typename component::registers::sctrla>;
                using _Ctrlb = std::conditional_t<master, typename component::registers::mctrlb, typename component::registers::sctrlb>;
                using Data =  std::conditional_t<master, typename component::registers::mdata, typename component::registers::sdata>;
                using Status =  std::conditional_t<master, typename component::registers::mstatus, typename component::registers::sstatus>;
                using StatusBits = typename Status::type::special_bit;
                using stack_t = std::conditional_t<!_TWI::fifoEnabled,  etl::FiFo<command, 0>, std::conditional_t <!_TWI::InterruptEnabled , etl::FiFo<command, 10>, volatile etl::FiFo<command, 10>>>;
                static inline stack_t CommandStack;

                template<typename Reg>
                [[nodiscard, gnu::always_inline]] static inline auto &reg() {
                    return AVR::port::details::getRegister<Reg, instance::value>();
                }

                template<bool dummy = true, typename T = std::enable_if_t<dummy && _TWI::fifoEnabled>>
                static inline void transfer() {
                    uint8_t item;
                    if (_TWI::fifoOut.pop_front(item)) {
                        current.bytes--;
                        reg<Data>().raw() = item;
                    }
                }

                static inline void receive() {
                    current.bytes--;
                    uint8_t item = reg<Data>().raw();
                    _TWI::fifoIn.push_back(item);
                }

            public:
                ///////test/execute methods
                template<auto &funcRef, typename... FlagsToTest>
                requires (utils::sameTypes<StatusBits, FlagsToTest...>() &&

                etl::Concepts::Callable<decltype(funcRef)>)

                static inline auto doIfSet(FlagsToTest... flags) {
                    using retType = decltype(funcRef());
                    if (reg<Status>().areSet(flags...)) {
                        if constexpr (!std::is_same_v<retType, void>)
                            return funcRef();
                        else funcRef();
                    }
                    if constexpr (!std::is_same_v<retType, void>)
                        return retType{};
                }

                template<auto &funcRef, typename... FlagsToTest>
                requires (utils::sameTypes<StatusBits, FlagsToTest...>() &&

                etl::Concepts::Callable<decltype(funcRef)>)

                static inline auto doIfAnySet(FlagsToTest... flags) {
                    using retType = decltype(funcRef());
                    if (reg<Status>().anySet(flags...)) {
                        if constexpr (!std::is_same_v<retType, void>)
                            return funcRef();
                        else funcRef();
                    }
                    if constexpr (!std::is_same_v<retType, void>)
                        return retType{};
                }

                template<bool dummy = true, typename T = std::enable_if_t<
                        dummy && _TWI::fifoEnabled && !_TWI::isWriteOnly>>
                static inline bool get(bit_width &item) {
                    return _TWI::fifoIn.pop_front(item);
                }

                template<uint8_t address, bool dummy = true, typename T = std::enable_if_t<dummy && _TWI::fifoEnabled> >
                requires (address<(

                1 << 7))

                static inline void put(bit_width data) {
                    if constexpr(_TWI::isWriteOnly)
                        CommandStack.push_back(command{(address << 1), 1});
                    else
                        CommandStack.push_back(command{(address << 1), 1, write});
                    _TWI::fifoOut.push_back(data);
                }

                template<uint8_t address, bool dummy = true, typename T = std::enable_if_t<dummy && _TWI::fifoEnabled> >
                requires (address<(

                1 << 7))

                static inline void put(bit_width *data, uint8_t size) {

                    if constexpr(_TWI::isWriteOnly)
                        CommandStack.push_back(command{(address << 1), size});
                    else
                        CommandStack.push_back(command{(address << 1), size, write});
                    for (uint8_t i = 0; i < size; i++)
                        _TWI::fifoOut.push_back(data[i]);
                }

            };


            template<typename RW, typename accesstype, typename component, typename instance, typename alt, typename Setting, typename bit_width = mem_width>
            class TWIMaster : public _TWI<RW, accesstype, component, instance, bit_width, true> {

                template<bool dummy = true, typename T = std::enable_if_t<dummy && TWIMaster::fifoEnabled>>
                static inline void writeCondition() {
                    TWIMaster::template reg<typename TWIMaster::Addr>().set(TWIMaster::current.address);
                }

                template<uint8_t address>
                static inline void writeCondition() {
                    TWIMaster::template reg<typename TWIMaster::Addr>().set(address << 1);
                }

                template<uint8_t address>
                static inline void readCondition() {
                    TWIMaster::template reg<typename TWIMaster::Addr>().set((address << 1) | TWI_ADDREN_bm);
                }

                template<bool dummy = true, typename T = std::enable_if_t<dummy && TWIMaster::fifoEnabled>>
                static inline void readCondition() {
                    TWIMaster::template reg<typename TWIMaster::Addr>().set(TWIMaster::current.address | TWI_ADDREN_bm);
                }

                static inline void busStateIdle() {
                    TWIMaster::template reg<typename TWIMaster::Status>().toggle(TWIMaster::StatusBits::Busstate_idle);
                }

                static inline void turnOff() {
                    TWIMaster::template reg<typename TWIMaster::_Ctrla>().off(
                            TWIMaster::Mctrla::type::special_bit::Enable);
                }

                static inline void turnOn() {
                    TWIMaster::template reg<typename TWIMaster::_Ctrla>().on(
                            TWIMaster::Mctrla::type::special_bit::Enable);
                }

                static inline void stopTransaction() {
                    TWIMaster::template reg<typename TWIMaster::_Ctrlb>().on(
                            TWIMaster::_Ctrlb::type::special_bit::Mcmd_stop);
                }

                static inline void restart() {
                    TWIMaster::template reg<typename TWIMaster::_Ctrlb>().on(
                            TWIMaster::Mctrlb::special_bit::Mcmd_repstart);
                }


                [[nodiscard]] static constexpr uint8_t baudCalculate(long long targetFrequency, bool fmp , bool khz100 ){
                    if (fmp){
                        return ((DEFAULT_MCU::clockFrequency*1.0/targetFrequency)-10.0 - (DEFAULT_MCU::clockFrequency*1.0*DEFAULT_MCU::riseTime)) / 2.0;
                    } else if(! khz100) {
                        return ((DEFAULT_MCU::clockFrequency*1.0/targetFrequency)-10.0 - (DEFAULT_MCU::clockFrequency*1.0*DEFAULT_MCU::riseTime)) / 2.0;
                    } else {
                        return (((DEFAULT_MCU::clockFrequency*1.0/targetFrequency)-10.0 - (DEFAULT_MCU::clockFrequency*1.0*DEFAULT_MCU::riseTime)) / 2.0 + 0.5)-1.0;
                    }
                }

                template<uint8_t Address, access dir>
                struct ScopedTransaction {
                    const bool transActionSuccess;

                    ScopedTransaction() : transActionSuccess(startTransaction<Address,dir>()){}

                    template<bool Dummy = true, typename T = std::enable_if_t<!TWIMaster::isReadOnly && dir == Write>>
                    bool send(uint8_t Data){
                        return transfer(Data);
                    }

                    template<bool Dummy = true, typename T = std::enable_if_t<!TWIMaster::isWriteOnly && dir == Read>>
                    bool receive(uint8_t& Data){
                        return  TWIMaster::receive(Data);
                    }

                    ~ScopedTransaction(){
                        if(transActionSuccess)
                            stopTransaction();
                    }
                };

            public:

                template<uint8_t address,bool dummy = true, typename T = std::enable_if_t<dummy && !TWIMaster::fifoEnabled && !TWIMaster::isBlocking && ! TWIMaster::isReadOnly>>
                static inline auto scopedWrite(){
                    return ScopedTransaction<address, Write>();
                }

                template<uint8_t address,bool dummy = true, typename T = std::enable_if_t<dummy && !TWIMaster::fifoEnabled && !TWIMaster::isBlocking && ! TWIMaster::isReadOnly>>
                static inline auto scopedRead(){
                    return ScopedTransaction<address, Read>();
                }

                template<uint8_t address,access dir,bool dummy = true, typename T = std::enable_if_t<dummy && !TWIMaster::fifoEnabled && !TWIMaster::isBlocking && ! TWIMaster::isReadOnly>>
                [[nodiscard]] static inline bool startTransaction() requires(address < (1 <<7)) {
                    auto& statereg = TWIMaster::template reg<typename TWIMaster::Status>();
                    using statebits = typename TWIMaster::Status::type::special_bit;

                    if(statereg.areSet(statebits::Busstate_idle)){
                        if constexpr(dir == Write){
                            writeCondition<address>();
                        } else {
                            readCondition<address>();
                        }
                        return true;
                    }
                    return false;
                }

                template<bool dummy = true, typename T = std::enable_if_t<dummy && !TWIMaster::fifoEnabled && !TWIMaster::isBlocking && ! TWIMaster::isReadOnly>>
                [[nodiscard]] static inline bool transfer(uint8_t Data){
                    auto& statereg = TWIMaster::template reg<typename TWIMaster::Status>();
                    using statebits = typename TWIMaster::Status::type::special_bit;

                    if(statereg.areSet(statebits::Busstate_owner, statebits::Wif)){
                        TWIMaster::template reg<typename TWIMaster::Data>().raw() = Data;
                        return true;
                    }
                    return false;
                }

                template<bool dummy = true, typename T = std::enable_if_t<dummy && !TWIMaster::fifoEnabled && !TWIMaster::isBlocking && ! TWIMaster::isWriteOnly>>
                [[nodiscard]] static inline bool receive(uint8_t& Data) {
                    auto& statereg = TWIMaster::template reg<typename TWIMaster::Status>();
                    using statebits = typename TWIMaster::Status::type::special_bit;

                    if(statereg.areSet(statebits::Busstate_owner, statebits::Rif)){
                        Data = TWIMaster::template reg<typename TWIMaster::Data>().raw();
                        return true;
                    }
                    return false;
                }

                template<bool dummy = true, typename T = std::enable_if_t<dummy && !TWIMaster::fifoEnabled && !TWIMaster::isBlocking && ! TWIMaster::isWriteOnly>>
                [[nodiscard]] static inline bool endTransaction() {
                    auto& statereg = TWIMaster::template reg<typename TWIMaster::Status>();
                    using statebits = typename TWIMaster::Status::type::special_bit;

                    if(statereg.areSet(statebits::Busstate_owner)){
                        stopTransaction();
                        return true;
                    }
                    return false;
                }

                template<bool dummy = true, typename T = std::enable_if_t<
                        dummy && TWIMaster::fifoEnabled && !TWIMaster::InterruptEnabled> >
                static inline void periodic() {
                    auto& statereg = TWIMaster::template reg<typename TWIMaster::Status>();
                    using statebits = typename TWIMaster::Status::type::special_bit;

                    if constexpr(TWIMaster::isReadOnly) {
                        if (TWIMaster::current.bytes > 0) {
                            static constexpr auto rxMethod = TWIMaster::receive;
                            TWIMaster::template doIfSet<rxMethod>(TWIMaster::StatusBits::Rif);
                        } else {
                            if (statereg.areSet(statebits::Busstate_idle)) {

                                if (TWIMaster::CommandStack.pop_front(TWIMaster::current)) {
                                    readCondition();
                                }

                            } else if (statereg.areSet(statebits::Busstate_owner,TWIMaster::StatusBits::Wif)) {
                                stopTransaction();
                            }
                        }
                    } else if constexpr(TWIMaster::isWriteOnly) {
                        if (TWIMaster::current.bytes > 0) {
                            static constexpr auto txMethod = TWIMaster::transfer;
                            TWIMaster::template doIfSet<txMethod>(TWIMaster::StatusBits::Wif);
                        } else {
                            if (statereg.areSet(statebits::Busstate_idle)) {

                                if (TWIMaster::CommandStack.pop_front(TWIMaster::current)) {
                                        writeCondition();
                                }

                            } else if (statereg.areSet(statebits::Busstate_owner,TWIMaster::StatusBits::Wif)) {
                                stopTransaction();
                            }
                        }
                    } else if constexpr(!TWIMaster::isWriteOnly && !TWIMaster::isReadOnly) {
                        if (TWIMaster::current.bytes > 0) {
                            static constexpr auto txMethod = TWIMaster::_TWI::transfer;
                            static constexpr auto rxMethod = TWIMaster::_TWI::receive;
                            if (TWIMaster::current.access == write) {
                                TWIMaster::template doIfSet<txMethod>(TWIMaster::StatusBits::Wif);
                            } else {
                                TWIMaster::template doIfSet<rxMethod>(TWIMaster::StatusBits::Rif);
                            }
                        } else {
                            if (statereg.areSet(statebits::Busstate_idle)) {

                                if (TWIMaster::CommandStack.pop_front(TWIMaster::current)) {

                                    if (TWIMaster::current.access == write) {
                                        writeCondition();
                                    } else {
                                        readCondition();
                                    }
                                }

                            } else if (statereg.areSet(statebits::Busstate_owner,TWIMaster::StatusBits::Wif)) {
                                stopTransaction();
                            }
                        }
                    }

                }

                template<bool dummy = true, typename T = std::enable_if_t<dummy && TWIMaster::isBlocking>>
                static inline void transfer(uint8_t data) {
                    TWIMaster::template reg<TWIMaster::Mdata>().set(data);
                    while (!(TWIMaster::template reg<TWIMaster::Mstatus>().areSet(TWIMaster::InterruptFlagBits::Wif)));
                }

                template<bool dummy = true, typename T = std::enable_if_t<dummy && TWIMaster::isBlocking>>
                static inline void transfer(bit_width *data, uint8_t n) {
                    while (--n >= 0)
                        transfer(data[n]);
                }

                template<uint8_t address, bool dummy = true, typename T = std::enable_if_t<dummy && TWIMaster::isBlocking>>
                [[nodiscard]] static inline bit_width receive() {
                    static constexpr uint8_t addr = TWI_ADDREN_bm | (address << 1);

                    TWIMaster::template reg<typename TWIMaster::Maddr>().set(addr);
                    while (!(TWIMaster::template reg<TWIMaster::Mstatus>().areSet(
                            TWIMaster::Mstatus::special_bit::Rif)));
                    return TWIMaster::template reg<TWIMaster::Mdata>().raw();
                }

                template<bool dummy = true, typename T = std::enable_if_t<dummy && TWIMaster::isBlocking>>
                [[nodiscard]] static inline bit_width *receive(bit_width *data, uint8_t n) {
                    for (uint8_t i = 0; i < n; i++)
                        data[i] = receive();

                    return data;
                }
                template<uint8_t address, bool dummy = true, typename T = std::enable_if_t<dummy && TWIMaster::fifoEnabled> >
                requires (address<(1 << 7))
                static inline void put(bit_width *data, uint8_t size){
                    TWIMaster::_TWI::template put<address>(data,size);
                    auto& statereg = TWIMaster::template reg<typename TWIMaster::Status>();
                    using statebits = typename TWIMaster::Status::type::special_bit;
                    if constexpr (TWIMaster::InterruptEnabled) {
                        if (statereg.areSet(statebits::Busstate_idle)) {
                            if (TWIMaster::CommandStack.pop_front(TWIMaster::current)) {
                                writeCondition();
                            }

                        }
                    }
                }

                template<uint8_t address, bool dummy = true, typename T = std::enable_if_t<dummy && TWIMaster::fifoEnabled> >
                requires (address<(1 << 7))
                static inline void get(bit_width data){
                    TWIMaster::_TWI::template get<address>(data);
                    auto& statereg = TWIMaster::template reg<typename TWIMaster::Status>();
                    using statebits = typename TWIMaster::Status::type::special_bit;
                    if constexpr (TWIMaster::InterruptEnabled) {
                        if (statereg.areSet(statebits::Busstate_idle)) {
                            if (TWIMaster::CommandStack.pop_front(TWIMaster::current)) {
                                readCondition();
                            }
                        }
                    }
                }

                template<bool dummy = true, typename T = std::enable_if_t<dummy && TWIMaster::InterruptEnabled && TWIMaster::fifoEnabled && ! TWIMaster::isReadOnly>>
                static inline void intHandler() {
                    auto& statereg = TWIMaster::template reg<typename TWIMaster::Status>();
                    using statebits = typename TWIMaster::Status::type::special_bit;

                    if constexpr(TWIMaster::isReadOnly) {
                        if (TWIMaster::current.bytes > 0) {
                            TWIMaster::_TWI::receive();
                        } else {
                            stopTransaction();
                        }
                    } else if constexpr(TWIMaster::isWriteOnly) {
                        if (TWIMaster::current.bytes > 0) {
                            TWIMaster::_TWI::transfer();
                        } else {
                            stopTransaction();
                        }
                    } else {
                        if (TWIMaster::current.bytes > 0) {

                            if (TWIMaster::current.access == write) {
                                TWIMaster::_TWI::transfer();
                            } else {
                                TWIMaster::_TWI::receive();
                            }
                        } else {
                            stopTransaction();
                        }
                    }
                }

                //initialize and start<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
                template<auto frequency = 100000, bool khz100 = true>
                static inline void init() {

                    static constexpr bool fmp = static_cast<mem_width>(Setting::fastmode) != 0;
                    static_assert(!(khz100&&fmp) , "fmp and 100khz mode cannot combined");
                    static constexpr auto baudrate = baudCalculate(frequency,fmp,khz100);

                    if constexpr(TWIMaster::InterruptEnabled) {

                        if constexpr(TWIMaster::isReadOnly) {
                            TWIMaster::template reg<typename TWIMaster::_Ctrla>().on(TWIMaster::_Ctrla::type::special_bit::Rien);
                        }
                        if constexpr (TWIMaster::isWriteOnly) {
                            TWIMaster::template reg<typename TWIMaster::_Ctrla>().on(TWIMaster::_Ctrla::type::special_bit::Wien);
                        } else {
                            TWIMaster::template reg<typename TWIMaster::_Ctrla>().on(TWIMaster::_Ctrla::type::special_bit::Wien,TWIMaster::_Ctrla::type::special_bit::Rien);
                        }
                    }
                    TWIMaster::template reg<typename TWIMaster::Ctrla>().set(Setting::fastmode, Setting::holdtime,
                                                                             Setting::setuptime);
                    TWIMaster::template reg<typename TWIMaster::Baud>().set(baudrate);
                    TWIMaster::template reg<typename TWIMaster::_Ctrla>().on(Setting::quickcommand, Setting::smartmode,
                                                                              Setting::timeout,
                                                                              TWIMaster::_Ctrla::type::special_bit::Enable);
                    busStateIdle();
                    if constexpr (TWIMaster::InterruptEnabled)
                        sei();
                }
            };


            template<typename RW, typename accesstype, typename component, typename instance, typename alt, typename Setting, typename bit_width = mem_width>
            class TWISlave : public _TWI<RW, accesstype, component, instance, bit_width, true> {

                using Bridgectrl = typename component::registers::bridgectrl;
                using Ctrla =  typename component::registers::ctrla;
                using Dbgctrl = typename component::registers::dbgctrl;
                using Saddr =  typename component::registers::saddr;
                using Saddrmask =  typename component::registers::saddrmask;
                using Sctrla =  typename component::registers::sctrla;
                using Sctrlb =  typename component::registers::sctrlb;
                using Sdata =  typename component::registers::sdata;
                using Sstatus =  typename component::registers::sstatus;
                using status_bits = typename component::registers::sstatus::type::special_bit;

            public:

                template<uint8_t address>
                requires (address<(

                1<<7))

                static inline void init() {
                    alt::Scl::pin0::setInput();
                    TWISlave::template reg<Ctrla>().set(Setting::fastmode, Setting::holdtime, Setting::setuptime);
                    TWISlave::template reg<Saddr>().set(address << 1);
                }

                /*template<bool pinSetting = true>*/
                [[nodiscard]] static inline bit_width singleReceive() {
                    auto &statusreg = TWISlave::template reg<Sstatus>();
                    /*if constexpr(pinSetting){
                        alt::Sda::pin0::setInput();
                    }*/
                    while (!(statusreg.areSet(status_bits::Dif) && !statusreg.areSet(status_bits::Dir)));
                    return TWISlave::template reg<Sdata>.raw();
                }

                template</*bool pinSetting = true,*/ uint8_t address>
                [[nodiscard]] static inline bit_width singleTransfer() {
                    auto &statusreg = TWISlave::template reg<Sstatus>();
                    /*if constexpr(pinSetting){
                        alt::Sda::pin0::setInput();
                    }*/
                    while (!(statusreg.areSet(status_bits::Dif) && !statusreg.areSet(status_bits::Dir)));
                    return TWISlave::template reg<Sdata>.raw();
                }
            };
        }

        using TWI = typename DEFAULT_MCU::TWI;

        template<typename mcu= DEFAULT_MCU>
        using Basic_SDAHold =  typename mcu::TWI::SDAHold;
        using SDAHold = Basic_SDAHold<>;

        template<typename mcu= DEFAULT_MCU>
        using Basic_SDASetup = typename mcu::TWI::SDASetup;
        using SDASetup = Basic_SDASetup<>;

        template<typename mcu= DEFAULT_MCU>
        using Basic_MasterTimeout = typename mcu::TWI::MasterTimeout;
        using MasterTimeout = Basic_MasterTimeout<>;

        namespace details {
            using defComponent = AVR::rc::Instance<
                    TWI, // using ressource SPI
                    AVR::rc::Number<0>, //using instance '0'
                    AVR::portmux::PortMux<0>>; // using portmux 0 alternative

            using defRC = rc::RessourceController<defComponent>;
            using defInst = typename defRC::getRessource<defComponent>::type;
        }

        template<typename accesstype = blocking, typename instance = details::defInst, typename RW = AVR::ReadWrite, bool fastModePlus = false, SDAHold holdTime = SDAHold::Setup4Cycles, SDASetup sdaSetup = SDASetup::SDASetup_300ns,
                bool quickCommand = true, bool smartMode = true, MasterTimeout timeOut = MasterTimeout::Disabled, typename bit_width = mem_width>
        using TWIMaster = AVR::twi::details::TWIMaster<RW, accesstype, TWI::Component_t, typename instance::t1, typename instance::t2, TWI::template TWIMasterSetting<fastModePlus, holdTime, sdaSetup, quickCommand, smartMode, timeOut>, bit_width>;

        template<typename accesstype = blocking, typename instance = details::defInst, typename RW = AVR::ReadWrite, bool fastModePlus = false, SDAHold holdTime = SDAHold::Setup4Cycles, SDASetup sdaSetup = SDASetup::SDASetup_300ns, typename bit_width = mem_width>
        using TWISlave = AVR::twi::details::TWISlave<RW, accesstype, TWI::Component_t, typename instance::t1, typename instance::t2, TWI::template TWISlaveSetting<fastModePlus, holdTime, sdaSetup>, bit_width>;

    }

}
