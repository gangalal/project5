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

#define AD0CR (*(volatile unsigned int *)(0x40034000))
#define AD0GDR (*(volatile unsigned int *)(0x40034004))
#define PINSEL1 (*(volatile unsigned int *)(0x4002C004))
// TODO #define PINMODE0 (*(volatile unsigned int *)(0x4002 C040))

// Initialize the timer (use timer instead of PWM to set high time and decay length)
// TODO complete initialization
void timerInit() {
	PCONP |= (1 << 1);					// set the PCADC bit to power ADC
	PCLKSEL0 &= ~(1 << 3) | (1 << 2);	// reset clock selection - D type Reg?
	// PCLKSEL0 |= (0 << 3) | (0 << 2);
	// PINSEL1 = ( << );					// TODO Select timer pins
	// PINMODE0 = ( << );					// TODO Select pin mode with timer functions
}

// Initialize the analog to digital converter
void ADCInit() {
	PCONP |= (1 << 1);		// set the PCADC bit to power ADC
	AD0CR = (1 << 21);		// enable PDN bit to turn on ADC bit
	AD0GDR;					// read GDR and clear result
	AD0CR = 0;				// disable PDN Nbit to turn off ADC bit
	AD0CR = (1 << 21);		// enable PDN bit to turn on ADC bit
	PINSEL1 = (1 << 14); 	// TODO double check this line of code
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

int main(void) {

	while (1) {

	}
	return 0;
}
