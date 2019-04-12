
/*
* Atmega4808.hpp
*
* Created: 26.02.2019 22:15:24
*  Author: keven
*/

#pragma once
#include <stddef.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "../hw_abstractions/Basics.hpp"
//type informations for the templates in the hw includes
using mem_width = uint8_t;
using ptr_t = uintptr_t;

//hw includes
#include "hal/Atmega4808Port.hpp"
#include "hal/Atmega4808EVSYS.hpp"
#include "hal/Atmega4808SPI.hpp"
#include "hal/ATmega4808TWI.hpp"
#include "hal/ATmega4808CPU.hpp"
#include "hal/ATmega4808USART.hpp"
#include "../hw_abstractions/CPU.hpp"
#include "../hw_abstractions/RessourceController.hpp"

namespace mega4808 {

	template<auto frequency>
	class Atmega4808 {

		template<typename Alias>
		friend struct AVR::rc::details::resolveComponent;
		
		template<typename MCU,typename FIRST,typename... PINS>
		friend class AVR::rc::ResController;
		
		public:

	    struct isZero{};

		static constexpr auto clockFrequency = frequency;
		
		NoConstructors(Atmega4808);
		
		template<typename T>
		static inline constexpr bool is_atomic(){
            return false;
		}

		template<typename p>
	    struct Port {

            using port = typename utils::condEqual<AVR::port::A, p, port_details::port<port_details::ports::porta>,
                    typename utils::condEqual<AVR::port::C, p, port_details::port<port_details::ports::portc>,
                            typename utils::condEqual<AVR::port::D, p, port_details::port<port_details::ports::portd>,
                                    typename utils::condEqual<AVR::port::F, p, port_details::port<port_details::ports::portf>, void
                                    >::type
                            >::type
                    >::type
            >::type;

            static constexpr auto baseAddress = port::port;
            using Component_t = port_details::portComponent;
        };
		
		struct EventSystem {

			template<mem_width number>
			using generators = mega4808::details::generatorChannel<number>;
			
			using users = mega4808::details::users;
		};

		struct SPI : public AVR::rc::details::RCComponent<spi_details::spis, typename spi_details::spiComponent> {

			using TransferMode = mega4808::spi_details::TransferMode;
			using Prescaler = mega4808::spi_details::Prescaler;

			template<bool msb,  bool clockDoubled, bool slaveSelectDisable,TransferMode transferMode, bool buffered,
			bool waitForReceive, Prescaler prescaler>
			requires(!(!buffered && waitForReceive))
			struct SPIMasterSetting{

				using AConf = spi_details::spiComponent::CTRLAMasks;
				using BConf = spi_details::spiComponent::CTRLBMasks;
				
				static constexpr AConf Msb = msb ? static_cast<AConf>(0) : AConf::Dord;
				static constexpr AConf clkx2 = clockDoubled ? AConf::Clk2x :  static_cast<AConf>(0);
				static constexpr AConf presc = static_cast<AConf>(prescaler);
				static constexpr BConf ssd = slaveSelectDisable ? BConf::Ssd :  static_cast<BConf>(0);
				static constexpr BConf tmode = static_cast<BConf>(transferMode);
				static constexpr BConf buf = buffered ? BConf::Bufen : static_cast<BConf>(0);
				static constexpr BConf bufwait = buffered ? BConf::Bufwr : static_cast<BConf>(0);
			};

			template<bool msb,TransferMode transferMode, bool buffered, bool waitForReceive>
			requires(!(!buffered && waitForReceive))
			struct SPISlaveSetting{

				using AConf = spi_details::spiComponent::CTRLAMasks;
				using BConf = spi_details::spiComponent::CTRLBMasks;
				
				static constexpr AConf Msb = msb ? static_cast<AConf>(0) : AConf::Dord;
				static constexpr BConf tmode = static_cast<BConf>(transferMode);
				static constexpr BConf buf = buffered ? BConf::Bufen : static_cast<BConf>(0);
				static constexpr BConf bufwait = buffered ? BConf::Bufwr : static_cast<BConf>(0);
			};

		    private:
		    using Component_t = spi_details::spiComponent;
		};
		
