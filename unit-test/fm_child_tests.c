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

/*
 * UT Testing
 */
#include "fm_test_utils.h"

/*
 * UT includes
 */
#include "uttest.h"
#include "utassert.h"
#include "utstubs.h"

#include <unistd.h>
#include <stdlib.h>
#include "cfe.h"
#include "cfe_msgids.h"

/* Unit test helpers */

void UT_FM_Child_Cmd_Assert(int32 cmd_ctr, int32 cmderr_ctr, int32 cmdwarn_ctr, int32 previous_cc)
{
    UtAssert_INT32_EQ(FM_GlobalData.ChildCmdCounter, cmd_ctr);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCmdErrCounter, cmderr_ctr);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCmdWarnCounter, cmdwarn_ctr);

    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, previous_cc);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
}

/*********************************************************************************
 *          TEST CASE FUNCTIONS
 *********************************************************************************/

/* ****************
 * Init Tests
 * ***************/
void Test_FM_ChildInit_CountSemCreateNotSuccess(void)
{
    /* Arrange */
    UT_SetDefaultReturnValue(UT_KEY(OS_CountSemCreate), !CFE_SUCCESS);

    /* Act */
    UtAssert_INT32_EQ(FM_ChildInit(), !CFE_SUCCESS);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CHILD_INIT_SEM_ERR_EID);
}

void Test_FM_ChildInit_MutSemCreateNotSuccess(void)
{
    /* Arrange */
    UT_SetDefaultReturnValue(UT_KEY(OS_MutSemCreate), !CFE_SUCCESS);

    /* Act */
    UtAssert_INT32_EQ(FM_ChildInit(), !CFE_SUCCESS);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CHILD_INIT_QSEM_ERR_EID);
}

void Test_FM_ChildInit_MuteSemCreateSuccess_CreateChildTaskNotSuccess(void)
{
    /* Arrange */
    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_CreateChildTask), !CFE_SUCCESS);

    /* Act */
    UtAssert_INT32_EQ(FM_ChildInit(), !CFE_SUCCESS);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CHILD_INIT_CREATE_ERR_EID);
}

void Test_FM_ChildInit_ReturnSuccess(void)
{
    UtAssert_INT32_EQ(FM_ChildInit(), CFE_SUCCESS);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
}

/* ****************
 * ChildTask Tests
 * ***************/
void Test_FM_ChildTask_ChildLoopCalled(void)
{
    /* Arrange */
    FM_GlobalData.ChildSemaphore = FM_UT_OBJID_1;

    UT_SetDefaultReturnValue(UT_KEY(OS_CountSemTake), !CFE_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildTask());

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_STUB_COUNT(CFE_ES_ExitChildTask, 1);

    /* Assert */
    /* SendEvent called once in ChildTask and should be called once in ChildLoop */
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CHILD_INIT_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_CHILD_TERM_SEM_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_ERROR);
}

/* ****************
 * ChildProcess Tests
 * ***************/
void Test_FM_ChildProcess_ChildReadIndexGreaterChildQDepth(void)
{
    /* Arrange */
    FM_GlobalData.ChildReadIndex                                       = FM_CHILD_QUEUE_DEPTH - 1;
    FM_GlobalData.ChildQueue[FM_GlobalData.ChildReadIndex].CommandCode = -1;

    /* Act */
    UtAssert_VOIDCALL(FM_ChildProcess());

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, 0);
    UtAssert_INT32_EQ(FM_GlobalData.ChildReadIndex, 0);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CHILD_EXE_ERR_EID);
}

void Test_FM_ChildProcess_FMCopyCC(void)
{
    /* Arrange */
    FM_GlobalData.ChildQueue[0].CommandCode = FM_COPY_FILE_CC;

    /* Act */
    UtAssert_VOIDCALL(FM_ChildProcess());

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 0, FM_GlobalData.ChildQueue[0].CommandCode);

    UtAssert_STUB_COUNT(OS_cp, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_COPY_CMD_INF_EID);
}

void Test_FM_ChildProcess_FMMoveCC(void)
{
    /* Arrange */
    FM_GlobalData.ChildQueue[0].CommandCode = FM_MOVE_FILE_CC;

    /* Act */
    UtAssert_VOIDCALL(FM_ChildProcess());

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 0, FM_GlobalData.ChildQueue[0].CommandCode);

    UtAssert_STUB_COUNT(OS_mv, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_MOVE_CMD_INF_EID);
}

void Test_FM_ChildProcess_FMRenameCC(void)
{
    /* Arrange */
    FM_GlobalData.ChildQueue[0].CommandCode = FM_RENAME_FILE_CC;

    /* Act */
    UtAssert_VOIDCALL(FM_ChildProcess());

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 0, FM_GlobalData.ChildQueue[0].CommandCode);

    UtAssert_STUB_COUNT(OS_rename, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_RENAME_CMD_INF_EID);
}

void Test_FM_ChildProcess_FMDeleteCC(void)
{
    /* Arrange */
    FM_GlobalData.ChildQueue[0].CommandCode = FM_DELETE_FILE_CC;

    /* Act */
    UtAssert_VOIDCALL(FM_ChildProcess());

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 0, FM_GlobalData.ChildQueue[0].CommandCode);

    UtAssert_STUB_COUNT(OS_remove, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_CMD_INF_EID);
}

void Test_FM_ChildProcess_FMDeleteAllCC(void)
{
    /* Arrange */
    FM_GlobalData.ChildQueue[0].CommandCode = FM_DELETE_ALL_FILES_CC;

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildProcess());

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, FM_GlobalData.ChildQueue[0].CommandCode);

    UtAssert_STUB_COUNT(OS_DirectoryOpen, 1);
    UtAssert_STUB_COUNT(OS_DirectoryClose, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_ALL_OS_ERR_EID);
}

void Test_FM_ChildProcess_FMDecompressCC(void)
{
    /* Arrange */
    FM_GlobalData.ChildQueue[0].CommandCode = FM_DECOMPRESS_FILE_CC;

    UT_SetDefaultReturnValue(UT_KEY(FM_Decompress_Impl), !CFE_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildProcess());

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, FM_GlobalData.ChildQueue[0].CommandCode);

    UtAssert_STUB_COUNT(FM_Decompress_Impl, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DECOM_CFE_ERR_EID);
}

void Test_FM_ChildProcess_FMConcatCC(void)
{
    /* Arrange */
    FM_GlobalData.ChildQueue[0].CommandCode = FM_CONCAT_FILES_CC;
    FM_GlobalData.ChildCurrentCC            = 1;

    UT_SetDefaultReturnValue(UT_KEY(OS_cp), !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildProcess());

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, FM_GlobalData.ChildQueue[0].CommandCode);

    UtAssert_STUB_COUNT(OS_cp, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CONCAT_OSCPY_ERR_EID);
}

void Test_FM_ChildProcess_FMCreateDirCC(void)
{
    /* Arrange */
    FM_GlobalData.ChildQueue[0].CommandCode = FM_CREATE_DIRECTORY_CC;
    FM_GlobalData.ChildCurrentCC            = 1;

    UT_SetDefaultReturnValue(UT_KEY(OS_mkdir), !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildProcess());

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, FM_GlobalData.ChildQueue[0].CommandCode);

    UtAssert_STUB_COUNT(OS_mkdir, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CREATE_DIR_OS_ERR_EID);
}

void Test_FM_ChildProcess_FMDeleteDirCC(void)
{
    /* Arrange */
    FM_GlobalData.ChildQueue[0].CommandCode = FM_DELETE_DIRECTORY_CC;
    FM_GlobalData.ChildCurrentCC            = 1;

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildProcess());

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, FM_GlobalData.ChildQueue[0].CommandCode);

    UtAssert_STUB_COUNT(OS_DirectoryOpen, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_OPENDIR_OS_ERR_EID);
}

void Test_FM_ChildProcess_FMGetFileInfoCC(void)
{
    /* Arrange */
    FM_GlobalData.ChildQueue[0].CommandCode   = FM_GET_FILE_INFO_CC;
    FM_GlobalData.ChildQueue[0].FileInfoCRC   = !FM_IGNORE_CRC;
    FM_GlobalData.ChildQueue[0].FileInfoState = FM_NAME_IS_FILE_OPEN;
    FM_GlobalData.ChildCurrentCC              = 1;

    UT_SetDefaultReturnValue(UT_KEY(CFE_MSG_Init), CFE_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildProcess());

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 1, FM_GlobalData.ChildQueue[0].CommandCode);

    UtAssert_STUB_COUNT(CFE_MSG_Init, 1);
    UtAssert_STUB_COUNT(CFE_SB_TransmitMsg, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_FILE_INFO_STATE_WARNING_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_GET_FILE_INFO_CMD_INF_EID);
}

void Test_FM_ChildProcess_FMGetDirListsFileCC(void)
{
    /* Arrange */
    FM_GlobalData.ChildQueue[0].CommandCode = FM_GET_DIR_LIST_FILE_CC;
    FM_GlobalData.ChildCurrentCC            = 1;

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildProcess());

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, FM_GlobalData.ChildQueue[0].CommandCode);

    UtAssert_STUB_COUNT(OS_DirectoryOpen, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_FILE_OSOPENDIR_ERR_EID);
}

void Test_FM_ChildProcess_FMGetDirListsPktCC(void)
{
    /* Arrange */
    FM_GlobalData.ChildQueue[0].CommandCode = FM_GET_DIR_LIST_PKT_CC;
    FM_GlobalData.ChildCurrentCC            = 1;

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildProcess());

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 0, FM_GlobalData.ChildQueue[0].CommandCode);

    UtAssert_STUB_COUNT(OS_DirectoryOpen, 1);
    UtAssert_STUB_COUNT(OS_DirectoryRead, 1);
    UtAssert_STUB_COUNT(CFE_MSG_Init, 1);
    UtAssert_STUB_COUNT(OS_DirectoryClose, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_PKT_CMD_INF_EID);
}

