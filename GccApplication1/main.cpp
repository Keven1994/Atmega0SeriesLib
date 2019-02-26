/*
 * GccApplication1.cpp
 *
 * Created: 26.01.2019 16:32:01
 * Author : Keven
 */ 
#define F_CPU 4000000UL

#include <stddef.h>
#include <util/delay.h>
#include <avr/io.h>
#include "mega4809/Atmega4809.hpp"


	
using namespace mega4809; //specify used mmcu
//using ch1 = typename mega4809::eventsystem::Channel<1>;
	static inline auto& testvar = PORTF;
int main( ) {
	using portf = Atmega4809::Ports::portf;
	using porta = Atmega4809::Ports::porta;
	
	using ch1 = Atmega4809::EventSystem::ch1;
	
	//eventsystem example
	ch1::setGenerator<ch1::generators::PortAGenerator<0>::portAPin>(); //sets the generator for channel1
	ch1::registerListener<ch1::users::evportf>();					   //register listener portf for channel1
	
	//configure ports for use
	portf::getDir().on(); // every pin in Register dir will be 1 (0xff) == output
	constexpr Pin pin5(5); // pin5 variable for general usage
	porta::getDir().on();
	porta::getDir().off(pin5); // sets pin5 to off == input

    while (true)  //hello world mic
    {	
		portf::getOut().off();
		auto isPin5Set = porta::getIn().areSet(pin5); // test e specific pin
		//auto fullRegVal = porta::getIn().getRegister().raw(); get full register value
		if(isPin5Set){
			for(uint8_t i = 2; i < 6;i++){
				Pin p(i);
				portf::getMember<portf::registers::OUT>().on(p); //alternative way to get a member register of portx
			}
		}		

    }
}

	//ch1::setGenerator<ch1::PortAGenerator<0>::portAPin>();
	//EVSYS.USEREVOUTF = 1 << 1; //user pf listen to ch 1 ->
	//eventsystem::EventSystem::listenToChannel<1,eventsystem::EventSystem::portf>();