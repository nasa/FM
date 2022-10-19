/************************************************************************
 * NASA Docket No. GSC-18,918-1, and identified as “Core Flight
 * Software System (cFS) File Manager Application Version 2.6.1”
 *
 * Copyright (c) 2021 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/**
 * @file
 *  Coverage Unit Test cases for the fm_app implementations
 */

/*
 * Includes
 */
/*
 * UT includes
 */
#include "uttest.h"
#include "utassert.h"
#include "utstubs.h"

/* fm testing */
#include "cfe.h"
#include "fm_msg.h"
#include "fm_msgdefs.h"
#include "fm_msgids.h"
#include "fm_app.h"
#include "fm_tbl.h"
#include "fm_child.h"
#include "fm_cmds.h"
#include "fm_cmd_utils.h"
#include "fm_events.h"
#include "fm_perfids.h"
#include "fm_platform_cfg.h"
#include "fm_version.h"
#include "fm_verify.h"
#include "fm_app.h"
#include "fm_test_utils.h"
#include <unistd.h>
#include <stdlib.h>
#include "cfe.h"

/*********************************************************************************
 *          TEST CASE FUNCTIONS
 *********************************************************************************/

/* ********************************
 * AppMain Tests
 * ********************************/
void Test_FM_AppMain_AppInitNotSuccess(void)
{
    /* Arrange */
    UT_SetDefaultReturnValue(UT_KEY(CFE_EVS_Register), !CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_RunLoop), false);

    /* Act */
    UtAssert_VOIDCALL(FM_AppMain());

    /* Assert */
    UtAssert_STUB_COUNT(CFE_ES_RunLoop, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_STUB_COUNT(CFE_ES_ExitApp, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_STARTUP_EVENTS_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_EXIT_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_ERROR);
}

void Test_FM_AppMain_SBReceiveBufferDefaultOption(void)
{
    /* Arrange */
    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_RunLoop), true);
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 2, false);
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_ReceiveBuffer), -1);

    /* Act */
    UtAssert_VOIDCALL(FM_AppMain());

    /* Assert */
    UtAssert_STUB_COUNT(CFE_ES_RunLoop, 2);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 3);
    UtAssert_STUB_COUNT(CFE_ES_ExitApp, 1);
    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_STARTUP_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_SB_RECEIVE_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[2].EventID, FM_EXIT_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[2].EventType, CFE_EVS_EventType_ERROR);
}

void Test_FM_AppMain_SBReceiveBufferIsTimeOut(void)
{
    /* Arrange */
    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_RunLoop), true);
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 2, false);
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_ReceiveBuffer), CFE_SB_TIME_OUT);

    /* Act */
    UtAssert_VOIDCALL(FM_AppMain());

    /* Assert */
    UtAssert_STUB_COUNT(CFE_ES_RunLoop, 2);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_STUB_COUNT(CFE_ES_ExitApp, 1);
    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 1);
    UtAssert_STUB_COUNT(FM_ReleaseTablePointers, 1);
    UtAssert_STUB_COUNT(FM_AcquireTablePointers, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_EXIT_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_ERROR);
}

void Test_FM_AppMain_ReceiveBufferSuccessBufPtrIsNull(void)
{
    /* Arrange */
    CFE_SB_Buffer_t *sbbufptr = NULL;

    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_RunLoop), true);
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 2, false);
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_ReceiveBuffer), CFE_SUCCESS);
    UT_SetDataBuffer(UT_KEY(CFE_SB_ReceiveBuffer), &sbbufptr, sizeof(sbbufptr), false);

    /* Act */
    UtAssert_VOIDCALL(FM_AppMain());

    /* Assert */
    UtAssert_STUB_COUNT(CFE_ES_RunLoop, 2);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 3);
    UtAssert_STUB_COUNT(CFE_ES_ExitApp, 1);
    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_SB_RECEIVE_NULL_PTR_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[2].EventID, FM_EXIT_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[2].EventType, CFE_EVS_EventType_ERROR);
}

