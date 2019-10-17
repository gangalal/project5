/*
 ===============================================================================
 Name        : temprature.c
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
#define PCLKSEL0 (*(volatile unsigned int *)(0x400FC1A8))
#define T0TCR (*(volatile unsigned int *)(0x40004004))

#define AD0CR (*(volatile unsigned int *)(0x40034000))
#define AD0GDR (*(volatile unsigned int *)(0x40034004))
#define PINSEL1 (*(volatile unsigned int *)(0x4002C004))

void timerInit() {
	PCONP |= (1 << 1);
	PCLKSEL0 &= ~(1 << 3) | (1 << 2); // resetting clock selection
	PCLKSEL0 |= (0 << 3) | (0 << 2); //
	//to do complete init....
}

void ADCInit() {
	PCONP |= (1 << 1);
	AD0CR = (1 << 21);
	AD0GDR;
	AD0CR = 0;
	AD0CR = (1 << 21);
	PINSEL1 = (1 << 14); //need to make sure
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
	}
	T0TCR &= ~(1 << 1);
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
	AD0CR = (1 << channel) | (0 << 8) | (1 << 21);
	AD0CR |= (1 << 24);
	while (((AD0GDR >> 31) & 1) == 0) {

	}
	return (AD0GDR >> 4) & (0xFFF);
}

int main(void) {

	while (1) {

	}
	return 0;
}
