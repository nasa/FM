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

typedef struct
{
    uint16      ExpectedEvent;
    uint32      MatchCount;
    const char *ExpectedFormat;
} UT_CheckEvent_t;

/*
 * Helper function to set up for event checking
 * This attaches the hook function to CFE_EVS_SendEvent
 */
/*
static void UT_CheckEvent_Setup(UT_CheckEvent_t *Evt, uint16 ExpectedEvent, const char *ExpectedFormat)
{
    memset(Evt, 0, sizeof(*Evt));
    Evt->ExpectedEvent  = ExpectedEvent;
    Evt->ExpectedFormat = ExpectedFormat;
    UT_SetVaHookFunction(UT_KEY(CFE_EVS_SendEvent), UT_CheckEvent_Hook, Evt);
}
*/
/*
**********************************************************************************
**          TEST CASE FUNCTIONS
**********************************************************************************
*/

/* ********************************
 * AppMain Tests
 * ********************************/
void Test_FM_AppMain_AppInitNotSuccess(void)
{
    // Arrange
    UT_SetDefaultReturnValue(UT_KEY(CFE_EVS_Register), !CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_RunLoop), false);

    // Act
    FM_AppMain();

    uint8 count_runloop   = UT_GetStubCount(UT_KEY(CFE_ES_RunLoop));
    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_exitapp   = UT_GetStubCount(UT_KEY(CFE_ES_ExitApp));

    // Assert
    UtAssert_INT32_EQ(count_runloop, 1);
    UtAssert_INT32_EQ(count_sendevent, 2);
    UtAssert_INT32_EQ(count_exitapp, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_STARTUP_EVENTS_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_EXIT_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_ERROR);
}

void Test_FM_AppMain_SBReceiveBufferDefaultOption(void)
{
    // Arrange
    UT_SetDefaultReturnValue(UT_KEY(CFE_EVS_Register), !CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_RunLoop), true);
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 2, false);
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_ReceiveBuffer), -1);

    // Act
    FM_AppMain();

    uint8 count_runloop   = UT_GetStubCount(UT_KEY(CFE_ES_RunLoop));
    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_exitapp   = UT_GetStubCount(UT_KEY(CFE_ES_ExitApp));
    uint8 count_recbuff   = UT_GetStubCount(UT_KEY(CFE_SB_ReceiveBuffer));

    // Assert
    UtAssert_INT32_EQ(count_runloop, 2);
    UtAssert_INT32_EQ(count_sendevent, 3);
    UtAssert_INT32_EQ(count_exitapp, 1);
    UtAssert_INT32_EQ(count_recbuff, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_STARTUP_EVENTS_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[2].EventID, FM_EXIT_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[2].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_SB_RECEIVE_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_ERROR);
}

void Test_FM_AppMain_SBReceiveBufferIsTimeOut(void)
{
    // Arrange
    UT_SetDefaultReturnValue(UT_KEY(CFE_EVS_Register), !CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_RunLoop), true);
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 2, false);
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_ReceiveBuffer), CFE_SB_TIME_OUT);

    // Act
    FM_AppMain();

    uint8 count_runloop     = UT_GetStubCount(UT_KEY(CFE_ES_RunLoop));
    uint8 count_sendevent   = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_exitapp     = UT_GetStubCount(UT_KEY(CFE_ES_ExitApp));
    uint8 count_recbuff     = UT_GetStubCount(UT_KEY(CFE_SB_ReceiveBuffer));
    uint8 count_releaseptrs = UT_GetStubCount(UT_KEY(FM_ReleaseTablePointers));
    uint8 count_acquireptrs = UT_GetStubCount(UT_KEY(FM_AcquireTablePointers));

    // Assert
    UtAssert_INT32_EQ(count_runloop, 2);
    UtAssert_INT32_EQ(count_sendevent, 2);
    UtAssert_INT32_EQ(count_exitapp, 1);
    UtAssert_INT32_EQ(count_recbuff, 1);
    UtAssert_INT32_EQ(count_releaseptrs, 1);
    UtAssert_INT32_EQ(count_acquireptrs, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_EXIT_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_ERROR);
}

void Test_FM_AppMain_ReceiveBufferSuccessBufPtrIsNull(void)
{
    // Arrange
    CFE_SB_Buffer_t *dummy_buffer = NULL;
    UT_SetDefaultReturnValue(UT_KEY(CFE_EVS_Register), !CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_RunLoop), true);
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 2, false);
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_ReceiveBuffer), CFE_SUCCESS);
    UT_SetDataBuffer(UT_KEY(CFE_SB_ReceiveBuffer), &dummy_buffer, sizeof(dummy_buffer), false);

    // Act
    FM_AppMain();

    uint8 count_runloop   = UT_GetStubCount(UT_KEY(CFE_ES_RunLoop));
    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_exitapp   = UT_GetStubCount(UT_KEY(CFE_ES_ExitApp));
    uint8 count_recbuff   = UT_GetStubCount(UT_KEY(CFE_SB_ReceiveBuffer));

    // Assert
    UtAssert_INT32_EQ(count_runloop, 2);
    UtAssert_INT32_EQ(count_sendevent, 3);
    UtAssert_INT32_EQ(count_exitapp, 1);
    UtAssert_INT32_EQ(count_recbuff, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_SB_RECEIVE_NULL_PTR_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[2].EventID, FM_EXIT_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[2].EventType, CFE_EVS_EventType_ERROR);
}

