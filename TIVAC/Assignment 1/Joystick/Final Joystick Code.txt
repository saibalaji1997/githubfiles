#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

// Function prototypes
void ConfigureTimer(void);
void Timer0IntHandler(void);

int main(void)
{
    // Set the clock frequency
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    // Enable the peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    // Configure the timer interrupt
    ConfigureTimer();

    // Enable the timer interrupt
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    IntEnable(INT_TIMER0A);

    // Configure the joystick pins as input
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3);

    // Enable the interrupt
    IntMasterEnable();

    // Loop forever
    while(1)
    {
        // Read the joystick values
        uint32_t xVal = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_2);
        uint32_t yVal = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_3);

        // Print the values to the terminal
        printf("X: %d, Y: %d\n", xVal, yVal);
    }
}

// Configure the timer interrupt
void ConfigureTimer(void)
{
    // Set the timer period
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/2);

    // Enable the timer
    TimerEnable(TIMER0_BASE, TIMER_A);
}

// Timer interrupt handler
void Timer0IntHandler(void)
{
    // Clear the interrupt
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    // Read the joystick values
    uint32_t xVal = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_2);
    uint32_t yVal = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_3);

    // Print the values to the terminal
    printf("X: %d, Y: %d\n",
}