void Test_FM_ChildProcess_FMSetFilePermCC(void)
{
    /* Arrange */
    FM_GlobalData.ChildQueue[0].CommandCode = FM_SET_PERMISSIONS_CC;
    FM_GlobalData.ChildCurrentCC            = 1;

    UT_SetDefaultReturnValue(UT_KEY(OS_chmod), !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildProcess());

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, FM_GlobalData.ChildQueue[0].CommandCode);

    UtAssert_STUB_COUNT(OS_chmod, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_SET_PERM_OS_ERR_EID);
}

void Test_FM_ChildProcess_DefaultSwitch(void)
{
    /* Arrange */
    FM_GlobalData.ChildQueue[0].CommandCode = -1;

    /* Act */
    UtAssert_VOIDCALL(FM_ChildProcess());

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, 0);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CHILD_EXE_ERR_EID);
}

/* ****************
 * ChildCopyCmd Tests
 * ***************/
void Test_FM_ChildCopyCmd_OScpIsSuccess(void)
{
    FM_ChildQueueEntry_t queue_entry = {.CommandCode = FM_COPY_FILE_CC};

    /* Act */
    UtAssert_VOIDCALL(FM_ChildCopyCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_COPY_CMD_INF_EID);
}

void Test_FM_ChildCopyCmd_OScpNotSuccess(void)
{
    FM_ChildQueueEntry_t queue_entry = {.CommandCode = FM_COPY_FILE_CC};

    /* Arrange */
    UT_SetDefaultReturnValue(UT_KEY(OS_cp), !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildCopyCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_COPY_OS_ERR_EID);
}

/* ****************
 * ChildMoveCmd Tests
 * ***************/
void Test_FM_ChildMoveCmd_OSmvNotSuccess(void)
{
    FM_ChildQueueEntry_t queue_entry = {.CommandCode = FM_MOVE_FILE_CC};

    /* Arrange */
    UT_SetDefaultReturnValue(UT_KEY(OS_mv), !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildMoveCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_mv, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_MOVE_OS_ERR_EID);
}

void Test_FM_ChildMoveCmd_OSmvSuccess(void)
{
    FM_ChildQueueEntry_t queue_entry = {.CommandCode = FM_MOVE_FILE_CC};

    /* Act */
    UtAssert_VOIDCALL(FM_ChildMoveCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_mv, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_MOVE_CMD_INF_EID);
}

/* ****************
 * ChildRenameCmd Tests
 * ***************/
void Test_FM_ChildRenameCmd_OSRenameSuccess(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode = FM_RENAME_FILE_CC};

    /* Act */
    UtAssert_VOIDCALL(FM_ChildRenameCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_rename, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_RENAME_CMD_INF_EID);
}

void Test_FM_ChildRenameCmd_OSRenameNotSuccess(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode = FM_RENAME_FILE_CC};

    UT_SetDefaultReturnValue(UT_KEY(OS_rename), !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildRenameCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_rename, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_RENAME_OS_ERR_EID);
}

/* ****************
 * ChildDeleteCmd Tests
 * ***************/
void Test_FM_ChildDeleteCmd_OSRemoveSuccess(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode = FM_DELETE_FILE_CC};

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDeleteCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_remove, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_CMD_INF_EID);
}

void Test_FM_ChildDeleteCmd_OSRemoveNotSuccess(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode = FM_DELETE_FILE_CC};

    UT_SetDefaultReturnValue(UT_KEY(OS_remove), !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDeleteCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_remove, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_OS_ERR_EID);
}

/* ****************
 * ChildDeleteAllFilesCmd Tests
 * ***************/
void Test_FM_ChildDeleteAllFilesCmd_DirOpenNotSuccess(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {
        .CommandCode = FM_DELETE_ALL_FILES_CC, .Source1 = "source1", .Source2 = "source2"};

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDeleteAllFilesCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_DirectoryOpen, 1);
    UtAssert_STUB_COUNT(OS_DirectoryClose, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_ALL_OS_ERR_EID);
}

void Test_FM_ChildDeleteAllFilesCmd_DirReadNotSuccess(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {
        .CommandCode = FM_DELETE_ALL_FILES_CC, .Source1 = "source1", .Source2 = "source2"};

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDeleteAllFilesCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_DirectoryOpen, 1);
    UtAssert_STUB_COUNT(OS_DirectoryClose, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_ALL_CMD_INF_EID);
}

void Test_FM_ChildDeleteAllFilesCmd_DirEntryThisDirectory(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {
        .CommandCode = FM_DELETE_ALL_FILES_CC, .Source1 = "source1", .Source2 = "source2"};
    os_dirent_t direntry = {.FileName = FM_THIS_DIRECTORY};

    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &direntry, sizeof(direntry), false);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDeleteAllFilesCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_DirectoryOpen, 1);
    UtAssert_STUB_COUNT(OS_DirectoryClose, 1);
    UtAssert_STUB_COUNT(OS_DirectoryRead, 2);
    UtAssert_STUB_COUNT(FM_GetFilenameState, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_ALL_CMD_INF_EID);
}

void Test_FM_ChildDeleteAllFilesCmd_DirEntryParentDirectory(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {
        .CommandCode = FM_DELETE_ALL_FILES_CC, .Source1 = "source1", .Source2 = "source2"};
    os_dirent_t direntry = {.FileName = FM_PARENT_DIRECTORY};

    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &direntry, sizeof(direntry), false);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDeleteAllFilesCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_DirectoryOpen, 1);
    UtAssert_STUB_COUNT(OS_DirectoryClose, 1);
    UtAssert_STUB_COUNT(OS_DirectoryRead, 2);
    UtAssert_STUB_COUNT(FM_GetFilenameState, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_ALL_CMD_INF_EID);
}

void Test_FM_ChildDeleteAllFilesCmd_PathFilenameLengthGreaterMaxPthLen(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode = FM_DELETE_ALL_FILES_CC,
                                        .Source1     = "source1",
                                        .Source2     = "source2HasAReallyReallyLongNameSomeSayTheNameIs42Characters"};
    os_dirent_t          direntry    = {.FileName = "ThisDirectory"};

    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &direntry, sizeof(direntry), false);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDeleteAllFilesCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 1, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_DirectoryOpen, 1);
    UtAssert_STUB_COUNT(OS_DirectoryClose, 1);
    UtAssert_STUB_COUNT(OS_DirectoryRead, 2);
    UtAssert_STUB_COUNT(FM_GetFilenameState, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_ALL_CMD_INF_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_DELETE_ALL_FILES_ND_WARNING_EID);
}

void Test_FM_ChildDeleteAllFilesCmd_InvalidFilenameState(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {
        .CommandCode = FM_DELETE_ALL_FILES_CC, .Source1 = "source1", .Source2 = "source2"};
    os_dirent_t direntry = {.FileName = "ThisDirectory"};

    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &direntry, sizeof(direntry), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_GetFilenameState), FM_NAME_IS_INVALID);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDeleteAllFilesCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 1, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_DirectoryOpen, 1);
    UtAssert_STUB_COUNT(OS_DirectoryClose, 1);
    UtAssert_STUB_COUNT(OS_DirectoryRead, 2);
    UtAssert_STUB_COUNT(FM_GetFilenameState, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_ALL_CMD_INF_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_DELETE_ALL_FILES_ND_WARNING_EID);
}

void Test_FM_ChildDeleteAllFilesCmd_NotInUseFilenameState(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {
        .CommandCode = FM_DELETE_ALL_FILES_CC, .Source1 = "source1", .Source2 = "source2"};
    os_dirent_t direntry = {.FileName = "ThisDirectory"};

    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &direntry, sizeof(direntry), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_GetFilenameState), FM_NAME_IS_NOT_IN_USE);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDeleteAllFilesCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 1, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_DirectoryOpen, 1);
    UtAssert_STUB_COUNT(OS_DirectoryClose, 1);
    UtAssert_STUB_COUNT(OS_DirectoryRead, 2);
    UtAssert_STUB_COUNT(FM_GetFilenameState, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_ALL_CMD_INF_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_DELETE_ALL_FILES_ND_WARNING_EID);
}

void Test_FM_ChildDeleteAllFilesCmd_DirectoryFilenameState(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {
        .CommandCode = FM_DELETE_ALL_FILES_CC, .Source1 = "source1", .Source2 = "source2"};
    os_dirent_t direntry = {.FileName = "ThisDirectory"};

    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &direntry, sizeof(direntry), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_GetFilenameState), FM_NAME_IS_DIRECTORY);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDeleteAllFilesCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 1, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_DirectoryOpen, 1);
    UtAssert_STUB_COUNT(OS_DirectoryClose, 1);
    UtAssert_STUB_COUNT(OS_DirectoryRead, 2);
    UtAssert_STUB_COUNT(FM_GetFilenameState, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_ALL_CMD_INF_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_DELETE_ALL_SKIP_WARNING_EID);
}

void Test_FM_ChildDeleteAllFilesCmd_OpenFilenameState(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {
        .CommandCode = FM_DELETE_ALL_FILES_CC, .Source1 = "source1", .Source2 = "source2"};
    os_dirent_t direntry = {.FileName = "ThisDirectory"};

    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &direntry, sizeof(direntry), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_GetFilenameState), FM_NAME_IS_FILE_OPEN);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDeleteAllFilesCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 1, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_DirectoryOpen, 1);
    UtAssert_STUB_COUNT(OS_DirectoryClose, 1);
    UtAssert_STUB_COUNT(OS_DirectoryRead, 2);
    UtAssert_STUB_COUNT(FM_GetFilenameState, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_ALL_CMD_INF_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_DELETE_ALL_FILES_ND_WARNING_EID);
}

