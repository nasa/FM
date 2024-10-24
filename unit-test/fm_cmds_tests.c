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
 *  File Manager (FM) Child task (low priority command handler)
 */

#include "cfe.h"
#include "fm_msg.h"
#include "fm_msgdefs.h"
#include "fm_msgids.h"
#include "fm_events.h"
#include "fm_app.h"
#include "fm_child.h"
#include "fm_cmds.h"
#include "fm_cmd_utils.h"
#include "fm_perfids.h"
#include "fm_platform_cfg.h"
#include "fm_verify.h"
#include <string.h>

/*
 * UT Assert
 */
#include "fm_test_utils.h"

/*
 * UT includes
 */
#include "uttest.h"
#include "utassert.h"
#include "utstubs.h"

uint8 call_count_CFE_EVS_SendEvent;

/*
**********************************************************************************
**          TEST CASE FUNCTIONS
**********************************************************************************
*/
/******************/
/* Noop Tests     */
/******************/

void Test_FM_NoopCmd_Success(void)
{
    int32 strCmpResult;
    char  ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH, "%%s command: FM version %%d.%%d.%%d.%%d");

    bool Result = FM_NoopCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_NoopCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_NOOP_INF_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);
}

void add_FM_NoopCmd_tests(void)
{
    UtTest_Add(Test_FM_NoopCmd_Success, FM_Test_Setup, FM_Test_Teardown, "Test_FM_NoopCmd_Success");
}

/****************************/
/* Reset Counters Tests     */
/****************************/

void Test_FM_ResetCountersCmd_Success(void)
{
    int32 strCmpResult;
    char  ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    bool  Result;
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH, "%%s command");

    FM_GlobalData.CommandCounter      = 1;
    FM_GlobalData.CommandErrCounter   = 1;
    FM_GlobalData.ChildCmdCounter     = 1;
    FM_GlobalData.ChildCmdErrCounter  = 1;
    FM_GlobalData.ChildCmdWarnCounter = 1;

    Result = FM_ResetCountersCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_ResetCountersCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_RESET_INF_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);

    UtAssert_INT32_EQ(FM_GlobalData.CommandCounter, 0);
    UtAssert_INT32_EQ(FM_GlobalData.CommandErrCounter, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCmdCounter, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCmdErrCounter, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCmdWarnCounter, 0);
}

void add_FM_ResetCountersCmd_tests(void)
{
    UtTest_Add(Test_FM_ResetCountersCmd_Success, FM_Test_Setup, FM_Test_Teardown, "Test_FM_ResetCountersCmd_Success");
}

/****************************/
/* Copy File Tests     */
/****************************/

void Test_FM_CopyFileCmd_Success(void)
{
    FM_OvwSourceTargetFilename_Payload_t *CmdPtr;
    bool                                  Result;

    CmdPtr = &UT_CmdBuf.CopyFileCmd.Payload;

    strncpy(CmdPtr->Source, "src1", sizeof(CmdPtr->Source) - 1);
    strncpy(CmdPtr->Target, "tgt", sizeof(CmdPtr->Target) - 1);
    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_CopyFileCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_CopyFileCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, FM_COPY_FILE_CC);
}

