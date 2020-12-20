/**
 *  \file   dcan.h
 *
 *  \brief  DCAN APIs and macros.
 *
 *   This file contains the driver API prototypes and macro definitions for
 *   DCAN peripheral.
 */

/* Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */
#ifndef __DCAN_H__
#define __DCAN_H__

#include "hw_dcan.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
/*
** Values that can be passed to DCAN APIs
*/

/******************************************************************************/
/*
** Value that can be passed to 'DCANBitTimingConfig' API as 'bitTime'.
*/
/* Configure propagation delay, syncronization jump width and segment2 setting */
#define DCAN_BIT_TIME(propDel, jumpWidth, segmnt2) \
                                     ((unsigned int)\
                                     (((jumpWidth - 1) & 0x03u) << 6) | \
                                     (((segmnt2 - 1) & 0x07u) << 12) | \
                                     (((propDel + jumpWidth - 1) & 0x0Fu) << 8))

/******************************************************************************/
/*
** Values that can be passed to 'DCANIntLineEnable' and 'DCANIntLineDisable'APIs 
** as 'enableLine' and 'disableLine' respectively. 
** 'enableLine','disableLine' can take both the values.
** For e.g. (DCAN_INT_LINE0 | DCAN_INT_LINE1) can be passed.
*/ 
/* Enable/Disable Interrupt line 0 of DCAN peripheral */
#define DCAN_INT_LINE0                       (DCAN_CTL_IE0)
/* Enable/Disable Interrupt line 1 of DCAN peripheral */
#define DCAN_INT_LINE1                       (DCAN_CTL_IE1)

/******************************************************************************/
/*
** Values that can be passed to 'DCANIntEnable' and 'DCANIntDisable' APIs as 
** 'intFlags'.
** 'intFlags' can take more than one value.
** For e.g. (DCAN_STATUS_CHANGE_INT | DCAN_ERROR_INT) can be passed.
*/
/* Enable/Disable the Status change interrupt of DCAN peripheral */
#define DCAN_STATUS_CHANGE_INT               (0x00000004u)
/* Enable/Disable the error interrupt of DCAN peripheral */
#define DCAN_ERROR_INT                       (0x00000008u)

/******************************************************************************/
/*
** Values that can be passed to 'DCANAutoReTransmitControl' API as 'autoReTxn'.
*/
/* Enable Auto Re-transmission of DCAN peripheral */
#define DCAN_AUTO_RETXN_ENABLE               (0x00000000u)
/* Disable Auto Re-transmission of DCAN peripheral */
#define DCAN_AUTO_RETXN_DISABLE              (0x00000020u)

/******************************************************************************/
/*
** Values that can be passed to 'DCANConfigRegWriteAccessControl' API as 
** 'regConfig'.
*/
/* CPU has write access to the configuration registers */
#define DCAN_CONF_REG_WR_ACCESS_ENABLE       (DCAN_CTL_CCE)
/* CPU has no write access to the configuration registers */
#define DCAN_CONF_REG_WR_ACCESS_DISABLE      (DCAN_CTL_CCE_NOACCESS)

/******************************************************************************/
/*
** Values that can be passed to 'DCANTestModeControl' API as 'testMode'.
*/
/* Enable Test mode of operation */
#define DCAN_TEST_MODE_ENABLE                (DCAN_CTL_TEST)
/* Disable Test mode of operation */
#define DCAN_TEST_MODE_DISABLE               (DCAN_CTL_TEST_NORMALMODE)

/******************************************************************************/
/*
** Values that can be passed to 'DCANDebugSuspendModeConfig' API as 'modeConfig'.
*/
/* 
** DCAN will wait for a started transmission or reception to be completed before 
** entering debug/suspend mode.
*/
#define DCAN_DEBUG_SUSPEND_WAIT              (DCAN_CTL_IDS_WAIT)
/*
** DCAN will interrupt any transmission or reception, and enter debug/suspend 
** mode immediately.
*/
#define DCAN_DEBUG_SUSPEND_INTERRUPT         (DCAN_CTL_IDS)

/******************************************************************************/
/*
** Values that can be passed to 'DCANAutoBusOnControl' API as 'busControl'.
*/
/* Enable the Auto-bus-on feature */
#define DCAN_AUTO_BUS_ON_ENABLE              (DCAN_CTL_ABO)
/* Disable the Auto-bus-on feature */
#define DCAN_AUTO_BUS_ON_DISABLE             (DCAN_CTL_ABO_DISABLED)

/******************************************************************************/
/*
** Values that can be passed to 'DCANParityControl' API as 'paritySet'.
*/
/* Enable Parity function */
#define DCAN_PARITY_FUNC_ENABLE              (DCAN_CTL_PMD_ENABLED << \
                                              DCAN_CTL_PMD_SHIFT)
