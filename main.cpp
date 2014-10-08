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
#include "base64.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int testBase64()
{
	int     ret;
	bool    good;
	bool    hasErrors = false;
	uint8_t allData[64*6/8] = {0};
	uint8_t testAllData[64*6/8] = {0};
	char    allDataBase64[(sizeof(allData)+2)/3*4+1];
	char    allDataBase64DotSlash[(sizeof(allData)+2)/3*4+1];
	char    allDataBase64DotSlashOrdered[(sizeof(allData)+2)/3*4+1];

	for (int i = 0, j = 0; i < 64; i += 4, j += 3)
	{
		allData[j  ] = (uint8_t) (( i      << 2) | ((i + 1) >> 4));
		allData[j+1] = (uint8_t) (((i + 1) << 4) | ((i + 2) >> 2));
		allData[j+2] = (uint8_t) (((i + 2) << 6) |  (i + 3)      );
	}

	for (size_t size = 1; size <= sizeof(allData); size++)
	{
		base64Encode(allDataBase64, allData, size);
		printf("base64Encode(allData) = %s\n", allDataBase64);

		base64EncodeDotSlash(allDataBase64DotSlash, allData, size);
		printf("base64EncodeDotSlash(allData) = %s\n", allDataBase64DotSlash);

		base64EncodeDotSlashOrdered(allDataBase64DotSlashOrdered, allData, size);
		printf("base64EncodeDotSlash(allData) = %s\n\n", allDataBase64DotSlashOrdered);

		for (size_t i = 0; i < size; i++)
		{
			testAllData[i] = 0;
		}
		ret = base64Decode(testAllData, allDataBase64, strlen(allDataBase64));
		printf("base64Decode(allDataBase64) ret = %u: ", ret);
		good = true;
		for (size_t i = 0; i < size; i++)
		{
			if (allData[i] != testAllData[i])
			{
				good = false;
			}
		}
		if (ret != 0 || !good)
		{
			hasErrors = true;
		}
		printf("%s\n", (good ? "good" : "bad"));

		for (size_t i = 0; i < size; i++)
		{
			testAllData[i] = 0;
		}
		ret = base64DecodeDotSlash(testAllData, allDataBase64DotSlash, strlen(allDataBase64DotSlash));
		printf("base64DecodeDotSlash(allDataBase64DotSlash) ret = %u: ", ret);
		good = true;
		for (size_t i = 0; i < size; i++)
		{
			if (allData[i] != testAllData[i])
			{
				good = false;
			}
		}
		if (ret != 0 || !good)
		{
			hasErrors = true;
		}
		printf("%s\n", (good ? "good" : "bad"));

		for (size_t i = 0; i < size; i++)
		{
			testAllData[i] = 0;
		}
		ret = base64DecodeDotSlashOrdered(testAllData, allDataBase64DotSlashOrdered, strlen(allDataBase64DotSlashOrdered));
		printf("base64DecodeDotSlashOrdered(allDataBase64DotSlashOrdered) ret = %u: ", ret);
		good = true;
		for (size_t i = 0; i < size; i++)
		{
			if (allData[i] != testAllData[i])
			{
				good = false;
			}
		}
		if (ret != 0 || !good)
		{
			hasErrors = true;
		}
		printf("%s\n\n\n", (good ? "good" : "bad"));
	}

	printf("Should error:\n");
	ret = base64Decode(testAllData, allDataBase64DotSlash, sizeof(allDataBase64DotSlash) - 1);
	printf("base64Decode(allDataBase64DotSlash) ret = %u: %s\n", ret, (ret != 0 ? "good" : "bad"));
	if (ret == 0)
	{
		hasErrors = true;
	}
	ret = base64DecodeDotSlash(testAllData, allDataBase64, sizeof(allDataBase64) - 1);
	printf("base64DecodeDotSlash(allDataBase64) ret = %u: %s\n", ret, (ret != 0 ? "good" : "bad"));
	if (ret == 0)
	{
		hasErrors = true;
	}
	ret = base64DecodeDotSlashOrdered(testAllData, allDataBase64, sizeof(allDataBase64) - 1);
	printf("base64DecodeDotSlashOrdered(allDataBase64) ret = %u: %s\n", ret, (ret != 0 ? "good" : "bad"));
	if (ret == 0)
	{
		hasErrors = true;
	}

	if (hasErrors)
	{
		printf("*** FAILED ***\n");
	}
	else
	{
		printf("*** PASSED ***\n");
	}
	return hasErrors;
}