void Test_FM_AppMain_BufPtrNotEqualNull(void)
{
    // Arrange
    CFE_SB_Buffer_t  dummy_buffer;
    CFE_SB_Buffer_t *DummyBufPtr = &dummy_buffer;
    CFE_SB_MsgId_t   dummy_id    = CFE_SB_INVALID_MSG_ID;

    UT_SetDefaultReturnValue(UT_KEY(CFE_EVS_Register), !CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_RunLoop), true);
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 2, false);
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_ReceiveBuffer), CFE_SUCCESS);
    UT_SetDataBuffer(UT_KEY(CFE_SB_ReceiveBuffer), &DummyBufPtr, sizeof(DummyBufPtr), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &dummy_id, sizeof(dummy_id), false);

    // Act
    FM_AppMain();

    uint8 count_runloop   = UT_GetStubCount(UT_KEY(CFE_ES_RunLoop));
    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_exitapp   = UT_GetStubCount(UT_KEY(CFE_ES_ExitApp));
    uint8 count_recbuff   = UT_GetStubCount(UT_KEY(CFE_SB_ReceiveBuffer));

    // Assert
    UtAssert_INT32_EQ(count_runloop, 2);
    UtAssert_INT32_EQ(count_sendevent, 3);
    UtAssert_INT32_EQ(count_exitapp, 1);
    UtAssert_INT32_EQ(count_recbuff, 1);
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
    // Arrange
    // Need to create dummy msgptr that is HousekeepingRequest
    CFE_SB_Buffer_t *dummy_buffer = NULL;
    CFE_SB_MsgId_t   msgid        = CFE_SB_ValueToMsgId(FM_SEND_HK_MID);

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &msgid, sizeof(msgid), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), false);

    /* Act */
    FM_ProcessPkt(dummy_buffer);

    uint8 count_isvalidcmdpktlength = UT_GetStubCount(UT_KEY(FM_IsValidCmdPktLength));
    uint8 count_sendevent           = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    // Assert
    UtAssert_INT32_EQ(count_isvalidcmdpktlength, 1);
    UtAssert_INT32_EQ(count_sendevent, 0);
}

void Test_FM_ProcessPkt_CheckMessageReturnGroundCommand(void)
{
    // Arrange
    CFE_SB_Buffer_t * dummy_buffer  = NULL;
    CFE_SB_MsgId_t    msgid         = CFE_SB_ValueToMsgId(FM_CMD_MID);
    CFE_MSG_FcnCode_t dummy_cc      = -1;
    FM_GlobalData.CommandErrCounter = UT_Utils_Any_uint8();

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &msgid, sizeof(msgid), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &dummy_cc, sizeof(dummy_cc), false);

    uint8 cmderrcounter_before = FM_GlobalData.CommandErrCounter;

    // Act
    FM_ProcessPkt(dummy_buffer);

    uint8 count_sendevent  = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_getfcncode = UT_GetStubCount(UT_KEY(CFE_MSG_GetFcnCode));

    // Assert
    UtAssert_INT32_EQ(cmderrcounter_before + 1, FM_GlobalData.CommandErrCounter);
    UtAssert_INT32_EQ(count_getfcncode, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CC_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
}

void Test_FM_ProcessPkt_CheckDefaultSwitchMessage(void)
{
    // Arrange
    CFE_SB_Buffer_t *dummy_buffer = NULL;
    CFE_SB_MsgId_t   msgid        = CFE_SB_INVALID_MSG_ID;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &msgid, sizeof(msgid), false);

    // Act
    FM_ProcessPkt(dummy_buffer);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    // Assert
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_MID_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
}

/* ********************************
 * AppInit Tests
 * *******************************/
