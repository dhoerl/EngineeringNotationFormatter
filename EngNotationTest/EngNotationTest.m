//
//  EngNotationTest.m
//  EngNotationTest
//
//  Created by David Hoerl on 6/20/13.
//  Copyright (c) 2013 David Hoerl
//  Copyright (C) 2013 by Martin Moene (original set of tests)
//

#import "EngNotationTest.h"

#import "EngNotation.h"


@implementation EngNotationTest

- (void)setUp
{
    [super setUp];
    
    // Set-up code here.
	[super raiseAfterFailure];
	//[super continueAfterFailure];
}

- (void)tearDown
{
    // Tear-down code here.
    
    [super tearDown];
}

# define CAT2(X, Y) X ## Y
# define CAT(X, Y) CAT2(X, Y)

//#define TEST(x) - (void) test ## __COUNTER__ _ ## x
//#define TEST(x) - (void) test ## __COUNTER__ _ ## x

#if 0
bool approx( double const a, double const b )
{
    const double scale = 1.0;
    const double epsilon = 1 * DBL_EPSILON;

    bool ret = fabs(a - b) < epsilon * (scale + fmax( fabs(a), fabs(b) ) );
	if(!ret) printf("APPROX FAILED: a=%lf b=%lf\n", a, b);
	return ret;
}
#else
bool approx( double const a, double const b )
{
	double dbls[2] = { a, b };
	
	for(int i=0; i<2; ++i) {
		int classify = fpclassify(dbls[i]);
		if(classify != FP_NORMAL) {
			switch(classify) {
			case FP_SUBNORMAL:
				printf("YIKES: subnormal!\n");
				return false;
			case FP_ZERO:
				break;
			default:
				printf("YIKES: unknown!\n");
				return false;
			case FP_INFINITE:
				printf("YIKES: Infinite\n");
				return false;
			case FP_NAN:
				printf("YIKES: NaN\n");
				return false;
			}
		}
	}

    double foo = a - b;
	return !isnormal(foo);
}
#endif


#define print(a, b) \
    {\
        char *c = b;\
		if(strcmp(a, c) ) printf("MISMATCH: \"%s\" != \"%s\"\n", a, c); \
		free(c);\
	}

#define ASSERT1(x) STAssertTrue(x, @"Mismatch1")
#define ASSERT2(x, y) {\
		char *z = y;\
		STAssertFalse( strcmp(x, y), @"Mismatch2: created value \"%s\"", z);\
		free(z);\
	}


//#define to_engineering_string to_engineering_string
//#define from_engineering_string from_engineering_string
//#define eng_prefixed false
//#define eng_exponential	true
//#define step_engineering_string step_engineering_string

#define TEST(x) - (void) CAT( CAT(test, __COUNTER__), x )

- (void)xtest
{
    //STFail(@"Unit tests are not implemented yet in EngNotationTest");
#if 0
	//printf("ENG: %s %s\n", "99.9 z", to_engineering_string(99.9e-21, 1, 0) );
	//printf("ENG: %s %s\n", "100. z", to_engineering_string(99.95e-21, 1, 0) );
	//printf("ENG: %s %s\n", "99.9 z", to_engineering_string(99.94e-21, 1, 0) );
	char *str = to_engineering_string( 99.9986e-21, 5, eng_prefixed );
	printf("STR: %s\n\n", str);
	str = to_engineering_string( 99.9985e-21, 5, eng_prefixed );
	printf("STR: %s\n\n", str);
	str = to_engineering_string( 98.9986e-21, 5, eng_prefixed );
	printf("STR: %s\n\n", str);

    str = to_engineering_string( 99.95e-21, 3, eng_prefixed );
	printf("STR: %s\n\n", str);
    str = to_engineering_string( 99.94e-21, 3, eng_prefixed );
	printf("STR: %s\n\n", str);
#endif

	char *str;
	//str = to_engineering_string( -999.9999e-27, 6, eng_prefixed );
	str = to_engineering_string( 1e98, 3, eng_exponential );
	printf("STR: %s\n\n", str);

	//double foo = from_engineering_string( "1 y" );
	//printf("FOO=%lf\n", foo);
	
	// printf("ENG: %s %s\n", "1.00 y", to_engineering_string(1e-24, 5, 0) );
	//printf("ENG: %s %s\n", "1.00 z", to_engineering_string(1e-21, 1, 0) );

	printf("DONE!!!\n");
}
	
