/*
* Register.h
*
* Created: 27.01.2019 19:30:56
*  Author: Keven
*/


#pragma once
#include "Pin.hpp"
#include "../tools/utils.h"

namespace reg {
	
	namespace accessmode {
		struct RW {}; struct ReadOnly {};
	}
	
	namespace specialization {
		//data and flag equal
		struct Data{}; struct Control {}; struct Toggle{};
	}
	
	template<typename Access = accessmode::RW, typename Specialization = specialization::Data, typename Bits = void, typename size = mem_width>
	class Register;
	
	template<typename size>
	class Register<accessmode::RW, specialization::Data, void, size>{

		volatile size reg;
		
		public:
		NoConstructors(Register);
		using regSize = size;
		template<typename... ARGS>
		inline void on(const ARGS... bits) volatile {
			if constexpr(sizeof...(ARGS) == 0){
				reg = static_cast<size>(-1);
			} else
			reg |= (static_cast<size>(bits) | ...);
		}
		
		template<typename... ARGS>
		inline void off(const ARGS... bits) volatile {
			if constexpr(sizeof...(ARGS) == 0){
				reg = static_cast<size>(0);
			} else
			reg &= ~(static_cast<size>(bits) | ...);
		}

		template<typename... ARGS>
		inline void invert(const ARGS... bits) volatile {
			if constexpr(sizeof...(ARGS) == 0){
				reg ^= static_cast<size>(-1);
			} else
			reg ^= (static_cast<size>(bits) | ...);
		}

		//SFINAE
		[[nodiscard]] bool areSet() volatile;

		template<typename... ARGS>
		[[nodiscard]] inline bool areSet(const ARGS...bits) const volatile {
			return ((static_cast<size>(bits) | ...) & reg) == (static_cast<size>(bits) | ...);
		}
		
		[[nodiscard]] volatile size& raw() volatile {
			return reg;
		}

		[[nodiscard]] volatile size raw() const volatile {
			return reg;
		}
		/*
		function to cast the structs to Register
		*/
		[[nodiscard]] static inline volatile Register& getRegister(volatile size& reg) {
			return reinterpret_cast<volatile Register&>(reg);
		}
	}__attribute__((packed));

	template<typename size>
	class Register<accessmode::ReadOnly,specialization::Data,void,size> {
		const volatile size reg;
		
		public:
		NoConstructors(Register);
		using regSize = size;
		[[nodiscard]] volatile mem_width raw() const volatile {
			return reg;
		}
		
		//SFINAE
		[[nodiscard]] bool areSet() volatile;

		template<typename... ARGS>
		[[nodiscard]] inline bool areSet(const ARGS...bits) const volatile {
			return ((static_cast<size>(bits) | ...) & reg) == (static_cast<size>(bits) | ...);
		}
		/*
		function to cast the structs to Register
		*/
		[[nodiscard]] static inline volatile Register& getRegister(volatile size& reg) {
			return reinterpret_cast<volatile Register&>(reg);
		}
	}__attribute__((packed));
	
	template<typename Bits, typename size>
	class Register<accessmode::RW,specialization::Control,Bits,size> {
		static_assert(! utils::isEqual<Bits,void>::value, "enum type expected");
		volatile size reg;
		
		public:
		NoConstructors(Register);
		using regSize = size;
		using special_bit = Bits;
		
		template<typename... ARGS>
		inline void on(const ARGS... bits) volatile {
			static_assert(utils::isEqual<special_bit,typename utils::front<ARGS...>::type>::value && utils::sameTypes<ARGS...>(),"only the special bits are allowed");
			if constexpr(sizeof...(ARGS) == 0){
				reg = static_cast<size>(-1);
			} else
			reg |= (static_cast<size>(bits) | ...);
		}
		
		template<typename... ARGS>
		inline void off(const ARGS... bits) volatile {
			static_assert(utils::isEqual<special_bit,typename utils::front<ARGS...>::type>::value && utils::sameTypes<ARGS...>(),"only the special bits are allowed");
			if constexpr(sizeof...(ARGS) == 0){
				reg = static_cast<size>(0);
			} else
			reg &= ~(static_cast<size>(bits) | ...);
		}

		template<typename... ARGS>
		inline void invert(const ARGS... bits) volatile {
			static_assert(utils::isEqual<special_bit,typename utils::front<ARGS...>::type>::value && utils::sameTypes<ARGS...>(),"only the special bits are allowed");
			if constexpr(sizeof...(ARGS) == 0){
				reg ^= static_cast<size>(-1);
			} else
			reg ^= (static_cast<size>(bits) | ...);
		}

		//SFINAE
		[[nodiscard]] bool areSet() volatile;

		template<typename... ARGS>
		[[nodiscard]] inline bool areSet(const ARGS...bits) const volatile {
			static_assert(utils::isEqual<special_bit,typename utils::front<ARGS...>::type>::value && utils::sameTypes<ARGS...>(),"only the special bits are allowed");
			return ((static_cast<size>(bits) | ...) & reg) == (static_cast<size>(bits) | ...);
		}
		
		[[nodiscard]] volatile size& raw() volatile {
			return reg;
		}

		[[nodiscard]] volatile size raw() const volatile {
			return reg;
		}
		/*
		function to cast the structs to Register
		*/
		[[nodiscard]] static inline volatile Register& getRegister(volatile size& reg) {
			return reinterpret_cast<volatile Register&>(reg);
		}
	}__attribute__((packed));
	
	template<typename Bits, typename size>
	class Register<accessmode::ReadOnly,specialization::Control,Bits,size>;
	
	template<typename size>
	class Register<accessmode::RW,specialization::Toggle,void,size> {

		volatile size reg;
		
		public:
		NoConstructors(Register);
		using regSize = size;
		template<typename... ARGS>
		inline void invert(const ARGS... bits) volatile {
			if constexpr(sizeof...(ARGS) == 0){
				reg = static_cast<size>(-1);
			} else
			reg = (static_cast<size>(bits) | ...);
		}

		//SFINAE
		[[nodiscard]] bool areSet() volatile;

		template<typename... ARGS>
		[[nodiscard]] inline bool areSet(const ARGS...bits) const volatile {
			return ((static_cast<size>(bits) | ...) & reg) == (static_cast<size>(bits) | ...);
		}
		
		[[nodiscard]] volatile size& raw() volatile {
			return reg;
		}

		[[nodiscard]] volatile size raw() const volatile {
			return reg;
		}
		/*
		function to cast the structs to Register
		*/
		[[nodiscard]] static inline volatile Register& getRegister(volatile size& reg) {
			return reinterpret_cast<volatile Register&>(reg);
		}
	}__attribute__((packed));
	
	template<typename size>
	class Register<accessmode::ReadOnly,specialization::Toggle,void,size>;
}