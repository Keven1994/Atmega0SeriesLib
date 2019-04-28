
/*
* Atmega4808.hpp
*
* Created: 26.02.2019 22:15:24
*  Author: keven
*/

#pragma once
#include "../hw_abstractions/Basics.hpp"
//type informations for the templates in the hw includes

//hw includes
#include "hal/Atmega4808Port.hpp"
#include "mega4808/hal/Atmega4808EventSystem.hpp"
#include "hal/Atmega4808SPI.hpp"
#include "hal/ATmega4808TWI.hpp"
#include "hal/ATmega4808CPU.hpp"
#include "hal/ATmega4808USART.hpp"
#include "hal/ATmega4808ADC.hpp"
#include "../hw_abstractions/CPU.hpp"
#include "../hw_abstractions/RessourceController.hpp"
#include "../DeviceFamilys/ATmegaZero.hpp"

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

		struct SPI : public AVR::rc::details::RCComponent<spi_details::spis, spi_details::spiComponent>
	, public AVR::details::AtmegaZero::template SPI_C< spi_details::spiComponent>{


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


		    using Component_t = spi_details::spiComponent;
		};
		
		struct TWI : public AVR::rc::details::RCComponent<twi_details::twis, twi_details::twiComponent> ,
		        public AVR::details::AtmegaZero::template TWI_C< twi_details::twiComponent> {

            static constexpr auto riseTime = 1.5/1000000000;

			template<bool fastModePlus, SDAHold holdTime, SDASetup sdaSetup, bool quickCommand, bool smartMode, MasterTimeout timeOut>
			struct TWIMasterSetting{

				using AConf = twi_details::twiComponent::CTRLAMasks;
				using MAConf = twi_details::twiComponent::MCTRLAMasks;
				
				static constexpr AConf fastmode = fastModePlus ? AConf::Fmpen : static_cast<AConf>(0);
				static constexpr AConf holdtime = static_cast<AConf>(holdTime);
				static constexpr AConf setuptime = static_cast<AConf>(sdaSetup);
				static constexpr MAConf quickcommand = quickCommand ? MAConf::Qcen : static_cast<MAConf>(0);
				static constexpr MAConf smartmode = smartMode ? MAConf::Smen : static_cast<MAConf>(0);
				static constexpr MAConf timeout = static_cast<MAConf>(timeOut);
			};

			template<bool fastModePlus, SDAHold holdTime, SDASetup sdaSetup>
			struct TWISlaveSetting{

				using AConf = twi_details::twiComponent::CTRLAMasks;
				
				static constexpr AConf fastmode = fastModePlus ? AConf::Fmpen : static_cast<AConf>(0);
				static constexpr AConf holdtime = static_cast<AConf>(holdTime);
				static constexpr AConf setuptime = static_cast<AConf>(sdaSetup);
			};


            using Component_t = mega4808::twi_details::twiComponent;
		};

		struct USART : public AVR::rc::details::RCComponent<usart_details::usarts, usart_details::usartComponent>,
                       public AVR::details::AtmegaZero::template USART_C< usart_details::usartComponent> {

		    template<USART::RS485Mode RSMode, USART::ReceiverMode receiverMode,
                    USART::CommunicationMode ComMode, USART::ParityMode parityMode,
                    USART::StopBitMode stopBitMode, USART::CharacterSize CharSize, bool Msb,
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

		    using Component_t = usart_details::usartComponent;
		};

		template<bool unchecked,typename... PINS>
	    struct ADC : public AVR::rc::details::RCComponent<AVR::rc::details::GenericRessource<PINS...>, adc_details::adcComponent, adc_details::adcs>
		         , public AVR::details::AtmegaZero::template ADC_C< adc_details::adcComponent> {
            using AConf = adc_details::adcComponent::CTRLAMasks;
            using BConf = adc_details::adcComponent::CTRLBMasks;
            using CConf = adc_details::adcComponent::CTRLCMasks;
            using DConf = adc_details::adcComponent::CTRLDMasks;
            using EConf = adc_details::adcComponent::CTRLEMasks;
            using SampleControl = adc_details::adcComponent::SAMPCTRLMasks;
            using MUXPOSMasks = typename adc_details::adcComponent::MUXPOSMasks;

            template<typename inst, typename first>
            requires(sizeof...(PINS) > 0)
            struct MuxResolve{
                static_assert(Meta::contains<Meta::List<PINS...>,first>::value || unchecked, "this pin was not checked");
                static constexpr auto value =  (MUXPOSMasks)(std::is_same_v<first,typename inst::Ain::pin0> ? ADC_MUXPOS_AIN0_gc :
                                                             std::is_same_v<first,typename inst::Ain::pin1> ? ADC_MUXPOS_AIN1_gc :
                                                             std::is_same_v<first,typename inst::Ain::pin2> ? ADC_MUXPOS_AIN2_gc :
                                                             std::is_same_v<first,typename inst::Ain::pin3> ? ADC_MUXPOS_AIN3_gc :
                                                             std::is_same_v<first,typename inst::Ain::pin4> ? ADC_MUXPOS_AIN4_gc :
                                                             std::is_same_v<first,typename inst::Ain::pin5> ? ADC_MUXPOS_AIN5_gc :
                                                             std::is_same_v<first,typename inst::Ain::pin6> ? ADC_MUXPOS_AIN6_gc :
                                                             std::is_same_v<first,typename inst::Ain::pin7> ? ADC_MUXPOS_AIN7_gc :
                                                             std::is_same_v<first,typename inst::Ain::pin8> ? ADC_MUXPOS_AIN12_gc :
                                                             std::is_same_v<first,typename inst::Ain::pin9> ? ADC_MUXPOS_AIN13_gc :
                                                             std::is_same_v<first,typename inst::Ain::pin10> ? ADC_MUXPOS_AIN14_gc :
                                                             ADC_MUXPOS_AIN15_gc);
            };


            template<bool FreeRun, bool Bit8, bool Standby, ADC::Accumulations Accumulations, ADC::Prescaler Prescale
                    , ADC::WindowComparation WindowComparation, etl::Concepts::NamedConstant sampleDelay, etl::Concepts::NamedConstant sampleLength>
            struct ADCSetting {
                static constexpr AConf freerun = FreeRun ? static_cast<AConf >(adc_details::adcComponent::CTRLAMasks::Freerun) : static_cast<AConf >(0);
                static constexpr AConf resolution = Bit8 ? static_cast<AConf >(adc_details::adcComponent::CTRLAMasks::Ressel_8bit) : static_cast<AConf >(adc_details::adcComponent::CTRLAMasks::Ressel_10bit) ;
                static constexpr AConf standby = Standby ? static_cast<AConf >(adc_details::adcComponent::CTRLAMasks::Freerun) : static_cast<AConf >(0);
                static constexpr BConf accumulations = static_cast<BConf>(Accumulations);
                static constexpr CConf prescale = static_cast<CConf>(Prescale);
                static constexpr DConf sampledelay = sampleDelay::value == 0 ?
                                                     static_cast<DConf>(adc_details::adcComponent::CTRLDMasks::Asdv_asvon) :  static_cast<DConf>(calcUS( sampleDelay::value));
                static constexpr SampleControl samplelength = sampleLength::value == 0 ?
                                                     static_cast<SampleControl>(adc_details::adcComponent::CTRLDMasks::Asdv_asvon) : static_cast<SampleControl>(calcUS( sampleLength::value) >= 2 ? calcUS( sampleLength::value) : 2);
                //TODO: init delay?
                static constexpr EConf windowcomp = static_cast<EConf>(WindowComparation);

            private:
                template<uint16_t us>
                [[nodiscard]] static constexpr uint8_t calcUS(){
                    constexpr auto mcuclk = DEFAULT_MCU::clockFrequency;
                    constexpr auto adcclkus = (mcuclk/1000000) * (Prescale ==  ADC::Prescaler::Div2 ? 2: Prescale ==  ADC::Prescaler::Div4 ? 4:
                            Prescale ==  ADC::Prescaler::Div8 ? 8:Prescale ==  ADC::Prescaler::Div16 ? 16: Prescale ==  ADC::Prescaler::Div32 ? 32:
                            Prescale ==  ADC::Prescaler::Div64 ? 64: Prescale ==  ADC::Prescaler::Div128 ? 128: 256);
                    static_assert(us >= adcclkus, "sample delay value was lower than ADC Cycle duration!");
                    return us / adcclkus;
                }

            };

            using Component_t = adc_details::adcComponent;

		};

		struct Status {
            using Component_t = cpu_details::cpuComponent;
		};
		
	};
}