void Test_FM_AppMain_BufPtrNotEqualNull(void)
{
    /* Arrange */
    CFE_SB_Buffer_t  sbbuf;
    CFE_SB_Buffer_t *sbbufptr = &sbbuf;
    CFE_SB_MsgId_t   msgid    = CFE_SB_INVALID_MSG_ID;

    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_RunLoop), true);
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 2, false);
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_ReceiveBuffer), CFE_SUCCESS);
    UT_SetDataBuffer(UT_KEY(CFE_SB_ReceiveBuffer), &sbbufptr, sizeof(sbbufptr), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &msgid, sizeof(msgid), false);

    /* Act */
    UtAssert_VOIDCALL(FM_AppMain());

    /* Assert */
    UtAssert_STUB_COUNT(CFE_ES_RunLoop, 2);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 3);
    UtAssert_STUB_COUNT(CFE_ES_ExitApp, 1);
    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_MID_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[2].EventID, FM_EXIT_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[2].EventType, CFE_EVS_EventType_ERROR);
}

/* ********************************
 * ProcessPkt Tests
 * ********************************/
void Test_FM_ProcessPkt_CheckMessageReturnHKRequest(void)
{
    /* Arrange */
    CFE_SB_MsgId_t msgid = CFE_SB_ValueToMsgId(FM_SEND_HK_MID);

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &msgid, sizeof(msgid), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), false);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessPkt(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(FM_IsValidCmdPktLength, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_ProcessPkt_CheckMessageReturnGroundCommand(void)
{
    /* Arrange */
    CFE_SB_MsgId_t    msgid    = CFE_SB_ValueToMsgId(FM_CMD_MID);
    CFE_MSG_FcnCode_t fcn_code = -1;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &msgid, sizeof(msgid), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessPkt(NULL));

    /* Assert */
    UtAssert_INT32_EQ(FM_GlobalData.CommandErrCounter, 1);
    UtAssert_STUB_COUNT(CFE_MSG_GetFcnCode, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CC_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
}

void Test_FM_ProcessPkt_CheckDefaultSwitchMessage(void)
{
    /* Arrange */
    CFE_SB_MsgId_t msgid = CFE_SB_INVALID_MSG_ID;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &msgid, sizeof(msgid), false);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessPkt(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_MID_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
}

/* ********************************
 * AppInit Tests
 * *******************************/
void Test_FM_AppInit_EVSRegisterNotSuccess(void)
{
    /* Arrange */
    UT_SetDefaultReturnValue(UT_KEY(CFE_EVS_Register), !CFE_SUCCESS);

    /* Act */
    UtAssert_INT32_EQ(FM_AppInit(), !CFE_SUCCESS);

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_Register, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_STARTUP_EVENTS_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
}

void Test_FM_AppInit_CreatePipeFail(void)
{
    /* Arrange */
    UT_SetDefaultReturnValue(UT_KEY(CFE_EVS_Register), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_CreatePipe), !CFE_SUCCESS);

    /* Act */
    UtAssert_INT32_EQ(FM_AppInit(), !CFE_SUCCESS);

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_Register, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_SB_CreatePipe, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_STARTUP_CREAT_PIPE_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
}

void Test_FM_AppInit_HKSubscribeFail(void)
{
    /* Arrange */
    UT_SetDefaultReturnValue(UT_KEY(CFE_EVS_Register), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_CreatePipe), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_Subscribe), !CFE_SUCCESS);

    /* Act */
    UtAssert_INT32_EQ(FM_AppInit(), !CFE_SUCCESS);

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_Register, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_SB_CreatePipe, 1);
    UtAssert_STUB_COUNT(CFE_SB_Subscribe, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_STARTUP_SUBSCRIB_HK_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
}

