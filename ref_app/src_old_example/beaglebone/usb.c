
/**
 * \file   usb.c
 *
 * \brief  This file contains the board specific code for enabling the use of
 *         usb driver.
 */

/*  Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
 *  ALL RIGHTS RESERVED. */

#include "soc_AM335x.h"
#include "hw_control_AM335x.h"
#include "hw_usbOtg_AM335x.h"
#include "hw_types.h"
#include "beaglebone.h"
#include "hw_cm_per.h"
#include "hw_cm_wkup.h"

/**  
 * \brief  This API returns a unique number which identifies itself  
 *         with the USB IP in AM335x SoC.  
 * \param  None  
 * \return This returns a number '2' which is unique to USB IP in AM335x.
 */
unsigned int USBVersionGet(void)
{
    return 2;
}

/**
 * \brief   This function enables USB clocks
 *          
 * \param   None
 *
 * \return  None.
 *
 */
void USB0ModuleClkConfig(void)
{
	HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_CLKDCOLDO_DPLL_PER) |= 
		(CM_WKUP_CM_CLKDCOLDO_DPLL_PER_DPLL_CLKDCOLDO_GATE_CTRL | 
					CM_WKUP_CM_CLKDCOLDO_DPLL_PER_ST_DPLL_CLKDCOLDO);

	
	HWREG(SOC_PRCM_REGS + CM_PER_USB0_CLKCTRL) |=
                             CM_PER_USB0_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_PRCM_REGS + CM_PER_USB0_CLKCTRL) &
      CM_PER_USB0_CLKCTRL_MODULEMODE) != CM_PER_USB0_CLKCTRL_MODULEMODE_ENABLE);


    /*
    ** Waiting for IDLEST field in CM_PER_USB0_CLKCTRL register to attain the
    ** desired value.
    */
    while((CM_PER_USB0_CLKCTRL_IDLEST_FUNC <<
           CM_PER_USB0_CLKCTRL_IDLEST_SHIFT)!=
          (HWREG(SOC_CM_PER_REGS + CM_PER_USB0_CLKCTRL) &
           CM_PER_USB0_CLKCTRL_IDLEST));

}

/**  
 * \brief  This API  enables the USB Interrupts through subsystem specific wrapper 
 *	       registers
 * \param  Base address 
 * \return None
 */
void USBEnableInt(unsigned int ulBase)
{
	HWREG(ulBase + USB_0_IRQ_ENABLE_SET_0) = 0xFFFFFFFF;
	HWREG(ulBase + USB_0_IRQ_ENABLE_SET_1) = 0xFFFFFFFF;
#ifdef DMA_MODE
	HWREG(USBSS_BASE + USBSS_IRQ_ENABLE_SET) = 0xFFFFFFFF;
#endif
}

/**  
 * \brief  This API  Clear  the USB Interrupts through subsystem specific wrapper 
 *	       registers
 * \param  Base address 
 * \return None
 */
void USBClearInt(unsigned int ulBase)
{

}
/**  
 * \brief  This API  enables the USB Module clock
 *	       registers
 * \param  Base address 
 * \return None
 */
void USBModuleClkEnable(unsigned int ulIndex, unsigned int ulBase)
{
	//
	//Call the clock enabel API
	//
	USB0ModuleClkConfig();	
}

/**  
 * \brief  This API Disables the module clock
 *	       registers
 * \param  Base address 
 * \return None
 */
void USBModuleClkDisable(unsigned int ulIndex, unsigned int ulBase)
{
	//
	//Disables the module clock
	//
	HWREG(SOC_PRCM_REGS + CM_PER_USB0_CLKCTRL) |=
                             CM_PER_USB0_CLKCTRL_MODULEMODE_DISABLE;
}

/****************************** End Of File *********************************/