		struct TWI : public AVR::rc::details::RCComponent<twi_details::twis, twi_details::twiComponent>  {
			
			using SDAHold = twi_details::SDAHold;
			using SDASetup = twi_details::SDASetup;
			using MasterTimeout = twi_details::MasterTimeout;

			template<bool fastModePlus, SDAHold holdTime, SDASetup sdaSetup, bool quickCommand, bool smartMode, MasterTimeout timeOut, mem_width baudRate>
			struct TWIMasterSetting{

				using AConf = twi_details::twiComponent::CTRLAMasks;
				using MAConf = twi_details::twiComponent::MCTRLAMasks;
				
				static constexpr AConf fastmode = fastModePlus ? AConf::Fmpen : static_cast<AConf>(0);
				static constexpr AConf holdtime = static_cast<AConf>(holdTime);
				static constexpr AConf setuptime = static_cast<AConf>(sdaSetup);
				static constexpr MAConf quickcommand = quickCommand ? MAConf::Qcen : static_cast<MAConf>(0);
				static constexpr MAConf smartmode = smartMode ? MAConf::Smen : static_cast<MAConf>(0);
				static constexpr MAConf timeout = static_cast<MAConf>(timeOut);
				static constexpr mem_width baud = baudRate;
			};

			template<bool fastModePlus, SDAHold holdTime, SDASetup sdaSetup>
			struct TWISlaveSetting{

				using AConf = twi_details::twiComponent::CTRLAMasks;
				
				static constexpr AConf fastmode = fastModePlus ? AConf::Fmpen : static_cast<AConf>(0);
				static constexpr AConf holdtime = static_cast<AConf>(holdTime);
				static constexpr AConf setuptime = static_cast<AConf>(sdaSetup);
			};

		private:
            using Component_t = mega4808::twi_details::twiComponent;
		};

		struct USART : public AVR::rc::details::RCComponent<usart_details::usarts, usart_details::usartComponent> {

		    using RS485Mode = usart_details::RS485Mode;
		    using ReceiverMode = usart_details::ReceiverMode;
		    using CommunicationMode = usart_details::CommunicationMode;
		    using StopBitMode = usart_details::StopBitMode;
		    using ParityMode = usart_details::ParityMode;
		    using CharacterSize = usart_details::CharacterSize;
		    using Interrupts = usart_details::InterruptEnable ;

		    template<usart_details::RS485Mode RSMode, usart_details::ReceiverMode receiverMode,
		            usart_details::CommunicationMode ComMode, usart_details::ParityMode parityMode,
		            usart_details::StopBitMode stopBitMode, usart_details::CharacterSize CharSize, bool Msb,
		            bool StartFrameDetection, bool OpenDrainMode, bool MultiProcessor , bool LoopBackMode>
		    struct USARTSetting {
                using AConf = usart_details::usartComponent::CTRLAMasks;
                using BConf = usart_details::usartComponent::CTRLBMasks;
                using CConf = usart_details::usartComponent::CTRLCMasks;

                static constexpr AConf rsmode = static_cast<AConf>(RSMode);
                static constexpr AConf loopbackmode = LoopBackMode ? AConf::Lbme : static_cast<AConf>(0);
                static constexpr BConf opendrainmode = OpenDrainMode ? BConf::Odme : static_cast<BConf>(0);
                static constexpr BConf startframedetection = StartFrameDetection ? BConf::Sfden : static_cast<BConf>(0);
                static constexpr BConf multiprocessor = MultiProcessor ? BConf::Mpcm : static_cast<BConf>(0);
                static constexpr BConf receivermode = static_cast<BConf>(receiverMode);
                static constexpr CConf commode = static_cast<CConf>(ComMode);
                static constexpr CConf paritymode = static_cast<CConf>(parityMode);
                static constexpr CConf stopbitmode = static_cast<CConf>(stopBitMode);
                static constexpr CConf charsize = static_cast<CConf>(CharSize);
                static constexpr CConf msb = !Msb ? CConf::Udord : static_cast<CConf>(0);
		    };
		private:
		    using Component_t = usart_details::usartComponent;
		};

		class Status {
		    public:
            using Component_t = cpu_details::cpuComponent;
		};
		
	};
}