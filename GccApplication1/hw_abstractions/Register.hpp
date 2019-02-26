/*
 * Register.h
 *
 * Created: 27.01.2019 19:30:56
 *  Author: Keven
 */ 


#pragma once
#include "Pin.hpp"

struct RW {}; struct ReadOnly {};
template<typename Access = RW>
struct Register {
	volatile mem_width reg;
	Register() = delete;
	Register(const Register&) = delete;
	Register(Register&&) = delete;
		
	inline void on() volatile {
		reg = static_cast<mem_width>(0xffffffffffffffffULL);
	}
	
	inline void off() volatile {
		reg = static_cast<mem_width>(0ULL);
	}
	
	template<typename... ARGS>
	[[gnu::always_inline]] inline void on(const ARGS... pins) volatile {
		reg |= (static_cast<mem_width>(pins) | ...);
	}
			
	inline void invert() volatile  {
		reg ^= reg;
	}
				
	template<typename... ARGS>
	inline void off(const ARGS... pins) volatile {
		reg &= ~(static_cast<mem_width>(pins) | ...);
	}

	template<typename... ARGS>
	inline void invert(const ARGS... pins) volatile {
		reg ^= (static_cast<mem_width>(pins) | ...);
	}

	//SFINAE
	[[nodiscard]] bool areSet() volatile;

	template<typename... ARGS>
	[[nodiscard]] inline bool areSet(const ARGS...pins) const volatile {
		return ((static_cast<mem_width>(pins) | ...) & reg) == (static_cast<mem_width>(pins) | ...);			
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
struct Register<ReadOnly> {
	const volatile mem_width reg;
	Register() = delete;
	Register(const Register&) = delete;
	Register(Register&&) = delete;
	
	[[nodiscard]] volatile mem_width raw() const volatile {
		return reg;
	}
	
	//SFINAE
	[[nodiscard]] bool areSet() volatile;

	template<typename... ARGS>
	[[nodiscard]] inline bool areSet(const ARGS...pins) const volatile {
		return ((static_cast<mem_width>(pins) | ...) & reg) == (static_cast<mem_width>(pins) | ...);
	}
	/*
	function to cast the structs to Register
	*/
	[[nodiscard]] static inline volatile Register& getRegister(volatile mem_width& reg) {
		return reinterpret_cast<volatile Register&>(reg);
	}
}__attribute__((packed));