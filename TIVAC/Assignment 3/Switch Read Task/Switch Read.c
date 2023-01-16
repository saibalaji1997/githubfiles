#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"

//Initialize switch
void switch_init(void){
    //Enable the switch
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    //Configure the switch as input
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0);
    //Configure the pull-up resistors for the switch
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}

//Read switch status
uint8_t switch_read(void){
    return GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0);
}
