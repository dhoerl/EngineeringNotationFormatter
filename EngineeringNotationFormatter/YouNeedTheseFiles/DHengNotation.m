//
// DHengNotation.m
// EngineeringNotationFormatter
//
// Copyright (C) 2013 by David Hoerl
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

#import "DHengNotation.h"

#import "EngNotation.h"

NSString *DHdbl2eng(double value, int digits, BOOL numeric)
{
	NSString *ret;
	char *str = dbl2eng(value, digits, (bool)numeric);
	
	if(str) {
		ret = [[NSString alloc] initWithBytesNoCopy:str length:strlen(str) encoding:NSUTF8StringEncoding freeWhenDone:YES];
	}
	return ret;
}

double DHeng2dbl(NSString *str)
{
	double ret = eng2dbl([str cStringUsingEncoding:NSUTF8StringEncoding]);
	return ret;
}


NSString *DFstepEng(NSString *oldEng, int digits, BOOL numeric, BOOL positive)
{
	NSString *ret;
	char *str = stepEng([oldEng cStringUsingEncoding:NSUTF8StringEncoding], digits, numeric, positive);
	if(str) {
		ret = [[NSString alloc] initWithBytesNoCopy:str length:strlen(str) encoding:NSUTF8StringEncoding freeWhenDone:YES];
	}
	return ret;
}