void Test_FM_AppInit_EVSRegisterNotSuccess(void)
{
    // Arrange
    UT_SetDefaultReturnValue(UT_KEY(CFE_EVS_Register), !CFE_SUCCESS);

    // Act
    int32 result_appinit = FM_AppInit();

    uint8 count_evsregister = UT_GetStubCount(UT_KEY(CFE_EVS_Register));
    uint8 count_sendevent   = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    // Assert
    UtAssert_INT32_EQ(result_appinit, !CFE_SUCCESS);
    UtAssert_INT32_EQ(count_evsregister, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_STARTUP_EVENTS_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
}

void Test_FM_AppInit_CreatePipeFail(void)
{
    // Arrange
    UT_SetDefaultReturnValue(UT_KEY(CFE_EVS_Register), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_CreatePipe), !CFE_SUCCESS);

    // Act
    int32 result_appinit = FM_AppInit();

    uint8 count_evsregister = UT_GetStubCount(UT_KEY(CFE_EVS_Register));
    uint8 count_sendevent   = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_createpipe  = UT_GetStubCount(UT_KEY(CFE_SB_CreatePipe));

    // Assert
    UtAssert_INT32_EQ(result_appinit, !CFE_SUCCESS);
    UtAssert_INT32_EQ(count_evsregister, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(count_createpipe, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_STARTUP_CREAT_PIPE_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
}

void Test_FM_AppInit_HKSubscribeFail(void)
{
    // Arrange
    UT_SetDefaultReturnValue(UT_KEY(CFE_EVS_Register), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_CreatePipe), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_Subscribe), !CFE_SUCCESS);

    // Act
    int32 result_appinit = FM_AppInit();

    uint8 count_evsregister = UT_GetStubCount(UT_KEY(CFE_EVS_Register));
    uint8 count_sendevent   = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_createpipe  = UT_GetStubCount(UT_KEY(CFE_SB_CreatePipe));
    uint8 count_sbsubscribe = UT_GetStubCount(UT_KEY(CFE_SB_Subscribe));

    // Assert
    UtAssert_INT32_EQ(result_appinit, !CFE_SUCCESS);
    UtAssert_INT32_EQ(count_evsregister, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(count_createpipe, 1);
    UtAssert_INT32_EQ(count_sbsubscribe, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_STARTUP_SUBSCRIB_HK_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
}

void Test_FM_AppInit_GroundCmdSubscribeFail(void)
{
    // Arrange
    UT_SetDefaultReturnValue(UT_KEY(CFE_EVS_Register), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_CreatePipe), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_Subscribe), CFE_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(CFE_SB_Subscribe), 2, !CFE_SUCCESS);

    // Act
    int32 result_appinit = FM_AppInit();

    uint8 count_evsregister = UT_GetStubCount(UT_KEY(CFE_EVS_Register));
    uint8 count_sendevent   = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_createpipe  = UT_GetStubCount(UT_KEY(CFE_SB_CreatePipe));
    uint8 count_sbsubscribe = UT_GetStubCount(UT_KEY(CFE_SB_Subscribe));

    // Assert
    UtAssert_INT32_EQ(result_appinit, !CFE_SUCCESS);
    UtAssert_INT32_EQ(count_evsregister, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(count_createpipe, 1);
    UtAssert_INT32_EQ(count_sbsubscribe, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_STARTUP_SUBSCRIB_GCMD_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
}

void Test_FM_AppInit_TableInitNotSuccess(void)
{
    // Arrange
    UT_SetDefaultReturnValue(UT_KEY(CFE_EVS_Register), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_CreatePipe), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_Subscribe), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(FM_TableInit), !CFE_SUCCESS);

    // Act
    int32 result_appinit = FM_AppInit();

    uint8 count_evsregister = UT_GetStubCount(UT_KEY(CFE_EVS_Register));
    uint8 count_sendevent   = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_createpipe  = UT_GetStubCount(UT_KEY(CFE_SB_CreatePipe));
    uint8 count_sbsubscribe = UT_GetStubCount(UT_KEY(CFE_SB_Subscribe));

    // Assert
    UtAssert_INT32_EQ(result_appinit, !CFE_SUCCESS);
    UtAssert_INT32_EQ(count_evsregister, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(count_createpipe, 1);
    UtAssert_INT32_EQ(count_sbsubscribe, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_STARTUP_TABLE_INIT_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
}

void Test_FM_AppInit_TableInitSuccess(void)
{
    // Arrange
    UT_SetDefaultReturnValue(UT_KEY(CFE_EVS_Register), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_CreatePipe), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_Subscribe), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(FM_TableInit), CFE_SUCCESS);

    // Act
    int32 result_appinit = FM_AppInit();

    uint8 count_evsregister = UT_GetStubCount(UT_KEY(CFE_EVS_Register));
    uint8 count_sendevent   = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_createpipe  = UT_GetStubCount(UT_KEY(CFE_SB_CreatePipe));
    uint8 count_sbsubscribe = UT_GetStubCount(UT_KEY(CFE_SB_Subscribe));
    uint8 count_childinit   = UT_GetStubCount(UT_KEY(FM_ChildInit));

    // Assert
    UtAssert_INT32_EQ(result_appinit, CFE_SUCCESS);
    UtAssert_INT32_EQ(count_evsregister, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(count_createpipe, 1);
    UtAssert_INT32_EQ(count_sbsubscribe, 2);
    UtAssert_INT32_EQ(count_childinit, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_STARTUP_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
}

/* ********************************
 * Report HK Tests
 * *******************************/
void Test_FM_ReportHK_ReturnPktLengthTrue(void)
{
    // Arrange
    CFE_MSG_CommandHeader_t *dummy_msg = NULL;
    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_GetOpenFilesData), 0);

    FM_GlobalData.CommandCounter      = UT_Utils_Any_uint8();
    FM_GlobalData.CommandErrCounter   = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdErrCounter  = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8();
    FM_GlobalData.ChildQueueCount     = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCurrentCC      = UT_Utils_Any_uint8();
    FM_GlobalData.ChildPreviousCC     = UT_Utils_Any_uint8();

    uint8 cmdcounter_before          = FM_GlobalData.CommandCounter;
    uint8 cmderrcounter_before       = FM_GlobalData.CommandErrCounter;
    uint8 childcc_before             = FM_GlobalData.ChildCmdCounter;
    uint8 childcmderrcount_before    = FM_GlobalData.ChildCmdErrCounter;
    uint8 childcmdwarncounter_before = FM_GlobalData.ChildCmdWarnCounter;
    uint8 childquecount_before       = FM_GlobalData.ChildQueueCount;
    uint8 childcurrentcc_before      = FM_GlobalData.ChildCurrentCC;
    uint8 childprevcc_before         = FM_GlobalData.ChildPreviousCC;

    // Act
    FM_ReportHK(dummy_msg);

    uint8 count_validcmdpktlength = UT_GetStubCount(UT_KEY(FM_IsValidCmdPktLength));
    uint8 count_releasetblptrs    = UT_GetStubCount(UT_KEY(FM_ReleaseTablePointers));
    uint8 count_acquiretblptrs    = UT_GetStubCount(UT_KEY(FM_AcquireTablePointers));
    uint8 count_msginit           = UT_GetStubCount(UT_KEY(CFE_MSG_Init));
    uint8 count_getopenfiles      = UT_GetStubCount(UT_KEY(FM_GetOpenFilesData));
    uint8 count_timestampmsg      = UT_GetStubCount(UT_KEY(CFE_SB_TimeStampMsg));
    uint8 count_transmitmsg       = UT_GetStubCount(UT_KEY(CFE_SB_TransmitMsg));

    // Assert
    UtAssert_INT32_EQ(count_validcmdpktlength, 1);
    UtAssert_INT32_EQ(count_releasetblptrs, 1);
    UtAssert_INT32_EQ(count_acquiretblptrs, 1);
    UtAssert_INT32_EQ(count_msginit, 1);
    UtAssert_INT32_EQ(count_getopenfiles, 1);
    UtAssert_INT32_EQ(count_timestampmsg, 1);
    UtAssert_INT32_EQ(count_transmitmsg, 1);
    UtAssert_INT32_EQ(FM_GlobalData.HousekeepingPkt.CommandCounter, cmdcounter_before);
    UtAssert_INT32_EQ(FM_GlobalData.HousekeepingPkt.CommandErrCounter, cmderrcounter_before);
    UtAssert_INT32_EQ(FM_GlobalData.HousekeepingPkt.NumOpenFiles, 0);
    UtAssert_INT32_EQ(FM_GlobalData.HousekeepingPkt.ChildCmdCounter, childcc_before);
    UtAssert_INT32_EQ(FM_GlobalData.HousekeepingPkt.ChildCmdErrCounter, childcmderrcount_before);
    UtAssert_INT32_EQ(FM_GlobalData.HousekeepingPkt.ChildCmdWarnCounter, childcmdwarncounter_before);
    UtAssert_INT32_EQ(FM_GlobalData.HousekeepingPkt.ChildQueueCount, childquecount_before);
    UtAssert_INT32_EQ(FM_GlobalData.HousekeepingPkt.ChildCurrentCC, childcurrentcc_before);
    UtAssert_INT32_EQ(FM_GlobalData.HousekeepingPkt.ChildPreviousCC, childprevcc_before);
}

