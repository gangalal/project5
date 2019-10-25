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

#include <cr_section_macros.h>

#include <stdio.h>
#include<stdlib.h>

#define PCONP (*(volatile unsigned int *)(0x400FC0C4))
#define PCLKSEL0 (*(volatile unsigned int *)(0x400FC1A8)) 	// Timer 0 Clock select
#define PCLKSEL1 (*(volatile unsigned int *)(0x400FC1AC)) 	// Timer 2 Clock select

#define PINSEL0 (*(volatile unsigned int *)(0x4002C000))
#define PINSEL1 (*(volatile unsigned int *)(0x4002C004))
#define PINMODE0 (*(volatile unsigned int *)(0x4002C040))

#define T0TCR (*(volatile unsigned int *)(0x40004004)) 		// Timer 0
#define T0TC (*(volatile unsigned int *)(0x40004008))		// Timer Counter

#define T2TCR (*(volatile unsigned int *)(0x40090004)) 		// Timer 2
#define T2EMR (*(volatile unsigned int *)(0x4009003C)) 		// EMR Match Register for Timer 2
#define T2MCR (*(volatile unsigned int *)(0x40090014)) 		// MCR Match Register for Timer 2
#define T2CTCR (*(volatile unsigned int *)(0x40090070)) 	// Count control Match Register for Timer 2
#define T2MR3 (*(volatile unsigned int *)(0x40000024)) 		// Match Register 3

#define DACR (*(volatile unsigned int *)(0x4008C000)) 		// DAC Register

// TODO complete initialization

// This timer generates our waveform
void timerInit0() {
	PCONP |= (1 << 1);					// Timer 0 Enable
	PCLKSEL0 &= ~ (3 << 2);				// Reset clock selection on Timer 0, default = 1MHz
}

// This timer controls the timing of decay for each tone
void timerInit2() {

	PCONP |= (1 << 22);					// Timer 2 Enable
	PCLKSEL1 &= ~(3 << 13); // Reset clock selection on Timer 2, default = 1MHz
	PINSEL0 |= (1 << 19) | (1 << 18); // Select Timer 2, Match register 3 on LPC P0.9
	T2MCR |= (1 << 10);	// Set Reset on MR3, TC will be reset if MR3 matches it
	T2EMR |= (3 << 10); // toggle on match
}

// Start Timer 0
static inline void timerStart() {

	T0TCR |= (1 << 0);

}

// Stop Timer 0
static inline void timerStop() {

	T0TCR &= ~(1 << 0);

}

// Clear Counter
static inline void timerReset() {

	T0TCR |= (1 << 1);
	while (T0TC != 0) {
		// wait until timer has changed
	}
	T0TCR &= ~(1 << 1);	//

}

static inline int timerRead_us() {

	return T0TC; // Read Timer 0 Counter

}

void wait_us(int usec) {

	timerStart();
	timerReset();
	while(timerRead_us() < usec) {
	}
}

void wait_ticks(unsigned long count) {
	volatile int ticks;
	for (ticks = 0; ticks < count; ticks++) {
		// do nothing
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

// input frequency desired, compare with match register; if match, reset timer counter
void timerGenFreq(int freq) {

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

	PINSEL1 = (1 << 21) | (0 << 20); 	// enable AOUT pins
	timerInit0();
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
		chooseFreq(880);


	// This function produces ~ 523 Hz
	/*	for (int i = 0; i < 1000; i++) {
			DACR = i << 6;
			wait_us(880);
			DACR = 0;
			wait_us(880);
		}*/

		// This function produces ~ 587 Hz
/*		for (int i = 0; i < 1000; i++) {
			DACR = i << 6;
			wait_us(820);
			DACR = 0;
			wait_us(820);
		}*/

		// This function produces ~ 659 Hz
		/*for (int i = 0; i < 1000; i++) {
			 DACR = i << 6;
			 wait_us(690);
			 DACR = 0;
			 wait_us(690);
		}*/

		// This function produces ~ 784 Hz
		/*for (int i = 0; i < 1000; i+=3) {
				 DACR = i << 6; 				// shifting a larger and larger number into DAC
				 wait_us(568);
				 DACR = 0;						// resets the DAC register
				 wait_us(568);
		}*/

		// This function produces ~ 880 Hz
		/*for (int i = 0; i < 0; i+=3) {
			DACR = i << 6;
			wait_us(498);
			DACR = 0;
			wait_us(498);
		}*/
	}

	return 0;

}
