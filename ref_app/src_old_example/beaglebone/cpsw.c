/*
 * \file   cpsw.c
 *
 * \brief  This file contains functions which configure CPSW instance
 */

/* Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */
#include "soc_AM335x.h"
#include "hw_control_AM335x.h"
#include "hw_types.h"
#include "beaglebone.h"
#include "hw_cm_per.h"

/******************************************************************************
**                       INTERNAL MACRO DEFINITIONS
******************************************************************************/
#define CPSW_MII_SEL_MODE                     (0x00u)
#define CPSW_MDIO_SEL_MODE                    (0x00u)
#define LEN_MAC_ADDR                          (0x06u)
#define OFFSET_MAC_ADDR                       (0x30u)

/******************************************************************************
**                          FUNCTION DEFINITIONS
******************************************************************************/
/**
 * \brief   This function selects the CPSW pins for use in RGMII mode.
 *          
 * \param   None
 *
 * \return  None.
 *
 */
void CPSWPinMuxSetup(void)
{
    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_RXERR) =
        CONTROL_CONF_MII1_RXERR_CONF_MII1_RXERR_RXACTIVE | CPSW_MII_SEL_MODE;
    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_TXEN) =  CPSW_MII_SEL_MODE;
    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_RXDV) =  
         CONTROL_CONF_MII1_RXDV_CONF_MII1_RXDV_RXACTIVE | CPSW_MII_SEL_MODE;
    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_TXD3) =  CPSW_MII_SEL_MODE;
    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_TXD2) =  CPSW_MII_SEL_MODE;
    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_TXD1) =  CPSW_MII_SEL_MODE;
    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_TXD0) =  CPSW_MII_SEL_MODE;
    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_TXCLK) = 
          CONTROL_CONF_MII1_TXCLK_CONF_MII1_TXCLK_RXACTIVE | CPSW_MII_SEL_MODE;
    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_RXCLK) = 
         CONTROL_CONF_MII1_RXCLK_CONF_MII1_RXCLK_RXACTIVE | CPSW_MII_SEL_MODE;
    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_RXD3) = 
         CONTROL_CONF_MII1_RXD3_CONF_MII1_RXD3_RXACTIVE | CPSW_MII_SEL_MODE;
    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_RXD2) = 
         CONTROL_CONF_MII1_RXD2_CONF_MII1_RXD2_RXACTIVE | CPSW_MII_SEL_MODE;
    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_RXD1) =  
         CONTROL_CONF_MII1_RXD1_CONF_MII1_RXD1_RXACTIVE | CPSW_MII_SEL_MODE;
    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MII1_RXD0) =  
         CONTROL_CONF_MII1_RXD0_CONF_MII1_RXD0_RXACTIVE | CPSW_MII_SEL_MODE;
    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MDIO_DATA) =
         CONTROL_CONF_MDIO_DATA_CONF_MDIO_DATA_RXACTIVE 
         | CONTROL_CONF_MDIO_DATA_CONF_MDIO_DATA_PUTYPESEL
         | CPSW_MDIO_SEL_MODE;
    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MDIO_CLK) =
         CONTROL_CONF_MDIO_CLK_CONF_MDIO_CLK_PUTYPESEL | CPSW_MDIO_SEL_MODE;
}

/**
 * \brief   Enables CPSW clocks
 *
 * \param   None
 *
 * \return  None.
 */
void CPSWClkEnable(void)
{
    HWREG(SOC_PRCM_REGS + CM_PER_CPGMAC0_CLKCTRL) =
                      CM_PER_CPGMAC0_CLKCTRL_MODULEMODE_ENABLE;

    while(0 != (HWREG(SOC_PRCM_REGS + CM_PER_CPGMAC0_CLKCTRL)
                & CM_PER_CPGMAC0_CLKCTRL_IDLEST));
 
    HWREG(SOC_PRCM_REGS + CM_PER_CPSW_CLKSTCTRL) =
                      CM_PER_CPSW_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while(0 == (HWREG(SOC_PRCM_REGS + CM_PER_CPSW_CLKSTCTRL)
                & CM_PER_CPSW_CLKSTCTRL_CLKACTIVITY_CPSW_125MHZ_GCLK));
} 

/**
 * \brief   This function sets the RGMII mode for both ports
 *
 * \param   None 
 *
 * \return  None.
 */
void EVMPortMIIModeSelect(void)
{
    /* Select MII, Internal Delay mode */
    HWREG(SOC_CONTROL_REGS + CONTROL_GMII_SEL) = 0x00;   
}

/**
 * \brief   This function returns the MAC address for the EVM
 *
 * \param   addrIdx    the MAC address index. 
 * \param   macAddr    the Pointer where the MAC address shall be stored
 *     'addrIdx' can be either 0 or 1
 *
 * \return  None.
 */
void EVMMACAddrGet(unsigned int addrIdx, unsigned char *macAddr)
{
    macAddr[0] =  (HWREG(SOC_CONTROL_REGS + CONTROL_MAC_ID_LO(addrIdx)) 
                   >> 8) & 0xFF;
    macAddr[1] =  (HWREG(SOC_CONTROL_REGS + CONTROL_MAC_ID_LO(addrIdx)))
                  & 0xFF;
    macAddr[2] =  (HWREG(SOC_CONTROL_REGS + CONTROL_MAC_ID_HI(addrIdx))
                   >> 24) & 0xFF;
    macAddr[3] =  (HWREG(SOC_CONTROL_REGS + CONTROL_MAC_ID_HI(addrIdx))
                   >> 16) & 0xFF;
    macAddr[4] =  (HWREG(SOC_CONTROL_REGS + CONTROL_MAC_ID_HI(addrIdx))
                   >> 8) & 0xFF;
    macAddr[5] =  (HWREG(SOC_CONTROL_REGS + CONTROL_MAC_ID_HI(addrIdx)))
                  & 0xFF;
}

/****************************** End Of File *********************************/
