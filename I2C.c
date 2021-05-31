#include "inc/tm4c123gh6pm.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom_map.h"


/* wait until I2C Master module is busy */
/*  and if not busy and no error return 0 */
 int I2C_wait_till_done(void)
{
    while(I2C3_MCS_R & 1);   /* wait until I2C master is not busy */
    return I2C3_MCS_R & 0xE; /* return I2C error code, 0 if no error*/
}

char I2C3_Wr(int slaveAddr, char memAddr, uint8_t data)
{

    char error;

    /* send slave address and starting address */
    I2C3_MSA_R = slaveAddr << 1;
    I2C3_MDR_R = memAddr;
    I2C3_MCS_R = 3;                      /* S-(saddr+w)-ACK-maddr-ACK */

    error = I2C_wait_till_done();       /* wait until write is complete */
    if (error) return error;

    /* send data */
    I2C3_MDR_R = data;
    I2C3_MCS_R = 5;                      /* -data-ACK-P */
    error = I2C_wait_till_done();       /* wait until write is complete */
    while(I2C3_MCS_R & 0x40);            /* wait until bus is not busy */
    error = I2C3_MCS_R & 0xE;
    if (error) return error;

    return 0;       /* no error */
}


// Receive one byte of data from I2C slave device
char I2C3_Write_Multiple(int slave_address, char slave_memory_address, int bytes_count, uint8_t* data)
{   
    char error;
    if (bytes_count <= 0)
        return -1;                  /* no write was performed */
    /* send slave address and starting address */
    I2C3_MSA_R = slave_address << 1;
    I2C3_MDR_R = slave_memory_address;
    I2C3_MCS_R = 3;                  /* S-(saddr+w)-ACK-maddr-ACK */

    error = I2C_wait_till_done();   /* wait until write is complete */
    if (error) return error;

    /* send data one byte at a time */
    while (bytes_count > 1)
    {
        I2C3_MDR_R = *data++;             /* write the next byte */
        I2C3_MCS_R = 1;                   /* -data-ACK- */
        error = I2C_wait_till_done();
        if (error) return error;
        bytes_count--;
    }
    
    /* send last byte and a STOP */
    I2C3_MDR_R = *data++;                 /* write the last byte */
    I2C3_MCS_R = 5;                       /* -data-ACK-P */
    error = I2C_wait_till_done();
    while(I2C3_MCS_R & 0x40);             /* wait until bus is not busy */
    if (error) return error;
    return 0;       /* no error */
}