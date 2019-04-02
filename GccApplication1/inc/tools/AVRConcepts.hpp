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

template <typename T, typename memWidth>
concept bool isEventSystemUser() {
	return requires(T a) {
		{a.listener} -> volatile memWidth&;
	};
}

template <typename T, typename memWidth>
concept bool isIntegralConstant() {
	return requires(T a) {
		{a.value} -> memWidth;
	};
}

template <class T, typename memWidth>
concept bool isPin() {
	return requires(T a) {
		{a.pinValue} -> memWidth;
		
	};
}