void Test_FM_ChildDeleteAllFilesCmd_ClosedFilename_OSRmNotSuccess(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {
        .CommandCode = FM_DELETE_ALL_FILES_CC, .Source1 = "source1", .Source2 = "source2"};
    os_dirent_t direntry = {.FileName = "ThisDirectory"};

    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &direntry, sizeof(direntry), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_GetFilenameState), FM_NAME_IS_FILE_CLOSED);
    UT_SetDefaultReturnValue(UT_KEY(OS_remove), !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDeleteAllFilesCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 1, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_DirectoryOpen, 1);
    UtAssert_STUB_COUNT(OS_DirectoryClose, 1);
    UtAssert_STUB_COUNT(OS_DirectoryRead, 2);
    UtAssert_STUB_COUNT(FM_GetFilenameState, 1);
    UtAssert_STUB_COUNT(OS_remove, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_ALL_CMD_INF_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_DELETE_ALL_FILES_ND_WARNING_EID);
}

void Test_FM_ChildDeleteAllFilesCmd_ClosedFilename_OSrmSuccess(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {
        .CommandCode = FM_DELETE_ALL_FILES_CC, .Source1 = "source1", .Source2 = "source2"};
    os_dirent_t direntry = {.FileName = "ThisDirectory"};

    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &direntry, sizeof(direntry), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_GetFilenameState), FM_NAME_IS_FILE_CLOSED);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDeleteAllFilesCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_DirectoryOpen, 1);
    UtAssert_STUB_COUNT(OS_DirectoryClose, 1);
    UtAssert_STUB_COUNT(OS_DirectoryRead, 2);
    UtAssert_STUB_COUNT(FM_GetFilenameState, 1);
    UtAssert_STUB_COUNT(OS_remove, 1);
    UtAssert_STUB_COUNT(OS_DirectoryRewind, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_ALL_CMD_INF_EID);
}

void Test_FM_ChildDeleteAllFilesCmd_FilenameStateDefaultReturn(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {
        .CommandCode = FM_DELETE_ALL_FILES_CC, .Source1 = "source1", .Source2 = "source2"};
    os_dirent_t direntry = {.FileName = "ThisDirectory"};

    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &direntry, sizeof(direntry), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_GetFilenameState), -1); /* default case */

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDeleteAllFilesCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 1, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_DirectoryOpen, 1);
    UtAssert_STUB_COUNT(OS_DirectoryClose, 1);
    UtAssert_STUB_COUNT(OS_DirectoryRead, 2);
    UtAssert_STUB_COUNT(FM_GetFilenameState, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_ALL_CMD_INF_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_DELETE_ALL_FILES_ND_WARNING_EID);
}

/* ****************
 * ChildDecompressFileCmd Tests
 * ***************/

void Test_FM_ChildDecompressFileCmd_FSDecompressSuccess(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode = FM_DECOMPRESS_FILE_CC};

    FM_GlobalData.ChildCurrentCC = 1;

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDecompressFileCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(FM_Decompress_Impl, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DECOM_CMD_INF_EID);
}

void Test_FM_ChildDecompressFileCmd_FSDecompressNotSuccess(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode = FM_DECOMPRESS_FILE_CC};

    FM_GlobalData.ChildCurrentCC = 1;
    UT_SetDefaultReturnValue(UT_KEY(FM_Decompress_Impl), !CFE_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDecompressFileCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(FM_Decompress_Impl, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DECOM_CFE_ERR_EID);
}

/* ****************
 * ChildConcatFilesCmd Tests
 * ***************/
void Test_FM_ChildConcatFilesCmd_OSCpNotSuccess(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode = FM_CONCAT_FILES_CC, .Source1 = "source1", .Source2 = "source2"};

    FM_GlobalData.ChildCurrentCC = 1;
    UT_SetDefaultReturnValue(UT_KEY(OS_cp), !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildConcatFilesCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_cp, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CONCAT_OSCPY_ERR_EID);
}

void Test_FM_ChildConcatFilesCmd_OSOpenCreateSourceNotSuccess(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode = FM_CONCAT_FILES_CC, .Source1 = "source1", .Source2 = "source2"};

    UT_SetDefaultReturnValue(UT_KEY(OS_OpenCreate), !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildConcatFilesCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_cp, 1);
    UtAssert_STUB_COUNT(OS_OpenCreate, 1);
    UtAssert_STUB_COUNT(OS_remove, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CONCAT_OPEN_SRC2_ERR_EID);
}

void Test_FM_ChildConcatFilesCmd_OSOpenCreateTargetNotSuccess(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode = FM_CONCAT_FILES_CC, .Source1 = "source1", .Source2 = "source2"};

    UT_SetDeferredRetcode(UT_KEY(OS_OpenCreate), 2, !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildConcatFilesCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_cp, 1);
    UtAssert_STUB_COUNT(OS_OpenCreate, 2);
    UtAssert_STUB_COUNT(OS_remove, 1);
    UtAssert_STUB_COUNT(OS_close, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CONCAT_OPEN_TGT_ERR_EID);
}

void Test_FM_ChildConcatFilesCmd_OSReadBytesZero(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode = FM_CONCAT_FILES_CC, .Source1 = "source1", .Source2 = "source2"};

    UT_SetDefaultReturnValue(UT_KEY(OS_read), 0);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildConcatFilesCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_read, 1);
    UtAssert_STUB_COUNT(OS_cp, 1);
    UtAssert_STUB_COUNT(OS_OpenCreate, 2);
    UtAssert_STUB_COUNT(OS_remove, 0);
    UtAssert_STUB_COUNT(OS_close, 2);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CONCAT_CMD_INF_EID);
}

void Test_FM_ChildConcatFilesCmd_OSReadBytesLessThanZero(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode = FM_CONCAT_FILES_CC, .Source1 = "source1", .Source2 = "source2"};

    UT_SetDefaultReturnValue(UT_KEY(OS_read), -1);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildConcatFilesCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_read, 1);
    UtAssert_STUB_COUNT(OS_cp, 1);
    UtAssert_STUB_COUNT(OS_OpenCreate, 2);
    UtAssert_STUB_COUNT(OS_remove, 1);
    UtAssert_STUB_COUNT(OS_close, 2);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CONCAT_OSRD_ERR_EID);
}

void Test_FM_ChildConcatFilesCmd_BytesWrittenNotEqualBytesRead(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode = FM_CONCAT_FILES_CC, .Source1 = "source1", .Source2 = "source2"};

    UT_SetDefaultReturnValue(UT_KEY(OS_read), 1);
    UT_SetDefaultReturnValue(UT_KEY(OS_write), 0);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildConcatFilesCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_read, 1);
    UtAssert_STUB_COUNT(OS_cp, 1);
    UtAssert_STUB_COUNT(OS_OpenCreate, 2);
    UtAssert_STUB_COUNT(OS_remove, 1);
    UtAssert_STUB_COUNT(OS_close, 2);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CONCAT_OSWR_ERR_EID);
}

void Test_FM_ChildConcatFilesCmd_CopyInProgressTrueLoopCountEqualChildFileLoopCount(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode = FM_CONCAT_FILES_CC, .Source1 = "source1", .Source2 = "source2"};

    UT_SetDefaultReturnValue(UT_KEY(OS_read), 1);
    UT_SetDefaultReturnValue(UT_KEY(OS_write), 1);
    UT_SetDeferredRetcode(UT_KEY(OS_read), FM_CHILD_FILE_LOOP_COUNT + 1, -1);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildConcatFilesCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_read, FM_CHILD_FILE_LOOP_COUNT + 1);
    UtAssert_STUB_COUNT(OS_TaskDelay, 1);
    UtAssert_STUB_COUNT(OS_cp, 1);
    UtAssert_STUB_COUNT(OS_OpenCreate, 2);
    UtAssert_STUB_COUNT(OS_remove, 1);
    UtAssert_STUB_COUNT(OS_close, 2);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CONCAT_OSRD_ERR_EID);
}

/* ****************
 * ChildFileInfoCmd Tests
 * ***************/
void Test_FM_ChildFileInfoCmd_FileInfoCRCEqualIgnoreCRC(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode   = FM_GET_FILE_INFO_CC,
                                        .Source1       = "source1",
                                        .Source2       = "source2",
                                        .FileInfoCRC   = FM_IGNORE_CRC,
                                        .FileInfoState = FM_NAME_IS_FILE_CLOSED};

    /* Act */
    UtAssert_VOIDCALL(FM_ChildFileInfoCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_OpenCreate, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_FILE_INFO_CMD_INF_EID);
}

void Test_FM_ChildFileInfoCmd_FileInfoStateIsNotFileClosed(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode   = FM_GET_FILE_INFO_CC,
                                        .Source1       = "source1",
                                        .Source2       = "source2",
                                        .FileInfoCRC   = CFE_ES_CrcType_CRC_8,
                                        .FileInfoState = FM_NAME_IS_FILE_OPEN};

    /* Act */
    UtAssert_VOIDCALL(FM_ChildFileInfoCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 1, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_OpenCreate, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_FILE_INFO_STATE_WARNING_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_GET_FILE_INFO_CMD_INF_EID);
}

void Test_FM_ChildFileInfoCmd_FileInfoCRCEqualMission8(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode   = FM_GET_FILE_INFO_CC,
                                        .Source1       = "source1",
                                        .Source2       = "source2",
                                        .FileInfoCRC   = CFE_ES_CrcType_CRC_8,
                                        .FileInfoState = FM_NAME_IS_FILE_CLOSED};

    UT_SetDefaultReturnValue(UT_KEY(OS_OpenCreate), !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildFileInfoCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 1, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_OpenCreate, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_FILE_INFO_OPEN_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_GET_FILE_INFO_CMD_INF_EID);
}

void Test_FM_ChildFileInfoCmd_FileInfoCRCEqualMission16(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode   = FM_GET_FILE_INFO_CC,
                                        .Source1       = "source1",
                                        .Source2       = "source2",
                                        .FileInfoCRC   = CFE_ES_CrcType_CRC_16,
                                        .FileInfoState = FM_NAME_IS_FILE_CLOSED};

    UT_SetDefaultReturnValue(UT_KEY(OS_OpenCreate), !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildFileInfoCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 1, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_OpenCreate, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_FILE_INFO_OPEN_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_GET_FILE_INFO_CMD_INF_EID);
}

