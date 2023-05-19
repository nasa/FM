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
#include "fm_events.h"
#include "fm_dispatch.h"
#include "fm_cmds.h"
#include "fm_app.h"
#include "fm_test_utils.h"
#include <unistd.h>
#include <stdlib.h>
#include "cfe.h"

/*********************************************************************************
 *          TEST CASE FUNCTIONS
 *********************************************************************************/

/* ********************************
 * Process Command Tests
 * *******************************/
void Test_FM_ProcessCmd_NoopCmdCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_NOOP_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    length = sizeof(FM_NoopCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_NoopCmd), CFE_SUCCESS);

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
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_RESET_COUNTERS_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    length = sizeof(FM_ResetCountersCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_ResetCountersCmd), CFE_SUCCESS);

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
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_COPY_FILE_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    length = sizeof(FM_CopyFileCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_CopyFileCmd), CFE_SUCCESS);

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
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_MOVE_FILE_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    length = sizeof(FM_MoveFileCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_MoveFileCmd), CFE_SUCCESS);

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
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_RENAME_FILE_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    length = sizeof(FM_RenameFileCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_RenameFileCmd), CFE_SUCCESS);

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
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_DELETE_FILE_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    length = sizeof(FM_DeleteFileCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_DeleteFileCmd), CFE_SUCCESS);

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
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_DELETE_ALL_FILES_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    length = sizeof(FM_DeleteAllFilesCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_DeleteAllFilesCmd), CFE_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessCmd(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_DeleteAllFilesCmd, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandCounter, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandErrCounter, 0);
}

void Test_FM_ProcessCmd_DecompressFileCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_DECOMPRESS_FILE_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    length = sizeof(FM_DecompressFileCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_DecompressFileCmd), CFE_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessCmd(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_DecompressFileCmd, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandCounter, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandErrCounter, 0);
}

void Test_FM_ProcessCmd_ConcatFilesCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_CONCAT_FILES_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    length = sizeof(FM_ConcatFilesCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_ConcatFilesCmd), CFE_SUCCESS);

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
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_GET_FILE_INFO_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    length = sizeof(FM_GetFileInfoCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_GetFileInfoCmd), CFE_SUCCESS);

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
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_GET_OPEN_FILES_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    length = sizeof(FM_GetOpenFilesCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_GetOpenFilesCmd), CFE_SUCCESS);

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
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_CREATE_DIRECTORY_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    length = sizeof(FM_CreateDirectoryCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_CreateDirectoryCmd), CFE_SUCCESS);

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
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_DELETE_DIRECTORY_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    length = sizeof(FM_DeleteDirectoryCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_DeleteDirectoryCmd), CFE_SUCCESS);

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
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_GET_DIR_LIST_FILE_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    length = sizeof(FM_GetDirListFileCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_GetDirListFileCmd), CFE_SUCCESS);

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
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_GET_DIR_LIST_PKT_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    length = sizeof(FM_GetDirListPktCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_GetDirListPktCmd), CFE_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessCmd(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_GetDirListPktCmd, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandCounter, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandErrCounter, 0);
}

void Test_FM_ProcessCmd_MonitorFilesystemSpaceCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_MONITOR_FILESYSTEM_SPACE_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    length = sizeof(FM_MonitorFilesystemSpaceCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_MonitorFilesystemSpaceCmd), CFE_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessCmd(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(FM_MonitorFilesystemSpaceCmd, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandCounter, 1);
    UtAssert_INT32_EQ(FM_GlobalData.CommandErrCounter, 0);
}