/* Disable Parity function */
#define DCAN_PARITY_FUNC_DISABLE             (DCAN_CTL_PMD_DISABLED << \
                                              DCAN_CTL_PMD_SHIFT)

/******************************************************************************/
/*
** Values that can be used to return the status from 'DCANInternalDebugStatusGet' 
** API.
*/
/* Peripheral is not in debug mode */
#define DCAN_NOT_IN_DEBUG_MODE               (DCAN_CTL_INITDBG_DISABLED)
/* Peripheral is in debug mode or is ready for debug access */
#define DCAN_IN_DEBUG_MODE                   (DCAN_CTL_INITDBG)

/******************************************************************************/
/*
** Values that can be passed to 'DCANDmaRequestLineEnable' and 
** 'DCANDmaRequestLineDisable' APIs as 'ifRegFlags'.
** 'ifRegFlags' can take more than one value.
** For e.g. (DCAN_DMA_REQUEST_LINE_ENABLE_IF1 | DCAN_DMA_REQUEST_LINE_ENABLE_IF2)
** can be passed as 'ifRegFlags'
*/
/* Enable/Disable DMA request line for IF1 */
#define DCAN_DMA_REQUEST_LINE_IF1            (DCAN_CTL_DE1)
/* Enable/Disable DMA request line for IF2 */
#define DCAN_DMA_REQUEST_LINE_IF2            (DCAN_CTL_DE2)
/* Enable/Disable DMA request line for IF3 */
#define DCAN_DMA_REQUEST_LINE_IF3            (DCAN_CTL_DE3)

/******************************************************************************/
/*
** Values that can be passed to 'DCANPwrDwnControl' as 'pwrDwnCtl' 
*/
/* Local power down is not requested */
#define DCAN_LOCAL_PWR_DWN_OFF               (DCAN_CTL_PDR_NOTPOWERDOWN)
/* Local power down mode is requested */
#define DCAN_LOCAL_PWR_DWN_ON                (DCAN_CTL_PDR)

/******************************************************************************/
/*
** Values that can be passed on to 'DCANPwrDwnWkUpControl' API as 'wkUpCtl'
*/
/* No detection of a dominant CAN bus level when in local power down mode */
#define DCAN_WKUP_DETECTION_DISABLED         (DCAN_CTL_WUBA_NODETECTION)
/* Detection of a dominant CAN bus level when in local power down is enabled */
#define DCAN_WKUP_DETECTION_ENABLED          (DCAN_CTL_WUBA)

/******************************************************************************/
/*
** Values that can be used to check the status of DCAN_ES register got from
** 'DCANErrAndStatusRegInfoGet' API.
*/
/* No error is detected */
#define DCAN_LST_ERRCODE_NO_ERR              (DCAN_ES_LEC_NOERROR)
/* Stuff error is detected */
#define DCAN_LST_ERRCODE_STUFF_ERR           (DCAN_ES_LEC_STUFFERROR)
/* Form error is detected */
#define DCAN_LST_ERRCODE_FORM_ERR            (DCAN_ES_LEC_FORMERROR)
/* Ack error is detected */
#define DCAN_LST_ERRCODE_ACK_ERR             (DCAN_ES_LEC_ACKERROR)
/* Bit1 error is detected */
#define DCAN_LST_ERRCODE_BIT1_ERR            (DCAN_ES_LEC_BIT1ERROR)
/* Bit0 error is detected */
#define DCAN_LST_ERRCODE_BIT0_ERR            (DCAN_ES_LEC_BIT0ERROR)
/* CRC error detected */
#define DCAN_LST_ERRCODE_CRC_ERR             (DCAN_ES_LEC_CRCERROR)
/* No event is generated on the CAN bus since last read of DCAN_ES */
#define DCAN_NO_EVENT_ON_CAN_BUS             (DCAN_ES_LEC_NOEVENT)
/* DCAN transmitted message successfully */
#define DCAN_TXD_MSG_SUCCESSFULLY            (DCAN_ES_TXOK)
/* DCAN received message successfully */
#define DCAN_RXD_MSG_SUCCESSFULLY            (DCAN_ES_RXOK)
/* DCAN is in error passive state */
#define DCAN_CORE_IN_ERR_PASSIVE             (DCAN_ES_EPASS)
/* Atleast one of the counters have reached the error warning limit */
#define DCAN_ERR_WARN_STATE_RCHD             (DCAN_ES_EWARN)
/* DCAN is in Bus off state */
#define DCAN_MOD_IN_BUS_OFF_STATE            (DCAN_ES_BOFF)
/* Parity error detected */
#define DCAN_PARITY_ERR_DETECTED             (DCAN_ES_PER)
/* DCAN initiated system wakeup */
#define DCAN_INITIATED_SYSTEM_WKUP           (DCAN_ES_WAKEUPPND)
/* DCAN is in local power down mode */
#define DCAN_IN_LOCAL_PWR_DWN_MODE           (DCAN_ES_PDA)

