//
// Created by keven on 04.04.2019.
//

#pragma once

#include <type_traits>
#include "../MCUSelect.hpp"
#include "../tools/concepts.h"

namespace AVR {

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
