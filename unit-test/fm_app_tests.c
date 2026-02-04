/************************************************************************
 * NASA Docket No. GSC-19,200-1, and identified as "cFS Draco"
 *
 * Copyright (c) 2023 United States Government as represented by the
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
#include "fm_table_utils.h"
#include "fm_child.h"
#include "fm_cmds.h"
#include "fm_cmd_utils.h"
#include "fm_dispatch.h"
#include "fm_eventids.h"
#include "fm_perfids.h"
#include "fm_platform_cfg.h"
#include "fm_version.h"
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
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_STUB_COUNT(CFE_ES_WriteToSysLog, 2);
    UtAssert_STUB_COUNT(CFE_ES_ExitApp, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_EXIT_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
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
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_INIT_INF_EID);
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
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_EXIT_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_ERROR);

    /* Arrange */
    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_RunLoop), true);
    UT_SetDeferredRetcode(UT_KEY(CFE_ES_RunLoop), 2, false);
    UT_SetDefaultReturnValue(UT_KEY(CFE_SB_ReceiveBuffer), CFE_SB_NO_MESSAGE);

    /* Act */
    UtAssert_VOIDCALL(FM_AppMain());
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
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_STUB_COUNT(CFE_ES_ExitApp, 1);
    UtAssert_STUB_COUNT(CFE_SB_ReceiveBuffer, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_INIT_INF_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_EXIT_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_ERROR);
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
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CR_PIPE_ERR_EID);
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
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_INIT_INF_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
}

/* * * * * * * * * * * * * *
 * Add Method Tests
 * * * * * * * * * * * * * */
void add_FM_AppMain_tests(void)
{
    UtTest_Add(Test_FM_AppMain_AppInitNotSuccess, FM_Test_Setup, FM_Test_Teardown, "Test_FM_AppMain_AppInitNotSuccess");

    UtTest_Add(Test_FM_AppMain_SBReceiveBufferDefaultOption,
               FM_Test_Setup,
               FM_Test_Teardown,
               "Test_FM_AppMain_SBReceiveBufferDefaultOption");

    UtTest_Add(Test_FM_AppMain_SBReceiveBufferIsTimeOut,
               FM_Test_Setup,
               FM_Test_Teardown,
               "Test_FM_AppMain_SBReceiveBufferIsTimeOut");

    UtTest_Add(Test_FM_AppMain_ReceiveBufferSuccessBufPtrIsNull,
               FM_Test_Setup,
               FM_Test_Teardown,
               "Test_FM_AppMain_ReceiveBufferSuccessBufPtrIsNull");

    UtTest_Add(Test_FM_AppMain_BufPtrNotEqualNull,
               FM_Test_Setup,
               FM_Test_Teardown,
               "Test_FM_AppMain_BufPtrNotEqualNull");
}

void add_FM_AppInit_tests(void)
{
    UtTest_Add(Test_FM_AppInit_EVSRegisterNotSuccess,
               FM_Test_Setup,
               FM_Test_Teardown,
               "Test_FM_AppInit_EVSRegisterNotSuccess");

    UtTest_Add(Test_FM_AppInit_CreatePipeFail, FM_Test_Setup, FM_Test_Teardown, "Test_FM_AppInit_CreatePipeFail");

    UtTest_Add(Test_FM_AppInit_HKSubscribeFail, FM_Test_Setup, FM_Test_Teardown, "Test_FM_AppInit_HKSubscribeFail");

    UtTest_Add(Test_FM_AppInit_GroundCmdSubscribeFail,
               FM_Test_Setup,
               FM_Test_Teardown,
               "Test_FM_AppInit_GroundCmdSubscribeFail");

    UtTest_Add(Test_FM_AppInit_TableInitNotSuccess,
               FM_Test_Setup,
               FM_Test_Teardown,
               "Test_FM_AppInit_TableInitNotSuccess");

    UtTest_Add(Test_FM_AppInit_TableInitSuccess, FM_Test_Setup, FM_Test_Teardown, "Test_FM_AppInit_TableInitSuccess");
}

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    add_FM_AppInit_tests();
    add_FM_AppMain_tests();
}