/******************************************************************************/
/*
** Values that can be sent to 'DCANErrCntrRegStatusGet' API as 'cntrFlags'.
** 'cntrFlags' can take more than one value. 
** For e.g. (DCAN_TX_ERR_CNTR | DCAN_RX_ERR_CNTR | DCAN_RX_ERR_PASSIVE) can be 
** sent as 'cntrFlags'.
*/
/* Macro used to return the status of TEC[7:0] field of DCAN_ERRC register */
#define DCAN_TX_ERR_CNTR                     (DCAN_ERRC_TEC)
/* Macro used to return the status of REC[6:0] field of DCAN_ERRC register*/
#define DCAN_RX_ERR_CNTR                     (DCAN_ERRC_REC)
/* Macro used to return the status of RP field of DCAN_ERRC register */
#define DCAN_RX_ERR_PASSIVE                  (DCAN_ERRC_RP)

/******************************************************************************/
/*
** Values that can be passed to 'DCANIntRegStatusGet' API as 'intLnFlag'.
** 'intLnFlag' can take more than one value.
** For e.g. (DCAN_INT_LINE0_STAT | DCAN_INT_LINE1_STAT) can be passed as 
** 'intLnFlag'.
*/
/* Macro used to return the status of INT0ID[15:0] field of DCAN_INT register */
#define DCAN_INT_LINE0_STAT                  (DCAN_INT_INT0ID)
/* Macro used to return the status of INT1ID[23:16] field of DCAN_INT register */
#define DCAN_INT_LINE1_STAT                  (DCAN_INT_INT1ID)

/******************************************************************************/
/*
** Values that can be passed to 'DCANTestModesEnable' and 'DCANTestModesDisable'
** API as 'tstMode'.
*/
/* Macro that can be used to enable/disable silent mode */
#define DCAN_TST_SILENT_MD                   (DCAN_TEST_SILENT)
/* Macro that can be used to enable/disable loopback mode */
#define DCAN_TST_LPBCK_MD                    (DCAN_TEST_LBACK)
/* Macro that can be used to enable/disable external loopback mode */
#define DCAN_TST_EXTLPBCK_MD                 (DCAN_TEST_EXL)
/* Macro that can be used to enable/disable loopback with silent mode */
#define DCAN_TST_LPBCK_SILENT_MD             (DCAN_TEST_LBACK | \
                                              DCAN_TEST_SILENT)
/* Macro that can be used to enable/disable direct access to RAM */
#define DCAN_TST_RAM_DIRECT_ACCESS           (DCAN_TEST_RDA)

/******************************************************************************/
/*
** Values that can be passed to 'DCANTxPinControl' API as 'pinCtl'.
*/
/* CAN_TX pin is controlled by CAN core */
#define DCAN_TST_TX_NRML_OP                 (DCAN_TEST_TX_NORMAL)
/* Sample point can be monitored at CAN_TX pin */
#define DCAN_TST_TX_SAMPLE_PT_MNTR          (DCAN_TEST_TX_SAMPLEPOINT << \
                                             DCAN_TEST_TX_SHIFT)
/* CAN_TX pin drives a dominant value */
#define DCAN_TST_TX_DRIV_DOM_VAL            (DCAN_TEST_TX_DOMINANT << \
                                             DCAN_TEST_TX_SHIFT)
/* CAN_TX pin drives a recessive value */
#define DCAN_TST_TX_DRIV_RSV_VAL            (DCAN_TEST_TX_RECESSIVE << \
                                             DCAN_TEST_TX_SHIFT)

/******************************************************************************/
/*
** Values that can be used to check the status received from 'DCANRxPinStatusGet'
** API.
*/
/* CAN bus is dominant */
#define DCAN_TST_RX_IS_DOM                  (DCAN_TEST_RX_DOMINANT)
/* CAN bus is recessive */
#define DCAN_TST_RX_IS_RSV                  (DCAN_TEST_RX)

/******************************************************************************/
/*
** Values that can be used to check the status received from 
** 'DCANParityErrCdRegStatusGet' API.
*/
/* Macro used to return the message number status from DCAN_PERR register */
#define DCAN_PARITY_ERR_MSG_NUM             (DCAN_PERR_MESSAGE_NUMBER) 
/* Macro used to return the word number status from DCAN_PERR register */
#define DCAN_PARITY_ERR_WRD_NUM             (DCAN_PERR_WORD_NUMBER)

