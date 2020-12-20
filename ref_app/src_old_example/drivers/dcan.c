/**
 *  \file   dcan.c
 *
 *  \brief  DCAN APIs.
 *
 *   This file contains the device abstraction layer APIs for 
 *   Dual Controller Area Network(DCAN).
 */

/* Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#include "hw_types.h"
#include "dcan.h"

/*******************************************************************************
*                       INTERNAL MACRO DEFINITIONS
*******************************************************************************/
#define BRP_MASK               (0x0000003Fu)
#define BRPE_MASK              (0x000003C0u)
#define TX_REQUEST_X_MASK      (0x0000FFFFu)
#define NEW_DATA_X_MASK        (0x0000FFFFu)
#define INT_PEND_X_MASK        (0x0000FFFFu)
#define MSG_VALID_X_MASK       (0x0000FFFFu)
#define BRPE_SHIFT             (16u)
#define DCAN_STD_ID_SHIFT      (18u)

/*******************************************************************************
*                        API FUNCTION DEFINITIONS
*******************************************************************************/
/**
 * \brief   This API will enable the DCAN peripheral in Initialization mode. 
 *
 * \param   baseAdd       Base Address of the DCAN Module Registers.
 *
 * \return  None.
 *
 * \note    By calling this API, communication on CAN bus is stopped and the
 *          CAN module enters initialization mode.
 *
 **/
void DCANInitModeSet(unsigned int baseAdd)
{
    /* Set the Init field of DCAN_CTL register */
    HWREG(baseAdd + DCAN_CTL) |= DCAN_CTL_INIT;

    /* Wait for Init bit to set */ 
    while(!(HWREG(baseAdd + DCAN_CTL) & DCAN_CTL_INIT));
}

/**
 * \brief   This API will enable the DCAN peripheral in Normal mode of 
 *          operation.
 *
 * \param   baseAdd       Base Address of the DCAN Module Registers.
 *
 * \return  None.
 *
 * \note    By calling this API, the DCAN module exits Initialization mode 
 *          and communication on CAN bus is started. 
 *
 **/
void DCANNormalModeSet(unsigned int baseAdd)
{
    /* Clear the CCE and Init bit */
    HWREG(baseAdd + DCAN_CTL) &= ~DCAN_CTL_INIT;

    /* Wait for Init bit to clear */
    while(HWREG(baseAdd + DCAN_CTL) & DCAN_CTL_INIT);
}

/**
 * \brief   This API will perform the bit timing configurations for DCAN.
 *
 * \param   baseAdd       Base Address of the DCAN Module Registers.
 * \param   clkFreq       Input Clk frequency for the DCAN module in MHz.
 * \param   tmQnta        Time quanta in nano seconds.
 * \param   bitTime       This variable will hold values of propagation delay,
 *                        sync jump width, time segment 2 value.
 *
 * 'bitTime' can take this value \n
 *    DCAN_BIT_TIME(propDel, jumpWidth, segmnt2) ->
 *         propDel - Propagation delay on CAN bus \n
 *         jumpWidth - Synchronization jump width on the bus \n
 *         segmnt2 - Segment2 time \n
 *
 * \return  None.
 *
 * \note   To configure CAN bit timing the DCAN peripheral should be in 
 *         initialization mode and CPU should have access to the DCAN 
 *         configuration registers. 
 *         8 MHz is the minimum CAN clock frequency required to operate 
 *         the DCAN at a bit rate of 1MBits/s.
 *         The parameters -
 *         clkFreq - Takes values in MHz \n
 *         tmQnta - Takes values in nanoseconds \n
 *         If the user wants CAN_CLK = 24Mhz and time quanta(tq) = 1000ns
 *         then clkFreq = 24 and tmQnta = 1000.
 *
 **/
void DCANBitTimingConfig(unsigned int baseAdd, unsigned int clkFreq, 
                         unsigned int tmQnta, unsigned int bitTime)
{
    unsigned int bitTimeConf;
    unsigned int preScaler;

    /* Calculate the prescaler value */
    preScaler = ((unsigned int)((tmQnta * clkFreq) / 1000) - 1);

    /* Pack the prescaler value and bitTime variable */
    bitTimeConf = (bitTime | (preScaler & BRP_MASK) | 
                             ((preScaler & BRPE_MASK) << BRPE_SHIFT));

    /* Write the value to the Bit timing register */
    HWREG(baseAdd + DCAN_BTR) = bitTimeConf;
}

/**
 * \brief   This API will reset the DCAN peripheral.
 *
 * \param   baseAdd       Base Address of the DCAN Module Registers.
 *
 * \return  None.
 *
 * \note    To perform software reset the DCAN peripheral should be enabled in
 *          initialization mode. 
 *
 **/
void DCANReset(unsigned int baseAdd)
{
    /* Set the SWR bit of DCAN_CTL register */
    HWREG(baseAdd + DCAN_CTL) |= DCAN_CTL_SWR;

    /* Poll in the loop until reset completes */
    while((HWREG(baseAdd + DCAN_CTL) & DCAN_CTL_SWR));
}

/**
 * \brief   This API will enable the Interrupts of DCAN peripheral.
 *
 * \param   baseAdd       Base Address of the DCAN Module Registers.
 * \param   intFlags      Enable Interrupts.
 *
 *    'intFlags' can take the following values \n
 *    DCAN_STATUS_CHANGE_INT - Enable Status change Interrupt \n
 *    DCAN_ERROR_INT - Enable error Interrupt \n
 *
 * \return  None.
 *
 **/
void DCANIntEnable(unsigned int baseAdd, unsigned int intFlags)
{
    /* Enable the DCAN interrupts */
    HWREG(baseAdd + DCAN_CTL) |= (intFlags & (DCAN_CTL_SIE | DCAN_CTL_EIE));
}

/**
 * \brief   This API will disable the Interrupts of DCAN peripheral.
 *
 * \param   baseAdd       Base Address of the DCAN Module Registers.
 * \param   intFlags      Disable Interrupts.
 *
 *    'intFlags' can take the following values \n
 *    DCAN_STATUS_CHANGE_INT - Disable Status change Interrupt \n
 *    DCAN_ERROR_INT - Disable error Interrupt \n
 *
 * \return  None.
 *
 **/
void DCANIntDisable(unsigned int baseAdd, unsigned int intFlags)
{
    /* Disable the DCAN interrupts */
    HWREG(baseAdd + DCAN_CTL) &= ~(intFlags & (DCAN_CTL_SIE | DCAN_CTL_EIE));
}

/**
 * \brief   This API will enable/disable the auto re-transmission of 
 *          unsuccessful messages of DCAN peripheral.
 *
 * \param   baseAdd       Base Address of the DCAN Module Registers.
 * \param   autoReTxn     Enable/disable setting for Auto re-transmision.
 *
 *    'autoReTxn' can take the following values \n
 *    DCAN_AUTO_RETXN_ENABLE - Enable auto re-transmission \n
 *    DCAN_AUTO_RETXN_DISABLE - Disable auto re-transmission \n
 *
 * \return  None.
 *
 **/
void DCANAutoReTransmitControl(unsigned int baseAdd, unsigned int autoReTxn)
{
    /* Clear the DAR field of DCAN_CTL register */
    HWREG(baseAdd + DCAN_CTL) &= ~DCAN_CTL_DAR;

    /* Set the user sent value to DAR field */
    HWREG(baseAdd + DCAN_CTL) |= (autoReTxn & DCAN_CTL_DAR);
}

/**
 * \brief   This API will enable/disable the CPU write access to the 
 *          configuration  registers of DCAN peripheral.
 *
 * \param   baseAdd       Base Address of the DCAN Module Registers.
 * \param   regConfig     Enable/disable write access to configuration 
 *                        registers.
 *
 *    'regConfig' can take the following values \n
 *    DCAN_CONF_REG_WR_ACCESS_ENABLE - Enable write access \n
 *    DCAN_CONF_REG_WR_ACCESS_DISABLE - Disable write access \n
 *
 * \return  None.
 *
 * \note    If the user wants to enable write access to configuration registers,
 *          then DCAN peripheral should be in initialization mode before doing 
 *          so.
 *
 **/
void DCANConfigRegWriteAccessControl(unsigned int baseAdd, 
                                     unsigned int regConfig)
{
    /* Clear the CCE field of DCAN_CTL register */
    HWREG(baseAdd + DCAN_CTL) &= ~DCAN_CTL_CCE;

    /* Set the CCE field with the user sent value */
    HWREG(baseAdd + DCAN_CTL) |= (regConfig & DCAN_CTL_CCE);
}

/**
 * \brief   This API will enable/disable the test mode of DCAN peripheral.
 *
 * \param   baseAdd       Base Address of the DCAN Module Registers.
 * \param   testMode      Enable/disable test mode.
 *
 *    'testMode' can take the following values \n
 *    DCAN_TEST_MODE_ENABLE - Enable test mode of DCAN peripheral \n
 *    DCAN_TEST_MODE_DISABLE - Disable test mode and enable normal mode 
 *                             of DCAN peripheral \n
 *
 * \return  None.
 *
 **/
