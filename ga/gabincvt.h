// $Header$
/* ----------------------------------------------------------------------------
  bincvt.h
  mbwall 29jun95
  Copyright (c) 1995 Massachusetts Institute of Technology
					 all rights reserved

 DESCRIPTION:
  Header for the binary to decimal conversion routines.  The binary-to-decimal
routines include encoders and decoders.  We define a standard binary encoder/
decoder set as well as one Gray encoder/decoder.  You can define your own if
you want a different Gray coding.
---------------------------------------------------------------------------- */
#pragma once

#include <gatypes.h>

// The encoder converts a decimal value into a binary string.  The decoder
// converts a string of bits into a decimal value.  Both types of functions
// return an error code to indicate whether or not the conversion was
// successful.  The caller must make sure that sufficient space is available
// for the arguments.  The encoder will set the value to whatever it was able
// to encode, so be sure to check the return status and make your value such
// that you can check it if you get a non-zero return code.
using GABinaryEncoder = int (*)(float &, GABit *, unsigned int, float, float);
using GABinaryDecoder = int (*)(float &, const GABit *, unsigned int, float, float);

// These are publicly available, but we don't want to advertise them.  They are
// mostly just for testing purposes.  These are unscaled versions of the
// converter routines.
int GABinaryEncode(float &, GABit *bits, unsigned int, float, float);
int GABinaryDecode(float &, const GABit *bits, unsigned int, float, float);

int GAGrayEncode(float &, GABit *bits, unsigned int, float, float);
int GAGrayDecode(float &, const GABit *bits, unsigned int, float, float);
