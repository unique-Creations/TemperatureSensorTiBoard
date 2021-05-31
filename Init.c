#include <stdbool.h>
#include <stdint.h>
#include "Display.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom_map.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"

#define GPIO_PCTL_PB6_T0CCP0    0x07000000


void OLED_Init(void) {
    
    OLED_Command(OLED_DISPLAYOFF);         // 0xAE
    OLED_Command(OLED_SETDISPLAYCLOCKDIV); // 0xD5
    OLED_Command(0x80);                    // the suggested ratio 0x80
    OLED_Command(OLED_SETMULTIPLEX);       // 0xA8
    OLED_Command(0x1F);
    OLED_Command(OLED_SETDISPLAYOFFSET);   // 0xD3
    OLED_Command(0x0);                        // no offset
    OLED_Command(OLED_SETSTARTLINE | 0x0); // line #0
    OLED_Command(OLED_CHARGEPUMP);         // 0x8D
    OLED_Command(0xAF);
    OLED_Command(OLED_MEMORYMODE);         // 0x20
    OLED_Command(0x00);                    // 0x0 act like ks0108
    OLED_Command(OLED_SEGREMAP | 0x1);
    OLED_Command(OLED_COMSCANDEC);
    OLED_Command(OLED_SETCOMPINS);         // 0xDA
    OLED_Command(0x02);
    OLED_Command(OLED_SETCONTRAST);        // 0x81
    OLED_Command(0x8F);
    OLED_Command(OLED_SETPRECHARGE);       // 0xd9
    OLED_Command(0xF1);
    OLED_Command(OLED_SETVCOMDETECT);      // 0xDB
    OLED_Command(0x40);
    OLED_Command(OLED_DISPLAYALLON_RESUME);// 0xA4
    OLED_Command(OLED_NORMALDISPLAY);      // 0xA6
    OLED_Command(OLED_DISPLAYON);          //--turn on oled panel

}

 //I2C intialization and GPIO alternate function configuration
void I2C3_Init ( void )
{
	SYSCTL_RCGCI2C_R |= 0x08; // Enable the clock for I2C 3
	SYSCTL_RCGCGPIO_R  |= 0x08 ; // Enable the clock for port D
	GPIO_PORTD_DEN_R |= 0x03; // Assert DEN for port D
// Configure Port D pins 0 and 1 as I2C 3
	GPIO_PORTD_AFSEL_R |= 0x00000003 ;
	GPIO_PORTD_PCTL_R |= 0x00000033 ;
	GPIO_PORTD_ODR_R |= 0x00000002 ; // SDA (PD1 ) pin as open darin
	I2C3_MCR_R = 0x0010 ; // Enable I2C 3 master function
	I2C3_MTPR_R  = 0x07 ;
}

void PortFunctionInit(void)
{	
	SYSCTL_RCGC2_R |= 0x00000031; //Activate clock for portE & PortF & PortA
	GPIO_PORTE_DIR_R &= ~0x08; //Make PE3 input
	GPIO_PORTE_AFSEL_R |= 0x08; //enable alternate function on PE3
	GPIO_PORTE_DEN_R &= ~0x08; //disable digital I/O on PE3
	GPIO_PORTE_AMSEL_R |= 0x08; //enable analog function on PE3
	
	GPIO_PORTA_DIR_R &= ~0x04;        /* make PB6 an input pin */   
	GPIO_PORTA_DEN_R |= 0x04;         /* make PB6 as digital pin */		
	GPIO_PORTA_AFSEL_R &= ~0x04; //enable alternate function on PE3	
	GPIO_PORTE_AMSEL_R &= ~0x04; //enable analog function on PE3
	
	GPIO_PORTF_DIR_R |= 0x0E;        /* make PB6 an input pin */   
	GPIO_PORTF_DEN_R |= 0x0E;         /* make PB6 as digital pin */			
}

//ADC0 initializaiton
void ADC0_Init(void)
{
		SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ); // configure the system clock to be 40MHz
		SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);	//activate the clock of ADC0
		GPIOPinTypeADC(GPIO_PORTE_BASE,GPIO_PIN_3); //Set PE3 as ADC
		ADCSequenceDisable(ADC0_BASE, 3); //disable ADC0 before the configuration is complete
		ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0); // will use ADC0, SS3, processor-trigger, priority 0
		ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0|ADC_CTL_IE|ADC_CTL_END); //ADC0 SS3 Step 0, sample from external temperature sensor
		//ADC0 SS3 Step 0, sample from internal temperature sensor, completion of this step will set RIS, last sample of the sequence
		IntPrioritySet(INT_ADC0SS3, 0 );  	 // configure ADC0 SS3 interrupt priority as 0
		IntEnable(INT_ADC0SS3);    				// enable interrupt 31 in NVIC (ADC0 SS3)
		ADCIntEnableEx(ADC0_BASE, ADC_INT_SS3);      // arm interrupt of ADC0 SS3
		ADCSequenceEnable(ADC0_BASE, 3); //enable ADC0
}