void Test_FM_ReportHK_ReturnPktLengthFalse(void)
{
    // Arrange
    CFE_MSG_CommandHeader_t *dummy_msg = NULL;
    UT_SetDefaultReturnValue(UT_KEY(FM_IsValidCmdPktLength), false);

    // Act
    FM_ReportHK(dummy_msg);

    uint8 count_releasetblptrs = UT_GetStubCount(UT_KEY(FM_ReleaseTablePointers));
    uint8 count_transmitmsg    = UT_GetStubCount(UT_KEY(CFE_SB_TransmitMsg));

    // Assert
    UtAssert_INT32_EQ(count_releasetblptrs, 0);
    UtAssert_INT32_EQ(count_transmitmsg, 0);
}

/* ********************************
 * Process Command Tests
 * *******************************/
void Test_FM_ProcessCmd_NoopCmdCCReturn(void)
{
    // Arrange
    CFE_SB_Buffer_t * dummy_buffer   = NULL;
    CFE_MSG_FcnCode_t dummy_fcn_code = FM_NOOP_CC;
    FM_GlobalData.CommandCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.CommandErrCounter  = UT_Utils_Any_uint8();
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &dummy_fcn_code, sizeof(dummy_fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_NoopCmd), true);

    uint8 commandcounter_before    = FM_GlobalData.CommandCounter;
    uint8 commanderrcounter_before = FM_GlobalData.CommandErrCounter;

    // Act
    FM_ProcessCmd(dummy_buffer);

    uint8 count_sendevent  = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_commandhnd = UT_GetStubCount(UT_KEY(FM_NoopCmd));

    // Assert
    UtAssert_INT32_EQ(count_sendevent, 0);
    UtAssert_INT32_EQ(count_commandhnd, 1);
    UtAssert_INT32_EQ(commandcounter_before + 1, FM_GlobalData.CommandCounter);
    UtAssert_INT32_EQ(commanderrcounter_before, FM_GlobalData.CommandErrCounter);
}

void Test_FM_ProcessCmd_ResetCountersCCReturn(void)
{
    // Arrange
    CFE_SB_Buffer_t * dummy_buffer   = NULL;
    CFE_MSG_FcnCode_t dummy_fcn_code = FM_RESET_CC;
    FM_GlobalData.CommandCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.CommandErrCounter  = UT_Utils_Any_uint8();
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &dummy_fcn_code, sizeof(dummy_fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_ResetCountersCmd), true);

    uint8 commandcounter_before    = FM_GlobalData.CommandCounter;
    uint8 commanderrcounter_before = FM_GlobalData.CommandErrCounter;

    // Act
    FM_ProcessCmd(dummy_buffer);

    uint8 count_sendevent  = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_commandhnd = UT_GetStubCount(UT_KEY(FM_ResetCountersCmd));

    // Assert
    UtAssert_INT32_EQ(count_sendevent, 0);
    UtAssert_INT32_EQ(count_commandhnd, 1);
    UtAssert_INT32_EQ(commandcounter_before, FM_GlobalData.CommandCounter);
    UtAssert_INT32_EQ(commanderrcounter_before, FM_GlobalData.CommandErrCounter);
}