/******************************************************************************/
/*
** Value that can be used while checking the status obtained from 
** 'DCANTxRqstXStatusGet' API.
** 'n' can take these values
** 1 <= n <= 8.
*/
/* Value that can be used to check the status of DCAN_TXRQST_X register */
#define DCAN_TXRQST_X_REG(n)                (DCAN_TXRQ_X_TXRQSTREG1 <<  \
                                            (((n) - 1) * 2))

/******************************************************************************/
/*
** Value that can be used while checking the status obtained from 
** 'DCANNewDataXStatusGet' API.
** 'n' can take these values
** 1 <= n <= 8.
*/
/* Value that can be used to check the status of DCAN_NWDAT_X register */
#define DCAN_NEWDAT_X_REG(n)                (DCAN_NWDAT_X_NEWDATREG1 << \
                                            (((n) - 1) * 2))

/******************************************************************************/
/*
** Value that can be used while checking the status obtained from
** 'DCANMsgValidXStatusGet' API.
** 'n' can take these values
** 1 <= n <= 8.
*/
/* Value that can be used to check the status of DCAN_MSGVAL_X register */
#define DCAN_MSGVAL_X_REG(n)                (DCAN_MSGVAL_X_MSGVALREG1 << \
                                            (((n) - 1) * 2))

/******************************************************************************/
/*
** Value that can be used while checking the status obtained from
** 'DCANIntPendingXStatusGet' API.
** 'n' can take these values
** 1 <= n <= 8.
*/
/* Value that can be used to check the status of DCAN_INTPND_X register */
#define DCAN_INTPND_X_REG(n)                (DCAN_INTPND_X_INTPNDREG1 << \
                                            (((n) - 1) * 2))

/******************************************************************************/
/*
** Values can be used to check the status obtained from 'DCANIFBusyStatusGet'
** API.
*/
/* Transfer between IF1/IF2 register set and message RAM is in progress */
#define DCAN_IF_BUSY                        (DCAN_IFCMD_BUSY)
/* No transfer between IF1/IF2 register set and message RAM */
#define DCAN_IF_NOT_BUSY                    (DCAN_IFCMD_BUSY_NOTRANSFER)

/******************************************************************************/
/*
** Values that can be sent to 'DCANMsgIdSet' API as 'idLength'.
*/
/* Identifier of 11 bit size is used */
#define DCAN_11_BIT_ID                      (DCAN_IFARB_XTD_11_BIT)
/* Identifier of 29 bit size is used */
#define DCAN_29_BIT_ID                      (DCAN_IFARB_XTD)

/******************************************************************************/
/*
** Values that can be sent to 'DCANIntMuxConfig' API as 'intLine'
*/
/* DCANINT0 line is active if corresponding IntPnd flag is one */
#define DCAN_INT0_ACTIVE                    (0x00000000u)
/* DCANINT1 line is active if corresponding IntPnd flag is one */
#define DCAN_INT1_ACTIVE                    (0x00000001u) 
                                             
/******************************************************************************/
/*
** Values that can be sent to 'DCANMsgObjValidate', 'DCANMsgObjInvalidate, 
** 'DCANCommandRegSet', 'DCANIFBusyStatusGet, 'DCANMsgIdSet', 
** 'DCANMsgDirectionSet', 'DCANDataWrite', 'DCANDataRead', 'DCANDataLengthCodeSet',
** 'DCANRemoteEnableControl', 'DCANMsgObjIntEnable', 'DCANMsgObjIntDisable', 
** 'DCANFIFOEndOfBlockControl', 'DCANMsgObjectDirectionConfig', 
** 'DCANMsgObjectMskConfig', APIs as ''regNum'.
*/
/* Interface register 1 is used */
#define DCAN_IF1_REG                        (1u)
/* Interface register 2 is used */
#define DCAN_IF2_REG                        (2u)
/* Interface register 3 is used */
#define DCAN_IF3_REG                        (3u)

/******************************************************************************/
/*
** Values that can be sent to 'DCANMsgDirectionSet' API as 'msgDir'.
*/
/* Message object set to transmit a message*/
#define DCAN_TX_DIR                         (DCAN_IFARB_DIR)
/* Message object set to receive a message*/
#define DCAN_RX_DIR                         (DCAN_IFARB_DIR_RECEIVE)

