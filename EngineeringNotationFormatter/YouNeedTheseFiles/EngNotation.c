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

#include "EngNotation.h"

#define PREFIX_START (-24)
/* 
 * Smallest power of ten for which there is a prefix defined.
 * If the set of prefixes will be extended, change this constant
 * and update the table "prefix".
 */

#define NUM_PREFIX 17
static char *prefix[NUM_PREFIX] = {
  "y", "z", "a", "f",
  "p", "n", "µ", "m",
  "",  "k", "M", "G",
  "T", "P", "E", "Z",
  "Y"
};
static char *reversePrefix[NUM_PREFIX] = {
 "e-24", "e-21", "e-18", "e-15",
 "e-12", "e-9", "e-6", "e-3",
 "", "e3", "e6", "e9",
 "e12", "e15", "e18", "e21",
 "e24"
};
#define PREFIX_END (PREFIX_START+(int)((sizeof(prefix)/sizeof(char *)-1)*3))

static char *eng2exp(const char *val);

char *dbl2eng(double value, unsigned int digits, bool numeric)
{
	double display, fract;
	char *sign;

	if(digits < 3) {
		digits = 3;
	}

	if(value < 0.0) {
		sign = "-";
		value = fabs(value);
	} else {
		sign = "";
	}

	// correctly round to desired precision
	int expof10 = lrint( floor( log10(value) ) );

	int power = (int)digits - 1 - expof10;
	value *= pow(10.0, power);

	fract = modf(value, &display);
	if(fract >= 0.5) {
		display += 1.0;
	}

	value = display * pow(10.0, -power);	//  expof10 - digits + 1

	if(expof10 > 0) {
		expof10 = (expof10/3)*3;
	} else {
		expof10 = ((-expof10+3)/3)*(-3);
	}

	value *= pow(10.0, -expof10);
	if (value >= 1000.0) {
		value /= 1000.0;
		expof10 += 3;
	} else
	if(value >= 100.0) {
		digits -= 2;
	} else
	if(value >= 10.0) {
		digits -= 1;
	}
	
	char *result;
	if(numeric || (expof10 < PREFIX_START) || (expof10 > PREFIX_END)) {
		asprintf(&result, "%s%.*fe%d", sign, digits-1, value, expof10);
	} else {
		asprintf(&result, "%s%.*f %s", sign, digits-1, value, prefix[(expof10-PREFIX_START)/3]);
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
	char *tmp = NULL;

	if(len > 3) {
		// space and last value which might be a UTF-8 char
		int c = val[len-1];
		if(!isdigit(c)) {
			for(int i=0; i<NUM_PREFIX; ++i) {
				const char *p = prefix[i];
				size_t plen = strlen(p);
				size_t strt = len - plen;
				const char *tst = &val[strt];
				if(!strcmp(tst, p)) {
					// need 4+null for new string
					tmp = (char *)malloc(strt + 4 + 1);
					strncpy(tmp, val, strt);
					tmp[strt] = '\0';
					strcat(tmp, reversePrefix[i]);
					break;
				}
			}
		}
	}
	if(!tmp) {
		tmp = strdup(val);
	}
	return tmp;
}


