#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

struct Color{
	uint8_t R;
	uint8_t G;
	uint8_t B;
	uint8_t A;

	uint32_t GetHash(){
		return R<<24 | B <<16 | G << 8 | 255;
	}
};

#endif