TEST( basic_to )
{
    ASSERT2( "1.23 k", to_engineering_string( 1230, 3, false ) );
    ASSERT2( "1.23 k", to_engineering_string( 1230, 3, eng_prefixed ) );

    ASSERT2( "1.23e3", to_engineering_string( 1230, 3, true ) );
    ASSERT2( "1.23e3", to_engineering_string( 1230, 3, eng_exponential ) );

    ASSERT2( "0.00", to_engineering_string( 0, 3, eng_prefixed ) );
    ASSERT2( "0.00e0", to_engineering_string( 0, 3, eng_exponential ) );

    ASSERT2( "1.23 kPa", to_engineering_string_unit( 1230, 3, false, "Pa" ) );
    ASSERT2( "1.23 kPa", to_engineering_string_unit( 1230, 3, eng_prefixed, "Pa" ) );

    ASSERT2( "1.23e3 Pa", to_engineering_string_unit( 1230, 3, true, "Pa"  ) );
    ASSERT2( "1.23e3 Pa", to_engineering_string_unit( 1230, 3, eng_exponential, "Pa" ) );

    ASSERT2( "100e96", to_engineering_string( 1e98, 3, eng_exponential ) );
    ASSERT2( "-10.0e-99", to_engineering_string( -1e-98, 3, eng_exponential ) );

    ASSERT2( "999.999 Y", to_engineering_string( 999.999e24, 6, eng_prefixed ) );
    ASSERT2( "-1.00000 y", to_engineering_string( -999.9999e-27, 6, eng_prefixed ) );
};

TEST( basic_from )
{
    ASSERT1( approx( 98.76e-3, from_engineering_string( "98.76 m" ) ) );
    ASSERT1( approx( 98.76e-3, from_engineering_string( "98.76 ml" ) ) );
};

TEST( proper_rounding )
{
    ASSERT2( "100 z", to_engineering_string( 99.951e-21, 3, eng_prefixed ) );	// need the final 1 to force number past .950
    ASSERT2( "99.9 z", to_engineering_string( 99.949e-21, 3, eng_prefixed ) );

    ASSERT2( "100.09 z", to_engineering_string( 100.0949e-21, 5, eng_prefixed ) );
    ASSERT2( "99.999 z", to_engineering_string( 99.99851e-21, 5, eng_prefixed ) );
};

TEST( bad_data )
{
    ASSERT2( "NaN", to_engineering_string( strtod("NaN", NULL), 3, eng_prefixed ) );
    ASSERT2( "INFINITE", to_engineering_string( strtod("INFINITY", NULL), 3, eng_exponential ) );

    ASSERT2( "100e96", to_engineering_string( 1e98, 3, eng_prefixed ) );		// too big for eng prefix
    ASSERT2( "-10.0e-99", to_engineering_string( -1e-98, 3, eng_prefixed ) );	// too big for eng prefix

	ASSERT1( isnan(  from_engineering_string( " " ) ) );
	ASSERT1( isnan(  from_engineering_string( "Howdie" ) ) );
	ASSERT1( isnan(  from_engineering_string( "1 Q" ) ) );
	ASSERT1( fpclassify( from_engineering_string( "0" ) ) & FP_ZERO );
};

TEST( prefixes )
{
    ASSERT1( approx( 1e-24, from_engineering_string( "1 y" ) ) );
    ASSERT1( approx( 1e-21, from_engineering_string( "1 z" ) ) );
    ASSERT1( approx( 1e-18, from_engineering_string( "1 a" ) ) );
    ASSERT1( approx( 1e-15, from_engineering_string( "1 f" ) ) );
    ASSERT1( approx( 1e-12, from_engineering_string( "1 p" ) ) );
    ASSERT1( approx( 1e-9 , from_engineering_string( "1 n" ) ) );
    ASSERT1( approx( 1e-6 , from_engineering_string( "1 u" ) ) );
    ASSERT1( approx( 1e-3 , from_engineering_string( "1 m" ) ) );
    ASSERT1( approx( 1    , from_engineering_string( "1 "  ) ) );
    ASSERT1( approx( 1    , from_engineering_string( "1"   ) ) );
    ASSERT1( approx( 1e+3 , from_engineering_string( "1 k" ) ) );
    ASSERT1( approx( 1e+6 , from_engineering_string( "1 M" ) ) );
    ASSERT1( approx( 1e+9 , from_engineering_string( "1 G" ) ) );
    ASSERT1( approx( 1e+12, from_engineering_string( "1 T" ) ) );
    ASSERT1( approx( 1e+15, from_engineering_string( "1 P" ) ) );
    ASSERT1( approx( 1e+18, from_engineering_string( "1 E" ) ) );
    ASSERT1( approx( 1e+21, from_engineering_string( "1 Z" ) ) );
    ASSERT1( approx( 1e+24, from_engineering_string( "1 Y" ) ) );
};