void Test_FM_CopyFileCmd_BadOverwrite(void)
{
    bool Result;

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_CopyFileCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_CopyFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_CopyFileCmd_SourceNotExist(void)
{
    bool Result;

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_CopyFileCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_CopyFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_CopyFileCmd_NoOverwriteTargetExists(void)
{
    bool Result;

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_CopyFileCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_CopyFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_CopyFileCmd_OverwriteFileOpen(void)
{
    FM_OvwSourceTargetFilename_Payload_t *CmdPtr;
    bool                                  Result;

    CmdPtr = &UT_CmdBuf.CopyFileCmd.Payload;

    CmdPtr->Overwrite                       = 1;
    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_CopyFileCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_CopyFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_CopyFileCmd_NoChildTask(void)
{
    bool Result;

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    Result = FM_CopyFileCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_CopyFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void add_FM_CopyFileCmd_tests(void)
{
    UtTest_Add(Test_FM_CopyFileCmd_Success, FM_Test_Setup, FM_Test_Teardown, "Test_FM_CopyFileCmd_Success");

    UtTest_Add(Test_FM_CopyFileCmd_BadOverwrite, FM_Test_Setup, FM_Test_Teardown, "Test_FM_CopyFileCmd_BadOverwrite");

    UtTest_Add(Test_FM_CopyFileCmd_SourceNotExist, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_CopyFileCmd_SourceNotExist");

    UtTest_Add(Test_FM_CopyFileCmd_NoOverwriteTargetExists, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_CopyFileCmd_NoOverwriteTargetExists");

    UtTest_Add(Test_FM_CopyFileCmd_OverwriteFileOpen, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_CopyFileCmd_OverwriteFileOpen");

    UtTest_Add(Test_FM_CopyFileCmd_NoChildTask, FM_Test_Setup, FM_Test_Teardown, "Test_FM_CopyFileCmd_NoChildTask");
}

/****************************/
/* Move File Tests          */
/****************************/

void Test_FM_MoveFileCmd_Success(void)
{
    bool Result;

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_MoveFileCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_MoveFileCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, FM_MOVE_FILE_CC);
}

void Test_FM_MoveFileCmd_BadOverwrite(void)
{
    bool Result;

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_MoveFileCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_MoveFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_MoveFileCmd_SourceNotExist(void)
{
    bool Result;

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_MoveFileCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_MoveFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_MoveFileCmd_NoOverwriteTargetExists(void)
{
    bool Result;

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_MoveFileCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_MoveFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_MoveFileCmd_OverwriteFileOpen(void)
{
    FM_OvwSourceTargetFilename_Payload_t *CmdPtr;
    bool                                  Result;

    CmdPtr = &UT_CmdBuf.MoveFileCmd.Payload;

    CmdPtr->Overwrite                       = 1;
    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_MoveFileCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_MoveFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_MoveFileCmd_NoChildTask(void)
{
    bool Result;

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    Result = FM_MoveFileCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_MoveFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void add_FM_MoveFileCmd_tests(void)
{
    UtTest_Add(Test_FM_MoveFileCmd_Success, FM_Test_Setup, FM_Test_Teardown, "Test_FM_MoveFileCmd_Success");

    UtTest_Add(Test_FM_MoveFileCmd_BadOverwrite, FM_Test_Setup, FM_Test_Teardown, "Test_FM_MoveFileCmd_BadOverwrite");

    UtTest_Add(Test_FM_MoveFileCmd_SourceNotExist, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_MoveFileCmd_SourceNotExist");

    UtTest_Add(Test_FM_MoveFileCmd_NoOverwriteTargetExists, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_MoveFileCmd_NoOverwriteTargetExists");

    UtTest_Add(Test_FM_MoveFileCmd_OverwriteFileOpen, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_MoveFileCmd_OverwriteFileOpen");

    UtTest_Add(Test_FM_MoveFileCmd_NoChildTask, FM_Test_Setup, FM_Test_Teardown, "Test_FM_MoveFileCmd_NoChildTask");
}

/****************************/
/* Rename File Tests        */
/****************************/

void Test_FM_RenameFileCmd_Success(void)
{
    FM_SourceTargetFileName_Payload_t *CmdPtr;
    bool                               Result;

    CmdPtr = &UT_CmdBuf.RenameFileCmd.Payload;

    strncpy(CmdPtr->Source, "src1", sizeof(CmdPtr->Source) - 1);
    strncpy(CmdPtr->Target, "tgt", sizeof(CmdPtr->Target) - 1);

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_RenameFileCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_RenameFileCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, FM_RENAME_FILE_CC);
}

void Test_FM_RenameFileCmd_SourceNotExist(void)
{
    bool Result;

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_RenameFileCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_RenameFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_RenameFileCmd_TargetExists(void)
{
    bool Result;

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_RenameFileCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_RenameFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_RenameFileCmd_NoChildTask(void)
{
    bool Result;

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    Result = FM_RenameFileCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_RenameFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void add_FM_RenameFileCmd_tests(void)
{
    UtTest_Add(Test_FM_RenameFileCmd_Success, FM_Test_Setup, FM_Test_Teardown, "Test_FM_RenameFileCmd_Success");

    UtTest_Add(Test_FM_RenameFileCmd_SourceNotExist, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_RenameFileCmd_SourceNotExist");

    UtTest_Add(Test_FM_RenameFileCmd_TargetExists, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_RenameFileCmd_TargetExists");

    UtTest_Add(Test_FM_RenameFileCmd_NoChildTask, FM_Test_Setup, FM_Test_Teardown, "Test_FM_RenameFileCmd_NoChildTask");
}

/****************************/
/* Delete File Tests        */
/****************************/

void Test_FM_DeleteFileCmd_Success(void)
{
    bool Result;

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    CFE_MSG_FcnCode_t forced_CmdCode = FM_DELETE_FILE_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &forced_CmdCode, sizeof(forced_CmdCode), false);

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_DeleteFileCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_DeleteFileCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, FM_DELETE_FILE_CC);
}

void Test_FM_DeleteFileCmd_FileNotClosed(void)
{
    bool Result;

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    CFE_MSG_FcnCode_t forced_CmdCode = FM_DELETE_FILE_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &forced_CmdCode, sizeof(forced_CmdCode), false);

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_DeleteFileCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_DeleteFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_DeleteFileCmd_NoChildTask(void)
{
    bool Result;

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    CFE_MSG_FcnCode_t forced_CmdCode = FM_DELETE_FILE_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &forced_CmdCode, sizeof(forced_CmdCode), false);

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    Result = FM_DeleteFileCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_DeleteFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void add_FM_DeleteFileCmd_tests(void)
{
    UtTest_Add(Test_FM_DeleteFileCmd_Success, FM_Test_Setup, FM_Test_Teardown, "Test_FM_DeleteFileCmd_Success");

    UtTest_Add(Test_FM_DeleteFileCmd_FileNotClosed, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_DeleteFileCmd_FileNotClosed");

    UtTest_Add(Test_FM_DeleteFileCmd_NoChildTask, FM_Test_Setup, FM_Test_Teardown, "Test_FM_DeleteFileCmd_NoChildTask");
}

/****************************/
/* Delete All Files Tests   */
/****************************/

void Test_FM_DeleteAllFilesCmd_Success(void)
{
    FM_DirectoryName_Payload_t *CmdPtr;
    bool                        Result;

    CmdPtr = &UT_CmdBuf.DeleteAllFilesCmd.Payload;

    strncpy(CmdPtr->Directory, "dir", sizeof(CmdPtr->Directory) - 1);

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_DeleteAllFilesCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_DeleteAllFilesCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, FM_DELETE_ALL_FILES_CC);
}

void Test_FM_DeleteAllFilesCmd_DirNoExist(void)
{
    bool Result;

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_DeleteAllFilesCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_DeleteAllFilesCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_DeleteAllFilesCmd_NoChildTask(void)
{
    FM_DirectoryName_Payload_t *CmdPtr;
    bool                        Result;

    CmdPtr = &UT_CmdBuf.DeleteAllFilesCmd.Payload;

    strncpy(CmdPtr->Directory, "dir", sizeof(CmdPtr->Directory) - 1);
    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    Result = FM_DeleteAllFilesCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_DeleteAllFilesCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void add_FM_DeleteAllFilesCmd_tests(void)
{
    UtTest_Add(Test_FM_DeleteAllFilesCmd_Success, FM_Test_Setup, FM_Test_Teardown, "Test_FM_DeleteAllFilesCmd_Success");

    UtTest_Add(Test_FM_DeleteAllFilesCmd_DirNoExist, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_DeleteAllFilesCmd_DirNoExist");

    UtTest_Add(Test_FM_DeleteAllFilesCmd_NoChildTask, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_DeleteAllFilesCmd_NoChildTask");
}

/****************************/
/* Decompress File Test s   */
/****************************/

void Test_FM_DecompressFileCmd_Success(void)
{
    bool Result;

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_DecompressFileCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_DecompressFileCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, FM_DECOMPRESS_FILE_CC);
}

void Test_FM_DecompressFileCmd_SourceFileOpen(void)
{
    bool Result;

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_DecompressFileCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_DecompressFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_DecompressFileCmd_TargetFileExists(void)
{
    bool Result;

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_DecompressFileCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_DecompressFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_DecompressFileCmd_NoChildTask(void)
{
    bool Result;

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    Result = FM_DecompressFileCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_DecompressFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void add_FM_DecompressFileCmd_tests(void)
{
    UtTest_Add(Test_FM_DecompressFileCmd_Success, FM_Test_Setup, FM_Test_Teardown, "Test_FM_DecompressFileCmd_Success");

    UtTest_Add(Test_FM_DecompressFileCmd_SourceFileOpen, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_DecompressFileCmd_SourceFileOpen");

    UtTest_Add(Test_FM_DecompressFileCmd_TargetFileExists, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_DecompressFileCmd_TargetFileExists");

    UtTest_Add(Test_FM_DecompressFileCmd_NoChildTask, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_DecompressFileCmd_NoChildTask");
}

/****************************/
/* Concat Files Tests       */
/****************************/

void Test_FM_ConcatFilesCmd_Success(void)
{
    FM_TwoSourceOneTarget_Payload_t *CmdPtr;
    bool                             Result;

    CmdPtr = &UT_CmdBuf.ConcatFilesCmd.Payload;

    strncpy(CmdPtr->Source1, "src1", sizeof(CmdPtr->Source1) - 1);
    strncpy(CmdPtr->Source2, "src2", sizeof(CmdPtr->Source2) - 1);
    strncpy(CmdPtr->Target, "tgt", sizeof(CmdPtr->Target) - 1);

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_ConcatFilesCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_ConcatFilesCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, FM_CONCAT_FILES_CC);
}

void Test_FM_ConcatFilesCmd_SourceFile1NotClosed(void)
{
    bool Result;

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result                       = FM_ConcatFilesCmd(&UT_CmdBuf.Buf);
    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_ConcatFilesCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_ConcatFilesCmd_SourceFile2NotClosed(void)
{
    bool Result;

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), true);
    UT_SetDeferredRetcode(UT_KEY(FM_VerifyFileClosed), 2, false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result                       = FM_ConcatFilesCmd(&UT_CmdBuf.Buf);
    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_ConcatFilesCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_ConcatFilesCmd_TargetFileExists(void)
{
    bool Result;

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result                       = FM_ConcatFilesCmd(&UT_CmdBuf.Buf);
    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_ConcatFilesCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_ConcatFilesCmd_NoChildTask(void)
{
    bool Result;

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    Result                       = FM_ConcatFilesCmd(&UT_CmdBuf.Buf);
    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_ConcatFilesCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void add_FM_ConcatFilesCmd_tests(void)
{
    UtTest_Add(Test_FM_ConcatFilesCmd_Success, FM_Test_Setup, FM_Test_Teardown, "Test_FM_ConcatFilesCmd_Success");

    UtTest_Add(Test_FM_ConcatFilesCmd_SourceFile1NotClosed, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ConcatFilesCmd_SourceFile1NotClosed");

    UtTest_Add(Test_FM_ConcatFilesCmd_SourceFile2NotClosed, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ConcatFilesCmd_SourceFile2NotClosed");

    UtTest_Add(Test_FM_ConcatFilesCmd_TargetFileExists, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ConcatFilesCmd_TargetFileExists");

    UtTest_Add(Test_FM_ConcatFilesCmd_NoChildTask, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ConcatFilesCmd_NoChildTask");
}

/****************************/
/* Get File Info Tests      */
/****************************/

void Test_FM_GetFileInfoCmd_Success(void)
{
    FM_FilenameAndCRC_Payload_t *CmdPtr;
    bool                         Result;

    CmdPtr = &UT_CmdBuf.GetFileInfoCmd.Payload;

    strncpy(CmdPtr->Filename, "file", sizeof(CmdPtr->Filename) - 1);

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyNameValid), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_GetFileInfoCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_GetFileInfoCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, FM_GET_FILE_INFO_CC);
}

void Test_FM_GetFileInfoCmd_InvalidName(void)
{
    bool Result;

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyNameValid), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_GetFileInfoCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_GetFileInfoCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_GetFileInfoCmd_NoChildTask(void)
{
    bool Result;

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyNameValid), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    Result = FM_GetFileInfoCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_GetFileInfoCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void add_FM_GetFileInfoCmd_tests(void)
{
    UtTest_Add(Test_FM_GetFileInfoCmd_Success, FM_Test_Setup, FM_Test_Teardown, "Test_FM_GetFileInfoCmd_Success");

    UtTest_Add(Test_FM_GetFileInfoCmd_InvalidName, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_GetFileInfoCmd_InvalidName");

    UtTest_Add(Test_FM_GetFileInfoCmd_NoChildTask, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_GetFileInfoCmd_NoChildTask");
}

/****************************/
/* Get Open Files Tests     */
/****************************/

void Test_FM_GetOpenFilesCmd_Success(void)
{
    int32 strCmpResult;
    char  ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH, "%%s command");

    bool Result = FM_GetOpenFilesCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_GetOpenFilesCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_OPEN_FILES_CMD_INF_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);
}

void add_FM_GetOpenFilesCmd_tests(void)
{
    UtTest_Add(Test_FM_GetOpenFilesCmd_Success, FM_Test_Setup, FM_Test_Teardown, "Test_FM_GetOpenFilesCmd_Success");
}

/****************************/
/* Create Directory Tests   */
/****************************/

void Test_FM_CreateDirectoryCmd_Success(void)
{
    FM_DirectoryName_Payload_t *CmdPtr;
    bool                        Result;

    CmdPtr = &UT_CmdBuf.CreateDirectoryCmd.Payload;

    strncpy(CmdPtr->Directory, "dir", sizeof(CmdPtr->Directory) - 1);
    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_CreateDirectoryCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_CreateDirectoryCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, FM_CREATE_DIRECTORY_CC);
}

void Test_FM_CreateDirectoryCmd_DirExists(void)
{
    bool Result;

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirNoExist), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_CreateDirectoryCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_CreateDirectoryCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_CreateDirectoryCmd_NoChildTask(void)
{
    bool Result;

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    Result = FM_CreateDirectoryCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_CreateDirectoryCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void add_FM_CreateDirectoryCmd_tests(void)
{
    UtTest_Add(Test_FM_CreateDirectoryCmd_Success, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_CreateDirectoryCmd_Success");

    UtTest_Add(Test_FM_CreateDirectoryCmd_DirExists, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_CreateDirectoryCmd_DirExists");

    UtTest_Add(Test_FM_CreateDirectoryCmd_NoChildTask, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_CreateDirectoryCmd_NoChildTask");
}

/****************************/
/* Delete Directory Tests   */
/****************************/

void Test_FM_DeleteDirectoryCmd_Success(void)
{
    FM_DirectoryName_Payload_t *CmdPtr;
    bool                        Result;

    CmdPtr = &UT_CmdBuf.DeleteDirectoryCmd.Payload;

    strncpy(CmdPtr->Directory, "dir", sizeof(CmdPtr->Directory) - 1);
    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_DeleteDirectoryCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_DeleteDirectoryCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, FM_DELETE_DIRECTORY_CC);
}

void Test_FM_DeleteDirectoryCmd_DirNoExist(void)
{
    bool Result;

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_DeleteDirectoryCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_DeleteDirectoryCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_DeleteDirectoryCmd_NoChildTask(void)
{
    bool Result;

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    Result = FM_DeleteDirectoryCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_DeleteDirectoryCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void add_FM_DeleteDirectoryCmd_tests(void)
{
    UtTest_Add(Test_FM_DeleteDirectoryCmd_Success, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_DeleteDirectoryCmd_Success");

    UtTest_Add(Test_FM_DeleteDirectoryCmd_DirNoExist, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_DeleteDirectoryCmd_DirNoExist");

    UtTest_Add(Test_FM_DeleteDirectoryCmd_NoChildTask, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_DeleteDirectoryCmd_NoChildTask");
}

/****************************/
/* Get Dir List File Tests  */
/****************************/

void Test_FM_GetDirListFileCmd_Success(void)
{
    FM_GetDirectoryToFile_Payload_t *CmdPtr;
    bool                             Result;

    CmdPtr = &UT_CmdBuf.GetDirListFileCmd.Payload;

    strncpy(CmdPtr->Filename, "file", sizeof(CmdPtr->Filename) - 1);
    strncpy(CmdPtr->Directory, "dir", sizeof(CmdPtr->Directory) - 1);
    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_GetDirListFileCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_GetDirListFileCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, FM_GET_DIR_LIST_FILE_CC);
}

void Test_FM_GetDirListFileCmd_SuccessDefaultPath(void)
{
    FM_GetDirectoryToFile_Payload_t *CmdPtr;
    bool                             Result;

    CmdPtr = &UT_CmdBuf.GetDirListFileCmd.Payload;

    strncpy(CmdPtr->Directory, "dir", sizeof(CmdPtr->Directory) - 1);
    CmdPtr->Filename[0]                     = '\0';
    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_GetDirListFileCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_GetDirListFileCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, FM_GET_DIR_LIST_FILE_CC);
}

void Test_FM_GetDirListFileCmd_SourceNotExist(void)
{
    bool Result;

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_GetDirListFileCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_GetDirListFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_GetDirListFileCmd_TargetFileOpen(void)
{
    FM_GetDirectoryToFile_Payload_t *CmdPtr;
    bool                             Result;

    CmdPtr = &UT_CmdBuf.GetDirListFileCmd.Payload;

    strncpy(CmdPtr->Filename, "file", sizeof(CmdPtr->Filename) - 1);
    strncpy(CmdPtr->Directory, "dir", sizeof(CmdPtr->Directory) - 1);

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_GetDirListFileCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_GetDirListFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_GetDirListFileCmd_NoChildTask(void)
{
    FM_GetDirectoryToFile_Payload_t *CmdPtr;
    bool                             Result;

    CmdPtr = &UT_CmdBuf.GetDirListFileCmd.Payload;

    strncpy(CmdPtr->Filename, "file", sizeof(CmdPtr->Filename) - 1);
    strncpy(CmdPtr->Directory, "dir", sizeof(CmdPtr->Directory) - 1);

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    Result = FM_GetDirListFileCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_GetDirListFileCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void add_FM_GetDirListFileCmd_tests(void)
{
    UtTest_Add(Test_FM_GetDirListFileCmd_Success, FM_Test_Setup, FM_Test_Teardown, "Test_FM_GetDirListFileCmd_Success");

    UtTest_Add(Test_FM_GetDirListFileCmd_SuccessDefaultPath, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_GetDirListFileCmd_SuccessDefaultPath");

    UtTest_Add(Test_FM_GetDirListFileCmd_SourceNotExist, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_GetDirListFileCmd_SourceNotExist");

    UtTest_Add(Test_FM_GetDirListFileCmd_TargetFileOpen, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_GetDirListFileCmd_TargetFileOpen");

    UtTest_Add(Test_FM_GetDirListFileCmd_NoChildTask, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_GetDirListFileCmd_NoChildTask");
}

/****************************/
/* Get Dir List Pkt Tests  */
/****************************/

void Test_FM_GetDirListPktCmd_Success(void)
{
    FM_GetDirectoryToPkt_Payload_t *CmdPtr;
    bool                            Result;

    CmdPtr = &UT_CmdBuf.GetDirListPktCmd.Payload;

    strncpy(CmdPtr->Directory, "dir", sizeof(CmdPtr->Directory) - 1);

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_GetDirListPktCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_GetDirListPktCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, FM_GET_DIR_LIST_PKT_CC);
}

void Test_FM_GetDirListPktCmd_SourceNotExist(void)
{
    bool Result;

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_GetDirListPktCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_GetDirListPktCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_GetDirListPktCmd_NoChildTask(void)
{
    bool Result;

    FM_GlobalData.ChildWriteIndex           = 0;
    FM_GlobalData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    Result = FM_GetDirListPktCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_GetDirListPktCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void add_FM_GetDirListPktCmd_tests(void)
{
    UtTest_Add(Test_FM_GetDirListPktCmd_Success, FM_Test_Setup, FM_Test_Teardown, "Test_FM_GetDirListPktCmd_Success");

    UtTest_Add(Test_FM_GetDirListPktCmd_SourceNotExist, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_GetDirListPktCmd_SourceNotExist");

    UtTest_Add(Test_FM_GetDirListPktCmd_NoChildTask, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_GetDirListPktCmd_NoChildTask");
}

void UT_Handler_MonitorSpace(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    uint64 *Bytes  = UT_Hook_GetArgValueByName(Context, "ByteCount", uint64 *);
    uint64 *Blocks = UT_Hook_GetArgValueByName(Context, "BlockCount", uint64 *);
    uint64 *Ref    = UserObj;

    *Blocks = *Ref;
    *Bytes  = *Ref * 100;
}

/****************************/
/* Get Free Space Tests     */
/****************************/

void Test_FM_MonitorFilesystemSpaceCmd_Success(void)
{
    FM_MonitorReportPkt_Payload_t *ReportPtr;

    int32 strCmpResult;
    char  ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    uint8 call_count_CFE_SB_TransmitMsg;
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH, "%%s command");

    FM_MonitorTable_t Table;
    uint64            RefVal1;
    uint64            RefVal2;

    RefVal1 = 20;
    RefVal2 = 10;

    memset(&Table, 0, sizeof(Table));

    Table.Entries[0].Type    = FM_MonitorTableEntry_Type_VOLUME_FREE_SPACE;
    Table.Entries[0].Enabled = FM_TABLE_ENTRY_ENABLED;
    Table.Entries[1].Type    = FM_MonitorTableEntry_Type_DIRECTORY_ESTIMATE;
    Table.Entries[1].Enabled = FM_TABLE_ENTRY_ENABLED;
    Table.Entries[2].Type    = FM_MonitorTableEntry_Type_VOLUME_FREE_SPACE;
    Table.Entries[2].Enabled = FM_TABLE_ENTRY_DISABLED;

    FM_GlobalData.MonitorTablePtr = &Table;

    UT_SetHandlerFunction(UT_KEY(FM_GetVolumeFreeSpace), UT_Handler_MonitorSpace, &RefVal1);
    UT_SetHandlerFunction(UT_KEY(FM_GetDirectorySpaceEstimate), UT_Handler_MonitorSpace, &RefVal2);

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    UtAssert_BOOL_TRUE(FM_MonitorFilesystemSpaceCmd(&UT_CmdBuf.Buf));

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_MONITOR_FILESYSTEM_SPACE_CMD_INF_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);

    call_count_CFE_SB_TransmitMsg = UT_GetStubCount(UT_KEY(CFE_SB_TransmitMsg));

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(call_count_CFE_SB_TransmitMsg, 1);

    ReportPtr = &FM_GlobalData.MonitorReportPkt.Payload;
    UtAssert_UINT32_EQ(ReportPtr->FileSys[0].Bytes, 2000);
    UtAssert_UINT32_EQ(ReportPtr->FileSys[0].Blocks, 20);
    UtAssert_UINT32_EQ(ReportPtr->FileSys[1].Bytes, 1000);
    UtAssert_UINT32_EQ(ReportPtr->FileSys[1].Blocks, 10);
    UtAssert_UINT32_EQ(ReportPtr->FileSys[2].Bytes, 0);
    UtAssert_UINT32_EQ(ReportPtr->FileSys[2].Blocks, 0);
}

void Test_FM_MonitorFilesystemSpaceCmd_NullFreeSpaceTable(void)
{
    int32 strCmpResult;
    char  ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    uint8 call_count_CFE_SB_TransmitMsg;
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "%%s error: file system free space table is not loaded");

    FM_GlobalData.MonitorTablePtr = NULL;

    UtAssert_BOOL_FALSE(FM_MonitorFilesystemSpaceCmd(&UT_CmdBuf.Buf));

    call_count_CFE_EVS_SendEvent  = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    call_count_CFE_SB_TransmitMsg = UT_GetStubCount(UT_KEY(CFE_SB_TransmitMsg));

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_FREE_SPACE_TBL_ERR_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);

    UtAssert_INT32_EQ(call_count_CFE_SB_TransmitMsg, 0);
}

void Test_FM_MonitorFilesystemSpaceCmd_ImplCallFails(void)
{
    FM_MonitorReportPkt_Payload_t *ReportPtr;
    FM_MonitorTable_t              Table;
    uint8                          call_count_CFE_SB_TransmitMsg;

    int32 strCmpResult;
    char  ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    char  ExpectedEventString2[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];

    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "Could not get file system free space for %%s. Returned 0x%%08X");
    snprintf(ExpectedEventString2, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH, "%%s command");

    memset(&Table, 0, sizeof(Table));
    Table.Entries[0].Type    = FM_MonitorTableEntry_Type_VOLUME_FREE_SPACE;
    Table.Entries[0].Enabled = true;

    FM_GlobalData.MonitorTablePtr = &Table;

    UT_SetDefaultReturnValue(UT_KEY(FM_GetVolumeFreeSpace), CFE_STATUS_EXTERNAL_RESOURCE_FAIL);

    /* Assert */
    UtAssert_BOOL_FALSE(FM_MonitorFilesystemSpaceCmd(&UT_CmdBuf.Buf));

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_MONITOR_FILESYSTEM_SPACE_CMD_INF_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);

    strCmpResult = strncmp(ExpectedEventString2, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);

    call_count_CFE_SB_TransmitMsg = UT_GetStubCount(UT_KEY(CFE_SB_TransmitMsg));

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(call_count_CFE_SB_TransmitMsg, 1);
    ReportPtr = &FM_GlobalData.MonitorReportPkt.Payload;
    UtAssert_ZERO(ReportPtr->FileSys[0].Blocks);
    UtAssert_ZERO(ReportPtr->FileSys[0].Bytes);
}

void Test_FM_MonitorFilesystemSpaceCmd_NotImpl(void)
{
    FM_MonitorReportPkt_Payload_t *ReportPtr;
    uint8                          call_count_CFE_SB_TransmitMsg;
    FM_MonitorTable_t              Table;

    int32 strCmpResult;
    char  ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    char  ExpectedEventString2[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];

    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "Could not get file system free space for %%s. Returned 0x%%08X");
    snprintf(ExpectedEventString2, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH, "%%s command");

    memset(&Table, 0, sizeof(Table));
    Table.Entries[0].Type    = 142;
    Table.Entries[0].Enabled = true;

    FM_GlobalData.MonitorTablePtr = &Table;

    /* Assert */
    UtAssert_BOOL_FALSE(FM_MonitorFilesystemSpaceCmd(&UT_CmdBuf.Buf));

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_MONITOR_FILESYSTEM_SPACE_CMD_INF_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);

    strCmpResult = strncmp(ExpectedEventString2, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);

    call_count_CFE_SB_TransmitMsg = UT_GetStubCount(UT_KEY(CFE_SB_TransmitMsg));

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(call_count_CFE_SB_TransmitMsg, 1);
    ReportPtr = &FM_GlobalData.MonitorReportPkt.Payload;
    UtAssert_ZERO(ReportPtr->FileSys[0].Blocks);
    UtAssert_ZERO(ReportPtr->FileSys[0].Bytes);
}

void add_FM_MonitorFilesystemSpaceCmd_tests(void)
{
    UtTest_Add(Test_FM_MonitorFilesystemSpaceCmd_Success, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_MonitorFilesystemSpaceCmd_Success");

    UtTest_Add(Test_FM_MonitorFilesystemSpaceCmd_NullFreeSpaceTable, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_MonitorFilesystemSpaceCmd_NullFreeSpaceTable");

    UtTest_Add(Test_FM_MonitorFilesystemSpaceCmd_ImplCallFails, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_MonitorFilesystemSpaceCmd_ImplCallFails");

    UtTest_Add(Test_FM_MonitorFilesystemSpaceCmd_NotImpl, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_MonitorFilesystemSpaceCmd_NotImpl");
}

/****************************/
/* Set Table State Cmd      */
/****************************/

void Test_FM_SetTableStateCmd_Success(void)
{
    FM_TableIndexAndState_Payload_t *CmdPtr;
    FM_MonitorTable_t                Table;

    CmdPtr = &UT_CmdBuf.SetTableStateCmd.Payload;

    CmdPtr->TableEntryState = FM_TABLE_ENTRY_ENABLED;
    CmdPtr->TableEntryIndex = 0;

    int32 strCmpResult;
    char  ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH, "%%s command: index = %%d, state = %%d");

    memset(&Table, 0, sizeof(Table));

    Table.Entries[0].Type         = FM_MonitorTableEntry_Type_VOLUME_FREE_SPACE;
    FM_GlobalData.MonitorTablePtr = &Table;

    UtAssert_BOOL_TRUE(FM_SetTableStateCmd(&UT_CmdBuf.Buf));

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_SET_TABLE_STATE_CMD_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(FM_GlobalData.MonitorTablePtr->Entries[0].Enabled, FM_TABLE_ENTRY_ENABLED);
}

void Test_FM_SetTableStateCmd_NullFreeSpaceTable(void)
{
    FM_TableIndexAndState_Payload_t *CmdPtr;
    bool                             Result;

    CmdPtr = &UT_CmdBuf.SetTableStateCmd.Payload;

    CmdPtr->TableEntryState = FM_TABLE_ENTRY_ENABLED;
    CmdPtr->TableEntryIndex = 0;

    int32 strCmpResult;
    char  ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "%%s error: file system free space table is not loaded");

    FM_GlobalData.MonitorTablePtr = NULL;

    Result = FM_SetTableStateCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_SetTableStateCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_SET_TABLE_STATE_TBL_ERR_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);
}

void Test_FM_SetTableStateCmd_TableEntryIndexTooLarge(void)
{
    FM_TableIndexAndState_Payload_t *CmdPtr;
    FM_MonitorTable_t                Table;
    bool                             Result;

    CmdPtr = &UT_CmdBuf.SetTableStateCmd.Payload;

    CmdPtr->TableEntryState = FM_TABLE_ENTRY_ENABLED;
    CmdPtr->TableEntryIndex = FM_TABLE_ENTRY_COUNT;

    int32 strCmpResult;
    char  ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "%%s error: invalid command argument: index = %%d");

    memset(&Table, 0, sizeof(Table));

    FM_GlobalData.MonitorTablePtr = &Table;

    Result = FM_SetTableStateCmd(&UT_CmdBuf.Buf);

    /* Assert */
    UtAssert_True(Result == false, "FM_SetTableStateCmd returned false");

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_SET_TABLE_STATE_ARG_IDX_ERR_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);
}

void Test_FM_SetTableStateCmd_BadNewState(void)
{
    FM_TableIndexAndState_Payload_t *CmdPtr;
    FM_MonitorTable_t                Table;
    bool                             Result;

    CmdPtr = &UT_CmdBuf.SetTableStateCmd.Payload;

    CmdPtr->TableEntryState = 55;
    CmdPtr->TableEntryIndex = 0;

    int32 strCmpResult;
    char  ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "%%s error: invalid command argument: state = %%d");

    memset(&Table, 0, sizeof(Table));

    FM_GlobalData.MonitorTablePtr = &Table;

    Result = FM_SetTableStateCmd(&UT_CmdBuf.Buf);

    /* Assert */
    UtAssert_True(Result == false, "FM_SetTableStateCmd returned false");

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_SET_TABLE_STATE_ARG_STATE_ERR_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);
}

void Test_FM_SetTableStateCmd_BadCurrentState(void)
{
    FM_TableIndexAndState_Payload_t *CmdPtr;
    FM_MonitorTable_t                Table;

    CmdPtr = &UT_CmdBuf.SetTableStateCmd.Payload;

    CmdPtr->TableEntryState = FM_TABLE_ENTRY_DISABLED;
    CmdPtr->TableEntryIndex = 0;

    int32 strCmpResult;
    char  ExpectedEventString[CFE_MISSION_EVS_MAX_MESSAGE_LENGTH];
    snprintf(ExpectedEventString, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH,
             "%%s error: cannot modify unused table entry: index = %%d");

    memset(&Table, 0, sizeof(Table));

    FM_GlobalData.MonitorTablePtr = &Table;

    UtAssert_BOOL_FALSE(FM_SetTableStateCmd(&UT_CmdBuf.Buf));

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_SET_TABLE_STATE_UNUSED_ERR_EID);

    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);

    strCmpResult = strncmp(ExpectedEventString, context_CFE_EVS_SendEvent[0].Spec, CFE_MISSION_EVS_MAX_MESSAGE_LENGTH);

    UtAssert_True(strCmpResult == 0, "Event string matched expected result, '%s'", context_CFE_EVS_SendEvent[0].Spec);

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 1);
}

void add_FM_SetTableStateCmd_tests(void)
{
    UtTest_Add(Test_FM_SetTableStateCmd_Success, FM_Test_Setup, FM_Test_Teardown, "Test_FM_SetTableStateCmd_Success");

    UtTest_Add(Test_FM_SetTableStateCmd_NullFreeSpaceTable, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_SetTableStateCmd_NullFreeSpaceTable");

    UtTest_Add(Test_FM_SetTableStateCmd_TableEntryIndexTooLarge, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_SetTableStateCmd_TableEntryIndexTooLarge");

    UtTest_Add(Test_FM_SetTableStateCmd_BadNewState, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_SetTableStateCmd_BadNewState");

    UtTest_Add(Test_FM_SetTableStateCmd_BadCurrentState, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_SetTableStateCmd_BadCurrentState");
}

/****************************/
/* Set Permissions Tests    */
/****************************/

void Test_FM_SetPermissionsCmd_Success(void)
{
    FM_FilenameAndMode_Payload_t *CmdPtr;
    bool                          Result;

    CmdPtr = &UT_CmdBuf.SetPermissionsCmd.Payload;

    strncpy(CmdPtr->FileName, "file", sizeof(CmdPtr->FileName) - 1);

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyNameValid), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_SetPermissionsCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == true, "FM_SetPermissionsCmd returned true");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, FM_SET_PERMISSIONS_CC);
}

void Test_FM_SetPermissionsCmd_BadName(void)
{
    bool Result;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyNameValid), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_SetPermissionsCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_SetPermissionsCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void Test_FM_SetPermissionsCmd_NoChildTask(void)
{
    bool Result;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyNameValid), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Result = FM_SetPermissionsCmd(&UT_CmdBuf.Buf);

    call_count_CFE_EVS_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    /* Assert */
    UtAssert_True(Result == false, "FM_SetPermissionsCmd returned false");

    UtAssert_INT32_EQ(call_count_CFE_EVS_SendEvent, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildQueue[0].CommandCode, 0);
}

void add_FM_SetPermissionsCmd_tests(void)
{
    UtTest_Add(Test_FM_SetPermissionsCmd_Success, FM_Test_Setup, FM_Test_Teardown, "Test_FM_SetPermissionsCmd_Success");

    UtTest_Add(Test_FM_SetPermissionsCmd_BadName, FM_Test_Setup, FM_Test_Teardown, "Test_FM_SetPermissionsCmd_BadName");

    UtTest_Add(Test_FM_SetPermissionsCmd_NoChildTask, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_SetPermissionsCmd_NoChildTask");
}

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    add_FM_NoopCmd_tests();
    add_FM_ResetCountersCmd_tests();
    add_FM_CopyFileCmd_tests();
    add_FM_MoveFileCmd_tests();
    add_FM_RenameFileCmd_tests();
    add_FM_DeleteFileCmd_tests();
    add_FM_DeleteAllFilesCmd_tests();
    add_FM_DecompressFileCmd_tests();
    add_FM_ConcatFilesCmd_tests();
    add_FM_GetFileInfoCmd_tests();
    add_FM_GetOpenFilesCmd_tests();
    add_FM_CreateDirectoryCmd_tests();
    add_FM_DeleteDirectoryCmd_tests();
    add_FM_GetDirListFileCmd_tests();
    add_FM_GetDirListPktCmd_tests();
    add_FM_MonitorFilesystemSpaceCmd_tests();
    add_FM_SetTableStateCmd_tests();
    add_FM_SetPermissionsCmd_tests();
}