void Test_FM_ProcessCmd_SetTableStateCCReturn(void)
{
    /* Arrange */
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_SET_TABLE_STATE_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    length = sizeof(FM_SetTableStateCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_SetTableStateCmd), CFE_SUCCESS);

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
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    fcn_code = FM_SET_PERMISSIONS_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    length = sizeof(FM_SetPermissionsCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_SetPermissionsCmd), CFE_SUCCESS);

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
    CFE_MSG_FcnCode_t fcn_code;

    fcn_code = -1;
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

    UtTest_Add(Test_FM_ProcessCmd_DecompressFileCCReturn, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ProcessCmd_DecompressFileCCReturn");

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

    UtTest_Add(Test_FM_ProcessCmd_MonitorFilesystemSpaceCCReturn, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_PRocessCmd_MonitorFilesystemSpaceCCReturn");

    UtTest_Add(Test_FM_ProcessCmd_SetTableStateCCReturn, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_PRocessCmd_SetTableStateCCReturn");

    UtTest_Add(Test_FM_ProcessCmd_SetPermissionsCCReturn, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_PRocessCmd_SetPermissionsCCReturn");

    UtTest_Add(Test_FM_ProcessCmd_DefaultReturn, FM_Test_Setup, FM_Test_Teardown, "Test_FM_PRocessCmd_DefaultReturn");
}

/* ********************************
 * ProcessPkt Tests
 * ********************************/
void Test_FM_ProcessPkt_CheckMessageReturnHKRequest(void)
{
    /* Arrange */
    CFE_SB_MsgId_t    msgid = CFE_SB_ValueToMsgId(FM_SEND_HK_MID);
    CFE_MSG_FcnCode_t fcn_code;
    size_t            length;

    msgid = CFE_SB_ValueToMsgId(FM_SEND_HK_MID);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetMsgId), &msgid, sizeof(msgid), false);
    fcn_code = 0;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &fcn_code, sizeof(fcn_code), false);
    length = sizeof(FM_SendHkCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);

    /* Act */
    UtAssert_VOIDCALL(FM_ProcessPkt(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_ProcessPkt_CheckMessageReturnGroundCommand(void)
{
    /* Arrange */
    CFE_SB_MsgId_t    msgid = CFE_SB_ValueToMsgId(FM_CMD_MID);
    CFE_MSG_FcnCode_t fcn_code;

    fcn_code = -1;
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

/* * * * * * * * * * * * * *
 * Add Method Tests
 * * * * * * * * * * * * * */
void add_FM_ProcessPkt_tests(void)
{
    UtTest_Add(Test_FM_ProcessPkt_CheckMessageReturnHKRequest, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ProcessPkt_ReportHK");

    UtTest_Add(Test_FM_ProcessPkt_CheckMessageReturnGroundCommand, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ProcessPkt_CheckMessageReturnGroundCommand");

    UtTest_Add(Test_FM_ProcessPkt_CheckDefaultSwitchMessage, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ProcessPkt_CheckDefaultSwitchMessage");
}

/*****************
 * IsValidCmdPktLength Tests
 ****************/
void Test_FM_IsValidCmdPktLength(void)
{
    size_t length  = 5;
    uint32 eventid = 1;

    /* Matching length */
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_BOOL_TRUE(FM_IsValidCmdPktLength(NULL, length, 1, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);

    /* Mismatched length */
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_BOOL_FALSE(FM_IsValidCmdPktLength(NULL, length + 1, eventid, "Cmd Text"));
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, eventid);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
}

/*****************
 * Bad Length Tests for each command
 ****************/

void Test_FM_NoopVerifyDispatch(void)
{
    size_t length;

    UT_SetDefaultReturnValue(UT_KEY(FM_NoopCmd), CFE_SUCCESS);

    length = 1; /* bad size for any message */
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_NoopVerifyDispatch(&UT_CmdBuf.NoopCmd), CFE_STATUS_VALIDATION_FAILURE);

    length = sizeof(FM_NoopCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_NoopVerifyDispatch(&UT_CmdBuf.NoopCmd), CFE_SUCCESS);
}

void Test_FM_ResetCountersVerifyDispatch(void)
{
    size_t length;

    UT_SetDefaultReturnValue(UT_KEY(FM_ResetCountersCmd), CFE_SUCCESS);

    length = 1; /* bad size for any message */
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_ResetCountersVerifyDispatch(&UT_CmdBuf.ResetCountersCmd), CFE_STATUS_VALIDATION_FAILURE);

    length = sizeof(FM_ResetCountersCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_ResetCountersVerifyDispatch(&UT_CmdBuf.ResetCountersCmd), CFE_SUCCESS);
}

void Test_FM_CopyFileVerifyDispatch(void)
{
    size_t length;

    UT_SetDefaultReturnValue(UT_KEY(FM_CopyFileCmd), CFE_SUCCESS);

    length = 1; /* bad size for any message */
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_CopyFileVerifyDispatch(&UT_CmdBuf.CopyFileCmd), CFE_STATUS_VALIDATION_FAILURE);

    length = sizeof(FM_CopyFileCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_CopyFileVerifyDispatch(&UT_CmdBuf.CopyFileCmd), CFE_SUCCESS);
}

void Test_FM_MoveFileVerifyDispatch(void)
{
    size_t length;

    UT_SetDefaultReturnValue(UT_KEY(FM_MoveFileCmd), CFE_SUCCESS);

    length = 1; /* bad size for any message */
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_MoveFileVerifyDispatch(&UT_CmdBuf.MoveFileCmd), CFE_STATUS_VALIDATION_FAILURE);

    length = sizeof(FM_MoveFileCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_MoveFileVerifyDispatch(&UT_CmdBuf.MoveFileCmd), CFE_SUCCESS);
}

void Test_FM_RenameFileVerifyDispatch(void)
{
    size_t length;

    UT_SetDefaultReturnValue(UT_KEY(FM_RenameFileCmd), CFE_SUCCESS);

    length = 1; /* bad size for any message */
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_RenameFileVerifyDispatch(&UT_CmdBuf.RenameFileCmd), CFE_STATUS_VALIDATION_FAILURE);

    length = sizeof(FM_RenameFileCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_RenameFileVerifyDispatch(&UT_CmdBuf.RenameFileCmd), CFE_SUCCESS);
}

void Test_FM_DeleteFileVerifyDispatch(void)
{
    size_t length;

    UT_SetDefaultReturnValue(UT_KEY(FM_DeleteFileCmd), CFE_SUCCESS);

    length = 1; /* bad size for any message */
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_DeleteFileVerifyDispatch(&UT_CmdBuf.DeleteFileCmd), CFE_STATUS_VALIDATION_FAILURE);

    length = sizeof(FM_DeleteFileCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_DeleteFileVerifyDispatch(&UT_CmdBuf.DeleteFileCmd), CFE_SUCCESS);
}

void Test_FM_DeleteAllFilesVerifyDispatch(void)
{
    size_t length;

    UT_SetDefaultReturnValue(UT_KEY(FM_DeleteAllFilesCmd), CFE_SUCCESS);

    length = 1; /* bad size for any message */
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_DeleteAllFilesVerifyDispatch(&UT_CmdBuf.DeleteAllFilesCmd), CFE_STATUS_VALIDATION_FAILURE);

    length = sizeof(FM_DeleteAllFilesCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_DeleteAllFilesVerifyDispatch(&UT_CmdBuf.DeleteAllFilesCmd), CFE_SUCCESS);
}

