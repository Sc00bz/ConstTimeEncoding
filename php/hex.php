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

// ************************
// *** Helper Functions ***
// ************************

function hexDecodeNibble($src)
{
	// 0-9  0x30-0x39
	// A-F  0x41-0x46 or a-f  0x61-0x66
	$ret = -1;

	// if ($src > 0x2f && $src < 0x3a) $ret += $src - 0x30 + 1; // -47
	$ret += (((0x2f - $src) & ($src - 0x3a)) >> 8) & ($src - 47);

	$src |= 0x20;

	// if ($src > 0x60 && $src < 0x67) $ret += $src - 0x61 + 10 + 1; // -86
	$ret += (((0x60 - $src) & ($src - 0x67)) >> 8) & ($src - 86);

	return $ret;
}

function hexDecodeNibbleLower($src)
{
	// 0-9  0x30-0x39
	// a-f  0x61-0x66
	$ret = -1;

	// if ($src > 0x2f && $src < 0x3a) $ret += $src - 0x30 + 1; // -47
	$ret += (((0x2f - $src) & ($src - 0x3a)) >> 8) & ($src - 47);

	// if ($src > 0x60 && $src < 0x67) $ret += $src - 0x61 + 10 + 1; // -86
	$ret += (((0x60 - $src) & ($src - 0x67)) >> 8) & ($src - 86);

	return $ret;
}

function hexDecodeNibbleUpper($src)
{
	// 0-9  0x30-0x39
	// A-F  0x41-0x46
	$ret = -1;

	// if ($src > 0x2f && $src < 0x3a) $ret += $src - 0x30 + 1; // -47
	$ret += (((0x2f - $src) & ($src - 0x3a)) >> 8) & ($src - 47);

	// if ($src > 0x40 && $src < 0x47) $ret += $src - 0x41 + 10 + 1; // -54
	$ret += (((0x40 - $src) & ($src - 0x47)) >> 8) & ($src - 54);

	return $ret;
}

function hexEncodeNibbleLower($src)
{
	// 0-9  0x30-0x39
	// a-f  0x61-0x66
	$src += 0x30;

	// if (in > 0x39) in += 0x61 - 0x3a;
	$src += ((0x39 - $src) >> 8) & (0x61 - 0x3a);

	return chr($src);
}

function hexEncodeNibbleUpper($src)
{
	// 0-9  0x30-0x39
	// A-F  0x41-0x46
	$src += 0x30;

	// if (in > 0x39) in += 0x41 - 0x3a;
	$src += ((0x39 - $src) >> 8) & (0x41 - 0x3a);

	return chr($src);
}



// **********************
// *** Main Functions ***
// **********************

function hexDecode($src)
{
	$err = 0;
	$srcLen = strlen($src);
	if ($srcLen % 2 != 0)
	{
		return 1;
	}
	$dest = '';
	for ($i = 0; $i < $srcLen; $i += 2)
	{
		$tmp = (hexDecodeNibble(ord($src[$i + 0])) << 4) | hexDecodeNibble(ord($src[$i + 1]));
		$err |= $tmp >> 8;
		$dest .= chr($tmp & 0xff);
	}
	if ($err != 0)
	{
		return false;
	}
	return $dest;
}

function hexDecodeLower($src)
{
	$err = 0;
	$srcLen = strlen($src);
	if ($srcLen % 2 != 0)
	{
		return 1;
	}
	$dest = '';
	for ($i = 0; $i < $srcLen; $i += 2)
	{
		$tmp = (hexDecodeNibbleLower(ord($src[$i + 0])) << 4) | hexDecodeNibbleLower(ord($src[$i + 1]));
		$err |= $tmp >> 8;
		$dest .= chr($tmp & 0xff);
	}
	if ($err != 0)
	{
		return false;
	}
	return $dest;
}

function hexDecodeUpper($src)
{
	$err = 0;
	$srcLen = strlen($src);
	if ($srcLen % 2 != 0)
	{
		return 1;
	}
	$dest = '';
	for ($i = 0; $i < $srcLen; $i += 2)
	{
		$tmp = (hexDecodeNibbleUpper(ord($src[$i + 0])) << 4) | hexDecodeNibbleUpper(ord($src[$i + 1]));
		$err |= $tmp >> 8;
		$dest .= chr($tmp & 0xff);
	}
	if ($err != 0)
	{
		return false;
	}
	return $dest;
}

function hexEncode($src)
{
	$dest = '';
	$srcLen = strlen($src);
	for ($i = 0; $i < $srcLen; $i++)
	{
		$ch = ord($src[$i]);
		$dest .= hexEncodeNibbleLower($ch >> 4) . hexEncodeNibbleLower($ch & 0x0f);
	}
	return $dest;
}

function hexEncodeUpper($src)
{
	$dest = '';
	$srcLen = strlen($src);
	for ($i = 0; $i < $srcLen; $i++)
	{
		$ch = ord($src[$i]);
		$dest .= hexEncodeNibbleUpper($ch >> 4) . hexEncodeNibbleUpper($ch & 0x0f);
	}
	return $dest;
}
