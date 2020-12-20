/*
 * \file    UsbphyGS70.c
 *
 * \brief   This file contains AM335x USB Phy  related functions.
 *
*/

/* Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#include "hw_types.h"
#include "soc_AM335x.h"	
#include "hw_usbphyGS70.h"

/**
 * \brief This function will switch on the USB Phy  
 *          
 *
 * \param    None
 *
 * \return   None
 *
  **/
void UsbPhyOn()
{
	unsigned int  usbphycfg = 0;

	usbphycfg = HWREG(CFGCHIP2_USBPHYCTRL); 
	usbphycfg &= ~(USBPHY_CM_PWRDN | USBPHY_OTG_PWRDN);
	usbphycfg |= (USBPHY_OTGVDET_EN | USBPHY_OTGSESSEND_EN);

	HWREG(CFGCHIP2_USBPHYCTRL) = usbphycfg;		
}


/**
 * \brief This function will switch off  the USB Phy  
 *          
 *
 * \param    None
 *
 * \return   None
 *
  **/
void UsbPhyOff()
{
	unsigned int  usbphycfg = 0;
	
	usbphycfg = HWREG(CFGCHIP2_USBPHYCTRL); 
	usbphycfg |= (USBPHY_CM_PWRDN | USBPHY_OTG_PWRDN);
	HWREG(CFGCHIP2_USBPHYCTRL) = usbphycfg;
}

