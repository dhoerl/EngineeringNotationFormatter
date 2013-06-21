//
// EngNotation.c
// EngineeringNotationFormatter
//
// Copyright (C) 2005-2009 by Jukka Korpela
// Copyright (C) 2009-2013 by David Hoerl
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "EngNotation.h"

// Set to 1 for debugging info
#if 0
#define LOG(...) printf(__VA_ARGS__)
#else
#define LOG(...)
#endif


#define PREFIX_START (-24)
/* 
 * Smallest power of ten for which there is a prefix defined.
 * If the set of prefixes will be extended, change this constant
 * and update the table "prefix".
 */

#ifndef ENG_FORMAT_MICRO_GLYPH
# define ENG_FORMAT_MICRO_GLYPH "µ"
#endif

#define NUM_PREFIX 17
static char *prefix[NUM_PREFIX] = {
  "y", "z", "a", "f",
  "p", "n", ENG_FORMAT_MICRO_GLYPH, "m",
  "",  "k", "M", "G",
  "T", "P", "E", "Z",
  "Y"
};
static char *reversePrefix[NUM_PREFIX] = {
 "e-24", "e-21", "e-18", "e-15",
 "e-12", "e-9", "e-6", "e-3",
 "e0", "e3", "e6", "e9",
 "e12", "e15", "e18", "e21",
 "e24"
};
#define PREFIX_END (PREFIX_START+(int)((sizeof(prefix)/sizeof(char *)-1)*3))

static char *eng2exp(const char *val);


char *dbl2eng(double value, unsigned int digits, bool numeric)
{
	int is_signed = signbit(value);
	char *sign = is_signed ? "-" : "";
	
	int classify = fpclassify(value);
	if(classify != FP_NORMAL) {
		switch(classify) {
		case FP_SUBNORMAL:
		case FP_ZERO:
		default:
		{
			char *s;
			if(numeric) {
				asprintf(&s, "%s%.*fe0", sign, digits-1, 0.0);
			} else {
				asprintf(&s, "%s%.*f", sign, digits-1, 0.0);
			}
			return s;
		}
		case FP_INFINITE:
			return strdup("INFINITE");
			break;
		case FP_NAN:
			return strdup("NaN");
			break;
		}
	}
	if(is_signed) {
		value = fabs(value);
	}

	if(digits < 3) {
		digits = 3;
	} else
	if(digits > 9) {
		digits = 9;
	}
	LOG("\n\n");

	// correctly round to desired precision
	int expof10 = lrint( floor( log10(value) ) );
	if(expof10 > 0) {
		expof10 = (expof10/3)*3;
		LOG("GTO: expof10=%d\n", expof10);
	} else {
		expof10 = ((-expof10+3)/3)*(-3);
		LOG("LTE: expof10=%d\n", expof10);
	}

	value *= pow(10.0, -expof10);
	LOG("value=%lf\n", value);

	long lintgr, lfract;
	{
		double intgr, fract;
		
		fract = modf(value, &intgr);
		lintgr = lrint(intgr);
		LOG("compute: %ld . %lf\n", lintgr, fract);
	
		if(lintgr >= 1000) {
			digits -= 3;				// fractional digits
		} else
		if(lintgr >= 100) {
			LOG("lvalue > 100 (%ld)\n", lintgr);
			digits -= 2;				// fractional digits
		} else
		if(lintgr >= 10) {
			LOG("is > 10 (%ld)\n", lintgr);
			digits -= 1;				// fractional digits
		} else
		if(lintgr >= 1) {
			LOG("is > 1 (%ld)\n", lintgr);
		} else {
			LOG("YIKES: < 1\n");
			//assert(!"Impossible to get < 1 unless the fractional part is 1!");
			digits += 1;				// fractional digits
		}
		
		// how much to multiple the fraction to get it to round the one-off value
		double fractMult = pow(10.0, (int)digits - 1);
		long lfractMult = lrint(fractMult);
		LOG("digits=%d fractMult=%lf lfractMult=%ld\n", digits, fractMult, lfractMult);

		// round the fraction to the correct number of places
		fract *= fractMult;
		lfract = lrint(fract);
		LOG("lintgr=%ld lfract=%ld\n", lintgr, lfract);

		// did the rounding the fractional component cause an increase in the integral value?
		if(lfract >= lfractMult) {
			LOG("ROUND\n");
			lfract -= lfractMult;			// remove overflow value

			long nlintgr = lintgr + 1;
			if( (lintgr < 1000 && nlintgr >= 1000) || (lintgr < 100 && nlintgr >= 100) || (lintgr < 10 && nlintgr >= 10) || (lintgr < 1 && nlintgr >= 1) ) {
				LOG("WRAPPER\n");
				lfract /= 10;
				fractMult /= 10;
				digits -= 1;
			}
			lintgr = nlintgr;				// rounded up, so increase integral part
		}

		if(lintgr >= 1000) {
			LOG(">1000!!!\n");
			expof10 += 3;
			digits += 3;

			long fullVal = lrint(lintgr*fractMult) + lfract;
			long fullMult = lrintf(1000.0 * fractMult);
			LOG("fullVal=%ld fullMult=%ld\n", fullVal, fullMult);

			lintgr = fullVal / fullMult;
			lfract = fullVal - (lintgr * fullMult);
		}
		LOG("FractionalMult=%ld\n", lfractMult);
	}
	
	char *result;
	if(numeric || (expof10 < PREFIX_START) || (expof10 > PREFIX_END)) {
		LOG("RESULT 1: digits=%d\n", digits-1);
		asprintf(&result, "%s%ld.%0.*lde%d", sign, lintgr, digits-1, lfract, expof10);
	} else {
		LOG("RESULT 2: digits=%d\n", digits-1);
		const char *s = prefix[(expof10-PREFIX_START)/3];
		asprintf(&result, "%s%ld.%0.*ld%s%s", sign, lintgr, digits-1, lfract, *s ? " " : "", s);
	}
	return result;
}