void DCANTestModeControl(unsigned int baseAdd, unsigned int testMode)
{
    /* Clear the Test field of DCAN_CTL register */
    HWREG(baseAdd + DCAN_CTL) &= ~DCAN_CTL_TEST;    

    /* Set the Test field with the user sent value */
    HWREG(baseAdd + DCAN_CTL) |= (testMode & DCAN_CTL_TEST);
}

/**
 * \brief   This API will configure the required setting for debug/suspend 
 *          support feature of DCAN peripheral.
 *
 * \param   baseAdd       Base Address of the DCAN Module Registers.
 * \param   modeConfig    Debug support setting.
 *
 *    'modeConfig' can take the following values \n
 *    DCAN_DEBUG_SUSPEND_WAIT - When debug/suspend mode is requested, DCAN 
 *                              will wait for a started transmission or 
 *                              reception to be completed before entering 
 *                              debug/suspend mode.
 *
 *    DCAN_DEBUG_SUSPEND_INTERRUPT - When debug/suspend mode is requested, DCAN
 *                                   will interrupt any transmission or reception 
 *                                   and enter debug/suspend mode immediately.
 *
 * \return  None.
 *
 **/
void DCANDebugSuspendModeConfig(unsigned int baseAdd, unsigned int modeConfig)
{
    /* Clear the IDS field of DCAN_CTL register */
    HWREG(baseAdd + DCAN_CTL) &= ~DCAN_CTL_IDS;

    /* Set the IDS field with the user sent value */
    HWREG(baseAdd + DCAN_CTL) |= (modeConfig & DCAN_CTL_IDS);
}

/**
 * \brief   This API will enable/disable the Auto-bus-on feature of DCAN 
 *          peripheral.
 *
 * \param   baseAdd       Base Address of the DCAN Module Registers.
 * \param   busControl    Auto-bus-on mode setting.
 *
 *    'busControl' can take the following values \n
 *    DCAN_AUTO_BUS_ON_ENABLE - Enable Auto-bus-on feature of DCAN peripheral \n 
 *    DCAN_AUTO_BUS_ON_DISABLE - Disable Auto-bus-on feature of DCAN 
 *                               peripheral \n 
 *
 * \return  None.
 *
 **/
void DCANAutoBusOnControl(unsigned int baseAdd, unsigned int busControl)
{
    /* Clear the ABO field of DCAN_CTL register */
    HWREG(baseAdd + DCAN_CTL) &= ~DCAN_CTL_ABO;

    /* Set the ABO field with the user sent value */
    HWREG(baseAdd + DCAN_CTL) |= (busControl & DCAN_CTL_ABO);
}

/**
 * \brief   This API will enable/disable the parity function of DCAN
 *          peripheral.
 *
 * \param   baseAdd       Base Address of the DCAN Module Registers.
 * \param   paritySet     Configure parity.
 *
 *    'paritySet' can take the following values \n
 *    DCAN_PARITY_FUNC_ENABLE - Enable parity function of DCAN peripheral \n
 *    DCAN_PARITY_FUNC_DISABLE - Disable parity function of DCAN peripheral \n
 *
 * \return  None.
 *
 **/
void DCANParityControl(unsigned int baseAdd, unsigned int paritySet)
{
    /* Clear the PMD field of DCAN_CTL register */
    HWREG(baseAdd + DCAN_CTL) &= ~DCAN_CTL_PMD;

    /* Set the PMD field with the user sent value */
    HWREG(baseAdd + DCAN_CTL) |= (paritySet & DCAN_CTL_PMD);
}

/**
 * \brief   This API will check the status of debug state and return the status
 *          to the caller. 
 *
 * \param   baseAdd       Base Address of the DCAN Module Registers.
 *
 * \return  This API returns the status of the DCAN debug state.
 *
 *          The user can use these macros for checking the status \n
 *          DCAN_NOT_IN_DEBUG_MODE - DCAN is not in debug mode \n
 *          DCAN_IN_DEBUG_MODE - DCAN is in debug mode \n 
 *
 **/
unsigned int DCANInternalDebugStatusGet(unsigned int baseAdd)
{
    /* Return the debug state of DCAN peripheral */
    return(HWREG(baseAdd + DCAN_CTL) & DCAN_CTL_INITDBG);
}

/**
 * \brief   This API will enable Interrupt lines of DCAN peripheral.
 *
 * \param   baseAdd       Base Address of the DCAN Module Registers.
 * \param   enableLine    Enable Interrupt lines.
 *
 *    'enableLine' can take the following values \n
 *    DCAN_INT_LINE0 - Enable Interrupt line 0 \n
 *    DCAN_INT_LINE1 - Enable Interrupt line 1 \n
 * 
 * \return  None.
 *
 * \note    The Error and Status change interrupts can only be routed to 
 *          DCANINT0 line and message object interrupts can be routed to 
 *          both lines. Hence DCANINT0 line has to be enabled if error and 
 *          status change interrupts have to be serviced.
 *
 **/
void DCANIntLineEnable(unsigned int baseAdd, unsigned int enableLine)
{
    /* Enable the interrupt lines */
    HWREG(baseAdd + DCAN_CTL) |= (enableLine & (DCAN_CTL_IE1 | DCAN_CTL_IE0));
}

/**
 * \brief   This API will disable the Interrupt lines of DCAN peripheral.
 *
 * \param   baseAdd       Base Address of the DCAN Module Registers.
 * \param   disableLine   Disable Interrupt lines.
 *
 *    'disableLine' can take the following values \n
 *    DCAN_INT_LINE0 - Disable interrupt line 0 \n
 *    DCAN_INT_LINE1 - Disable interrupt line 1\n
 *
 * \return  None.
 *
 **/
void DCANIntLineDisable(unsigned int baseAdd, unsigned int disableLine)
{
    /* Disable the interrupt lines */
    HWREG(baseAdd + DCAN_CTL) &= ~(disableLine & (DCAN_CTL_IE1 | DCAN_CTL_IE0));
}

/**
 * \brief   This API will enable the DMA request line for Interface 
 *          registers(IF[1:3]) of the DCAN peripheral.
 *
 * \param   baseAdd       Base Address of the DCAN Module Registers.
 * \param   ifRegFlags    Enable DMA request lines.
 *
 *    'ifRegFlags' can take the following values \n
 *    DCAN_DMA_REQUEST_LINE_IF1 - Enable DMA request line for IF1 \n
 *    DCAN_DMA_REQUEST_LINE_IF2 - Enable DMA request line for IF2 \n
 *    DCAN_DMA_REQUEST_LINE_IF3 - Enable DMA request line for IF3 \n
 *
 * \return  None.
 *
 **/
void DCANDmaRequestLineEnable(unsigned int baseAdd, unsigned int ifRegFlags)
{
    /* Enable the DMA request lines */
    HWREG(baseAdd + DCAN_CTL) |= (ifRegFlags & 
                                 (DCAN_CTL_DE1 | DCAN_CTL_DE2 | DCAN_CTL_DE3)); 
}

/**
 * \brief   This API will disable the DMA request line for Interface
 *          registers(IF[1:3]) of the DCAN peripheral.
 *
 * \param   baseAdd       Base Address of the DCAN Module Registers.
 * \param   ifRegFlags    Disable DMA request lines.
 *
 *    'ifRegFlags' can take the following values \n
 *    DCAN_DMA_REQUEST_LINE_IF1 - Disable DMA request line for IF1 \n
 *    DCAN_DMA_REQUEST_LINE_IF2 - Disable DMA request line for IF2 \n
 *    DCAN_DMA_REQUEST_LINE_IF3 - Disable DMA request line for IF3 \n
 *
 * \return  None.
 *
 **/
void DCANDmaRequestLineDisable(unsigned int baseAdd, unsigned int ifRegFlags)
{
    /* Disable the DMA request lines */
    HWREG(baseAdd + DCAN_CTL) &= ~(ifRegFlags & 
                                  (DCAN_CTL_DE1 | DCAN_CTL_DE2 | DCAN_CTL_DE3));
}

/**
 * \brief   This API will control the setting for local power down mode.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   pwrDwnCtl           Power down mode setting.
 *
 *    'pwrDwnCtl' can take the following values \n
 *    DCAN_LOCAL_PWR_DWN_OFF - Local power down mode is off \n
 *    DCAN_LOCAL_PWR_DWN_ON - Local power down mode is on \n 
 *
 * \return  None.
 *
 * \note    DCAN on entering Local power down mode will wait until a bus idle 
 *          state is recognized and after this it enters the Initialization mode 
 *          by setting the Init bit in DCAN_CTL register and it also sets PDA 
 *          bit in the DCAN_ES register.
 *          When in local power down mode, the application should not clear the
 *          Init bit while PDR is set. If there are any messages in the message 
 *          RAM which are configured as transmit messages and the application 
 *          resets the Init bit then these messages may be sent.
 *
 **/
void DCANPwrDwnControl(unsigned int baseAdd, unsigned int pwrDwnCtl)
{
    /* Clear the PDR field of DCAN_CTL register */
    HWREG(baseAdd + DCAN_CTL) &= ~DCAN_CTL_PDR;

    /* Set the PDR field with the user sent value */
    HWREG(baseAdd + DCAN_CTL) |= (pwrDwnCtl & DCAN_CTL_PDR);
}

