//
//  ViewController.m
//  EngineeringNotationFormatter
//
//  Created by David Hoerl on 4/17/13.
//  Copyright (c) 2013 dhoerl. All rights reserved.
//

#import "ViewController.h"
#import "DHengNotation.h"


@interface ViewController ()

@end

@implementation ViewController
{
	IBOutlet UILabel *digitsLabel;
	IBOutlet UILabel *actualLabel;
	IBOutlet UILabel *processedLabel;
	IBOutlet UILabel *reversedLabel;
	IBOutlet UISegmentedControl *numericSegControl;
	IBOutlet UISlider *digitSlider;
	IBOutlet UISlider *valueSlider;
	
	int stepValue;
}

- (void)viewDidLoad
{
    [super viewDidLoad];

	[self digitsAction:digitSlider];
	[self valueAction:valueSlider];
	[self update];
}

// IBActions
- (IBAction)numericAction:(id)sender
{
	[self update];
}
- (IBAction)digitsAction:(id)sender
{
	int digits = digitSlider.value;
	digitsLabel.text = [NSString stringWithFormat:@"%d", digits];
	
	[self update];
}
- (IBAction)valueAction:(id)sender
{
	double value = [self rawToExp:valueSlider.value];
	actualLabel.text = [NSString stringWithFormat:@"%.9e", value];

	[self update];
}
- (IBAction)stepper:(id)sender
{
	int newVal = ((UIStepper *)sender).value;
	BOOL positive = newVal >= stepValue ? YES : NO;
	stepValue = newVal;

	BOOL numeric = [numericSegControl selectedSegmentIndex] ? YES : NO;
	processedLabel.text = DHStepEngineeringString(processedLabel.text, digitSlider.value, numeric, positive);
}

- (void)update
{
	BOOL numeric = [numericSegControl selectedSegmentIndex] ? YES : NO;
	unsigned int digits = digitSlider.value;
	double value = [self rawToExp:valueSlider.value];
	
	NSString *engStr = DHToEngineeringString(value, digits, numeric);
	//NSLog(@"val=%f digits=%d numeric=%d", value, digits, numeric);
	processedLabel.text = engStr;
	double rev = DHFromEngineeringString(engStr);
	reversedLabel.text = [NSString stringWithFormat:@"%.9e", rev];
	
	// NSLog(@"STEP: %@", DHStepEngineeringString(engStr, digits, numeric, YES));
	
}

- (double)rawToExp:(double)raw
{
	double value = pow(10, raw);
	return value;
}

@end