void Test_FM_DecompressFileVerifyDispatch(void)
{
    size_t length;

    UT_SetDefaultReturnValue(UT_KEY(FM_DecompressFileCmd), CFE_SUCCESS);

    length = 1; /* bad size for any message */
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_DecompressFileVerifyDispatch(&UT_CmdBuf.DecompressFileCmd), CFE_STATUS_VALIDATION_FAILURE);

    length = sizeof(FM_DecompressFileCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_DecompressFileVerifyDispatch(&UT_CmdBuf.DecompressFileCmd), CFE_SUCCESS);
}

void Test_FM_ConcatFilesVerifyDispatch(void)
{
    size_t length;

    UT_SetDefaultReturnValue(UT_KEY(FM_ConcatFilesCmd), CFE_SUCCESS);

    length = 1; /* bad size for any message */
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_ConcatFilesVerifyDispatch(&UT_CmdBuf.ConcatFilesCmd), CFE_STATUS_VALIDATION_FAILURE);

    length = sizeof(FM_ConcatFilesCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_ConcatFilesVerifyDispatch(&UT_CmdBuf.ConcatFilesCmd), CFE_SUCCESS);
}

void Test_FM_GetFileInfoVerifyDispatch(void)
{
    size_t length;

    UT_SetDefaultReturnValue(UT_KEY(FM_GetFileInfoCmd), CFE_SUCCESS);

    length = 1; /* bad size for any message */
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_GetFileInfoVerifyDispatch(&UT_CmdBuf.GetFileInfoCmd), CFE_STATUS_VALIDATION_FAILURE);

    length = sizeof(FM_GetFileInfoCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_GetFileInfoVerifyDispatch(&UT_CmdBuf.GetFileInfoCmd), CFE_SUCCESS);
}

