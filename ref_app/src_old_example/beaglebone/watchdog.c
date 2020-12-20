/*
 * \file   watchdog.c
 *
 * \brief  This file contains functions which configure clock for watchdog 
 *         timer.
 */

/* Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */
#include "soc_AM335x.h"
#include "hw_types.h"
#include "beaglebone.h"
#include "hw_cm_per.h"
#include "hw_cm_wkup.h"
#include "hw_cm_dpll.h"

/*
** No pin muxing is required for WDT
*/

/*
** Enable clocks for WDT.
*/
void WatchdogTimer1ModuleClkConfig(void)
{
    /* Select 32kHz clock from 32K clock divider for WDT1 */
    HWREG(SOC_CM_DPLL_REGS + CM_DPLL_CLKSEL_WDT1_CLK) = 
                                    CM_DPLL_CLKSEL_WDT1_CLK_CLKSEL_SEL2;
   
    while((HWREG(SOC_CM_DPLL_REGS + CM_DPLL_CLKSEL_WDT1_CLK) & 
    CM_DPLL_CLKSEL_WDT1_CLK_CLKSEL) != CM_DPLL_CLKSEL_WDT1_CLK_CLKSEL_SEL2);

    /* Configuration of L4_PER bus */
    HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) =
                             CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) &
    CM_PER_L3S_CLKSTCTRL_CLKTRCTRL) != CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) =
                             CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) &
     CM_PER_L3_CLKSTCTRL_CLKTRCTRL) != CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) =
                             CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) &
                               CM_PER_L3_INSTR_CLKCTRL_MODULEMODE) !=
                                   CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE);

    HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) =
                             CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) &
        CM_PER_L3_CLKCTRL_MODULEMODE) != CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE);

    HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) =
                             CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
                              CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL) !=
                                CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    /* Configuration for L4_WKUP bus */
    HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) = 
                             CM_WKUP_CLKSTCTRL_CLKTRCTRL_SW_WKUP;        

    while((HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) & 
           CM_WKUP_CLKSTCTRL_CLKTRCTRL) != CM_WKUP_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CONTROL_CLKCTRL) = 
                                      CM_WKUP_CONTROL_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CONTROL_CLKCTRL) & 
     CM_WKUP_CONTROL_CLKCTRL_MODULEMODE) != CM_WKUP_CONTROL_CLKCTRL_MODULEMODE_ENABLE);

    HWREG(SOC_CM_WKUP_REGS + CM_WKUP_DEBUGSS_CLKCTRL) |= 
                                    CM_WKUP_DEBUGSS_CLKCTRL_MODULEMODE_ENABLE;
    
    while((HWREG(SOC_CM_WKUP_REGS + CM_WKUP_DEBUGSS_CLKCTRL) & 
      CM_WKUP_DEBUGSS_CLKCTRL_MODULEMODE) != CM_WKUP_DEBUGSS_CLKCTRL_MODULEMODE_ENABLE);

    HWREG(SOC_CM_WKUP_REGS + CM_WKUP_WDT1_CLKCTRL) = 
                                   CM_WKUP_WDT1_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_CM_WKUP_REGS + CM_WKUP_WDT1_CLKCTRL) & 
           CM_WKUP_WDT1_CLKCTRL_MODULEMODE) != CM_WKUP_WDT1_CLKCTRL_MODULEMODE_ENABLE);

    while(!(HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) & 
           (CM_WKUP_CLKSTCTRL_CLKACTIVITY_L4_WKUP_GCLK | 
            CM_WKUP_CLKSTCTRL_CLKACTIVITY_WDT1_GCLK)));

    while(!(HWREG(SOC_CM_WKUP_REGS + CM_WKUP_L4WKUP_CLKCTRL) & 0x02u));

    while(!(HWREG(SOC_CM_WKUP_REGS + CM_WKUP_WKUP_M3_CLKCTRL) & 0x02u));

    while(!(HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL) & 0x02u));
}