void Test_FM_ProcessCmd_CopyFileCCReturn(void)
{
    // Arrange
    CFE_SB_Buffer_t * dummy_buffer   = NULL;
    CFE_MSG_FcnCode_t dummy_fcn_code = FM_COPY_CC;
    FM_GlobalData.CommandCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.CommandErrCounter  = UT_Utils_Any_uint8();
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &dummy_fcn_code, sizeof(dummy_fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_CopyFileCmd), true);

    uint8 commandcounter_before    = FM_GlobalData.CommandCounter;
    uint8 commanderrcounter_before = FM_GlobalData.CommandErrCounter;

    // Act
    FM_ProcessCmd(dummy_buffer);

    uint8 count_sendevent  = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_commandhnd = UT_GetStubCount(UT_KEY(FM_CopyFileCmd));

    // Assert
    UtAssert_INT32_EQ(count_sendevent, 0);
    UtAssert_INT32_EQ(count_commandhnd, 1);
    UtAssert_INT32_EQ(commandcounter_before + 1, FM_GlobalData.CommandCounter);
    UtAssert_INT32_EQ(commanderrcounter_before, FM_GlobalData.CommandErrCounter);
}

void Test_FM_ProcessCmd_MoveFileCCReturn(void)
{
    // Arrange
    CFE_SB_Buffer_t * dummy_buffer   = NULL;
    CFE_MSG_FcnCode_t dummy_fcn_code = FM_MOVE_CC;
    FM_GlobalData.CommandCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.CommandErrCounter  = UT_Utils_Any_uint8();
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &dummy_fcn_code, sizeof(dummy_fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_MoveFileCmd), true);

    uint8 commandcounter_before    = FM_GlobalData.CommandCounter;
    uint8 commanderrcounter_before = FM_GlobalData.CommandErrCounter;

    // Act
    FM_ProcessCmd(dummy_buffer);

    uint8 count_sendevent  = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_commandhnd = UT_GetStubCount(UT_KEY(FM_MoveFileCmd));

    // Assert
    UtAssert_INT32_EQ(count_sendevent, 0);
    UtAssert_INT32_EQ(count_commandhnd, 1);
    UtAssert_INT32_EQ(commandcounter_before + 1, FM_GlobalData.CommandCounter);
    UtAssert_INT32_EQ(commanderrcounter_before, FM_GlobalData.CommandErrCounter);
}

void Test_FM_ProcessCmd_RenameFileCCReturn(void)
{
    // Arrange
    CFE_SB_Buffer_t * dummy_buffer   = NULL;
    CFE_MSG_FcnCode_t dummy_fcn_code = FM_RENAME_CC;
    FM_GlobalData.CommandCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.CommandErrCounter  = UT_Utils_Any_uint8();
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &dummy_fcn_code, sizeof(dummy_fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_RenameFileCmd), true);

    uint8 commandcounter_before    = FM_GlobalData.CommandCounter;
    uint8 commanderrcounter_before = FM_GlobalData.CommandErrCounter;

    // Act
    FM_ProcessCmd(dummy_buffer);

    uint8 count_sendevent  = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_commandhnd = UT_GetStubCount(UT_KEY(FM_RenameFileCmd));

    // Assert
    UtAssert_INT32_EQ(count_sendevent, 0);
    UtAssert_INT32_EQ(count_commandhnd, 1);
    UtAssert_INT32_EQ(commandcounter_before + 1, FM_GlobalData.CommandCounter);
    UtAssert_INT32_EQ(commanderrcounter_before, FM_GlobalData.CommandErrCounter);
}

void Test_FM_ProcessCmd_DeleteFileCCReturn(void)
{
    // Arrange
    CFE_SB_Buffer_t * dummy_buffer   = NULL;
    CFE_MSG_FcnCode_t dummy_fcn_code = FM_DELETE_CC;
    FM_GlobalData.CommandCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.CommandErrCounter  = UT_Utils_Any_uint8();
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &dummy_fcn_code, sizeof(dummy_fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_DeleteFileCmd), true);

    uint8 commandcounter_before    = FM_GlobalData.CommandCounter;
    uint8 commanderrcounter_before = FM_GlobalData.CommandErrCounter;

    // Act
    FM_ProcessCmd(dummy_buffer);

    uint8 count_sendevent  = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_commandhnd = UT_GetStubCount(UT_KEY(FM_DeleteFileCmd));

    // Assert
    UtAssert_INT32_EQ(count_sendevent, 0);
    UtAssert_INT32_EQ(count_commandhnd, 1);
    UtAssert_INT32_EQ(commandcounter_before + 1, FM_GlobalData.CommandCounter);
    UtAssert_INT32_EQ(commanderrcounter_before, FM_GlobalData.CommandErrCounter);
}

void Test_FM_ProcessCmd_DeleteAllFilesCCReturn(void)
{
    // Arrange
    CFE_SB_Buffer_t * dummy_buffer   = NULL;
    CFE_MSG_FcnCode_t dummy_fcn_code = FM_DELETE_ALL_CC;
    FM_GlobalData.CommandCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.CommandErrCounter  = UT_Utils_Any_uint8();
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &dummy_fcn_code, sizeof(dummy_fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_DeleteAllFilesCmd), true);

    uint8 commandcounter_before    = FM_GlobalData.CommandCounter;
    uint8 commanderrcounter_before = FM_GlobalData.CommandErrCounter;

    // Act
    FM_ProcessCmd(dummy_buffer);

    uint8 count_sendevent  = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_commandhnd = UT_GetStubCount(UT_KEY(FM_DeleteAllFilesCmd));

    // Assert
    UtAssert_INT32_EQ(count_sendevent, 0);
    UtAssert_INT32_EQ(count_commandhnd, 1);
    UtAssert_INT32_EQ(commandcounter_before + 1, FM_GlobalData.CommandCounter);
    UtAssert_INT32_EQ(commanderrcounter_before, FM_GlobalData.CommandErrCounter);
}

