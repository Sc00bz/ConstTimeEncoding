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

#ifndef CONST_TIME_BASE64_H
#define CONST_TIME_BASE64_H

void base64Encode               (char *dest, const void *src, size_t srcLen);
int  base64Decode               (void *dest, const char *src, size_t srcLen);
void base64EncodeDotSlash       (char *dest, const void *src, size_t srcLen);
int  base64DecodeDotSlash       (void *dest, const char *src, size_t srcLen);
void base64EncodeDotSlashOrdered(char *dest, const void *src, size_t srcLen);
int  base64DecodeDotSlashOrdered(void *dest, const char *src, size_t srcLen);

#endif