/**
 * \brief   This API will control the setting for wake up from local power 
 *          down mode.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   wkUpCtl             Wake up from power down setting.
 *
 *    'wkUpCtl' can take the following values \n
 *    DCAN_WKUP_DETECTION_DISABLED - Local power down mode is disabled \n
 *    DCAN_WKUP_DETECTION_ENABLED - Local power down mode is enabled \n
 *
 * \return  None.
 *
 * \note    To manually wakeup DCAN peripheral from power down mode the 
 *          application should manually clear the PDR bit by calling the API
 *          'DCANPwrDwnControl' by passing DCAN_LOCAL_PWR_DWN_OFF as the 
 *          arguement for 'pwrDwnCtl'. Once this is done the Init bit should 
 *          be cleared by calling the API 'DCANNormalModeSet'.
 *          But if wake up detection is enabled by passing the macro 
 *          DCAN_WKUP_DETECTION_ENABLED to 'wkUpCtl' then user need not clear
 *          the PDR and Init bit manually since it will be done by the DCAN 
 *          controller.
 *          The first CAN message, which initiates the bus activity, cannot be 
 *          received. This means that the first message received in power-down 
 *          and automatic wake-up mode is lost.
 *
 **/
void DCANPwrDwnWkUpControl(unsigned int baseAdd, unsigned int wkUpCtl)
{
    /* Clear the WUBA field of DCAN_CTL register */
    HWREG(baseAdd + DCAN_CTL) &= ~DCAN_CTL_WUBA;

    /* Set the WUBA field with the user sent value */
    HWREG(baseAdd + DCAN_CTL) |= (wkUpCtl & DCAN_CTL_WUBA);
}

/**
 * \brief   This API will return the status of Interrupt register DCAN_INT.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   intLnFlag           Int line no whose status is to be returned.
 *
 *    'intLnFlag' can take the following values \n
 *    DCAN_INT_LINE0_STAT - Status of Interrupt line 0 \n
 *    DCAN_INT_LINE1_STAT - Status of Interrupt line 1 \n
 *
 * \return  Returns the status of DCAN_INT register.
 *
 **/
unsigned int DCANIntRegStatusGet(unsigned int baseAdd, unsigned int intLnFlag)
{
    /* Return the status of DCAN_INT register */
    return(HWREG(baseAdd + DCAN_INT) & intLnFlag);
}

/**
 * \brief   This API will return the status of Error and status DCAN_ES register.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 *
 * \return  Returns the status of DCAN_ES register.
 *
 *          The user can use these macros for handling the status of DCAN_ES
 *          register in the application code.
 *
 *          DCAN_LST_ERRCODE_NO_ERR - No error in LEC field \n
 *          DCAN_LST_ERRCODE_STUFF_ERR - Stuff error is detected in LEC field \n
 *          DCAN_LST_ERRCODE_FORM_ERR - Form error detected in LEC field \n
 *          DCAN_LST_ERRCODE_ACK_ERR - Ack error detected in LEC field \n
 *          DCAN_LST_ERRCODE_BIT1_ERR - Bit1 error detected in LEC field \n
 *          DCAN_LST_ERRCODE_BIT0_ERR - Bit0 error detected in LEC field \n
 *          DCAN_LST_ERRCODE_CRC_ERR - CRC error detected in LEC field \n
 *          DCAN_NO_EVENT_ON_CAN_BUS - No event generated on CAN bus since 
 *                                     last read of DCAN_ES register \n
 *          DCAN_TXD_MSG_SUCCESSFULLY - DCAN transmitted message successfully \n
 *          DCAN_RXD_MSG_SUCCESSFULLY - DCAN received message successfully \n
 *          DCAN_CORE_IN_ERR_PASSIVE - DCAN core in error passive state \n
 *          DCAN_ERR_WARN_STATE_RCHD - Atleast one of the counters have reached 
 *                                     error warning limit \n
 *          DCAN_MOD_IN_BUS_OFF_STATE - DCAN in bus off state \n
 *          DCAN_PARITY_ERR_DETECTED - Parity error detected \n
 *          DCAN_INITIATED_SYSTEM_WKUP - DCAN initiated system wakeup \n
 *          DCAN_IN_LOCAL_PWR_DWN_MODE - DCAN in local power down mode \n
 *
 * \note    Reading the error and status register will clear/set certain bits 
 *          in the error and status register. For more information please refer 
 *          the DCAN Technical Reference Manual(TRM). 
 *          For debug support, the auto clear functionality of error and status 
 *          register is disabled when in debug/suspend mode.
 *
 **/
unsigned int DCANErrAndStatusRegInfoGet(unsigned int baseAdd)
{
    /* Return the status of DCAN_ES register to the caller */
    return(HWREG(baseAdd + DCAN_ES));
}

/**
 * \brief   This API will indicate end of interrupt(EOI) for parity error.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 *
 * \return  None. 
 *
 **/
void DCANParityEndOfIntSet(unsigned int baseAdd)
{
    /* Set the PARITYERR_EOI field of DCAN_ES register */
    HWREG(baseAdd + DCAN_PARITYERR_EOI) |= DCAN_PARITYERR_EOI_PARITYERR_EOI;
}

/**
 * \brief   This API will return the status of Error counter register.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   cntrFlags           Status to be returned from the API.
 *
 *  'cntrFlags' can take the following values \n
 *   DCAN_TX_ERR_CNTR - Return the TEC[7:0] field of DCAN_ERRC register \n
 *   DCAN_RX_ERR_CNTR - Return the REC[6:0] field of DCAN_ERRC register \n
 *   DCAN_RX_ERR_PASSIVE - Return the receiver error passive status \n
 *
 * \return  This API returns the status of DCAN_ERRC register.
 *
 **/
unsigned int DCANErrCntrRegStatusGet(unsigned int baseAdd, 
                                     unsigned int cntrFlags)
{
    /* Return the status of DCAN_ERRC register to the caller */
    return(HWREG(baseAdd + DCAN_ERRC) & cntrFlags);
}

/**
 * \brief   This API will enable the test modes of DCAN peripheral.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   tstMode             Enable test mode.
 *
 *  'tstMode' can take the following values \n
 *   DCAN_TST_SILENT_MD - Enable silent mode \n
 *   DCAN_TST_LPBCK_MD - Enable loopback mode \n
 *   DCAN_TST_EXTLPBCK_MD - Enable external loopback mode \n
 *   DCAN_TST_LPBCK_SILENT_MD - Enable loopback with silent mode \n
 *   DCAN_TST_RAM_DIRECT_ACCESS - Direct access to RAM enabled \n
 *
 * \return  None.
 *
 * \note    Before calling the API 'DCANTestModesEnable' ensure to enable DCAN 
 *          in test mode by using the API 'DCANTestModeControl'.
 *          When internal loop-back mode is active, if external loopback is 
 *          enabled then external loopback will be ignored. 
 *
 **/
void DCANTestModesEnable(unsigned int baseAdd, unsigned int tstMode)
{
    /* Set test mode fields of DCAN_TEST register with the user sent value */
    HWREG(baseAdd + DCAN_TEST) |= (tstMode & 
                                  (DCAN_TEST_RDA | DCAN_TEST_EXL | 
                                   DCAN_TEST_LBACK | DCAN_TEST_SILENT));
}

/**
 * \brief   This API will disable the test modes of DCAN peripheral.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   tstMode             Disable test mode.
 *
 *  'tstMode' can take the following values \n
 *   DCAN_TST_SILENT_MD - Disable silent mode \n
 *   DCAN_TST_LPBCK_MD - Disable loopback mode \n
 *   DCAN_TST_EXTLPBCK_MD - Disable external loopback mode \n
 *   DCAN_TST_LPBCK_SILENT_MD - Disable loopback with silent mode \n
 *   DCAN_TST_RAM_DIRECT_ACCESS - Direct access to RAM disabled
 *                                (normal operation enabled) \n
 *
 * \return  None.
 *
 * \note    Usage of this API is only valid if DCAN is enabled in test mode.
 *          DCAN can be enabled in test mode by using the API 
 *          'DCANTestModeControl'.
 *
 **/
void DCANTestModesDisable(unsigned int baseAdd, unsigned int tstMode)
{
    /* Clear the mode fields of DCAN_TEST register with the user sent value */
    HWREG(baseAdd + DCAN_TEST) &= ~(tstMode &
                                   (DCAN_TEST_RDA | DCAN_TEST_EXL | 
                                    DCAN_TEST_LBACK | DCAN_TEST_SILENT));
}

/**
 * \brief   This API will configure the control for CAN_TX pin.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   pinCtl              Pin control used for CAN_TX.
 *
 *  'pinCtl' can take the following values \n
 *   DCAN_TST_TX_NRML_OP - Normal operation of CAN_TX pin \n
 *   DCAN_TST_TX_SAMPLE_PT_MNTR - Sample point can be monitored at CAN_TX pin \n
 *   DCAN_TST_TX_DRIV_DOM_VAL - CAN_TX pin drives a dominant value \n
 *   DCAN_TST_TX_DRIV_RSV_VAL - CAN_TX pin drives a recessive value \n
 *
 * \return  None.
 *
 * \note    Usage of this API is only valid if DCAN is enabled in test mode.
 *          DCAN can be enabled in test mode by using the API
 *          'DCANTestModeControl'.
 *          Setting for CAN_TX pin other than DCAN_TST_TX_NRML_OP will disturb
 *          message transfer.
 *
 **/
