/*
 * AVRConcepts.h
 *
 * Created: 30.01.2019 18:37:41
 *  Author: Keven
 */ 


#pragma once

template <typename mem_width, template<typename> typename T>
concept bool isRegister() {
	return requires(T<mem_width> a) {
		{a.reg} -> volatile mem_width;
		{a.on()} -> void;
		{a.off()} -> void;
		{a.invert()} -> void;
		{a.raw()} -> volatile mem_width&;
	};
}

template <class T>
concept bool isPin() {
	return requires(T a) {
		{a.pinValue} -> uint8_t;
		
	};
}