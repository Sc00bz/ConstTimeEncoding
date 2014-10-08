/*
	Copyright (c) 2014 Steve "Sc00bz" Thomas (steve at tobtu dot com)

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#include "hex.h"
#include <stdint.h>

// ************************
// *** Helper Functions ***
// ************************

inline int hexDecodeNibble(char src)
{
	// 0-9  0x30-0x39
	// A-F  0x41-0x46 or a-f  0x61-0x66
	int ch  = (unsigned char) src;
	int ret = -1;

	// if (ch > 0x2f && ch < 0x3a) ret += ch - 0x30 + 1; // -47
	ret += (((0x2f - ch) & (ch - 0x3a)) >> 8) & (ch - 47);

	ch |= 0x20;

	// if (ch > 0x60 && ch < 0x67) ret += ch - 0x61 + 10 + 1; // -86
	ret += (((0x60 - ch) & (ch - 0x67)) >> 8) & (ch - 86);

	return ret;
}

inline int hexDecodeNibbleLower(char src)
{
	// 0-9  0x30-0x39
	// a-f  0x61-0x66
	int ch  = (unsigned char) src;
	int ret = -1;

	// if (ch > 0x2f && ch < 0x3a) ret += ch - 0x30 + 1; // -47
	ret += (((0x2f - ch) & (ch - 0x3a)) >> 8) & (ch - 47);

	// if (ch > 0x60 && ch < 0x67) ret += ch - 0x61 + 10 + 1; // -86
	ret += (((0x60 - ch) & (ch - 0x67)) >> 8) & (ch - 86);

	return ret;
}

inline int hexDecodeNibbleUpper(char src)
{
	// 0-9  0x30-0x39
	// A-F  0x41-0x46
	int ch  = (unsigned char) src;
	int ret = -1;

	// if (ch > 0x2f && ch < 0x3a) ret += ch - 0x30 + 1; // -47
	ret += (((0x2f - ch) & (ch - 0x3a)) >> 8) & (ch - 47);

	// if (ch > 0x40 && ch < 0x47) ret += ch - 0x41 + 10 + 1; // -54
	ret += (((0x40 - ch) & (ch - 0x47)) >> 8) & (ch - 54);

	return ret;
}

inline int hexDecodeByte(const char src[2])
{
	return (hexDecodeNibble(src[0]) << 4) | hexDecodeNibble(src[1]);
}

inline int hexDecodeByteLower(const char src[2])
{
	return (hexDecodeNibbleLower(src[0]) << 4) | hexDecodeNibbleLower(src[1]);
}

inline int hexDecodeByteUpper(const char src[2])
{
	return (hexDecodeNibbleUpper(src[0]) << 4) | hexDecodeNibbleUpper(src[1]);
}

inline char hexEncodeNibbleLower(unsigned int src)
{
	// 0-9  0x30-0x39
	// a-f  0x61-0x66
	src += 0x30;

	// if (in > 0x39) in += 0x61 - 0x3a;
	src += ((0x39 - src) >> 8) & (0x61 - 0x3a);

	return (char) src;
}

inline char hexEncodeNibbleUpper(unsigned int src)
{
	// 0-9  0x30-0x39
	// A-F  0x41-0x46
	src += 0x30;

	// if (in > 0x39) in += 0x41 - 0x3a;
	src += ((0x39 - src) >> 8) & (0x41 - 0x3a);

	return (char) src;
}

inline void hexEncodeByteLower(char dest[2], uint8_t src)
{
	dest[0] = hexEncodeNibbleLower(src >> 4);
	dest[1] = hexEncodeNibbleLower(src & 0x0f);
}

inline void hexEncodeByteUpper(char dest[2], uint8_t src)
{
	dest[0] = hexEncodeNibbleUpper(src >> 4);
	dest[1] = hexEncodeNibbleUpper(src & 0x0f);
}


// **********************
// *** Main Functions ***
// **********************

int hexDecode(void *dest, const char *src, size_t srcLen)
{
	int err = 0;

	if (srcLen % 2 != 0)
	{
		return 1;
	}
	srcLen /= 2;
	for (size_t i = 0; i < srcLen; i++)
	{
		int tmp = hexDecodeByte(src + 2 * i);

		err |= tmp >> 8;
		((uint8_t*) dest)[i] = (uint8_t) tmp;
	}
	return err != 0;
}

int hexDecodeLower(void *dest, const char *src, size_t srcLen)
{
	int err = 0;

	if (srcLen % 2 != 0)
	{
		return 1;
	}
	srcLen /= 2;
	for (size_t i = 0; i < srcLen; i++)
	{
		int tmp = hexDecodeByteLower(src + 2 * i);

		err |= tmp >> 8;
		((uint8_t*) dest)[i] = (uint8_t) tmp;
	}
	return err != 0;
}

int hexDecodeUpper(void *dest, const char *src, size_t srcLen)
{
	int err = 0;

	if (srcLen % 2 != 0)
	{
		return 1;
	}
	srcLen /= 2;
	for (size_t i = 0; i < srcLen; i++)
	{
		int tmp = hexDecodeByteUpper(src + 2 * i);

		err |= tmp >> 8;
		((uint8_t*) dest)[i] = (uint8_t) tmp;
	}
	return err != 0;
}

void hexEncode(char *dest, const void *src, size_t srcLen)
{
	for (size_t i = 0; i < srcLen; i++)
	{
		hexEncodeByteLower(dest + 2 * i, ((const uint8_t*) src)[i]);
	}
	dest[2 * srcLen] = 0;
}

void hexEncodeUpper(char *dest, const void *src, size_t srcLen)
{
	for (size_t i = 0; i < srcLen; i++)
	{
		hexEncodeByteUpper(dest + 2 * i, ((const uint8_t*) src)[i]);
	}
	dest[2 * srcLen] = 0;
}