void DCANTxPinControl(unsigned int baseAdd, unsigned int pinCtl)
{
    /* Clear the TX[1:0] field of DCAN_TEST register */
    HWREG(baseAdd + DCAN_TEST) &= ~DCAN_TEST_TX;

    /* Set the TX[1:0] field with the user sent value */
    HWREG(baseAdd + DCAN_TEST) |= (pinCtl & DCAN_TEST_TX);
}

/**
 * \brief   This API will return the status of CAN_RX pin.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 *
 * \return  Returns the status of CAN_RX pin.
 *
 *          User can use the below macros to check the status \n
 *          DCAN_TST_RX_IS_DOM - CAN bus is dominant \n
 *          DCAN_TST_RX_IS_RSV - CAN bus is recessive \n
 *
 * \note    Usage of this API is only valid if DCAN is enabled in test mode.
 *          DCAN can be enabled in test mode by using the API
 *          'DCANTestModeControl'.
 *
 **/
unsigned int DCANRxPinStatusGet(unsigned int baseAdd)
{
    /* Return the status of CAN_RX pin to the caller */
    return(HWREG(baseAdd + DCAN_TEST) & DCAN_TEST_RX);
}

/**
 * \brief   This API will return the status of parity error code register
 *          DCAN_PERR.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   statFlg             Status of word/message number to be returned
 *                              from API.
 *
 * \return  Returns the status of DCAN_PERR register.
 *
 *          User can use the below macros to check the status \n
 *          DCAN_PARITY_ERR_MSG_NUM - Message object number where parity 
 *                                    error is detected \n
 *          DCAN_PARITY_ERR_WRD_NUM - Word number where parity error is 
 *                                    detected \n
 *
 **/
unsigned int DCANParityErrCdRegStatusGet(unsigned int baseAdd, 
                                         unsigned int statFlg)
{
    /* Return the status of Parity error code register */
    return(HWREG(baseAdd + DCAN_PERR) & statFlg); 
}

/**
 * \brief   This API will set the Auto-bus on timer value to the DCAN_ABOTR
 *          register.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   timeVal             Auto-bus on timer value.
 *
 * \return  None.
 *
 * \note    This API is valid only if the auto-bus-on feature is enabled using
 *          'DCANAutoBusOnControl' API.
 *          On write access to the CAN control register while auto-bus-on timer
 *          is running, the auto-bus-on procedure will be aborted.
 *          During debug/suspend mode, running Auto-bus-on timer will be paused.
 *
 **/
void DCANAutoBusOnTimeValSet(unsigned int baseAdd, unsigned int timeVal)
{
    /* Set the user sent value to DCAN_ABOTR register */
    HWREG(baseAdd + DCAN_ABOTR) = timeVal;
}

/**
 * \brief   This API will return the auto-bus-on time register value.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 *
 * \return  Returns the auto-bus-on timer value.
 *
 * \note    This API is valid only if the auto-bus-on feature is enabled using
 *          'DCANAutoBusOnControl' API.
 *          On write access to the CAN control register while auto-bus-on timer
 *          is running, the auto-bus-on procedure will be aborted.
 *          During debug/suspend mode, running Auto-bus-on timer will be paused.
 *
 **/
unsigned int DCANAutoBusOnTimeValGet(unsigned int baseAdd)
{
    /* Return the Auto-bus-on timer value to the caller */
    return(HWREG(baseAdd + DCAN_ABOTR));
}

/**
 * \brief   This API will return the status from DCAN_TXRQ_X register.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 *
 * \return  Returns the status from the transmit request X register.
 *          User can use the below macro to check the status \n
 *          DCAN_TXRQST_X_REG(n) - Status of DCAN_TXRQ_X register
 *                                 corresponding to n \n
 *          where 1 <= n <= 8 \n
 *
 **/
unsigned int DCANTxRqstXStatusGet(unsigned int baseAdd)
{
    /* Return the status from DCAN_TXRQ_X register */
    return(HWREG(baseAdd + DCAN_TXRQ_X) & TX_REQUEST_X_MASK);
}

/**
 * \brief   This API will return the status from DCAN_TXRQ(n) register.
 *          Where n = 12,34,56,78.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   msgNum              Message object number whose TxRqst status is to
 *                              be returned.
 *
 * \return  Returns the transmit request status from DCAN_TXRQ(n) register.
 *          Where n = 12,34,56,78.
 *
 * \note    Values for 'msgNum' can range from \n
 *          1 <= msgNum <= 128.
 *
 **/
unsigned int DCANTxRqstStatusGet(unsigned int baseAdd, unsigned int msgNum)
{
    unsigned int regNum;
    unsigned int offSet;

    regNum = (msgNum - 1) / 32;
    offSet = (msgNum - 1) % 32;

    /* Return the status from DCAN_TXRQ register */
    return(HWREG(baseAdd + DCAN_TXRQ(regNum)) & (1 << offSet));
}

/**
 * \brief   This API will return the lowest message object number whose transmit 
 *          request status is not set from DCAN_TXRQ(n) register.
 *          Where n = 12,34,56,78.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 *
 * \return  Returns the transmit request status from DCAN_TXRQ(n) register.
 *          Where n = 12,34,56,78.
 *
 * \note    This API is similar to 'DCANTxRqstStatusGet'. Only difference is
 *          that the user need not send the message number to read the TxRqst 
 *          status of that message number. This API will return the lowest 
 *          message object number whose TxRqst status is not set.
 *
 **/
unsigned int DCANTxRqstStatGet(unsigned int baseAdd)
{
    unsigned int index = 1;
    unsigned int regNum;
    unsigned int offSet;

    while(index < 128)
    {
        regNum = (index - 1) / 32;
        offSet = (index - 1) % 32;

        if(!(HWREG(baseAdd + DCAN_TXRQ(regNum)) & (1 << offSet)))
        {
            break;
        }
        index++;
    }

    return(index);
}

/**
 * \brief   This API will return the status from DCAN_NWDAT_X register.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 *
 * \return  Returns the status from DCAN_NWDAT_X register.
 *          User can use the below macro to check the status \n
 *          DCAN_NEWDAT_X_REG(n) - Status of DCAN_NWDAT_X register
 *                                 corresponding to n \n
 *          where 1 <= n <= 8 \n
 *
 **/
unsigned int DCANNewDataXStatusGet(unsigned int baseAdd)
{
    /* Return the status from DCAN_NWDAT_X register */
    return(HWREG(baseAdd + DCAN_NWDAT_X) & NEW_DATA_X_MASK); 
}

/**
 * \brief   This API will return the status from DCAN_NWDAT(n) register.
 *          Where n = 12,34,56,78.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   msgNum              Message object number whose NewDat status is 
 *                              to be returned.
 *
 * \return  Returns the New data status from DCAN_NWDAT(n) register.
 *          Where n = 12,34,56,78.
 *
 * \note    Values for 'msgNum' can range from \n
 *          1 <= msgNum <= 128.
 *
 **/
unsigned int DCANNewDataStatusGet(unsigned int baseAdd, unsigned int msgNum)
{
    unsigned int regNum;
    unsigned int offSet;

    regNum = (msgNum - 1) / 32;
    offSet = (msgNum - 1) % 32;

    /* Return the status from DCAN_NWDAT register */
    return(HWREG(baseAdd + DCAN_NWDAT(regNum)) & (1 << offSet));
}

/**
 * \brief   This API will return the lowest message object number whose new 
 *          data status is set from DCAN_NWDAT(n) register.
 *          Where n = 12,34,56,78.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 *
 * \return  Returns the new data status from DCAN_NWDAT(n) register.
 *          Where n = 12,34,56,78.
 *
 * \note    This API is similar to 'DCANNewDataStatusGet'. Only difference is
 *          that the user need not send the message number to read the NewData
 *          status of that message number. This API will return the lowest
 *          message object number whose NewData status is set.
 *
 **/
unsigned int DCANNewDataStatGet(unsigned int baseAdd)
{
    unsigned int index = 1;
    unsigned int regNum;
    unsigned int offSet;

    while(index < 128)
    {
        regNum = (index - 1) / 32;
        offSet = (index - 1) % 32;

        if((HWREG(baseAdd + DCAN_NWDAT(regNum)) & (1 << offSet)))
        {
            break; 
        }
        index++;
    }

    return(index);
}

/**
 * \brief   This API will return the status from DCAN_INTPND_X register.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 *
 * \return  Returns the status from DCAN_INTPND_X register.
 *          User can use the below macro to check the status \n
 *          DCAN_INTPND_X_REG(n) - Status of DCAN_INTPND_X register
 *                                 corresponding to n \n
 *          where 1 <= n <= 8 \n
 *
 **/
unsigned int DCANIntPendingXStatusGet(unsigned int baseAdd)
{
    /* Return the status from DCAN_INTPND_X register */
    return(HWREG(baseAdd + DCAN_INTPND_X) & INT_PEND_X_MASK);
}

/**
 * \brief   This API will return the status from DCAN_INTPND(n) register.
 *          Where n = 12,34,56,78.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   msgNum              Message object number whose IntPnd status is to
 *                              be returned.
 *
 * \return  Returns the Interrupt pending status from DCAN_INTPND(n) register.
 *          Where n = 12,34,56,78.
 *
 * \note    Values for 'msgNum' can range from \n
 *          1 <= msgNum <= 128.
 *
 **/