void Test_FM_AppInit_GroundCmdSubscribeFail(void)
{
    /* Arrange */
    UT_SetDefaultReturnValue(UT_KEY(CFE_EVS_Register), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_CreatePipe), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_Subscribe), CFE_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_Subscribe), 2, !CFE_SUCCESS);

    /* Act */
    UtAssert_INT32_EQ(FM_AppInit(), !CFE_SUCCESS);

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_Register, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_SB_CreatePipe, 1);
    UtAssert_STUB_COUNT(CFE_SB_Subscribe, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_STARTUP_SUBSCRIB_GCMD_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
}

void Test_FM_AppInit_TableInitNotSuccess(void)
{
    /* Arrange */
    UT_SetDefaultReturnValue(UT_KEY(CFE_EVS_Register), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_CreatePipe), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_Subscribe), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(FM_TableInit), !CFE_SUCCESS);

    /* Act */
    UtAssert_INT32_EQ(FM_AppInit(), !CFE_SUCCESS);

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_Register, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_SB_CreatePipe, 1);
    UtAssert_STUB_COUNT(CFE_SB_Subscribe, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_STARTUP_TABLE_INIT_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
}

void Test_FM_AppInit_TableInitSuccess(void)
{
    /* Arrange */
    UT_SetDefaultReturnValue(UT_KEY(CFE_EVS_Register), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_CreatePipe), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_Subscribe), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(FM_TableInit), CFE_SUCCESS);

    /* Act */
    UtAssert_INT32_EQ(FM_AppInit(), CFE_SUCCESS);

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_Register, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_SB_CreatePipe, 1);
    UtAssert_STUB_COUNT(CFE_SB_Subscribe, 2);
    UtAssert_STUB_COUNT(FM_ChildInit, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_STARTUP_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
}

/* ********************************
 * Report HK Tests
 * *******************************/
void Test_FM_ReportHK_ReturnPktLengthTrue(void)
{
    /* Arrange */
    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_GetOpenFilesData), 0);

    /* Set non-zero values to assert */
    FM_GlobalData.CommandCounter      = 1;
    FM_GlobalData.CommandErrCounter   = 2;
    FM_GlobalData.ChildCmdCounter     = 3;
    FM_GlobalData.ChildCmdErrCounter  = 4;
    FM_GlobalData.ChildCmdWarnCounter = 5;
    FM_GlobalData.ChildQueueCount     = 6;
    FM_GlobalData.ChildCurrentCC      = 7;
    FM_GlobalData.ChildPreviousCC     = 8;

    /* Act */
    UtAssert_VOIDCALL(FM_ReportHK(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(FM_IsValidCmdPktLength, 1);
    UtAssert_STUB_COUNT(FM_ReleaseTablePointers, 1);
    UtAssert_STUB_COUNT(FM_AcquireTablePointers, 1);
    UtAssert_STUB_COUNT(CFE_MSG_Init, 1);
    UtAssert_STUB_COUNT(FM_GetOpenFilesData, 1);
    UtAssert_STUB_COUNT(CFE_SB_TimeStampMsg, 1);
    UtAssert_STUB_COUNT(CFE_SB_TransmitMsg, 1);
    UtAssert_INT32_EQ(FM_GlobalData.HousekeepingPkt.CommandCounter, FM_GlobalData.CommandCounter);
    UtAssert_INT32_EQ(FM_GlobalData.HousekeepingPkt.CommandErrCounter, FM_GlobalData.CommandErrCounter);
    UtAssert_INT32_EQ(FM_GlobalData.HousekeepingPkt.NumOpenFiles, 0);
    UtAssert_INT32_EQ(FM_GlobalData.HousekeepingPkt.ChildCmdCounter, FM_GlobalData.ChildCmdCounter);
    UtAssert_INT32_EQ(FM_GlobalData.HousekeepingPkt.ChildCmdErrCounter, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(FM_GlobalData.HousekeepingPkt.ChildCmdWarnCounter, FM_GlobalData.ChildCmdWarnCounter);
    UtAssert_INT32_EQ(FM_GlobalData.HousekeepingPkt.ChildQueueCount, FM_GlobalData.ChildQueueCount);
    UtAssert_INT32_EQ(FM_GlobalData.HousekeepingPkt.ChildCurrentCC, FM_GlobalData.ChildCurrentCC);
    UtAssert_INT32_EQ(FM_GlobalData.HousekeepingPkt.ChildPreviousCC, FM_GlobalData.ChildPreviousCC);
}

void Test_FM_ReportHK_ReturnPktLengthFalse(void)
{
    /* Arrange */
    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), false);

    /* Act */
    UtAssert_VOIDCALL(FM_ReportHK(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(FM_ReleaseTablePointers, 0);
    UtAssert_STUB_COUNT(CFE_SB_TransmitMsg, 0);
}

/* ********************************
 * Process Command Tests
 * *******************************/
void Test_FM_ProcessCmd_NoopCmdCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code = FM_NOOP_CC;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_NoopCmd), true);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessCmd(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_NoopCmd, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandCounter, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandErrCounter, 0);
}