void Test_FM_ChildFileInfoCmd_FileInfoCRCEqualMission32(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode   = FM_GET_FILE_INFO_CC,
                                        .Source1       = "source1",
                                        .Source2       = "source2",
                                        .FileInfoCRC   = CFE_ES_CrcType_CRC_32,
                                        .FileInfoState = FM_NAME_IS_FILE_CLOSED};

    UT_SetDefaultReturnValue(UT_KEY(OS_OpenCreate), !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildFileInfoCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 1, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_OpenCreate, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_FILE_INFO_OPEN_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_GET_FILE_INFO_CMD_INF_EID);
}

void Test_FM_ChildFileInfoCmd_FileInfoCRCNotEqualToAnyMissionES(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode   = FM_GET_FILE_INFO_CC,
                                        .Source1       = "source1",
                                        .Source2       = "source2",
                                        .FileInfoCRC   = -1,
                                        .FileInfoState = FM_NAME_IS_FILE_CLOSED};

    /* Act */
    UtAssert_VOIDCALL(FM_ChildFileInfoCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 1, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_OpenCreate, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_FILE_INFO_TYPE_WARNING_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_GET_FILE_INFO_CMD_INF_EID);
}

void Test_FM_ChildFileInfoCmd_OSOpenCreateTrueBytesReadZero(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode   = FM_GET_FILE_INFO_CC,
                                        .Source1       = "source1",
                                        .Source2       = "source2",
                                        .FileInfoCRC   = CFE_ES_CrcType_CRC_16,
                                        .FileInfoState = FM_NAME_IS_FILE_CLOSED};

    UT_SetDefaultReturnValue(UT_KEY(OS_read), 0);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildFileInfoCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_read, 1);
    UtAssert_STUB_COUNT(OS_OpenCreate, 1);
    UtAssert_STUB_COUNT(OS_close, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_FILE_INFO_CMD_INF_EID);
}

void Test_FM_ChildFileInfoCmd_BytesReadLessThanZero(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode   = FM_GET_FILE_INFO_CC,
                                        .Source1       = "source1",
                                        .Source2       = "source2",
                                        .FileInfoCRC   = CFE_ES_CrcType_CRC_16,
                                        .FileInfoState = FM_NAME_IS_FILE_CLOSED};

    UT_SetDefaultReturnValue(UT_KEY(OS_read), -1);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildFileInfoCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 1, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_read, 1);
    UtAssert_STUB_COUNT(OS_OpenCreate, 1);
    UtAssert_STUB_COUNT(OS_close, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_FILE_INFO_READ_WARNING_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_GET_FILE_INFO_CMD_INF_EID);
}

void Test_FM_ChildFileInfoCmd_BytesReadGreaterThanZero(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode   = FM_GET_FILE_INFO_CC,
                                        .Source1       = "source1",
                                        .Source2       = "source2",
                                        .FileInfoCRC   = CFE_ES_CrcType_CRC_8,
                                        .FileInfoState = FM_NAME_IS_FILE_CLOSED};

    UT_SetDefaultReturnValue(UT_KEY(OS_read), 1);
    UT_SetDeferredRetcode(UT_KEY(OS_read), FM_CHILD_FILE_LOOP_COUNT + 1, 0);
    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_CalculateCRC), 0);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildFileInfoCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_read, FM_CHILD_FILE_LOOP_COUNT + 1);
    UtAssert_STUB_COUNT(OS_TaskDelay, 1);
    UtAssert_STUB_COUNT(OS_OpenCreate, 1);
    UtAssert_STUB_COUNT(OS_close, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_FILE_INFO_CMD_INF_EID);
}

/* ****************
 * ChildCreateDirectoryCmd Tests
 * ***************/
void Test_FM_ChildCreateDirectoryCmd_OSMkDirSuccess(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode = FM_CREATE_DIRECTORY_CC};

    /* Act */
    UtAssert_VOIDCALL(FM_ChildCreateDirectoryCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_mkdir, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CREATE_DIR_CMD_INF_EID);
}

void Test_FM_ChildCreateDirectoryCmd_OSMkDirNotSuccess(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode = FM_CREATE_DIRECTORY_CC};

    UT_SetDefaultReturnValue(UT_KEY(OS_mkdir), !CFE_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildCreateDirectoryCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_mkdir, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CREATE_DIR_OS_ERR_EID);
}

/* ****************
 * ChildDeleteDirectoryCmd Tests
 * ***************/
void Test_FM_ChildDeleteDirectoryCmd_OSDirectoryOpenNoSuccess(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode = FM_DELETE_DIRECTORY_CC};

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDeleteDirectoryCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_DirectoryOpen, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_OPENDIR_OS_ERR_EID);
}

void Test_FM_ChildDeleteDirectoryCmd_OSDirectoryReadNoSuccess(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode = FM_DELETE_DIRECTORY_CC};

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDeleteDirectoryCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_DirectoryOpen, 1);
    UtAssert_STUB_COUNT(OS_DirectoryRead, 1);
    UtAssert_STUB_COUNT(OS_DirectoryClose, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_DIR_CMD_INF_EID);
}

void Test_FM_ChildDeleteDirectoryCmd_StrCmpThisDirectoryZero(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode = FM_DELETE_DIRECTORY_CC};
    os_dirent_t          direntry    = {.FileName = FM_THIS_DIRECTORY};

    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &direntry, sizeof(direntry), false);
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDeleteDirectoryCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_DirectoryOpen, 1);
    UtAssert_STUB_COUNT(OS_DirectoryRead, 2);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_DIR_CMD_INF_EID);
}

void Test_FM_ChildDeleteDirectoryCmd_StrCmpParentDirectoryZero(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode = FM_DELETE_DIRECTORY_CC};
    os_dirent_t          direntry    = {.FileName = FM_PARENT_DIRECTORY};

    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &direntry, sizeof(direntry), false);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDeleteDirectoryCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_DirectoryOpen, 1);
    UtAssert_STUB_COUNT(OS_DirectoryRead, 3);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_DIR_EMPTY_ERR_EID);
}

void Test_FM_ChildDeleteDirectoryCmd_RemoveTheDirIsTrueRmDirSuccess(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode = FM_DELETE_DIRECTORY_CC};

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDeleteDirectoryCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_DirectoryOpen, 1);
    UtAssert_STUB_COUNT(OS_DirectoryRead, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_DIR_CMD_INF_EID);
}

void Test_FM_ChildDeleteDirectoryCmd_RemoveDirTrueOSRmDirNotSuccess(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode = FM_DELETE_DIRECTORY_CC};

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), !OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_rmdir), !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDeleteDirectoryCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_DirectoryOpen, 1);
    UtAssert_STUB_COUNT(OS_DirectoryRead, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_RMDIR_OS_ERR_EID);
}

/* ****************
 * ChildDirListFileCmd Tests
 * ***************/
void Test_FM_ChildDirListFileCmd_OSDirOpenNotSuccess(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode = FM_GET_DIR_LIST_FILE_CC};

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDirListFileCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_DirectoryOpen, 1);
    UtAssert_STUB_COUNT(OS_DirectoryClose, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_FILE_OSOPENDIR_ERR_EID);
}

void Test_FM_ChildDirListFileCmd_ChildDirListFileInitFalse(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {
        .CommandCode = FM_GET_DIR_LIST_FILE_CC, .Source1 = "source1", .Target = "target"};

    UT_SetDefaultReturnValue(UT_KEY(OS_OpenCreate), !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDirListFileCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_DirectoryOpen, 1);
    UtAssert_STUB_COUNT(OS_DirectoryClose, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_close, 0);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_FILE_OSCREAT_ERR_EID);
}

void Test_FM_ChildDirListFileCmd_ChildDirListFileInitTrue(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {
        .CommandCode = FM_GET_DIR_LIST_FILE_CC, .Source1 = "source1", .Target = "target"};

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDirListFileCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_DirectoryOpen, 1);
    UtAssert_STUB_COUNT(OS_DirectoryClose, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_close, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_FILE_CMD_INF_EID);
}

/* ****************
 * ChildDirListPktCmd Tests
 * ***************/
void Test_FM_ChildDirListPktCmd_OSDirOpenNotSuccess(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {
        .CommandCode = FM_GET_DIR_LIST_PKT_CC, .Source1 = "source1", .Source2 = "source2"};

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDirListPktCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_DirectoryOpen, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_PKT_OS_ERR_EID);
}

void Test_FM_ChildDirListPktCmd_OSDirReadNotSuccess(void)
{
    FM_DirListPkt_Payload_t *ReportPtr;

    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {
        .CommandCode = FM_GET_DIR_LIST_PKT_CC, .Source1 = "source1", .Source2 = "source2"};

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDirListPktCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_DirectoryOpen, 1);
    UtAssert_STUB_COUNT(OS_DirectoryRead, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_PKT_CMD_INF_EID);

    ReportPtr = &FM_GlobalData.DirListPkt.Payload;
    UtAssert_UINT32_EQ(ReportPtr->PacketFiles, 0);
}

void Test_FM_ChildDirListPktCmd_DirEntryNameThisDirectory(void)
{
    FM_DirListPkt_Payload_t *ReportPtr;

    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {
        .CommandCode = FM_GET_DIR_LIST_PKT_CC, .Source1 = "source1", .Source2 = "source2"};
    os_dirent_t direntry = {.FileName = FM_THIS_DIRECTORY};

    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &direntry, sizeof(direntry), false);
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDirListPktCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_DirectoryOpen, 1);
    UtAssert_STUB_COUNT(OS_DirectoryRead, 2);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_PKT_CMD_INF_EID);

    ReportPtr = &FM_GlobalData.DirListPkt.Payload;
    UtAssert_UINT32_EQ(ReportPtr->PacketFiles, 0);
}

