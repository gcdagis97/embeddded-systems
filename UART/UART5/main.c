/* p4_3.c: Sending "Hello" to UART5 on TI ARM LaunchPad (TM4C123GH6PM) */
/* UART5 Tx is on PE5 */
/* Use TeraTerm to see the message "Hello" on a PC */

#include <stdint.h>
#include "tm4c123gh6pm.h"

void UART5Tx(char c);
void delayMs(int n);
void SystemInit(void);
	
int main(void) 
{
    char message[] = "Hello";
    int i;
    
    SYSCTL->RCGCUART |= 0x20;  /* provide clock to UART5 */
    SYSCTL->RCGCGPIO |= 0x10;  /* Enable clock to PORTE */

    /* UART5 initialization */
    UART5->CTL = 0;         /* disable UART5 */
    UART5->IBRD = 104;      /* 16MHz/16=1MHz, 1MHz/104=9600 baud rate */
    UART5->FBRD = 11;       /* fraction part, see Example 4-4 */
    UART5->CC = 0;          /* use system clock */
    UART5->LCRH = 0x60;     /* 8-bit, no parity, 1-stop bit */
    UART5->CTL = 0x301;     /* enable UART5, TXE, RXE */
    
    /* UART5 TX5 and RX5 use PE5 and PE4. Set them up. */
    GPIOE->DEN = 0x30;      /* make PE5, PE4 as digital */
    GPIOE->AMSEL = 0;       /* turn off analog function */
    GPIOE->AFSEL = 0x30;    /* use PE5, PE4 alternate function */
    GPIOE->PCTL = 0x00110000;  /* configure PE5, PE4 for UART5 */

    delayMs(1);             /* wait for output line to stabilize */
    
    for(;;)
    {
        for (i = 0; i < 5; i++)
            UART5Tx(message[i]);
    }   
}

/* UART5 Transmit */
void UART5Tx(char c)  
{
    while((UART5->FR & 0x20) != 0); /* wait until Tx buffer not full */
    UART5->DR = c;                  /* before giving it another byte */
}

/* Append delay functions and SystemInit() here */


void delayMs(int n)
{
		int i, j;
		for(i=0; i<n; i++)
			for (j=0; j<3180; j++)
				{}
}

void SystemInit(void)
{
	SCB->CPACR |= 0x00F00000;
}
