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
 *  File Manager (FM) Child task (low priority command handler)
 */

#include "cfe.h"
#include "fm_msg.h"
#include "fm_msgdefs.h"
#include "fm_msgids.h"
#include "fm_eventids.h"
#include "fm_app.h"
#include "fm_child.h"
#include "fm_cmds.h"
#include "fm_cmd_utils.h"
#include "fm_perfids.h"
#include "fm_platform_cfg.h"
#include "fm_extern_typedefs.h"
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
    CFE_Status_t Status;
    FM_NoopCmd_t NoopCmd;

    memset(&NoopCmd, 0, sizeof(NoopCmd));

    Status = FM_NoopCmd(&NoopCmd);

    /* Verify results */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    FM_Test_Verify_Event(0, FM_NOOP_INF_EID, CFE_EVS_EventType_INFORMATION, "No-op command: FM version %d.%d.%d.%d");
}

/****************************/
/* Reset Counters Tests     */
/****************************/

void Test_FM_ResetCountersCmd_Success(void)
{
    CFE_Status_t          Status;
    FM_ResetCountersCmd_t ResetCountersCmd;

    memset(&ResetCountersCmd, 0, sizeof(ResetCountersCmd));

    FM_AppData.HkTlm.Payload.CommandCounter             = 1;
    FM_AppData.HkTlm.Payload.CommandErrorCounter        = 1;
    FM_AppData.HkTlm.Payload.ChildCommandCounter        = 1;
    FM_AppData.HkTlm.Payload.ChildCommandErrorCounter   = 1;
    FM_AppData.HkTlm.Payload.ChildCommandWarningCounter = 1;

    Status = FM_ResetCountersCmd(&ResetCountersCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandCounter, 0);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.CommandErrorCounter, 0);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.ChildCommandCounter, 0);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.ChildCommandErrorCounter, 0);
    UtAssert_INT32_EQ(FM_AppData.HkTlm.Payload.ChildCommandWarningCounter, 0);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    FM_Test_Verify_Event(0, FM_RESET_INF_EID, CFE_EVS_EventType_INFORMATION, "Reset Counters command");
}

/****************************/
/* Copy File Tests     */
/****************************/