void Test_FM_ProcessCmd_ResetCountersCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code = FM_RESET_CC;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_ResetCountersCmd), true);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessCmd(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_ResetCountersCmd, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandCounter, 0);
    UtAssert_INT32_EQ(FM_GlobalData.CommandErrCounter, 0);
}

void Test_FM_ProcessCmd_CopyFileCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code = FM_COPY_CC;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_CopyFileCmd), true);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessCmd(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_CopyFileCmd, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandCounter, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandErrCounter, 0);
}

void Test_FM_ProcessCmd_MoveFileCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code = FM_MOVE_CC;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_MoveFileCmd), true);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessCmd(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_MoveFileCmd, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandCounter, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandErrCounter, 0);
}

void Test_FM_ProcessCmd_RenameFileCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code = FM_RENAME_CC;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_RenameFileCmd), true);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessCmd(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_RenameFileCmd, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandCounter, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandErrCounter, 0);
}

void Test_FM_ProcessCmd_DeleteFileCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code = FM_DELETE_CC;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_DeleteFileCmd), true);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessCmd(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_DeleteFileCmd, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandCounter, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandErrCounter, 0);
}

void Test_FM_ProcessCmd_DeleteAllFilesCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code = FM_DELETE_ALL_CC;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_DeleteAllFilesCmd), true);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessCmd(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_DeleteAllFilesCmd, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandCounter, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandErrCounter, 0);
}

#ifdef FM_INCLUDE_DECOMPRESS
void Test_FM_ProcessCmd_DecompressFileCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code = FM_DECOMPRESS_CC;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_DecompressFileCmd), true);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessCmd(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_DecompressFileCmd, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandCounter, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandErrCounter, 0);
}
#endif

void Test_FM_ProcessCmd_ConcatFilesCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code = FM_CONCAT_CC;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_ConcatFilesCmd), true);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessCmd(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_ConcatFilesCmd, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandCounter, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandErrCounter, 0);
}

void Test_FM_ProcessCmd_GetFileInfoCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code = FM_GET_FILE_INFO_CC;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_GetFileInfoCmd), true);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessCmd(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_GetFileInfoCmd, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandCounter, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandErrCounter, 0);
}

void Test_FM_ProcessCmd_GetOpenFilesCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code = FM_GET_OPEN_FILES_CC;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_GetOpenFilesCmd), true);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessCmd(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_GetOpenFilesCmd, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandCounter, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandErrCounter, 0);
}

void Test_FM_ProcessCmd_CreateDirectoryCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code = FM_CREATE_DIR_CC;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_CreateDirectoryCmd), true);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessCmd(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_CreateDirectoryCmd, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandCounter, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandErrCounter, 0);
}

