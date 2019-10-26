/*

 ===============================================================================

 Name        : Assignment_5_Wind_Chime.c

 Author      : Gangalal Lama Dong, Daniel Glover, and Jessica Horner

 Version     : V.3

 Copyright   : $(copyright)

 Description : main definition

 ===============================================================================

 */

#ifdef __USE_CMSIS

#include "LPC17xx.h"

#endif

#include <cr_section_macros.h>

#include <stdio.h>
#include<stdlib.h>

#define FIO0DIR (*(volatile unsigned int*)(0x2009C000))

#define PCONP (*(volatile unsigned int *)(0x400FC0C4))
#define PCLKSEL0 (*(volatile unsigned int *)(0x400FC1A8)) 	// Timer 0 Clock select
#define PCLKSEL1 (*(volatile unsigned int *)(0x400FC1AC)) 	// Timer 2 Clock select

#define PINSEL0 (*(volatile unsigned int *)(0x4002C000))
#define PINSEL1 (*(volatile unsigned int *)(0x4002C004))

#define T0TCR (*(volatile unsigned int *)(0x40004004)) 		// Timer 0
#define T0TC (*(volatile unsigned int *)(0x40004008))		// Timer Counter

#define T2TCR (*(volatile unsigned int *)(0x40090004)) 		// Timer 2
#define T2EMR (*(volatile unsigned int *)(0x4009003C)) 		// EMR Match Register for Timer 2
#define T2MCR (*(volatile unsigned int *)(0x40090014)) 		// MCR Match Register for Timer 2
#define T2CTCR (*(volatile unsigned int *)(0x40090070)) 	// Count control Match Register for Timer 2
#define T2MR3 (*(volatile unsigned int *)(0x40000024)) 		// Match Register 3

#define DACR (*(volatile unsigned int *)(0x4008C000)) 		// DAC Register

// TODO complete initialization


// Start Timer 0
static inline void timer0Start() {
	T0TCR |= (1 << 0);
}

// Start Timer 0
static inline void timer2Start() {
	T2TCR |= (1 << 0);
}

// Stop Timer 0
static inline void timer0Stop() {
	T0TCR &= ~(1 << 0);
}

// Clear Counter
static inline void timer0Reset() {
	T0TCR |= (1 << 1);
	while (T0TC != 0) {
		// wait until timer has changed
	}
	T0TCR &= ~(1 << 1);	//
}

// This timer generates our waveform
void timer0Init() {
	PCONP |= (1 << 1);					// Timer 0 Enable
	PCLKSEL0 &= ~ (3 << 2);				// Reset clock selection on Timer 0, default = 1MHz
	timer0Start();
}

// This timer controls the timing of decay for each tone
void timer2Init() {
	PCONP |= (1 << 22);					// Timer 2 Enable
	PCLKSEL1 &= ~(3 << 13); // Reset clock selection on Timer 2, default = 1MHz
	PINSEL0 |= (1 << 19) | (1 << 18); // Select Timer 2, Match register 3 on LPC P0.9
	T2MCR |= (1 << 10);	// Set Reset on MR3, TC will be reset if MR3 matches it
	T2EMR |= (3 << 10); // Toggle on match
	timer2Start();
}

// Read Timer 0
static inline int timer0Read_us() {
	return T0TC; // Read Timer 0 Counter
}


void wait_us(int usec) {
	timer0Start();
	timer0Reset();
	while(timer0Read_us() < usec) {
	}
}

// Configure Timer 2 for frequency generation
void configT2MR3(int freq) {
	timer0Reset();			// Reset Timer 0
	T2MR3 = timer0Read_us()/(2*freq);	// load T2MR3 with match value based on frequency PCLK/(2*freq)
	while (timer0Read_us() < 200000) {
		int aout = (1 - (timer0Read_us() / 2000000))*1023;
		DACR = aout << 6;
	}
}

// for calculated wait time, generate desired waveform
void genFreq(int waitTime) {
	for (int i = 0; i < 1023; i++) {
		DACR = i << 6;
		wait_us(waitTime);
		DACR = 0;
		wait_us(waitTime);
	}
}

void chooseFreq(int freq) {
	switch(freq){
	case 523: genFreq(880); break;		// generates a frequency of approximately 523 Hz
	case 587: genFreq(820); break;		// generates a frequency of approximately 587 Hz
	case 659: genFreq(690); break;		// generates a frequency of approximately 690 Hz
	case 784: genFreq(568); break;		// generates a frequency of approximately 784 Hz
	case 880: genFreq(498); break;		// generates a frequency of approximately 880 Hz
	}
}

int main(void) {
	FIO0DIR |= (1 << 9);				// Set T2MR3 as an output
	PINSEL1 = (1 << 21) | (0 << 20); 	// enable AOUT pins
	timer0Init();						// Initialize Timer 0
	timer2Init();						// Initialize Timer 2


	/*time_t t;
	srand((unsigned) time(&t));
	int count;
	int num;
	printf("Ten Random Numbers:\t");
	for (count = 1; count <= 10; count++) {
		num = rand() % 7 *1.5;
		printf("%d\t", num);
	}*/

	while (1) {
		configT2MR3(523); // let us try 523 Hz!!

	}

	return 0;

}
