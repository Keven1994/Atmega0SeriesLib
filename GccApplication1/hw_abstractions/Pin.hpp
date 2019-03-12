/*
* Pin.hpp
*
* Created: 27.01.2019 19:29:16
*  Author: Keven
*/


#pragma once

struct Pin {
	mem_width pinValue;
	
	constexpr explicit Pin(mem_width number) : pinValue( 1 << number ) {}
	constexpr Pin(const Pin& other) : pinValue(other.pinValue) {}
	constexpr Pin(Pin&& other) = delete;
	constexpr Pin operator~() const {
		return Pin(~pinValue);
	}
	
	template<typename T>
	void operator&=(T other){
		pinValue &= static_cast<mem_width>(other);
	}
	template<typename T>
	void operator|=(T other){
		pinValue |= static_cast<mem_width>(other);
	}

	template<typename T>
	void operator^=(T other){
		pinValue ^= static_cast<mem_width>(other);
	}
	
	constexpr void operator=(mem_width number) { pinValue = 1 << number; }
	constexpr void operator=(const Pin& other) { pinValue = other.pinValue; }
	constexpr void operator=(Pin&& other) = delete;
	constexpr explicit operator mem_width() const{
		return pinValue;
	}
};

template<typename T>
inline void operator^=(T& lhs,const Pin rhs) {
	lhs ^= static_cast<mem_width>(rhs);
}

template<typename T>
inline void operator&=(T& lhs,const Pin rhs){
	lhs &= static_cast<mem_width>(rhs);
}

template<typename T>
inline void operator|=(T& lhs,const Pin rhs){
	lhs |= static_cast<mem_width>(rhs);
}