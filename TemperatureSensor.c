#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/interrupt.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/timer.h"
#include "inc/hw_gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom_map.h"
#include "driverlib/gpio.h"
#include "Display.h"

#define RED_MASK 0X02
#define BLUE_MASK 0x04
#define GREEN_MASK 0X08

uint32_t ui32ADC0Value[2];
volatile uint32_t ui32TempValueC;
volatile int ui32TempValueF;

		
//interrupt handler
void ADC0_Handler(void)
{
		ADCIntClear(ADC0_BASE, 3);
		ADCProcessorTrigger(ADC0_BASE, 3);
		ADCSequenceDataGet(ADC0_BASE, 3, ui32ADC0Value);
		ui32TempValueC = (ui32ADC0Value[0]-700)/10;
		ui32TempValueF = ((ui32TempValueC * 9) / 5) + 32;
		
		if(ui32TempValueF >= 85){
			GPIO_PORTF_DATA_R |= RED_MASK;
		}
		if(ui32TempValueF >= 70 && ui32TempValueF < 85){
			GPIO_PORTF_DATA_R |= GREEN_MASK;
		}
		if(ui32TempValueF < 70){
			GPIO_PORTF_DATA_R |= BLUE_MASK;
		}
		
		OLED_YX(0, 0 );
		OLED_Write_String( "Temperature:" );
		/////////////////////
    // Temperature 
    ////////////////////				
    OLED_YX(2, 5);
    OLED_Write_Integer(ui32TempValueF);
		OLED_YX(2, 7);
		OLED_Write_String(" F");
		SysCtlDelay(40000000);
		GPIO_PORTF_DATA_R &= ~0x0E;
		IntMasterDisable();
}