unsigned int DCANIntPendingStatusGet(unsigned int baseAdd, unsigned int msgNum)
{
    unsigned int regNum;
    unsigned int offSet;

    regNum = (msgNum - 1) / 32;
    offSet = (msgNum - 1) % 32;

    /* Return the status from DCAN_INTPND register */
    return(HWREG(baseAdd + DCAN_INTPND(regNum)) & (1 << offSet));
}

/**
 * \brief   This API will return the status from DCAN_MSGVAL_X register.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 *
 * \return  Returns the status from DCAN_MSGVAL_X register.
 *          User can use the below macro to check the status \n
 *          DCAN_MSGVAL_X_REG(n) - Status of DCAN_MSGVAL_X register
 *                                 corresponding to n \n
 *          where 1 <= n <= 8 \n
 *
 **/
unsigned int DCANMsgValidXStatusGet(unsigned int baseAdd)
{
    /* Return the status from DCAN_MSGVAL_X register */
    return(HWREG(baseAdd + DCAN_MSGVAL_X) & MSG_VALID_X_MASK);
}

/**
 * \brief   This API will return the status from DCAN_MSGVAL(n) register.
 *          Where n = 12,34,56,78.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   msgNum              Message object number whose MsgVal status is to
 *                              be returned.
 *
 * \return  Returns the Message valid status from DCAN_MSGVAL(n) register.
 *          Where n = 12,34,56,78.
 *
 * \note    Values for 'msgNum' can take the following values \n
 *          1 <= msgNum <= 128.
 *
 **/
unsigned int DCANMsgValidStatusGet(unsigned int baseAdd, unsigned int msgNum)
{
    unsigned int regNum;
    unsigned int offSet;

    regNum = (msgNum - 1) / 32;
    offSet = (msgNum - 1) % 32;

    /* Return the status from DCAN_MSGVAL register */
    return(HWREG(baseAdd + DCAN_MSGVAL(regNum)) & (1 << offSet));
}

/**
 * \brief   This API will configure which Interrupt line will be used to 
 *          service interrupts from message objects.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   intLine             Interrupt line to be configured.
 * \param   msgNum              Message object number whose IntMux is to be 
 *                              configured.
 *
 * 'intLine' can take the following values \n
 *    DCAN_INT0_ACTIVE - DCANINT0 line is active if corresponding 
 *                       IntPnd flag is set \n
 *    DCAN_INT1_ACTIVE - DCANINT1 line is active if corresponding 
 *                       IntPnd flag is one \n
 *
 * \return  None.
 *
 * \note    Values for 'msgNum' can range from \n
 *          1 <= msgNum <= 128.
 *
 **/
void DCANIntMuxConfig(unsigned int baseAdd, unsigned int intLine,
                      unsigned int msgNum)
{
    unsigned int regNum;
    unsigned int offSet;

    regNum = (msgNum - 1) / 32;
    offSet = (msgNum - 1) % 32;

    /* Clear the IntMux field of DCAN_INTMUX register corresponding to msgNum */
    HWREG(baseAdd + DCAN_INTMUX(regNum)) &= ~(1 << offSet);

    /* Set the DCAN_INTMUX field corresponding to msgNum */
    HWREG(baseAdd + DCAN_INTMUX(regNum)) |= (msgNum << offSet);
}

/**
 * \brief   This API will validate a message object. 
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   regNum              Interface register number used.
 *
 * 'regNum' can take the following values \n
 *    DCAN_IF1_REG - IF register 1 is used \n
 *    DCAN_IF2_REG - IF register 2 is used \n
 *
 * \return  None. 
 *
 **/
void DCANMsgObjValidate(unsigned int baseAdd, unsigned int regNum)
{
    /* Wait in loop until busy bit is cleared */
    while(DCANIFBusyStatusGet(baseAdd, regNum));

    /* Validate the message object */
    HWREG(baseAdd + DCAN_IFARB(regNum)) |= DCAN_IFARB_MSGVAL;
}

/**
 * \brief   This API will invalidate a message object.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   regNum              Interface register number used.
 *
 * 'regNum' can take the following values \n
 *    DCAN_IF1_REG - IF register 1 is used \n
 *    DCAN_IF2_REG - IF register 2 is used \n
 *
 * \return  None.
 *
 **/
void DCANMsgObjInvalidate(unsigned int baseAdd, unsigned int regNum)
{
    /* Wait in loop until busy bit is cleared */
    while(DCANIFBusyStatusGet(baseAdd, regNum));

    /* Invalidate the message object */
    HWREG(baseAdd + DCAN_IFARB(regNum)) &= ~DCAN_IFARB_MSGVAL;
}

/**
 * \brief   This API will set the fields of DCAN_IFCMD register.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   cmdFlags            Fields of the DCAN_IFCMD register which are 
 *                              to be set.
 * \param   objNum              Message object number to be configured.
 * \param   regNum              Interface register number used.
 *
 * 'cmdFlags' can take the following values \n
 *    DCAN_DMA_ACTIVE - Enable DMA feature \n
 *    DCAN_DAT_A_ACCESS - Access data from IF DataA register \n
 *    DCAN_DAT_B_ACCESS - Access data from IF DataB register \n
 *    DCAN_TXRQST_ACCESS - Access the TxRqst bit \n
 *    DCAN_CLR_INTPND - Clear the IntPnd bit \n
 *    DCAN_ACCESS_CTL_BITS - Access control bits \n
 *    DCAN_ACCESS_ARB_BITS - Access Arbitration bits \n
 *    DCAN_ACCESS_MSK_BITS - Access the mask bits \n
 *    DCAN_MSG_WRITE - Transfer direction is from IF registers to 
 *                     message RAM \n
 *    DCAN_MSG_READ - Transfer direction is from message RAM to 
 *                    IF registers \n
 *
 * 'regNum' can take the following values \n
 *    DCAN_IF1_REG - IF register 1 is used \n
 *    DCAN_IF2_REG - IF register 2 is used \n
 *
 * \return  None.
 *
 **/
void DCANCommandRegSet(unsigned int baseAdd, unsigned int cmdFlags,
                       unsigned int objNum, unsigned int regNum)
{
    /* Wait in loop until busy bit is cleared */
    while(DCANIFBusyStatusGet(baseAdd, regNum));

    /* Clear the DCAN_IFCMD register fields */
    HWREG(baseAdd + DCAN_IFCMD(regNum)) &= ~(DCAN_IFCMD_DMAACTIVE | 
                                             DCAN_IFCMD_DATAA | 
                                             DCAN_IFCMD_DATAB | 
                                             DCAN_IFCMD_TXRQST_NEWDAT | 
                                             DCAN_IFCMD_CLRINTPND |
                                             DCAN_IFCMD_CONTROL | 
                                             DCAN_IFCMD_ARB |
                                             DCAN_IFCMD_MASK | 
                                             DCAN_IFCMD_MESSAGENUMBER |
                                             DCAN_IFCMD_WR_RD);

    /* Wait in loop until busy bit is cleared */
    while(DCANIFBusyStatusGet(baseAdd, regNum));

    /* Set the DCAN_IFCMD register fields represented by cmdFlags */
    HWREG(baseAdd + DCAN_IFCMD(regNum)) |= ((cmdFlags & 
                                           (DCAN_IFCMD_DMAACTIVE | 
                                            DCAN_IFCMD_DATAA | 
                                            DCAN_IFCMD_DATAB | 
                                            DCAN_IFCMD_TXRQST_NEWDAT |
                                            DCAN_IFCMD_CLRINTPND |
                                            DCAN_IFCMD_CONTROL |
                                            DCAN_IFCMD_ARB |
                                            DCAN_IFCMD_MASK |
                                            DCAN_IFCMD_WR_RD)) | 
                                            (objNum & DCAN_IFCMD_MESSAGENUMBER));
}

/**
 * \brief   This API will return the status of Busy field from DCAN_IFCMD 
 *          register. 
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   regNum              Interface register number used.
 *
 * 'regNum' can take the following values \n
 *    DCAN_IF1_REG - Interface register 1 is used \n
 *    DCAN_IF2_REG - Interface register 2 is used \n
 *
 * \return  Returns the Busy bit status from the DCAN_IFCMD register.
 *          User can use the below macros to check the status \n
 *          DCAN_IF_BUSY - Transfer between IF register and message RAM is 
 *                         in progress.
 *          DCAN_IF_NOT_BUSY - No Transfer between IF register and message RAM.
 *
 **/
unsigned int DCANIFBusyStatusGet(unsigned int baseAdd, unsigned int regNum)
{
    /* Returns the status of BUSY field from DCAN_IF_CMD register */
    return(HWREG(baseAdd + DCAN_IFCMD(regNum)) & DCAN_IFCMD_BUSY);
}

/**
 * \brief   This API will set the message identifier length and number.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   msgId               Message identifier number.
 * \param   idLength            Identifier length.
 * \param   regNum              Interface register number used.
 *
 * 'idLength' can take the following values \n
 *    DCAN_11_BIT_ID - 11 bit identifier is used \n
 *    DCAN_29_BIT_ID - 29 bit identifier is used \n
 *
 * 'regNum' can take the following values \n
 *    DCAN_IF1_REG - Interface register 1 is used \n
 *    DCAN_IF2_REG - Interface register 2 is used \n
 *
 * \return  None.
 *
 **/
