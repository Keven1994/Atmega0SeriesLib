/*
* Pin.hpp
*
* Created: 27.01.2019 19:29:16
*  Author: Keven
*/


#pragma once

struct Bit {
	mem_width pinValue;
	
	constexpr explicit Bit(mem_width number) : pinValue( 1 << number ) {}
	constexpr Bit(const Bit& other) : pinValue(other.pinValue) {}
	constexpr Bit(Bit&& other) = delete;
	constexpr mem_width operator~() const {
		return ~pinValue;
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
	constexpr void operator=(const Bit& other) { pinValue = other.pinValue; }
	constexpr void operator=(Bit&& other) = delete;
	constexpr explicit operator mem_width() const{
		return pinValue;
	}
};

template<typename T>
inline void operator^=(T& lhs,const Bit rhs) {
	lhs ^= static_cast<mem_width>(rhs);
}

template<typename T>
inline void operator&=(T& lhs,const Bit rhs){
	lhs &= static_cast<mem_width>(rhs);
}

template<typename T>
inline void operator|=(T& lhs,const Bit rhs){
	lhs |= static_cast<mem_width>(rhs);
}

template<typename T>
constexpr inline mem_width operator^(const T lhs,const Bit rhs) {
	return static_cast<mem_width>(lhs) ^ static_cast<mem_width>(rhs);
}
template<typename T>
constexpr inline mem_width operator&(const T lhs,const Bit rhs) {
	return static_cast<mem_width>(lhs) & static_cast<mem_width>(rhs);
}
template<typename T>
constexpr inline mem_width operator|(const T lhs,const Bit rhs) {
	return static_cast<mem_width>(lhs) | static_cast<mem_width>(rhs);
}
template<typename T>
constexpr inline mem_width operator+(const T lhs,const Bit rhs) {
	return static_cast<mem_width>(lhs) + static_cast<mem_width>(rhs);
}
template<typename T>
constexpr inline mem_width operator-(const T lhs,const Bit rhs) {
	return static_cast<mem_width>(lhs) - static_cast<mem_width>(rhs);
}
template<typename T>
constexpr inline mem_width operator*(const T lhs,const Bit rhs) {
	return static_cast<mem_width>(lhs) * static_cast<mem_width>(rhs);
}
template<typename T>
constexpr inline mem_width operator/(const T lhs,const Bit rhs) {
	return static_cast<mem_width>(lhs) / static_cast<mem_width>(rhs);
}