void Test_FM_ChildDirListPktCmd_DirEntryNameParentDirectory(void)
{
    FM_DirListPkt_Payload_t *ReportPtr;

    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {
        .CommandCode = FM_GET_DIR_LIST_PKT_CC, .Source1 = "source1", .Source2 = "source2"};
    os_dirent_t direntry = {.FileName = FM_PARENT_DIRECTORY};

    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &direntry, sizeof(direntry), false);
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDirListPktCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_DirectoryOpen, 1);
    UtAssert_STUB_COUNT(OS_DirectoryRead, 2);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_PKT_CMD_INF_EID);

    ReportPtr = &FM_GlobalData.DirListPkt.Payload;
    UtAssert_UINT32_EQ(ReportPtr->PacketFiles, 0);
}

void Test_FM_ChildDirListPktCmd_DirListOffsetNotExceeded(void)
{
    FM_DirListPkt_Payload_t *ReportPtr;

    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {
        .CommandCode = FM_GET_DIR_LIST_PKT_CC, .Source1 = "source1", .Source2 = "source2", .DirListOffset = 1};
    os_dirent_t direntry = {.FileName = "filename"};

    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &direntry, sizeof(direntry), false);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDirListPktCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_DirectoryOpen, 1);
    UtAssert_STUB_COUNT(OS_DirectoryRead, 2);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_PKT_CMD_INF_EID);

    ReportPtr = &FM_GlobalData.DirListPkt.Payload;
    UtAssert_UINT32_EQ(ReportPtr->FirstFile, 1);
    UtAssert_UINT32_EQ(ReportPtr->TotalFiles, 1);
    UtAssert_UINT32_EQ(ReportPtr->PacketFiles, 0);
}

void Test_FM_ChildDirListPktCmd_DirListOffsetExceeded(void)
{
    FM_DirListPkt_Payload_t *ReportPtr;

    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {
        .CommandCode = FM_GET_DIR_LIST_PKT_CC, .Source1 = "source1", .Source2 = "source2"};
    os_dirent_t direntry[FM_DIR_LIST_PKT_ENTRIES + 1];

    /* Unit under test doesn't really care if the entry name is empty */
    memset(direntry, 0, sizeof(direntry));

    /* Will fill the entire packet and have one more */
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), sizeof(direntry) / sizeof(direntry[0]) + 1, !OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &direntry, sizeof(direntry), false);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDirListPktCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_DirectoryOpen, 1);
    UtAssert_STUB_COUNT(OS_DirectoryRead, sizeof(direntry) / sizeof(direntry[0]) + 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_PKT_CMD_INF_EID);

    ReportPtr = &FM_GlobalData.DirListPkt.Payload;
    UtAssert_UINT32_EQ(ReportPtr->FirstFile, 0);
    UtAssert_UINT32_EQ(ReportPtr->TotalFiles, sizeof(direntry) / sizeof(direntry[0]));
    UtAssert_UINT32_EQ(ReportPtr->PacketFiles, FM_DIR_LIST_PKT_ENTRIES);
}

void Test_FM_ChildDirListPktCmd_PathAndEntryLengthGreaterMaxPathLength(void)
{
    FM_DirListPkt_Payload_t *ReportPtr;

    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode = FM_DELETE_ALL_FILES_CC,
                                        .Source1     = "source1",
                                        .Source2 = "source2_has_quite_a_long_name_to_make_path_length_longer_than_64"};
    os_dirent_t          direntry    = {.FileName = "direntry_long"};

    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &direntry, sizeof(direntry), false);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDirListPktCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 1, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_DirectoryOpen, 1);
    UtAssert_STUB_COUNT(OS_DirectoryRead, 2);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_PKT_WARNING_EID);

    ReportPtr = &FM_GlobalData.DirListPkt.Payload;
    UtAssert_UINT32_EQ(ReportPtr->FirstFile, 0);
    UtAssert_UINT32_EQ(ReportPtr->TotalFiles, 1);
    UtAssert_UINT32_EQ(ReportPtr->PacketFiles, 0);
}

/* ****************
 * ChildSetPermissionsCmd Tests
 * ***************/
void Test_FM_ChildSetPermissionsCmd_OSChmodSuccess(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode = FM_SET_PERMISSIONS_CC};

    /* Act */
    UtAssert_VOIDCALL(FM_ChildSetPermissionsCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_chmod, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_SET_PERM_CMD_INF_EID);
}

void Test_FM_ChildSetPermissionsCmd_OSChmodNotSuccess(void)
{
    /* Arrange */
    FM_ChildQueueEntry_t queue_entry = {.CommandCode = FM_SET_PERMISSIONS_CC};

    UT_SetDefaultReturnValue(UT_KEY(OS_chmod), !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildSetPermissionsCmd(&queue_entry));

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, queue_entry.CommandCode);

    UtAssert_STUB_COUNT(OS_chmod, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_SET_PERM_OS_ERR_EID);
}

/* ****************
 * ChildDirListFileInit Tests
 * ***************/
void Test_FM_ChildDirListFileInit_OSOpenCreateFail(void)
{
    /* Arrange */
    osal_id_t   fileid;
    const char *directory = "directory";
    const char *filename  = "filename";

    UT_SetDefaultReturnValue(UT_KEY(OS_OpenCreate), !OS_SUCCESS);

    /* Act */
    UtAssert_BOOL_FALSE(FM_ChildDirListFileInit(&fileid, directory, filename));

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, 0);

    UtAssert_STUB_COUNT(OS_OpenCreate, 1);
    UtAssert_STUB_COUNT(OS_close, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_FILE_OSCREAT_ERR_EID);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCmdErrCounter, 1);
}

void Test_FM_ChildDirListFileInit_FSWriteHeaderNotSameSizeFSHeadert(void)
{
    /* Arrange */
    osal_id_t   fileid;
    osal_id_t   LocalFileHandle = FM_UT_OBJID_1;
    const char *directory       = "directory";
    const char *filename        = "filename";

    UT_SetDataBuffer(UT_KEY(OS_OpenCreate), &LocalFileHandle, sizeof(osal_id_t), false);
    UT_SetDefaultReturnValue(UT_KEY(CFE_FS_WriteHeader), sizeof(CFE_FS_Header_t) - 1);

    /* Act */
    UtAssert_BOOL_FALSE(FM_ChildDirListFileInit(&fileid, directory, filename));

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, 0);

    UtAssert_STUB_COUNT(OS_OpenCreate, 1);
    UtAssert_STUB_COUNT(CFE_FS_WriteHeader, 1);
    UtAssert_STUB_COUNT(OS_close, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_FILE_WRHDR_ERR_EID);
}

void Test_FM_ChildDirListFileInit_OSWriteNotSameSizeDirListFileStatst(void)
{
    /* Arrange */
    osal_id_t   fileid;
    const char *directory       = "directory";
    const char *filename        = "filename";
    osal_id_t   LocalFileHandle = FM_UT_OBJID_1;

    UT_SetDataBuffer(UT_KEY(OS_OpenCreate), &LocalFileHandle, sizeof(osal_id_t), false);
    UT_SetDefaultReturnValue(UT_KEY(OS_write), sizeof(FM_DirListFileStats_t) - 1);

    /* Act */
    UtAssert_BOOL_FALSE(FM_ChildDirListFileInit(&fileid, directory, filename));

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, 0);

    UtAssert_STUB_COUNT(OS_OpenCreate, 1);
    UtAssert_STUB_COUNT(CFE_FS_WriteHeader, 1);
    UtAssert_STUB_COUNT(OS_write, 1);
    UtAssert_STUB_COUNT(OS_close, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_FILE_WRBLANK_ERR_EID);
}

void Test_FM_ChildDirListFileInit_OSWriteSameSizeDirListFileStatst(void)
{
    /* Arrange */
    osal_id_t   fileid;
    const char *directory = "directory";
    const char *filename  = "filename";

    /* Act */
    UtAssert_BOOL_TRUE(FM_ChildDirListFileInit(&fileid, directory, filename));

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 0, 0, 0);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 0);
    UtAssert_STUB_COUNT(OS_OpenCreate, 1);
    UtAssert_STUB_COUNT(CFE_FS_WriteHeader, 1);
    UtAssert_STUB_COUNT(OS_write, 1);
    UtAssert_STUB_COUNT(OS_close, 0);
}

/* ****************
 * ChildDirListFileLoop Tests
 * ***************/
void Test_FM_ChildDirListFileLoop_OSDirReadNotSuccess(void)
{
    /* Arrange */
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDirListFileLoop(FM_UT_OBJID_1, FM_UT_OBJID_2, "dir", "dir/", "fname", false));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 0, 0);

    UtAssert_STUB_COUNT(OS_DirectoryRead, 1);
    UtAssert_STUB_COUNT(OS_write, 0);
    UtAssert_STUB_COUNT(OS_lseek, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_FILE_CMD_INF_EID);

    UtAssert_UINT32_EQ(FM_GlobalData.DirListFileStats.DirEntries, 0);
    UtAssert_UINT32_EQ(FM_GlobalData.DirListFileStats.FileEntries, 0);
}

void Test_FM_ChildDirListFileLoop_OSDirEntryNameIsThisDirectory(void)
{
    /* Arrange */
    os_dirent_t direntry = {.FileName = FM_THIS_DIRECTORY};

    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &direntry, sizeof(direntry), false);
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDirListFileLoop(FM_UT_OBJID_1, FM_UT_OBJID_2, "dir", "dir/", "fname", false));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 0, 0);

    UtAssert_STUB_COUNT(OS_DirectoryRead, 2);
    UtAssert_STUB_COUNT(OS_write, 0);
    UtAssert_STUB_COUNT(OS_lseek, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_FILE_CMD_INF_EID);

    UtAssert_UINT32_EQ(FM_GlobalData.DirListFileStats.DirEntries, 0);
    UtAssert_UINT32_EQ(FM_GlobalData.DirListFileStats.FileEntries, 0);
}