void DCANMsgIdSet(unsigned int baseAdd, unsigned int msgId, 
                  unsigned int idLength, unsigned int regNum)
{

    if(idLength == DCAN_11_BIT_ID)
    {
        msgId <<= DCAN_STD_ID_SHIFT;
    }
  
    /* Wait in loop until busy bit is cleared */
    while(DCANIFBusyStatusGet(baseAdd, regNum));

    /* Clear the Msk field of DCAN_IFARB register */
    HWREG(baseAdd + DCAN_IFARB(regNum)) &= ~(DCAN_IFARB_MSK | DCAN_IFARB_XTD);

    /* Wait in loop until busy bit is cleared */
    while(DCANIFBusyStatusGet(baseAdd, regNum));

    /* Set the Msk field with the ID value */
    HWREG(baseAdd + DCAN_IFARB(regNum)) |= ((msgId & DCAN_IFARB_MSK) | 
                                            (idLength & DCAN_IFARB_XTD));
}

/**
 * \brief   This API will set the direction for the message object.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   msgDir              Message direction for the message object.
 * \param   regNum              Interface register number used.
 *
 * 'msgDir' can take the following values \n
 *    DCAN_TX_DIR - Message object set to transmit a message \n
 *    DCAN_RX_DIR - Message object set to receive a message \n
 *
 * 'regNum' can take the following values \n
 *    DCAN_IF1_REG - Interface register 1 is used \n
 *    DCAN_IF2_REG - Interface register 2 is used \n
 *
 * \return  None.
 *
 **/
void DCANMsgDirectionSet(unsigned int baseAdd, unsigned int msgDir, 
                         unsigned int regNum)
{
    /* Wait in loop until busy bit is cleared */
    while(DCANIFBusyStatusGet(baseAdd, regNum));

    /* Clear the Dir field of DCAN_IFARB register */
    HWREG(baseAdd + DCAN_IFARB(regNum)) &= ~DCAN_IFARB_DIR;

    /* Wait in loop until busy bit is cleared */
    while(DCANIFBusyStatusGet(baseAdd, regNum));

    /* Set the Dir field with the user sent value */
    HWREG(baseAdd + DCAN_IFARB(regNum)) |= (msgDir & DCAN_IFARB_DIR);
}

/**
 * \brief   This API will write data bytes to the IF Data registers.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   dataPtr             Pointer used to fetch data bytes.
 * \param   regNum              Interface register number used.
 * 
 * 'regNum' can take the following values \n
 *    DCAN_IF1_REG - IF register 1 is used \n
 *    DCAN_IF2_REG - IF register 2 is used \n
 *
 * \return  None.
 *
 **/
void DCANDataWrite(unsigned int baseAdd, unsigned int* dataPtr, 
                   unsigned int regNum)
{
    /* Wait in loop until busy bit is cleared */
    while(DCANIFBusyStatusGet(baseAdd, regNum));

    /* Write the lower 4 data bytes to IFDATA register */
    HWREG(baseAdd + DCAN_IFDATA(regNum)) = *dataPtr++;

    /* Wait in loop until busy bit is cleared */
    while(DCANIFBusyStatusGet(baseAdd, regNum));

    /* Write the higher 4 data bytes to IFDATB register */
    HWREG(baseAdd + DCAN_IFDATB(regNum)) = *dataPtr;
}

/**
 * \brief   This API will read the data bytes from the IF Data registers.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   regNum              Interface register number used.
 *
 * 'regNum' can take the following values \n
 *    DCAN_IF1_REG - IF register 1 is used \n
 *    DCAN_IF2_REG - IF register 2 is used \n
 *    DCAN_IF3_REG - IF register 3 is used \n
 *
 * \return  None.
 *
 **/
void DCANDataRead(unsigned int baseAdd, unsigned int* data, 
                  unsigned int regNum)
{
    /* Read the data bytes from the DCAN_IFDATA register */
    *data++ = HWREG(baseAdd + DCAN_IFDATA(regNum));

    /* Read the data bytes from the DCAN_IFDATB register */
    *data = HWREG(baseAdd + DCAN_IFDATB(regNum));
}

/**
 * \brief   This API will set the data length code.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   dlc                 Data length code.
 * \param   regNum              Interface register number used.
 *
 * 'dlc' can take the below values \n
 *    dlc can range between 1-8 for 1-8 data bytes \n
 *    dlc value lying between 9-15 will configure it for 8 data bytes \n 
 * 
 * 'regNum' can take the following values \n
 *    DCAN_IF1_REG - IF register 1 is used \n
 *    DCAN_IF2_REG - IF register 2 is used \n
 *
 * \return  None.
 *
 **/
void DCANDataLengthCodeSet(unsigned int baseAdd, unsigned int dlc, 
                           unsigned int regNum)
{
    /* Wait in loop until busy bit is cleared */
    while(DCANIFBusyStatusGet(baseAdd, regNum));

    /* Clear the DLC field of DCAN_IFMCTL register */
    HWREG(baseAdd + DCAN_IFMCTL(regNum)) &= ~DCAN_IFMCTL_DATALENGTHCODE;

    /* Wait in loop until busy bit is cleared */
    while(DCANIFBusyStatusGet(baseAdd, regNum));

    /* Set the DLC field with the user sent value */
    HWREG(baseAdd + DCAN_IFMCTL(regNum)) |= (dlc & DCAN_IFMCTL_DATALENGTHCODE);
}

/**
 * \brief   This API will configure the setting for remote enable.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   remEnable           Configure remote enable.
 * \param   regNum              Interface register number used.
 *
 * 'remEnable' can take the following values \n
 *    DCAN_REMOTE_ENABLE - At the reception of a remote frame TxRqst is set \n
 *    DCAN_REMOTE_DISABLE - At the reception of a remote frame TxRqst is not 
 *                          changed \n
 *
 * 'regNum' can take the following values \n
 *    DCAN_IF1_REG - IF register 1 is used \n
 *    DCAN_IF2_REG - IF register 2 is used \n
 *
 * \return  None.
 *
 **/
void DCANRemoteEnableControl(unsigned int baseAdd, unsigned int remEnable, 
                             unsigned int regNum)
{
    /* Wait in loop until busy bit is cleared */
    while(DCANIFBusyStatusGet(baseAdd, regNum));

    /* Clear the RmtEn field of DCAN_IFMCTL register */
    HWREG(baseAdd + DCAN_IFMCTL(regNum)) &= ~DCAN_IFMCTL_RMTEN;

    /* Wait in loop until busy bit is cleared */
    while(DCANIFBusyStatusGet(baseAdd, regNum));

    /* Set the remote enable field with the user sent value */
    HWREG(baseAdd + DCAN_IFMCTL(regNum)) |= (remEnable & DCAN_IFMCTL_RMTEN);
}

/**
 * \brief   This API will enable the Message object interrupts of the DCAN 
 *          peripheral.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   intFlags            Enable the message object interrupts 
 *                              represented by intFlags.
 * \param   regNum              Interface register number used.
 *
 * 'intFlags' can take the following values \n
 *    DCAN_TRANSMIT_INT - Enable the transmit interrupt \n 
 *    DCAN_RECEIVE_INT - Enable the receive interrupt \n
 *
 * 'regNum' can take the following values \n
 *    DCAN_IF1_REG - IF register 1 is used \n
 *    DCAN_IF2_REG - IF register 2 is used \n
 *
 * \return  None.
 *
 **/
void DCANMsgObjIntEnable(unsigned int baseAdd, unsigned int intFlags, 
                         unsigned int regNum)
{
    /* Wait in loop until busy bit is cleared */
    while(DCANIFBusyStatusGet(baseAdd, regNum));

    /* Enable Message object interrupts */
    HWREG(baseAdd + DCAN_IFMCTL(regNum)) |= (intFlags & 
                                            (DCAN_IFMCTL_TXIE | 
                                             DCAN_IFMCTL_RXIE));
}

/**
 * \brief   This API will disable the Message object interrupts of the DCAN
 *          peripheral.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   intFlags            Disable the message object interrupts
 *                              represented by intFlags.
 * \param   regNum              Interface register number used.
 *
 * 'intFlags' can take the following values \n
 *    DCAN_TRANSMIT_INT - Disable the transmit interrupt \n
 *    DCAN_RECEIVE_INT - Disable the receive interrupt \n
 *
 * 'regNum' can take the following values \n
 *    DCAN_IF1_REG - IF register 1 is used \n
 *    DCAN_IF2_REG - IF register 2 is used \n
 *
 * \return  None.
 *
 **/
void DCANMsgObjIntDisable(unsigned int baseAdd, unsigned int intFlags,
                          unsigned int regNum)
{
    /* Wait in loop until busy bit is cleared */
    while(DCANIFBusyStatusGet(baseAdd, regNum));

    /* Enable Message object interrupts */
    HWREG(baseAdd + DCAN_IFMCTL(regNum)) &= ~(intFlags &
                                             (DCAN_IFMCTL_TXIE |
                                              DCAN_IFMCTL_RXIE));
}

