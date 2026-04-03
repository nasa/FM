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
#include "fm_eventids.h"
#include "fm_dispatch.h"
#include "fm_cmds.h"
#include "fm_app.h"
#include "fm_test_utils.h"
#include "fm_topicids.h"
#include "fm_fcncodes.h"
#include <unistd.h>
#include <stdlib.h>
#include "cfe.h"

/*********************************************************************************
 *          TEST CASE FUNCTIONS
 *********************************************************************************/

/* ********************************
 * Verify Command Length Tests
 * *******************************/

void Test_FM_VerifyCmdLength_Nominal(void)
{
    bool              Result;
    size_t            ExpectedLen;
    CFE_SB_MsgId_t    MsgId;
    CFE_MSG_FcnCode_t FcnCode;

    /* Set up values for test */
    ExpectedLen = sizeof(FM_NoopCmd_t);
    MsgId       = CFE_SB_MSGID_C(FM_MISSION_CMD_TOPICID);
    FcnCode     = FM_NOOP_CC;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &ExpectedLen, sizeof(size_t), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(CFE_SB_MsgId_t), false);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(CFE_MSG_FcnCode_t), false);

    /* Run function under test */
    Result = FM_VerifyCmdLength(NULL, sizeof(FM_NoopCmd_t));

    /* Evaluate run */
    UtAssert_BOOL_TRUE(Result);

    UtAssert_EQ(uint8, FM_AppData.HkTlm.Payload.CommandCounter, 0);
    UtAssert_EQ(uint8, FM_AppData.HkTlm.Payload.CommandErrorCounter, 0);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_VerifyCmdLength_InvalidSize(void)
{
    size_t            ExpectedLen;
    CFE_SB_MsgId_t    MsgId;
    CFE_MSG_FcnCode_t FcnCode;

    /* Set up values for test */
    /* Since every case in the switch statement has a conditional this needs to call each one */
    for (FcnCode = 0; FcnCode < FM_NUM_COMMANDS; ++FcnCode)
    {
        FM_Test_Setup();

        FM_AppData.HkTlm.Payload.CommandErrorCounter = 0;

        ExpectedLen = 1;
        MsgId       = CFE_SB_MSGID_C(FM_MISSION_CMD_TOPICID);

        UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &ExpectedLen, sizeof(size_t), false);
        UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &MsgId, sizeof(CFE_SB_MsgId_t), false);
        UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(CFE_MSG_FcnCode_t), false);
        UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &FcnCode, sizeof(CFE_MSG_FcnCode_t), false);

        /* Run function under test */
        UtAssert_VOIDCALL(FM_ProcessGroundCommand(NULL));

        UtAssert_EQ(uint8, FM_AppData.HkTlm.Payload.CommandCounter, 0);
        UtAssert_EQ(uint8, FM_AppData.HkTlm.Payload.CommandErrorCounter, 1);

        UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);

        if (context_CFE_EVS_SendEvent[0].EventID != FM_CC_ERR_EID)
        {
            FM_Test_Verify_Event(0,
                                 FM_CMD_LEN_ERR_EID,
                                 CFE_EVS_EventType_ERROR,
                                 "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u");
        }
    }
}

/* ********************************
 * Process Command Tests
 * *******************************/
void Test_FM_ProcessGroundCommand_NoopCmdCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_NOOP_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);

    length = sizeof(FM_NoopCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessGroundCommand(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_NoopCmd, 1);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandCounter, 0);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandErrorCounter, 0);
}

void Test_FM_ProcessGroundCommand_ResetCountersCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_RESET_COUNTERS_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);

    length = sizeof(FM_ResetCountersCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessGroundCommand(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_ResetCountersCmd, 1);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandCounter, 0);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandErrorCounter, 0);
}

void Test_FM_ProcessGroundCommand_CopyFileCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_COPY_FILE_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);

    length = sizeof(FM_CopyFileCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessGroundCommand(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_CopyFileCmd, 1);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandCounter, 0);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandErrorCounter, 0);
}