/******************************************************************************/
/*
** Values that can be sent to 'DCANRemoteEnableControl' API as 'remEnable'.
*/
/* At the reception of a remote frame TxRqst is set */
#define DCAN_REMOTE_ENABLE                  (DCAN_IFMCTL_RMTEN)
/* At the reception of a remote frame TxRqst is not changed */
#define DCAN_REMOTE_DISABLE                 (DCAN_IFMCTL_RMTEN_DISABLE)

/******************************************************************************/
/*
** Values that can be sent to 'DCANMsgObjIntEnable' and 'DCANMsgObjIntDisable' 
** APIs as 'intFlags'.
*/
/* Enable/disable transmit interrupt */
#define DCAN_TRANSMIT_INT            (DCAN_IFMCTL_TXIE)
/* Enable/disable receive interrupt */
#define DCAN_RECEIVE_INT            (DCAN_IFMCTL_RXIE)

/******************************************************************************/
/*
** Values that can be sent to 'DCANFIFOEndOfBlockControl' API as 'eob'.
*/
/* Enable end of block */
#define DCAN_END_OF_BLOCK_ENABLE            (DCAN_IFMCTL_EOB)
/* Disable end of block */
#define DCAN_END_OF_BLOCK_DISABLE           (0x00000000u)

/******************************************************************************/
/*
** Values that can be passed to 'DCANCommandRegSet' API as 'cmdFlags'.
** 'cmdFlags' can be passed by combining more than one macro.
** For e.g. (DCAN_DMA_ACTIVE | DCAN_DAT_A_ACCESS | DCAN_DAT_B_ACCESS)
*/
/* Enable DMA feature */
#define DCAN_DMA_ACTIVE                     (DCAN_IFCMD_DMAACTIVE)
/* Access data from IF DataA register */
#define DCAN_DAT_A_ACCESS                   (DCAN_IFCMD_DATAA)
/* Access data from IF DataB register */
#define DCAN_DAT_B_ACCESS                   (DCAN_IFCMD_DATAB) 
/* Access the TxRqst bit */
#define DCAN_TXRQST_ACCESS                  (DCAN_IFCMD_TXRQST_NEWDAT)
/* Clear the IntPnd bit */
#define DCAN_CLR_INTPND                     (DCAN_IFCMD_CLRINTPND)
/* Access control bits */
#define DCAN_ACCESS_CTL_BITS                (DCAN_IFCMD_CONTROL)
/* Access Arbitration bits */
#define DCAN_ACCESS_ARB_BITS                (DCAN_IFCMD_ARB)
/* Access the mask bits */
#define DCAN_ACCESS_MSK_BITS                (DCAN_IFCMD_MASK)
/* Transfer direction is from IF registers to message RAM */
#define DCAN_MSG_WRITE                      (DCAN_IFCMD_WR_RD)
/* Transfer direction is from message RAM to IF registers */
#define DCAN_MSG_READ                       (0x00000000u)

/******************************************************************************/
/*
** Values that can be passed to 'DCAN_IDENTIFIER_MSK' macro as idType.
*/
/* 11 bit id is used */
#define DCAN_ID_MSK_11_BIT                  (18u)
/* 29 bit id is used */
#define DCAN_ID_MSK_29_BIT                  (0u)

/******************************************************************************/
/*
** Value that can be passed to 'DCANMsgObjectMskConfig' API as 'idMsk'.
** 'mask' can take values between 0 <= idMsk <= 0x1FFFFFFF \n
** 'idType' can take the following values \n
** DCAN_ID_MSK_11_BIT - 11 bit identifier is used \n
** DCAN_ID_MSK_29_BIT - 29 bit identifier is used \n 
*/
#define DCAN_IDENTIFIER_MSK(mask, idType)  (mask << idType)

/******************************************************************************/
/*
** Values that can be passed to 'DCANMsgObjectMskConfig' API as 'msgDir'.
*/
/* Message direction bit is used for acceptance filtering */
#define DCAN_MSK_MSGDIR_ENABLE              (DCAN_IFMSK_MDIR)
/* Message direction bit has no effect on acceptance filtering */
#define DCAN_MSK_MSGDIR_DISABLE             (DCAN_IFMSK_MDIR_NOTUSED)

/******************************************************************************/
/*
** Values that can be passed to 'DCANMsgObjectMskConfig' API as 'extId'.
*/
/* The IDE bit is used for acceptance filtering */
#define DCAN_MSK_EXT_ID_ENABLE              (DCAN_IFMSK_MXTD)
/* The IDE bit is not used for acceptance filtering */
#define DCAN_MSK_EXT_ID_DISABLE             (DCAN_IFMSK_MXTD_NOTUSED)

