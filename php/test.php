<?php

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

header('Content-Type: text/plain');
require_once('base64.php');
require_once('hex.php');

function testBase64()
{
	$hasErrors = false;
	$allData = '';
	for ($i = 0; $i < 64; $i += 4)
	{
		$allData .=
			chr(( $i      << 2) | (($i + 1) >> 4)) .
			chr((($i + 1) << 4) | (($i + 2) >> 2)) .
			chr((($i + 2) << 6) |  ($i + 3)      );
	}

	for ($size = 1; $size <= 48; $size++)
	{
		$allDataBase64 = base64Encode(substr($allData, 0, $size));
		printf("base64Encode(allData) = %s\n", $allDataBase64);

		$allDataBase64DotSlash = base64EncodeDotSlash(substr($allData, 0, $size));
		printf("base64EncodeDotSlash(allData) = %s\n", $allDataBase64DotSlash);

		$allDataBase64DotSlashOrdered = base64EncodeDotSlashOrdered(substr($allData, 0, $size));
		printf("base64EncodeDotSlashOrdered(allData) = %s\n\n", $allDataBase64DotSlashOrdered);

		$testAllData = base64Decode($allDataBase64);
		printf("base64Decode(allDataBase64) ret = %u: ", ($testAllData === false ? 1 : 0));
		$good = true;
		for ($i = 0; $i < $size; $i++)
		{
			if ($testAllData[$i] != $allData[$i])
			{
				$good = false;
			}
		}
		if ($testAllData === false || !$good)
		{
			$hasErrors = true;
		}
		printf("%s\n", ($good ? "good" : "bad"));

		$testAllData = base64DecodeDotSlash($allDataBase64DotSlash);
		printf("base64DecodeDotSlash(allDataBase64DotSlash) ret = %u: ", ($testAllData === false ? 1 : 0));
		$good = true;
		for ($i = 0; $i < $size; $i++)
		{
			if ($testAllData[$i] != $allData[$i])
			{
				$good = false;
			}
		}
		if ($testAllData === false || !$good)
		{
			$hasErrors = true;
		}
		printf("%s\n", ($good ? "good" : "bad"));

		$testAllData = base64DecodeDotSlashOrdered($allDataBase64DotSlashOrdered);
		printf("base64DecodeDotSlashOrdered(allDataBase64DotSlashOrdered) ret = %u: ", ($testAllData === false ? 1 : 0));
		$good = true;
		for ($i = 0; $i < $size; $i++)
		{
			if ($testAllData[$i] != $allData[$i])
			{
				$good = false;
			}
		}
		if ($testAllData === false || !$good)
		{
			$hasErrors = true;
		}
		printf("%s\n\n\n", ($good ? "good" : "bad"));
	}

	printf("Should error:\n");
	$testAllData = base64Decode($allDataBase64DotSlash);
	printf("base64Decode(allDataBase64DotSlash) ret = %u: %s\n", ($testAllData === false ? 1 : 0), ($testAllData === false ? "good" : "bad"));
	if ($testAllData !== false)
	{
		$hasErrors = true;
	}
	$testAllData = base64DecodeDotSlash($allDataBase64);
	printf("base64DecodeDotSlash(allDataBase64) ret = %u: %s\n", ($testAllData === false ? 1 : 0), ($testAllData === false ? "good" : "bad"));
	if ($testAllData !== false)
	{
		$hasErrors = true;
	}
	$testAllData = base64DecodeDotSlashOrdered($allDataBase64);
	printf("base64DecodeDotSlashOrdered(allDataBase64) ret = %u: %s\n", ($testAllData === false ? 1 : 0), ($testAllData === false ? "good" : "bad"));
	if ($testAllData !== false)
	{
		$hasErrors = true;
	}

	if ($hasErrors)
	{
		printf("*** FAILED ***\n");
	}
	else
	{
		printf("*** PASSED ***\n");
	}
	return $hasErrors;
}

function testHex()
{
	$hasErrors = false;
	$allBytes = '';
	for ($i = 0; $i < 256; $i++)
	{
		$allBytes .= chr($i);
	}

	$allBytesHex = hexEncode($allBytes);
	printf("hexEncode(allBytes) = %s\n", $allBytesHex);

	$allBytesHexUpper = hexEncodeUpper($allBytes);
	printf("hexEncodeUpper(allBytes) = %s\n\n", $allBytesHexUpper);


	$allBytes = hexDecode($allBytesHex);
	printf("hexDecode(allBytesHexLower) ret = %u: ", ($allBytes === false ? 1 : 0));
	$good = true;
	for ($i = 0; $i < 256; $i++)
	{
		if ($i != ord($allBytes[$i]))
		{
			$good = false;
		}
	}
	if ($allBytes === false || !$good)
	{
		$hasErrors = true;
	}
	printf("%s\n", ($good ? "good" : "bad"));


	$allBytes = hexDecode($allBytesHexUpper);
	printf("hexDecode(allBytesHexUpper) ret = %u: ", ($allBytes === false ? 1 : 0));
	$good = true;
	for ($i = 0; $i < 256; $i++)
	{
		if ($i != ord($allBytes[$i]))
		{
			$good = false;
		}
	}
	if ($allBytes === false || !$good)
	{
		$hasErrors = true;
	}
	printf("%s\n", ($good ? "good" : "bad"));


	$allBytes = hexDecodeLower($allBytesHex);
	printf("hexDecodeLower(allBytesHexLower) ret = %u: ", ($allBytes === false ? 1 : 0));
	$good = true;
	for ($i = 0; $i < 256; $i++)
	{
		if ($i != ord($allBytes[$i]))
		{
			$good = false;
		}
	}
	if ($allBytes === false || !$good)
	{
		$hasErrors = true;
	}
	printf("%s\n", ($good ? "good" : "bad"));


	$allBytes = hexDecodeUpper($allBytesHexUpper);
	printf("hexDecodeUpper(allBytesHexUpper) ret = %u: ", ($allBytes === false ? 1 : 0));
	$good = true;
	for ($i = 0; $i < 256; $i++)
	{
		if ($i != ord($allBytes[$i]))
		{
			$good = false;
		}
	}
	if ($allBytes === false || !$good)
	{
		$hasErrors = true;
	}
	printf("%s\n", ($good ? "good" : "bad"));


	printf("\nShould error:\n");
	$allBytes = hexDecodeLower($allBytesHexUpper);
	printf("hexDecodeLower(allBytesHexUpper) ret = %u\n", ($allBytes === false ? 1 : 0), ($allBytes === false ? "good" : "bad"));
	if ($allBytes !== false)
	{
		$hasErrors = true;
	}
	$allBytes = hexDecodeUpper($allBytesHex);
	printf("hexDecodeUpper(allBytesHexLower) ret = %u\n", ($allBytes === false ? 1 : 0), ($allBytes === false ? "good" : "bad"));
	if ($allBytes !== false)
	{
		$hasErrors = true;
	}

	if ($hasErrors)
	{
		printf("*** FAILED ***\n");
	}
	else
	{
		printf("*** PASSED ***\n");
	}
	return $hasErrors;
}


printf("*** Base64 ***\n");
$hasErrors = testBase64() != 0;

printf("\n*** Hex ***\n");
$hasErrors = testHex() != 0 || $hasErrors;

if ($hasErrors)
{
	printf("\n***** FAILED *****\n");
}
else
{
	printf("\n***** ALL GOOD *****\n");
}
