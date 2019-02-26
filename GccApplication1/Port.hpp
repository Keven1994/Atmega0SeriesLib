/*
 * Port.h
 *
 * Created: 29.01.2019 11:45:45
 *  Author: Keven
 */ 

#pragma once
#include "Register.hpp"
#include "utils.h"

namespace port {

	namespace {
		template<ptr_t baseAddress,auto offset, typename regEnum>
		class Port {

			static inline auto& port = *((PORT_t*)(baseAddress + static_cast<mem_width>(offset)));

			public:
			
			using registers = regEnum;
			
			Port() = delete;
			Port(const Port&) = delete;
			Port(Port&&) = delete;

			template<registers reg>
			[[nodiscard]] static inline auto& getMember(){
				auto* tmp = (mem_width*)&port;
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
