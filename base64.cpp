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

#include "base64.h"
#include <stdint.h>

// ************************
// *** Helper Functions ***
// ************************

// Base64 character set:
// [A-Z]      [a-z]      [0-9]      +     /
// 0x41-0x5a, 0x61-0x7a, 0x30-0x39, 0x2b, 0x2f

inline int base64Decode6Bits(char src)
{
	int ch  = (unsigned char) src;
	int ret = -1;

	// if (ch > 0x40 && ch < 0x5b) ret += ch - 0x41 + 1; // -64
	ret += (((0x40 - ch) & (ch - 0x5b)) >> 8) & (ch - 64);

	// if (ch > 0x60 && ch < 0x7b) ret += ch - 0x61 + 26 + 1; // -70
	ret += (((0x60 - ch) & (ch - 0x7b)) >> 8) & (ch - 70);

	// if (ch > 0x2f && ch < 0x3a) ret += ch - 0x30 + 52 + 1; // 5
	ret += (((0x2f - ch) & (ch - 0x3a)) >> 8) & (ch + 5);

	// if (ch == 0x2b) ret += 62 + 1;
	ret += (((0x2a - ch) & (ch - 0x2c)) >> 8) & 63;

	// if (ch == 0x2f) ret += 63 + 1;
	ret += (((0x2e - ch) & (ch - 0x30)) >> 8) & 64;

	return ret;
}

inline int base64Decode3Bytes(uint8_t dest[3], const char src[4])
{
	int c0 = base64Decode6Bits(src[0]);
	int c1 = base64Decode6Bits(src[1]);
	int c2 = base64Decode6Bits(src[2]);
	int c3 = base64Decode6Bits(src[3]);

	dest[0] = (uint8_t) ((c0 << 2) | (c1 >> 4));
	dest[1] = (uint8_t) ((c1 << 4) | (c2 >> 2));
	dest[2] = (uint8_t) ((c2 << 6) |  c3      );
	return ((c0 | c1 | c2 | c3) >> 8) & 1;
}

inline char base64Encode6Bits(unsigned int src)
{
	int diff = 0x41;

	// if (in > 25) diff += 0x61 - 0x41 - 26; // 6
	diff += ((25 - src) >> 8) & 6;

	// if (in > 51) diff += 0x30 - 0x61 - 26; // -75
	diff -= ((51 - src) >> 8) & 75;

	// if (in > 61) diff += 0x2b - 0x30 - 10; // -15
	diff -= ((61 - src) >> 8) & 15;

	// if (in > 62) diff += 0x2f - 0x2b - 1; // 3
	diff += ((62 - src) >> 8) & 3;

	return (char) (src + diff);
}

inline void base64Encode3Bytes(char dest[4], const uint8_t src[3])
{
	unsigned int b0 = src[0];
	unsigned int b1 = src[1];
	unsigned int b2 = src[2];

	dest[0] = base64Encode6Bits(              b0 >> 2       );
	dest[1] = base64Encode6Bits(((b0 << 4) | (b1 >> 4)) & 63);
	dest[2] = base64Encode6Bits(((b1 << 2) | (b2 >> 6)) & 63);
	dest[3] = base64Encode6Bits(  b2                    & 63);
}


// Base64 character set:
// ./         [A-Z]      [a-z]     [0-9]
// 0x2e-0x2f, 0x41-0x5a, 0x61-0x7a, 0x30-0x39

inline int base64Decode6BitsDotSlash(char src)
{
	int ch  = (unsigned char) src;
	int ret = -1;

	// if (ch > 0x2d && ch < 0x30) ret += ch - 0x2e + 1; // -45
	ret += (((0x2d - ch) & (ch - 0x30)) >> 8) & (ch - 45);

	// if (ch > 0x40 && ch < 0x5b) ret += ch - 0x41 + 2 + 1; // -62
	ret += (((0x40 - ch) & (ch - 0x5b)) >> 8) & (ch - 62);

	// if (ch > 0x60 && ch < 0x7b) ret += ch - 0x61 + 28 + 1; // -68
	ret += (((0x60 - ch) & (ch - 0x7b)) >> 8) & (ch - 68);

	// if (ch > 0x2f && ch < 0x3a) ret += ch - 0x30 + 54 + 1; // 7
	ret += (((0x2f - ch) & (ch - 0x3a)) >> 8) & (ch + 7);

	return ret;
}

inline int base64Decode3BytesDotSlash(uint8_t dest[3], const char src[4])
{
	int c0 = base64Decode6BitsDotSlash(src[0]);
	int c1 = base64Decode6BitsDotSlash(src[1]);
	int c2 = base64Decode6BitsDotSlash(src[2]);
	int c3 = base64Decode6BitsDotSlash(src[3]);

	dest[0] = (uint8_t) ((c0 << 2) | (c1 >> 4));
	dest[1] = (uint8_t) ((c1 << 4) | (c2 >> 2));
	dest[2] = (uint8_t) ((c2 << 6) |  c3      );
	return ((c0 | c1 | c2 | c3) >> 8) & 1;
}

