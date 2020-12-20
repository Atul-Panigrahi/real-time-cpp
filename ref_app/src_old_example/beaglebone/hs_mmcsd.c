/*
 * \file   hs_mmcsd.c
 *
 * \brief  This file contains functions which configure the HS/MMCSD controller
 */

/* Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */
#include "hw_control_AM335x.h"
#include "soc_AM335x.h"
#include "hw_cm_per.h"
#include "hw_types.h"
#include "hw_cm_dpll.h"
#include "hs_mmcsd.h"

void HSMMCSDPinMuxSetup(void)
{
    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MMC0_DAT3) =
                   (0 << CONTROL_CONF_MMC0_DAT3_CONF_MMC0_DAT3_MMODE_SHIFT)    |
                   (0 << CONTROL_CONF_MMC0_DAT3_CONF_MMC0_DAT3_PUDEN_SHIFT)    |
                   (1 << CONTROL_CONF_MMC0_DAT3_CONF_MMC0_DAT3_PUTYPESEL_SHIFT)|
                   (1 << CONTROL_CONF_MMC0_DAT3_CONF_MMC0_DAT3_RXACTIVE_SHIFT);

    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MMC0_DAT2) =
                   (0 << CONTROL_CONF_MMC0_DAT2_CONF_MMC0_DAT2_MMODE_SHIFT)    |
                   (0 << CONTROL_CONF_MMC0_DAT2_CONF_MMC0_DAT2_PUDEN_SHIFT)    |
                   (1 << CONTROL_CONF_MMC0_DAT2_CONF_MMC0_DAT2_PUTYPESEL_SHIFT)|
                   (1 << CONTROL_CONF_MMC0_DAT2_CONF_MMC0_DAT2_RXACTIVE_SHIFT);

    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MMC0_DAT1) =
                   (0 << CONTROL_CONF_MMC0_DAT1_CONF_MMC0_DAT1_MMODE_SHIFT)    |
                   (0 << CONTROL_CONF_MMC0_DAT1_CONF_MMC0_DAT1_PUDEN_SHIFT)    |
                   (1 << CONTROL_CONF_MMC0_DAT1_CONF_MMC0_DAT1_PUTYPESEL_SHIFT)|
                   (1 << CONTROL_CONF_MMC0_DAT1_CONF_MMC0_DAT1_RXACTIVE_SHIFT);

    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MMC0_DAT0) =
                   (0 << CONTROL_CONF_MMC0_DAT0_CONF_MMC0_DAT0_MMODE_SHIFT)    |
                   (0 << CONTROL_CONF_MMC0_DAT0_CONF_MMC0_DAT0_PUDEN_SHIFT)    |
                   (1 << CONTROL_CONF_MMC0_DAT0_CONF_MMC0_DAT0_PUTYPESEL_SHIFT)|
                   (1 << CONTROL_CONF_MMC0_DAT0_CONF_MMC0_DAT0_RXACTIVE_SHIFT);

    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MMC0_CLK) =
                   (0 << CONTROL_CONF_MMC0_CLK_CONF_MMC0_CLK_MMODE_SHIFT)    |
                   (0 << CONTROL_CONF_MMC0_CLK_CONF_MMC0_CLK_PUDEN_SHIFT)    |
                   (1 << CONTROL_CONF_MMC0_CLK_CONF_MMC0_CLK_PUTYPESEL_SHIFT)|
                   (1 << CONTROL_CONF_MMC0_CLK_CONF_MMC0_CLK_RXACTIVE_SHIFT);
 
    HWREG(SOC_CONTROL_REGS + CONTROL_CONF_MMC0_CMD) =
                   (0 << CONTROL_CONF_MMC0_CMD_CONF_MMC0_CMD_MMODE_SHIFT)    |
                   (0 << CONTROL_CONF_MMC0_CMD_CONF_MMC0_CMD_PUDEN_SHIFT)    |
                   (1 << CONTROL_CONF_MMC0_CMD_CONF_MMC0_CMD_PUTYPESEL_SHIFT)|
                   (1 << CONTROL_CONF_MMC0_CMD_CONF_MMC0_CMD_RXACTIVE_SHIFT);
}


/**
 * \brief   This function will configure the required clocks for HS MMC/SD instance.
 *
 * \return  None.
 *
 */
void HSMMCSDModuleClkConfig(void)
{
    HWREG(SOC_PRCM_REGS + CM_PER_MMC0_CLKCTRL) |= 
                             CM_PER_MMC0_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_PRCM_REGS + CM_PER_MMC0_CLKCTRL) & 
      CM_PER_MMC0_CLKCTRL_MODULEMODE) != CM_PER_MMC0_CLKCTRL_MODULEMODE_ENABLE);
}

