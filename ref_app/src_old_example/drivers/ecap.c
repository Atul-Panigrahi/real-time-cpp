/**
 *  \file   ecap.c
 *
 *  \brief  ECAP APIs.
 *
 *   This file contains the device abstraction layer APIs for ECAP
 */

/* Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */


#include "hw_ecap.h"
#include "hw_types.h"
#include "ecap.h"


/*******************************************************************************
*                        API FUNCTION DEFINITIONS
*******************************************************************************/

/**
 * \brief   This function enables capture loading.
 *
 * \param   baseAdd   It is the Memory address of the ECAP instance used.
 *
 * \return  None.
 *
 **/
void ECAPCaptureLoadingEnable(unsigned int baseAdd)
{
    HWREGH(baseAdd + ECAP_ECCTL1) |= ECAP_ECCTL1_CAPLDEN; 
}

/**
 * \brief   This function disables capture loading
 *
 * \param   baseAdd   It is the Memory address of the ECAP instance used.
 *
 * \return  None.
 *
 **/
void ECAPCaptureLoadingDisable(unsigned int baseAdd)
{
    HWREGH(baseAdd + ECAP_ECCTL1) &= ~ECAP_ECCTL1_CAPLDEN; 
}

/**
 * \brief   This function configures prescale value.
 *
 * \param   baseAdd   It is the Memory address of the ECAP instance used.
 * \param   prescale  It is the value which is used to prescale the incoming
 *                    input.
 *
 *          prescale can take any integer value between 0 and 62
 *
 * \return  None.
 *
 **/
void ECAPPrescaleConfig(unsigned int baseAdd, unsigned int prescale)
{
    HWREGH(baseAdd + ECAP_ECCTL1) &= 0xffffc1ff;
    HWREGH(baseAdd + ECAP_ECCTL1) |= (prescale << ECAP_ECCTL1_PRESCALE_SHIFT);
}

/**
 * \brief   This function configures ecapture module to operate in capture
 *          mode or in APWM mode.
 *
 * \param   baseAdd     It is the Memory address of the ECAP instance used.
 * \param   modeSelect  It is the value which determines whether ecapture
 *                      module to operate in capture mode or in APWM mode.\n
 * 
 *         modeSelect can take following macros.
 *
 *         ECAP_CAPTURE_MODE - Capture Mode.\n
 *         ECAP_APWM_MODE    - APWM Mode.\n
 *
 * \return  None.
 *
 **/
void ECAPOperatingModeSelect(unsigned int baseAdd, unsigned int modeSelect)
{
    if(modeSelect)
    {
         HWREGH(baseAdd + ECAP_ECCTL2) &= ~ECAP_ECCTL2_CAP_APWM;
    }
    else
    {
         HWREGH(baseAdd + ECAP_ECCTL2) |=  ECAP_ECCTL2_CAP_APWM;
    }
}
    
/**
 * \brief   This function returns time-stamp for a given capture event.
 *
 * \param   baseAdd     It is the Memory address of the ECAP instance used.
 * \param   capEvtFlag  It is the value which determines for which capture event
 *                      time-stam has to returned.
 * 
 *         capEvtFlag can take following macros.
 *
 *         ECAP_CAPTURE_EVENT_1 - Capture Event 1.\n
 *         ECAP_CAPTURE_EVENT_2 - Capture Event 2.\n
 *         ECAP_CAPTURE_EVENT_3 - Capture Event 3.\n
 *         ECAP_CAPTURE_EVENT_4 - Capture Event 4.\n
 *
 * \return  Returns the time-stamp for given capure event.
 *
 **/
unsigned int ECAPTimeStampRead(unsigned int baseAdd, unsigned int capEvtFlag)
{
    return(HWREG(baseAdd + capEvtFlag));
}

/**
 * \brief   This function configures the counter register which is used as 
 *          Capture Time base.
 *
 * \param   baseAdd     It is the Memory address of the ECAP instance used.
 * \param   countVal    It is counter value to be configured.
 *
 * \return  None.
 *
 **/