void Test_FM_GetOpenFilesVerifyDispatch(void)
{
    size_t length;

    UT_SetDefaultReturnValue(UT_KEY(FM_GetOpenFilesCmd), CFE_SUCCESS);

    length = 1; /* bad size for any message */
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_GetOpenFilesVerifyDispatch(&UT_CmdBuf.GetOpenFilesCmd), CFE_STATUS_VALIDATION_FAILURE);

    length = sizeof(FM_GetOpenFilesCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_GetOpenFilesVerifyDispatch(&UT_CmdBuf.GetOpenFilesCmd), CFE_SUCCESS);
}

void Test_FM_CreateDirectoryVerifyDispatch(void)
{
    size_t length;

    UT_SetDefaultReturnValue(UT_KEY(FM_CreateDirectoryCmd), CFE_SUCCESS);

    length = 1; /* bad size for any message */
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_CreateDirectoryVerifyDispatch(&UT_CmdBuf.CreateDirectoryCmd), CFE_STATUS_VALIDATION_FAILURE);

    length = sizeof(FM_CreateDirectoryCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_CreateDirectoryVerifyDispatch(&UT_CmdBuf.CreateDirectoryCmd), CFE_SUCCESS);
}

void Test_FM_DeleteDirectoryVerifyDispatch(void)
{
    size_t length;

    UT_SetDefaultReturnValue(UT_KEY(FM_DeleteDirectoryCmd), CFE_SUCCESS);

    length = 1; /* bad size for any message */
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_DeleteDirectoryVerifyDispatch(&UT_CmdBuf.DeleteDirectoryCmd), CFE_STATUS_VALIDATION_FAILURE);

    length = sizeof(FM_DeleteDirectoryCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_DeleteDirectoryVerifyDispatch(&UT_CmdBuf.DeleteDirectoryCmd), CFE_SUCCESS);
}

void Test_FM_GetDirListFileVerifyDispatch(void)
{
    size_t length;

    UT_SetDefaultReturnValue(UT_KEY(FM_GetDirListFileCmd), CFE_SUCCESS);

    length = 1; /* bad size for any message */
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_GetDirListFileVerifyDispatch(&UT_CmdBuf.GetDirListFileCmd), CFE_STATUS_VALIDATION_FAILURE);

    length = sizeof(FM_GetDirListFileCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_GetDirListFileVerifyDispatch(&UT_CmdBuf.GetDirListFileCmd), CFE_SUCCESS);
}

void Test_FM_GetDirListPktVerifyDispatch(void)
{
    size_t length;

    UT_SetDefaultReturnValue(UT_KEY(FM_GetDirListPktCmd), CFE_SUCCESS);

    length = 1; /* bad size for any message */
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_GetDirListPktVerifyDispatch(&UT_CmdBuf.GetDirListPktCmd), CFE_STATUS_VALIDATION_FAILURE);

    length = sizeof(FM_GetDirListPktCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_GetDirListPktVerifyDispatch(&UT_CmdBuf.GetDirListPktCmd), CFE_SUCCESS);
}

void Test_FM_MonitorFilesystemSpaceVerifyDispatch(void)
{
    size_t length;

    UT_SetDefaultReturnValue(UT_KEY(FM_MonitorFilesystemSpaceCmd), CFE_SUCCESS);

    length = 1; /* bad size for any message */
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_MonitorFilesystemSpaceVerifyDispatch(&UT_CmdBuf.GetFreeSpaceCmd),
                      CFE_STATUS_VALIDATION_FAILURE);

    length = sizeof(FM_MonitorFilesystemSpaceCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_MonitorFilesystemSpaceVerifyDispatch(&UT_CmdBuf.GetFreeSpaceCmd), CFE_SUCCESS);
}

void Test_FM_SetTableStateVerifyDispatch(void)
{
    size_t length;

    UT_SetDefaultReturnValue(UT_KEY(FM_SetTableStateCmd), CFE_SUCCESS);

    length = 1; /* bad size for any message */
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_SetTableStateVerifyDispatch(&UT_CmdBuf.SetTableStateCmd), CFE_STATUS_VALIDATION_FAILURE);

    length = sizeof(FM_SetTableStateCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_SetTableStateVerifyDispatch(&UT_CmdBuf.SetTableStateCmd), CFE_SUCCESS);
}

