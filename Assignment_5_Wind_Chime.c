/*
===============================================================================
 Name        : Assignment_5_Wind_Chime.c
 Author      : $(author)
 Version     :
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

//#define FIO0DIR (*(volatile unsigned int*)(0x2009C000))

#define PCONP (*(volatile unsigned int *)(0x400FC0C4))
#define PCLKSEL0 (*(volatile unsigned int *)(0x400FC1A8)) 	// Timer 0 Clock select
#define PCLKSEL1 (*(volatile unsigned int *)(0x400FC1AC)) 	// Timer 2 Clock select

#define PINSEL0 (*(volatile unsigned int *)(0x4002C000))	// PINSELx Register for Timer 2
#define PINSEL1 (*(volatile unsigned int *)(0x4002C004))	// PINSELx Register for Aout

#define T0TCR (*(volatile unsigned int *)(0x40004004)) 		// Timer 0 Timer Control Register
#define T0TC (*(volatile unsigned int *)(0x40004008))		// Timer 0 Timer Counter

#define T2TCR (*(volatile unsigned int *)(0x40090004)) 		// Timer 2 Timer Control Register
#define T2TC (*(volatile unsigned int *)(0x40090008)) 		// Timer 2 Timer Counter
#define T2EMR (*(volatile unsigned int *)(0x4009003C)) 		// EMR Match Register for Timer 2
#define T2MCR (*(volatile unsigned int *)(0x40090014)) 		// MCR Match Register for Timer 2
#define T2CTCR (*(volatile unsigned int *)(0x40090070)) 	// Count Control Register for Timer 2 Selects between timer and counter mode
#define T2MR3 (*(volatile unsigned int *)(0x40000024)) 		// Match Register 3

#define DACR (*(volatile unsigned int *)(0x4008C000)) 		// DAC Register


// This timer generates our waveform
void timer0Init() {
	PCONP |= (1 << 1);					// Timer 0 Enable
	PCLKSEL0 &= ~ (3 << 2);				// Reset clock selection on Timer 0, default = 1MHz
	//timer0Start();
}

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

// Clear Counter/Timer 0
static inline void timer0Reset() {
	T0TCR |= (1 << 1);
	while (T0TC != 0) {
		// wait until timer has changed
	}
	T0TCR &= ~(1 << 1);	//
}

// Read Timer 0
static inline int timer0Read_us(void) {
	return T0TC; // Read Timer 0 Counter
}

// True read Timer 0 for floating value in DACR conversion
static inline float timer0Read(void) {
	return timer0Read_us() /1000000.0;
}


void wait_us(int usec) {
	timer0Start();
	timer0Reset();
	while(timer0Read() < usec) {
	}
}

// function for basic wait in seconds
void wait(float sec) {
	wait_us(sec * 1000000);
}




// This timer controls the timing of decay for each tone
void timer2Init() {
	PCONP |= (1 << 22);		// Timer 2 Enable
	PCLKSEL1 &= ~(1 << 12); // Reset clock selection on Timer 2, default = 1MHz
	PCLKSEL1 &= ~(1 << 13); // Reset clock selection on Timer 2, default = 1MHz
	PINSEL0 |= (1 << 18);	// Select Timer 2, Match register 3 on LPC P0.9
	PINSEL0 |= (1 << 19); // Select Timer 2, Match register 3 on LPC P0.9
	T2MCR |= (1 << 10);	// Set Reset on MR3, TC will be reset if MR3 matches it
	T2EMR |= (3 << 10); // Toggle on match EMC3
	T2CTCR &= ~(1 << 0);
	T2CTCR &= ~(1 << 1);
	timer2Start();
}

// Reset Timer 2
static inline void timer2Reset() {
	T2TCR &= ~ (1 << 1);
	while (T2TC != 0) {
		// wait until timer has changed
	}
	T2TCR &= ~(1 << 1);	//
}

// Configure Timer 2 for frequency generation
void configT2MR3 (int freq) {
	timer0Reset();			// Reset Timer 0
	T2MR3 = (1000000/(2*freq));	// load T2MR3 with match value based on frequency PCLK/(2*freq)
	while (timer0Read() < 2500000) {
		int aout = (1 - (timer0Read() / 2000000.0))*1023.0;
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


void chooseFreq (int freq) {
	switch(freq) {
	case 523: genFreq(880); break;		// generates a frequency of approximately 523 Hz
	case 587: genFreq(820); break;		// generates a frequency of approximately 587 Hz
	case 659: genFreq(690); break;		// generates a frequency of approximately 690 Hz
	case 784: genFreq(568); break;		// generates a frequency of approximately 784 Hz
	case 880: genFreq(498); break;		// generates a frequency of approximately 880 Hz
	}
}

int main(void) {
	PINSEL1 |= (1 << 21); 	// enable AOUT pin 18
	PINSEL1 &= ~(1 << 20);	// enable AOUT pin 18
	timer0Init();			// Initialize Timer 0
	timer2Init();			// Initialize Timer 2

	/*time_t t;
	srand((unsigned) time(&t));
	int count;
	int num;
	printf("Ten Random Numbers:\t");
	for (count = 1; count <= 10; count++) {
		num = rand() % 7 *1.5;
		printf("%d\t", num);
	}*/

	while(1) {
		configT2MR3(956);
		timer2Reset();
		printf("hello world\n");
		//wait_us(1000000);
	}
	return 0;

}



