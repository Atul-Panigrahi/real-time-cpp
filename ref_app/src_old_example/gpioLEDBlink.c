/* 
 * \file   gpioLEDBlink.c
 *
 * \brief  This application uses a GPIO pin to control the blinking of
 *         an LED present on the baseboard.
 */

#include "soc_AM335x.h"
#include "beaglebone.h"
#include "gpio_v2.h"


/*****************************************************************************
**                INTERNAL MACRO DEFINITIONS
*****************************************************************************/
#define GPIO_INSTANCE_ADDRESS           (SOC_GPIO_1_REGS)
#define GPIO_INSTANCE_PIN_NUMBER        (23)

/*****************************************************************************
**                INTERNAL FUNCTION PROTOTYPES
*****************************************************************************/
static void Delay(unsigned int count);

/*****************************************************************************
**                INTERNAL FUNCTION DEFINITIONS
*****************************************************************************/

int main()
{
    /* Enabling functional clocks for GPIO0 instance. */
    GPIO1ModuleClkConfig();

    /* Selecting GPIO0[19] pin for use. */
    GPIO1Pin23PinMuxSetup();
    
    /* Enabling the GPIO module. */
    GPIOModuleEnable(GPIO_INSTANCE_ADDRESS);

    /* Resetting the GPIO module. */
    GPIOModuleReset(GPIO_INSTANCE_ADDRESS);

    /* Setting the GPIO pin as an output pin. */
    GPIODirModeSet(GPIO_INSTANCE_ADDRESS,
                   GPIO_INSTANCE_PIN_NUMBER,
                   GPIO_DIR_OUTPUT);
    while(1)
    {
        /* Driving a logic HIGH on the GPIO pin. */
        GPIOPinWrite(GPIO_INSTANCE_ADDRESS,
                     GPIO_INSTANCE_PIN_NUMBER,
                     GPIO_PIN_HIGH);

        Delay(0x3FFFF);

        /* Driving a logic LOW on the GPIO pin. */
        GPIOPinWrite(GPIO_INSTANCE_ADDRESS,
                     GPIO_INSTANCE_PIN_NUMBER,
                     GPIO_PIN_LOW);

        Delay(0x3FFFF);
    }

} 

/*
** A function which is used to generate a delay.
*/

static void Delay(volatile unsigned int count)
{
    while(count--);
}


/******************************* End of file *********************************/