void Test_FM_SetPermissionsVerifyDispatch(void)
{
    size_t length;

    UT_SetDefaultReturnValue(UT_KEY(FM_SetPermissionsCmd), CFE_SUCCESS);

    length = 1; /* bad size for any message */
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_SetPermissionsVerifyDispatch(&UT_CmdBuf.SetPermissionsCmd), CFE_STATUS_VALIDATION_FAILURE);

    length = sizeof(FM_SetPermissionsCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_INT32_EQ(FM_SetPermissionsVerifyDispatch(&UT_CmdBuf.SetPermissionsCmd), CFE_SUCCESS);
}

void Test_FM_SendHkVerifyDispatch(void)
{
    size_t length;

    length = 1; /* bad size for any message */
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_VOIDCALL(FM_SendHkVerifyDispatch(&UT_CmdBuf.SendHkCmd));

    length = sizeof(FM_SendHkCmd_t);
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetSize), &length, sizeof(length), false);
    UtAssert_VOIDCALL(FM_SendHkVerifyDispatch(&UT_CmdBuf.SendHkCmd));
}

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    add_FM_ProcessPkt_tests();
    add_FM_ProcessCmd_tests();

    UtTest_Add(Test_FM_IsValidCmdPktLength, FM_Test_Setup, FM_Test_Teardown, "Test_FM_IsValidCmdPktLength");

    UtTest_Add(Test_FM_NoopVerifyDispatch, FM_Test_Setup, FM_Test_Teardown, "Test_FM_NoopVerifyDispatch");
    UtTest_Add(Test_FM_ResetCountersVerifyDispatch, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ResetCountersVerifyDispatch");
    UtTest_Add(Test_FM_CopyFileVerifyDispatch, FM_Test_Setup, FM_Test_Teardown, "Test_FM_CopyFileVerifyDispatch");

    UtTest_Add(Test_FM_MoveFileVerifyDispatch, FM_Test_Setup, FM_Test_Teardown, "Test_FM_MoveFileVerifyDispatch");

    UtTest_Add(Test_FM_RenameFileVerifyDispatch, FM_Test_Setup, FM_Test_Teardown, "Test_FM_RenameFileVerifyDispatch");
    UtTest_Add(Test_FM_DeleteFileVerifyDispatch, FM_Test_Setup, FM_Test_Teardown, "Test_FM_DeleteFileVerifyDispatch");
    UtTest_Add(Test_FM_DeleteAllFilesVerifyDispatch, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_DeleteAllFilesVerifyDispatch");

    UtTest_Add(Test_FM_DecompressFileVerifyDispatch, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_DecompressFileVerifyDispatch");

    UtTest_Add(Test_FM_ConcatFilesVerifyDispatch, FM_Test_Setup, FM_Test_Teardown, "Test_FM_ConcatFilesVerifyDispatch");

    UtTest_Add(Test_FM_GetFileInfoVerifyDispatch, FM_Test_Setup, FM_Test_Teardown, "Test_FM_GetFileInfoVerifyDispatch");

    UtTest_Add(Test_FM_GetOpenFilesVerifyDispatch, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_GetOpenFilesVerifyDispatch");

    UtTest_Add(Test_FM_CreateDirectoryVerifyDispatch, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_CreateDirectoryVerifyDispatch");

    UtTest_Add(Test_FM_DeleteDirectoryVerifyDispatch, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_DeleteDirectoryVerifyDispatch");

    UtTest_Add(Test_FM_GetDirListFileVerifyDispatch, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_GetDirListFileVerifyDispatch");

    UtTest_Add(Test_FM_GetDirListPktVerifyDispatch, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_GetDirListPktVerifyDispatch");

    UtTest_Add(Test_FM_MonitorFilesystemSpaceVerifyDispatch, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_MonitorFilesystemSpaceVerifyDispatch");

    UtTest_Add(Test_FM_SetTableStateVerifyDispatch, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_SetTableStateVerifyDispatch");

    UtTest_Add(Test_FM_SetPermissionsVerifyDispatch, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_SetPermissionsVerifyDispatch");

    UtTest_Add(Test_FM_SendHkVerifyDispatch, FM_Test_Setup, FM_Test_Teardown, "Test_FM_SendHkVerifyDispatch");
}