int testHex()
{
	int     ret;
	bool    good;
	bool    hasErrors = false;
	uint8_t allBytes[256];
	char    allBytesHex[2 * sizeof(allBytes) + 1];
	char    allBytesHexUpper[2 * sizeof(allBytes) + 1];

	for (int i = 0; i < sizeof(allBytes); i++)
	{
		allBytes[i] = (uint8_t) i;
	}

	hexEncode(allBytesHex, allBytes, sizeof(allBytes));
	printf("hexEncode(allBytes) = %s\n", allBytesHex);

	hexEncodeUpper(allBytesHexUpper, allBytes, sizeof(allBytes));
	printf("hexEncodeUpper(allBytes) = %s\n\n", allBytesHexUpper);

	for (int i = 0; i < sizeof(allBytes); i++)
	{
		allBytes[i] = 0;
	}
	ret = hexDecode(allBytes, allBytesHex, sizeof(allBytesHex) - 1);
	printf("hexDecode(allBytesHexLower) ret = %u: ", ret);
	good = true;
	for (int i = 0; i < sizeof(allBytes); i++)
	{
		if (i != allBytes[i])
		{
			good = false;
		}
	}
	if (ret != 0 || !good)
	{
		hasErrors = true;
	}
	printf("%s\n", (good ? "good" : "bad"));

	for (int i = 0; i < sizeof(allBytes); i++)
	{
		allBytes[i] = 0;
	}
	ret = hexDecode(allBytes, allBytesHexUpper, sizeof(allBytesHexUpper) - 1);
	printf("hexDecode(allBytesHexUpper) ret = %u: ", ret);
	good = true;
	for (int i = 0; i < sizeof(allBytes); i++)
	{
		if (i != allBytes[i])
		{
			good = false;
		}
	}
	if (ret != 0 || !good)
	{
		hasErrors = true;
	}
	printf("%s\n", (good ? "good" : "bad"));

	for (int i = 0; i < sizeof(allBytes); i++)
	{
		allBytes[i] = 0;
	}
	ret = hexDecodeLower(allBytes, allBytesHex, sizeof(allBytesHex) - 1);
	printf("hexDecodeLower(allBytesHexLower) ret = %u: ", ret);
	good = true;
	for (int i = 0; i < sizeof(allBytes); i++)
	{
		if (i != allBytes[i])
		{
			good = false;
		}
	}
	if (ret != 0 || !good)
	{
		hasErrors = true;
	}
	printf("%s\n", (good ? "good" : "bad"));

	for (int i = 0; i < sizeof(allBytes); i++)
	{
		allBytes[i] = 0;
	}
	ret = hexDecodeUpper(allBytes, allBytesHexUpper, sizeof(allBytesHexUpper) - 1);
	printf("hexDecodeUpper(allBytesHexUpper) ret = %u: ", ret);
	good = true;
	for (int i = 0; i < sizeof(allBytes); i++)
	{
		if (i != allBytes[i])
		{
			good = false;
		}
	}
	if (ret != 0 || !good)
	{
		hasErrors = true;
	}
	printf("%s\n", (good ? "good" : "bad"));

	printf("\nShould error:\n");
	ret = hexDecodeLower(allBytes, allBytesHexUpper, sizeof(allBytesHexUpper) - 1);
	printf("hexDecodeLower(allBytesHexUpper) ret = %u\n", ret, (ret != 0 ? "good" : "bad"));
	if (ret == 0)
	{
		hasErrors = true;
	}
	ret = hexDecodeUpper(allBytes, allBytesHex, sizeof(allBytesHex) - 1);
	printf("hexDecodeUpper(allBytesHexLower) ret = %u\n", ret, (ret != 0 ? "good" : "bad"));
	if (ret == 0)
	{
		hasErrors = true;
	}

	if (hasErrors)
	{
		printf("*** FAILED ***\n");
	}
	else
	{
		printf("*** PASSED ***\n");
	}
	return hasErrors;
}

int main()
{
	bool hasErrors;

	printf("*** Base64 ***\n");
	hasErrors = testBase64() != 0;

	printf("\n*** Hex ***\n");
	hasErrors = testHex() != 0 || hasErrors;
	
	if (hasErrors)
	{
		printf("\n***** FAILED *****\n");
	}
	else
	{
		printf("\n***** ALL GOOD *****\n");
	}
	return 0;
}
