#pragma once
#include "tools/utils.h"
namespace mega4808{
		struct MHZ4{static constexpr auto value = 4000000UL;}; struct MHZ12{static constexpr auto value = 12000000UL;}; struct MHZ20{static constexpr auto value = 20000000UL;};
		
		template<typename frequenzy = MHZ4>
		requires(utils::isEqual<MHZ4,frequenzy>::value || utils::isEqual<MHZ12,frequenzy>::value || utils::isEqual<MHZ20,frequenzy>::value)
	struct Atmega4808;
}

using DEFAULT_MCU = mega4808::Atmega4808<>;
using DefaultMcuType = DEFAULT_MCU;