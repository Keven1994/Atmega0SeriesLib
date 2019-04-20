//
// Created by keven on 15.04.2019.
//

#pragma once

namespace AVR {
    namespace details {
        struct AtmegaZero {

            template<typename comp>
            struct SPI_C {
                enum class TransferMode : mem_width {
                    //Leading edge: Rising, sample - Trailing edge: Falling, setup
                            Mode0 = static_cast<mem_width>(comp::CTRLBMasks::Mode_0),
                    //Leading edge: Rising, setup - Trailing edge: Falling, sample
                            Mode1 = static_cast<mem_width>(comp::CTRLBMasks::Mode_1),
                    //Leading edge: Falling, sample - Trailing edge: Rising, setup
                            Mode2 = static_cast<mem_width>(comp::CTRLBMasks::Mode_2),
                    //Leading edge: Falling, setup - Trailing edge: Rising, sample
                            Mode3 = static_cast<mem_width>(comp::CTRLBMasks::Mode_3)
                };

                enum class Prescaler : mem_width {
                    Div4 = static_cast<mem_width>(comp::CTRLAMasks::Presc_div4),
                    Div16 = static_cast<mem_width>(comp::CTRLAMasks::Presc_div16),
                    Div64 = static_cast<mem_width>(comp::CTRLAMasks::Presc_div64),
                    Div128 = static_cast<mem_width>(comp::CTRLAMasks::Presc_div128)
                };
            };

            template<typename twiComponent>
            struct TWI_C {
                enum class SDASetup: mem_width {
                    SDASetup_off = static_cast<mem_width>(twiComponent::CTRLAMasks::Sdahold_off),
                    SDASetup_50ns = static_cast<mem_width>(twiComponent::CTRLAMasks::Sdahold_50ns) ,
                    SDASetup_300ns = static_cast<mem_width>(twiComponent::CTRLAMasks::Sdahold_300ns) ,
                    SDASetup_500ns = static_cast<mem_width>(twiComponent::CTRLAMasks::Sdahold_500ns) ,
                };

                enum class SDAHold: mem_width {
                    Setup4Cycles = static_cast<mem_width>(twiComponent::CTRLAMasks::Sdasetup_4cyc),
                    Setup8Cycles = static_cast<mem_width>(twiComponent::CTRLAMasks::Sdasetup_8cyc)
                };

                enum class MasterTimeout: mem_width {
                    Disabled = static_cast<mem_width>(twiComponent::MCTRLAMasks::Timeout_disabled),
                    T_50us = static_cast<mem_width>(twiComponent::MCTRLAMasks::Timeout_50us),
                    T_100us = static_cast<mem_width>(twiComponent::MCTRLAMasks::Timeout_100us),
                    T_200us = static_cast<mem_width>(twiComponent::MCTRLAMasks::Timeout_200us)
                };

            };

            template<typename usartComponent>
            struct USART_C {
                enum class CommunicationMode: mem_width {
                    Asynchronous = static_cast<mem_width>(usartComponent::CTRLCMasks::Cmode_asynchronous),
                    Synchronous = static_cast<mem_width>(usartComponent::CTRLCMasks::Cmode_synchronous) ,
                    InfraRed = static_cast<mem_width>(usartComponent::CTRLCMasks::Cmode_ircom) ,
                    MSPI = static_cast<mem_width>(usartComponent::CTRLCMasks::Cmode_mspi)
                };

                enum class ParityMode: mem_width {
                    Disabled = static_cast<mem_width>(usartComponent::CTRLCMasks::Pmode_disabled),
                    Even = static_cast<mem_width>(usartComponent::CTRLCMasks::Pmode_even) ,
                    Odd = static_cast<mem_width>(usartComponent::CTRLCMasks::Pmode_odd)
                };

                enum class StopBitMode: mem_width {
                    OneBit = static_cast<mem_width>(usartComponent::CTRLCMasks::Sbmode_1bit),
                    TwoBits = static_cast<mem_width>(usartComponent::CTRLCMasks::Sbmode_2bit)
                };

                enum class ReceiverMode : mem_width {
                    Normal = static_cast<mem_width>(usartComponent::CTRLBMasks::Rxmode_normal),
                    Double = static_cast<mem_width>(usartComponent::CTRLBMasks::Rxmode_clk2x),
                    GenericAutoBaud = static_cast<mem_width>(usartComponent::CTRLBMasks::Rxmode_genauto),
                    LINConstraintAutoBaud = static_cast<mem_width>(usartComponent::CTRLBMasks::Rxmode_linauto)
                };

                enum class Interrupts : mem_width {
                    RxIE = static_cast<mem_width>(usartComponent::CTRLAMasks ::Rxcie),
                    TxIE = static_cast<mem_width>(usartComponent::CTRLAMasks ::Txcie),
                    DreIE = static_cast<mem_width>(usartComponent::CTRLAMasks ::Dreie),
                    ReceiverStartFrameDetection = static_cast<mem_width>(usartComponent::CTRLAMasks ::Rxsie),
                    AutoBaudError = static_cast<mem_width>(usartComponent::CTRLAMasks ::Abeie)
                };

                enum class RS485Mode : mem_width  {
                    Disabled = static_cast<mem_width>(usartComponent::CTRLAMasks::Rs485_off),
                    InternalTransmitter = static_cast<mem_width>(usartComponent::CTRLAMasks ::Rs485_int),
                    ExternalTransmitter = static_cast<mem_width>(usartComponent::CTRLAMasks ::Rs485_ext)
                };

                enum class CharacterSize : mem_width {
                    Bit5 = static_cast<mem_width>(usartComponent::CTRLCMasks::Chsize_5bit),
                    Bit6 = static_cast<mem_width>(usartComponent::CTRLCMasks::Chsize_6bit),
                    Bit7 = static_cast<mem_width>(usartComponent::CTRLCMasks::Chsize_7bit),
                    Bit8 = static_cast<mem_width>(usartComponent::CTRLCMasks::Chsize_8bit),
                    Bit9LowFirst = static_cast<mem_width>(usartComponent::CTRLCMasks::Chsize_9bitl),
                    Bit9HighFirst = static_cast<mem_width>(usartComponent::CTRLCMasks::Chsize_9bith)
                };
            };
        };
    }
}