void Test_FM_ProcessGroundCommand_MoveFileCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_MOVE_FILE_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);

    length = sizeof(FM_MoveFileCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessGroundCommand(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_MoveFileCmd, 1);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandCounter, 0);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandErrorCounter, 0);
}

void Test_FM_ProcessGroundCommand_RenameFileCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_RENAME_FILE_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);

    length = sizeof(FM_RenameFileCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessGroundCommand(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_RenameFileCmd, 1);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandCounter, 0);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandErrorCounter, 0);
}

void Test_FM_ProcessGroundCommand_DeleteFileCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_DELETE_FILE_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);

    length = sizeof(FM_DeleteFileCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessGroundCommand(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_DeleteFileCmd, 1);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandCounter, 0);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandErrorCounter, 0);
}

void Test_FM_ProcessGroundCommand_DeleteAllFilesCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_DELETE_ALL_FILES_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);

    length = sizeof(FM_DeleteAllFilesCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessGroundCommand(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_DeleteAllFilesCmd, 1);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandCounter, 0);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandErrorCounter, 0);
}

void Test_FM_ProcessGroundCommand_DecompressFileCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_DECOMPRESS_FILE_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);

    length = sizeof(FM_DecompressFileCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessGroundCommand(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_DecompressFileCmd, 1);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandCounter, 0);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandErrorCounter, 0);
}

void Test_FM_ProcessGroundCommand_ConcatFilesCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_CONCAT_FILES_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);

    length = sizeof(FM_ConcatFilesCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessGroundCommand(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_ConcatFilesCmd, 1);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandCounter, 0);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandErrorCounter, 0);
}

void Test_FM_ProcessGroundCommand_GetFileInfoCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_GET_FILE_INFO_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);

    length = sizeof(FM_GetFileInfoCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessGroundCommand(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_GetFileInfoCmd, 1);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandCounter, 0);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandErrorCounter, 0);
}

void Test_FM_ProcessGroundCommand_GetOpenFilesCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_GET_OPEN_FILES_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);

    length = sizeof(FM_GetOpenFilesCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessGroundCommand(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_GetOpenFilesCmd, 1);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandCounter, 0);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandErrorCounter, 0);
}

void Test_FM_ProcessGroundCommand_CreateDirectoryCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_CREATE_DIRECTORY_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);

    length = sizeof(FM_CreateDirectoryCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessGroundCommand(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_CreateDirectoryCmd, 1);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandCounter, 0);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandErrorCounter, 0);
}

void Test_FM_ProcessGroundCommand_DeleteDirectoryCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_DELETE_DIRECTORY_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);

    length = sizeof(FM_DeleteDirectoryCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessGroundCommand(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_DeleteDirectoryCmd, 1);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandCounter, 0);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandErrorCounter, 0);
}

void Test_FM_ProcessGroundCommand_GetDirListFileCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_GET_DIR_LIST_FILE_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);

    length = sizeof(FM_GetDirListFileCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessGroundCommand(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_GetDirListFileCmd, 1);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandCounter, 0);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandErrorCounter, 0);
}

void Test_FM_ProcessGroundCommand_GetDirListPktCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_GET_DIR_LIST_PKT_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);

    length = sizeof(FM_GetDirListPktCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessGroundCommand(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_GetDirListPktCmd, 1);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandCounter, 0);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandErrorCounter, 0);
}

void Test_FM_ProcessGroundCommand_MonitorFilesystemSpaceCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_MONITOR_FILESYSTEM_SPACE_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);

    length = sizeof(FM_MonitorFilesystemSpaceCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessGroundCommand(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_MonitorFilesystemSpaceCmd, 1);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandCounter, 0);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandErrorCounter, 0);
}

void Test_FM_ProcessGroundCommand_SetTableStateCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_SET_TABLE_STATE_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);

    length = sizeof(FM_SetTableStateCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessGroundCommand(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_SetTableStateCmd, 1);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandCounter, 0);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandErrorCounter, 0);
}

void Test_FM_ProcessGroundCommand_SetPermissionsCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_SET_PERMISSIONS_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);

    length = sizeof(FM_SetPermissionsCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessGroundCommand(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_SetPermissionsCmd, 1);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandCounter, 0);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandErrorCounter, 0);
}