TEST( exponents_easy )
{
    ASSERT2( "1.23 y", to_engineering_string( 1.23e-24, 3, eng_prefixed ) );
    ASSERT2( "1.23 z", to_engineering_string( 1.23e-21, 3, eng_prefixed ) );
    ASSERT2( "1.23 a", to_engineering_string( 1.23e-18, 3, eng_prefixed ) );
    ASSERT2( "1.23 f", to_engineering_string( 1.23e-15, 3, eng_prefixed ) );
    ASSERT2( "1.23 p", to_engineering_string( 1.23e-12, 3, eng_prefixed ) );
    ASSERT2( "1.23 n", to_engineering_string( 1.23e-9 , 3, eng_prefixed ) );
    ASSERT2( "1.23 u", to_engineering_string( 1.23e-6 , 3, eng_prefixed ) );
    ASSERT2( "1.23 m", to_engineering_string( 1.23e-3 , 3, eng_prefixed ) );
    ASSERT2( "1.23"  , to_engineering_string( 1.23    , 3, eng_prefixed ) );
    ASSERT2( "1.23 k", to_engineering_string( 1.23e+3 , 3, eng_prefixed ) );
    ASSERT2( "1.23 M", to_engineering_string( 1.23e+6 , 3, eng_prefixed ) );
    ASSERT2( "1.23 G", to_engineering_string( 1.23e+9 , 3, eng_prefixed ) );
    ASSERT2( "1.23 T", to_engineering_string( 1.23e+12, 3, eng_prefixed ) );
    ASSERT2( "1.23 P", to_engineering_string( 1.23e+15, 3, eng_prefixed ) );
    ASSERT2( "1.23 E", to_engineering_string( 1.23e+18, 3, eng_prefixed ) );
    ASSERT2( "1.23 Z", to_engineering_string( 1.23e+21, 3, eng_prefixed ) );
    ASSERT2( "1.23 Y", to_engineering_string( 1.23e+24, 3, eng_prefixed ) );
};

TEST( exponents_hard )
{
    // minimally 3 digits:
    // Result e.g. "1000 z" in stead of "1 a":
#ifdef __cplusplus
    std::cout << "Warning(" << __LINE__ << "): Printing failing prefix tests:" << std::endl;
#endif

    print( "1.00 y", to_engineering_string( 1e-24, 1, eng_prefixed ) );
    print( "1.00 z", to_engineering_string( 1e-21, 3, eng_prefixed ) );
    print( "1.00 a", to_engineering_string( 1e-18, 1, eng_prefixed ) );
    print( "1.00 f", to_engineering_string( 1e-15, 1, eng_prefixed ) );
    print( "1.00 p", to_engineering_string( 1e-12, 1, eng_prefixed ) );
    print( "1.00 n", to_engineering_string( 1e-9 , 1, eng_prefixed ) );
    print( "1.00 u", to_engineering_string( 1e-6 , 1, eng_prefixed ) );
    print( "1.00 m", to_engineering_string( 1e-3 , 1, eng_prefixed ) );
    print( "1.00"  , to_engineering_string( 1    , 1, eng_prefixed ) );
    print( "1.00 k", to_engineering_string( 1e+3 , 1, eng_prefixed ) );
    print( "1.00 M", to_engineering_string( 1e+6 , 1, eng_prefixed ) );
    print( "1.00 G", to_engineering_string( 1e+9 , 1, eng_prefixed ) );
    print( "1.00 T", to_engineering_string( 1e+12, 1, eng_prefixed ) );
    print( "1.00 P", to_engineering_string( 1e+15, 1, eng_prefixed ) );
    print( "1.00 E", to_engineering_string( 1e+18, 1, eng_prefixed ) );
    print( "1.00 Z", to_engineering_string( 1e+21, 1, eng_prefixed ) );
    print( "1.00 Y", to_engineering_string( 1e+24, 1, eng_prefixed ) );
};

TEST( round_trip )
{
    ASSERT1( approx( 54.32, from_engineering_string( to_engineering_string( 54.32, 4, false ) ) ) );
    ASSERT1( approx( 54.32, from_engineering_string( to_engineering_string( 54.32, 4, true  ) ) ) );

    ASSERT1( approx( 543.2e2, from_engineering_string( to_engineering_string( 543.2e2, 4, false ) ) ) );
    ASSERT1( approx( 543.2e2, from_engineering_string( to_engineering_string( 543.2e2, 4, true  ) ) ) );
};

TEST( step )
{
    ASSERT2( "1.01" , step_engineering_string( "1.0"  , 3, eng_prefixed, true ) );
    ASSERT2( "1.01e0", step_engineering_string( "1.0"  , 3, eng_exponential, true ) );

    ASSERT2( "1.01 k", step_engineering_string( "1.0 k", 3, eng_prefixed, true ) );
    ASSERT2( "1.01e3", step_engineering_string( "1.0 k", 3, eng_exponential, true ) );

    ASSERT2( "990 m" , step_engineering_string( "1.0"  , 3, eng_prefixed, false ) );
    ASSERT2( "990e-3", step_engineering_string( "1.0"  , 3, eng_exponential, false ) );

    ASSERT2( "990 k" , step_engineering_string( "1.0 M", 3, eng_prefixed, false ) );
    ASSERT2( "990e3" , step_engineering_string( "1.0 M", 3, eng_exponential, false ) );

    ASSERT2( "990"  , step_engineering_string( "1.0 k", 3, eng_prefixed, false ) );
    ASSERT2( "990e0" , step_engineering_string( "1.0 k", 3, eng_exponential, false ) );
};


@end