/******************************************************************************/
/*
** Values that can be passed to 'DCANIF3ObservationFlagSet' API as 'obsFlags'.
** 'obsFlags' can take more than 1 value.
** For e.g. (DCAN_MASK_DATA | DCAN_ARB_DATA)
*/
/* Mask data read observation */
#define DCAN_MASK_DATA                      (DCAN_IF3OBS_MASK)
/* Arbitration data read observation */
#define DCAN_ARB_DATA                       (DCAN_IF3OBS_ARB)
/* Ctrl read observation */
#define DCAN_CTRL_DATA                      (DCAN_IF3OBS_CTRL) 
/* Data A read observation */
#define DCAN_DAT_A_DATA                     (DCAN_IF3OBS_DATAA)
/* Data B read observation */
#define DCAN_DAT_B_DATA                     (DCAN_IF3OBS_DATAB)

/******************************************************************************/
/*
** Values that can be used to check the status obtained from 
** 'DCANIF3ObservationFlagStatGet' API.
*/
/* IF3 status of Mask data read access */
#define DCAN_IF3_MASK_STATUS                (DCAN_IF3OBS_IF3SM)
/* IF3 status of Arbitration data read access */
#define DCAN_IF3_ARB_STATUS                 (DCAN_IF3OBS_IF3SA)
/* IF3 status of Control bits read access */
#define DCAN_IF3_CTRL_STATUS                (DCAN_IF3OBS_IF3SC)
/* IF3 status of Data A read access */
#define DCAN_IF3_DAT_A_STATUS               (DCAN_IF3OBS_IF3SDA)
/* IF3 status of Data B read access */
#define DCAN_IF3_DAT_B_STATUS               (DCAN_IF3OBS_IF3SDB)
/* IF3 Update data status */
#define DCAN_IF3_UPDATE_STATUS              (DCAN_IF3OBS_IF3UPD)

/******************************************************************************/
/*
** Values that can be used to check the status obtained from 
** 'DCANIF3MaskStatusGet' API.
*/
/* Read Identifier mask */
#define DCAN_ID_MSK_READ                    (DCAN_IF3MSK_MSK)
/* Read mask message direction */
#define DCAN_MSK_MSG_DIR_READ               (DCAN_IF3MSK_MDIR)
/* Read mask extended identifier */
#define DCAN_MSK_EXT_ID_READ                (DCAN_IF3MSK_MXTD)

/******************************************************************************/
/*
** Values that can be used to check the status obtained from 'DCANIF3ArbStatusGet'
** API.
*/
/* Read message identifier */
#define DCAN_MSG_ID_READ                    (DCAN_IF3ARB_MSK)
/* Read message direction */
#define DCAN_MSG_DIR_READ                   (DCAN_IF3ARB_DIR)
/* Read extended identifier */
#define DCAN_EXT_ID_READ                    (DCAN_IF3ARB_XTD)
/* Read message valid status */
#define DCAN_MSGVAL_READ                    (DCAN_IF3ARB_MSGVAL)

/******************************************************************************/
/*
** Values that can be used to check the status obtained from 
** 'DCANIFMsgCtlStatusGet' API.
*/
/* Read data length code */
#define DCAN_DAT_LEN_CODE_READ              (DCAN_IF3MCTL_DATALENGTHCODE)
/* Read end of block bit */
#define DCAN_END_OF_BLOCK_READ              (DCAN_IF3MCTL_EOB)
/* Read transmit request bit */
#define DCAN_TXRQST_READ                    (DCAN_IF3MCTL_TXRQST)
/* Read remote enable bit */
#define DCAN_RMT_ENABLE_READ                (DCAN_IF3MCTL_RMTEN)
/* Read Rx interrupt enable bit */
#define DCAN_RX_INT_ENABLE_READ             (DCAN_IF3MCTL_RXIE)
/* Read Tx interrupt enable bit */
#define DCAN_TX_INT_ENABLE_READ             (DCAN_IF3MCTL_TXIE)
/* Read use acceptance mask bit */
#define DCAN_UMASK_READ                     (DCAN_IF3MCTL_UMASK)
/* Read interrupt pending status */
#define DCAN_INTPND_READ                    (DCAN_IF3MCTL_INTPND)
/* Read message lost status */
#define DCAN_MSG_LOST_READ                  (DCAN_IF3MCTL_MSGLST)
/* Read new data status */
#define DCAN_NEWDAT_READ                    (DCAN_IF3MCTL_NEWDAT)

/******************************************************************************/
/*
** Values that can be passed to 'DCANTxPinModeConfig' API as 'txPinMode'.
*/
/* DCAN Tx Pin is used as general purpose pin */
#define DCAN_TX_PIN_GIO_MODE                (0x00000000u)
/* DCAN Tx Pin is used as functional pin */
#define DCAN_TX_PIN_FUNC_MODE               (0x00000008u)

