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
	//double display, fract;
	char *sign;

	if(digits < 3) {
		digits = 3;
	} else
	if(digits > 9) {
		digits = 9;
	}

	if(!isnormal(value)) {
	
	
	}
	
	
	if(value < 0.0) {
		sign = "-";
		value = fabs(value);
	} else {
		sign = "";
	}
printf("\n\n\n");
	// correctly round to desired precision
	int expof10 = lrint( floor( log10(value) ) );
//printf("expof10=%d\n", expof10);

#if 0
							int power = (int)digits - 1 - expof10;
						printf("power=%d\n", power);

						printf("value=%lf\n", value);
							value *= pow(10.0, power);
						printf("...value=%lf\n", value);

							fract = modf(value, &display);
						printf("fract=%lf\n", fract);
							if(fract >= 0.5) {
								display += 1.0;
						printf("BUMP display %lf\n", display);
							}

							value = display * pow(10.0, -power);	//  expof10 - digits + 1
						printf("FINAL value=%lf\n", value);
#endif

	if(expof10 > 0) {
		expof10 = (expof10/3)*3;
printf("expof10-1 =%d\n", expof10);
	} else {
		expof10 = ((-expof10+3)/3)*(-3);
printf("expof10-2 =%d\n", expof10);
	}

	value *= pow(10.0, -expof10);
printf("value=%lf\n", value);

	long lintgr, lfract;
	{
		double intgr, fract;
		// doublepower; 
		
		fract = modf(value, &intgr);
		lintgr = lrint(intgr);
printf("compute: %ld . %lf\n", lintgr, fract);
	
		if(lintgr >= 1000) {
			digits -= 3;				// fractional digits
		} else
		if(lintgr >= 100) {
printf("lvalue > 100\n");
			digits -= 2;				// fractional digits
		} else
		if(lintgr >= 10) {
printf("is > 10\n");
			digits -= 1;				// fractional digits
		} else {
printf("is > 10\n");
			assert(!"Impossible to get < 1!");
		}
		
		double fractMult = pow(10.0, (int)digits - 1);
		long lfractMult = lrint(fractMult);
printf("digits=%d fractMult=%lf lfractMult=%ld\n", digits, fractMult, lfractMult);

		// round the fraction to the correct number of places
		fract *= fractMult;
		lfract = lrint(fract);

printf("lintgr=%ld lfract=%ld\n", lintgr, lfract);

		// did the rounding the fractional component cause an increase in the integral value?
		if(lfract >= lfractMult) {
printf("ROUND\n");
			lfract -= lfractMult;			// remove overflow value

			long nlintgr = lintgr + 1;
			if( (lintgr < 1000 && nlintgr >= 1000) || (lintgr < 100 && nlintgr >= 100) || (lintgr < 10 && nlintgr >= 10) || (lintgr < 1 && nlintgr >= 1) ) {
printf("WRAPPER\n");
				lfract /= 10;
				lfractMult /= 10;
				fractMult /= 10;
				digits -= 1;
			}
			lintgr = nlintgr;				// rounded up, so increase integral part
		}

		if(lintgr >= 1000) {
printf(">1000!!!\n");
			expof10 += 3;
			digits += 3;
			long fullVal = lrint(lintgr*fractMult) + lfract;
			long fullMult = lrintf(1000.0 * fractMult);
printf("lfractMult=%ld fullVal=%ld fullMult=%ld\n", lfractMult, fullVal, fullMult);
			lintgr = fullVal / fullMult;
			lfract = fullVal - (lintgr * fullMult);
		}
printf("FractionalMult=%ld\n", lfractMult);
	}
	
	char *result;
	if(numeric || (expof10 < PREFIX_START) || (expof10 > PREFIX_END)) {
printf("RESULT 1: digits=%d\n", digits-1);
		asprintf(&result, "%s%ld.%0.*lde%d", sign, lintgr, digits-1, lfract, expof10);
		//asprintf(&result, "%s%.*fe%d", sign, digits-1, value, expof10);
	} else {
printf("RESULT 2: digits=%d\n", digits-1);
		const char *s = prefix[(expof10-PREFIX_START)/3];
		asprintf(&result, "%s%ld.%0.*ld%s%s", sign, lintgr, digits-1, lfract, *s ? " " : "", s);
		//asprintf(&result, "%s%.*f%s%s", sign, digits-1, value, s);
		//asprintf(&result, "%s%.*f %s", sign, digits-1, value, s);
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
				if(!plen) continue;	// no prefix
				size_t strt = len - plen;
				if(!strt) continue;	// malformed
				const char *tst = &val[strt];
				if(!strcmp(tst, p)) {
					// need 4+null for new string
					tmp = (char *)malloc(strt + 4 + 1);
					strncpy(tmp, val, strt-1);	// -1 removes the space
					tmp[strt-1] = '\0';
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


#if 0
//value += .000001;

	// Now going to "pre-round" the value, within one power
	int compute = 1;
	for(int loops = 0; loops <= 10; ++loops) {
		if(loops == 10) return NULL;
		double intgr, fract;
		long lintgr, lfract;
		
		if(compute) {
			fract = modf(value, &intgr);
			lintgr = lrint(intgr);
printf("compute: %ld . %lf\n", lintgr, fract);
			if(lintgr >= 1000) {
printf("lvalue > 1000\n");
				value /= 1000.0;
				expof10 += 3;
				compute = 1;
				continue;
			} else
			if(lintgr < 1) {
				// protection code, should never happen
printf("is < 1\n");
				value *= 1000.0;
				expof10 -= 3;
				compute = 1;
				continue;
			}
			compute = 0;

		}
		
		if(lintgr >= 100) {
printf("lvalue > 100\n");
			digits -= 2;				// fractional digits
		} else
		if(lintgr >= 10) {
printf("is > 10\n");
			digits -= 1;				// fractional digits
		} else {
printf("is > 10\n");
		}
		
		double fractMult = pow(10.0, digits);
		long lfractMult = lrint(fractMult);
		if(lfractMult > 1) {
			// round the fraction to the correct number of places
			fract *= fractMult;
			lfract = lrint(fract);
			if(lfract >= lfractMult) {
		
		
		
		
		}
		
		
		
	}

#if 0
		unsigned int ldigits;
		long lvalue = lrint(value);	// correct or may round up
		if (lvalue >= 1000) {
printf("lvalue > 1000\n");
			ldigits = digits;
			value /= 1000.0;
		} else
		if(lvalue >= 100) {
printf("lvalue > 100\n");
			ldigits = digits - 1;
		} else
		if(lvalue >= 10) {
printf("is > 10\n");
			digits -= 1;
		}
	
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
#endif
	
	
	
	}



#if 0
		unsigned int ldigits;
		long lvalue = lrint(value);	// correct or may round up
		if (lvalue >= 1000) {
printf("lvalue > 1000\n");
			ldigits = digits;
			value /= 1000.0;
		} else
		if(lvalue >= 100) {
printf("lvalue > 100\n");
			ldigits = digits - 1;
		} else
		if(lvalue >= 10) {
printf("is > 10\n");
			digits -= 1;
		}
	
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
#endif

#endif

