/*
 * Port.h
 *
 * Created: 29.01.2019 11:45:45
 *  Author: Keven
 */ 
#include "Register.hpp"

#pragma once

template<typename mem_width = uint8_t>
struct Port {
	volatile Register& reg;
	Port(volatile PORT_t& reg) : reg(reg) {}
};