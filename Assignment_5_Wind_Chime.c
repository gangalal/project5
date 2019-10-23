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

#define PCONP (*(volatile unsigned int *)(0x400FC0C4))
#define PCLKSEL0 (*(volatile unsigned int *)(0x400FC1A8)) // Timer 0 Clock select
#define PCLKSEL1 (*(volatile unsigned int *)(0x400FC1AC)) // Timer 2 Clock select
#define T0TCR (*(volatile unsigned int *)(0x40004004)) // Timer 0
#define T2TCR (*(volatile unsigned int *)(0x40090004)) // Timer 2
#define DACR (*(volatile unsigned int *)(0x4008C000)) // DAC Register
//#define MR2 (*(volatile unsigned int *)(0x40004020))
#define T2MR3 (*(volatile unsigned int *)(0x40000024)) // Match Register 3
#define PINSEL0 (*(volatile unsigned int *)(0x4002C000))
#define PINSEL1 (*(volatile unsigned int *)(0x4002C004))
#define PINMODE0 (*(volatile unsigned int *)(0x4002C040))
#define T2EMR (*(volatile unsigned int *)(0x4009003C)) // EMR Match Register for Timer 2
#define T2MCR (*(volatile unsigned int *)(0x40090014)) // MCR Match Register for Timer 2
#define T2CTCR (*(volatile unsigned int *)(0x40090070)) // Count control Match Register for Timer 2

//#define AD0CR (*(volatile unsigned int *)(0x40034000)) // DO NOT NEED
//#define AD0GDR (*(volatile unsigned int *)(0x40034004)) // DO NOT NEED


// Initialize the timer (use timer instead of PWM to set high time and decay length)
// TODO complete initialization

// initialize Timer 0
void timerInit0() {
	PCONP |= (1 << 1);					// Timer 0 Enable
	PCLKSEL0 &= ~(1 << 3) | (1 << 2);	// Reset clock selection on Timer 0, default = 1MHz
//	PINSEL0 |= (1<<19 )|(1<<18);		// Select Timer 2, Match register 3 on LPC P0.9
//	PINMODE0 |= (1<<19 )|(1<<18);	 	// Select pin mode with timer functions
}

void timerInit2() {
	PCONP |= (1 << 22);					// Timer 2 Enable
	PCLKSEL1 &= ~(1 << 13) | (1 << 12); // Reset clock selection on Timer 2, default = 1MHz
	PINSEL0 |= (1<<19 )|(1<<18);		// Select Timer 2, Match register 3 on LPC P0.9
	PINMODE0 |= (1<<19 )|(1<<18);	 	// Select pin mode with timer functions
	T2MCR |= (1 << 10);					// Set Reset on MR3, TC will be reset if MR3 matches it
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
	while (T0TCR != 0) {
		// wait until timer has changed
	}
	T0TCR &= ~(1 << 1);	//
}

static inline int timerRead_us() {
	return T0TCR; // Read Timer 0
}






void wait_us(int usec) {
	timerStart();
	timerReset();
	while (timerRead_us() < usec) {
	}
}

//int ADCRead(int channel) {
//	AD0CR = (1 << channel) | (0 << 8) | (1 << 21);	// (0 << 8) is the default
//	AD0CR |= (1 << 24);								// start conversion
//	while (((AD0GDR >> 31) & 1) == 0) {
//		// wait until the ADC has finished the conversion
//	}
//	return (AD0GDR >> 4) & (0xFFF);		// read result, return result
//}

void wait_ticks(unsigned long count) {
	volatile int ticks;
	for (ticks = 0; ticks < count; ticks++) {

	}
}

int main(void) {
	PINSEL1 = (1 << 21) | (0 << 20); 	// enable AOUT pins
	timerInit0();
	while (1) {
		/*for (int i = 0; i < 1023; i = i + 3) {
			DACR = i << 6;
			wait_ticks(1000);
		}
		for (int i = 1023; i > 0; i = i - 3) {
			DACR = i << 6;
			wait_ticks(1000);
		}*/


	}

	return 0;
}
