#include "delay.h"

/* volatile counters so the compiler cannot optimize the loops away.
   Tuned for CCLK = 60 MHz. */
void delay_ms(u32 ms)
{
	u32 i;
	volatile u32 j;
	for (i = 0; i < ms; i++)
		for (j = 0; j < 6000; j++);
}

void delay_us(u32 us)
{
	u32 i;
	volatile u32 j;
	for (i = 0; i < us; i++)
		for (j = 0; j < 6; j++);
}