void Test_FM_CopyFileCmd_Success(void)
{
    CFE_Status_t     Status;
    FM_CopyFileCmd_t CopyFileCmd;

    memset(&CopyFileCmd, 0, sizeof(CopyFileCmd));

    strncpy(CopyFileCmd.Payload.SourcePath, "src1", sizeof(CopyFileCmd.Payload.SourcePath) - 1);
    strncpy(CopyFileCmd.Payload.TargetPath, "tgt", sizeof(CopyFileCmd.Payload.TargetPath) - 1);
    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_CopyFileCmd(&CopyFileCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, FM_COPY_FILE_CC);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_CopyFileCmd_BadOverwrite(void)
{
    CFE_Status_t     Status;
    FM_CopyFileCmd_t CopyFileCmd;

    memset(&CopyFileCmd, 0, sizeof(CopyFileCmd));

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_CopyFileCmd(&CopyFileCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_CopyFileCmd_SourceNotExist(void)
{
    CFE_Status_t     Status;
    FM_CopyFileCmd_t CopyFileCmd;

    memset(&CopyFileCmd, 0, sizeof(CopyFileCmd));

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_CopyFileCmd(&CopyFileCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_CopyFileCmd_NoOverwriteTargetExists(void)
{
    CFE_Status_t     Status;
    FM_CopyFileCmd_t CopyFileCmd;

    memset(&CopyFileCmd, 0, sizeof(CopyFileCmd));

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_CopyFileCmd(&CopyFileCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_CopyFileCmd_OverwriteFileOpen(void)
{
    CFE_Status_t     Status;
    FM_CopyFileCmd_t CopyFileCmd;

    memset(&CopyFileCmd, 0, sizeof(CopyFileCmd));

    CopyFileCmd.Payload.Overwrite        = 1;
    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_CopyFileCmd(&CopyFileCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_CopyFileCmd_NoChildTask(void)
{
    CFE_Status_t     Status;
    FM_CopyFileCmd_t CopyFileCmd;

    memset(&CopyFileCmd, 0, sizeof(CopyFileCmd));

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    Status = FM_CopyFileCmd(&CopyFileCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/****************************/
/* Move File Tests          */
/****************************/

void Test_FM_MoveFileCmd_Success(void)
{
    CFE_Status_t     Status;
    FM_MoveFileCmd_t MoveFileCmd;

    memset(&MoveFileCmd, 0, sizeof(MoveFileCmd));

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_MoveFileCmd(&MoveFileCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, FM_MOVE_FILE_CC);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_MoveFileCmd_BadOverwrite(void)
{
    CFE_Status_t     Status;
    FM_MoveFileCmd_t MoveFileCmd;

    memset(&MoveFileCmd, 0, sizeof(MoveFileCmd));

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_MoveFileCmd(&MoveFileCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_MoveFileCmd_SourceNotExist(void)
{
    CFE_Status_t     Status;
    FM_MoveFileCmd_t MoveFileCmd;

    memset(&MoveFileCmd, 0, sizeof(MoveFileCmd));

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_MoveFileCmd(&MoveFileCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_MoveFileCmd_NoOverwriteTargetExists(void)
{
    CFE_Status_t     Status;
    FM_MoveFileCmd_t MoveFileCmd;

    memset(&MoveFileCmd, 0, sizeof(MoveFileCmd));

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_MoveFileCmd(&MoveFileCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_MoveFileCmd_OverwriteFileOpen(void)
{
    CFE_Status_t     Status;
    FM_MoveFileCmd_t MoveFileCmd;

    memset(&MoveFileCmd, 0, sizeof(MoveFileCmd));

    MoveFileCmd.Payload.Overwrite        = 1;
    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_MoveFileCmd(&MoveFileCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_MoveFileCmd_NoChildTask(void)
{
    CFE_Status_t     Status;
    FM_MoveFileCmd_t MoveFileCmd;

    memset(&MoveFileCmd, 0, sizeof(MoveFileCmd));

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyOverwrite), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    Status = FM_MoveFileCmd(&MoveFileCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/****************************/
/* Rename File Tests        */
/****************************/

void Test_FM_RenameFileCmd_Success(void)
{
    CFE_Status_t       Status;
    FM_RenameFileCmd_t RenameFileCmd;

    memset(&RenameFileCmd, 0, sizeof(RenameFileCmd));

    strncpy(RenameFileCmd.Payload.SourcePath, "src1", sizeof(RenameFileCmd.Payload.SourcePath) - 1);
    strncpy(RenameFileCmd.Payload.TargetPath, "tgt", sizeof(RenameFileCmd.Payload.TargetPath) - 1);

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_RenameFileCmd(&RenameFileCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, FM_RENAME_FILE_CC);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_RenameFileCmd_SourceNotExist(void)
{
    CFE_Status_t       Status;
    FM_RenameFileCmd_t RenameFileCmd;

    memset(&RenameFileCmd, 0, sizeof(RenameFileCmd));

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_RenameFileCmd(&RenameFileCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_RenameFileCmd_TargetExists(void)
{
    CFE_Status_t       Status;
    FM_RenameFileCmd_t RenameFileCmd;

    memset(&RenameFileCmd, 0, sizeof(RenameFileCmd));

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_RenameFileCmd(&RenameFileCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_RenameFileCmd_NoChildTask(void)
{
    CFE_Status_t       Status;
    FM_RenameFileCmd_t RenameFileCmd;

    memset(&RenameFileCmd, 0, sizeof(RenameFileCmd));

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    Status = FM_RenameFileCmd(&RenameFileCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/****************************/
/* Delete File Tests        */
/****************************/

void Test_FM_DeleteFileCmd_Success(void)
{
    CFE_Status_t       Status;
    FM_DeleteFileCmd_t DeleteFileCmd;

    memset(&DeleteFileCmd, 0, sizeof(DeleteFileCmd));

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    CFE_MSG_FcnCode_t forced_CmdCode = FM_DELETE_FILE_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &forced_CmdCode, sizeof(forced_CmdCode), false);

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_DeleteFileCmd(&DeleteFileCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, FM_DELETE_FILE_CC);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_DeleteFileCmd_FileNotClosed(void)
{
    CFE_Status_t       Status;
    FM_DeleteFileCmd_t DeleteFileCmd;

    memset(&DeleteFileCmd, 0, sizeof(DeleteFileCmd));

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    CFE_MSG_FcnCode_t forced_CmdCode = FM_DELETE_FILE_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &forced_CmdCode, sizeof(forced_CmdCode), false);

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_DeleteFileCmd(&DeleteFileCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_DeleteFileCmd_NoChildTask(void)
{
    CFE_Status_t       Status;
    FM_DeleteFileCmd_t DeleteFileCmd;

    memset(&DeleteFileCmd, 0, sizeof(DeleteFileCmd));

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    CFE_MSG_FcnCode_t forced_CmdCode = FM_DELETE_FILE_CC;
    UT_SetDataBuffer(UT_KEY(CFE_MSG_GetFcnCode), &forced_CmdCode, sizeof(forced_CmdCode), false);

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    Status = FM_DeleteFileCmd(&DeleteFileCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/****************************/
/* Delete All Files Tests   */
/****************************/

void Test_FM_DeleteAllFilesCmd_Success(void)
{
    CFE_Status_t           Status;
    FM_DeleteAllFilesCmd_t DeleteAllFilesCmd;

    memset(&DeleteAllFilesCmd, 0, sizeof(DeleteAllFilesCmd));

    strncpy(DeleteAllFilesCmd.Payload.Path, "dir", sizeof(DeleteAllFilesCmd.Payload.Path) - 1);

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_DeleteAllFilesCmd(&DeleteAllFilesCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, FM_DELETE_ALL_FILES_CC);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_DeleteAllFilesCmd_DirNoExist(void)
{
    CFE_Status_t           Status;
    FM_DeleteAllFilesCmd_t DeleteAllFilesCmd;

    memset(&DeleteAllFilesCmd, 0, sizeof(DeleteAllFilesCmd));

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_DeleteAllFilesCmd(&DeleteAllFilesCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_DeleteAllFilesCmd_NoChildTask(void)
{
    CFE_Status_t           Status;
    FM_DeleteAllFilesCmd_t DeleteAllFilesCmd;

    memset(&DeleteAllFilesCmd, 0, sizeof(DeleteAllFilesCmd));

    strncpy(DeleteAllFilesCmd.Payload.Path, "dir", sizeof(DeleteAllFilesCmd.Payload.Path) - 1);
    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    Status = FM_DeleteAllFilesCmd(&DeleteAllFilesCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/****************************/
/* Decompress File Test s   */
/****************************/

void Test_FM_DecompressFileCmd_Success(void)
{
    CFE_Status_t           Status;
    FM_DecompressFileCmd_t DecompressFileCmd;

    memset(&DecompressFileCmd, 0, sizeof(DecompressFileCmd));

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_DecompressFileCmd(&DecompressFileCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, FM_DECOMPRESS_FILE_CC);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_DecompressFileCmd_SourceFileOpen(void)
{
    CFE_Status_t           Status;
    FM_DecompressFileCmd_t DecompressFileCmd;

    memset(&DecompressFileCmd, 0, sizeof(DecompressFileCmd));

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_DecompressFileCmd(&DecompressFileCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_DecompressFileCmd_TargetFileExists(void)
{
    CFE_Status_t           Status;
    FM_DecompressFileCmd_t DecompressFileCmd;

    memset(&DecompressFileCmd, 0, sizeof(DecompressFileCmd));

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_DecompressFileCmd(&DecompressFileCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_DecompressFileCmd_NoChildTask(void)
{
    CFE_Status_t           Status;
    FM_DecompressFileCmd_t DecompressFileCmd;

    memset(&DecompressFileCmd, 0, sizeof(DecompressFileCmd));

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    Status = FM_DecompressFileCmd(&DecompressFileCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/****************************/
/* Concat Files Tests       */
/****************************/

void Test_FM_ConcatFilesCmd_Success(void)
{
    CFE_Status_t        Status;
    FM_ConcatFilesCmd_t ConcatFilesCmd;

    memset(&ConcatFilesCmd, 0, sizeof(ConcatFilesCmd));

    strncpy(ConcatFilesCmd.Payload.SourcePath1, "src1", sizeof(ConcatFilesCmd.Payload.SourcePath1) - 1);
    strncpy(ConcatFilesCmd.Payload.SourcePath2, "src2", sizeof(ConcatFilesCmd.Payload.SourcePath2) - 1);
    strncpy(ConcatFilesCmd.Payload.TargetPath, "tgt", sizeof(ConcatFilesCmd.Payload.TargetPath) - 1);

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_ConcatFilesCmd(&ConcatFilesCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, FM_CONCAT_FILES_CC);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_ConcatFilesCmd_SourceFile1NotClosed(void)
{
    CFE_Status_t        Status;
    FM_ConcatFilesCmd_t ConcatFilesCmd;

    memset(&ConcatFilesCmd, 0, sizeof(ConcatFilesCmd));

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_ConcatFilesCmd(&ConcatFilesCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_ConcatFilesCmd_SourceFile2NotClosed(void)
{
    CFE_Status_t        Status;
    FM_ConcatFilesCmd_t ConcatFilesCmd;

    memset(&ConcatFilesCmd, 0, sizeof(ConcatFilesCmd));

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), true);
    UT_SetDeferredRetcode(UT_KEY(FM_VerifyFileClosed), 2, false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_ConcatFilesCmd(&ConcatFilesCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_ConcatFilesCmd_TargetFileExists(void)
{
    CFE_Status_t        Status;
    FM_ConcatFilesCmd_t ConcatFilesCmd;

    memset(&ConcatFilesCmd, 0, sizeof(ConcatFilesCmd));

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_ConcatFilesCmd(&ConcatFilesCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_ConcatFilesCmd_NoChildTask(void)
{
    CFE_Status_t        Status;
    FM_ConcatFilesCmd_t ConcatFilesCmd;

    memset(&ConcatFilesCmd, 0, sizeof(ConcatFilesCmd));

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileClosed), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    Status = FM_ConcatFilesCmd(&ConcatFilesCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/****************************/
/* Get File Info Tests      */
/****************************/

void Test_FM_GetFileInfoCmd_Success(void)
{
    CFE_Status_t        Status;
    FM_GetFileInfoCmd_t GetFileInfoCmd;

    memset(&GetFileInfoCmd, 0, sizeof(GetFileInfoCmd));

    strncpy(GetFileInfoCmd.Payload.Path, "file", sizeof(GetFileInfoCmd.Payload.Path) - 1);

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyNameValid), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_GetFileInfoCmd(&GetFileInfoCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, FM_GET_FILE_INFO_CC);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_GetFileInfoCmd_InvalidName(void)
{
    CFE_Status_t        Status;
    FM_GetFileInfoCmd_t GetFileInfoCmd;

    memset(&GetFileInfoCmd, 0, sizeof(GetFileInfoCmd));

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyNameValid), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_GetFileInfoCmd(&GetFileInfoCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_GetFileInfoCmd_NoChildTask(void)
{
    CFE_Status_t        Status;
    FM_GetFileInfoCmd_t GetFileInfoCmd;

    memset(&GetFileInfoCmd, 0, sizeof(GetFileInfoCmd));

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyNameValid), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    Status = FM_GetFileInfoCmd(&GetFileInfoCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/****************************/
/* Get Open Files Tests     */
/****************************/

void Test_FM_GetOpenFilesCmd_Success(void)
{
    CFE_Status_t         Status;
    FM_GetOpenFilesCmd_t GetOpenFilesCmd;

    memset(&GetOpenFilesCmd, 0, sizeof(GetOpenFilesCmd));

    Status = FM_GetOpenFilesCmd(&GetOpenFilesCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);

    FM_Test_Verify_Event(0, FM_GET_OPEN_FILES_CMD_INF_EID, CFE_EVS_EventType_INFORMATION, "%s command");
}

/****************************/
/* Create Directory Tests   */
/****************************/

void Test_FM_CreateDirectoryCmd_Success(void)
{
    CFE_Status_t            Status;
    FM_CreateDirectoryCmd_t CreateDirectoryCmd;

    memset(&CreateDirectoryCmd, 0, sizeof(CreateDirectoryCmd));

    strncpy(CreateDirectoryCmd.Payload.Path, "dir", sizeof(CreateDirectoryCmd.Payload.Path) - 1);
    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_CreateDirectoryCmd(&CreateDirectoryCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, FM_CREATE_DIRECTORY_CC);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_CreateDirectoryCmd_DirExists(void)
{
    CFE_Status_t            Status;
    FM_CreateDirectoryCmd_t CreateDirectoryCmd;

    memset(&CreateDirectoryCmd, 0, sizeof(CreateDirectoryCmd));

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirNoExist), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_CreateDirectoryCmd(&CreateDirectoryCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_CreateDirectoryCmd_NoChildTask(void)
{
    CFE_Status_t            Status;
    FM_CreateDirectoryCmd_t CreateDirectoryCmd;

    memset(&CreateDirectoryCmd, 0, sizeof(CreateDirectoryCmd));

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirNoExist), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    Status = FM_CreateDirectoryCmd(&CreateDirectoryCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/****************************/
/* Delete Directory Tests   */
/****************************/

void Test_FM_DeleteDirectoryCmd_Success(void)
{
    CFE_Status_t            Status;
    FM_DeleteDirectoryCmd_t DeleteDirectoryCmd;

    memset(&DeleteDirectoryCmd, 0, sizeof(DeleteDirectoryCmd));

    strncpy(DeleteDirectoryCmd.Payload.Path, "dir", sizeof(DeleteDirectoryCmd.Payload.Path) - 1);
    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_DeleteDirectoryCmd(&DeleteDirectoryCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, FM_DELETE_DIRECTORY_CC);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_DeleteDirectoryCmd_DirNoExist(void)
{
    CFE_Status_t            Status;
    FM_DeleteDirectoryCmd_t DeleteDirectoryCmd;

    memset(&DeleteDirectoryCmd, 0, sizeof(DeleteDirectoryCmd));

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_DeleteDirectoryCmd(&DeleteDirectoryCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_DeleteDirectoryCmd_NoChildTask(void)
{
    CFE_Status_t            Status;
    FM_DeleteDirectoryCmd_t DeleteDirectoryCmd;

    memset(&DeleteDirectoryCmd, 0, sizeof(DeleteDirectoryCmd));

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    Status = FM_DeleteDirectoryCmd(&DeleteDirectoryCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/****************************/
/* Get Dir List File Tests  */
/****************************/

void Test_FM_GetDirListFileCmd_Success(void)
{
    CFE_Status_t           Status;
    FM_GetDirListFileCmd_t GetDirListFileCmd;

    memset(&GetDirListFileCmd, 0, sizeof(GetDirListFileCmd));

    strncpy(GetDirListFileCmd.Payload.OutputFilePath, "file", sizeof(GetDirListFileCmd.Payload.OutputFilePath) - 1);
    strncpy(GetDirListFileCmd.Payload.DirectoryPath, "dir", sizeof(GetDirListFileCmd.Payload.DirectoryPath) - 1);
    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_GetDirListFileCmd(&GetDirListFileCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, FM_GET_DIR_LIST_FILE_CC);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_GetDirListFileCmd_SuccessDefaultPath(void)
{
    CFE_Status_t           Status;
    FM_GetDirListFileCmd_t GetDirListFileCmd;

    memset(&GetDirListFileCmd, 0, sizeof(GetDirListFileCmd));

    strncpy(GetDirListFileCmd.Payload.DirectoryPath, "dir", sizeof(GetDirListFileCmd.Payload.DirectoryPath) - 1);
    GetDirListFileCmd.Payload.OutputFilePath[0] = '\0';
    FM_AppData.ChildWriteIndex                  = 0;
    FM_AppData.ChildQueue[0].CommandCode        = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_GetDirListFileCmd(&GetDirListFileCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, FM_GET_DIR_LIST_FILE_CC);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_GetDirListFileCmd_SourceNotExist(void)
{
    CFE_Status_t           Status;
    FM_GetDirListFileCmd_t GetDirListFileCmd;

    memset(&GetDirListFileCmd, 0, sizeof(GetDirListFileCmd));

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_GetDirListFileCmd(&GetDirListFileCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_GetDirListFileCmd_TargetFileOpen(void)
{
    CFE_Status_t           Status;
    FM_GetDirListFileCmd_t GetDirListFileCmd;

    memset(&GetDirListFileCmd, 0, sizeof(GetDirListFileCmd));

    strncpy(GetDirListFileCmd.Payload.OutputFilePath, "file", sizeof(GetDirListFileCmd.Payload.OutputFilePath) - 1);
    strncpy(GetDirListFileCmd.Payload.DirectoryPath, "dir", sizeof(GetDirListFileCmd.Payload.DirectoryPath) - 1);

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_GetDirListFileCmd(&GetDirListFileCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_GetDirListFileCmd_NoChildTask(void)
{
    CFE_Status_t           Status;
    FM_GetDirListFileCmd_t GetDirListFileCmd;

    memset(&GetDirListFileCmd, 0, sizeof(GetDirListFileCmd));

    strncpy(GetDirListFileCmd.Payload.OutputFilePath, "file", sizeof(GetDirListFileCmd.Payload.OutputFilePath) - 1);
    strncpy(GetDirListFileCmd.Payload.DirectoryPath, "dir", sizeof(GetDirListFileCmd.Payload.DirectoryPath) - 1);

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyFileNotOpen), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    Status = FM_GetDirListFileCmd(&GetDirListFileCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/****************************/
/* Get Dir List Pkt Tests  */
/****************************/

void Test_FM_GetDirListPktCmd_Success(void)
{
    CFE_Status_t          Status;
    FM_GetDirListPktCmd_t GetDirListPktCmd;

    memset(&GetDirListPktCmd, 0, sizeof(GetDirListPktCmd));

    strncpy(GetDirListPktCmd.Payload.Path, "dir", sizeof(GetDirListPktCmd.Payload.Path) - 1);

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_GetDirListPktCmd(&GetDirListPktCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, FM_GET_DIR_LIST_PKT_CC);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_GetDirListPktCmd_SourceNotExist(void)
{
    CFE_Status_t          Status;
    FM_GetDirListPktCmd_t GetDirListPktCmd;

    memset(&GetDirListPktCmd, 0, sizeof(GetDirListPktCmd));

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_GetDirListPktCmd(&GetDirListPktCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_GetDirListPktCmd_NoChildTask(void)
{
    CFE_Status_t          Status;
    FM_GetDirListPktCmd_t GetDirListPktCmd;

    memset(&GetDirListPktCmd, 0, sizeof(GetDirListPktCmd));

    FM_AppData.ChildWriteIndex           = 0;
    FM_AppData.ChildQueue[0].CommandCode = 0;

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), false);

    Status = FM_GetDirListPktCmd(&GetDirListPktCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
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
    FM_MonitorFilesystemSpaceCmd_t MonitorFilesystemSpaceCmd;
    FM_MonitorReportPkt_Payload_t *ReportPtr;

    uint8 call_count_CFE_SB_TransmitMsg;

    FM_MonitorTable_t Table;
    uint64            RefVal1;
    uint64            RefVal2;
    void             *TablePtr;

    RefVal1 = 20;
    RefVal2 = 10;

    memset(&Table, 0, sizeof(Table));
    memset(&MonitorFilesystemSpaceCmd, 0, sizeof(MonitorFilesystemSpaceCmd));

    Table.Entries[0].Type    = FM_MonitorTableEntry_Type_VOLUME_FREE_SPACE;
    Table.Entries[0].Enabled = FM_TABLE_ENTRY_ENABLED;
    Table.Entries[1].Type    = FM_MonitorTableEntry_Type_DIRECTORY_ESTIMATE;
    Table.Entries[1].Enabled = FM_TABLE_ENTRY_ENABLED;
    Table.Entries[2].Type    = FM_MonitorTableEntry_Type_VOLUME_FREE_SPACE;
    Table.Entries[2].Enabled = FM_TABLE_ENTRY_DISABLED;

    TablePtr = &Table;
    UT_SetDataBuffer(UT_KEY(CFE_TBL_GetAddress), &TablePtr, sizeof(TablePtr), false);

    UT_SetHandlerFunction(UT_KEY(FM_GetVolumeFreeSpace), UT_Handler_MonitorSpace, &RefVal1);
    UT_SetHandlerFunction(UT_KEY(FM_GetDirectorySpaceEstimate), UT_Handler_MonitorSpace, &RefVal2);

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyDirExists), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    FM_MonitorFilesystemSpaceCmd(&MonitorFilesystemSpaceCmd);

    call_count_CFE_SB_TransmitMsg = UT_GetStubCount(UT_KEY(CFE_SB_TransmitMsg));

    UtAssert_INT32_EQ(call_count_CFE_SB_TransmitMsg, 1);

    ReportPtr = &FM_AppData.MonitorReportPkt.Payload;
    UtAssert_UINT32_EQ(ReportPtr->FileSys[0].Bytes, 2000);
    UtAssert_UINT32_EQ(ReportPtr->FileSys[0].Blocks, 20);
    UtAssert_UINT32_EQ(ReportPtr->FileSys[1].Bytes, 1000);
    UtAssert_UINT32_EQ(ReportPtr->FileSys[1].Blocks, 10);
    UtAssert_UINT32_EQ(ReportPtr->FileSys[2].Bytes, 0);
    UtAssert_UINT32_EQ(ReportPtr->FileSys[2].Blocks, 0);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    FM_Test_Verify_Event(0, FM_MONITOR_FILESYSTEM_SPACE_CMD_INF_EID, CFE_EVS_EventType_INFORMATION, "%s command");
}

void Test_FM_MonitorFilesystemSpaceCmd_NullFreeSpaceTable(void)
{
    FM_MonitorFilesystemSpaceCmd_t MonitorFilesystemSpaceCmd;
    uint8                          call_count_CFE_SB_TransmitMsg;

    memset(&MonitorFilesystemSpaceCmd, 0, sizeof(MonitorFilesystemSpaceCmd));

    UT_SetDeferredRetcode(UT_KEY(CFE_TBL_GetAddress), 1, CFE_TBL_ERR_NEVER_LOADED);

    FM_MonitorFilesystemSpaceCmd(&MonitorFilesystemSpaceCmd);

    call_count_CFE_SB_TransmitMsg = UT_GetStubCount(UT_KEY(CFE_SB_TransmitMsg));
    UtAssert_INT32_EQ(call_count_CFE_SB_TransmitMsg, 0);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    FM_Test_Verify_Event(0,
                         FM_GET_FREE_SPACE_TBL_ERR_EID,
                         CFE_EVS_EventType_ERROR,
                         "%s error: file system free space table unavailable: status=%08x");
}

void Test_FM_MonitorFilesystemSpaceCmd_ImplCallFails(void)
{
    FM_MonitorFilesystemSpaceCmd_t MonitorFilesystemSpaceCmd;
    FM_MonitorReportPkt_Payload_t *ReportPtr;
    FM_MonitorTable_t              Table;
    uint8                          call_count_CFE_SB_TransmitMsg;
    void                          *TablePtr;

    memset(&Table, 0, sizeof(Table));
    memset(&MonitorFilesystemSpaceCmd, 0, sizeof(MonitorFilesystemSpaceCmd));

    Table.Entries[0].Type    = FM_MonitorTableEntry_Type_VOLUME_FREE_SPACE;
    Table.Entries[0].Enabled = true;

    TablePtr = &Table;
    UT_SetDataBuffer(UT_KEY(CFE_TBL_GetAddress), &TablePtr, sizeof(TablePtr), false);

    UT_SetDefaultReturnValue(UT_KEY(FM_GetVolumeFreeSpace), CFE_STATUS_EXTERNAL_RESOURCE_FAIL);

    /* Assert */
    FM_MonitorFilesystemSpaceCmd(&MonitorFilesystemSpaceCmd);

    call_count_CFE_SB_TransmitMsg = UT_GetStubCount(UT_KEY(CFE_SB_TransmitMsg));

    UtAssert_INT32_EQ(call_count_CFE_SB_TransmitMsg, 1);
    ReportPtr = &FM_AppData.MonitorReportPkt.Payload;
    UtAssert_ZERO(ReportPtr->FileSys[0].Blocks);
    UtAssert_ZERO(ReportPtr->FileSys[0].Bytes);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    FM_Test_Verify_Event(0, FM_MONITOR_FILESYSTEM_SPACE_CMD_INF_EID, CFE_EVS_EventType_INFORMATION, "%s command");
}

void Test_FM_MonitorFilesystemSpaceCmd_NotImpl(void)
{
    FM_MonitorFilesystemSpaceCmd_t MonitorFilesystemSpaceCmd;
    FM_MonitorReportPkt_Payload_t *ReportPtr;
    uint8                          call_count_CFE_SB_TransmitMsg;
    FM_MonitorTable_t              Table;
    void                          *TablePtr;

    memset(&Table, 0, sizeof(Table));
    memset(&MonitorFilesystemSpaceCmd, 0, sizeof(MonitorFilesystemSpaceCmd));

    Table.Entries[0].Type    = 142;
    Table.Entries[0].Enabled = true;

    TablePtr = &Table;
    UT_SetDataBuffer(UT_KEY(CFE_TBL_GetAddress), &TablePtr, sizeof(TablePtr), false);

    /* Assert */
    FM_MonitorFilesystemSpaceCmd(&MonitorFilesystemSpaceCmd);

    call_count_CFE_SB_TransmitMsg = UT_GetStubCount(UT_KEY(CFE_SB_TransmitMsg));

    UtAssert_INT32_EQ(call_count_CFE_SB_TransmitMsg, 1);
    ReportPtr = &FM_AppData.MonitorReportPkt.Payload;
    UtAssert_ZERO(ReportPtr->FileSys[0].Blocks);
    UtAssert_ZERO(ReportPtr->FileSys[0].Bytes);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    FM_Test_Verify_Event(0, FM_MONITOR_FILESYSTEM_SPACE_CMD_INF_EID, CFE_EVS_EventType_INFORMATION, "%s command");
}

/****************************/
/* Set Table State Cmd      */
/****************************/

void Test_FM_SetTableStateCmd_Success(void)
{
    CFE_Status_t          Status;
    FM_SetTableStateCmd_t SetTableStateCmd;
    FM_MonitorTable_t     Table;
    void                 *TablePtr;

    memset(&Table, 0, sizeof(Table));
    memset(&SetTableStateCmd, 0, sizeof(SetTableStateCmd));

    SetTableStateCmd.Payload.State = FM_TABLE_ENTRY_ENABLED;
    SetTableStateCmd.Payload.Index = 0;

    Table.Entries[0].Type = FM_MonitorTableEntry_Type_VOLUME_FREE_SPACE;

    TablePtr = &Table;
    UT_SetDataBuffer(UT_KEY(CFE_TBL_GetAddress), &TablePtr, sizeof(TablePtr), false);

    Status = FM_SetTableStateCmd(&SetTableStateCmd);

    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(Table.Entries[0].Enabled, FM_TABLE_ENTRY_ENABLED);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    FM_Test_Verify_Event(0,
                         FM_SET_TABLE_STATE_CMD_EID,
                         CFE_EVS_EventType_INFORMATION,
                         "%s command: index = %d, state = %d");
}

void Test_FM_SetTableStateCmd_NullFreeSpaceTable(void)
{
    CFE_Status_t          Status;
    FM_SetTableStateCmd_t SetTableStateCmd;

    memset(&SetTableStateCmd, 0, sizeof(SetTableStateCmd));

    SetTableStateCmd.Payload.State = FM_TABLE_ENTRY_ENABLED;
    SetTableStateCmd.Payload.Index = 0;

    UT_SetDeferredRetcode(UT_KEY(CFE_TBL_GetAddress), 1, CFE_TBL_ERR_NEVER_LOADED);

    Status = FM_SetTableStateCmd(&SetTableStateCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    FM_Test_Verify_Event(0,
                         FM_SET_TABLE_STATE_TBL_ERR_EID,
                         CFE_EVS_EventType_ERROR,
                         "%s error: file system free space table unavailable: status=%08x");
}

void Test_FM_SetTableStateCmd_IndexTooLarge(void)
{
    CFE_Status_t          Status;
    FM_SetTableStateCmd_t SetTableStateCmd;
    FM_MonitorTable_t     Table;
    void                 *TablePtr;

    memset(&Table, 0, sizeof(Table));
    memset(&SetTableStateCmd, 0, sizeof(SetTableStateCmd));

    SetTableStateCmd.Payload.State = FM_TABLE_ENTRY_ENABLED;
    SetTableStateCmd.Payload.Index = FM_TABLE_ENTRY_COUNT;

    TablePtr = &Table;
    UT_SetDataBuffer(UT_KEY(CFE_TBL_GetAddress), &TablePtr, sizeof(TablePtr), false);

    Status = FM_SetTableStateCmd(&SetTableStateCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    FM_Test_Verify_Event(0,
                         FM_SET_TABLE_STATE_ARG_IDX_ERR_EID,
                         CFE_EVS_EventType_ERROR,
                         "%s error: invalid command argument: index = %d");
}

void Test_FM_SetTableStateCmd_BadNewState(void)
{
    CFE_Status_t          Status;
    FM_SetTableStateCmd_t SetTableStateCmd;
    FM_MonitorTable_t     Table;
    void                 *TablePtr;

    memset(&Table, 0, sizeof(Table));
    memset(&SetTableStateCmd, 0, sizeof(SetTableStateCmd));

    SetTableStateCmd.Payload.State = 55;
    SetTableStateCmd.Payload.Index = 0;

    TablePtr = &Table;
    UT_SetDataBuffer(UT_KEY(CFE_TBL_GetAddress), &TablePtr, sizeof(TablePtr), false);

    Status = FM_SetTableStateCmd(&SetTableStateCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    FM_Test_Verify_Event(0,
                         FM_SET_TABLE_STATE_ARG_STATE_ERR_EID,
                         CFE_EVS_EventType_ERROR,
                         "%s error: invalid command argument: state = %d");
}

void Test_FM_SetTableStateCmd_BadCurrentState(void)
{
    CFE_Status_t          Status;
    FM_SetTableStateCmd_t SetTableStateCmd;
    FM_MonitorTable_t     Table;
    void                 *TablePtr;

    SetTableStateCmd.Payload.State = FM_TABLE_ENTRY_DISABLED;
    SetTableStateCmd.Payload.Index = 0;

    memset(&Table, 0, sizeof(Table));
    memset(&SetTableStateCmd, 0, sizeof(SetTableStateCmd));

    TablePtr = &Table;
    UT_SetDataBuffer(UT_KEY(CFE_TBL_GetAddress), &TablePtr, sizeof(TablePtr), false);

    Status = FM_SetTableStateCmd(&SetTableStateCmd);

    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    FM_Test_Verify_Event(0,
                         FM_SET_TABLE_STATE_UNUSED_ERR_EID,
                         CFE_EVS_EventType_ERROR,
                         "%s error: cannot modify unused table entry: index = %d");
}

/****************************/
/* Set Permissions Tests    */
/****************************/

void Test_FM_SetPermissionsCmd_Success(void)
{
    CFE_Status_t           Status;
    FM_SetPermissionsCmd_t SetPermissionsCmd;

    memset(&SetPermissionsCmd, 0, sizeof(SetPermissionsCmd));

    strncpy(SetPermissionsCmd.Payload.Path, "file", sizeof(SetPermissionsCmd.Payload.Path) - 1);

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyNameValid), true);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_SetPermissionsCmd(&SetPermissionsCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, FM_SET_PERMISSIONS_CC);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_SetPermissionsCmd_BadName(void)
{
    CFE_Status_t           Status;
    FM_SetPermissionsCmd_t SetPermissionsCmd;

    memset(&SetPermissionsCmd, 0, sizeof(SetPermissionsCmd));

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyNameValid), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_SetPermissionsCmd(&SetPermissionsCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

void Test_FM_SetPermissionsCmd_NoChildTask(void)
{
    CFE_Status_t           Status;
    FM_SetPermissionsCmd_t SetPermissionsCmd;

    memset(&SetPermissionsCmd, 0, sizeof(SetPermissionsCmd));

    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyNameValid), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_VerifyChildTask), true);

    Status = FM_SetPermissionsCmd(&SetPermissionsCmd);

    /* Assert */
    UtAssert_EQ(CFE_Status_t, Status, CFE_SUCCESS);

    UtAssert_INT32_EQ(FM_AppData.ChildQueue[0].CommandCode, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* ********************************
 * Report HK Tests
 * *******************************/
void Test_FM_SendHkCmd(void)
{
    /* Arrange */
    UT_SetDefaultReturnValue(UT_KEY(FM_GetOpenFilesData), 0);

    /* Set non-zero values to assert */
    FM_AppData.HkTlm.Payload.CommandCounter             = 1;
    FM_AppData.HkTlm.Payload.CommandErrorCounter        = 2;
    FM_AppData.HkTlm.Payload.ChildCommandCounter        = 3;
    FM_AppData.HkTlm.Payload.ChildCommandErrorCounter   = 4;
    FM_AppData.HkTlm.Payload.ChildCommandWarningCounter = 5;
    FM_AppData.HkTlm.Payload.ChildQueueCount            = 6;
    FM_AppData.HkTlm.Payload.ChildCurrentCC             = 7;
    FM_AppData.HkTlm.Payload.ChildPreviousCC            = 8;

    /* Act */
    UtAssert_VOIDCALL(FM_SendHkCmd(NULL));

    /* Assert */
    UtAssert_STUB_COUNT(CFE_TBL_Manage, 1);
    UtAssert_STUB_COUNT(FM_GetOpenFilesData, 1);
    UtAssert_STUB_COUNT(CFE_SB_TimeStampMsg, 1);
    UtAssert_STUB_COUNT(CFE_SB_TransmitMsg, 1);
}

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(Test_FM_SendHkCmd);
    ADD_TEST(Test_FM_NoopCmd_Success);
    ADD_TEST(Test_FM_ResetCountersCmd_Success);
    ADD_TEST(Test_FM_CopyFileCmd_Success);
    ADD_TEST(Test_FM_CopyFileCmd_BadOverwrite);
    ADD_TEST(Test_FM_CopyFileCmd_SourceNotExist);
    ADD_TEST(Test_FM_CopyFileCmd_NoOverwriteTargetExists);
    ADD_TEST(Test_FM_CopyFileCmd_OverwriteFileOpen);
    ADD_TEST(Test_FM_CopyFileCmd_NoChildTask);
    ADD_TEST(Test_FM_MoveFileCmd_Success);
    ADD_TEST(Test_FM_MoveFileCmd_BadOverwrite);
    ADD_TEST(Test_FM_MoveFileCmd_SourceNotExist);
    ADD_TEST(Test_FM_MoveFileCmd_NoOverwriteTargetExists);
    ADD_TEST(Test_FM_MoveFileCmd_OverwriteFileOpen);
    ADD_TEST(Test_FM_MoveFileCmd_NoChildTask);
    ADD_TEST(Test_FM_RenameFileCmd_Success);
    ADD_TEST(Test_FM_RenameFileCmd_SourceNotExist);
    ADD_TEST(Test_FM_RenameFileCmd_TargetExists);
    ADD_TEST(Test_FM_RenameFileCmd_NoChildTask);
    ADD_TEST(Test_FM_DeleteFileCmd_Success);
    ADD_TEST(Test_FM_DeleteFileCmd_FileNotClosed);
    ADD_TEST(Test_FM_DeleteFileCmd_NoChildTask);
    ADD_TEST(Test_FM_DeleteAllFilesCmd_Success);
    ADD_TEST(Test_FM_DeleteAllFilesCmd_DirNoExist);
    ADD_TEST(Test_FM_DeleteAllFilesCmd_NoChildTask);
    ADD_TEST(Test_FM_DecompressFileCmd_Success);
    ADD_TEST(Test_FM_DecompressFileCmd_SourceFileOpen);
    ADD_TEST(Test_FM_DecompressFileCmd_TargetFileExists);
    ADD_TEST(Test_FM_DecompressFileCmd_NoChildTask);
    ADD_TEST(Test_FM_ConcatFilesCmd_Success);
    ADD_TEST(Test_FM_ConcatFilesCmd_SourceFile1NotClosed);
    ADD_TEST(Test_FM_ConcatFilesCmd_SourceFile2NotClosed);
    ADD_TEST(Test_FM_ConcatFilesCmd_TargetFileExists);
    ADD_TEST(Test_FM_ConcatFilesCmd_NoChildTask);
    ADD_TEST(Test_FM_GetFileInfoCmd_Success);
    ADD_TEST(Test_FM_GetFileInfoCmd_InvalidName);
    ADD_TEST(Test_FM_GetFileInfoCmd_NoChildTask);
    ADD_TEST(Test_FM_GetOpenFilesCmd_Success);
    ADD_TEST(Test_FM_CreateDirectoryCmd_Success);
    ADD_TEST(Test_FM_CreateDirectoryCmd_DirExists);
    ADD_TEST(Test_FM_CreateDirectoryCmd_NoChildTask);
    ADD_TEST(Test_FM_DeleteDirectoryCmd_Success);
    ADD_TEST(Test_FM_DeleteDirectoryCmd_DirNoExist);
    ADD_TEST(Test_FM_DeleteDirectoryCmd_NoChildTask);
    ADD_TEST(Test_FM_GetDirListFileCmd_Success);
    ADD_TEST(Test_FM_GetDirListFileCmd_SuccessDefaultPath);
    ADD_TEST(Test_FM_GetDirListFileCmd_SourceNotExist);
    ADD_TEST(Test_FM_GetDirListFileCmd_TargetFileOpen);
    ADD_TEST(Test_FM_GetDirListFileCmd_NoChildTask);
    ADD_TEST(Test_FM_GetDirListPktCmd_Success);
    ADD_TEST(Test_FM_GetDirListPktCmd_SourceNotExist);
    ADD_TEST(Test_FM_GetDirListPktCmd_NoChildTask);
    ADD_TEST(Test_FM_MonitorFilesystemSpaceCmd_Success);
    ADD_TEST(Test_FM_MonitorFilesystemSpaceCmd_NullFreeSpaceTable);
    ADD_TEST(Test_FM_MonitorFilesystemSpaceCmd_ImplCallFails);
    ADD_TEST(Test_FM_MonitorFilesystemSpaceCmd_NotImpl);
    ADD_TEST(Test_FM_SetTableStateCmd_Success);
    ADD_TEST(Test_FM_SetTableStateCmd_NullFreeSpaceTable);
    ADD_TEST(Test_FM_SetTableStateCmd_IndexTooLarge);
    ADD_TEST(Test_FM_SetTableStateCmd_BadNewState);
    ADD_TEST(Test_FM_SetTableStateCmd_BadCurrentState);
    ADD_TEST(Test_FM_SetPermissionsCmd_Success);
    ADD_TEST(Test_FM_SetPermissionsCmd_BadName);
    ADD_TEST(Test_FM_SetPermissionsCmd_NoChildTask);
}