inline char base64Encode6BitsDotSlash(unsigned int src)
{
	src += 0x2e;

	// if (in > 0x2f) in += 0x41 - 0x30; // 17
	src += ((0x2f - src) >> 8) & 17;

	// if (in > 0x5a) in += 0x61 - 0x5b; // 6
	src += ((0x5a - src) >> 8) & 6;

	// if (in > 0x7a) in += 0x30 - 0x7b; // -75
	src -= ((0x7a - src) >> 8) & 75;

	return (char) src;
}

inline void base64Encode3BytesDotSlash(char dest[4], const uint8_t src[3])
{
	unsigned int b0 = src[0];
	unsigned int b1 = src[1];
	unsigned int b2 = src[2];

	dest[0] = base64Encode6BitsDotSlash(              b0 >> 2       );
	dest[1] = base64Encode6BitsDotSlash(((b0 << 4) | (b1 >> 4)) & 63);
	dest[2] = base64Encode6BitsDotSlash(((b1 << 2) | (b2 >> 6)) & 63);
	dest[3] = base64Encode6BitsDotSlash(  b2                    & 63);
}


// Base64 character set:
// [.-9]      [A-Z]      [a-z]
// 0x2e-0x39, 0x41-0x5a, 0x61-0x7a

inline int base64Decode6BitsDotSlashOrdered(char src)
{
	int ch  = (unsigned char) src;
	int ret = -1;

	// if (ch > 0x2d && ch < 0x3a) ret += ch - 0x2e + 1; // -45
	ret += (((0x2d - ch) & (ch - 0x3a)) >> 8) & (ch - 45);

	// if (ch > 0x40 && ch < 0x5b) ret += ch - 0x41 + 12 + 1; // -52
	ret += (((0x40 - ch) & (ch - 0x5b)) >> 8) & (ch - 52);

	// if (ch > 0x60 && ch < 0x7b) ret += ch - 0x61 + 38 + 1; // -58
	ret += (((0x60 - ch) & (ch - 0x7b)) >> 8) & (ch - 58);

	return ret;
}

inline int base64Decode3BytesDotSlashOrdered(uint8_t dest[3], const char src[4])
{
	int c0 = base64Decode6BitsDotSlashOrdered(src[0]);
	int c1 = base64Decode6BitsDotSlashOrdered(src[1]);
	int c2 = base64Decode6BitsDotSlashOrdered(src[2]);
	int c3 = base64Decode6BitsDotSlashOrdered(src[3]);

	dest[0] = (uint8_t) ((c0 << 2) | (c1 >> 4));
	dest[1] = (uint8_t) ((c1 << 4) | (c2 >> 2));
	dest[2] = (uint8_t) ((c2 << 6) |  c3      );
	return ((c0 | c1 | c2 | c3) >> 8) & 1;
}

inline char base64Encode6BitsDotSlashOrdered(unsigned int src)
{
	src += 0x2e;

	// if (in > 0x39) in += 0x41 - 0x3a; // 7
	src += ((0x39 - src) >> 8) & 7;

	// if (in > 0x5a) in += 0x61 - 0x5b; // 6
	src += ((0x5a - src) >> 8) & 6;

	return (char) src;
}

inline void base64Encode3BytesDotSlashOrdered(char dest[4], const uint8_t src[3])
{
	unsigned int b0 = src[0];
	unsigned int b1 = src[1];
	unsigned int b2 = src[2];

	dest[0] = base64Encode6BitsDotSlashOrdered(              b0 >> 2       );
	dest[1] = base64Encode6BitsDotSlashOrdered(((b0 << 4) | (b1 >> 4)) & 63);
	dest[2] = base64Encode6BitsDotSlashOrdered(((b1 << 2) | (b2 >> 6)) & 63);
	dest[3] = base64Encode6BitsDotSlashOrdered(  b2                    & 63);
}


// **********************
// *** Main Functions ***
// **********************

// Base64 character set "[A-Z][a-z][0-9]+/"
void base64Encode(char *dest, const void *src, size_t srcLen)
{
	for (; srcLen >= 3; srcLen -= 3)
	{
		base64Encode3Bytes(dest, (const uint8_t*) src);
		dest += 4;
		src   = (const uint8_t*) src + 3;
	}
	if (srcLen > 0)
	{
		uint8_t tmp[3] = {0, 0, 0};

		for (size_t i = 0; i < srcLen; i++)
		{
			tmp[i] = ((const uint8_t*) src)[i];
		}
		base64Encode3Bytes(dest, tmp);
		dest[3] = '=';
		if (srcLen == 1)
		{
			dest[2] = '=';
		}
		dest += 4;
	}
	*dest = 0;
}

