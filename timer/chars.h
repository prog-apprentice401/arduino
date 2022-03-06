#include <Arduino.h>

#ifndef __CHARS_H__
	#define __CHARS_H__

	byte clock[8] = {
		0b00000000,
		0b00001110,
		0b00010101,
		0b00010111,
		0b00010001,
		0b00001110,
		0b00000000,
		0b00000000,
	};
	byte bell[8] = {
		0b00000000,
		0b00000100,
		0b00001110,
		0b00001110,
		0b00011111,
		0b00000000,
		0b00000100,
		0b00000000
	};
#endif // __CHARS_H__