#ifdef FM_INCLUDE_DECOMPRESS
void Test_FM_ProcessCmd_DecompressFileCCReturn(void)
{
    // Arrange
    CFE_SB_Buffer_t * dummy_buffer   = NULL;
    CFE_MSG_FcnCode_t dummy_fcn_code = FM_DECOMPRESS_CC;

#define FM_INCLUDE_DECOMPRESS
    FM_GlobalData.CommandCounter    = UT_Utils_Any_uint8();
    FM_GlobalData.CommandErrCounter = UT_Utils_Any_uint8();
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &dummy_fcn_code, sizeof(dummy_fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_DecompressFileCmd), true);

    uint8 commandcounter_before    = FM_GlobalData.CommandCounter;
    uint8 commanderrcounter_before = FM_GlobalData.CommandErrCounter;

    // Act
    FM_ProcessCmd(dummy_buffer);

    uint8 count_sendevent  = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_commandhnd = UT_GetStubCount(UT_KEY(FM_DecompressFileCmd));

    // Assert
    UtAssert_INT32_EQ(count_sendevent, 0);
    UtAssert_INT32_EQ(count_commandhnd, 1);
    UtAssert_INT32_EQ(commandcounter_before + 1, FM_GlobalData.CommandCounter);
    UtAssert_INT32_EQ(commanderrcounter_before, FM_GlobalData.CommandErrCounter);
}
#endif

void Test_FM_ProcessCmd_ConcatFilesCCReturn(void)
{
    // Arrange
    CFE_SB_Buffer_t * dummy_buffer   = NULL;
    CFE_MSG_FcnCode_t dummy_fcn_code = FM_CONCAT_CC;
    FM_GlobalData.CommandCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.CommandErrCounter  = UT_Utils_Any_uint8();
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &dummy_fcn_code, sizeof(dummy_fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_ConcatFilesCmd), true);

    uint8 commandcounter_before    = FM_GlobalData.CommandCounter;
    uint8 commanderrcounter_before = FM_GlobalData.CommandErrCounter;

    // Act
    FM_ProcessCmd(dummy_buffer);

    uint8 count_sendevent  = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_commandhnd = UT_GetStubCount(UT_KEY(FM_ConcatFilesCmd));

    // Assert
    UtAssert_INT32_EQ(count_sendevent, 0);
    UtAssert_INT32_EQ(count_commandhnd, 1);
    UtAssert_INT32_EQ(commandcounter_before + 1, FM_GlobalData.CommandCounter);
    UtAssert_INT32_EQ(commanderrcounter_before, FM_GlobalData.CommandErrCounter);
}

void Test_FM_ProcessCmd_GetFileInfoCCReturn(void)
{
    // Arrange
    CFE_SB_Buffer_t * dummy_buffer   = NULL;
    CFE_MSG_FcnCode_t dummy_fcn_code = FM_GET_FILE_INFO_CC;
    FM_GlobalData.CommandCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.CommandErrCounter  = UT_Utils_Any_uint8();
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &dummy_fcn_code, sizeof(dummy_fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_GetFileInfoCmd), true);

    uint8 commandcounter_before    = FM_GlobalData.CommandCounter;
    uint8 commanderrcounter_before = FM_GlobalData.CommandErrCounter;

    // Act
    FM_ProcessCmd(dummy_buffer);

    uint8 count_sendevent  = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_commandhnd = UT_GetStubCount(UT_KEY(FM_GetFileInfoCmd));

    // Assert
    UtAssert_INT32_EQ(count_sendevent, 0);
    UtAssert_INT32_EQ(count_commandhnd, 1);
    UtAssert_INT32_EQ(commandcounter_before + 1, FM_GlobalData.CommandCounter);
    UtAssert_INT32_EQ(commanderrcounter_before, FM_GlobalData.CommandErrCounter);
}

void Test_FM_ProcessCmd_GetOpenFilesCCReturn(void)
{
    // Arrange
    CFE_SB_Buffer_t * dummy_buffer   = NULL;
    CFE_MSG_FcnCode_t dummy_fcn_code = FM_GET_OPEN_FILES_CC;
    FM_GlobalData.CommandCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.CommandErrCounter  = UT_Utils_Any_uint8();
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &dummy_fcn_code, sizeof(dummy_fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_GetOpenFilesCmd), true);

    uint8 commandcounter_before    = FM_GlobalData.CommandCounter;
    uint8 commanderrcounter_before = FM_GlobalData.CommandErrCounter;

    // Act
    FM_ProcessCmd(dummy_buffer);

    uint8 count_sendevent  = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_commandhnd = UT_GetStubCount(UT_KEY(FM_GetOpenFilesCmd));

    // Assert
    UtAssert_INT32_EQ(count_sendevent, 0);
    UtAssert_INT32_EQ(count_commandhnd, 1);
    UtAssert_INT32_EQ(commandcounter_before + 1, FM_GlobalData.CommandCounter);
    UtAssert_INT32_EQ(commanderrcounter_before, FM_GlobalData.CommandErrCounter);
}

