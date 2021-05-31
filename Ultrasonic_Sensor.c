#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "TemperatureSensor.h"
#include "driverlib/adc.h"
#include "Init.h"
#include "Display.h"


void checkPIR(void)
{ 
	if((GPIO_PORTA_DATA_R&0x04) == 0x04){
		GPIO_PORTA_DATA_R &= ~0x04;
		IntMasterEnable(); 
		ADC0_Init();
	  ADCProcessorTrigger(ADC0_BASE, 3);
	}
		
}