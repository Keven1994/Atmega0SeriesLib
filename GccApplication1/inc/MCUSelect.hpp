#pragma once
#include "tools/utils.h"
#include "MCUs.hpp"
#include <stddef.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#ifdef __AVR_DEV_LIB_NAME__

#ifdef _m4808
    #define _DEFAULT mega4808::Atmega4808<>


    using mem_width = uint8_t;
    using ptr_t = uintptr_t;
#elif defined(_m4809)
    #define _DEFAULT mega4809::Atmega4809<>
    using mem_width = uint8_t;
    using ptr_t = uintptr_t;
#else
#error "using library with unsupported Device"
#endif

#else
#error "using library without an Default MCU Device"
#endif

static constexpr bool Debug = false;

using DEFAULT_MCU = _DEFAULT;
using DefaultMcuType = DEFAULT_MCU;

#ifdef _m4808
#include "mega4808/Atmega4808.hpp"
#elif defined(_m4809)
#include "mega4809/Atmega4809.hpp"
#endif