void Test_FM_ProcessCmd_CreateDirectoryCCReturn(void)
{
    // Arrange
    CFE_SB_Buffer_t * dummy_buffer   = NULL;
    CFE_MSG_FcnCode_t dummy_fcn_code = FM_CREATE_DIR_CC;
    FM_GlobalData.CommandCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.CommandErrCounter  = UT_Utils_Any_uint8();
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &dummy_fcn_code, sizeof(dummy_fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_CreateDirectoryCmd), true);

    uint8 commandcounter_before    = FM_GlobalData.CommandCounter;
    uint8 commanderrcounter_before = FM_GlobalData.CommandErrCounter;

    // Act
    FM_ProcessCmd(dummy_buffer);

    uint8 count_sendevent  = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_commandhnd = UT_GetStubCount(UT_KEY(FM_CreateDirectoryCmd));

    // Assert
    UtAssert_INT32_EQ(count_sendevent, 0);
    UtAssert_INT32_EQ(count_commandhnd, 1);
    UtAssert_INT32_EQ(commandcounter_before + 1, FM_GlobalData.CommandCounter);
    UtAssert_INT32_EQ(commanderrcounter_before, FM_GlobalData.CommandErrCounter);
}

void Test_FM_ProcessCmd_DeleteDirectoryCCReturn(void)
{
    // Arrange
    CFE_SB_Buffer_t * dummy_buffer   = NULL;
    CFE_MSG_FcnCode_t dummy_fcn_code = FM_DELETE_DIR_CC;
    FM_GlobalData.CommandCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.CommandErrCounter  = UT_Utils_Any_uint8();
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &dummy_fcn_code, sizeof(dummy_fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_DeleteDirectoryCmd), true);

    uint8 commandcounter_before    = FM_GlobalData.CommandCounter;
    uint8 commanderrcounter_before = FM_GlobalData.CommandErrCounter;

    // Act
    FM_ProcessCmd(dummy_buffer);

    uint8 count_sendevent  = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_commandhnd = UT_GetStubCount(UT_KEY(FM_DeleteDirectoryCmd));

    // Assert
    UtAssert_INT32_EQ(count_sendevent, 0);
    UtAssert_INT32_EQ(count_commandhnd, 1);
    UtAssert_INT32_EQ(commandcounter_before + 1, FM_GlobalData.CommandCounter);
    UtAssert_INT32_EQ(commanderrcounter_before, FM_GlobalData.CommandErrCounter);
}

void Test_FM_ProcessCmd_GetDirListFileCCReturn(void)
{
    // Arrange
    CFE_SB_Buffer_t * dummy_buffer   = NULL;
    CFE_MSG_FcnCode_t dummy_fcn_code = FM_GET_DIR_FILE_CC;
    FM_GlobalData.CommandCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.CommandErrCounter  = UT_Utils_Any_uint8();
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &dummy_fcn_code, sizeof(dummy_fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_GetDirListFileCmd), true);

    uint8 commandcounter_before    = FM_GlobalData.CommandCounter;
    uint8 commanderrcounter_before = FM_GlobalData.CommandErrCounter;

    // Act
    FM_ProcessCmd(dummy_buffer);

    uint8 count_sendevent  = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_commandhnd = UT_GetStubCount(UT_KEY(FM_GetDirListFileCmd));

    // Assert
    UtAssert_INT32_EQ(count_sendevent, 0);
    UtAssert_INT32_EQ(count_commandhnd, 1);
    UtAssert_INT32_EQ(commandcounter_before + 1, FM_GlobalData.CommandCounter);
    UtAssert_INT32_EQ(commanderrcounter_before, FM_GlobalData.CommandErrCounter);
}

void Test_FM_ProcessCmd_GetDirListPktCCReturn(void)
{
    // Arrange
    CFE_SB_Buffer_t * dummy_buffer   = NULL;
    CFE_MSG_FcnCode_t dummy_fcn_code = FM_GET_DIR_PKT_CC;
    FM_GlobalData.CommandCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.CommandErrCounter  = UT_Utils_Any_uint8();
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &dummy_fcn_code, sizeof(dummy_fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_GetDirListPktCmd), true);

    uint8 commandcounter_before    = FM_GlobalData.CommandCounter;
    uint8 commanderrcounter_before = FM_GlobalData.CommandErrCounter;

    // Act
    FM_ProcessCmd(dummy_buffer);

    uint8 count_sendevent  = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_commandhnd = UT_GetStubCount(UT_KEY(FM_GetDirListPktCmd));

    // Assert
    UtAssert_INT32_EQ(count_sendevent, 0);
    UtAssert_INT32_EQ(count_commandhnd, 1);
    UtAssert_INT32_EQ(commandcounter_before + 1, FM_GlobalData.CommandCounter);
    UtAssert_INT32_EQ(commanderrcounter_before, FM_GlobalData.CommandErrCounter);
}