void ECAPCounterConfig(unsigned int baseAdd, unsigned int countVal)
{
    HWREG(baseAdd + ECAP_TSCTR) = countVal;
}

/**
 * \brief   This function configures Capture Event polarity.
 *          
 * \param   baseAdd     It is the Memory address of the ECAP instance used.
 * \param   capEvt1pol  It determines whether Capture Event1 has to be generated
 *                      on rising edge or falling edge of pulse.
 *
 * \param   capEvt2pol  It determines whether Capture Event2 has to be generated
 *                      on rising edge or falling edge of pulse.
 *
 * \param   capEvt3pol  It determines whether Capture Event3 has to be generated
 *                      on rising edge or falling edge of pulse.
 *
 * \param   capEvt4pol  It determines whether Capture Event4 has to be generated
 *                      on rising edge or falling edge of pulse.
 * 
 *                      0 - falling edge
 *                      1 - rising  edge
 * \return  None.
 *
 **/
void ECAPCapeEvtPolarityConfig(unsigned int baseAdd, unsigned int capEvt1pol,
                               unsigned int capEvt2pol, unsigned int capEvt3pol,
                               unsigned int capEvt4pol)
{
    HWREGH(baseAdd + ECAP_ECCTL1) |= capEvt1pol << 0;
    HWREGH(baseAdd + ECAP_ECCTL1) |= capEvt1pol << 2;
    HWREGH(baseAdd + ECAP_ECCTL1) |= capEvt1pol << 4;
    HWREGH(baseAdd + ECAP_ECCTL1) |= capEvt1pol << 6;
}

/**
 * \brief   This function configures Capture Event polarity.
 *          
 * \param   baseAdd      It is the Memory address of the ECAP instance used.
 * \param   CounterRst1  It determines whether counter has to be reset upon 
 *                       Capture Event1.
 *
 * \param   CounterRst2  It determines whether counter has to be reset upon 
 *                       Capture Event2.
 *
 * \param   CounterRst3  It determines whether counter has to be reset upon 
 *                       Capture Event3.
 *
 * \param   CounterRst4  It determines whether counter has to be reset upon 
 *                       Capture Event4.
 *
 *                       0 - Don't reset counter upon capture event.\n
 *                       1 - Reset upon counter capture event.\n
 *
 * \return  None.
 *
 **/
void ECAPCaptureEvtCntrRstConfig(unsigned int baseAdd, unsigned int CounterRst1,
                                 unsigned int CounterRst2,unsigned int CounterRst3,
                                 unsigned int CounterRst4)
{
    HWREGH(baseAdd + ECAP_ECCTL1) |= CounterRst1 << 1;
    HWREGH(baseAdd + ECAP_ECCTL1) |= CounterRst2 << 3;
    HWREGH(baseAdd + ECAP_ECCTL1) |= CounterRst3 << 5;
    HWREGH(baseAdd + ECAP_ECCTL1) |= CounterRst4 << 7;
}

/**
 * \brief   This function configures ECAP to Continuous mode.
 *
 * \param   baseAdd   It is the Memory address of the ECAP instance used.
 *
 * \return  None.\n
 *
 * This API is valid only if ECAP is configured to Capture Mode.It has
 * no significance when ECAP is configured in APWM mode.
 **/
void ECAPContinousModeConfig(unsigned int baseAdd)
{
    HWREGH(baseAdd + ECAP_ECCTL2) &= ~ECAP_ECCTL2_CONT_ONESHT;  
}

/**
 * \brief   This function configures ECAP to One-shot mode and also
 *          stop value for this mode.
 *
 * \param   baseAdd   It is the Memory address of the ECAP instance used.
 *
 * \param   stopVal   It is the number of captures allowed to occur before
 *                    Capture register(1-4) are frozen.\n
 *
 *          stopVal can take following macros.\n
 *
 *          ECAP_CAPTURE_EVENT1_STOP - stop after Capture Event 1\n.
 *          ECAP_CAPTURE_EVENT2_STOP - stop after Capture Event 2\n.
 *          ECAP_CAPTURE_EVENT3_STOP - stop after Capture Event 3\n.
 *          ECAP_CAPTURE_EVENT4_STOP - stop after Capture Event 4\n.
 *
 * \return  None.\n
 *
 * This API is valid only if ECAP is configured to Capture Mode.It has
 * no significance when ECAP is configured in APWM mode.
 **/