/******************************************************************************/
/*
** Values that can be passed to 'DCANTxPinInputDirConfig' API as 'pinLogic'.
*/
/* DCAN Tx pin is used as an input pin with logic low */
#define DCAN_TX_PIN_IN_LOGIC_LOW            (0x00000000u)
/* DCAN Tx pin is used as an input pin with logic high */
#define DCAN_TX_PIN_IN_LOGIC_HIGH           (0x00000001u)

/******************************************************************************/
/*
** Values that can be passed to 'DCANTxPinOutputDirConfig' API as 'pinLogic'.
*/
/* DCAN Tx pin is used as an output pin with logic low */
#define DCAN_TX_PIN_OUT_LOGIC_LOW           (0x00000000u)
/* DCAN Tx pin is used as an output pin with logic high */
#define DCAN_TX_PIN_OUT_LOGIC_HIGH          (0x00000002u)

/******************************************************************************/
/*
** Values that can be passed to 'DCANRxPinModeConfig' API as 'rxPinMode'.
*/
/* DCAN Rx Pin is used as general purpose pin */
#define DCAN_RX_PIN_GIO_MODE                (0x00000000u)
/* DCAN Rx Pin is used as functional pin */
#define DCAN_RX_PIN_FUNC_MODE               (0x00000008u)

/******************************************************************************/
/*
** Values that can be passed to 'DCANRxPinInputDirConfig' API as 'pinLogic'.
*/
/* DCAN Rx pin is used as an input pin with logic low */
#define DCAN_RX_PIN_IN_LOGIC_LOW            (0x00000000u)
/* DCAN Rx pin is used as an input pin with logic high */
#define DCAN_RX_PIN_IN_LOGIC_HIGH           (0x00000001u)

/******************************************************************************/
/*
** Values that can be passed to 'DCANRxPinOutputDirConfig' API as 'pinLogic'.
*/
/* DCAN Rx pin is used as an output pin with logic low */
#define DCAN_RX_PIN_OUT_LOGIC_LOW           (0x00000000u)
/* DCAN Rx pin is used as an output pin with logic high */
#define DCAN_RX_PIN_OUT_LOGIC_HIGH          (0x00000002u)

/******************************************************************************/
/*                        DCAN API PROTOTYPES                                 */
/******************************************************************************/
extern void DCANInitModeSet(unsigned int baseAdd);
extern void DCANNormalModeSet(unsigned int baseAdd);
extern void DCANBitTimingConfig(unsigned int baseAdd, unsigned int clkFreq,
                                unsigned int tmQnta, unsigned int bitTime);
extern void DCANReset(unsigned int baseAdd);
extern void DCANIntEnable(unsigned int baseAdd, unsigned int intFlags);
extern void DCANIntDisable(unsigned int baseAdd, unsigned int intFlags);
extern void DCANAutoReTransmitControl(unsigned int baseAdd, unsigned int autoReTxn);
extern void DCANConfigRegWriteAccessControl(unsigned int baseAdd,
                                     unsigned int regConfig);
extern void DCANTestModeControl(unsigned int baseAdd, unsigned int testMode);
extern void DCANDebugSuspendModeConfig(unsigned int baseAdd, unsigned int modeConfig);
extern void DCANAutoBusOnControl(unsigned int baseAdd, unsigned int busControl);
extern void DCANParityControl(unsigned int baseAdd, unsigned int paritySet);
extern unsigned int DCANInternalDebugStatusGet(unsigned int baseAdd);
extern void DCANIntLineEnable(unsigned int baseAdd, unsigned int enableLine);
extern void DCANIntLineDisable(unsigned int baseAdd, unsigned int disableLine);
extern void DCANDmaRequestLineEnable(unsigned int baseAdd, unsigned int ifRegFlags);
extern void DCANDmaRequestLineDisable(unsigned int baseAdd, unsigned int ifRegFlags);
extern void DCANPwrDwnControl(unsigned int baseAdd, unsigned int pwrDwnCtl);
extern void DCANPwrDwnWkUpControl(unsigned int baseAdd, unsigned int wkUpCtl);
extern unsigned int DCANIntRegStatusGet(unsigned int baseAdd, unsigned int intLnFlag);
extern unsigned int DCANErrAndStatusRegInfoGet(unsigned int baseAdd);
extern void DCANParityEndOfIntSet(unsigned int baseAdd);
extern unsigned int DCANErrCntrRegStatusGet(unsigned int baseAdd,
                                     unsigned int cntrFlags);