void Test_FM_ProcessGroundCommand_DefaultReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code;

    fcn_code = -1;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessGroundCommand(NULL));

    /* Assert */
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandCounter, 0);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandErrorCounter, 1);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    FM_Test_Verify_Event(0, FM_CC_ERR_EID, CFE_EVS_EventType_ERROR, "Invalid ground command code: cc = %d");
}

/* ********************************
 * TaskPipe Tests
 * ********************************/
void Test_FM_TaskPipe_SendHkCmd(void)
{
    /* Arrange */
    CFE_SB_MsgId_t msgid;

    msgid = CFE_SB_ValueToMsgId(FM_SEND_HK_MID);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &msgid, sizeof(msgid), false);

    /* Act */
    UtAssert_VOIDCALL(FM_TaskPipe(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_SendHkCmd, 1);
}

void Test_FM_TaskPipe_ProcessGroundCommand(void)
{
    /* Arrange */
    CFE_SB_MsgId_t    msgid;
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_NOOP_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);

    length = sizeof(FM_NoopCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);

    msgid = CFE_SB_ValueToMsgId(FM_CMD_MID);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &msgid, sizeof(msgid), false);

    /* Act */
    UtAssert_VOIDCALL(FM_TaskPipe(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(FM_NoopCmd, 1);
}

void Test_FM_TaskPipe_CheckDefaultSwitchMessage(void)
{
    /* Arrange */
    CFE_SB_MsgId_t msgid = CFE_SB_INVALID_MSG_ID;

    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &msgid, sizeof(msgid), false);

    /* Act */
    UtAssert_VOIDCALL(FM_TaskPipe(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    FM_Test_Verify_Event(0, FM_MID_ERR_EID, CFE_EVS_EventType_ERROR, "Invalid command pipe message ID: 0x%08lX");
}

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(Test_FM_VerifyCmdLength_Nominal);
    ADD_TEST(Test_FM_VerifyCmdLength_InvalidSize);
    ADD_TEST(Test_FM_ProcessGroundCommand_NoopCmdCCReturn);
    ADD_TEST(Test_FM_ProcessGroundCommand_ResetCountersCCReturn);
    ADD_TEST(Test_FM_ProcessGroundCommand_CopyFileCCReturn);
    ADD_TEST(Test_FM_ProcessGroundCommand_MoveFileCCReturn);
    ADD_TEST(Test_FM_ProcessGroundCommand_RenameFileCCReturn);
    ADD_TEST(Test_FM_ProcessGroundCommand_DeleteFileCCReturn);
    ADD_TEST(Test_FM_ProcessGroundCommand_DeleteAllFilesCCReturn);
    ADD_TEST(Test_FM_ProcessGroundCommand_DecompressFileCCReturn);
    ADD_TEST(Test_FM_ProcessGroundCommand_ConcatFilesCCReturn);
    ADD_TEST(Test_FM_ProcessGroundCommand_GetFileInfoCCReturn);
    ADD_TEST(Test_FM_ProcessGroundCommand_GetOpenFilesCCReturn);
    ADD_TEST(Test_FM_ProcessGroundCommand_CreateDirectoryCCReturn);
    ADD_TEST(Test_FM_ProcessGroundCommand_DeleteDirectoryCCReturn);
    ADD_TEST(Test_FM_ProcessGroundCommand_GetDirListFileCCReturn);
    ADD_TEST(Test_FM_ProcessGroundCommand_GetDirListPktCCReturn);
    ADD_TEST(Test_FM_ProcessGroundCommand_MonitorFilesystemSpaceCCReturn);
    ADD_TEST(Test_FM_ProcessGroundCommand_SetTableStateCCReturn);
    ADD_TEST(Test_FM_ProcessGroundCommand_SetPermissionsCCReturn);
    ADD_TEST(Test_FM_ProcessGroundCommand_DefaultReturn);
    ADD_TEST(Test_FM_TaskPipe_SendHkCmd);
    ADD_TEST(Test_FM_TaskPipe_ProcessGroundCommand);
    ADD_TEST(Test_FM_TaskPipe_CheckDefaultSwitchMessage);
}
