/* p11_3.c: Use PWM to control LED intensity */

/* This program is based on p10_2. In the infinite loop, the value of CMPA register is decremented by 100 every 20 ms. The decreasing CMPA value causes the duty cycle to lengthen. */

#include "TM4C123GH6PM.h"

int main(void)
{
    void delayMs(int n);
    int x = 0;
    
    /* Enable Peripheral Clocks */
    SYSCTL->RCGCPWM |= 2;       /* enable clock to PWM1 */
    SYSCTL->RCGCGPIO |= 0x20;   /* enable clock to PORTF */
    SYSCTL->RCC &= ~0x00100000; /* no pre-divide for PWM clock */

    /* Enable port PF3 for PWM1 M1PWM7 */
    GPIOF->AFSEL = 8;           /* PF3 uses alternate function */
    GPIOF->PCTL &= ~0x0000F000; /* make PF3 PWM output pin */
    GPIOF->PCTL |= 0x00005000;
    GPIOF->DEN |= 8;            /* pin digital */
    
    PWM1->_3_CTL = 0;           /* stop counter */
    PWM1->_3_GENB = 0x0000008C; /* M1PWM7 output set when reload, */
                                /* clear when match PWMCMPA */
    PWM1->_3_LOAD = 16000;      /* set load value for 1kHz (16MHz/16000) */
    PWM1->_3_CMPA = 15999;      /* set duty cycle to min */
    PWM1->_3_CTL = 1;           /* start timer */
    PWM1->ENABLE = 0x80;        /* start PWM1 ch7 */
    
    for(;;)
    {	
			
			  /*
				//Decrease Brightness
        x = x + 100;
				if (x >= 15999) x = 0;
			
				//Increase Brightness
			  x = x - 100;
				if (x <= 0) x = 15999;
			  */
			
				do
				{
					x = x + 100;
					PWM1->_3_CMPA = x;
          delayMs(20);
				}
					while (x < 15900);
						
				do
				{
					x = x - 100;
					PWM1->_3_CMPA = x;
          delayMs(20);
				}
					while (x > 0);
			
        //PWM1->_3_CMPA = x;
        //delayMs(20);
    }
}


/* delay n milliseconds (16 MHz CPU clock) */
void delayMs(int n)
{
    int i, j;
    for(i = 0 ; i < n; i++)
        for(j = 0; j < 3180; j++)
            {}  /* do nothing for 1 ms */
}

/* This function is called by the startup assembly code to perform system specific initialization tasks. */
void SystemInit(void)
{
    /* Grant coprocessor access */
    /* This is required since TM4C123G has a floating point coprocessor */
    SCB->CPACR |= 0x00f00000;
}