void Test_FM_ChildDirListFileLoop_OSDirEntryNameIsParentDirectory(void)
{
    /* Arrange */
    os_dirent_t direntry = {.FileName = FM_PARENT_DIRECTORY};

    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &direntry, sizeof(direntry), false);
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDirListFileLoop(FM_UT_OBJID_1, FM_UT_OBJID_2, "dir", "dir/", "fname", false));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 0, 0);

    UtAssert_STUB_COUNT(OS_DirectoryRead, 2);
    UtAssert_STUB_COUNT(OS_write, 0);
    UtAssert_STUB_COUNT(OS_lseek, 0);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_FILE_CMD_INF_EID);
}

void Test_FM_ChildDirListFileLoop_PathLengthAndEntryLengthGreaterMaxPathLen(void)
{
    /* Arrange */
    char        dirwithsep[OS_MAX_PATH_LEN];
    os_dirent_t direntry = {.FileName = "directory_nam"};

    memset(dirwithsep, 0xFF, sizeof(dirwithsep));
    dirwithsep[sizeof(dirwithsep) - 1] = '\0';

    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &direntry, sizeof(direntry), false);
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDirListFileLoop(FM_UT_OBJID_1, FM_UT_OBJID_2, "dir", dirwithsep, "fname", false));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 1, 0);

    UtAssert_STUB_COUNT(OS_DirectoryRead, 2);
    UtAssert_STUB_COUNT(OS_write, 1);
    UtAssert_STUB_COUNT(OS_lseek, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_FILE_WARNING_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_GET_DIR_FILE_CMD_INF_EID);

    UtAssert_INT32_EQ(FM_GlobalData.DirListFileStats.DirEntries, 1);
    UtAssert_INT32_EQ(FM_GlobalData.DirListFileStats.FileEntries, 0);
}

void Test_FM_ChildDirListFileLoop_FileEntriesGreaterFMDirListFileEntries(void)
{
    os_dirent_t direntry = {.FileName = "directory_nam"};
    uint32      entrycnt = FM_DIR_LIST_FILE_ENTRIES + 1;

    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &direntry, sizeof(direntry), false);
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), entrycnt + 1, !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDirListFileLoop(FM_UT_OBJID_1, FM_UT_OBJID_2, "dir", "dir/", "fname", false));

    /* Assert */
    UT_FM_Child_Cmd_Assert(1, 0, 0, 0);

    UtAssert_STUB_COUNT(OS_DirectoryRead, entrycnt + 1);
    UtAssert_STUB_COUNT(OS_write, FM_DIR_LIST_FILE_ENTRIES + 1);
    UtAssert_STUB_COUNT(OS_lseek, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_FILE_CMD_INF_EID);

    UtAssert_INT32_EQ(FM_GlobalData.DirListFileStats.DirEntries, entrycnt);
    UtAssert_INT32_EQ(FM_GlobalData.DirListFileStats.FileEntries, FM_DIR_LIST_FILE_ENTRIES);
}

void Test_FM_ChildDirListFileLoop_BytesWrittenNotEqualWriteLength(void)
{
    /* Arrange */
    os_dirent_t direntry = {.FileName = "directory_nam"};

    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &direntry, sizeof(direntry), false);
    UT_SetDefaultReturnValue(UT_KEY(OS_write), sizeof(FM_DirListEntry_t));
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDirListFileLoop(FM_UT_OBJID_1, FM_UT_OBJID_2, "dir", "dir/", "fname", false));

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, 0);

    UtAssert_STUB_COUNT(OS_DirectoryRead, 2);
    UtAssert_STUB_COUNT(OS_write, 2);
    UtAssert_STUB_COUNT(OS_lseek, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_FILE_UPSTATS_ERR_EID);

    UtAssert_INT32_EQ(FM_GlobalData.DirListFileStats.DirEntries, 1);
    UtAssert_INT32_EQ(FM_GlobalData.DirListFileStats.FileEntries, 1);
}

void Test_FM_ChildDirListFileLoop_BytesWrittenNotEqualWriteLengthInLoop(void)
{
    /* Arrange */
    os_dirent_t direntry = {.FileName = "directory_nam"};

    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &direntry, sizeof(direntry), false);
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_write), sizeof(FM_DirListEntry_t) - 1);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildDirListFileLoop(FM_UT_OBJID_1, FM_UT_OBJID_2, "dir", "dir/", "fname", false));

    /* Assert */
    UT_FM_Child_Cmd_Assert(0, 1, 0, 0);

    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_STUB_COUNT(OS_DirectoryRead, 1);
    UtAssert_STUB_COUNT(OS_write, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_FILE_WRENTRY_ERR_EID);

    UtAssert_INT32_EQ(FM_GlobalData.DirListFileStats.DirEntries, 0);
    UtAssert_INT32_EQ(FM_GlobalData.DirListFileStats.FileEntries, 0);
}

/* ****************
 * ChildSizeTimeMode Tests
 * ***************/
void Test_FM_ChildSizeTimeMode_OsStatNoSuccess(void)
{
    /* Arrange */
    uint32 filesize = 1;
    uint32 filetime = 1;
    uint32 filemode = 1;

    UT_SetDefaultReturnValue(UT_KEY(OS_stat), !OS_SUCCESS);

    /* Act */
    UtAssert_INT32_EQ(FM_ChildSizeTimeMode("fname", &filesize, &filetime, &filemode), !OS_SUCCESS);

    /* Assert */
    UtAssert_STUB_COUNT(OS_stat, 1);
    UtAssert_UINT32_EQ(filesize, 0);
    UtAssert_UINT32_EQ(filetime, 0);
    UtAssert_UINT32_EQ(filemode, 0);
}

void Test_FM_ChildSizeTimeMode_OSFilestateTimeDefined(void)
{
    /* Arrange */
    uint32 filesize = 0;
    uint32 filetime = 0;
    uint32 filemode = 0;

    OS_time_t  ostime     = {.ticks = 1};
    os_fstat_t filestatus = {.FileModeBits = 2, .FileTime = ostime, .FileSize = 3};

    UT_SetDataBuffer(UT_KEY(OS_stat), &filestatus, sizeof(filestatus), false);

    /* Act */
    UtAssert_INT32_EQ(FM_ChildSizeTimeMode("fname", &filesize, &filetime, &filemode), OS_SUCCESS);

    /* Assert */
    UtAssert_STUB_COUNT(OS_stat, 1);
    UtAssert_UINT32_EQ(filetime, OS_FILESTAT_TIME(filestatus));
    UtAssert_UINT32_EQ(filesize, OS_FILESTAT_SIZE(filestatus));
    UtAssert_UINT32_EQ(filemode, OS_FILESTAT_MODE(filestatus));
}

/* ****************
 * ChildLoop Tests
 * ***************/
void Test_FM_ChildLoop_CountSemTakeNotSuccess(void)
{
    /* Arrange */
    UT_SetDefaultReturnValue(UT_KEY(OS_CountSemTake), !CFE_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildLoop());

    /* Assert */
    UtAssert_STUB_COUNT(OS_CountSemTake, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CHILD_TERM_SEM_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCmdErrCounter, 0);
}

void Test_FM_ChildLoop_ChildQCountEqualZero(void)
{
    /* Act */
    UtAssert_VOIDCALL(FM_ChildLoop());

    /* Assert */
    UtAssert_STUB_COUNT(OS_CountSemTake, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CHILD_TERM_EMPTYQ_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCmdErrCounter, 1);
}

void Test_FM_ChildLoop_ChildReadIndexEqualChildQDepth(void)
{
    /* Arrange */
    FM_GlobalData.ChildQueueCount = 1;
    FM_GlobalData.ChildReadIndex  = FM_CHILD_QUEUE_DEPTH;

    /* Act */
    UtAssert_VOIDCALL(FM_ChildLoop());

    /* Assert */
    UtAssert_STUB_COUNT(OS_CountSemTake, 1);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CHILD_TERM_QIDX_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCmdErrCounter, 1);
}

void Test_FM_ChildLoop_CountSemTakeSuccessDefault(void)
{
    /* Arrange */
    FM_GlobalData.ChildQueueCount    = 1;
    FM_GlobalData.ChildReadIndex     = 0;
    FM_ChildQueueEntry_t queue_entry = {.CommandCode = -1};

    FM_GlobalData.ChildQueue[0] = queue_entry;
    UT_SetDeferredRetcode(UT_KEY(OS_CountSemTake), 2, !CFE_SUCCESS);

    /* Act */
    UtAssert_VOIDCALL(FM_ChildLoop());

    /* Assert */
    UtAssert_STUB_COUNT(OS_CountSemTake, 2);
    UtAssert_STUB_COUNT(CFE_EVS_SendEvent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CHILD_EXE_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_CHILD_TERM_SEM_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCmdErrCounter, 1);
}

/* ****************
 * ChildSleepStat Tests
 * ***************/

void Test_FM_ChildSleepStat_getSizeTimeModeFalse(void)
{
    /* Arrange */
    FM_DirListEntry_t DirListData    = {.EntrySize = 1, .ModifyTime = 1, .Mode = 1};
    int32             FilesTillSleep = 1;

    /* Assert */
    UtAssert_VOIDCALL(FM_ChildSleepStat("fname", &DirListData, &FilesTillSleep, false));
    UtAssert_INT32_EQ(DirListData.EntrySize, 0);
    UtAssert_INT32_EQ(DirListData.ModifyTime, 0);
    UtAssert_INT32_EQ(DirListData.Mode, 0);
}