void Test_FM_ProcessCmd_DeleteDirectoryCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code = FM_DELETE_DIR_CC;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_DeleteDirectoryCmd), true);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessCmd(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_DeleteDirectoryCmd, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandCounter, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandErrCounter, 0);
}

void Test_FM_ProcessCmd_GetDirListFileCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code = FM_GET_DIR_FILE_CC;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_GetDirListFileCmd), true);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessCmd(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_GetDirListFileCmd, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandCounter, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandErrCounter, 0);
}

void Test_FM_ProcessCmd_GetDirListPktCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code = FM_GET_DIR_PKT_CC;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_GetDirListPktCmd), true);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessCmd(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_GetDirListPktCmd, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandCounter, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandErrCounter, 0);
}

void Test_FM_ProcessCmd_GetFreeSpaceCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code = FM_GET_FREE_SPACE_CC;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_GetFreeSpaceCmd), true);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessCmd(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_GetFreeSpaceCmd, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandCounter, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandErrCounter, 0);
}

void Test_FM_ProcessCmd_SetTableStateCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code = FM_SET_TABLE_STATE_CC;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_SetTableStateCmd), true);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessCmd(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_SetTableStateCmd, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandCounter, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandErrCounter, 0);
}

void Test_FM_ProcessCmd_SetPermissionsCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code = FM_SET_FILE_PERM_CC;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_SetPermissionsCmd), true);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessCmd(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_SetPermissionsCmd, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandCounter, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandErrCounter, 0);
}

void Test_FM_ProcessCmd_DefaultReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code = -1;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessCmd(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandCounter, 0);
    UtAssert_INT32_EQ(FM_GlobalData.CommandErrCounter, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CC_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
}

/* * * * * * * * * * * * * *
 * Add Method Tests
 * * * * * * * * * * * * * */
