

//#define TEST
#define SPIMASTER




/////////////COMPILE TESTS/////////////////
#include "mega4808/Atmega4808.hpp"
using namespace mega4808;




//////////RESSOURCE CONTROLLER/////////////
#include "hw_abstractions/RessourceController.hpp"
#include "hw_abstractions/Port.hpp"


struct testC1 {
	template<auto N ,bool dummy = true>
	struct inst;
	
	template<bool dummy>
	struct inst<0, dummy>
	{
		template<auto N ,bool dummy1 = true>
		struct alt;
		
		[[nodiscard,gnu::always_inline]] static inline auto& value()  { return PORTA;}
		
		template<bool dummy1>
		struct alt<0, dummy1>
		{
			struct Scl {
				using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::portf>,3>;
			};
			
			struct Sda {
				using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::portf>,2>;
			};
			
			using list = Meta::List<typename Scl::pin0, typename Sda::pin0>;
		};
	};
};

struct TestComponent1 : public AVR::rc::details::RCComponent<testC1>  {
	
	template<typename Alias>
	friend struct resolveComponent;
	
};

struct testC2 {
	template<auto N ,bool dummy = true>
	struct inst;
	
	template<bool dummy>
	struct inst<0, dummy>
	{
		template<auto N ,bool dummy1 = true>
		struct alt;
		
		[[nodiscard,gnu::always_inline]] static inline auto& value()  { return PORTA;}
		
		template<bool dummy1>
		struct alt<0, dummy1>
		{
			struct Scl {
				using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::porta>,3>;
			};
			
			struct Sda {
				using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::porta>,2>;
			};
			
			using list = Meta::List<typename Scl::pin0, typename Sda::pin0>;
		};
	};
};

struct TestComponent2 : public AVR::rc::details::RCComponent<testC2>  {
	
	template<typename Alias>
	friend struct resolveComponent;
	
};

struct testC3 {
	template<auto N ,bool dummy = true>
	struct inst;
	
	template<bool dummy>
	struct inst<0, dummy>
	{
		template<auto N ,bool dummy1 = true>
		struct alt;
		
		[[nodiscard,gnu::always_inline]] static inline auto& value()  { return PORTA;}
		
		template<bool dummy1>
		struct alt<0, dummy1>
		{
			struct Scl {
				using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::portf>,3>;
			};
			
			struct Sda {
				using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::portf>,2>;
			};
			
			using list = Meta::List<typename Scl::pin0, typename Sda::pin0>;
		};
	};
};

struct TestComponent3 : public AVR::rc::details::RCComponent<testC3>  {
	
	template<typename Alias>
	friend struct resolveComponent;
	
};

struct testC4 {
	template<auto N ,bool dummy = true>
	struct inst;
	
	template<bool dummy>
	struct inst<0, dummy>
	{
		template<auto N ,bool dummy1 = true>
		struct alt;
		
		[[nodiscard,gnu::always_inline]] static inline auto& value()  { return PORTA;}
		
		template<bool dummy1>
		struct alt<0, dummy1>
		{
			struct Scl {
				using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::portf>,3>;
			};
			
			struct Sda {
				using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::portf>,2>;
			};
			
			using list = Meta::List<typename Scl::pin0, typename Sda::pin0>;
		};
		
		template<bool dummy1>
		struct alt<1, dummy1>
		{
			struct Scl {
				using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::portf>,5>;
			};
			
			struct Sda {
				using pin0 = AVR::port::details::PortPin<port_details::port<port_details::ports::portf>,4>;
			};
			
			using list = Meta::List<typename Scl::pin0, typename Sda::pin0>;
		};
	};
};

struct TestComponent4 : public AVR::rc::details::RCComponent<testC4>  {
	
	template<typename Alias>
	friend struct resolveComponent;
	
};

using testRessource = AVR::rc::Instance<
TestComponent1, // using ressource SPI
AVR::rc::Number<0>, //using instance '0'
AVR::portmux::PortMux<0>>; // using portmux 0 alternative
using RC1 = AVR::rc::RessourceController<testRessource>; //acquire ressource
//using tres1 = RC1::getRessource_t<testRessource>; //get the ressource -> work (no double layed ressources)

using testRessource1 = AVR::rc::Instance<
TestComponent1, // using ressource SPI
AVR::rc::Number<0>, //using instance '0'
AVR::portmux::PortMux<2>>; // using portmux 0 alternative

//should fail -> PortMux2 not existing (in this test case)
//uncomment for test/using tres2 = RC1::getRessource_t<testRessource1>;

using testRessource2 = AVR::rc::Instance<
TestComponent2, // using ressource SPI
AVR::rc::Number<0>, //using instance '0'
AVR::portmux::PortMux<0>>; // using portmux 0 alternative

//should work -> no conflicting pins and different components pins{C1{f3,f2}, C2{a3,a2}}
using RC2 = AVR::rc::RessourceController<testRessource,testRessource2>; //acquire ressource
//using tres3 = RC2::getRessource_t<testRessource>;
//using tres4 = RC2::getRessource_t<testRessource2>;

using testRessource3 = AVR::rc::Instance<
TestComponent3, // using ressource SPI
AVR::rc::Number<0>, //using instance '0'
AVR::portmux::PortMux<0>>; // using portmux 0 alternative

using RC3 = AVR::rc::RessourceController<testRessource,testRessource2,testRessource3>; //acquire ressource
//using tres5 = RC3::getRessource_t<testRessource>;
//using tres6 = RC3::getRessource_t<testRessource2>;
//using tres7 = RC3::getRessource_t<testRessource3>; should not work -> res3 and 0 have conflicting pins
//tres7::t2::_;

using testRessource4 = AVR::rc::Instance<
TestComponent4, // using ressource SPI
AVR::rc::Number<0>, //using instance '0'
AVR::portmux::PortMux<0>>; // using portmux 0 alternative

using testRessource5 = AVR::rc::Instance<
TestComponent4, // using ressource SPI
AVR::rc::Number<0>, //using instance '0'
AVR::portmux::PortMux<1>>; // using portmux 0 alternative

using RC4 = AVR::rc::RessourceController<testRessource4,testRessource5>; //acquire ressource
//using tres5 = RC4::getRessource_t<testRessource5>; should fail -> 2 alternatives from a instance 