/**
 * \brief   This API will configure the end of block settings for the DCAN
 *          peripheral.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   eob                 Configure End of block.
 * \param   regNum              Interface register number used.
 *
 * 'eob' can take the following values \n
 *    DCAN_END_OF_BLOCK_ENABLE - Enable end of block \n
 *    DCAN_END_OF_BLOCK_DISABLE - Disable end of block \n
 *
 * 'regNum' can take the following values \n
 *    DCAN_IF1_REG - IF register 1 is used \n
 *    DCAN_IF2_REG - IF register 2 is used \n
 *
 * \return  None.
 *
 **/
void DCANFIFOEndOfBlockControl(unsigned int baseAdd, unsigned int eob, 
                               unsigned int regNum)
{
    /* Wait in loop until busy bit is cleared */
    while(DCANIFBusyStatusGet(baseAdd, regNum));

    /* Clear the EOB field of DCAN_IFMCTL register */
    HWREG(baseAdd + DCAN_IFMCTL(regNum)) &= ~DCAN_IFMCTL_EOB;

    /* Wait in loop until busy bit is cleared */
    while(DCANIFBusyStatusGet(baseAdd, regNum));

    /* Set the EOB field with the user sent value */
    HWREG(baseAdd + DCAN_IFMCTL(regNum)) |= (eob & DCAN_IFMCTL_EOB);
}

/**
 * \brief   This API will configure the mask settings for a message object.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   idMsk               Identifier mask.
 * \param   msgDir              Mask message direction.
 * \param   extId               Mask extended identifier.
 * \param   regNum              Interface register number used.
 *
 * 'idMsk' can take the following value \n
 *    DCAN_IDENTIFIER_MSK(mask, idType) \n
 *    where 0 <= mask <= 0x1FFFFFFF \n
 *
 *    'idType' can take the following values \n
 *    DCAN_ID_MSK_11_BIT - 11 bit identifier mask is used \n
 *    DCAN_ID_MSK_29_BIT - 29 bit identifier mask is used \n
 *
 * 'msgDir' can take the following values \n
 *    DCAN_MSK_MSGDIR_ENABLE - Message direction bit is used for 
 *                             acceptance filtering \n
 *    DCAN_MSK_MSGDIR_DISABLE - Message direction bit has no effect on 
 *                              acceptance filtering \n
 * 
 * 'extId' can take the following values \n
 *    DCAN_MSK_EXT_ID_ENABLE - The IDE bit is used for acceptance filtering \n
 *    DCAN_MSK_EXT_ID_DISABLE - The IDE bit is not used for acceptance 
 *                              filtering \n
 *
 * 'regNum' can take the following values \n
 *    DCAN_IF1_REG - IF register 1 is used \n
 *    DCAN_IF2_REG - IF register 2 is used \n
 *
 * \return  None.
 *
 **/
void DCANMsgObjectMskConfig(unsigned int baseAdd, unsigned int idMsk,
                            unsigned int msgDir, unsigned int extId,
                            unsigned int regNum)
{
    /* Wait in loop until busy bit is cleared */
    while(DCANIFBusyStatusGet(baseAdd, regNum));

    /* Write to the DCAN_IFMSK register */
    HWREG(baseAdd + DCAN_IFMSK(regNum)) = ((idMsk & DCAN_IF1MSK_MSK) | 
                                           (msgDir & DCAN_IFMSK_MDIR) | 
                                           (extId & DCAN_IFMSK_MXTD));
}

/**
 * \brief   This API will configure IF3 register set so that it is 
 *          automatically updated with the received value in message RAM.
 *          Where n = 12,34,56,78.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   msgNum              Message object number for which IF3 register 
 *                              set has to be updated.
 *
 * \return  None.
 *
 * \note    Values for 'msgNum' can range from \n
 *          1 <= msgNum <= 128.
 *          IF3 Update enable should not be set for transmit objects.
 *
 **/
void DCANIF3RegUpdateEnableSet(unsigned int baseAdd, unsigned int msgNum)
{
    unsigned int regNum;
    unsigned int offSet;

    regNum = (msgNum - 1) / 32;
    offSet = (msgNum - 1) % 32;

    /* Set the DCAN_IF3UPD register with the proper value */
    HWREG(baseAdd + DCAN_IF3UPD(regNum)) |= (1 << offSet);
}

/**
 * \brief   This API will set the observation flag bits in the IF3 observation
 *          register which are used to determine which data sections of the IF3
 *          interface register set have to be read in order to complete a DMA 
 *          read cycle.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   obsFlags            Flags which are to be set.
 *
 * 'obsFlags' can take the follwing values \n
 *    DCAN_MASK_DATA - Set Mask data read observation \n
 *    DCAN_ARB_DATA - Set Arbitration data read observation \n
 *    DCAN_CTRL_DATA - Set Ctrl read observation \n
 *    DCAN_DAT_A_DATA - Set Data A read observation \n
 *    DCAN_DAT_B_DATA - Set Data B read observation \n
 *
 * \return  None.
 *
 **/
void DCANIF3ObservationFlagSet(unsigned int baseAdd, unsigned int obsFlags)
{
    /* Set the appropriate flags in the DCAN_IF3OBS register */
    HWREGB(baseAdd + DCAN_IF3OBS) |= (obsFlags & (DCAN_IF3OBS_MASK | 
                                      DCAN_IF3OBS_ARB | DCAN_IF3OBS_CTRL | 
                                      DCAN_IF3OBS_DATAA | DCAN_IF3OBS_DATAB));
}

/**
 * \brief   This API will clear the observation flag bits in the IF3 observation
 *          register which are used to determine which data sections of the IF3
 *          interface register set have to be read in order to complete a DMA
 *          read cycle.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   obsFlags            Flags which are to be cleared.
 *
 * 'obsFlags' can take the following values \n
 *    DCAN_MASK_DATA - Clear Mask data read observation \n
 *    DCAN_ARB_DATA - Clear Arbitration data read observation \n
 *    DCAN_CTRL_DATA - Clear Ctrl read observation \n
 *    DCAN_DAT_A_DATA - Clear Data A read observation \n
 *    DCAN_DAT_B_DATA - Clear Data B read observation \n
 *
 * \return  None.
 *
 **/
void DCANIF3ObservationFlagClear(unsigned int baseAdd, unsigned int obsFlags)
{
    /* Set the appropriate flags in the DCAN_IF3OBS register */
    HWREGB(baseAdd + DCAN_IF3OBS) &= ~(obsFlags & (DCAN_IF3OBS_MASK | 
                                       DCAN_IF3OBS_ARB | DCAN_IF3OBS_CTRL | 
                                       DCAN_IF3OBS_DATAA| DCAN_IF3OBS_DATAB));
}

/**
 * \brief   This API will return observation flag status from the DCAN_IF3OBS
 *          register. 
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 *
 * \return  Returns the observation flag status from the DCAN_IF3OBS register.
 *          The following macros can be used to check the status \n
 *          DCAN_IF3_MASK_STATUS - IF3 status of Mask data read access \n
 *          DCAN_IF3_ARB_STATUS - IF3 status of Arbitration data read access \n
 *          DCAN_IF3_CTRL_STATUS - IF3 status of Control bits read access \n
 *          DCAN_IF3_DAT_A_STATUS - IF3 status of Data A read access \n
 *          DCAN_IF3_DAT_B_STATUS - IF3 status of Data B read access \n
 *          DCAN_IF3_UPDATE_STATUS - IF3 Update data status \n
 *
 **/
unsigned char DCANIF3ObservationFlagStatGet(unsigned int baseAdd)
{
    /* Return the observation flag status from the DCAN_IF3OBS register */
    return(HWREGB(baseAdd + DCAN_IF3OBS + 1));
}

/**
 * \brief   This API will return the status from the DCAN_IF3MSK register.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 *
 * \return  Returns the status of DCAN_IF3MSK register.
 *          The following macros can be used to check the status \n
 *          DCAN_ID_MSK_READ - Read Identifier mask \n
 *          DCAN_MSK_MSG_DIR_READ - Read mask message direction \n
 *          DCAN_MSK_EXT_ID_READ - Read mask extended identifier \n
 *
 **/
unsigned int DCANIF3MaskStatusGet(unsigned int baseAdd)
{
    /* Return the status of DCAN_IF3MSK register */
    return(HWREG(baseAdd + DCAN_IFMSK(DCAN_IF3_REG)));
}

/**
 * \brief   This API will return the status from the DCAN_IF3ARB register.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 *
 * \return  Returns the status of DCAN_IF3ARB register.
 *          The following macros can be used to check the status \n
 *          DCAN_MSG_ID_READ - Read message Identifier \n
 *          DCAN_MSG_DIR_READ - Read message direction \n
 *          DCAN_EXT_ID_READ - Read extended identifier \n
 *          DCAN_MSGVAL_READ - Read message valid status \n
 *
 **/
unsigned int DCANIF3ArbStatusGet(unsigned int baseAdd)
{
    /* Return the status of DCAN_IF3ARB register */
    return(HWREG(baseAdd + DCAN_IFARB(DCAN_IF3_REG)));
}

