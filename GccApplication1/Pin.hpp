/*
 * Pin.h
 *
 * Created: 27.01.2019 19:29:16
 *  Author: Keven
 */ 


#pragma once

template<typename mem_width = uint8_t>
struct Pin {
	const mem_width pinValue;
	
	explicit constexpr Pin(mem_width number) : pinValue( 1 << number ) {}
	constexpr Pin(const Pin& other) : pinValue(other.pinValue) {}
	constexpr Pin(Pin&& other) = delete;
	constexpr void operator=(mem_width number) { pinValue = 1 << number; }
	constexpr void operator=(const Pin& other) { pinValue = other.pinValue; }
	constexpr void operator=(Pin&& other) = delete;
	explicit operator mem_width() const{
		return pinValue;
	}
};