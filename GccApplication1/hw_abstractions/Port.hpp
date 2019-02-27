/*
 * Port.h
 *
 * Created: 29.01.2019 11:45:45
 *  Author: Keven
 */ 

#pragma once
#include "../hw_abstractions/Register.hpp"
#include "../tools/utils.h"

namespace port {

	namespace {
		template<typename P, typename regEnum>
		class Port {

			static inline auto& port = P::value;

			public:
			using pins = typename P::pins;
			using registers = regEnum;
			
			Port() = delete;
			Port(const Port&) = delete;
			Port(Port&&) = delete;

			template<registers reg>
			[[nodiscard]] static inline auto& getMember(){
				auto* tmp = reinterpret_cast<mem_width*>(&port);
				return Register<>::getRegister(*( tmp + reg));
			}

			[[nodiscard]] static inline auto& getDir() {
				return Register<>::getRegister(port.DIR);
			};

			[[nodiscard]] static inline auto& getIn() {
				return Register<ReadOnly>::getRegister(port.IN);
			};

			[[nodiscard]] static inline auto& getOut() {
				return Register<>::getRegister(port.OUT);
			};
				
		}__attribute__((packed));
	}
}
//legacy