void Test_FM_ProcessCmd_GetFreeSpaceCCReturn(void)
{
    // Arrange
    CFE_SB_Buffer_t * dummy_buffer   = NULL;
    CFE_MSG_FcnCode_t dummy_fcn_code = FM_GET_FREE_SPACE_CC;
    FM_GlobalData.CommandCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.CommandErrCounter  = UT_Utils_Any_uint8();
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &dummy_fcn_code, sizeof(dummy_fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_GetFreeSpaceCmd), true);

    uint8 commandcounter_before    = FM_GlobalData.CommandCounter;
    uint8 commanderrcounter_before = FM_GlobalData.CommandErrCounter;

    // Act
    FM_ProcessCmd(dummy_buffer);

    uint8 count_sendevent  = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_commandhnd = UT_GetStubCount(UT_KEY(FM_GetFreeSpaceCmd));

    // Assert
    UtAssert_INT32_EQ(count_sendevent, 0);
    UtAssert_INT32_EQ(count_commandhnd, 1);
    UtAssert_INT32_EQ(commandcounter_before + 1, FM_GlobalData.CommandCounter);
    UtAssert_INT32_EQ(commanderrcounter_before, FM_GlobalData.CommandErrCounter);
}

void Test_FM_ProcessCmd_SetTableStateCCReturn(void)
{
    // Arrange
    CFE_SB_Buffer_t * dummy_buffer   = NULL;
    CFE_MSG_FcnCode_t dummy_fcn_code = FM_SET_TABLE_STATE_CC;
    FM_GlobalData.CommandCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.CommandErrCounter  = UT_Utils_Any_uint8();
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &dummy_fcn_code, sizeof(dummy_fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_SetTableStateCmd), true);

    uint8 commandcounter_before    = FM_GlobalData.CommandCounter;
    uint8 commanderrcounter_before = FM_GlobalData.CommandErrCounter;

    // Act
    FM_ProcessCmd(dummy_buffer);

    uint8 count_sendevent  = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_commandhnd = UT_GetStubCount(UT_KEY(FM_SetTableStateCmd));

    // Assert
    UtAssert_INT32_EQ(count_sendevent, 0);
    UtAssert_INT32_EQ(count_commandhnd, 1);
    UtAssert_INT32_EQ(commandcounter_before + 1, FM_GlobalData.CommandCounter);
    UtAssert_INT32_EQ(commanderrcounter_before, FM_GlobalData.CommandErrCounter);
}

void Test_FM_ProcessCmd_DeleteFileIntCCReturn(void)
{
    // Arrange
    CFE_SB_Buffer_t * dummy_buffer   = NULL;
    CFE_MSG_FcnCode_t dummy_fcn_code = FM_DELETE_INT_CC;
    FM_GlobalData.CommandCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.CommandErrCounter  = UT_Utils_Any_uint8();
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &dummy_fcn_code, sizeof(dummy_fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_DeleteFileCmd), true);

    uint8 commandcounter_before    = FM_GlobalData.CommandCounter;
    uint8 commanderrcounter_before = FM_GlobalData.CommandErrCounter;

    // Act
    FM_ProcessCmd(dummy_buffer);

    uint8 count_sendevent  = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_commandhnd = UT_GetStubCount(UT_KEY(FM_DeleteFileCmd));

    // Assert
    UtAssert_INT32_EQ(count_sendevent, 0);
    UtAssert_INT32_EQ(count_commandhnd, 1);
    UtAssert_INT32_EQ(commandcounter_before, FM_GlobalData.CommandCounter);
    UtAssert_INT32_EQ(commanderrcounter_before, FM_GlobalData.CommandErrCounter);
}

void Test_FM_ProcessCmd_SetPermissionsCCReturn(void)
{
    // Arrange
    CFE_SB_Buffer_t * dummy_buffer   = NULL;
    CFE_MSG_FcnCode_t dummy_fcn_code = FM_SET_FILE_PERM_CC;
    FM_GlobalData.CommandCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.CommandErrCounter  = UT_Utils_Any_uint8();
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &dummy_fcn_code, sizeof(dummy_fcn_code), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_SetPermissionsCmd), true);

    uint8 commandcounter_before    = FM_GlobalData.CommandCounter;
    uint8 commanderrcounter_before = FM_GlobalData.CommandErrCounter;

    // Act
    FM_ProcessCmd(dummy_buffer);

    uint8 count_sendevent  = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_commandhnd = UT_GetStubCount(UT_KEY(FM_SetPermissionsCmd));

    // Assert
    UtAssert_INT32_EQ(count_sendevent, 0);
    UtAssert_INT32_EQ(count_commandhnd, 1);
    UtAssert_INT32_EQ(commandcounter_before + 1, FM_GlobalData.CommandCounter);
    UtAssert_INT32_EQ(commanderrcounter_before, FM_GlobalData.CommandErrCounter);
}

void Test_FM_ProcessCmd_DefaultReturn(void)
{
    // Arrange
    CFE_SB_Buffer_t * dummy_buffer   = NULL;
    CFE_MSG_FcnCode_t dummy_fcn_code = -1;
    FM_GlobalData.CommandCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.CommandErrCounter  = UT_Utils_Any_uint8();
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &dummy_fcn_code, sizeof(dummy_fcn_code), false);

    uint8 commandcounter_before    = FM_GlobalData.CommandCounter;
    uint8 commanderrcounter_before = FM_GlobalData.CommandErrCounter;

    // Act
    FM_ProcessCmd(dummy_buffer);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    // Assert
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(commandcounter_before, FM_GlobalData.CommandCounter);
    UtAssert_INT32_EQ(commanderrcounter_before + 1, FM_GlobalData.CommandErrCounter);
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

    UtTest_Add(Test_FM_ProcessCmd_DeleteFileIntCCReturn, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_PRocessCmd_DeleteFileIntCCReturn");

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