void add_FM_AppMain_tests(void)
{
    UtTest_Add(Test_FM_AppMain_AppInitNotSuccess, FM_Test_Setup, FM_Test_Teardown, "Test_FM_AppMain_AppInitNotSuccess");

    UtTest_Add(Test_FM_AppMain_SBReceiveBufferDefaultOption, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_AppMain_SBReceiveBufferDefaultOption");

    UtTest_Add(Test_FM_AppMain_SBReceiveBufferIsTimeOut, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_AppMain_SBReceiveBufferIsTimeOut");

    UtTest_Add(Test_FM_AppMain_ReceiveBufferSuccessBufPtrIsNull, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_AppMain_ReceiveBufferSuccessBufPtrIsNull");

    UtTest_Add(Test_FM_AppMain_BufPtrNotEqualNull, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_AppMain_BufPtrNotEqualNull");
}

void add_FM_ProcessPkt_tests(void)
{
    UtTest_Add(Test_FM_ProcessPkt_CheckMessageReturnHKRequest, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ProcessPkt_ReportHK");

    UtTest_Add(Test_FM_ProcessPkt_CheckMessageReturnGroundCommand, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ProcessPkt_CheckMessageReturnGroundCommand");

    UtTest_Add(Test_FM_ProcessPkt_CheckDefaultSwitchMessage, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ProcessPkt_CheckDefaultSwitchMessage");
}

void add_FM_AppInit_tests(void)
{
    UtTest_Add(Test_FM_AppInit_EVSRegisterNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_AppInit_EVSRegisterNotSuccess");

    UtTest_Add(Test_FM_AppInit_CreatePipeFail, FM_Test_Setup, FM_Test_Teardown, "Test_FM_AppInit_CreatePipeFail");

    UtTest_Add(Test_FM_AppInit_HKSubscribeFail, FM_Test_Setup, FM_Test_Teardown, "Test_FM_AppInit_HKSubscribeFail");

    UtTest_Add(Test_FM_AppInit_GroundCmdSubscribeFail, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_AppInit_GroundCmdSubscribeFail");

    UtTest_Add(Test_FM_AppInit_TableInitNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_AppInit_TableInitNotSuccess");

    UtTest_Add(Test_FM_AppInit_TableInitSuccess, FM_Test_Setup, FM_Test_Teardown, "Test_FM_AppInit_TableInitSuccess");
}

/* * * * * * * * * * * * * *
 * Add Method Tests
 * * * * * * * * * * * * * */
void add_FM_ProcessCmd_tests(void)
{
    UtTest_Add(Test_FM_ProcessCmd_NoopCmdCCReturn, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ProcessCmd_NoopCmd_Return");

    UtTest_Add(Test_FM_ProcessCmd_ResetCountersCCReturn, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ProcessCmd_NResetCountersCCReturn");

    UtTest_Add(Test_FM_ProcessCmd_CopyFileCCReturn, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ProcessCmd_CopyFileCCReturn");

    UtTest_Add(Test_FM_ProcessCmd_MoveFileCCReturn, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ProcessCmd_MoveFileCCReturn");

    UtTest_Add(Test_FM_ProcessCmd_RenameFileCCReturn, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ProcessCmd_RenameFileCCReturn");

    UtTest_Add(Test_FM_ProcessCmd_DeleteFileCCReturn, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ProcessCmd_DeleteFileCCReturn");

    UtTest_Add(Test_FM_ProcessCmd_DeleteAllFilesCCReturn, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ProcessCmd_DeleteAllFilesCCReturn");
#ifdef FM_INCLUDE_DECOMPRESS
    UtTest_Add(Test_FM_ProcessCmd_DecompressFileCCReturn, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ProcessCmd_DecompressFileCCReturn");
#endif
    UtTest_Add(Test_FM_ProcessCmd_ConcatFilesCCReturn, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_PRocessCmd_ConcatFilesCCReturn");

    UtTest_Add(Test_FM_ProcessCmd_GetFileInfoCCReturn, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_PRocessCmd_GetFileInfoCCReturn");

    UtTest_Add(Test_FM_ProcessCmd_GetOpenFilesCCReturn, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_PRocessCmd_GetOpenFilesCCReturn");

    UtTest_Add(Test_FM_ProcessCmd_CreateDirectoryCCReturn, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_PRocessCmd_CreateDirectoryCCReturn");

    UtTest_Add(Test_FM_ProcessCmd_DeleteDirectoryCCReturn, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_PRocessCmd_DeleteDirectoryCCReturn");

    UtTest_Add(Test_FM_ProcessCmd_GetDirListFileCCReturn, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ProcessCmd_GetDirListFIleCCReturn");

    UtTest_Add(Test_FM_ProcessCmd_GetDirListPktCCReturn, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_PRocessCmd_GetDirListPktCCReturn");

    UtTest_Add(Test_FM_ProcessCmd_GetFreeSpaceCCReturn, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_PRocessCmd_GetFreeSpaceCCReturn");

    UtTest_Add(Test_FM_ProcessCmd_SetTableStateCCReturn, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_PRocessCmd_SetTableStateCCReturn");

    UtTest_Add(Test_FM_ProcessCmd_SetPermissionsCCReturn, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_PRocessCmd_SetPermissionsCCReturn");

    UtTest_Add(Test_FM_ProcessCmd_DefaultReturn, FM_Test_Setup, FM_Test_Teardown, "Test_FM_PRocessCmd_DefaultReturn");
}

void add_FM_ReportHK_tests(void)
{
    UtTest_Add(Test_FM_ReportHK_ReturnPktLengthTrue, FM_Test_Setup, FM_Test_Teardown, "Test_FM_ReportHK_Return");
    UtTest_Add(Test_FM_ReportHK_ReturnPktLengthFalse, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ReportHK_ReturnPktLengthFalse");
}

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    add_FM_ProcessPkt_tests();
    add_FM_AppInit_tests();
    add_FM_AppMain_tests();
    add_FM_ReportHK_tests();
    add_FM_ProcessCmd_tests();
}
