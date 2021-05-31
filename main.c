#include "Display.h"
#include "TemperatureSensor.h"
#include "Init.h"
#include "driverlib/adc.h"
#include "Motion_Sensor.h"

void Delay_ms(int time_ms);

int main(void)
{	
	I2C3_Init();
	OLED_Init();
	PortFunctionInit();
	
  while ( 1 ) {
		checkPIR();	
		Delay_ms(1000);
		OLED_Clear();
	}
			
} 

void Delay_ms(int time_ms)
{
    int i, j;
    for(i = 0 ; i < time_ms; i++)
        for(j = 0; j < 3180; j++)
            {}
}