void ECAPOneShotModeConfig(unsigned int baseAdd, unsigned int stopVal)
{
    HWREGH(baseAdd + ECAP_ECCTL2) |= ECAP_ECCTL2_CONT_ONESHT;
   
    HWREGH(baseAdd + ECAP_ECCTL2) &= 0xfffffff9;

    HWREGH(baseAdd + ECAP_ECCTL2) |= stopVal;
}  

/**
 * \brief   This function configures ECAP to One-Short Re-arming.
 *
 * \param   baseAdd   It is the Memory address of the ECAP instance used.
 *
 * \return  None.\n
 * 
 * When this API is invoked following things happen.\n
 *
 * 1. Resets Mod4 counter to zero.\n
 * 2. Un-freezes the Mod4 counter.\n
 * 3. Enables capture register loads.\n
 *
 **/
void ECAPOneShotREARM(unsigned int baseAdd)
{
    HWREGH(baseAdd + ECAP_ECCTL2) |= ECAP_ECCTL2_RE_ARM;
}

/**
 * \brief   This function configures output polarity for APWM output.
 *
 * \param   baseAdd   It is the Memory address of the ECAP instance used.
 * \param   flag      It is the value which determines the output polarity
 *                    for APWM output.\n
 *
 *         flag can take following macros.\n
 *
 *         ECAP_APWM_ACTIVE_HIGH.\n
 *         ECAP_APWM_ACTIVE_LOW.\n
 *
 * \return  None.
 * 
 **/
void ECAPAPWMPolarityConfig(unsigned int baseAdd, unsigned int flag)
{
    if(flag)
    {
         HWREGH(baseAdd + ECAP_ECCTL2) |= ECAP_ECCTL2_APWMPOL;
    }
    else
    {
         HWREGH(baseAdd + ECAP_ECCTL2) &= ~ECAP_ECCTL2_APWMPOL;
    }
}

/**
 * \brief   This function configures counter to stop or free running
 *          based on its input argument flag.
 *
 * \param   baseAdd   It is the Memory address of the ECAP instance used.
 * \param   flag      It is the value which determine counter to be configured
 *                    to stop or free running.\n
 *
 *         flag can take following macros.\n
 *
 *         ECAP_COUNTER_STOP.\n
 *         ECAP_COUNTER_FREE_RUNNING.\n
 *
 * \return  None.
 * 
 **/
void ECAPCounterControl(unsigned int baseAdd, unsigned int flag)
{
    if(flag)
    {
         HWREGH(baseAdd + ECAP_ECCTL2) |= ECAP_ECCTL2_TSCTRSTOP;
    }
    else
    {
         HWREGH(baseAdd + ECAP_ECCTL2) &= ~ECAP_ECCTL2_TSCTRSTOP;
    }
}

/**
 * \brief   This function configures Sync-In and Sync-Out.
 *
 * \param   baseAdd   It is the Memory address of the ECAP instance used.
 * \param   syncIn    It is the value which determines whether to disable
 *                    syncIn or to enable counter to be loaded from
 *                    CNTPHS register upon a SYNCI signal.\n
 *
 *         syncIn can take following macros.\n
 *
 *         ECAP_SYNC_IN_DISABLE.\n
 *         ECAP_ENABLE_COUNTER - Enables counter to load from CNTPHS
 *                               register upon SYNCI signal.\n
 *
 * \param   syncOut   It is the value which select type of syncOut signal
 *                    (i.e select syncIn event to be the Sync-Out signal,
 *                         select PRD_eq event to be Sync-Out signal).\n
 *                    
 *
 *         syncOut can take following macros.\n
 *
 *         ECAP_SYNC_IN - Select syncIn event to be the Sync-Out signal.\n
 *         ECAP_PRD_EQ  - Select PRD_eq event to be Sync-Out signal.\n
 *         ECAP_SYNC_OUT_DISABLE - Disable syncOut signal.\n
 *
 * \return  None.
 * 
 **/