void Test_FM_ChildSleepStat_FilesTillSleepPositive(void)
{
    /* Arrange */
    FM_DirListEntry_t DirListData           = {.EntrySize = 1, .ModifyTime = 1, .Mode = 1};
    int32             FilesTillSleep        = FM_CHILD_STAT_SLEEP_FILECOUNT + 1;
    int32             FilesTillSleep_before = FilesTillSleep;

    /* Assert */
    UtAssert_VOIDCALL(FM_ChildSleepStat("fname", &DirListData, &FilesTillSleep, true));
    UtAssert_INT32_EQ(FilesTillSleep, FilesTillSleep_before - 1);
}

void Test_FM_ChildSleepStat_FilesTillSleepLTEQZero(void)
{
    /* Arrange */
    FM_DirListEntry_t DirListData    = {.EntrySize = 1, .ModifyTime = 1, .Mode = 1};
    int32             FilesTillSleep = 0;

    /* Assert */
    UtAssert_VOIDCALL(FM_ChildSleepStat("fname", &DirListData, &FilesTillSleep, true));
    UtAssert_STUB_COUNT(OS_TaskDelay, 1);
    UtAssert_INT32_EQ(FilesTillSleep, FM_CHILD_STAT_SLEEP_FILECOUNT - 1);
}

/* * * * * * * * * * * * * *
 * Add Method Tests
 * * * * * * * * * * * * * */
