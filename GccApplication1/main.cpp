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

#define MEGA4808

#ifdef MEGA4808
	//fix unavailable ports
	#undef PORTB
	#undef PORTE
	///////////////////////
	#include "mega4808/Atmega4808.hpp"
	
	using namespace mega4808; //specify used mmcu
	using portf = Atmega4808::Ports::portf; 
	using porta = Atmega4808::Ports::porta;
	using ch1 = Atmega4808::EventSystem::ch1;
#elif defined(MEGA4809)
	#include "mega4808/Atmega4809.hpp"
	
	using namespace mega4809; //specify used mmcu
	using portf = Atmega4809::Ports::portf;
	using porta = Atmega4809::Ports::porta;
	using ch1 = Atmega4809::EventSystem::ch1;
#endif
	
int main( ) {
	//eventsystem example
	ch1::setGenerator<ch1::generators::PortAGenerator<porta::pins::pin0>>(); //sets the generator for channel1
	ch1::registerListener<ch1::users::evportf>();					   //register listener portf for channel1
	
	//configure ports for use
	portf::getDir().on(); // every pin in Register dir will be 1 (0xff) == output
	porta::getDir().on();
	
	//get a pin from the port --> this is safe, the pin will be available
	constexpr auto pa5Pin = porta::pins::pin5;
	
	porta::getDir().off(pa5Pin); // sets pin5 to off == input

    while (true)  //hello world mic
    {	
		portf::getOut().off();
		auto isPin5Set = porta::getIn().areSet(pa5Pin); // test e specific pin
		//auto fullRegVal = porta::getIn().getRegister().raw(); get full register value
		if(isPin5Set){
			for(uint8_t i = 2; i < 6;i++){
				Pin p(i); //unsafe usage --> pins should be accessed from the class to ensure they can be used
				portf::getMember<portf::registers::OUT>().on(p); //alternative way to get a member register of portf
			}
		}		

    }
}

	//ch1::setGenerator<ch1::PortAGenerator<0>::portAPin>();
	//EVSYS.USEREVOUTF = 1 << 1; //user pf listen to ch 1 ->
	//eventsystem::EventSystem::listenToChannel<1,eventsystem::EventSystem::portf>();