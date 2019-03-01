/*
* Register.h
*
* Created: 27.01.2019 19:30:56
*  Author: Keven
*/


#pragma once
#include "Pin.hpp"
#include "../tools/utils.h"

struct RW {}; struct ReadOnly {};
template<typename Access = RW>
class Register {
	volatile mem_width reg;
	
	public:
	NoConstructors(Register);
	
	template<typename... ARGS>
	inline void on(const ARGS... bits) volatile {
		if constexpr(sizeof...(ARGS) == 0){
			reg = static_cast<mem_width>(-1);
		} else
		reg |= (static_cast<mem_width>(bits) | ...);
	}
	
	template<typename... ARGS>
	inline void off(const ARGS... bits) volatile {
		if constexpr(sizeof...(ARGS) == 0){
			reg = static_cast<mem_width>(0);
		} else
		reg &= ~(static_cast<mem_width>(bits) | ...);
	}

	template<typename... ARGS>
	inline void invert(const ARGS... bits) volatile {
		if constexpr(sizeof...(ARGS) == 0){
			reg = static_cast<mem_width>(-1);
		} else
		reg ^= (static_cast<mem_width>(bits) | ...);
	}

	//SFINAE
	[[nodiscard]] bool areSet() volatile;

	template<typename... ARGS>
	[[nodiscard]] inline bool areSet(const ARGS...bits) const volatile {
		return ((static_cast<mem_width>(bits) | ...) & reg) == (static_cast<mem_width>(bits) | ...);
	}
	
	[[nodiscard]] volatile mem_width& raw() volatile {
		return reg;
	}

	[[nodiscard]] volatile mem_width raw() const volatile {
		return reg;
	}
	/*
	function to cast the structs to Register
	*/
	[[nodiscard]] static inline volatile Register& getRegister(volatile mem_width& reg) {
		return reinterpret_cast<volatile Register&>(reg);
	}
}__attribute__((packed));

template<>
class Register<ReadOnly> {
	const volatile mem_width reg;
	
	public:
	NoConstructors(Register);
	
	[[nodiscard]] volatile mem_width raw() const volatile {
		return reg;
	}
	
	//SFINAE
	[[nodiscard]] bool areSet() volatile;

	template<typename... ARGS>
	[[nodiscard]] inline bool areSet(const ARGS...bits) const volatile {
		return ((static_cast<mem_width>(bits) | ...) & reg) == (static_cast<mem_width>(bits) | ...);
	}
	/*
	function to cast the structs to Register
	*/
	[[nodiscard]] static inline volatile Register& getRegister(volatile mem_width& reg) {
		return reinterpret_cast<volatile Register&>(reg);
	}
}__attribute__((packed));