int base64Decode(void *dest, const char *src, size_t srcLen)
{
	int err = 0;

	for (; srcLen > 4; srcLen -= 4)
	{
		err |= base64Decode3Bytes((uint8_t*) dest, src);
		dest  = (uint8_t*) dest + 3;
		src  += 4;
	}
	if (srcLen > 0)
	{
		size_t  i;
		uint8_t tmpOut[3];
		char    tmpIn[4] = {'A', 'A', 'A', 'A'};

		for (i = 0; i < srcLen && src[i] != '='; i++)
		{
			tmpIn[i] = src[i];
		}
		if (i < 2)
		{
			err = 1;
		}
		srcLen = i - 1;
		err |= base64Decode3Bytes(tmpOut, tmpIn);
		for (i = 0; i < srcLen; i++)
		{
			((uint8_t*) dest)[i] = tmpOut[i];
		}
	}
	return err;
}

// Base64 character set "./[A-Z][a-z][0-9]"
void base64EncodeDotSlash(char *dest, const void *src, size_t srcLen)
{
	for (; srcLen >= 3; srcLen -= 3)
	{
		base64Encode3BytesDotSlash(dest, (const uint8_t*) src);
		dest += 4;
		src   = (const uint8_t*) src + 3;
	}
	if (srcLen > 0)
	{
		uint8_t tmp[3] = {0, 0, 0};

		for (size_t i = 0; i < srcLen; i++)
		{
			tmp[i] = ((const uint8_t*) src)[i];
		}
		base64Encode3BytesDotSlash(dest, tmp);
		dest[3] = '=';
		if (srcLen == 1)
		{
			dest[2] = '=';
		}
		dest += 4;
	}
	*dest = 0;
}

int base64DecodeDotSlash(void *dest, const char *src, size_t srcLen)
{
	int err = 0;

	for (; srcLen > 4; srcLen -= 4)
	{
		err |= base64Decode3BytesDotSlash((uint8_t*) dest, src);
		dest  = (uint8_t*) dest + 3;
		src  += 4;
	}
	if (srcLen > 0)
	{
		size_t  i;
		uint8_t tmpOut[3];
		char    tmpIn[4] = {'A', 'A', 'A', 'A'};

		for (i = 0; i < srcLen && src[i] != '='; i++)
		{
			tmpIn[i] = src[i];
		}
		if (i < 2)
		{
			err = 1;
		}
		srcLen = i - 1;
		err |= base64Decode3BytesDotSlash(tmpOut, tmpIn);
		for (i = 0; i < srcLen; i++)
		{
			((uint8_t*) dest)[i] = tmpOut[i];
		}
	}
	return err;
}

// Base64 character set "[.-9][A-Z][a-z]" or "./[0-9][A-Z][a-z]"
void base64EncodeDotSlashOrdered(char *dest, const void *src, size_t srcLen)
{
	for (; srcLen >= 3; srcLen -= 3)
	{
		base64Encode3BytesDotSlashOrdered(dest, (const uint8_t*) src);
		dest += 4;
		src   = (const uint8_t*) src + 3;
	}
	if (srcLen > 0)
	{
		uint8_t tmp[3] = {0, 0, 0};

		for (size_t i = 0; i < srcLen; i++)
		{
			tmp[i] = ((const uint8_t*) src)[i];
		}
		base64Encode3BytesDotSlashOrdered(dest, tmp);
		dest[3] = '=';
		if (srcLen == 1)
		{
			dest[2] = '=';
		}
		dest += 4;
	}
	*dest = 0;
}

int base64DecodeDotSlashOrdered(void *dest, const char *src, size_t srcLen)
{
	int err = 0;

	for (; srcLen > 4; srcLen -= 4)
	{
		err |= base64Decode3BytesDotSlashOrdered((uint8_t*) dest, src);
		dest  = (uint8_t*) dest + 3;
		src  += 4;
	}
	if (srcLen > 0)
	{
		size_t  i;
		uint8_t tmpOut[3];
		char    tmpIn[4] = {'A', 'A', 'A', 'A'};

		for (i = 0; i < srcLen && src[i] != '='; i++)
		{
			tmpIn[i] = src[i];
		}
		if (i < 2)
		{
			err = 1;
		}
		srcLen = i - 1;
		err |= base64Decode3BytesDotSlashOrdered(tmpOut, tmpIn);
		for (i = 0; i < srcLen; i++)
		{
			((uint8_t*) dest)[i] = tmpOut[i];
		}
	}
	return err;
}
