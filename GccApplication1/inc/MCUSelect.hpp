#pragma once
#include "tools/utils.h"
#include "MCUs.hpp"

#ifdef __AVR_DEV_LIB_NAME__

#ifdef _m4808
    #include "mega4808/Atmega4808.hpp"
    #define _DEFAULT mega4808::Atmega4808<>
#elif defined(_m4809)
    #define _DEFAULT mega4809::Atmega4809<>
#else
#error "using library with unsupported Device"
#endif

#else
#error "using library without an Default MCU Device"
#endif

static constexpr bool Debug = false;

using DEFAULT_MCU = _DEFAULT;
using DefaultMcuType = DEFAULT_MCU;