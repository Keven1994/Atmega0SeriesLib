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
	inline void on(const ARGS... bits) volatile {
		reg |= (static_cast<mem_width>(bits) | ...);
	}
			
	inline void invert() volatile  {
		reg ^= static_cast<mem_width>(0xffffffffffffffffULL);
	}
				
	template<typename... ARGS>
	inline void off(const ARGS... bits) volatile {
		reg &= ~(static_cast<mem_width>(bits) | ...);
	}

	template<typename... ARGS>
	inline void invert(const ARGS... bits) volatile {
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