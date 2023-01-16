#include <stdint.h>             //Variable definitions for the C99 standard
#include <stdbool.h>            //Boolean definitions for the C99 standard
#include "inc/tm4c123gh6pm.h"   //def. for the interrupt and register assignments on the Tiva C Series device on the launchPad board
#include "inc/hw_memmap.h"      //Macros defining the memory map of the Tiva C Series
#include "inc/hw_types.h"       //Defines common types and macros
#include "driverlib/sysctl.h"   //Defines macros for System Control API of Driverlib
#include "driverlib/interrupt.h"//defines & macros for NVIC Controller(Interrupt)API of driverlib.
#include "driverlib/gpio.h"     //Defines macros for GPIO API of Driverlib
#include "driverlib/timer.h"    //Defines and macros for Timer API of driverLib.

uint32_t ui32Period;
int led_state = 0;

void GPIOF0IntHandler(void)
{
    //clear interrupt flag on pin F0
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0);
    // increment the state variable
    led_state++;
    if(led_state > 3){
        led_state = 0;
    }
    // Turn on LED based on the led_state
    if(led_state == 0){
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
    }
    else if(led_state == 1){
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 2);
    }
    else if(led_state == 2){
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);
    }
    else if(led_state == 3){
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 8);
    }
}

int main(void)
{
    //System clock to 40Mhz (PLL= 400Mhz / 10 = 40Mhz)
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
}