void ECAPSyncInOutSelect(unsigned int baseAdd, unsigned int syncIn,
                         unsigned int syncOut)
{
    HWREGH(baseAdd + ECAP_ECCTL2) &= 0xffffffdf;
    HWREGH(baseAdd + ECAP_ECCTL2) |= syncIn;
    HWREGH(baseAdd + ECAP_ECCTL2) &= 0xffffff3f;
    HWREGH(baseAdd + ECAP_ECCTL2) |= syncOut;
}

/**
 * \brief   When ECAP module is configured in APWM mode capture 1 and capture 2
 *          registers are used as period and compare register.This function
 *          configures compare and period values to this register.
 *
 * \param   baseAdd    It is the Memory address of the ECAP instance used.
 * \param   compareVal It is the Compare value to be configured.
 * \param   periodVal  It is the Period value to be configured.
 * 
 * \return  None.
 *
 **/
void ECAPAPWMCaptureConfig(unsigned int baseAdd, unsigned int compareVal,
                           unsigned int periodVal)
{
    HWREG(baseAdd + ECAP_CAP1) = periodVal; 
    HWREG(baseAdd + ECAP_CAP2) = compareVal; 
}

/**
 * \brief   This function configures the Shadow register.
 *
 * \param   baseAdd    It is the Memory address of the ECAP instance used.
 * \param   compareVal It is the Compare value to be configured.
 * \param   periodVal  It is the Period value to be configured.
 * 
 * \return  None.
 *
 **/
void ECAPAPWMShadowCaptureConfig(unsigned int baseAdd, unsigned int compareVal,
                                 unsigned int periodVal)
{
    HWREG(baseAdd + ECAP_CAP1) = periodVal; 
    HWREG(baseAdd + ECAP_CAP2) = compareVal; 
}

/**
 * \brief   This function configures the counter phase value.
 *
 * \param   baseAdd      It is the Memory address of the ECAP instance used.
 * \param   cntPhaseVal  It is the counter phase value to be programmed for
 *                       phase lag/lead.
 *
 * \return  None.
 *
 **/
void ECAPCounterPhaseValConfig(unsigned int baseAdd, unsigned int cntPhaseVal)
{
    HWREG(baseAdd + ECAP_CTRPHS) = cntPhaseVal;
}

/**
 * \brief   This function enables the generation of interrupts if any of
 *          event interrupt are enable and corresponding event interrupt 
 *          flag is set.
 *
 * \param   baseAdd   It is the Memory address of the ECAP instance used.
 *
 * \return  None.
 *
 **/
void ECAPGlobalIntEnable(unsigned int baseAdd)
{
    HWREGH(baseAdd + ECAP_ECCLR) |= ECAP_ECCLR_INT;
}

/**
 * \brief   This function enables the specified interrups
 *
 * \param   baseAdd   It is the Memory address of the ECAP instance used.
 * \param   flag      It is the value which specifies the interrupts to
 *                    be enabled.\n
 *
 *          flag can take following macros
 *
 *          ECAP_CEVT1_INT   - Enable Capture Event 1 interrupt.\n
 *          ECAP_CEVT2_INT   - Enable Capture Event 2 interrupt.\n
 *          ECAP_CEVT3_INT   - Enable Capture Event 3 interrupt.\n
 *          ECAP_CEVT4_INT   - Enable Capture Event 4 interrupt.\n
 *          ECAP_CNTOVF_INT  - Enable Counter Overflow interrupt.\n
 *          ECAP_PRDEQ_INT   - Enable Period equal interrupt.\n
 *          ECAP_CMPEQ_INT   - Enable Compare equal interrupt.\n  
 *
 * \return  None.
 *
 **/
