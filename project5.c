/*
 ===============================================================================
 Name        : temprature.c
 Author      : Gangal Lama Dong, Daniel Glover, and Jessica Horner
 Version     : v 2.0
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
#define PCLKSEL0 (*(volatile unsigned int *)(0x400FC1A8))
#define T0TCR (*(volatile unsigned int *)(0x40004004))
#define DACR (*(volatile unsigned int *)(0x4008C000))
#define MR2 (*(volatile unsigned int *)(0x40004020))
#define PINSEL0 (*(volatile unsigned int *)(0x4002C000))
#define PINSEL1 (*(volatile unsigned int *)(0x4002C004))
#define PINMODE0 (*(volatile unsigned int *)(0x4002C040))

#define AD0CR (*(volatile unsigned int *)(0x40034000))
#define AD0GDR (*(volatile unsigned int *)(0x40034004))
// TODO #define PINMODE0 (*(volatile unsigned int *)(0x4002 C040))

// Initialize the timer (use timer instead of PWM to set high time and decay length)
// TODO complete initialization
void timerInit() {
	PCONP |= (1 << 1);					// set the PCADC bit to power ADC
	PCLKSEL0 &= ~(1 << 3) | (1 << 2);	// reset clock selection - D type Reg?
	PINSEL0 |= ( 1<<19 )|(1<<18);		// TODO Select timer pins
	PINMODE0 |= ( 1<<19 )|(1<<18);	 // TODO Select pin mode with timer functions
}

static inline void timerStart() {
	T0TCR |= (1 < 0);
}

static inline void timerStop() {
	T0TCR &= ~(1 << 0);
}

static inline void timerReset() {
	T0TCR |= (1 << 1);
	while (T0TCR != 0) {
		// wait until timer has changed
	}
	T0TCR &= ~(1 << 1);	//
}

static inline int timerRead_us() {
	return T0TCR;
}

void wait_us(int us) {
	timerStart();
	timerReset();
	while (timerRead_us() < us) {
	}
}

int ADCRead(int channel) {
	AD0CR = (1 << channel) | (0 << 8) | (1 << 21);	// (0 << 8) is the default
	AD0CR |= (1 << 24);								// start conversion
	while (((AD0GDR >> 31) & 1) == 0) {
		// wait until the ADC has finished the conversion
	}
	return (AD0GDR >> 4) & (0xFFF);		// read result, return result
}

void wait_ticks(unsigned long count) {
	volatile int ticks;
	for (ticks = 0; ticks < count; ticks++) {

	}
}

int main(void) {
	PINSEL1 = (1 << 21) | (0 << 20); 	// enable AOUT pins
	timerInit();
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
