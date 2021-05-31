#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"


/*******************************************************************************
Function: void OLED_Init ()
Returns: Nothing
Description: Initializes OLED 
 ******************************************************************************/
void OLED_Init(void);

/*******************************************************************************
Function: void I2C3_Init ()
Returns: Nothing
Description: Initializes I2C3 
 ******************************************************************************/
void I2C3_Init ( void );

/*******************************************************************************
Function: void PortFunctionInit()
Returns: Nothing
Description: Initializes ports that will be utilized
 ******************************************************************************/
void PortFunctionInit(void);


void ADC0_Init(void);