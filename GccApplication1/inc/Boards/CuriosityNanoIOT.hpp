//
// Created by keven on 11.04.2019.
//

#pragma once

#include <type_traits>
#include "../tools/utils.h"
#include "DebugOut.hpp"


#define DBGOUT

namespace AVR{
    namespace curiosity_details{
        using usartRessource = AVR::rc::Instance<
                AVR::usart::USART_Comp,
                utils::autoConstant<2>,
                utils::autoConstant<0>>;
        using RC = AVR::rc::RessourceController<usartRessource>;
        using usartres = typename RC::getRessource<usartRessource>;
    }
    using dbgout = details::_dbgout<curiosity_details::usartres,42>;

}