void ECAPIntEnable(unsigned int baseAdd, unsigned int flag)
{
    HWREGH(baseAdd + ECAP_ECEINT) |= flag;
}

/**
 * \brief   This function disables the specified interrups
 *
 * \param   baseAdd   It is the Memory address of the ECAP instance used.
 * \param   flag      It is the value which specifies the interrupts to
 *                    be disabled.\n
 *
 *          flag can take following macros.
 *
 *          ECAP_CEVT1_INT   - Enable Capture Event 1 interrupt.\n
 *          ECAP_CEVT2_INT   - Enable Capture Event 2 interrupt.\n
 *          ECAP_CEVT3_INT   - Enable Capture Event 3 interrupt.\n
 *          ECAP_CEVT4_INT   - Enable Capture Event 4 interrupt.\n
 *          ECAP_CNTOVF_INT  - Enable Counter Overflow interrupt.\n
 *          ECAP_PRDEQ_INT   - Enable Period equal interrupt.\n
 *          ECAP_CMPEQ_INT   - Enable Compare equal interrupt.\n  
 *
 * \return  None.
 *
 **/
void ECAPIntDisable(unsigned int baseAdd, unsigned int flag)
{
    HWREGH(baseAdd + ECAP_ECEINT) |= flag;
}

/**
 * \brief   This function returns the status specified interrups
 *
 * \param   baseAdd   It is the Memory address of the ECAP instance used.
 * \param   flag      It is the value which specifies the status of interrupts
 *                    to be returned.\n
 *
 *          flag can take following macros.
 *
 *          ECAP_CEVT1_INT   - Status of Capture Event 1 interrupt.\n
 *          ECAP_CEVT2_INT   - Status of Capture Event 2 interrupt.\n
 *          ECAP_CEVT3_INT   - Status of Capture Event 3 interrupt.\n
 *          ECAP_CEVT4_INT   - Status of Capture Event 4 interrupt.\n
 *          ECAP_CNTOVF_INT  - Status of Counter Overflow interrupt.\n
 *          ECAP_PRDEQ_INT   - Status of Period equal interrupt.\n
 *          ECAP_CMPEQ_INT   - Status of Compare equal interrupt.\n  
 *          ECAP_GLOBAL_INT  - Global interrupt status.\n 
 *
 * \returns Status of the specified interrupts.
 *
 **/
unsigned int ECAPIntStatus(unsigned int baseAdd, unsigned int flag)
{
    return (HWREGH(baseAdd + ECAP_ECFLG) & flag);
}

/**
 * \brief   This function clears of the status specified interrups
 *
 * \param   baseAdd   It is the Memory address of the ECAP instance used.
 * \param   flag      It is the value which specifies the status of interrupts
 *                    to be cleared.\n
 *
 *          flag can take following macros.
 *
 *          ECAP_CEVT1_INT   - Status of Capture Event 1 interrupt.\n
 *          ECAP_CEVT2_INT   - Status of Capture Event 2 interrupt.\n
 *          ECAP_CEVT3_INT   - Status of Capture Event 3 interrupt.\n
 *          ECAP_CEVT4_INT   - Status of Capture Event 4 interrupt.\n
 *          ECAP_CNTOVF_INT  - Status of Counter Overflow interrupt.\n
 *          ECAP_PRDEQ_INT   - Status of Period equal interrupt.\n
 *          ECAP_CMPEQ_INT   - Status of Compare equal interrupt.\n  
 *
 * \return  None.
 *
 **/
void ECAPIntStatusClear(unsigned int baseAdd, unsigned int flag)
{
    HWREGH(baseAdd + ECAP_ECCLR) = HWREGH(baseAdd + ECAP_ECCLR) & flag;
}

/**
 * \brief   This function returns the peripheral ID
 *
 * \param   baseAdd   It is the Memory address of the ECAP instance used.
 *
 * \return  Peripheral ID.
 *
 **/
unsigned int ECAPPeripheralIdGet(unsigned int baseAdd)
{
    return(HWREG(baseAdd + ECAP_REVID));
}