void add_FM_ChildInit_tests(void)
{
    UtTest_Add(Test_FM_ChildInit_CountSemCreateNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildInit_CountSemCreateNotSuccess");

    UtTest_Add(Test_FM_ChildInit_MutSemCreateNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildInit_MutSemCreateNotSuccess");

    UtTest_Add(Test_FM_ChildInit_MuteSemCreateSuccess_CreateChildTaskNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildInit_MuteSemCreateSuccess_CreateChildTaskNotSuccess");

    UtTest_Add(Test_FM_ChildInit_ReturnSuccess, FM_Test_Setup, FM_Test_Teardown, "Test_FM_ChildInit_ReturnSuccess");
}

void add_FM_ChildTask_tests(void)
{
    UtTest_Add(Test_FM_ChildTask_ChildLoopCalled, FM_Test_Setup, FM_Test_Teardown, "FM_ChildTask_ChildLoopCalled");
}

void add_FM_ChildProcess_tests(void)
{
    UtTest_Add(Test_FM_ChildProcess_FMCopyCC, FM_Test_Setup, FM_Test_Teardown, "Test_FM_ChildProcess_FMCopyCC");

    UtTest_Add(Test_FM_ChildProcess_FMMoveCC, FM_Test_Setup, FM_Test_Teardown, "Test_FM_ChildProcess_FMMoveCC");

    UtTest_Add(Test_FM_ChildProcess_FMRenameCC, FM_Test_Setup, FM_Test_Teardown, "Test_FM_ChildProcess_FMRenameCC");

    UtTest_Add(Test_FM_ChildProcess_FMDeleteCC, FM_Test_Setup, FM_Test_Teardown, "Test_FM_ChildProcess_FMDeleteCC");

    UtTest_Add(Test_FM_ChildProcess_FMDeleteAllCC, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildProcess_FMDeleteAllCC");

    UtTest_Add(Test_FM_ChildProcess_FMDecompressCC, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildProcess_FMDecompressCC");

    UtTest_Add(Test_FM_ChildProcess_FMConcatCC, FM_Test_Setup, FM_Test_Teardown, "Test_FM_ChildProcess_FMConcatCC");

    UtTest_Add(Test_FM_ChildProcess_FMCreateDirCC, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildProcess_FMCreateDirCC");

    UtTest_Add(Test_FM_ChildProcess_FMDeleteDirCC, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildProcess_FMDeleteDirCC");

    UtTest_Add(Test_FM_ChildProcess_FMGetFileInfoCC, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildProcess_FMGetFileInfoCC");

    UtTest_Add(Test_FM_ChildProcess_FMGetDirListsFileCC, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildProcess_FMGetDirListsFileCC");

    UtTest_Add(Test_FM_ChildProcess_FMGetDirListsPktCC, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildProcess_FMGetDirListsPktCC");

    UtTest_Add(Test_FM_ChildProcess_FMSetFilePermCC, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildProcess_FMSetFilePermCC");

    UtTest_Add(Test_FM_ChildProcess_DefaultSwitch, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildProcess_DefaultSwitch");

    UtTest_Add(Test_FM_ChildProcess_ChildReadIndexGreaterChildQDepth, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildProcess_ChildReadIndexGreaterChildQDepth");
}

void add_FM_ChildCopyCmd_tests(void)
{
    UtTest_Add(Test_FM_ChildCopyCmd_OScpIsSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildCopyCmd_OScpIsSuccess");

    UtTest_Add(Test_FM_ChildCopyCmd_OScpNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildCopyCmd_OScpNotSuccess");
}

void add_FM_ChildMoveCmd_tests(void)
{
    UtTest_Add(Test_FM_ChildMoveCmd_OSmvNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildMoveCmd_OSmvNotSuccess");

    UtTest_Add(Test_FM_ChildMoveCmd_OSmvSuccess, FM_Test_Setup, FM_Test_Teardown, "Test_FM_ChildMoveCmd_OSmvSuccess");
}

void add_FM_ChildRenameCmd_tests(void)
{
    UtTest_Add(Test_FM_ChildRenameCmd_OSRenameNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildRenameCmd_OSRenameNotSuccess");

    UtTest_Add(Test_FM_ChildRenameCmd_OSRenameSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildRenameCmd_OSRenameSuccess");
}

void add_FM_ChildDeleteCmd_tests(void)
{
    UtTest_Add(Test_FM_ChildDeleteCmd_OSRemoveSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteCmd_OSRemoveSuccess");

    UtTest_Add(Test_FM_ChildDeleteCmd_OSRemoveNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteCmd_OSRemoveNotSuccess");
}

void add_FM_ChildDeleteAllFilesCmd_tests(void)
{
    UtTest_Add(Test_FM_ChildDeleteAllFilesCmd_DirOpenNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteAllFilesCmd_DirOpenNotSuccess");

    UtTest_Add(Test_FM_ChildDeleteAllFilesCmd_DirReadNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteAllFilesCmd_DirReadNotSuccess");

    UtTest_Add(Test_FM_ChildDeleteAllFilesCmd_DirEntryThisDirectory, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteAllFilesCmd_DirEntryThisDirectory");

    UtTest_Add(Test_FM_ChildDeleteAllFilesCmd_DirEntryParentDirectory, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteAllFilesCmd_DirEntryParentDirectory");

    UtTest_Add(Test_FM_ChildDeleteAllFilesCmd_PathFilenameLengthGreaterMaxPthLen, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteAllFilesCmd_PathFilenameLengthGreaterMaxPthLen");

    UtTest_Add(Test_FM_ChildDeleteAllFilesCmd_InvalidFilenameState, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteAllFilesCmd_InvalidFilenameState");

    UtTest_Add(Test_FM_ChildDeleteAllFilesCmd_NotInUseFilenameState, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteAllFilesCmd_NotInUseFilenameState");

    UtTest_Add(Test_FM_ChildDeleteAllFilesCmd_DirectoryFilenameState, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteAllFilesCmd_DirectoryFilenameState");

    UtTest_Add(Test_FM_ChildDeleteAllFilesCmd_OpenFilenameState, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteAllFilesCmd_OpenFilenameState");

    UtTest_Add(Test_FM_ChildDeleteAllFilesCmd_ClosedFilename_OSRmNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteAllFilesCmd_ClosedFilename_OSRmNotSuccess");

    UtTest_Add(Test_FM_ChildDeleteAllFilesCmd_ClosedFilename_OSrmSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteAllFilesCmd_ClosedFilename_OSrmSuccess");

    UtTest_Add(Test_FM_ChildDeleteAllFilesCmd_FilenameStateDefaultReturn, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteAllFilesCmd_FilenameStateDefaultReturn");
}

void add_FM_ChildDecompressFileCmd_tests(void)
{
    UtTest_Add(Test_FM_ChildDecompressFileCmd_FSDecompressNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDecompressFileCmd_FSDecompressNotSuccess");

    UtTest_Add(Test_FM_ChildDecompressFileCmd_FSDecompressSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDecompressFileCmd_FSDecompressSuccess");
}

void add_FM_ChildConcatFilesCmd_tests(void)
{
    UtTest_Add(Test_FM_ChildConcatFilesCmd_OSCpNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildConcatFilesCmd_OSCpNotSuccess");

    UtTest_Add(Test_FM_ChildConcatFilesCmd_OSOpenCreateSourceNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildConcatFilesCmd_OSOpenCreateSourceNotSuccess");

    UtTest_Add(Test_FM_ChildConcatFilesCmd_OSOpenCreateTargetNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildConcatFilesCmd_OSOpenCreateTargetNotSuccess");

    UtTest_Add(Test_FM_ChildConcatFilesCmd_OSReadBytesZero, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildConcatFilesCmd_OSReadBytesZero");

    UtTest_Add(Test_FM_ChildConcatFilesCmd_OSReadBytesLessThanZero, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildConcatFilesCmd_OSReadBytesLessThanZero");

    UtTest_Add(Test_FM_ChildConcatFilesCmd_BytesWrittenNotEqualBytesRead, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildConcatFilesCmd_BytesWrittenNotEqualBytesRead");

    UtTest_Add(Test_FM_ChildConcatFilesCmd_CopyInProgressTrueLoopCountEqualChildFileLoopCount, FM_Test_Setup,
               FM_Test_Teardown, "Test_FM_ChildConcatFilesCmd_CopyInProgressTrueLoopCountEqualChildFileLoopCount");
}

void add_FM_ChildFileInfoCmd_tests(void)
{
    UtTest_Add(Test_FM_ChildFileInfoCmd_FileInfoCRCEqualIgnoreCRC, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildFileInfoCmd_FileInfoCRCEqualIgnoreCRC");

    UtTest_Add(Test_FM_ChildFileInfoCmd_FileInfoStateIsNotFileClosed, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildFileInfoCmd_FileInfoStateIsNotFileClosed");

    UtTest_Add(Test_FM_ChildFileInfoCmd_FileInfoCRCEqualMission8, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildFileInfoCmd_FileInfoCRCEqualMission8");

    UtTest_Add(Test_FM_ChildFileInfoCmd_FileInfoCRCEqualMission16, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildFileInfoCmd_FileInfoCRCEqualMission16");

    UtTest_Add(Test_FM_ChildFileInfoCmd_FileInfoCRCEqualMission32, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildFileInfoCmd_FileInfoCRCEqualMission32");

    UtTest_Add(Test_FM_ChildFileInfoCmd_FileInfoCRCNotEqualToAnyMissionES, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildFileInfoCmd_FileInfoCRCNotEqualToAnyMissionES");

    UtTest_Add(Test_FM_ChildFileInfoCmd_OSOpenCreateTrueBytesReadZero, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildFileInfoCmd_OSOpenCreateTrueBytesReadZero");

    UtTest_Add(Test_FM_ChildFileInfoCmd_BytesReadLessThanZero, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildFileInfoCmd_BytesReadLessThanZero");

    UtTest_Add(Test_FM_ChildFileInfoCmd_BytesReadGreaterThanZero, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildFileInfoCmd_BytesReadGreaterThanZero");
}

void add_FM_ChildCreateDirectoryCmd_tests(void)
{
    UtTest_Add(Test_FM_ChildCreateDirectoryCmd_OSMkDirNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildCreateDirectoryCmd_OSMkDirNotSuccess");

    UtTest_Add(Test_FM_ChildCreateDirectoryCmd_OSMkDirSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildCreateDirectoryCmd_OSMkDirSuccess");
}

void add_FM_ChildDeleteDirectoryCmd_tests(void)
{
    UtTest_Add(Test_FM_ChildDeleteDirectoryCmd_OSDirectoryOpenNoSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteDirectoryCmd_OSDirectoryOpenNoSuccess");

    UtTest_Add(Test_FM_ChildDeleteDirectoryCmd_OSDirectoryReadNoSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteDirectoryCmd_OSDirectoryReadNoSuccess");

    UtTest_Add(Test_FM_ChildDeleteDirectoryCmd_StrCmpThisDirectoryZero, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteDirectoryCmd_StrCmpThisDirectoryZero");

    UtTest_Add(Test_FM_ChildDeleteDirectoryCmd_StrCmpParentDirectoryZero, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteDirectoryCmd_StrCmpParentDirectoryZero");

    UtTest_Add(Test_FM_ChildDeleteDirectoryCmd_RemoveTheDirIsTrueRmDirSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteDirectoryCmd_RemoveTheDirIsTrueRmDirSuccess");

    UtTest_Add(Test_FM_ChildDeleteDirectoryCmd_RemoveDirTrueOSRmDirNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteDirectoryCmd_RemoveDirTrueOSRmDirNotSuccess");
}

void add_FM_ChildDirListFileCmd_tests(void)
{
    UtTest_Add(Test_FM_ChildDirListFileCmd_OSDirOpenNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDirListFileCmd_OSDirOpenNotSuccess");

    UtTest_Add(Test_FM_ChildDirListFileCmd_ChildDirListFileInitFalse, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDirListFileCmd_ChildDirListFileInitFalse");

    UtTest_Add(Test_FM_ChildDirListFileCmd_ChildDirListFileInitTrue, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDirListFileCmd_ChildDirListFileInitTrue");
}

void add_FM_ChildDirListPktCmd_tests(void)
{
    UtTest_Add(Test_FM_ChildDirListPktCmd_OSDirOpenNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDirListPktCmd_OSDirOpenNotSuccess");

    UtTest_Add(Test_FM_ChildDirListPktCmd_OSDirReadNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDirListPktCmd_OSDirReadNotSuccess");

    UtTest_Add(Test_FM_ChildDirListPktCmd_DirEntryNameThisDirectory, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDirListPktCmd_DirEntryNameThisDirectory");

    UtTest_Add(Test_FM_ChildDirListPktCmd_DirEntryNameParentDirectory, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDirListPktCmd_DirEntryNameParentDirectory");

    UtTest_Add(Test_FM_ChildDirListPktCmd_DirListOffsetNotExceeded, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDirListPktCmd_DirListOffsetNotExceeded");

    UtTest_Add(Test_FM_ChildDirListPktCmd_DirListOffsetExceeded, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDirListPktCmd_DirListOffsetExceeded");

    UtTest_Add(Test_FM_ChildDirListPktCmd_PathAndEntryLengthGreaterMaxPathLength, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDirListPktCmd_PathAndEntryLengthGreaterMaxPathLength");
}

void add_FM_ChildSetPermissionsCmd_tests(void)
{
    UtTest_Add(Test_FM_ChildSetPermissionsCmd_OSChmodNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildSetPermissionsCmd_OSChmodNotSuccess");

    UtTest_Add(Test_FM_ChildSetPermissionsCmd_OSChmodSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildSetPermissionsCmd_OSChmodSuccess");
}

void add_FM_ChildDirListFileInit_tests(void)
{
    UtTest_Add(Test_FM_ChildDirListFileInit_OSOpenCreateFail, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDirListFileInit_OSOpenCreateFail");

    UtTest_Add(Test_FM_ChildDirListFileInit_FSWriteHeaderNotSameSizeFSHeadert, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDirListFileInit_FSWriteHeaderNotSameSizeFSHeadert");

    UtTest_Add(Test_FM_ChildDirListFileInit_OSWriteNotSameSizeDirListFileStatst, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDirListFileInit_OSWriteNotSameSizeDirListFileStatst");

    UtTest_Add(Test_FM_ChildDirListFileInit_OSWriteSameSizeDirListFileStatst, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDirListFileInit_OSWriteSameSizeDirListFileStatst");
}

void add_FM_ChildDirListFileLoop_tests(void)
{
    UtTest_Add(Test_FM_ChildDirListFileLoop_OSDirReadNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDirListFileLoop_OSDirReadNotSuccess");

    UtTest_Add(Test_FM_ChildDirListFileLoop_OSDirEntryNameIsThisDirectory, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDirListFileLoop_OSDirEntryNameIsThisDirectory");

    UtTest_Add(Test_FM_ChildDirListFileLoop_OSDirEntryNameIsParentDirectory, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDirListFileLoop_OSDirEntryNameIsParentDirectory");

    UtTest_Add(Test_FM_ChildDirListFileLoop_PathLengthAndEntryLengthGreaterMaxPathLen, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDirListFileLoop_PathLengthAndEntryLengthGreaterMaxPathLen");

    UtTest_Add(Test_FM_ChildDirListFileLoop_FileEntriesGreaterFMDirListFileEntries, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDirListFileLoop_FileEntriesGreaterFMDirListFileEntries");

    UtTest_Add(Test_FM_ChildDirListFileLoop_BytesWrittenNotEqualWriteLength, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDirListFileLoop_BytesWrittenNotEqualWriteLength");

    UtTest_Add(Test_FM_ChildDirListFileLoop_BytesWrittenNotEqualWriteLengthInLoop, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDirListFileLoop_BytesWrittenNotEqualWriteLengthInLoop");
}

void add_FM_ChildSizeTimeMode_tests(void)
{
    UtTest_Add(Test_FM_ChildSizeTimeMode_OsStatNoSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildSizeTimeMode_OsStatNoSuccess");

    UtTest_Add(Test_FM_ChildSizeTimeMode_OSFilestateTimeDefined, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildSizeTimeMode_OSFilestateTimeDefined");
}

void add_FM_ChildSleepStat_tests(void)
{
    UtTest_Add(Test_FM_ChildSleepStat_getSizeTimeModeFalse, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildSleepStat_getSizeTimeModeFalse");

    UtTest_Add(Test_FM_ChildSleepStat_FilesTillSleepPositive, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildSleepStat_FilesTillSleepPositive");

    UtTest_Add(Test_FM_ChildSleepStat_FilesTillSleepLTEQZero, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildSleepStat_FilesTillSleepLTEQZero");
}

void add_FM_ChildLoop_tests(void)
{
    UtTest_Add(Test_FM_ChildLoop_CountSemTakeNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildLoop_CountSemTakeNotSuccess");

    UtTest_Add(Test_FM_ChildLoop_ChildQCountEqualZero, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildLoop_ChildQCountEqualZero");

    UtTest_Add(Test_FM_ChildLoop_ChildReadIndexEqualChildQDepth, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildLoop_ChildReadIndexEqualChildQDepth");

    UtTest_Add(Test_FM_ChildLoop_CountSemTakeSuccessDefault, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildLoop_CountSemTakeSuccessDefault");
}

/*
    UtTest_Add(,
        FM_Test_Setup, FM_Test_Teardown,
        "");
*/

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    add_FM_ChildInit_tests();
    add_FM_ChildTask_tests();
    add_FM_ChildProcess_tests();
    add_FM_ChildCopyCmd_tests();
    add_FM_ChildMoveCmd_tests();
    add_FM_ChildRenameCmd_tests();
    add_FM_ChildDeleteCmd_tests();
    add_FM_ChildDeleteAllFilesCmd_tests();
    add_FM_ChildDecompressFileCmd_tests();
    add_FM_ChildConcatFilesCmd_tests();
    add_FM_ChildFileInfoCmd_tests();
    add_FM_ChildCreateDirectoryCmd_tests();
    add_FM_ChildDeleteDirectoryCmd_tests();
    add_FM_ChildDirListFileCmd_tests();
    add_FM_ChildDirListPktCmd_tests();
    add_FM_ChildSetPermissionsCmd_tests();
    add_FM_ChildDirListFileInit_tests();
    add_FM_ChildDirListFileLoop_tests();
    add_FM_ChildSizeTimeMode_tests();
    add_FM_ChildSleepStat_tests();
    add_FM_ChildLoop_tests();
}
