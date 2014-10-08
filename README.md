Constant Time Encoding
======================

Constant time hex and base64, encode and decode.

Base64 encoding with these character sets:
* "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" base64Encode() and base64Decode()
* "./ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" base64EncodeDotSlash() and base64DecodeDotSlash()
* "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz" base64EncodeDotSlashOrdered() and base64DecodeDotSlashOrdered()

Semi-tested
===========
It looks correct there are some tests but there might be bugs.

PHP
===
I'm pretty sure there are problems with PHP and multi-byte character strings. I think it's something screwy with strlen() and maybe ord() and chr(). So you should probably not use this yet.
