#include "inc/tm4c123gh6pm.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int I2C_wait_till_done(void);


char I2C3_Write_Multiple(int slave_address, char slave_memory_address, int bytes_count, uint8_t* data);


char I2C3_Wr(int slaveAddr, char memAddr, uint8_t data);