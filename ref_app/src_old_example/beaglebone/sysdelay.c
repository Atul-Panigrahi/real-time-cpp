
/*
 * \file   sysdelay.c
 *
 * \brief  This file contains functions that configures a DMTimer instance
 *         for operation and to generate a requested amount of delay.
 *
*/

/* Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */
#include "soc_AM335x.h"
#include "beaglebone.h"
#include "interrupt.h"
#include "dmtimer.h"
#include "delay.h"

/******************************************************************************
**                      INTERNAL MACRO DEFINITIONS
*******************************************************************************/
#define TIMER_INITIAL_COUNT             (0xFFFFA23Fu)

/* If delay using interrupts is desire define this. If polling is desired,
   undefine this */
#define DELAY_USE_INTERRUPTS            1  

/******************************************************************************
**                      INTERNAL FUNCTION PROTOTYPES
*******************************************************************************/
static void DMTimerIsr(void);

/******************************************************************************
**                      INTERNAL VARIABLE DEFINITIONS
*******************************************************************************/
static volatile unsigned int flagIsr = 1;

/******************************************************************************
**                      FUNCTION DEFINITIONS
*******************************************************************************/

/**
 * This function configures the DMTimer7 instance. It also registers the
 * DMTimer Interrupt Service Routine in the Interrupt Controller, sets the
 * priority and enables the respective system interrupt.
 */

void SysDelayTimerSetup(void)
{   

#ifdef DELAY_USE_INTERRUPTS
    /* This function will enable clocks for the DMTimer7 instance */
    DMTimer7ModuleClkConfig();

    /* Registering DMTimerIsr */
    IntRegister(SYS_INT_TINT7, DMTimerIsr);

    /* Set the priority */
    IntPrioritySet(SYS_INT_TINT7, 0, AINTC_HOSTINT_ROUTE_IRQ);

    /* Enable the system interrupt */
    IntSystemEnable(SYS_INT_TINT7);

    DMTimerCounterSet(SOC_DMTIMER_7_REGS, 0);

    /* Load the load register with the reload count value */
    DMTimerReloadSet(SOC_DMTIMER_7_REGS, TIMER_INITIAL_COUNT);

    /* Configure the DMTimer for Auto-reload and compare mode */
    DMTimerModeConfigure(SOC_DMTIMER_7_REGS, DMTIMER_AUTORLD_NOCMP_ENABLE);
#else
    DMTimer7ModuleClkConfig();

    DMTimerModeConfigure(SOC_DMTIMER_7_REGS, DMTIMER_ONESHOT_NOCMP_ENABLE);
#endif

}

/**
 * This function generates the requested amount of delay in milliseconds.
 */

void Sysdelay(unsigned int milliSec)
{
#ifdef DELAY_USE_INTERRUPTS
    DMTimerCounterSet(SOC_DMTIMER_7_REGS, TIMER_INITIAL_COUNT);

    flagIsr = milliSec;

    /* Enable the DMTimer interrupts */
    DMTimerIntEnable(SOC_DMTIMER_7_REGS, DMTIMER_INT_OVF_EN_FLAG);

    /* Start the DMTimer */
    DMTimerEnable(SOC_DMTIMER_7_REGS);
    while(flagIsr > 0) ;

    DMTimerDisable(SOC_DMTIMER_7_REGS);

    /* Disable the DMTimer interrupts */
    DMTimerIntDisable(SOC_DMTIMER_7_REGS, DMTIMER_INT_OVF_EN_FLAG);

#else
    while(milliSec != 0)
    {
        DMTimerCounterSet(SOC_DMTIMER_7_REGS, 0);
        DMTimerEnable(SOC_DMTIMER_7_REGS);
        while(DMTimerCounterGet(SOC_DMTIMER_7_REGS) < 0x5DC0);
        DMTimerDisable(SOC_DMTIMER_7_REGS);
        milliSec--;
    }
 
#endif
}

#ifdef DELAY_USE_INTERRUPTS
/*
** DMTimer Interrupt Service Routine.
*/

static void DMTimerIsr(void)
{
    /* Disable the DMTimer interrupts */
    DMTimerIntDisable(SOC_DMTIMER_7_REGS, DMTIMER_INT_OVF_EN_FLAG);

    /* Clear the status of the interrupt flags */
    DMTimerIntStatusClear(SOC_DMTIMER_7_REGS, DMTIMER_INT_OVF_EN_FLAG);

    flagIsr--;

    /* Notify end of interrupt */
    DMTimerEndOfInterrupt(SOC_DMTIMER_7_REGS);

    /* Enable the DMTimer interrupts */
    DMTimerIntEnable(SOC_DMTIMER_7_REGS, DMTIMER_INT_OVF_EN_FLAG);
}
#endif