/**
 * \brief   This API will return the status from the DCAN_IFMCTL register.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   regNum              Interface register used.
 *
 * 'regNum' can take the following values \n
 *    DCAN_IF1_REG - Interface register 1 is used \n
 *    DCAN_IF2_REG - Interface register 2 is used \n
 *    DCAN_IF3_REG - Interface register 3 is used \n
 *
 * \return  Returns the status of DCAN_IFMCTL register.
 *          The following macros can be used to check the status \n
 *          DCAN_DAT_LEN_CODE_READ - Read data length code \n
 *          DCAN_END_OF_BLOCK_READ - Read end of block bit \n
 *          DCAN_TXRQST_READ - Read transmit request bit \n
 *          DCAN_RMT_ENABLE_READ - Read remote enable bit \n
 *          DCAN_RX_INT_ENABLE_READ - Read Rx interrupt enable bit \n
 *          DCAN_TX_INT_ENABLE_READ - Read Tx interrupt enable bit \n
 *          DCAN_UMASK_READ - Read use acceptance mask bit \n
 *          DCAN_INTPND_READ - Read interrupt pending status \n
 *          DCAN_MSG_LOST_READ - Read message lost status \n
 *          DCAN_NEWDAT_READ - Read new data status \n
 *
 **/
unsigned int DCANIFMsgCtlStatusGet(unsigned int baseAdd, unsigned int regNum)
{
    /* Return the status of DCAN_IFMCTl register */
    return(HWREG(baseAdd + DCAN_IFMCTL(regNum)));
}

/**
 * \brief   This API will configure the DCAN TX pin as a general purpose pin
 *          or as a  functional pin.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   txPinMode           Mode used for DCAN Tx pin.
 *
 * 'txPinMode' can take the following values \n
 *    DCAN_TX_PIN_GIO_MODE - Tx pin is used as a GIO pin \n
 *    DCAN_TX_PIN_FUNC_MODE - Tx pin is used as a functional pin of DCAN
 *                            core \n
 *
 * \return  None.
 *
 * \note    If DCAN Tx pin is to be used as a GIO pin then the Init bit in CAN
 *          control register must be set. If Tx pin is in GIO mode and the Init
 *          bit gets reset then the DCAN Tx pin will be automatically used as a
 *          functional pin of DCAN. In functional mode Tx pin will be used
 *          as output.
 *
 **/
void DCANTxPinModeConfig(unsigned int baseAdd, unsigned int txPinMode)
{
    /* Clear the Func field of DCAN_TIOC register */
    HWREG(baseAdd + DCAN_TIOC) &= ~DCAN_TIOC_FUNC;

    /* Set the Func field with the user sent value */
    HWREG(baseAdd + DCAN_TIOC) |= (txPinMode & DCAN_TIOC_FUNC);
}

/**
 * \brief   This API will configure the polarity for DCAN TX pin when it is
 *          used as an input pin.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   pinLogic            Logic used for Tx pin.
 *
 * 'pinLogic' can take the following values \n
 *    DCAN_TX_PIN_IN_LOGIC_LOW - Tx pin used as input with low logic \n
 *    DCAN_TX_PIN_IN_LOGIC_HIGH - Tx pin used as input with high logic \n
 *
 * \return  None.
 *
 * \note    Please ensure to configure the DCAN Tx pin as GIO before calling
 *          this API. This API is valid only if 'Init' bit of CAN control
 *          register is set. The CAN controller can go to functional mode if
 *          'Init' bit becomes reset even if this API is used. In functional
 *          mode Tx pin will be used as output.
 *
 **/
void DCANTxPinInputDirConfig(unsigned int baseAdd, unsigned int pinLogic)
{
    /* Configure the Tx pin of DCAN as input */
    HWREG(baseAdd + DCAN_TIOC) &= ~DCAN_TIOC_DIR;

    /* Clear the In field of DCAN_TIOC register */
    HWREG(baseAdd + DCAN_TIOC) &= ~DCAN_TIOC_IN;

    /* Set the In field with the user sent value */
    HWREG(baseAdd + DCAN_TIOC) |= (pinLogic & DCAN_TIOC_IN);
}

/**
 * \brief   This API will configure the polarity for DCAN TX pin when it is
 *          used as an output pin.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   pinLogic            Logic used for Tx pin.
 *
 * 'pinLogic' can take the following values \n
 *    DCAN_TX_PIN_OUT_LOGIC_LOW - Tx pin used as output with low logic \n
 *    DCAN_TX_PIN_OUT_LOGIC_HIGH - Tx pin used as output with high logic \n
 *
 * \return  None.
 *
 * \note    Please ensure to configure the DCAN Tx pin as GIO before calling
 *          this API. This API is valid only if 'Init' bit of CAN control
 *          register is set. The CAN controller can go to functional mode if
 *          'Init' bit becomes reset even if this API is used. In functional
 *          mode Tx pin will be used as output.
 *
 **/
void DCANTxPinOutputDirConfig(unsigned int baseAdd, unsigned int pinLogic)
{
    /* Configure the Tx pin of DCAN as output */
    HWREG(baseAdd + DCAN_TIOC) |= DCAN_TIOC_DIR;

    /* Clear the Out field of DCAN_TIOC register */
    HWREG(baseAdd + DCAN_TIOC) &= ~DCAN_TIOC_OUT;

    /* Set the Out field with the user sent value */
    HWREG(baseAdd + DCAN_TIOC) |= (pinLogic & DCAN_TIOC_OUT);
}

/**
 * \brief   This API will configure the DCAN RX pin as a general purpose pin
 *          or as a functional pin.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   rxPinMode           Mode used for DCAN Rx pin.
 *
 * 'rxPinMode' can take the following values \n
 *    DCAN_RX_PIN_GIO_MODE - Rx pin is used as a GIO pin \n
 *    DCAN_RX_PIN_FUNC_MODE - Rx pin is used as a functional pin of DCAN
 *                            core \n
 *
 * \return  None.
 *
 * \note    If DCAN Rx pin is to be used as a GIO pin then the Init bit in CAN
 *          control register must be set. If Rx pin is in GIO mode and the Init
 *          bit gets reset then the DCAN Rx pin will be automatically used as a
 *          functional pin of DCAN core. In functional mode Rx pin will be used
 *          as output.
 *
 **/
void DCANRxPinModeConfig(unsigned int baseAdd, unsigned int rxPinMode)
{
    /* Clear the Func field of DCAN_TIOC register */
    HWREG(baseAdd + DCAN_RIOC) &= ~DCAN_RIOC_FUNC;

    /* Set the Func field with the user sent value */
    HWREG(baseAdd + DCAN_RIOC) |= (rxPinMode & DCAN_RIOC_FUNC);
}

/**
 * \brief   This API will configure the polarity for DCAN RX pin when it is
 *          used as an input pin.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   pinLogic            Logic used for Rx pin.
 *
 * 'pinLogic' can take the following values \n
 *    DCAN_RX_PIN_IN_LOGIC_LOW - Rx pin used as input with low logic \n
 *    DCAN_RX_PIN_IN_LOGIC_HIGH - Rx pin used as input with high logic \n
 *
 * \return  None.
 *
 * \note    Please ensure to configure the DCAN Rx pin as GIO before calling
 *          this API. This API is valid only if 'Init' bit of CAN control
 *          register is set. The CAN controller can go to functional mode if
 *          'Init' bit becomes reset even if this API is used. In functional
 *          mode Rx pin will be used as output.
 *
 **/
void DCANRxPinInputDirConfig(unsigned int baseAdd, unsigned int pinLogic)
{
    /* Configure the Rx pin of DCAN as input */
    HWREG(baseAdd + DCAN_RIOC) &= ~DCAN_RIOC_DIR;

    /* Clear the In field of DCAN_RIOC register */
    HWREG(baseAdd + DCAN_RIOC) &= ~DCAN_RIOC_IN;

    /* Set the In field with the user sent value */
    HWREG(baseAdd + DCAN_RIOC) |= (pinLogic & DCAN_RIOC_IN);
}

/**
 * \brief   This API will configure the polarity for DCAN RX pin when it is
 *          used as an output pin.
 *
 * \param   baseAdd             Base Address of the DCAN Module Registers.
 * \param   pinLogic            Logic used for Rx pin.
 *
 * 'pinLogic' can take the following values \n
 *    DCAN_RX_PIN_OUT_LOGIC_LOW - Rx pin used as output with low logic \n
 *    DCAN_RX_PIN_OUT_LOGIC_HIGH - Rx pin used as output with high logic \n
 *
 * \return  None.
 *
 * \note    Please ensure to configure the DCAN Rx pin as GIO before calling
 *          this API. This API is valid only if 'Init' bit of CAN control
 *          register is set. The CAN controller can go to functional mode if
 *          'Init' bit becomes reset even if this API is used. In functional
 *          mode Rx pin will be used as output.
 *
 **/
void DCANRxPinOutputDirConfig(unsigned int baseAdd, unsigned int pinLogic)
{
    /* Configure the Rx pin of DCAN as output */
    HWREG(baseAdd + DCAN_RIOC) |= DCAN_RIOC_DIR;

    /* Clear the Out field of DCAN_RIOC register */
    HWREG(baseAdd + DCAN_RIOC) &= ~DCAN_RIOC_OUT;

    /* Set the Out field with the user sent value */
    HWREG(baseAdd + DCAN_RIOC) |= (pinLogic & DCAN_RIOC_OUT);
}

/****************************** END OF FILE ***********************************/
