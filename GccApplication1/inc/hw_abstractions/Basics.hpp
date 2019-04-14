//
// Created by keven on 04.04.2019.
//

#pragma once

#include <type_traits>
#include "../MCUSelect.hpp"
#include "../tools/concepts.h"
#include "../external/hal/protocolladapter.h"


namespace AVR {

    template<auto N> requires( N > 0)
    using UseFifo = std::integral_constant<int,N>;

    using NoFifo = std::integral_constant<int,0>;

    struct NoInterrupts : std::false_type {using Adapter = void;};

    template<etl::Concepts::ProtocolAdapter adapter = External::Hal::NullProtocollAdapter>
    struct Interrupts : std::true_type {using Adapter = adapter;};

    struct blocking {using fifo = NoFifo; static constexpr bool intEnabled = false;};

    template<etl::Concepts::NamedConstant fifoUse = NoFifo, typename interrupt = NoInterrupts>
    struct notBlocking;

    template<typename interrupt>
    struct notBlocking<NoFifo,interrupt> {
        static constexpr bool intEnabled = true;
        using Adapter = typename interrupt::Adapter;
        using fifo = NoFifo;
    };

    template<>
    struct notBlocking<NoFifo,NoInterrupts> {
        static constexpr bool intEnabled = false;

        using fifo = NoFifo;
    };

    /////////no fifo/no PA -> not allowed
    template<>
    struct notBlocking<NoFifo,Interrupts<>>;

    template<etl::Concepts::NamedConstant fifoUse>
    struct notBlocking<fifoUse,NoInterrupts> {
        static constexpr bool intEnabled = false;

        using fifo = fifoUse;
    };

    template<etl::Concepts::NamedConstant fifoUse>
    struct notBlocking<fifoUse,Interrupts<>> {
        static constexpr bool intEnabled = true;

        using fifo = fifoUse;

    };



    struct ReadOnly{}; struct WriteOnly{}; struct ReadWrite{};

    template<typename component, int N >
    [[nodiscard]] auto getBaseAddress(){
        return component::template getBaseAddress<N>();
    }

    template<template<typename> typename component, typename p>
    [[nodiscard]] auto getBaseAddress(){
        //special case Port -> is no RC Component so their functions arent available for this component
        if constexpr (etl::Concepts::NotRCComponent<component<p>>){
            return (typename component<p>::Component_t::registers*) &component<p>::baseAddress();
        } else
            return component<p>::getBaseAddress();
    }


    template<typename component>
    [[nodiscard]] auto getBaseAddress(){
        //special case Port -> is no RC Component so their functions arent available for this component
            return (typename component::Component_t::registers*) &component::baseAddress();
    }

}
