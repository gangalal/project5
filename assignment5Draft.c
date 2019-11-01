/*
 ===============================================================================
 Name        : Assignment_4_I2C.c
 Author      : $(Daniel Glover, Lama Dong, Jessica)
 Version     : V1
 Copyright   : $(copyright)
 Description : main definition
 ===============================================================================
 */

#ifdef __USE_CMSIS

#include "LPC17xx.h"

#endif

#include <cr_section_macros.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
#define T2MR3 (*(volatile unsigned int *)(0x40090024)) 		// Match Register 3
#define DACR (*(volatile unsigned int *)(0x4008C000)) 		// DAC Register

#define FIO0DIR (*(volatile unsigned int *)0x2009c000)		// Direction Register
#define FIO0PIN (*(volatile unsigned int *)0x2009c014)		// Pin Selection Register
#define PINMODE0 (*(volatile unsigned int *)0x4002C040)		// Pin Mode Register (pull-up, pull-down, etc)


int freq[] = { 520, 584, 656, 781, 877 }; //frequency to generate are added +3
int randNum[5]; //to hold the random number
int waveTable[256]; //to hold the sin wave

/**
 *  Read input for diagnostic mode function
 */
static inline int getSwitch1(void) {
	return ((FIO0PIN >> 10) & 1);
}

/**
 *  fill in the values for the sine wavetable
 */
void fillWaveTable() {
	for (int i = 0; i < 256; i++) {
		waveTable[i] = (500) * (1 + sin(2 * 3.14159265 * i / 256.0));
	}
}

/**
 *  Start Timer 0
 */
static inline void timer0Start() {
	T0TCR |= (1 << 0);
}

/**
 * Initialize Timer 0
 */
void timer0Init() {
	PCONP |= (1 << 1);			// Timer 0 Enable
	PCLKSEL0 &= ~(3 << 2);	// Reset clock selection on Timer 0, default = 1MHz
	timer0Start();
}

/**
 * Stop Timer 0
 */
static inline void timer0Stop() {
	T0TCR &= ~(1 << 0);
}

/**
 * Clear Timer 0 Counter
 */
static inline void timer0Reset() {
	T0TCR |= (1 << 1);
	while (T0TC != 0) {
		// wait until timer has changed
	}
	T0TCR &= ~(1 << 1);	//
}


/**
 * Read Timer 0
 */
static inline int timer0Read_us(void) {
	return T0TC; // Read Timer 0 Counter
}


/**
 *  True read Timer 0 for floating value in DACR conversion
 */
static inline float timer0Read(void) {
	return timer0Read_us() / 1000000.0;
}


/**
 * wait function in microseconds
 */
void wait_us(float usec) {
	timer0Start();
	timer0Reset();
	while (timer0Read_us() < usec) {
	}
}


/**
 * wait function in seconds
 */
void wait(float sec) {
	wait_us(sec * 1000000);
}


/**
 * Start Timer 2
 */
static inline void timer2Start() {
	T2TCR |= (1 << 0);
}


/**
 * Initialize Timer 2 to control the timing of decay for each tone
 */
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


/**
 * Reset Timer 2
 */
static inline void timer2Reset() {
	T2TCR |= (1 << 1);
	while (T2TC != 0) {
		// wait until timer has changed
	}
	T2TCR &= ~(1 << 1);	//
}


/**
 * Configure Timer 2 for frequency generation
 */
void configT2MR3(int freq) {
	timer0Reset();			// Reset Timer 0
	T2MR3 = (1000000 / (2 * freq));	// load T2MR3 with match value based on frequency PCLK/(2*freq)
	timer2Reset();
	while (timer0Read_us() < 2000000.0) {
		int aout = (1 - (timer0Read_us() / 2000000.0)) * 1023.0;
		DACR = aout << 6;
	}
}


/**
 * diagnostic mode
 */
void diagnosticMode() {
	while (getSwitch1() == 0) {
		configT2MR3(520);
		printf("generating 523Hz\n");
		configT2MR3(584);
		printf("generating 587Hz\n");
		configT2MR3(656);
		printf("generating 659Hz\n");
		configT2MR3(781);
		printf("generating 784Hz\n");
		configT2MR3(878);
		printf("generating 881Hz\n");
	}
}

/**
 * generates random number and inserts to randNum
 */
void generateRandomNum() {
	time_t t;
	srand((unsigned) time(&t));
	for (int i = 0; i < 5; i++) {
		int temp = rand() % 6 + 1;
		if (temp != 0) {
			randNum[i] = temp;
		}
		printf("%d\n", randNum[i]);
	}

}


/**
 * waits for count*1sec
 */
void wait_ticks(unsigned long count) {
	volatile int ticks;
	volatile int temp = 157142 * count;
	for (ticks = 0; ticks < temp; ticks++) {
		for (ticks = 0; ticks < count; ticks++) {

		}
	}
}

/**
 * waits for sometime
 */
void wait_ticks1(unsigned long count) {
	volatile int ticks;
	for (ticks = 0; ticks < count; ticks++) {
		for (ticks = 0; ticks < count; ticks++) {

		}
	}
}


/**
*random simulation
*/
void randomSimulation() {
	generateRandomNum();
	configT2MR3(520);
	printf("....generating 523Hz\n");
	wait_ticks(randNum[0]);
	if (getSwitch1() == 0) {
		diagnosticMode();
	}
	configT2MR3(656);
	printf(".....generating 659Hz\n");
	wait_ticks(randNum[1]);
	if (getSwitch1() == 0) {
		diagnosticMode();
	}
	configT2MR3(877);
	printf("....generating 877Hz\n");
	wait_ticks(randNum[2]);
	if (getSwitch1() == 0) {
		diagnosticMode();
	}
	configT2MR3(584);
	printf("....generating 584Hz\n");
	wait_ticks(randNum[3]);
	if (getSwitch1() == 0) {
		diagnosticMode();
	}
	configT2MR3(781);
	printf("....generating 781Hz\n");
	wait_ticks(randNum[4]);
	if (getSwitch1() == 0) {
		diagnosticMode();
	}

}


/**
 * generates sine wave
 */
void genSineWF() {
	int i = 0;
	for (i = 0; i < 256; i = i + 7) {
		DACR = ((int) waveTable[i]) << 6;
		wait_ticks1(5);

	}

}


/**
 * the main function
 */
int main(void) {
	FIO0DIR &= ~(1 << 10); // configure P0[10] as input (pin10) for diagnostic mode
	PINMODE0 &= ~(1 << 20);
	PINMODE0 &= ~(1 << 21);
	FIO0DIR &= ~(1 << 11); // configure P0[11] as input

	PINSEL1 = (1 << 21) | (0 << 20); 	// enable AOUT pins
	timer0Init();			// Initialize Timer 0
	timer2Init();			// Initialize Timer 2
	fillWaveTable();

	while (1) {

		if (getSwitch1 () == 0) {
		 diagnosticMode();
		 } else if (((FIO0PIN >> 11) & 0x01) == 0) {
			 genSineWF();
		 } else {
		 randomSimulation();
		 }
	}
	return 0;
}
