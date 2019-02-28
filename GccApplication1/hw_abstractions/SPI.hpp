
/*
 * SPI.hpp
 *
 * Created: 27.02.2019 10:21:27
 *  Author: keven
 */

#include "Register.hpp"

namespace spi {
		
	namespace{
		static inline constexpr mem_width interruptFlag = 1 << 7;
		static inline constexpr mem_width lsb = 1 << 6;
		static inline constexpr mem_width master = 1 << 5;
		static inline constexpr mem_width clk2x = 1 << 4;
		static inline constexpr mem_width ssd = 1 << 2;
		static inline constexpr mem_width enable = 1 << 0;
	}
					
	enum class TransferMode : mem_width {
		//Leading edge: Rising, sample - Trailing edge: Falling, setup
		Mode0 = 0b00, 
		//Leading edge: Rising, setup - Trailing edge: Falling, sample
		Mode1 = 0b01,
		//Leading edge: Falling, sample - Trailing edge: Rising, setup
		Mode2 = 0b10,
		//Leading edge: Falling, setup - Trailing edge: Rising, sample
		Mode3 = 0b11
	};
				
	enum class BufferMode : mem_width {
		unbuffered = 0b10 << 6,
		bufferedFirstDummy = 0b01 << 6,
		bufferedDirectWrite = 0b11 << 6
	};
		
	enum class Prescaler : mem_width {
		Div4 = 0b00,
		Div16 = 0b01 << 1,
		Div64 = 0b10 << 1,
		Div128 = 0b11 << 1
	};
		
	template<typename port, auto& sspin,auto& Mosipin, auto& Misopin, auto& Clkpin, bool MSB = true,  bool ClockDoubled = true, bool SlaveSelectDisable = true, 
	TransferMode transfermode = TransferMode::Mode0, BufferMode buffermode = BufferMode::unbuffered, Prescaler prescaler = Prescaler::Div4>
	struct SPIMaster {

		static inline void init(){
			auto& ctra = Register<>::getRegister(SPI0.CTRLA);
			auto& ctrb = Register<>::getRegister(SPI0.CTRLB);
			
			ctra.raw() = master; 
			port::getDir().on(Mosipin, Clkpin);
			port::getDir().off(Misopin);
			if constexpr(prescaler != Prescaler::Div4)
				ctra.on(prescaler);
			if constexpr(ClockDoubled)
				ctra.on(clk2x);
			ctrb.raw() = static_cast<mem_width>(transfermode);
			if constexpr(!MSB)
				ctra.on(lsb);
			if constexpr(SlaveSelectDisable){
				ctrb.on(ssd);
			} else {
				port::getDir().off(sspin);
			}
			ctrb.on(buffermode);
			ctra.on(enable);
		}
		
		[[nodiscard]] static inline mem_width singleTransmit(mem_width data) {
			auto& datareg = Register<>::getRegister(SPI0.DATA);
			auto& ints = Register<>::getRegister(SPI0.INTFLAGS);
			datareg.raw() = data;
			while(! ints.areSet(interruptFlag));
			return datareg.raw();
		}
		
		[[nodiscard]] static inline mem_width singleReceive() {
			auto& datareg = Register<>::getRegister(SPI0.DATA);
			auto& ints = Register<>::getRegister(SPI0.INTFLAGS);

			while(! ints.areSet(interruptFlag));
			return datareg.raw();
		}
		
		static inline void singleTransfer(mem_width data) {
			auto& datareg = Register<>::getRegister(SPI0.DATA);
			auto& ints = Register<>::getRegister(SPI0.INTFLAGS);
			datareg.raw() = data;
			while(! ints.areSet(interruptFlag));
		}
		
		[[nodiscard]] static inline mem_width* Transmit(mem_width* data, mem_width size) {
			for(mem_width i = 0; i < size; i++){
				data[i] = singleTransmit(data[i]);
			}
			return data;
		}
		
		[[nodiscard]] static inline mem_width* Receive(mem_width* data, mem_width size) {
			for(mem_width i = 0; i < size; i++){
				data[i] = singleReceive();
			}
			return data;
		}
		
		static inline void Transfer(mem_width* data, mem_width size) {
			for(mem_width i = 0; i < size; i++){
				singleTransmit(data[i]);
			}
		}
		
	};

	template<typename port, auto& sspin,auto& Mosipin, auto& Misopin, auto& Clkpin,bool MSB = true, TransferMode transfermode = TransferMode::Mode0, BufferMode buffermode = BufferMode::unbuffered>
	struct SPISlave {

		static inline void init(){
			auto& ctra = Register<>::getRegister(SPI0.CTRLA);
			auto& ctrb = Register<>::getRegister(SPI0.CTRLB);
			ctra.raw() = 0x00; //operate in slave mode
			port::getDir().off(Mosipin, Clkpin);
			port::getDir().on(Misopin);
			
			ctrb.raw() = static_cast<mem_width>(transfermode);
			if constexpr(!MSB)
				ctra.on(lsb);
				
			port::getDir().off(sspin); //ss as input
			
			ctrb.on(buffermode);
			ctra.on(enable);
		}

		[[nodiscard]] static inline mem_width singleTransmit(mem_width data) {
			auto& datareg = Register<>::getRegister(SPI0.DATA);
			auto& ints = Register<>::getRegister(SPI0.INTFLAGS);
			datareg.raw() = data;
			while(! ints.areSet(interruptFlag));
			return datareg.raw();
		}

		static inline void singleTransfer(mem_width data) {
			auto& datareg = Register<>::getRegister(SPI0.DATA);
			auto& ints = Register<>::getRegister(SPI0.INTFLAGS);
			datareg.raw() = data;
			while(! ints.areSet(interruptFlag));
		}
		
		[[nodiscard]] static inline mem_width singleReceive() {
			auto& datareg = Register<>::getRegister(SPI0.DATA);
			auto& ints = Register<>::getRegister(SPI0.INTFLAGS);

			while(! ints.areSet(interruptFlag));
			return datareg.raw();
		}

		[[nodiscard]] static inline mem_width* Receive(mem_width* data, mem_width size) {
			for(mem_width i = 0; i < size; i++){
				data[i] = singleReceive();
			}
			return data;
		}

		[[nodiscard]] static inline mem_width* Transmit(mem_width* data, mem_width size) {
			for(mem_width i = 0; i < size; i++){
				data[i] = singleTransmit(data[i]);
			}
			return data;
		}

		static inline void Transfer(mem_width* data, mem_width size) {
			for(mem_width i = 0; i < size; i++){
				singleTransmit(data[i]);
			}
		}
	};
}