extern void DCANTestModesEnable(unsigned int baseAdd, unsigned int tstMode);
extern void DCANTestModesDisable(unsigned int baseAdd, unsigned int tstMode);
extern void DCANTxPinControl(unsigned int baseAdd, unsigned int pinCtl);
extern unsigned int DCANRxPinStatusGet(unsigned int baseAdd);
extern unsigned int DCANParityErrCdRegStatusGet(unsigned int baseAdd,
                                         unsigned int statFlg);
extern void DCANAutoBusOnTimeValSet(unsigned int baseAdd, unsigned int timeVal);
extern unsigned int DCANAutoBusOnTimeValGet(unsigned int baseAdd);
extern unsigned int DCANTxRqstXStatusGet(unsigned int baseAdd);
extern unsigned int DCANTxRqstStatusGet(unsigned int baseAdd, unsigned int msgNum);
extern unsigned int DCANTxRqstStatGet(unsigned int baseAdd);
extern unsigned int DCANNewDataXStatusGet(unsigned int baseAdd);
extern unsigned int DCANNewDataStatusGet(unsigned int baseAdd, unsigned int msgNum);
extern unsigned int DCANNewDataStatGet(unsigned int baseAdd);
extern void DCANMsgObjValidate(unsigned int baseAdd, unsigned int regNum);
extern void DCANMsgObjInvalidate(unsigned baseAdd, unsigned int regNum);
extern void DCANCommandRegSet(unsigned int baseAdd, unsigned int cmdFlags,
                              unsigned int objNum, unsigned int regNum);
extern unsigned int DCANIntPendingXStatusGet(unsigned int baseAdd);
extern unsigned int DCANIntPendingStatusGet(unsigned int baseAdd, unsigned int msgNum);
extern unsigned int DCANMsgValidXStatusGet(unsigned int baseAdd);
extern unsigned int DCANMsgValidStatusGet(unsigned int baseAdd, unsigned int msgNum);
extern void DCANIntMuxConfig(unsigned int baseAdd, unsigned int intLine, 
                             unsigned int msgNum);
extern unsigned int DCANIFBusyStatusGet(unsigned int baseAdd, unsigned int regNum);
extern void DCANMsgIdSet(unsigned int baseAdd, unsigned int msgId,
                         unsigned int idLength, unsigned int regNum);
extern void DCANMsgDirectionSet(unsigned int baseAdd, unsigned int msgDir,
                         unsigned int regNum);
extern void DCANDataWrite(unsigned int baseAdd, unsigned int* dataPtr,
                   unsigned int regNum);
extern void DCANDataRead(unsigned int baseAdd, unsigned int* data, unsigned int regNum);
extern void DCANDataLengthCodeSet(unsigned int baseAdd, unsigned int dlc,
                           unsigned int regNum);
extern void DCANRemoteEnableControl(unsigned int baseAdd, unsigned int remEnable,
                            unsigned int regNum);
extern void DCANMsgObjIntEnable(unsigned int baseAdd, unsigned int intFlags,
                         unsigned int regNum);
extern void DCANMsgObjIntDisable(unsigned int baseAdd, unsigned int intFlags,
                          unsigned int regNum);
extern void DCANFIFOEndOfBlockControl(unsigned int baseAdd, unsigned int eob,
                              unsigned int regNum);
extern void DCANMsgObjectMskConfig(unsigned int baseAdd, unsigned int idMsk,
                            unsigned int msgDir, unsigned int extId,
                            unsigned int regNum);
extern void DCANIF3RegUpdateEnableSet(unsigned int baseAdd,
                                       unsigned int msgNum);
extern unsigned char DCANIF3ObservationFlagStatGet(unsigned int baseAdd);
extern void DCANIF3ObservationFlagSet(unsigned int baseAdd, unsigned int obsFlags);
extern void DCANIF3ObservationFlagClear(unsigned int baseAdd, unsigned int obsFlags);
extern unsigned int DCANIF3MaskStatusGet(unsigned int baseAdd);
extern unsigned int DCANIF3ArbStatusGet(unsigned int baseAdd);
extern unsigned int DCANIFMsgCtlStatusGet(unsigned int baseAdd, unsigned int regNum); 
extern void DCANTxPinModeConfig(unsigned int baseAdd, unsigned int txPinMode);
extern void DCANTxPinInputDirConfig(unsigned int baseAdd, unsigned int pinLogic);
extern void DCANTxPinOutputDirConfig(unsigned int baseAdd, unsigned int pinLogic);
extern void DCANRxPinModeConfig(unsigned int baseAdd, unsigned int rxPinMode);
extern void DCANRxPinInputDirConfig(unsigned int baseAdd, unsigned int pinLogic);
extern void DCANRxPinOutputDirConfig(unsigned int baseAdd, unsigned int pinLogic);


#ifdef __cplusplus
}
#endif

#endif