// converts the output of dbl2eng() into a double
double eng2dbl(const char *val)
{
	char *tstVal = eng2exp(val);
	double ret = strtod(tstVal, NULL);
	free(tstVal);
	return ret;
}

char *stepEng(const char *val, unsigned int digits, bool numeric, bool positive)
{
	char *tstVal = eng2exp(val);
	double value = strtod(tstVal, NULL);
	free(tstVal);

	if(digits < 3) {
		digits = 3;
	}
	// correctly round to desired precision
	int expof10 = lrint( floor( log10(value) ) );
	int power =  expof10 + 1 - (int)digits;
	
	double inc = pow(10, power) * (positive ? +1 : -1);
	double ret = value + inc;
	
	char *str = dbl2eng(ret, digits, numeric);
	return str;
}

static char *eng2exp(const char *val)
{
	size_t len = strlen(val);
	if(!len || !isdigit(val[0])) {
		return strdup("NaN");
	}
	char *tmp = NULL;

	if(len >= 3) {
		// space and last value which might be a UTF-8 char. Formats are "1 y", "900.", "900. y", "1.23 M"
		int c = val[len-1];
		if(!isdigit(c) && c != '.') {
			int i;
			for(i=0; i<NUM_PREFIX; ++i) {
				const char *p = prefix[i];
				size_t plen = strlen(p);
				if(!plen) continue;	// no prefix
				size_t strt = len - plen;
				if(!strt) continue;	// malformed
				const char *tst = &val[strt];
				LOG("COMPARE %s to %s\n", tst, p);
				if(!strcmp(tst, p)) {
					// need 4+null for new string
					tmp = (char *)malloc(strt + 4 + 1);
					strncpy(tmp, val, strt-1);	// -1 removes the space
					tmp[strt-1] = '\0';
					strcat(tmp, reversePrefix[i]);
					break;
				}
			}
			if(i == NUM_PREFIX) {
				tmp = strdup("NaN");
			}
		}
	}
	if(!tmp) {
		// could be "1" (cheating, as the eng will not return a string < 3 chars)
		tmp = strdup(val);
	}
	LOG("eng2exp RETURN %s\n", tmp);
	return tmp;
}
