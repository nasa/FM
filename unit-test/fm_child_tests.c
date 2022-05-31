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
#ifdef FM_INCLUDE_DECOMPRESS
#include "cfs_fs_lib.h"
#endif

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

/*
**********************************************************************************
**          TEST CASE FUNCTIONS
**********************************************************************************
*/
/* ****************
 * Init Tests
 * ***************/
void Test_FM_ChildInit_CountSemCreateNotSuccess(void)
{
    // Arrange
    UT_SetDefaultReturnValue(UT_KEY(OS_CountSemCreate), !CFE_SUCCESS);

    // Act
    int32 init_status = FM_ChildInit();

    uint8 count_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    // Assert
    UtAssert_True(init_status != CFE_SUCCESS, "The returned value is %i and should be %i", init_status, !CFE_SUCCESS);
    UtAssert_True(count_SendEvent == 1, "SendEvent was called %u time(s) and should be 1", count_SendEvent);
    /* Need to figure out how to get the arguments from SendEvent compaire those to taskeid expected */
}

void Test_FM_ChildInit_MutSemCreateNotSuccess(void)
{
    // Arrange
    UT_SetDefaultReturnValue(UT_KEY(OS_CountSemCreate), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_MutSemCreate), !CFE_SUCCESS);

    // Act
    int32 init_status = FM_ChildInit();

    uint8 count_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    // Assert
    UtAssert_True(init_status != CFE_SUCCESS, "The returned value is %i and should be %i", init_status, !CFE_SUCCESS);
    UtAssert_True(count_SendEvent == 1, "SendEvent was called %u time(s) and should be 1", count_SendEvent);
    /* TaskEID should be FM_CHILD_INIT_QSEM_ERR_EID */
}

void Test_FM_ChildInit_MuteSemCreateSuccess_CreateChildTaskNotSuccess(void)
{
    // Arrange
    UT_SetDefaultReturnValue(UT_KEY(OS_CountSemCreate), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_MutSemCreate), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_CreateChildTask), !CFE_SUCCESS);

    // Act
    int32 init_status = FM_ChildInit();

    uint8 count_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    // Assert
    UtAssert_True(init_status != CFE_SUCCESS, "The returned value is %i and should be %i", init_status, !CFE_SUCCESS);
    UtAssert_True(count_SendEvent == 1, "SendEvent was called %u time(s) and should be 1", count_SendEvent);
    /* TaskEID should be FM_CHILD_INIT_CREATE_ERR_EID */
}

void Test_FM_ChildInit_ReturnSuccess(void)
{
    // Arrange
    UT_SetDefaultReturnValue(UT_KEY(OS_CountSemCreate), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_MutSemCreate), CFE_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_CreateChildTask), CFE_SUCCESS);

    // Act
    int32 init_status = FM_ChildInit();

    uint8 count_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    // Assert
    UtAssert_True(init_status == CFE_SUCCESS, "The returned value is %i and should be %i", init_status, CFE_SUCCESS);
    UtAssert_True(count_SendEvent == 0, "SendEvent was called %u time(s) and should be 0", count_SendEvent);
}

/* ****************
 * ChildTask Tests
 * ***************/
void Test_FM_ChildTask_ChildLoopCalled(void)
{
    // Arrange
    FM_GlobalData.ChildSemaphore = FM_UT_OBJID_1;

    UT_SetDefaultReturnValue(UT_KEY(OS_CountSemTake), !CFE_SUCCESS);

    // Act
    FM_ChildTask();

    uint8 count_SendEvent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_ExitTask  = UT_GetStubCount(UT_KEY(CFE_ES_ExitChildTask));

    // Assert
    // SendEvent called once in ChildTask and should be called once in ChildLoop
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CHILD_INIT_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_CHILD_TERM_SEM_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(count_ExitTask, 1);
    UtAssert_INT32_EQ(count_SendEvent, 2);
}

/* ****************
 * ChildProcess Tests
 * ***************/
void Test_FM_ChildProcess_ChildReadIndexGreaterChildQDepth(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC                          = {.CommandCode = -1}; // Default switch statement
    FM_GlobalData.ChildReadIndex                           = FM_CHILD_QUEUE_DEPTH - 1;
    FM_GlobalData.ChildQueue[FM_GlobalData.ChildReadIndex] = dummy_CC;

    uint8 childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;

    // Act
    FM_ChildProcess();

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    // Assert
    UtAssert_INT32_EQ(childcmderrcounter_before + 1, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(FM_GlobalData.ChildReadIndex, 0);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CHILD_EXE_ERR_EID);
}

void Test_FM_ChildProcess_FMCopyCC(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {.CommandCode = FM_COPY_CC};
    FM_GlobalData.ChildQueue[0]   = dummy_CC;
    FM_GlobalData.ChildReadIndex  = 0;

    UT_SetDefaultReturnValue(UT_KEY(OS_cp), OS_SUCCESS);

    uint8 childcmdcounter_before = FM_GlobalData.ChildCmdCounter;

    // Act
    FM_ChildProcess();

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_oscp      = UT_GetStubCount(UT_KEY(OS_cp));

    // Assert
    UtAssert_INT32_EQ(childcmdcounter_before + 1, FM_GlobalData.ChildCmdCounter);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(count_oscp, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_COPY_CMD_EID);
}

void Test_FM_ChildProcess_FMMoveCC(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {.CommandCode = FM_MOVE_CC};
    FM_GlobalData.ChildQueue[0]   = dummy_CC;
    FM_GlobalData.ChildReadIndex  = 0;

    UT_SetDefaultReturnValue(UT_KEY(OS_mv), OS_SUCCESS);

    uint8 childcmdcounter_before = FM_GlobalData.ChildCmdCounter;

    // Act
    FM_ChildProcess();

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osmv      = UT_GetStubCount(UT_KEY(OS_mv));

    // Assert
    UtAssert_INT32_EQ(count_osmv, 1);
    UtAssert_INT32_EQ(childcmdcounter_before + 1, FM_GlobalData.ChildCmdCounter);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_MOVE_CMD_EID);
}

void Test_FM_ChildProcess_FMRenameCC(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {.CommandCode = FM_RENAME_CC};
    FM_GlobalData.ChildQueue[0]   = dummy_CC;
    FM_GlobalData.ChildReadIndex  = 0;

    UT_SetDefaultReturnValue(UT_KEY(OS_rename), OS_SUCCESS);

    uint8 childcmdcounter_before = FM_GlobalData.ChildCmdCounter;

    // Act
    FM_ChildProcess();

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osrename  = UT_GetStubCount(UT_KEY(OS_rename));

    // Assert
    UtAssert_INT32_EQ(count_osrename, 1);
    UtAssert_INT32_EQ(childcmdcounter_before + 1, FM_GlobalData.ChildCmdCounter);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_RENAME_CMD_EID);
}

void Test_FM_ChildProcess_FMDeleteCC(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {.CommandCode = FM_DELETE_CC};
    FM_GlobalData.ChildQueue[0]   = dummy_CC;
    FM_GlobalData.ChildReadIndex  = 0;

    UT_SetDefaultReturnValue(UT_KEY(OS_remove), OS_SUCCESS);

    uint8 childcmdcounter_before = FM_GlobalData.ChildCmdCounter;

    // Act
    FM_ChildProcess();

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osremove  = UT_GetStubCount(UT_KEY(OS_remove));

    // Assert
    UtAssert_INT32_EQ(count_osremove, 1);
    UtAssert_INT32_EQ(childcmdcounter_before + 1, FM_GlobalData.ChildCmdCounter);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_CMD_EID);
}

void Test_FM_ChildProcess_FMDeleteAllCC(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {.CommandCode = FM_DELETE_ALL_CC};
    FM_GlobalData.ChildQueue[0]   = dummy_CC;
    FM_GlobalData.ChildReadIndex  = 0;

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), !OS_SUCCESS);

    uint8 childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;

    // Act
    FM_ChildProcess();

    uint8 count_sendevent  = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdiropen  = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));
    uint8 count_osdirclose = UT_GetStubCount(UT_KEY(OS_DirectoryClose));

    // Assert
    UtAssert_INT32_EQ(count_osdiropen, 1);
    UtAssert_INT32_EQ(count_osdirclose, 0);
    UtAssert_INT32_EQ(childcmderrcounter_before + 1, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_ALL_OS_ERR_EID);
}

#ifdef FM_INCLUDE_DECOMPRESS
void Test_FM_ChildProcess_FMDecompressCC(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {.CommandCode = FM_DECOMPRESS_CC};
    FM_GlobalData.ChildQueue[0]   = dummy_CC;
    FM_GlobalData.ChildReadIndex  = 0;

    UT_SetDefaultReturnValue(UT_KEY(FS_LIB_Decompress), !CFE_SUCCESS);

    uint8 childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;

    // Act
    FM_ChildProcess();

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_fslibdec  = UT_GetStubCount(UT_KEY(FS_LIB_Decompress));

    // Assert
    UtAssert_INT32_EQ(count_fslibdec, 1);
    UtAssert_INT32_EQ(childcmderrcounter_before + 1, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DECOM_CFE_ERR_EID);
}
#endif

void Test_FM_ChildProcess_FMConcatCC(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {.CommandCode = FM_CONCAT_CC};
    FM_GlobalData.ChildQueue[0]   = dummy_CC;
    FM_GlobalData.ChildReadIndex  = 0;

    UT_SetDefaultReturnValue(UT_KEY(OS_cp), !OS_SUCCESS);

    uint8 childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;

    // Act
    FM_ChildProcess();

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_oscp      = UT_GetStubCount(UT_KEY(OS_cp));

    // Assert
    UtAssert_INT32_EQ(count_oscp, 1);
    UtAssert_INT32_EQ(childcmderrcounter_before + 1, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CONCAT_OSCPY_ERR_EID);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
}

void Test_FM_ChildProcess_FMCreateDirCC(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {.CommandCode = FM_CREATE_DIR_CC};
    FM_GlobalData.ChildQueue[0]   = dummy_CC;
    FM_GlobalData.ChildReadIndex  = 0;
    FM_GlobalData.ChildCurrentCC  = UT_Utils_Any_uint8();

    UT_SetDefaultReturnValue(UT_KEY(OS_mkdir), !OS_SUCCESS);

    uint8 childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;

    // Act
    FM_ChildProcess();

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_OSmkdir   = UT_GetStubCount(UT_KEY(OS_mkdir));

    // Assert
    UtAssert_INT32_EQ(count_OSmkdir, 1);
    UtAssert_INT32_EQ(childcmderrcounter_before + 1, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CREATE_DIR_OS_ERR_EID);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
}

void Test_FM_ChildProcess_FMDeleteDirCC(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {.CommandCode = FM_DELETE_DIR_CC};
    FM_GlobalData.ChildQueue[0]   = dummy_CC;
    FM_GlobalData.ChildReadIndex  = 0;
    FM_GlobalData.ChildCurrentCC  = UT_Utils_Any_uint8();

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), !OS_SUCCESS);

    uint8 childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;

    // Act
    FM_ChildProcess();

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_diropen   = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));

    // Assert
    UtAssert_INT32_EQ(count_diropen, 1);
    UtAssert_INT32_EQ(childcmderrcounter_before + 1, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_OPENDIR_OS_ERR_EID);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
}

void Test_FM_ChildProcess_FMGetFileInfoCC(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {
        .CommandCode = FM_GET_FILE_INFO_CC, .FileInfoCRC = !FM_IGNORE_CRC, .FileInfoState = FM_NAME_IS_FILE_OPEN};
    FM_GlobalData.ChildQueue[0]       = dummy_CC;
    FM_GlobalData.ChildReadIndex      = 0;
    FM_GlobalData.ChildCurrentCC      = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdCounter     = UT_Utils_Any_uint8();

    UT_SetDefaultReturnValue(UT_KEY(CFE_MSG_Init), CFE_SUCCESS);

    uint8 childcmdcounter_before     = FM_GlobalData.ChildCmdCounter;
    uint8 childcmdwarncounter_before = FM_GlobalData.ChildCmdWarnCounter;

    // Act
    FM_ChildProcess();

    uint8 count_sendevent   = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_msginit     = UT_GetStubCount(UT_KEY(CFE_MSG_Init));
    uint8 count_transmitmsg = UT_GetStubCount(UT_KEY(CFE_SB_TransmitMsg));

    // Assert
    UtAssert_INT32_EQ(count_msginit, 1);
    UtAssert_INT32_EQ(count_transmitmsg, 1);
    UtAssert_INT32_EQ(childcmdcounter_before + 1, FM_GlobalData.ChildCmdCounter);
    UtAssert_INT32_EQ(childcmdwarncounter_before + 1, FM_GlobalData.ChildCmdWarnCounter);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
    UtAssert_INT32_EQ(count_sendevent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_FILE_INFO_STATE_WARNING_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_GET_FILE_INFO_CMD_EID);
}

void Test_FM_ChildProcess_FMGetDirListsFileCC(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {.CommandCode = FM_GET_DIR_FILE_CC};
    FM_GlobalData.ChildQueue[0]   = dummy_CC;
    FM_GlobalData.ChildReadIndex  = 0;
    FM_GlobalData.ChildCurrentCC  = UT_Utils_Any_uint8();

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), !OS_SUCCESS);

    uint8 childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;

    // Act
    FM_ChildProcess();

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_diropen   = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));

    // Assert
    UtAssert_INT32_EQ(count_diropen, 1);
    UtAssert_INT32_EQ(childcmderrcounter_before + 1, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_FILE_OSOPENDIR_ERR_EID);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
}

void Test_FM_ChildProcess_FMGetDirListsPktCC(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {.CommandCode = FM_GET_DIR_PKT_CC};
    FM_GlobalData.ChildQueue[0]   = dummy_CC;
    FM_GlobalData.ChildReadIndex  = 0;
    FM_GlobalData.ChildCurrentCC  = UT_Utils_Any_uint8();

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), !OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_MSG_Init), CFE_SUCCESS);

    uint8 childcmdcounter_before = FM_GlobalData.ChildCmdCounter;

    // Act
    FM_ChildProcess();

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_diropen   = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));
    uint8 count_dirread   = UT_GetStubCount(UT_KEY(OS_DirectoryRead));
    uint8 count_cfemsg    = UT_GetStubCount(UT_KEY(CFE_MSG_Init));
    uint8 count_dirclose  = UT_GetStubCount(UT_KEY(OS_DirectoryClose));

    // Assert
    UtAssert_INT32_EQ(count_diropen, 1);
    UtAssert_INT32_EQ(count_dirread, 1);
    UtAssert_INT32_EQ(count_cfemsg, 1);
    UtAssert_INT32_EQ(count_dirclose, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_PKT_CMD_EID);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
    UtAssert_INT32_EQ(childcmdcounter_before + 1, FM_GlobalData.ChildCmdCounter);
}

void Test_FM_ChildProcess_FMDeleteIntCC(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC    = {.CommandCode = FM_DELETE_INT_CC};
    FM_GlobalData.ChildQueue[0]      = dummy_CC;
    FM_GlobalData.ChildReadIndex     = 0;
    FM_GlobalData.ChildCurrentCC     = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();

    UT_SetDefaultReturnValue(UT_KEY(OS_remove), !OS_SUCCESS);

    uint8 childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;

    // Act
    FM_ChildProcess();

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_dirremove = UT_GetStubCount(UT_KEY(OS_remove));

    // Assert
    UtAssert_INT32_EQ(count_dirremove, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_OS_ERR_EID);
    UtAssert_INT32_EQ(childcmderrcounter_before + 1, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
}

void Test_FM_ChildProcess_FMSetFilePermCC(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC    = {.CommandCode = FM_SET_FILE_PERM_CC};
    FM_GlobalData.ChildQueue[0]      = dummy_CC;
    FM_GlobalData.ChildReadIndex     = 0;
    FM_GlobalData.ChildCurrentCC     = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();

    UT_SetDefaultReturnValue(UT_KEY(OS_chmod), !OS_SUCCESS);

    uint8 childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;

    // Act
    FM_ChildProcess();

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_chmod     = UT_GetStubCount(UT_KEY(OS_chmod));

    // Assert
    UtAssert_INT32_EQ(count_chmod, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_SET_PERM_OS_ERR_EID);
    UtAssert_INT32_EQ(childcmderrcounter_before + 1, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
}

void Test_FM_ChildProcess_DefaultSwitch(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC    = {.CommandCode = -1};
    FM_GlobalData.ChildQueue[0]      = dummy_CC;
    FM_GlobalData.ChildReadIndex     = 0;
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();

    uint8 childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;

    // Act
    FM_ChildProcess();

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    // Assert
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CHILD_EXE_ERR_EID);
    UtAssert_INT32_EQ(childcmderrcounter_before + 1, FM_GlobalData.ChildCmdErrCounter);
}

/* ****************
 * ChildCopyCmd Tests
 * ***************/
void Test_FM_ChildCopyCmd_OScpIsSuccess(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {.CommandCode = 999};
    FM_GlobalData.ChildReadIndex  = 0;
    FM_GlobalData.ChildQueue[0]   = dummy_CC;

    UT_SetDefaultReturnValue(UT_KEY(OS_cp), OS_SUCCESS);
    uint8 count_childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;

    // Act
    FM_ChildCopyCmd(&dummy_CC);

    uint8 count_childcmdcounter_after    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_after = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_SendEvent                = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    // Assert
    UtAssert_True(count_childcmdcounter_after == count_childcmdcounter_before + 1,
                  "The ChildCmdCounter after is %u and it should be %u", count_childcmdcounter_after,
                  count_childcmdcounter_before + 1);
    UtAssert_True(count_childcmderrcounter_after == count_childcmderrcounter_before,
                  "The ChildCmdErrCounter after is %u and it should be %u", count_childcmderrcounter_after,
                  count_childcmderrcounter_before);
    UtAssert_True(count_SendEvent == 1, "SendEvent was called %u time(s) and should be 1", count_SendEvent);
}

void Test_FM_ChildCopyCmd_OScpNotSuccess(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {.CommandCode = 999};
    FM_GlobalData.ChildReadIndex  = 0;
    FM_GlobalData.ChildQueue[0]   = dummy_CC;

    UT_SetDefaultReturnValue(UT_KEY(OS_cp), !OS_SUCCESS);
    uint8 count_childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;

    // Act
    FM_ChildCopyCmd(&dummy_CC);

    uint8 count_childcmdcounter_after    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_after = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_SendEvent                = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));

    // Assert
    UtAssert_True(count_childcmdcounter_after == count_childcmdcounter_before,
                  "The ChildCmdCounter after is %u and it should be %u", count_childcmdcounter_after,
                  count_childcmdcounter_before);
    UtAssert_True(count_childcmderrcounter_after == count_childcmderrcounter_before + 1,
                  "The ChildCmdErrCounter after is %u and it should be %u", count_childcmderrcounter_after,
                  count_childcmderrcounter_before + 1);
    UtAssert_True(count_SendEvent == 1, "SendEvent was called %u time(s) and should be 1", count_SendEvent);
}

/* ****************
 * ChildMoveCmd Tests
 * ***************/
void Test_FM_ChildMoveCmd_OSmvNotSuccess(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC    = {.CommandCode = UT_Utils_Any_uint8()};
    FM_GlobalData.ChildCmdCounter    = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_mv), !OS_SUCCESS);

    uint8 count_childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 commandcode_before              = dummy_CC.CommandCode;

    // Act
    FM_ChildMoveCmd(&dummy_CC);

    uint8 count_childcmdcounter_after    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_after = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_sendevent                = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osmv                     = UT_GetStubCount(UT_KEY(OS_mv));

    // Assert
    UtAssert_INT32_EQ(count_osmv, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_MOVE_OS_ERR_EID);
    UtAssert_INT32_EQ(count_childcmderrcounter_before + 1, count_childcmderrcounter_after);
    UtAssert_INT32_EQ(count_childcmdcounter_before, count_childcmdcounter_after);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, commandcode_before);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
}

void Test_FM_ChildMoveCmd_OSmvSuccess(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC    = {.CommandCode = UT_Utils_Any_uint8()};
    FM_GlobalData.ChildCmdCounter    = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_mv), OS_SUCCESS);

    uint8 count_childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 commandcode_before              = dummy_CC.CommandCode;

    // Act
    FM_ChildMoveCmd(&dummy_CC);

    uint8 count_childcmdcounter_after    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_after = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_sendevent                = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osmv                     = UT_GetStubCount(UT_KEY(OS_mv));

    // Assert
    UtAssert_INT32_EQ(count_osmv, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_MOVE_CMD_EID);
    UtAssert_INT32_EQ(count_childcmderrcounter_before, count_childcmderrcounter_after);
    UtAssert_INT32_EQ(count_childcmdcounter_before + 1, count_childcmdcounter_after);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, commandcode_before);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
}

/* ****************
 * ChildRenameCmd Tests
 * ***************/
void Test_FM_ChildRenameCmd_OSRenameSuccess(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC    = {.CommandCode = UT_Utils_Any_uint8()};
    FM_GlobalData.ChildCmdCounter    = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_rename), OS_SUCCESS);

    uint8 count_childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 commandcode_before              = dummy_CC.CommandCode;

    // Act
    FM_ChildRenameCmd(&dummy_CC);

    uint8 count_childcmdcounter_after    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_after = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_sendevent                = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osrename                 = UT_GetStubCount(UT_KEY(OS_rename));

    // Assert
    UtAssert_INT32_EQ(count_osrename, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_RENAME_CMD_EID);
    UtAssert_INT32_EQ(count_childcmderrcounter_before, count_childcmderrcounter_after);
    UtAssert_INT32_EQ(count_childcmdcounter_before + 1, count_childcmdcounter_after);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, commandcode_before);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
}

void Test_FM_ChildRenameCmd_OSRenameNotSuccess(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC    = {.CommandCode = UT_Utils_Any_uint8()};
    FM_GlobalData.ChildCmdCounter    = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_rename), !OS_SUCCESS);

    uint8 count_childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 commandcode_before              = dummy_CC.CommandCode;

    // Act
    FM_ChildRenameCmd(&dummy_CC);

    uint8 count_childcmdcounter_after    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_after = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_sendevent                = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osrename                 = UT_GetStubCount(UT_KEY(OS_rename));

    // Assert
    UtAssert_INT32_EQ(count_osrename, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_RENAME_OS_ERR_EID);
    UtAssert_INT32_EQ(count_childcmderrcounter_before + 1, count_childcmderrcounter_after);
    UtAssert_INT32_EQ(count_childcmdcounter_before, count_childcmdcounter_after);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, commandcode_before);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
}

/* ****************
 * ChildDeleteCmd Tests
 * ***************/
void Test_FM_ChildDeleteCmd_OSRenameSuccess(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC    = {.CommandCode = FM_DELETE_INT_CC + 1};
    FM_GlobalData.ChildCmdCounter    = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_remove), OS_SUCCESS);

    uint8 count_childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 commandcode_before              = dummy_CC.CommandCode;

    // Act
    FM_ChildDeleteCmd(&dummy_CC);

    uint8 count_childcmdcounter_after    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_after = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_sendevent                = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osremove                 = UT_GetStubCount(UT_KEY(OS_remove));

    // Assert
    UtAssert_INT32_EQ(count_osremove, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_CMD_EID);
    UtAssert_INT32_EQ(count_childcmderrcounter_before, count_childcmderrcounter_after);
    UtAssert_INT32_EQ(count_childcmdcounter_before + 1, count_childcmdcounter_after);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, commandcode_before);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
}

void Test_FM_ChildDeleteCmd_OSRenameNotSuccess(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC    = {.CommandCode = UT_Utils_Any_uint8()};
    FM_GlobalData.ChildCmdCounter    = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_remove), !OS_SUCCESS);

    uint8 count_childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 commandcode_before              = dummy_CC.CommandCode;

    // Act
    FM_ChildDeleteCmd(&dummy_CC);

    uint8 count_childcmdcounter_after    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_after = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_sendevent                = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osremove                 = UT_GetStubCount(UT_KEY(OS_remove));

    // Assert
    UtAssert_INT32_EQ(count_osremove, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_OS_ERR_EID);
    UtAssert_INT32_EQ(count_childcmderrcounter_before + 1, count_childcmderrcounter_after);
    UtAssert_INT32_EQ(count_childcmdcounter_before, count_childcmdcounter_after);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, commandcode_before);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
}

void Test_FM_ChildDeleteCmd_CommandCodeIsDeleteIntCC(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC    = {.CommandCode = FM_DELETE_INT_CC};
    FM_GlobalData.ChildCmdCounter    = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_remove), OS_SUCCESS);

    uint8 count_childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 commandcode_before              = dummy_CC.CommandCode;

    // Act
    FM_ChildDeleteCmd(&dummy_CC);

    uint8 count_childcmdcounter_after    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_after = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_sendevent                = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osremove                 = UT_GetStubCount(UT_KEY(OS_remove));

    // Assert
    UtAssert_INT32_EQ(count_osremove, 1);
    UtAssert_INT32_EQ(count_sendevent, 0);
    UtAssert_INT32_EQ(count_childcmderrcounter_before, count_childcmderrcounter_after);
    UtAssert_INT32_EQ(count_childcmdcounter_before + 1, count_childcmdcounter_after);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, commandcode_before);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
}

/* ****************
 * ChildDeleteAllCmd Tests
 * ***************/
void Test_FM_ChildDeleteAllCmd_DirOpenNotSuccess(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {
        .CommandCode = FM_DELETE_ALL_CC, .Source1 = "dummy_source1", .Source2 = "dummy_source2"};
    FM_GlobalData.ChildQueue[0]      = dummy_CC;
    FM_GlobalData.ChildReadIndex     = 0;
    FM_GlobalData.ChildPreviousCC    = -1;
    FM_GlobalData.ChildCurrentCC     = -1;
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), !OS_SUCCESS);

    uint8 childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;

    // Act
    FM_ChildDeleteAllCmd(&dummy_CC);

    uint8 count_sendevent  = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdiropen  = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));
    uint8 count_osdirclose = UT_GetStubCount(UT_KEY(OS_DirectoryClose));

    // Assert
    UtAssert_INT32_EQ(count_osdiropen, 1);
    UtAssert_INT32_EQ(count_osdirclose, 0);
    UtAssert_INT32_EQ(childcmderrcounter_before + 1, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_ALL_OS_ERR_EID);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, FM_DELETE_ALL_CC);
}

void Test_FM_ChildDeleteAllCmd_DirReadNotSuccess(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {
        .CommandCode = FM_DELETE_ALL_CC, .Source1 = "dummy_source1", .Source2 = "dummy_source2"};
    FM_GlobalData.ChildQueue[0]       = dummy_CC;
    FM_GlobalData.ChildReadIndex      = 0;
    FM_GlobalData.ChildPreviousCC     = -1;
    FM_GlobalData.ChildCurrentCC      = -1;
    FM_GlobalData.ChildCmdErrCounter  = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8();

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), !OS_SUCCESS);

    uint8 childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 childcmdwarncount_before  = FM_GlobalData.ChildCmdWarnCounter;

    // Act
    FM_ChildDeleteAllCmd(&dummy_CC);

    uint8 count_sendevent  = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdiropen  = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));
    uint8 count_osdirclose = UT_GetStubCount(UT_KEY(OS_DirectoryClose));

    // Assert
    UtAssert_INT32_EQ(count_osdiropen, 1);
    UtAssert_INT32_EQ(count_osdirclose, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_ALL_CMD_EID);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, FM_DELETE_ALL_CC);
    UtAssert_INT32_EQ(childcmderrcounter_before, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(childcmdcounter_before + 1, FM_GlobalData.ChildCmdCounter);
    UtAssert_INT32_EQ(childcmdwarncount_before, FM_GlobalData.ChildCmdWarnCounter);
}

void Test_FM_ChildDeleteAllCmd_DirEntryThisDirectory(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {
        .CommandCode = FM_DELETE_ALL_CC, .Source1 = "dummy_source1", .Source2 = "dummy_source2"};
    FM_GlobalData.ChildQueue[0]       = dummy_CC;
    FM_GlobalData.ChildReadIndex      = 0;
    FM_GlobalData.ChildPreviousCC     = -1;
    FM_GlobalData.ChildCurrentCC      = -1;
    FM_GlobalData.ChildCmdErrCounter  = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8();

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), OS_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);
    os_dirent_t dummy_direntry = {.FileName = "."};
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &dummy_direntry, sizeof(dummy_direntry), false);

    uint8 childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 childcmdwarncount_before  = FM_GlobalData.ChildCmdWarnCounter;

    // Act
    FM_ChildDeleteAllCmd(&dummy_CC);

    uint8 count_sendevent  = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdiropen  = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));
    uint8 count_osdirclose = UT_GetStubCount(UT_KEY(OS_DirectoryClose));
    uint8 count_osdirread  = UT_GetStubCount(UT_KEY(OS_DirectoryRead));
    uint8 count_fmgetstate = UT_GetStubCount(UT_KEY(FM_GetFilenameState));

    // Assert
    UtAssert_INT32_EQ(count_osdiropen, 1);
    UtAssert_INT32_EQ(count_osdirclose, 1);
    UtAssert_INT32_EQ(count_osdirread, 2);
    UtAssert_INT32_EQ(count_fmgetstate, 0);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_ALL_CMD_EID);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, FM_DELETE_ALL_CC);
    UtAssert_INT32_EQ(childcmderrcounter_before, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(childcmdcounter_before + 1, FM_GlobalData.ChildCmdCounter);
    UtAssert_INT32_EQ(childcmdwarncount_before, FM_GlobalData.ChildCmdWarnCounter);
}

void Test_FM_ChildDeleteAllCmd_DirEntryParentDirectory(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {
        .CommandCode = FM_DELETE_ALL_CC, .Source1 = "dummy_source1", .Source2 = "dummy_source2"};
    FM_GlobalData.ChildQueue[0]       = dummy_CC;
    FM_GlobalData.ChildReadIndex      = 0;
    FM_GlobalData.ChildPreviousCC     = -1;
    FM_GlobalData.ChildCurrentCC      = -1;
    FM_GlobalData.ChildCmdErrCounter  = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8();

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), OS_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);
    os_dirent_t dummy_direntry = {.FileName = ".."};
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &dummy_direntry, sizeof(dummy_direntry), false);

    uint8 childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 childcmdwarncount_before  = FM_GlobalData.ChildCmdWarnCounter;

    // Act
    FM_ChildDeleteAllCmd(&dummy_CC);

    uint8 count_sendevent  = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdiropen  = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));
    uint8 count_osdirclose = UT_GetStubCount(UT_KEY(OS_DirectoryClose));
    uint8 count_osdirread  = UT_GetStubCount(UT_KEY(OS_DirectoryRead));
    uint8 count_fmgetstate = UT_GetStubCount(UT_KEY(FM_GetFilenameState));

    // Assert
    UtAssert_INT32_EQ(count_osdiropen, 1);
    UtAssert_INT32_EQ(count_osdirclose, 1);
    UtAssert_INT32_EQ(count_osdirread, 2);
    UtAssert_INT32_EQ(count_fmgetstate, 0);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_ALL_CMD_EID);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, FM_DELETE_ALL_CC);
    UtAssert_INT32_EQ(childcmderrcounter_before, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(childcmdcounter_before + 1, FM_GlobalData.ChildCmdCounter);
    UtAssert_INT32_EQ(childcmdwarncount_before, FM_GlobalData.ChildCmdWarnCounter);
}

void Test_FM_ChildDeleteAllCmd_PathFilenameLengthGreaterMaxPthLen(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC     = {.CommandCode = FM_DELETE_ALL_CC,
                                     .Source1     = "dummy_source1",
                                     .Source2     = "dummy_source2HasAReallyLongNameSomeSayTheNameIs42Characters"};
    FM_GlobalData.ChildQueue[0]       = dummy_CC;
    FM_GlobalData.ChildReadIndex      = 0;
    FM_GlobalData.ChildPreviousCC     = -1;
    FM_GlobalData.ChildCurrentCC      = -1;
    FM_GlobalData.ChildCmdErrCounter  = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8();

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), OS_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);
    os_dirent_t dummy_direntry = {.FileName = "ThisDirectory"};
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &dummy_direntry, sizeof(dummy_direntry), false);

    uint8 childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 childcmdwarncount_before  = FM_GlobalData.ChildCmdWarnCounter;

    // Act
    FM_ChildDeleteAllCmd(&dummy_CC);

    uint8 count_sendevent  = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdiropen  = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));
    uint8 count_osdirclose = UT_GetStubCount(UT_KEY(OS_DirectoryClose));
    uint8 count_osdirread  = UT_GetStubCount(UT_KEY(OS_DirectoryRead));
    uint8 count_fmgetstate = UT_GetStubCount(UT_KEY(FM_GetFilenameState));

    // Assert
    UtAssert_INT32_EQ(count_osdiropen, 1);
    UtAssert_INT32_EQ(count_osdirclose, 1);
    UtAssert_INT32_EQ(count_osdirread, 2);
    UtAssert_INT32_EQ(count_fmgetstate, 0);
    UtAssert_INT32_EQ(count_sendevent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_ALL_CMD_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_DELETE_ALL_FILES_ND_WARNING_EID);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, FM_DELETE_ALL_CC);
    UtAssert_INT32_EQ(childcmderrcounter_before, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(childcmdcounter_before + 1, FM_GlobalData.ChildCmdCounter);
    UtAssert_INT32_EQ(childcmdwarncount_before + 1, FM_GlobalData.ChildCmdWarnCounter);
}

void Test_FM_ChildDeleteAllCmd_InvalidFilenameState(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {
        .CommandCode = FM_DELETE_ALL_CC, .Source1 = "dummy_source1", .Source2 = "dummy_source2"};
    FM_GlobalData.ChildQueue[0]       = dummy_CC;
    FM_GlobalData.ChildReadIndex      = 0;
    FM_GlobalData.ChildPreviousCC     = -1;
    FM_GlobalData.ChildCurrentCC      = -1;
    FM_GlobalData.ChildCmdErrCounter  = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8();

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), OS_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);
    os_dirent_t dummy_direntry = {.FileName = "ThisDirectory"};
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &dummy_direntry, sizeof(dummy_direntry), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_GetFilenameState), FM_NAME_IS_INVALID);

    uint8 childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 childcmdwarncount_before  = FM_GlobalData.ChildCmdWarnCounter;

    // Act
    FM_ChildDeleteAllCmd(&dummy_CC);

    uint8 count_sendevent  = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdiropen  = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));
    uint8 count_osdirclose = UT_GetStubCount(UT_KEY(OS_DirectoryClose));
    uint8 count_osdirread  = UT_GetStubCount(UT_KEY(OS_DirectoryRead));
    uint8 count_fmgetstate = UT_GetStubCount(UT_KEY(FM_GetFilenameState));

    // Assert
    UtAssert_INT32_EQ(count_osdiropen, 1);
    UtAssert_INT32_EQ(count_osdirclose, 1);
    UtAssert_INT32_EQ(count_osdirread, 2);
    UtAssert_INT32_EQ(count_fmgetstate, 1);
    UtAssert_INT32_EQ(count_sendevent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_ALL_CMD_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_DELETE_ALL_FILES_ND_WARNING_EID);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, FM_DELETE_ALL_CC);
    UtAssert_INT32_EQ(childcmderrcounter_before, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(childcmdcounter_before + 1, FM_GlobalData.ChildCmdCounter);
    UtAssert_INT32_EQ(childcmdwarncount_before + 1, FM_GlobalData.ChildCmdWarnCounter);
}

void Test_FM_ChildDeleteAllCmd_NotInUseFilenameState(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {
        .CommandCode = FM_DELETE_ALL_CC, .Source1 = "dummy_source1", .Source2 = "dummy_source2"};
    FM_GlobalData.ChildQueue[0]       = dummy_CC;
    FM_GlobalData.ChildReadIndex      = 0;
    FM_GlobalData.ChildPreviousCC     = -1;
    FM_GlobalData.ChildCurrentCC      = -1;
    FM_GlobalData.ChildCmdErrCounter  = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8();

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), OS_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);
    os_dirent_t dummy_direntry = {.FileName = "ThisDirectory"};
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &dummy_direntry, sizeof(dummy_direntry), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_GetFilenameState), FM_NAME_IS_NOT_IN_USE);

    uint8 childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 childcmdwarncount_before  = FM_GlobalData.ChildCmdWarnCounter;

    // Act
    FM_ChildDeleteAllCmd(&dummy_CC);

    uint8 count_sendevent  = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdiropen  = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));
    uint8 count_osdirclose = UT_GetStubCount(UT_KEY(OS_DirectoryClose));
    uint8 count_osdirread  = UT_GetStubCount(UT_KEY(OS_DirectoryRead));
    uint8 count_fmgetstate = UT_GetStubCount(UT_KEY(FM_GetFilenameState));

    // Assert
    UtAssert_INT32_EQ(count_osdiropen, 1);
    UtAssert_INT32_EQ(count_osdirclose, 1);
    UtAssert_INT32_EQ(count_osdirread, 2);
    UtAssert_INT32_EQ(count_fmgetstate, 1);
    UtAssert_INT32_EQ(count_sendevent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_ALL_CMD_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_DELETE_ALL_FILES_ND_WARNING_EID);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, FM_DELETE_ALL_CC);
    UtAssert_INT32_EQ(childcmderrcounter_before, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(childcmdcounter_before + 1, FM_GlobalData.ChildCmdCounter);
    UtAssert_INT32_EQ(childcmdwarncount_before + 1, FM_GlobalData.ChildCmdWarnCounter);
}

void Test_FM_ChildDeleteAllCmd_DirectoryFilenameState(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {
        .CommandCode = FM_DELETE_ALL_CC, .Source1 = "dummy_source1", .Source2 = "dummy_source2"};
    FM_GlobalData.ChildQueue[0]       = dummy_CC;
    FM_GlobalData.ChildReadIndex      = 0;
    FM_GlobalData.ChildPreviousCC     = -1;
    FM_GlobalData.ChildCurrentCC      = -1;
    FM_GlobalData.ChildCmdErrCounter  = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8();

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), OS_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);
    os_dirent_t dummy_direntry = {.FileName = "ThisDirectory"};
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &dummy_direntry, sizeof(dummy_direntry), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_GetFilenameState), FM_NAME_IS_DIRECTORY);

    uint8 childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 childcmdwarncount_before  = FM_GlobalData.ChildCmdWarnCounter;

    // Act
    FM_ChildDeleteAllCmd(&dummy_CC);

    uint8 count_sendevent  = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdiropen  = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));
    uint8 count_osdirclose = UT_GetStubCount(UT_KEY(OS_DirectoryClose));
    uint8 count_osdirread  = UT_GetStubCount(UT_KEY(OS_DirectoryRead));
    uint8 count_fmgetstate = UT_GetStubCount(UT_KEY(FM_GetFilenameState));

    // Assert
    UtAssert_INT32_EQ(count_osdiropen, 1);
    UtAssert_INT32_EQ(count_osdirclose, 1);
    UtAssert_INT32_EQ(count_osdirread, 2);
    UtAssert_INT32_EQ(count_fmgetstate, 1);
    UtAssert_INT32_EQ(count_sendevent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_ALL_CMD_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_DELETE_ALL_SKIP_WARNING_EID);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, FM_DELETE_ALL_CC);
    UtAssert_INT32_EQ(childcmderrcounter_before, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(childcmdcounter_before + 1, FM_GlobalData.ChildCmdCounter);
    UtAssert_INT32_EQ(childcmdwarncount_before + 1, FM_GlobalData.ChildCmdWarnCounter);
}

void Test_FM_ChildDeleteAllCmd_OpenFilenameState(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {
        .CommandCode = FM_DELETE_ALL_CC, .Source1 = "dummy_source1", .Source2 = "dummy_source2"};
    FM_GlobalData.ChildQueue[0]       = dummy_CC;
    FM_GlobalData.ChildReadIndex      = 0;
    FM_GlobalData.ChildPreviousCC     = -1;
    FM_GlobalData.ChildCurrentCC      = -1;
    FM_GlobalData.ChildCmdErrCounter  = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8();

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), OS_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);
    os_dirent_t dummy_direntry = {.FileName = "ThisDirectory"};
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &dummy_direntry, sizeof(dummy_direntry), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_GetFilenameState), FM_NAME_IS_FILE_OPEN);

    uint8 childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 childcmdwarncount_before  = FM_GlobalData.ChildCmdWarnCounter;

    // Act
    FM_ChildDeleteAllCmd(&dummy_CC);

    uint8 count_sendevent  = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdiropen  = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));
    uint8 count_osdirclose = UT_GetStubCount(UT_KEY(OS_DirectoryClose));
    uint8 count_osdirread  = UT_GetStubCount(UT_KEY(OS_DirectoryRead));
    uint8 count_fmgetstate = UT_GetStubCount(UT_KEY(FM_GetFilenameState));

    // Assert
    UtAssert_INT32_EQ(count_osdiropen, 1);
    UtAssert_INT32_EQ(count_osdirclose, 1);
    UtAssert_INT32_EQ(count_osdirread, 2);
    UtAssert_INT32_EQ(count_fmgetstate, 1);
    UtAssert_INT32_EQ(count_sendevent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_ALL_CMD_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_DELETE_ALL_FILES_ND_WARNING_EID);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, FM_DELETE_ALL_CC);
    UtAssert_INT32_EQ(childcmderrcounter_before, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(childcmdcounter_before + 1, FM_GlobalData.ChildCmdCounter);
    UtAssert_INT32_EQ(childcmdwarncount_before + 1, FM_GlobalData.ChildCmdWarnCounter);
}

void Test_FM_ChildDeleteAllCmd_ClosedFilename_OSRmNotSuccess(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {
        .CommandCode = FM_DELETE_ALL_CC, .Source1 = "dummy_source1", .Source2 = "dummy_source2"};
    FM_GlobalData.ChildQueue[0]       = dummy_CC;
    FM_GlobalData.ChildReadIndex      = 0;
    FM_GlobalData.ChildPreviousCC     = -1;
    FM_GlobalData.ChildCurrentCC      = -1;
    FM_GlobalData.ChildCmdErrCounter  = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8();

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), OS_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);
    os_dirent_t dummy_direntry = {.FileName = "ThisDirectory"};
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &dummy_direntry, sizeof(dummy_direntry), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_GetFilenameState), FM_NAME_IS_FILE_CLOSED);
    UT_SetDefaultReturnValue(UT_KEY(OS_remove), !OS_SUCCESS);

    uint8 childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 childcmdwarncount_before  = FM_GlobalData.ChildCmdWarnCounter;

    // Act
    FM_ChildDeleteAllCmd(&dummy_CC);

    uint8 count_sendevent  = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdiropen  = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));
    uint8 count_osdirclose = UT_GetStubCount(UT_KEY(OS_DirectoryClose));
    uint8 count_osdirread  = UT_GetStubCount(UT_KEY(OS_DirectoryRead));
    uint8 count_fmgetstate = UT_GetStubCount(UT_KEY(FM_GetFilenameState));
    uint8 count_osremove   = UT_GetStubCount(UT_KEY(OS_remove));

    // Assert
    UtAssert_INT32_EQ(count_osdiropen, 1);
    UtAssert_INT32_EQ(count_osdirclose, 1);
    UtAssert_INT32_EQ(count_osdirread, 2);
    UtAssert_INT32_EQ(count_fmgetstate, 1);
    UtAssert_INT32_EQ(count_osremove, 1);
    UtAssert_INT32_EQ(count_sendevent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_ALL_CMD_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_DELETE_ALL_FILES_ND_WARNING_EID);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, FM_DELETE_ALL_CC);
    UtAssert_INT32_EQ(childcmderrcounter_before, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(childcmdcounter_before + 1, FM_GlobalData.ChildCmdCounter);
    UtAssert_INT32_EQ(childcmdwarncount_before + 1, FM_GlobalData.ChildCmdWarnCounter);
}

void Test_FM_ChildDeleteAllCmd_ClosedFilename_OSrmSuccess(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {
        .CommandCode = FM_DELETE_ALL_CC, .Source1 = "dummy_source1", .Source2 = "dummy_source2"};
    FM_GlobalData.ChildQueue[0]       = dummy_CC;
    FM_GlobalData.ChildReadIndex      = 0;
    FM_GlobalData.ChildPreviousCC     = -1;
    FM_GlobalData.ChildCurrentCC      = -1;
    FM_GlobalData.ChildCmdErrCounter  = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8();

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), OS_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);
    os_dirent_t dummy_direntry = {.FileName = "ThisDirectory"};
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &dummy_direntry, sizeof(dummy_direntry), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_GetFilenameState), FM_NAME_IS_FILE_CLOSED);
    UT_SetDefaultReturnValue(UT_KEY(OS_remove), OS_SUCCESS);

    uint8 childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 childcmdwarncount_before  = FM_GlobalData.ChildCmdWarnCounter;

    // Act
    FM_ChildDeleteAllCmd(&dummy_CC);

    uint8 count_sendevent   = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdiropen   = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));
    uint8 count_osdirclose  = UT_GetStubCount(UT_KEY(OS_DirectoryClose));
    uint8 count_osdirread   = UT_GetStubCount(UT_KEY(OS_DirectoryRead));
    uint8 count_fmgetstate  = UT_GetStubCount(UT_KEY(FM_GetFilenameState));
    uint8 count_osremove    = UT_GetStubCount(UT_KEY(OS_remove));
    uint8 count_osdirrewind = UT_GetStubCount(UT_KEY(OS_DirectoryRewind));

    // Assert
    UtAssert_INT32_EQ(count_osdiropen, 1);
    UtAssert_INT32_EQ(count_osdirclose, 1);
    UtAssert_INT32_EQ(count_osdirread, 2);
    UtAssert_INT32_EQ(count_fmgetstate, 1);
    UtAssert_INT32_EQ(count_osremove, 1);
    UtAssert_INT32_EQ(count_osdirrewind, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_ALL_CMD_EID);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, FM_DELETE_ALL_CC);
    UtAssert_INT32_EQ(childcmderrcounter_before, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(childcmdcounter_before + 1, FM_GlobalData.ChildCmdCounter);
    UtAssert_INT32_EQ(childcmdwarncount_before, FM_GlobalData.ChildCmdWarnCounter);
}

void Test_FM_ChildDeleteAllCmd_FilenameStateDefaultReturn(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {
        .CommandCode = FM_DELETE_ALL_CC, .Source1 = "dummy_source1", .Source2 = "dummy_source2"};
    FM_GlobalData.ChildQueue[0]       = dummy_CC;
    FM_GlobalData.ChildReadIndex      = 0;
    FM_GlobalData.ChildPreviousCC     = -1;
    FM_GlobalData.ChildCurrentCC      = -1;
    FM_GlobalData.ChildCmdErrCounter  = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8();

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), OS_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);
    os_dirent_t dummy_direntry = {.FileName = "ThisDirectory"};
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &dummy_direntry, sizeof(dummy_direntry), false);
    UT_SetDefaultReturnValue(UT_KEY(FM_GetFilenameState), -1); // default case

    uint8 childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 childcmdwarncount_before  = FM_GlobalData.ChildCmdWarnCounter;

    // Act
    FM_ChildDeleteAllCmd(&dummy_CC);

    uint8 count_sendevent  = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdiropen  = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));
    uint8 count_osdirclose = UT_GetStubCount(UT_KEY(OS_DirectoryClose));
    uint8 count_osdirread  = UT_GetStubCount(UT_KEY(OS_DirectoryRead));
    uint8 count_fmgetstate = UT_GetStubCount(UT_KEY(FM_GetFilenameState));

    // Assert
    UtAssert_INT32_EQ(count_osdiropen, 1);
    UtAssert_INT32_EQ(count_osdirclose, 1);
    UtAssert_INT32_EQ(count_osdirread, 2);
    UtAssert_INT32_EQ(count_fmgetstate, 1);
    UtAssert_INT32_EQ(count_sendevent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_ALL_CMD_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_DELETE_ALL_FILES_ND_WARNING_EID);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, FM_DELETE_ALL_CC);
    UtAssert_INT32_EQ(childcmderrcounter_before, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(childcmdcounter_before + 1, FM_GlobalData.ChildCmdCounter);
    UtAssert_INT32_EQ(childcmdwarncount_before + 1, FM_GlobalData.ChildCmdWarnCounter);
}

/* ****************
 * ChildDecompressCmd Tests
 * ***************/
#ifdef FM_INCLUDE_DECOMPRESS
void Test_FM_ChildDecompressCmd_FSDecompressSuccess(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC    = {.CommandCode = UT_Utils_Any_uint8()};
    FM_GlobalData.ChildCmdCounter    = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(FS_LIB_Decompress), CFE_SUCCESS);

    uint8 count_childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 commandcode_before              = dummy_CC.CommandCode;

    // Act
    FM_ChildDecompressCmd(&dummy_CC);

    uint8 count_childcmdcounter_after    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_after = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_sendevent                = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_fsdecompress             = UT_GetStubCount(UT_KEY(FS_LIB_Decompress));

    // Assert
    UtAssert_INT32_EQ(count_fsdecompress, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DECOM_CMD_EID);
    UtAssert_INT32_EQ(count_childcmderrcounter_before, count_childcmderrcounter_after);
    UtAssert_INT32_EQ(count_childcmdcounter_before + 1, count_childcmdcounter_after);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, commandcode_before);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
}

void Test_FM_ChildDecompressCmd_FSDecompressNotSuccess(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC    = {.CommandCode = UT_Utils_Any_uint8()};
    FM_GlobalData.ChildCmdCounter    = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(FS_LIB_Decompress), !CFE_SUCCESS);

    uint8 count_childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 commandcode_before              = dummy_CC.CommandCode;

    // Act
    FM_ChildDecompressCmd(&dummy_CC);

    uint8 count_childcmdcounter_after    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_after = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_sendevent                = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_fsdecompress             = UT_GetStubCount(UT_KEY(FS_LIB_Decompress));

    // Assert
    UtAssert_INT32_EQ(count_fsdecompress, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DECOM_CFE_ERR_EID);
    UtAssert_INT32_EQ(count_childcmderrcounter_before + 1, count_childcmderrcounter_after);
    UtAssert_INT32_EQ(count_childcmdcounter_before, count_childcmdcounter_after);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, commandcode_before);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
}
#endif

/* ****************
 * ChildConcatCmd Tests
 * ***************/
void Test_FM_ChildConcatCmd_OSCpNotSuccess(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {
        .CommandCode = FM_DELETE_ALL_CC, .Source1 = "dummy_source1", .Source2 = "dummy_source2"};
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdCounter    = UT_Utils_Any_uint8();

    UT_SetDefaultReturnValue(UT_KEY(OS_cp), !OS_SUCCESS);

    uint8 childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;

    // Act
    FM_ChildConcatCmd(&dummy_CC);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_oscopy    = UT_GetStubCount(UT_KEY(OS_cp));

    // Assert
    UtAssert_INT32_EQ(count_oscopy, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CONCAT_OSCPY_ERR_EID);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
    UtAssert_INT32_EQ(childcmderrcounter_before + 1, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(childcmdcounter_before, FM_GlobalData.ChildCmdCounter);
}

void Test_FM_ChildConcatCmd_OSOpenCreateSourceNotSuccess(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {
        .CommandCode = FM_DELETE_ALL_CC, .Source1 = "dummy_source1", .Source2 = "dummy_source2"};
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdCounter    = UT_Utils_Any_uint8();

    UT_SetDefaultReturnValue(UT_KEY(OS_cp), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_OpenCreate), !OS_SUCCESS);

    uint8 childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;

    // Act
    FM_ChildConcatCmd(&dummy_CC);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_oscopy    = UT_GetStubCount(UT_KEY(OS_cp));
    uint8 count_osopencrt = UT_GetStubCount(UT_KEY(OS_OpenCreate));
    uint8 count_osremove  = UT_GetStubCount(UT_KEY(OS_remove));

    // Assert
    UtAssert_INT32_EQ(count_oscopy, 1);
    UtAssert_INT32_EQ(count_osopencrt, 1);
    UtAssert_INT32_EQ(count_osremove, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CONCAT_OPEN_SRC2_ERR_EID);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
    UtAssert_INT32_EQ(childcmderrcounter_before + 1, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(childcmdcounter_before, FM_GlobalData.ChildCmdCounter);
}

void Test_FM_ChildConcatCmd_OSOpenCreateTargetNotSuccess(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {
        .CommandCode = FM_DELETE_ALL_CC, .Source1 = "dummy_source1", .Source2 = "dummy_source2"};
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdCounter    = UT_Utils_Any_uint8();

    UT_SetDefaultReturnValue(UT_KEY(OS_cp), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_OpenCreate), OS_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(OS_OpenCreate), 2, !OS_SUCCESS);

    uint8 childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;

    // Act
    FM_ChildConcatCmd(&dummy_CC);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_oscopy    = UT_GetStubCount(UT_KEY(OS_cp));
    uint8 count_osopencrt = UT_GetStubCount(UT_KEY(OS_OpenCreate));
    uint8 count_osremove  = UT_GetStubCount(UT_KEY(OS_remove));
    uint8 count_osclose   = UT_GetStubCount(UT_KEY(OS_close));

    // Assert
    UtAssert_INT32_EQ(count_oscopy, 1);
    UtAssert_INT32_EQ(count_osopencrt, 2);
    UtAssert_INT32_EQ(count_osremove, 1);
    UtAssert_INT32_EQ(count_osclose, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CONCAT_OPEN_TGT_ERR_EID);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
    UtAssert_INT32_EQ(childcmderrcounter_before + 1, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(childcmdcounter_before, FM_GlobalData.ChildCmdCounter);
}

void Test_FM_ChildConcatCmd_OSReadBytesZero(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {
        .CommandCode = FM_DELETE_ALL_CC, .Source1 = "dummy_source1", .Source2 = "dummy_source2"};
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdCounter    = UT_Utils_Any_uint8();

    UT_SetDefaultReturnValue(UT_KEY(OS_cp), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_OpenCreate), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_read), 0);

    uint8 childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;

    // Act
    FM_ChildConcatCmd(&dummy_CC);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_oscopy    = UT_GetStubCount(UT_KEY(OS_cp));
    uint8 count_osopencrt = UT_GetStubCount(UT_KEY(OS_OpenCreate));
    uint8 count_osremove  = UT_GetStubCount(UT_KEY(OS_remove));
    uint8 count_osclose   = UT_GetStubCount(UT_KEY(OS_close));
    uint8 count_osread    = UT_GetStubCount(UT_KEY(OS_read));

    // Assert
    UtAssert_INT32_EQ(count_osread, 1);
    UtAssert_INT32_EQ(count_oscopy, 1);
    UtAssert_INT32_EQ(count_osopencrt, 2);
    UtAssert_INT32_EQ(count_osremove, 0);
    UtAssert_INT32_EQ(count_osclose, 2);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CONCAT_CMD_EID);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
    UtAssert_INT32_EQ(childcmderrcounter_before, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(childcmdcounter_before + 1, FM_GlobalData.ChildCmdCounter);
}

void Test_FM_ChildConcatCmd_OSReadBytesLessThanZero(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {
        .CommandCode = FM_DELETE_ALL_CC, .Source1 = "dummy_source1", .Source2 = "dummy_source2"};
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdCounter    = UT_Utils_Any_uint8();

    UT_SetDefaultReturnValue(UT_KEY(OS_cp), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_OpenCreate), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_read), -1);

    uint8 childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;

    // Act
    FM_ChildConcatCmd(&dummy_CC);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_oscopy    = UT_GetStubCount(UT_KEY(OS_cp));
    uint8 count_osopencrt = UT_GetStubCount(UT_KEY(OS_OpenCreate));
    uint8 count_osremove  = UT_GetStubCount(UT_KEY(OS_remove));
    uint8 count_osclose   = UT_GetStubCount(UT_KEY(OS_close));
    uint8 count_osread    = UT_GetStubCount(UT_KEY(OS_read));

    // Assert
    UtAssert_INT32_EQ(count_osread, 1);
    UtAssert_INT32_EQ(count_oscopy, 1);
    UtAssert_INT32_EQ(count_osopencrt, 2);
    UtAssert_INT32_EQ(count_osremove, 1);
    UtAssert_INT32_EQ(count_osclose, 2);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CONCAT_OSRD_ERR_EID);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
    UtAssert_INT32_EQ(childcmderrcounter_before + 1, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(childcmdcounter_before, FM_GlobalData.ChildCmdCounter);
}

void Test_FM_ChildConcatCmd_BytesWrittenNotEqualBytesRead(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {
        .CommandCode = FM_DELETE_ALL_CC, .Source1 = "dummy_source1", .Source2 = "dummy_source2"};
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdCounter    = UT_Utils_Any_uint8();

    UT_SetDefaultReturnValue(UT_KEY(OS_cp), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_OpenCreate), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_read), 1);
    UT_SetDefaultReturnValue(UT_KEY(OS_write), 0);

    uint8 childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;

    // Act
    FM_ChildConcatCmd(&dummy_CC);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_oscopy    = UT_GetStubCount(UT_KEY(OS_cp));
    uint8 count_osopencrt = UT_GetStubCount(UT_KEY(OS_OpenCreate));
    uint8 count_osremove  = UT_GetStubCount(UT_KEY(OS_remove));
    uint8 count_osclose   = UT_GetStubCount(UT_KEY(OS_close));
    uint8 count_osread    = UT_GetStubCount(UT_KEY(OS_read));

    // Assert
    UtAssert_INT32_EQ(count_osread, 1);
    UtAssert_INT32_EQ(count_oscopy, 1);
    UtAssert_INT32_EQ(count_osopencrt, 2);
    UtAssert_INT32_EQ(count_osremove, 1);
    UtAssert_INT32_EQ(count_osclose, 2);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CONCAT_OSWR_ERR_EID);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
    UtAssert_INT32_EQ(childcmderrcounter_before + 1, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(childcmdcounter_before, FM_GlobalData.ChildCmdCounter);
}

void Test_FM_ChildConcatCmd_CopyInProgressTrueLoopCountEqualChildFileLoopCount(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {
        .CommandCode = FM_DELETE_ALL_CC, .Source1 = "dummy_source1", .Source2 = "dummy_source2"};
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdCounter    = UT_Utils_Any_uint8();

    UT_SetDefaultReturnValue(UT_KEY(OS_cp), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_OpenCreate), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_read), 1);
    UT_SetDefaultReturnValue(UT_KEY(OS_write), 1);
    UT_SetDeferredRetcode(UT_KEY(OS_read), 17, -1);

    uint8 childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;

    // Act
    FM_ChildConcatCmd(&dummy_CC);

    uint8 count_sendevent   = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_oscopy      = UT_GetStubCount(UT_KEY(OS_cp));
    uint8 count_osopencrt   = UT_GetStubCount(UT_KEY(OS_OpenCreate));
    uint8 count_osremove    = UT_GetStubCount(UT_KEY(OS_remove));
    uint8 count_osclose     = UT_GetStubCount(UT_KEY(OS_close));
    uint8 count_osread      = UT_GetStubCount(UT_KEY(OS_read));
    uint8 count_ostaskdelay = UT_GetStubCount(UT_KEY(OS_TaskDelay));

    // Assert
    UtAssert_INT32_EQ(count_osread, FM_CHILD_FILE_LOOP_COUNT + 1);
    UtAssert_INT32_EQ(count_ostaskdelay, 1);
    UtAssert_INT32_EQ(count_oscopy, 1);
    UtAssert_INT32_EQ(count_osopencrt, 2);
    UtAssert_INT32_EQ(count_osremove, 1);
    UtAssert_INT32_EQ(count_osclose, 2);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CONCAT_OSRD_ERR_EID);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
    UtAssert_INT32_EQ(childcmderrcounter_before + 1, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(childcmdcounter_before, FM_GlobalData.ChildCmdCounter);
}

/* ****************
 * ChildFileInfoCmd Tests
 * ***************/
void Test_FM_ChildFileInfoCmd_FileInfoCRCEqualIgnoreCRC(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC     = {.CommandCode   = FM_DELETE_ALL_CC,
                                     .Source1       = "dummy_source1",
                                     .Source2       = "dummy_source2",
                                     .FileInfoCRC   = FM_IGNORE_CRC,
                                     .FileInfoState = FM_NAME_IS_FILE_CLOSED};
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8();

    uint8 childcmdwarncountr_before = FM_GlobalData.ChildCmdWarnCounter;

    // Act
    FM_ChildFileInfoCmd(&dummy_CC);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osopencrt = UT_GetStubCount(UT_KEY(OS_OpenCreate));

    // Assert
    UtAssert_INT32_EQ(count_osopencrt, 0);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_FILE_INFO_CMD_EID);
    UtAssert_INT32_EQ(childcmdwarncountr_before, FM_GlobalData.ChildCmdWarnCounter);
}

void Test_FM_ChildFileInfoCmd_FileInfoStateIsNotFileClosed(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC     = {.CommandCode   = FM_DELETE_ALL_CC,
                                     .Source1       = "dummy_source1",
                                     .Source2       = "dummy_source2",
                                     .FileInfoCRC   = CFE_MISSION_ES_CRC_8,
                                     .FileInfoState = FM_NAME_IS_FILE_OPEN};
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8();

    uint8 childcmdwarncountr_before = FM_GlobalData.ChildCmdWarnCounter;

    // Act
    FM_ChildFileInfoCmd(&dummy_CC);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osopencrt = UT_GetStubCount(UT_KEY(OS_OpenCreate));

    // Assert
    UtAssert_INT32_EQ(count_osopencrt, 0);
    UtAssert_INT32_EQ(count_sendevent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_FILE_INFO_STATE_WARNING_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_GET_FILE_INFO_CMD_EID);
    UtAssert_INT32_EQ(childcmdwarncountr_before + 1, FM_GlobalData.ChildCmdWarnCounter);
}

void Test_FM_ChildFileInfoCmd_FileInfoCRCEqualMission8(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC     = {.CommandCode   = FM_DELETE_ALL_CC,
                                     .Source1       = "dummy_source1",
                                     .Source2       = "dummy_source2",
                                     .FileInfoCRC   = CFE_MISSION_ES_CRC_8,
                                     .FileInfoState = FM_NAME_IS_FILE_CLOSED};
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_OpenCreate), !OS_SUCCESS);

    uint8 childcmdwarncountr_before = FM_GlobalData.ChildCmdWarnCounter;

    // Act
    FM_ChildFileInfoCmd(&dummy_CC);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osopencrt = UT_GetStubCount(UT_KEY(OS_OpenCreate));

    // Assert
    UtAssert_INT32_EQ(count_osopencrt, 1);
    UtAssert_INT32_EQ(count_sendevent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_FILE_INFO_OPEN_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_GET_FILE_INFO_CMD_EID);
    UtAssert_INT32_EQ(childcmdwarncountr_before + 1, FM_GlobalData.ChildCmdWarnCounter);
}

void Test_FM_ChildFileInfoCmd_FileInfoCRCEqualMission16(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC     = {.CommandCode   = FM_DELETE_ALL_CC,
                                     .Source1       = "dummy_source1",
                                     .Source2       = "dummy_source2",
                                     .FileInfoCRC   = CFE_MISSION_ES_CRC_16,
                                     .FileInfoState = FM_NAME_IS_FILE_CLOSED};
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_OpenCreate), !OS_SUCCESS);

    uint8 childcmdwarncountr_before = FM_GlobalData.ChildCmdWarnCounter;

    // Act
    FM_ChildFileInfoCmd(&dummy_CC);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osopencrt = UT_GetStubCount(UT_KEY(OS_OpenCreate));

    // Assert
    UtAssert_INT32_EQ(count_osopencrt, 1);
    UtAssert_INT32_EQ(count_sendevent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_FILE_INFO_OPEN_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_GET_FILE_INFO_CMD_EID);
    UtAssert_INT32_EQ(childcmdwarncountr_before + 1, FM_GlobalData.ChildCmdWarnCounter);
}

void Test_FM_ChildFileInfoCmd_FileInfoCRCEqualMission32(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC     = {.CommandCode   = FM_DELETE_ALL_CC,
                                     .Source1       = "dummy_source1",
                                     .Source2       = "dummy_source2",
                                     .FileInfoCRC   = CFE_MISSION_ES_CRC_32,
                                     .FileInfoState = FM_NAME_IS_FILE_CLOSED};
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_OpenCreate), !OS_SUCCESS);

    uint8 childcmdwarncountr_before = FM_GlobalData.ChildCmdWarnCounter;

    // Act
    FM_ChildFileInfoCmd(&dummy_CC);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osopencrt = UT_GetStubCount(UT_KEY(OS_OpenCreate));

    // Assert
    UtAssert_INT32_EQ(count_osopencrt, 1);
    UtAssert_INT32_EQ(count_sendevent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_FILE_INFO_OPEN_ERR_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_GET_FILE_INFO_CMD_EID);
    UtAssert_INT32_EQ(childcmdwarncountr_before + 1, FM_GlobalData.ChildCmdWarnCounter);
}

void Test_FM_ChildFileInfoCmd_FileInfoCRCNotEqualToAnyMissionES(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC     = {.CommandCode   = FM_DELETE_ALL_CC,
                                     .Source1       = "dummy_source1",
                                     .Source2       = "dummy_source2",
                                     .FileInfoCRC   = -1,
                                     .FileInfoState = FM_NAME_IS_FILE_CLOSED};
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8();

    uint8 childcmdwarncountr_before = FM_GlobalData.ChildCmdWarnCounter;

    // Act
    FM_ChildFileInfoCmd(&dummy_CC);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osopencrt = UT_GetStubCount(UT_KEY(OS_OpenCreate));

    // Assert
    UtAssert_INT32_EQ(count_osopencrt, 0);
    UtAssert_INT32_EQ(count_sendevent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_FILE_INFO_TYPE_WARNING_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_GET_FILE_INFO_CMD_EID);
    UtAssert_INT32_EQ(childcmdwarncountr_before + 1, FM_GlobalData.ChildCmdWarnCounter);
}

void Test_FM_ChildFileInfoCmd_OSOpenCreateTrueBytesReadZero(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC     = {.CommandCode   = FM_DELETE_ALL_CC,
                                     .Source1       = "dummy_source1",
                                     .Source2       = "dummy_source2",
                                     .FileInfoCRC   = CFE_MISSION_ES_CRC_16,
                                     .FileInfoState = FM_NAME_IS_FILE_CLOSED};
    FM_GlobalData.ChildCmdCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8();

    UT_SetDefaultReturnValue(UT_KEY(OS_OpenCreate), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_read), 0);

    uint8 childcmdwarncountr_before = FM_GlobalData.ChildCmdWarnCounter;

    // Act
    FM_ChildFileInfoCmd(&dummy_CC);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osopencrt = UT_GetStubCount(UT_KEY(OS_OpenCreate));
    uint8 count_osclose   = UT_GetStubCount(UT_KEY(OS_close));
    uint8 count_osread    = UT_GetStubCount(UT_KEY(OS_read));

    // Assert
    UtAssert_INT32_EQ(count_osread, 1);
    UtAssert_INT32_EQ(count_osopencrt, 1);
    UtAssert_INT32_EQ(count_osclose, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_FILE_INFO_CMD_EID);
    UtAssert_INT32_EQ(childcmdwarncountr_before, FM_GlobalData.ChildCmdWarnCounter);
}

void Test_FM_ChildFileInfoCmd_BytesReadLessThanZero(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC     = {.CommandCode   = FM_DELETE_ALL_CC,
                                     .Source1       = "dummy_source1",
                                     .Source2       = "dummy_source2",
                                     .FileInfoCRC   = CFE_MISSION_ES_CRC_16,
                                     .FileInfoState = FM_NAME_IS_FILE_CLOSED};
    FM_GlobalData.ChildCmdCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8();

    UT_SetDefaultReturnValue(UT_KEY(OS_OpenCreate), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_read), -1);

    uint8 childcmdwarncountr_before = FM_GlobalData.ChildCmdWarnCounter;

    // Act
    FM_ChildFileInfoCmd(&dummy_CC);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osopencrt = UT_GetStubCount(UT_KEY(OS_OpenCreate));
    uint8 count_osclose   = UT_GetStubCount(UT_KEY(OS_close));
    uint8 count_osread    = UT_GetStubCount(UT_KEY(OS_read));

    // Assert
    UtAssert_INT32_EQ(count_osread, 1);
    UtAssert_INT32_EQ(count_osopencrt, 1);
    UtAssert_INT32_EQ(count_osclose, 1);
    UtAssert_INT32_EQ(count_sendevent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_FILE_INFO_READ_WARNING_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_GET_FILE_INFO_CMD_EID);
    UtAssert_INT32_EQ(childcmdwarncountr_before + 1, FM_GlobalData.ChildCmdWarnCounter);
}

void Test_FM_ChildFileInfoCmd_BytesReadGreaterThanZero(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC     = {.CommandCode   = FM_DELETE_ALL_CC,
                                     .Source1       = "dummy_source1",
                                     .Source2       = "dummy_source2",
                                     .FileInfoCRC   = CFE_MISSION_ES_CRC_8,
                                     .FileInfoState = FM_NAME_IS_FILE_CLOSED};
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8();

    UT_SetDefaultReturnValue(UT_KEY(OS_OpenCreate), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_read), 1);
    UT_SetDeferredRetcode(UT_KEY(OS_read), FM_CHILD_FILE_LOOP_COUNT + 1, 0);
    UT_SetDefaultReturnValue(UT_KEY(CFE_ES_CalculateCRC), 0);

    uint8 childcmdwarncountr_before = FM_GlobalData.ChildCmdWarnCounter;

    // Act
    FM_ChildFileInfoCmd(&dummy_CC);

    uint8 count_sendevent   = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osopencrt   = UT_GetStubCount(UT_KEY(OS_OpenCreate));
    uint8 count_osclose     = UT_GetStubCount(UT_KEY(OS_close));
    uint8 count_osread      = UT_GetStubCount(UT_KEY(OS_read));
    uint8 count_ostaskdelay = UT_GetStubCount(UT_KEY(OS_TaskDelay));

    // Assert
    UtAssert_INT32_EQ(count_osread, FM_CHILD_FILE_LOOP_COUNT + 1);
    UtAssert_INT32_EQ(count_ostaskdelay, 1);
    UtAssert_INT32_EQ(count_osopencrt, 1);
    UtAssert_INT32_EQ(count_osclose, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_FILE_INFO_CMD_EID);
    UtAssert_INT32_EQ(childcmdwarncountr_before, FM_GlobalData.ChildCmdWarnCounter);
}

/* ****************
 * ChildCreateDirCmd Tests
 * ***************/
void Test_FM_ChildCreateDirCmd_OSMkDirSuccess(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC    = {.CommandCode = UT_Utils_Any_uint8()};
    FM_GlobalData.ChildCmdCounter    = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_mkdir), CFE_SUCCESS);

    uint8 count_childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 commandcode_before              = dummy_CC.CommandCode;

    // Act
    FM_ChildCreateDirCmd(&dummy_CC);

    uint8 count_childcmdcounter_after    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_after = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_sendevent                = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osmkdir                  = UT_GetStubCount(UT_KEY(OS_mkdir));

    // Assert
    UtAssert_INT32_EQ(count_osmkdir, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CREATE_DIR_CMD_EID);
    UtAssert_INT32_EQ(count_childcmderrcounter_before, count_childcmderrcounter_after);
    UtAssert_INT32_EQ(count_childcmdcounter_before + 1, count_childcmdcounter_after);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, commandcode_before);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
}

void Test_FM_ChildCreateDirCmd_OSMkDirNotSuccess(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC    = {.CommandCode = UT_Utils_Any_uint8()};
    FM_GlobalData.ChildCmdCounter    = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_mkdir), !CFE_SUCCESS);

    uint8 count_childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 commandcode_before              = dummy_CC.CommandCode;

    // Act
    FM_ChildCreateDirCmd(&dummy_CC);

    uint8 count_childcmdcounter_after    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_after = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_sendevent                = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osmkdir                  = UT_GetStubCount(UT_KEY(OS_mkdir));

    // Assert
    UtAssert_INT32_EQ(count_osmkdir, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_CREATE_DIR_OS_ERR_EID);
    UtAssert_INT32_EQ(count_childcmderrcounter_before + 1, count_childcmderrcounter_after);
    UtAssert_INT32_EQ(count_childcmdcounter_before, count_childcmdcounter_after);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, commandcode_before);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
}

/* ****************
 * ChildDeleteDirCmd Tests
 * ***************/
void Test_FM_ChildDeleteDirCmd_OSDirectoryOpenNoSuccess(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC    = {.CommandCode = UT_Utils_Any_uint8()};
    FM_GlobalData.ChildCmdCounter    = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), !OS_SUCCESS);

    uint8 count_childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 commandcode_before              = dummy_CC.CommandCode;

    // Act
    FM_ChildDeleteDirCmd(&dummy_CC);

    uint8 count_childcmdcounter_after    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_after = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_sendevent                = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdiropen                = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));

    // Assert
    UtAssert_INT32_EQ(count_osdiropen, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_OPENDIR_OS_ERR_EID);
    UtAssert_INT32_EQ(count_childcmderrcounter_before + 1, count_childcmderrcounter_after);
    UtAssert_INT32_EQ(count_childcmdcounter_before, count_childcmdcounter_after);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, commandcode_before);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
}

void Test_FM_ChildDeleteDirCmd_OSDirectoryReadNoSuccess(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {.CommandCode = UT_Utils_Any_uint8()};
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), !OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_rmdir), OS_SUCCESS);

    uint8 commandcode_before = dummy_CC.CommandCode;

    // Act
    FM_ChildDeleteDirCmd(&dummy_CC);

    uint8 count_osdiropen  = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));
    uint8 count_osdirread  = UT_GetStubCount(UT_KEY(OS_DirectoryRead));
    uint8 count_osdirclose = UT_GetStubCount(UT_KEY(OS_DirectoryClose));

    // Assert
    UtAssert_INT32_EQ(count_osdiropen, 1);
    UtAssert_INT32_EQ(count_osdirread, 1);
    UtAssert_INT32_EQ(count_osdirclose, 1);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, commandcode_before);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
}

void Test_FM_ChildDeleteDirCmd_StrCmpThisDirectoryZero(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC    = {.CommandCode = UT_Utils_Any_uint8()};
    FM_GlobalData.ChildCmdCounter    = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(strcmp), 0);
    UT_SetDeferredRetcode(UT_KEY(strcmp), 1, 1);
    UT_SetDefaultReturnValue(UT_KEY(OS_rmdir), OS_SUCCESS);

    uint8 count_childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 commandcode_before              = dummy_CC.CommandCode;

    // Act
    FM_ChildDeleteDirCmd(&dummy_CC);

    uint8 count_childcmdcounter_after    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_after = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_sendevent                = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdiropen                = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));
    uint8 count_osdirread                = UT_GetStubCount(UT_KEY(OS_DirectoryRead));

    // Assert
    UtAssert_INT32_EQ(count_osdiropen, 1);
    UtAssert_INT32_EQ(count_osdirread, 2);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_DIR_EMPTY_ERR_EID);
    UtAssert_INT32_EQ(count_childcmderrcounter_before + 1, count_childcmderrcounter_after);
    UtAssert_INT32_EQ(count_childcmdcounter_before, count_childcmdcounter_after);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, commandcode_before);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
}

void Test_FM_ChildDeleteDirCmd_StrCmpParentDirectoryZero(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC    = {.CommandCode = UT_Utils_Any_uint8()};
    FM_GlobalData.ChildCmdCounter    = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_rmdir), OS_SUCCESS);

    os_dirent_t dummy_direntry = {.FileName = FM_PARENT_DIRECTORY};
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &dummy_direntry, sizeof(dummy_direntry), false);

    uint8 count_childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 commandcode_before              = dummy_CC.CommandCode;

    // Act
    FM_ChildDeleteDirCmd(&dummy_CC);

    uint8 count_childcmdcounter_after    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_after = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_sendevent                = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdiropen                = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));
    uint8 count_osdirread                = UT_GetStubCount(UT_KEY(OS_DirectoryRead));

    // Assert
    UtAssert_INT32_EQ(count_osdiropen, 1);
    UtAssert_INT32_EQ(count_osdirread, 3);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_DIR_EMPTY_ERR_EID);
    UtAssert_INT32_EQ(count_childcmderrcounter_before + 1, count_childcmderrcounter_after);
    UtAssert_INT32_EQ(count_childcmdcounter_before, count_childcmdcounter_after);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, commandcode_before);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
}

void Test_FM_ChildDeleteDirCmd_StrcmpNeitherDirectoryNotZero(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC    = {.CommandCode = UT_Utils_Any_uint8()};
    FM_GlobalData.ChildCmdCounter    = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), OS_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 1, !OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_rmdir), OS_SUCCESS);

    os_dirent_t dummy_direntry = {.FileName = "dummy_directory"};
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &dummy_direntry, sizeof(dummy_direntry), false);

    uint8 count_childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 commandcode_before              = dummy_CC.CommandCode;

    // Act
    FM_ChildDeleteDirCmd(&dummy_CC);

    uint8 count_childcmdcounter_after    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_after = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_sendevent                = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdiropen                = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));
    uint8 count_osdirread                = UT_GetStubCount(UT_KEY(OS_DirectoryRead));

    // Assert
    UtAssert_INT32_EQ(count_osdiropen, 1);
    UtAssert_INT32_EQ(count_osdirread, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_DIR_CMD_EID);
    UtAssert_INT32_EQ(count_childcmderrcounter_before, count_childcmderrcounter_after);
    UtAssert_INT32_EQ(count_childcmdcounter_before + 1, count_childcmdcounter_after);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, commandcode_before);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
}

void Test_FM_ChildDeleteDirCmd_RemoveTheDirIsTrueRmDirSuccess(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC    = {.CommandCode = UT_Utils_Any_uint8()};
    FM_GlobalData.ChildCmdCounter    = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), !OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_rmdir), OS_SUCCESS);

    uint8 count_childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 commandcode_before              = dummy_CC.CommandCode;

    // Act
    FM_ChildDeleteDirCmd(&dummy_CC);

    uint8 count_childcmdcounter_after    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_after = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_sendevent                = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdiropen                = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));
    uint8 count_osdirread                = UT_GetStubCount(UT_KEY(OS_DirectoryRead));

    // Assert
    UtAssert_INT32_EQ(count_osdiropen, 1);
    UtAssert_INT32_EQ(count_osdirread, 1); // read when going through while loop second time
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_DIR_CMD_EID);
    UtAssert_INT32_EQ(count_childcmderrcounter_before, count_childcmderrcounter_after);
    UtAssert_INT32_EQ(count_childcmdcounter_before + 1, count_childcmdcounter_after);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, commandcode_before);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
}

void Test_FM_ChildDeleteDirCmd_RemoveDirTrueOSRmDirNotSuccess(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC    = {.CommandCode = UT_Utils_Any_uint8()};
    FM_GlobalData.ChildCmdCounter    = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), !OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_rmdir), !OS_SUCCESS);

    uint8 count_childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 commandcode_before              = dummy_CC.CommandCode;

    // Act
    FM_ChildDeleteDirCmd(&dummy_CC);

    uint8 count_childcmdcounter_after    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_after = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_sendevent                = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdiropen                = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));
    uint8 count_osdirread                = UT_GetStubCount(UT_KEY(OS_DirectoryRead));

    // Assert
    UtAssert_INT32_EQ(count_osdiropen, 1);
    UtAssert_INT32_EQ(count_osdirread, 1); // read when going through while loop second time
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_DELETE_RMDIR_OS_ERR_EID);
    UtAssert_INT32_EQ(count_childcmderrcounter_before + 1, count_childcmderrcounter_after);
    UtAssert_INT32_EQ(count_childcmdcounter_before, count_childcmdcounter_after);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, commandcode_before);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
}

/* ****************
 * ChildDirListFileCmd Tests
 * ***************/
void Test_FM_ChildDirListFileCmd_OSDirOpenNotSuccess(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC    = {.CommandCode = UT_Utils_Any_uint8()};
    FM_GlobalData.ChildCmdCounter    = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), !OS_SUCCESS);

    uint8 count_childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 commandcode_before              = dummy_CC.CommandCode;

    // Act
    FM_ChildDirListFileCmd(&dummy_CC);

    uint8 count_childcmdcounter_after    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_after = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_sendevent                = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdiropen                = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));
    uint8 count_osdirclose               = UT_GetStubCount(UT_KEY(OS_DirectoryClose));

    // Assert
    UtAssert_INT32_EQ(count_osdiropen, 1);
    UtAssert_INT32_EQ(count_osdirclose, 0);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_FILE_OSOPENDIR_ERR_EID);
    UtAssert_INT32_EQ(count_childcmderrcounter_before + 1, count_childcmderrcounter_after);
    UtAssert_INT32_EQ(count_childcmdcounter_before, count_childcmdcounter_after);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, commandcode_before);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
}

void Test_FM_ChildDirListFileCmd_ChildDirListFileInitFalse(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {
        .CommandCode = UT_Utils_Any_uint8(), .Source1 = "dummy_source1", .Target = "dummy_target"};
    FM_GlobalData.ChildCmdCounter    = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), OS_SUCCESS);
    // Inner Function Arrange
    UT_SetDefaultReturnValue(UT_KEY(OS_OpenCreate), !OS_SUCCESS);

    uint8 count_childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 commandcode_before              = dummy_CC.CommandCode;

    // Act
    FM_ChildDirListFileCmd(&dummy_CC);

    uint8 count_childcmdcounter_after    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_after = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_sendevent                = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdiropen                = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));
    uint8 count_osdirclose               = UT_GetStubCount(UT_KEY(OS_DirectoryClose));
    uint8 count_osclose                  = UT_GetStubCount(UT_KEY(OS_close));

    // Assert
    UtAssert_INT32_EQ(count_osdiropen, 1);
    UtAssert_INT32_EQ(count_osdirclose, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(count_osclose, 0);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_FILE_OSCREAT_ERR_EID);
    UtAssert_INT32_EQ(count_childcmderrcounter_before + 1, count_childcmderrcounter_after);
    UtAssert_INT32_EQ(count_childcmdcounter_before, count_childcmdcounter_after);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, commandcode_before);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
}

void Test_FM_ChildDirListFileCmd_ChildDirListFileInitTrue(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {
        .CommandCode = UT_Utils_Any_uint8(), .Source1 = "dummy_source1", .Target = "dummy_target"};
    FM_GlobalData.ChildCmdCounter    = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_OpenCreate), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_FS_WriteHeader), 64);
    UT_SetDeferredRetcode(UT_KEY(OS_write), 2, 72);
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), !OS_SUCCESS);

    uint8 count_childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 commandcode_before              = dummy_CC.CommandCode;

    // Act
    FM_ChildDirListFileCmd(&dummy_CC);

    uint8 count_childcmdcounter_after    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_after = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_sendevent                = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdiropen                = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));
    uint8 count_osdirclose               = UT_GetStubCount(UT_KEY(OS_DirectoryClose));
    uint8 count_osclose                  = UT_GetStubCount(UT_KEY(OS_close));

    // Assert
    UtAssert_INT32_EQ(count_osdiropen, 1);
    UtAssert_INT32_EQ(count_osdirclose, 1); // read when going through while loop second time
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(count_osclose, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_FILE_CMD_EID);
    UtAssert_INT32_EQ(count_childcmderrcounter_before, count_childcmderrcounter_after);
    UtAssert_INT32_EQ(count_childcmdcounter_before + 1, count_childcmdcounter_after);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, commandcode_before);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
}

/* ****************
 * ChildDirListPktCmd Tests
 * ***************/
void Test_FM_ChildDirListPktCmd_OSDirOpenNotSuccess(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {
        .CommandCode = FM_DELETE_ALL_CC, .Source1 = "dummy_source1", .Source2 = "dummy_source2"};
    FM_GlobalData.ChildCmdCounter     = UT_Utils_Any_uint8_LessThan(MAX_UINT8);
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8_LessThan(MAX_UINT8);
    FM_GlobalData.ChildCmdErrCounter  = UT_Utils_Any_uint8_LessThan(MAX_UINT8);

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), !OS_SUCCESS);

    uint8 childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;

    // Act
    FM_ChildDirListPktCmd(&dummy_CC);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdiropen = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));

    // Assert
    UtAssert_INT32_EQ(count_osdiropen, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_PKT_OS_ERR_EID);
    UtAssert_INT32_EQ(childcmdcounter_before, FM_GlobalData.ChildCmdCounter);
    UtAssert_INT32_EQ(childcmderrcounter_before + 1, FM_GlobalData.ChildCmdErrCounter);
}

void Test_FM_ChildDirListPktCmd_OSDirReadNotSuccess(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {
        .CommandCode = FM_DELETE_ALL_CC, .Source1 = "dummy_source1", .Source2 = "dummy_source2"};
    FM_GlobalData.ChildCmdCounter     = UT_Utils_Any_uint8_LessThan(MAX_UINT8);
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8_LessThan(MAX_UINT8);
    FM_GlobalData.ChildCmdErrCounter  = UT_Utils_Any_uint8_LessThan(MAX_UINT8);

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), !OS_SUCCESS);

    uint8 childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;

    // Act
    FM_ChildDirListPktCmd(&dummy_CC);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdiropen = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));
    uint8 count_osdirread = UT_GetStubCount(UT_KEY(OS_DirectoryRead));

    // Assert
    UtAssert_INT32_EQ(count_osdiropen, 1);
    UtAssert_INT32_EQ(count_osdirread, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_PKT_CMD_EID);
    UtAssert_INT32_EQ(childcmdcounter_before + 1, FM_GlobalData.ChildCmdCounter);
    UtAssert_INT32_EQ(childcmderrcounter_before, FM_GlobalData.ChildCmdErrCounter);
}

void Test_FM_ChildDirListPktCmd_DirEntryNameThisDirectory(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {
        .CommandCode = FM_DELETE_ALL_CC, .Source1 = "dummy_source1", .Source2 = "dummy_source2"};
    FM_GlobalData.ChildCmdCounter     = UT_Utils_Any_uint8_LessThan(MAX_UINT8);
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8_LessThan(MAX_UINT8);
    FM_GlobalData.ChildCmdErrCounter  = UT_Utils_Any_uint8_LessThan(MAX_UINT8);

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), OS_SUCCESS);
    os_dirent_t dummy_direntry = {.FileName = FM_THIS_DIRECTORY};
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &dummy_direntry, sizeof(dummy_direntry), false);
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);

    uint8 childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;

    // Act
    FM_ChildDirListPktCmd(&dummy_CC);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdiropen = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));
    uint8 count_osdirread = UT_GetStubCount(UT_KEY(OS_DirectoryRead));

    // Assert
    UtAssert_INT32_EQ(count_osdiropen, 1);
    UtAssert_INT32_EQ(count_osdirread, 2);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_PKT_CMD_EID);
    UtAssert_INT32_EQ(childcmdcounter_before + 1, FM_GlobalData.ChildCmdCounter);
    UtAssert_INT32_EQ(childcmderrcounter_before, FM_GlobalData.ChildCmdErrCounter);
}

void Test_FM_ChildDirListPktCmd_DirEntryNameParentDirectory(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {
        .CommandCode = FM_DELETE_ALL_CC, .Source1 = "dummy_source1", .Source2 = "dummy_source2"};
    FM_GlobalData.ChildCmdCounter     = UT_Utils_Any_uint8_LessThan(MAX_UINT8);
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8_LessThan(MAX_UINT8);
    FM_GlobalData.ChildCmdErrCounter  = UT_Utils_Any_uint8_LessThan(MAX_UINT8);

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), OS_SUCCESS);
    os_dirent_t dummy_direntry = {.FileName = FM_PARENT_DIRECTORY};
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &dummy_direntry, sizeof(dummy_direntry), false);
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);

    uint8 childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;

    // Act
    FM_ChildDirListPktCmd(&dummy_CC);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdiropen = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));
    uint8 count_osdirread = UT_GetStubCount(UT_KEY(OS_DirectoryRead));

    // Assert
    UtAssert_INT32_EQ(count_osdiropen, 1);
    UtAssert_INT32_EQ(count_osdirread, 2);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_PKT_CMD_EID);
    UtAssert_INT32_EQ(childcmdcounter_before + 1, FM_GlobalData.ChildCmdCounter);
    UtAssert_INT32_EQ(childcmderrcounter_before, FM_GlobalData.ChildCmdErrCounter);
}

void Test_FM_ChildDirListPktCmd_DirListPktTotalFilesLessThanDirListPktFirstFile(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC     = {.CommandCode   = FM_DELETE_ALL_CC,
                                     .Source1       = "dummy_source1",
                                     .Source2       = "dummy_source2",
                                     .DirListOffset = UT_Utils_Any_uint8_LessThan(MAX_UINT8)};
    FM_GlobalData.ChildCmdCounter     = UT_Utils_Any_uint8_LessThan(MAX_UINT8);
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8_LessThan(MAX_UINT8);

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), OS_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);
    os_dirent_t dummy_direntry = {.FileName = "dummy_filename"};
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &dummy_direntry, sizeof(dummy_direntry), false);
    FM_GlobalData.DirListPkt.TotalFiles = dummy_CC.DirListOffset - 2; // minus 2 because it gets incremented

    uint8 childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 dirlist_totalfiles_before = FM_GlobalData.DirListPkt.TotalFiles;

    // Act
    FM_ChildDirListPktCmd(&dummy_CC);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdiropen = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));
    uint8 count_osdirread = UT_GetStubCount(UT_KEY(OS_DirectoryRead));

    // Assert
    UtAssert_INT32_EQ(count_osdiropen, 1);
    UtAssert_INT32_EQ(count_osdirread, 2);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_PKT_CMD_EID);
    UtAssert_INT32_EQ(childcmdcounter_before + 1, FM_GlobalData.ChildCmdCounter);
    UtAssert_INT32_EQ(dirlist_totalfiles_before + 1, FM_GlobalData.DirListPkt.TotalFiles);
}

void Test_FM_ChildDirListPktCmd_DirListPktPacketFilesGreaterDIRListPktEntries(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC     = {.CommandCode   = FM_DELETE_ALL_CC,
                                     .Source1       = "dummy_source1",
                                     .Source2       = "dummy_source2",
                                     .DirListOffset = UT_Utils_Any_uint8_LessThan(MAX_UINT8)};
    FM_GlobalData.ChildCmdCounter     = UT_Utils_Any_uint8_LessThan(MAX_UINT8);
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8_LessThan(MAX_UINT8);

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), OS_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);
    os_dirent_t dummy_direntry = {.FileName = "dummy_filename"};
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &dummy_direntry, sizeof(dummy_direntry), false);
    FM_GlobalData.DirListPkt.TotalFiles  = dummy_CC.DirListOffset + 1;
    FM_GlobalData.DirListPkt.PacketFiles = FM_DIR_LIST_PKT_ENTRIES + 1;

    uint8 childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 dirlist_totalfiles_before = FM_GlobalData.DirListPkt.TotalFiles;

    // Act
    FM_ChildDirListPktCmd(&dummy_CC);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdiropen = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));
    uint8 count_osdirread = UT_GetStubCount(UT_KEY(OS_DirectoryRead));

    // Assert
    UtAssert_INT32_EQ(count_osdiropen, 1);
    UtAssert_INT32_EQ(count_osdirread, 2);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_PKT_CMD_EID);
    UtAssert_INT32_EQ(childcmdcounter_before + 1, FM_GlobalData.ChildCmdCounter);
    UtAssert_INT32_EQ(dirlist_totalfiles_before + 1, FM_GlobalData.DirListPkt.TotalFiles);
}

void Test_FM_ChildDirListPktCmd_EntryLengthGreaterListEntryName(void)
{
    /*
    // Arrange
    FM_ChildQueueEntry_t dummy_CC = {.CommandCode   = FM_DELETE_ALL_CC,
                                     .Source1       = "dummy_source1",
                                     .Source2       = "dummy_source2",
                                     .DirListOffset = UT_Utils_Any_uint8_LessThan(MAX_UINT8)};
    FM_GlobalData.ChildCmdCounter       = UT_Utils_Any_uint8_LessThan(MAX_UINT8);
    FM_GlobalData.ChildCmdWarnCounter   = UT_Utils_Any_uint8_LessThan(MAX_UINT8);

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), OS_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);
    os_dirent_t dummy_direntry = {.FileName="MaxCharactersIs20"};
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &dummy_direntry, sizeof(dummy_direntry), false);
    FM_DirListEntry_t dummy_listentry = {.EntryName="short"};
    FM_GlobalData.DirListPkt.TotalFiles = dummy_CC.DirListOffset + 1;
    uint32 temp_packetfiles = FM_DIR_LIST_PKT_ENTRIES - 1;
    FM_GlobalData.DirListPkt.PacketFiles = temp_packetfiles;
    FM_GlobalData.DirListPkt.FileList[temp_packetfiles] = dummy_listentry;

    uint8 childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 childcmdwarncountr_before = FM_GlobalData.ChildCmdWarnCounter;
    uint8 dirlist_totalfiles_before = FM_GlobalData.DirListPkt.TotalFiles;

    // Act
    FM_ChildDirListPktCmd(&dummy_CC);

    uint8 count_sendevent   = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdiropen   = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));
    uint8 count_osdirread   = UT_GetStubCount(UT_KEY(OS_DirectoryRead));

    // Assert
    UtAssert_INT32_EQ(count_osdiropen, 1);
    UtAssert_INT32_EQ(count_osdirread, 2);
    UtAssert_INT32_EQ(count_sendevent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_PKT_WARNING_EID);
    UtAssert_INT32_EQ(childcmdcounter_before + 1, FM_GlobalData.ChildCmdCounter);
    UtAssert_INT32_EQ(childcmdwarncountr_before + 1, FM_GlobalData.ChildCmdWarnCounter);
    UtAssert_INT32_EQ(dirlist_totalfiles_before + 1, FM_GlobalData.DirListPkt.TotalFiles);
    UtAssert_INT32_EQ(temp_packetfiles, FM_GlobalData.DirListPkt.PacketFiles);
    */
    UtAssert_MIR("Unable to change the OS_MAX_FILE_NAME value in order to test this branch of the if statement");
}

void Test_FM_ChildDirListPktCmd_PathAndEntryLengthGreaterMaxPathLength(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC     = {.CommandCode = FM_DELETE_ALL_CC,
                                     .Source1     = "dummy_source1",
                                     .Source2     = "dummy_source2_has_a_long_name_to_make_path_length_longer_than_64",
                                     .DirListOffset = 1};
    FM_GlobalData.ChildCmdCounter     = UT_Utils_Any_uint8_LessThan(MAX_UINT8);
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8_LessThan(MAX_UINT8);

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), OS_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);
    os_dirent_t dummy_direntry = {.FileName = "dummy_direntry_long"};
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &dummy_direntry, sizeof(dummy_direntry), false);
    FM_DirListEntry_t dummy_listentry                              = {.EntryName =
                                             "dummy_filenameshort_ButLong_to_Get_To_64_Blank_Space_Long_String"};
    FM_GlobalData.DirListPkt.TotalFiles                            = dummy_CC.DirListOffset + 1;
    FM_GlobalData.DirListPkt.PacketFiles                           = FM_DIR_LIST_PKT_ENTRIES - 1;
    FM_GlobalData.DirListPkt.FileList[FM_DIR_LIST_PKT_ENTRIES - 1] = dummy_listentry;

    uint8 childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 childcmdwarncountr_before = FM_GlobalData.ChildCmdWarnCounter;
    uint8 dirlist_totalfiles_before = FM_GlobalData.DirListPkt.TotalFiles;

    // Act
    FM_ChildDirListPktCmd(&dummy_CC);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdiropen = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));
    uint8 count_osdirread = UT_GetStubCount(UT_KEY(OS_DirectoryRead));

    // Assert
    UtAssert_INT32_EQ(count_osdiropen, 1);
    UtAssert_INT32_EQ(count_osdirread, 2);
    UtAssert_INT32_EQ(count_sendevent, 2);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_PKT_WARNING_EID);
    UtAssert_INT32_EQ(childcmdcounter_before + 1, FM_GlobalData.ChildCmdCounter);
    UtAssert_INT32_EQ(childcmdwarncountr_before + 1, FM_GlobalData.ChildCmdWarnCounter);
    UtAssert_INT32_EQ(dirlist_totalfiles_before + 1, FM_GlobalData.DirListPkt.TotalFiles);
}

void Test_FM_ChildDirListPktCmd_EntryLengthAndTotalPathLengthSmallEnough(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC     = {.CommandCode     = FM_DELETE_ALL_CC,
                                     .Source1         = "dummy_source1",
                                     .Source2         = "dummy_source2",
                                     .DirListOffset   = UT_Utils_Any_uint8_LessThan(MAX_UINT8),
                                     .GetSizeTimeMode = false};
    FM_GlobalData.ChildCmdCounter     = UT_Utils_Any_uint8_LessThan(MAX_UINT8);
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8_LessThan(MAX_UINT8);

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryOpen), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), OS_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);
    os_dirent_t dummy_direntry = {.FileName = "dummy_direntry"};
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &dummy_direntry, sizeof(dummy_direntry), false);
    FM_DirListEntry_t dummy_listentry                              = {.EntryName = "dummy_filenameshort"};
    FM_GlobalData.DirListPkt.PacketFiles                           = FM_DIR_LIST_PKT_ENTRIES - 1;
    FM_GlobalData.DirListPkt.FileList[FM_DIR_LIST_PKT_ENTRIES - 1] = dummy_listentry;

    uint8 childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 childcmdwarncountr_before = FM_GlobalData.ChildCmdWarnCounter;
    uint8 dirlist_totalfiles_before = FM_GlobalData.DirListPkt.TotalFiles;

    // Act
    FM_ChildDirListPktCmd(&dummy_CC);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdiropen = UT_GetStubCount(UT_KEY(OS_DirectoryOpen));
    uint8 count_osdirread = UT_GetStubCount(UT_KEY(OS_DirectoryRead));

    // Assert
    UtAssert_INT32_EQ(count_osdiropen, 1);
    UtAssert_INT32_EQ(count_osdirread, 2);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(childcmdcounter_before + 1, FM_GlobalData.ChildCmdCounter);
    UtAssert_INT32_EQ(childcmdwarncountr_before, FM_GlobalData.ChildCmdWarnCounter);
    UtAssert_INT32_EQ(dirlist_totalfiles_before + 1, FM_GlobalData.DirListPkt.TotalFiles);
    UtAssert_INT32_EQ(FM_GlobalData.DirListPkt.PacketFiles + 1, FM_DIR_LIST_PKT_ENTRIES);
}

/* ****************
 * ChildSetPermissionsCmd Tests
 * ***************/
void Test_FM_ChildSetPermissionsCmd_OSChmodSuccess(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC    = {.CommandCode = UT_Utils_Any_uint8()};
    FM_GlobalData.ChildCmdCounter    = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_chmod), OS_SUCCESS);

    uint8 count_childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 commandcode_before              = dummy_CC.CommandCode;

    // Act
    FM_ChildSetPermissionsCmd(&dummy_CC);

    uint8 count_childcmdcounter_after    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_after = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_sendevent                = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_oschmod                  = UT_GetStubCount(UT_KEY(OS_chmod));

    // Assert
    UtAssert_INT32_EQ(count_oschmod, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_SET_PERM_CMD_EID);
    UtAssert_INT32_EQ(count_childcmderrcounter_before, count_childcmderrcounter_after);
    UtAssert_INT32_EQ(count_childcmdcounter_before + 1, count_childcmdcounter_after);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, commandcode_before);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
}

void Test_FM_ChildSetPermissionsCmd_OSChmodNotSuccess(void)
{
    // Arrange
    FM_ChildQueueEntry_t dummy_CC    = {.CommandCode = UT_Utils_Any_uint8()};
    FM_GlobalData.ChildCmdCounter    = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_chmod), !OS_SUCCESS);

    uint8 count_childcmdcounter_before    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;
    uint8 commandcode_before              = dummy_CC.CommandCode;

    // Act
    FM_ChildSetPermissionsCmd(&dummy_CC);

    uint8 count_childcmdcounter_after    = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmderrcounter_after = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_sendevent                = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_oschmod                  = UT_GetStubCount(UT_KEY(OS_chmod));

    // Assert
    UtAssert_INT32_EQ(count_oschmod, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_SET_PERM_OS_ERR_EID);
    UtAssert_INT32_EQ(count_childcmderrcounter_before + 1, count_childcmderrcounter_after);
    UtAssert_INT32_EQ(count_childcmdcounter_before, count_childcmdcounter_after);
    UtAssert_INT32_EQ(FM_GlobalData.ChildPreviousCC, commandcode_before);
    UtAssert_INT32_EQ(FM_GlobalData.ChildCurrentCC, 0);
}

/* ****************
 * ChildDirListFileInit Tests
 * ***************/
void Test_FM_ChildDirListFileInit_OSOpenCreateFail(void)
{
    // Arrange
    osal_id_t   fileid;
    const char *dummy_directory      = "dummy_directory";
    const char *dummy_filename       = "dummy_filename";
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_OpenCreate), !OS_SUCCESS);

    uint8 count_childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;

    // Act
    bool result = FM_ChildDirListFileInit(&fileid, dummy_directory, dummy_filename);

    uint8 count_childcmderrcounter_after = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_sendevent                = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osopencreate             = UT_GetStubCount(UT_KEY(OS_OpenCreate));
    uint8 count_osclose                  = UT_GetStubCount(UT_KEY(OS_close));

    // Assert
    UtAssert_INT32_EQ(result, false);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(count_osopencreate, 1);
    UtAssert_INT32_EQ(count_osclose, 0);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_FILE_OSCREAT_ERR_EID);
    UtAssert_INT32_EQ(count_childcmderrcounter_before + 1, count_childcmderrcounter_after);
}

void Test_FM_ChildDirListFileInit_FSWriteHeaderNotSameSizeFSHeadert(void)
{
    // Arrange
    osal_id_t   fileid;
    osal_id_t   LocalFileHandle = FM_UT_OBJID_1;
    const char *dummy_directory = "dummy_directory";
    const char *dummy_filename  = "dummy_filename";

    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_OpenCreate), OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_OpenCreate), &LocalFileHandle, sizeof(osal_id_t), false);
    UT_SetDefaultReturnValue(UT_KEY(CFE_FS_WriteHeader), sizeof(CFE_FS_Header_t) - 1);

    uint8 count_childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;

    // Act
    bool result = FM_ChildDirListFileInit(&fileid, dummy_directory, dummy_filename);

    uint8 count_childcmderrcounter_after = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_sendevent                = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osopencreate             = UT_GetStubCount(UT_KEY(OS_OpenCreate));
    uint8 count_osclose                  = UT_GetStubCount(UT_KEY(OS_close));
    uint8 count_fswriteheader            = UT_GetStubCount(UT_KEY(CFE_FS_WriteHeader));

    // Assert
    UtAssert_INT32_EQ(result, false);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(count_osopencreate, 1);
    UtAssert_INT32_EQ(count_fswriteheader, 1);
    UtAssert_INT32_EQ(count_osclose, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_FILE_WRHDR_ERR_EID);
    UtAssert_INT32_EQ(count_childcmderrcounter_before + 1, count_childcmderrcounter_after);
}

void Test_FM_ChildDirListFileInit_OSWriteNotSameSizeDirListFileStatst(void)
{
    // Arrange
    osal_id_t   fileid;
    const char *dummy_directory = "dummy_directory";
    const char *dummy_filename  = "dummy_filename";
    osal_id_t   LocalFileHandle = FM_UT_OBJID_1;

    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_OpenCreate), OS_SUCCESS);
    UT_SetDataBuffer(UT_KEY(OS_OpenCreate), &LocalFileHandle, sizeof(osal_id_t), false);
    UT_SetDefaultReturnValue(UT_KEY(CFE_FS_WriteHeader), sizeof(CFE_FS_Header_t));
    UT_SetDefaultReturnValue(UT_KEY(OS_write), sizeof(FM_DirListFileStats_t) - 1);

    uint8 count_childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;

    // Act
    bool result = FM_ChildDirListFileInit(&fileid, dummy_directory, dummy_filename);

    uint8 count_childcmderrcounter_after = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_sendevent                = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osopencreate             = UT_GetStubCount(UT_KEY(OS_OpenCreate));
    uint8 count_osclose                  = UT_GetStubCount(UT_KEY(OS_close));
    uint8 count_fswriteheader            = UT_GetStubCount(UT_KEY(CFE_FS_WriteHeader));
    uint8 count_oswrite                  = UT_GetStubCount(UT_KEY(OS_write));

    // Assert
    UtAssert_INT32_EQ(result, false);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(count_osopencreate, 1);
    UtAssert_INT32_EQ(count_fswriteheader, 1);
    UtAssert_INT32_EQ(count_oswrite, 1);
    UtAssert_INT32_EQ(count_osclose, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_FILE_WRBLANK_ERR_EID);
    UtAssert_INT32_EQ(count_childcmderrcounter_before + 1, count_childcmderrcounter_after);
}

void Test_FM_ChildDirListFileInit_OSWriteSameSizeDirListFileStatst(void)
{
    // Arrange
    osal_id_t   fileid;
    const char *dummy_directory      = "dummy_directory";
    const char *dummy_filename       = "dummy_filename";
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_OpenCreate), OS_SUCCESS);
    UT_SetDefaultReturnValue(UT_KEY(CFE_FS_WriteHeader), sizeof(CFE_FS_Header_t));
    UT_SetDeferredRetcode(UT_KEY(OS_write), 1, sizeof(FM_DirListFileStats_t));

    uint8 count_childcmderrcounter_before = FM_GlobalData.ChildCmdErrCounter;

    // Act
    bool result = FM_ChildDirListFileInit(&fileid, dummy_directory, dummy_filename);

    uint8 count_childcmderrcounter_after = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_sendevent                = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osopencreate             = UT_GetStubCount(UT_KEY(OS_OpenCreate));
    uint8 count_osclose                  = UT_GetStubCount(UT_KEY(OS_close));
    uint8 count_fswriteheader            = UT_GetStubCount(UT_KEY(CFE_FS_WriteHeader));
    uint8 count_oswrite                  = UT_GetStubCount(UT_KEY(OS_write));

    // Assert
    UtAssert_INT32_EQ(result, true);
    UtAssert_INT32_EQ(count_sendevent, 0);
    UtAssert_INT32_EQ(count_osopencreate, 1);
    UtAssert_INT32_EQ(count_fswriteheader, 1);
    UtAssert_INT32_EQ(count_oswrite, 1);
    UtAssert_INT32_EQ(count_osclose, 0);
    UtAssert_INT32_EQ(count_childcmderrcounter_before, count_childcmderrcounter_after);
}

/* ****************
 * ChildDirListFileLoop Tests
 * ***************/
void Test_FM_ChildDirListFileLoop_OSDirReadNotSuccess(void)
{
    // Arrange
    osal_id_t   dirid                 = FM_UT_OBJID_1;
    osal_id_t   fileid                = FM_UT_OBJID_2;
    const char *dummy_directory       = "dummy_directory";
    const char *dummy_filename        = "dummy_filename";
    const char *dummy_dirwithstep     = "dummy_dirwithstep";
    uint8       dummy_getsizetimemode = UT_Utils_Any_uint8();

    FM_GlobalData.ChildCmdErrCounter  = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), !OS_SUCCESS);

    uint8 count_childcmderrcounter_before  = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_childcmdcounter_before     = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmdwarncounter_before = FM_GlobalData.ChildCmdWarnCounter;

    // Act
    FM_ChildDirListFileLoop(dirid, fileid, dummy_directory, dummy_dirwithstep, dummy_filename, dummy_getsizetimemode);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdirread = UT_GetStubCount(UT_KEY(OS_DirectoryRead));
    uint8 count_oswrite   = UT_GetStubCount(UT_KEY(OS_write));
    uint8 count_oslseek   = UT_GetStubCount(UT_KEY(OS_lseek));

    // Assert
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(count_osdirread, 1);
    UtAssert_INT32_EQ(count_oswrite, 0);
    UtAssert_INT32_EQ(count_oslseek, 0);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_FILE_CMD_EID);
    UtAssert_INT32_EQ(count_childcmderrcounter_before, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(count_childcmdcounter_before + 1, FM_GlobalData.ChildCmdCounter);
    UtAssert_INT32_EQ(count_childcmdwarncounter_before, FM_GlobalData.ChildCmdWarnCounter);
}

void Test_FM_ChildDirListFileLoop_OSDirEntryNameIsThisDirectory(void)
{
    // Arrange
    osal_id_t   dirid                 = FM_UT_OBJID_1;
    osal_id_t   fileid                = FM_UT_OBJID_2;
    const char *dummy_directory       = "dummy_directory";
    const char *dummy_filename        = "dummy_filename";
    const char *dummy_dirwithstep     = "dummy_dirwithstep";
    uint8       dummy_getsizetimemode = UT_Utils_Any_uint8();

    FM_GlobalData.ChildCmdErrCounter  = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8();

    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), OS_SUCCESS);
    os_dirent_t dummy_direntry = {.FileName = "."};
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &dummy_direntry, sizeof(dummy_direntry), false);
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);

    uint8 count_childcmderrcounter_before  = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_childcmdcounter_before     = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmdwarncounter_before = FM_GlobalData.ChildCmdWarnCounter;

    // Act
    FM_ChildDirListFileLoop(dirid, fileid, dummy_directory, dummy_dirwithstep, dummy_filename, dummy_getsizetimemode);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdirread = UT_GetStubCount(UT_KEY(OS_DirectoryRead));
    uint8 count_oswrite   = UT_GetStubCount(UT_KEY(OS_write));
    uint8 count_oslseek   = UT_GetStubCount(UT_KEY(OS_lseek));

    // Assert
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(count_osdirread, 2);
    UtAssert_INT32_EQ(count_oswrite, 0);
    UtAssert_INT32_EQ(count_oslseek, 0);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_FILE_CMD_EID);
    UtAssert_INT32_EQ(count_childcmderrcounter_before, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(count_childcmdcounter_before + 1, FM_GlobalData.ChildCmdCounter);
    UtAssert_INT32_EQ(count_childcmdwarncounter_before, FM_GlobalData.ChildCmdWarnCounter);
}

void Test_FM_ChildDirListFileLoop_OSDirEntryNameIsParentDirectory(void)
{
    // Arrange
    osal_id_t   dirid                 = FM_UT_OBJID_1;
    osal_id_t   fileid                = FM_UT_OBJID_2;
    const char *dummy_directory       = "dummy_directory";
    const char *dummy_filename        = "dummy_filename";
    const char *dummy_dirwithstep     = "dummy_dirwithstep";
    uint8       dummy_getsizetimemode = UT_Utils_Any_uint8();

    FM_GlobalData.ChildCmdErrCounter  = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), OS_SUCCESS);
    os_dirent_t dummy_direntry = {.FileName = FM_PARENT_DIRECTORY};
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &dummy_direntry, sizeof(dummy_direntry), false);
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);

    uint8 count_childcmderrcounter_before  = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_childcmdcounter_before     = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmdwarncounter_before = FM_GlobalData.ChildCmdWarnCounter;

    // Act
    FM_ChildDirListFileLoop(dirid, fileid, dummy_directory, dummy_dirwithstep, dummy_filename, dummy_getsizetimemode);

    uint8 count_sendevent = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdirread = UT_GetStubCount(UT_KEY(OS_DirectoryRead));
    uint8 count_oswrite   = UT_GetStubCount(UT_KEY(OS_write));
    uint8 count_oslseek   = UT_GetStubCount(UT_KEY(OS_lseek));

    // Assert
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(count_osdirread, 2);
    UtAssert_INT32_EQ(count_oswrite, 0);
    UtAssert_INT32_EQ(count_oslseek, 0);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_FILE_CMD_EID);
    UtAssert_INT32_EQ(count_childcmderrcounter_before, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(count_childcmdcounter_before + 1, FM_GlobalData.ChildCmdCounter);
    UtAssert_INT32_EQ(count_childcmdwarncounter_before, FM_GlobalData.ChildCmdWarnCounter);
}

void Test_FM_ChildDirListFileLoop_PathLengthAndEntryLengthGreaterMaxPathLen(void)
{
    // Arrange
    osal_id_t   dirid           = FM_UT_OBJID_1;
    osal_id_t   fileid          = FM_UT_OBJID_2;
    const char *dummy_directory = "dummy_directory";
    const char *dummy_filename  = "dummy_filename";
    const char *dummy_dirwithstep =
        "dummy_dirwithstep_length_is_long_too_long_like_really_long_wish_i_knew_how_to_Make_this_easy";
    uint8 dummy_getsizetimemode = UT_Utils_Any_uint8();

    FM_GlobalData.ChildCmdErrCounter  = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), OS_SUCCESS);
    os_dirent_t dummy_direntry = {.FileName = "dummy_directory_nam"};
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &dummy_direntry, sizeof(dummy_direntry), false);
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);

    uint8 count_childcmderrcounter_before  = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_childcmdcounter_before     = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmdwarncounter_before = FM_GlobalData.ChildCmdWarnCounter;

    // Act
    FM_ChildDirListFileLoop(dirid, fileid, dummy_directory, dummy_dirwithstep, dummy_filename, dummy_getsizetimemode);

    uint8 count_sendevent   = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdirread   = UT_GetStubCount(UT_KEY(OS_DirectoryRead));
    uint8 count_oswrite     = UT_GetStubCount(UT_KEY(OS_write));
    uint8 count_oslseek     = UT_GetStubCount(UT_KEY(OS_lseek));
    uint8 count_direntries  = FM_GlobalData.DirListFileStats.DirEntries;
    uint8 count_fileentries = FM_GlobalData.DirListFileStats.FileEntries;

    // Assert
    UtAssert_INT32_EQ(count_sendevent, 2);
    UtAssert_INT32_EQ(count_osdirread, 2);
    UtAssert_INT32_EQ(count_oswrite, 1);
    UtAssert_INT32_EQ(count_oslseek, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_INFORMATION);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_FILE_WARNING_EID);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[1].EventID, FM_GET_DIR_FILE_CMD_EID);
    UtAssert_INT32_EQ(count_childcmderrcounter_before, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(count_childcmdcounter_before + 1, FM_GlobalData.ChildCmdCounter);
    UtAssert_INT32_EQ(count_childcmdwarncounter_before + 1, FM_GlobalData.ChildCmdWarnCounter);
    UtAssert_INT32_EQ(count_direntries, 1);
    UtAssert_INT32_EQ(count_fileentries, 0);
}

void Test_FM_ChildDirListFileLoop_FileEntriesGreaterFMDirListFileEntries(void)
{
    // Arrange
    osal_id_t   dirid                 = FM_UT_OBJID_1;
    osal_id_t   fileid                = FM_UT_OBJID_2;
    const char *dummy_directory       = "dummy_directory";
    const char *dummy_filename        = "dummy_filename";
    const char *dummy_dirwithstep     = "dummy_dirwithstep";
    uint8       dummy_getsizetimemode = UT_Utils_Any_uint8();

    FM_GlobalData.ChildCmdErrCounter  = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), OS_SUCCESS);
    os_dirent_t dummy_direntry = {.FileName = "dummy_directory_nam"};
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &dummy_direntry, sizeof(dummy_direntry), false);
    // Get number larger than FM_DIR_LIST_FILE_ENTRIES
    int32 dummy_dirread_number =
        UT_Utils_Any_int32_BetweenInclusive(FM_DIR_LIST_FILE_ENTRIES + 1, 2 * FM_DIR_LIST_FILE_ENTRIES);
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), dummy_dirread_number, !OS_SUCCESS);

    uint8 count_childcmderrcounter_before  = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_childcmdcounter_before     = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmdwarncounter_before = FM_GlobalData.ChildCmdWarnCounter;

    // Act
    FM_ChildDirListFileLoop(dirid, fileid, dummy_directory, dummy_dirwithstep, dummy_filename, dummy_getsizetimemode);

    uint8  count_sendevent   = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint32 count_osdirread   = UT_GetStubCount(UT_KEY(OS_DirectoryRead));
    uint32 count_oswrite     = UT_GetStubCount(UT_KEY(OS_write));
    uint8  count_oslseek     = UT_GetStubCount(UT_KEY(OS_lseek));
    uint32 count_direntries  = FM_GlobalData.DirListFileStats.DirEntries;
    uint32 count_fileentries = FM_GlobalData.DirListFileStats.FileEntries;

    // Assert
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(count_osdirread, dummy_dirread_number);
    UtAssert_INT32_EQ(count_oswrite, FM_DIR_LIST_FILE_ENTRIES + 1);
    UtAssert_INT32_EQ(count_oslseek, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_FILE_CMD_EID);
    UtAssert_INT32_EQ(count_childcmderrcounter_before, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(count_childcmdcounter_before + 1, FM_GlobalData.ChildCmdCounter);
    UtAssert_INT32_EQ(count_childcmdwarncounter_before, FM_GlobalData.ChildCmdWarnCounter);
    UtAssert_INT32_EQ(count_direntries, dummy_dirread_number - 1);
    UtAssert_INT32_EQ(count_fileentries, FM_DIR_LIST_FILE_ENTRIES);
}

void Test_FM_ChildDirListFileLoop_BytesWrittenNotEqualWriteLength(void)
{
    // Arrange
    osal_id_t   dirid                 = FM_UT_OBJID_1;
    osal_id_t   fileid                = FM_UT_OBJID_2;
    const char *dummy_directory       = "dummy_directory";
    const char *dummy_filename        = "dummy_filename";
    const char *dummy_dirwithstep     = "dummy_dirwithstep";
    uint8       dummy_getsizetimemode = UT_Utils_Any_uint8();

    FM_GlobalData.ChildCmdErrCounter  = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), OS_SUCCESS);
    os_dirent_t dummy_direntry = {.FileName = "dummy_directory_nam"};
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &dummy_direntry, sizeof(dummy_direntry), false);
    UT_SetDefaultReturnValue(UT_KEY(OS_write), sizeof(FM_DirListEntry_t));
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);

    uint8 count_childcmderrcounter_before  = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_childcmdcounter_before     = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmdwarncounter_before = FM_GlobalData.ChildCmdWarnCounter;

    // Act
    FM_ChildDirListFileLoop(dirid, fileid, dummy_directory, dummy_dirwithstep, dummy_filename, dummy_getsizetimemode);

    uint8 count_sendevent   = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdirread   = UT_GetStubCount(UT_KEY(OS_DirectoryRead));
    uint8 count_oswrite     = UT_GetStubCount(UT_KEY(OS_write));
    uint8 count_oslseek     = UT_GetStubCount(UT_KEY(OS_lseek));
    uint8 count_direntries  = FM_GlobalData.DirListFileStats.DirEntries;
    uint8 count_fileentries = FM_GlobalData.DirListFileStats.FileEntries;

    // Assert
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(count_osdirread, 2);
    UtAssert_INT32_EQ(count_oswrite, 2);
    UtAssert_INT32_EQ(count_oslseek, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_ERROR);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_FILE_UPSTATS_ERR_EID);
    UtAssert_INT32_EQ(count_childcmderrcounter_before + 1, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(count_childcmdcounter_before, FM_GlobalData.ChildCmdCounter);
    UtAssert_INT32_EQ(count_childcmdwarncounter_before, FM_GlobalData.ChildCmdWarnCounter);
    UtAssert_INT32_EQ(count_direntries, 1);
    UtAssert_INT32_EQ(count_fileentries, 1);
}

void Test_FM_ChildDirListFileLoop_WritelengthEqualToBytesWritten(void)
{
    // Arrange
    osal_id_t   dirid                 = FM_UT_OBJID_1;
    osal_id_t   fileid                = FM_UT_OBJID_2;
    const char *dummy_directory       = "dummy_directory";
    const char *dummy_filename        = "dummy_filename";
    const char *dummy_dirwithstep     = "dummy_dirwithstep";
    uint8       dummy_getsizetimemode = UT_Utils_Any_uint8();

    FM_GlobalData.ChildCmdErrCounter  = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdCounter     = UT_Utils_Any_uint8();
    FM_GlobalData.ChildCmdWarnCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_DirectoryRead), OS_SUCCESS);
    os_dirent_t dummy_direntry = {.FileName = "dummy_directory_nam"};
    UT_SetDataBuffer(UT_KEY(OS_DirectoryRead), &dummy_direntry, sizeof(dummy_direntry), false);
    UT_SetDeferredRetcode(UT_KEY(OS_DirectoryRead), 2, !OS_SUCCESS);

    uint8 count_childcmderrcounter_before  = FM_GlobalData.ChildCmdErrCounter;
    uint8 count_childcmdcounter_before     = FM_GlobalData.ChildCmdCounter;
    uint8 count_childcmdwarncounter_before = FM_GlobalData.ChildCmdWarnCounter;

    // Act
    FM_ChildDirListFileLoop(dirid, fileid, dummy_directory, dummy_dirwithstep, dummy_filename, dummy_getsizetimemode);

    uint8 count_sendevent   = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_osdirread   = UT_GetStubCount(UT_KEY(OS_DirectoryRead));
    uint8 count_oswrite     = UT_GetStubCount(UT_KEY(OS_write));
    uint8 count_oslseek     = UT_GetStubCount(UT_KEY(OS_lseek));
    uint8 count_direntries  = FM_GlobalData.DirListFileStats.DirEntries;
    uint8 count_fileentries = FM_GlobalData.DirListFileStats.FileEntries;

    // Assert
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(count_osdirread, 2);
    UtAssert_INT32_EQ(count_oswrite, 2);
    UtAssert_INT32_EQ(count_oslseek, 1);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventType, CFE_EVS_EventType_DEBUG);
    UtAssert_INT32_EQ(context_CFE_EVS_SendEvent[0].EventID, FM_GET_DIR_FILE_CMD_EID);
    UtAssert_INT32_EQ(count_childcmderrcounter_before, FM_GlobalData.ChildCmdErrCounter);
    UtAssert_INT32_EQ(count_childcmdcounter_before + 1, FM_GlobalData.ChildCmdCounter);
    UtAssert_INT32_EQ(count_childcmdwarncounter_before, FM_GlobalData.ChildCmdWarnCounter);
    UtAssert_INT32_EQ(count_direntries, 1);
    UtAssert_INT32_EQ(count_fileentries, 1);
}

/* ****************
 * ChildSizeTimeMode Tests
 * ***************/
void Test_FM_ChildSizeTimeMode_OsStatNoSuccess(void)
{
    // Arrange
    const char *dummy_filename = "dummy_filename";
    uint32      dummy_filesize = UT_Utils_Any_uint32();
    uint32      dummy_filetime = UT_Utils_Any_uint32();
    uint32      dummy_filemode = UT_Utils_Any_uint32();

    UT_SetDefaultReturnValue(UT_KEY(OS_stat), !OS_SUCCESS);

    // Act
    int32 result = FM_ChildSizeTimeMode(dummy_filename, &dummy_filesize, &dummy_filetime, &dummy_filemode);

    uint8 count_osstat = UT_GetStubCount(UT_KEY(OS_stat));

    // Assert
    UtAssert_INT32_EQ(result, !OS_SUCCESS);
    UtAssert_INT32_EQ(count_osstat, 1);
    UtAssert_INT32_EQ(dummy_filesize, 0);
    UtAssert_INT32_EQ(dummy_filetime, 0);
}

void Test_FM_ChildSizeTimeMode_OSFilestateTimeDefined(void)
{
    // Arrange
    const char *dummy_filename = "dummy_filename";
    uint32      dummy_filesize = UT_Utils_Any_uint32();
    uint32      dummy_filetime = UT_Utils_Any_uint32_GreaterThan(0);
    uint32      dummy_filemode = UT_Utils_Any_uint32();

    UT_SetDefaultReturnValue(UT_KEY(OS_stat), OS_SUCCESS);
    OS_time_t  dummy_ostime     = {.ticks = dummy_filetime + 1};
    os_fstat_t dummy_filestatus = {
        .FileModeBits = dummy_filemode, .FileTime = dummy_ostime, .FileSize = dummy_filesize};
    UT_SetDataBuffer(UT_KEY(OS_stat), &dummy_filestatus, sizeof(dummy_filestatus), false);

    // Act
    int32 result = FM_ChildSizeTimeMode(dummy_filename, &dummy_filesize, &dummy_filetime, &dummy_filemode);

    uint8  count_osstat     = UT_GetStubCount(UT_KEY(OS_stat));
    uint32 anticipated_time = OS_FILESTAT_TIME(dummy_filestatus);

    // Assert
    UtAssert_INT32_EQ(result, OS_SUCCESS);
    UtAssert_INT32_EQ(count_osstat, 1);
    UtAssert_INT32_EQ(anticipated_time, dummy_filetime);
}

/* ****************
 * ChildLoop Tests
 * ***************/
void Test_FM_ChildLoop_CountSemTakeNotSuccess(void)
{
    // Arrange
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    UT_SetDefaultReturnValue(UT_KEY(OS_CountSemTake), !CFE_SUCCESS);

    uint8 childcmdqcounter_before = FM_GlobalData.ChildCmdErrCounter;

    // Act
    FM_ChildLoop();

    uint8 count_sendevent    = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_countsemtake = UT_GetStubCount(UT_KEY(OS_CountSemTake));

    // Assert
    UtAssert_INT32_EQ(count_countsemtake, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(FM_CHILD_TERM_SEM_ERR_EID, context_CFE_EVS_SendEvent[0].EventID);
    UtAssert_INT32_EQ(CFE_EVS_EventType_ERROR, context_CFE_EVS_SendEvent[0].EventType);
    UtAssert_INT32_EQ(childcmdqcounter_before, FM_GlobalData.ChildCmdErrCounter);
}

void Test_FM_ChildLoop_ChildQCountEqualZero(void)
{
    // Arrange
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    FM_GlobalData.ChildQueueCount    = 0;
    UT_SetDefaultReturnValue(UT_KEY(OS_CountSemTake), CFE_SUCCESS);

    uint8 childcmdqcounter_before = FM_GlobalData.ChildCmdErrCounter;

    // Act
    FM_ChildLoop();

    uint8 count_sendevent    = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_countsemtake = UT_GetStubCount(UT_KEY(OS_CountSemTake));

    // Assert
    UtAssert_INT32_EQ(count_countsemtake, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(FM_CHILD_TERM_EMPTYQ_ERR_EID, context_CFE_EVS_SendEvent[0].EventID);
    UtAssert_INT32_EQ(CFE_EVS_EventType_ERROR, context_CFE_EVS_SendEvent[0].EventType);
    UtAssert_INT32_EQ(childcmdqcounter_before + 1, FM_GlobalData.ChildCmdErrCounter);
}

void Test_FM_ChildLoop_ChildReadIndexGreaterChildQDepth(void)
{
    // Arrange
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    FM_GlobalData.ChildQueueCount    = 1;
    FM_GlobalData.ChildReadIndex     = FM_CHILD_QUEUE_DEPTH + 1;
    UT_SetDefaultReturnValue(UT_KEY(OS_CountSemTake), CFE_SUCCESS);

    uint8 childcmdqcounter_before = FM_GlobalData.ChildCmdErrCounter;

    // Act
    FM_ChildLoop();

    uint8 count_sendevent    = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_countsemtake = UT_GetStubCount(UT_KEY(OS_CountSemTake));

    // Assert
    UtAssert_INT32_EQ(count_countsemtake, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(FM_CHILD_TERM_QIDX_ERR_EID, context_CFE_EVS_SendEvent[0].EventID);
    UtAssert_INT32_EQ(CFE_EVS_EventType_ERROR, context_CFE_EVS_SendEvent[0].EventType);
    UtAssert_INT32_EQ(childcmdqcounter_before + 1, FM_GlobalData.ChildCmdErrCounter);
}

void Test_FM_ChildLoop_ChildReadIndexEqualChildQDepth(void)
{
    // Arrange
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    FM_GlobalData.ChildQueueCount    = 1;
    FM_GlobalData.ChildReadIndex     = FM_CHILD_QUEUE_DEPTH;
    UT_SetDefaultReturnValue(UT_KEY(OS_CountSemTake), CFE_SUCCESS);

    uint8 childcmdqcounter_before = FM_GlobalData.ChildCmdErrCounter;

    // Act
    FM_ChildLoop();

    uint8 count_sendevent    = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_countsemtake = UT_GetStubCount(UT_KEY(OS_CountSemTake));

    // Assert
    UtAssert_INT32_EQ(count_countsemtake, 1);
    UtAssert_INT32_EQ(count_sendevent, 1);
    UtAssert_INT32_EQ(FM_CHILD_TERM_QIDX_ERR_EID, context_CFE_EVS_SendEvent[0].EventID);
    UtAssert_INT32_EQ(CFE_EVS_EventType_ERROR, context_CFE_EVS_SendEvent[0].EventType);
    UtAssert_INT32_EQ(childcmdqcounter_before + 1, FM_GlobalData.ChildCmdErrCounter);
}

void Test_FM_ChildLoop_CountSemTakeSuccessDefault(void)
{
    // Arrange
    FM_GlobalData.ChildCmdErrCounter = UT_Utils_Any_uint8();
    FM_GlobalData.ChildQueueCount    = 1;
    FM_GlobalData.ChildReadIndex     = 0;
    FM_ChildQueueEntry_t dummy_CC    = {.CommandCode = -1};
    FM_GlobalData.ChildQueue[0]      = dummy_CC;
    UT_SetDefaultReturnValue(UT_KEY(OS_CountSemTake), CFE_SUCCESS);
    UT_SetDeferredRetcode(UT_KEY(OS_CountSemTake), 2, !CFE_SUCCESS);

    uint8 childcmdqcounter_before = FM_GlobalData.ChildCmdErrCounter;

    // Act
    FM_ChildLoop();

    uint8 count_sendevent    = UT_GetStubCount(UT_KEY(CFE_EVS_SendEvent));
    uint8 count_countsemtake = UT_GetStubCount(UT_KEY(OS_CountSemTake));

    // Assert
    UtAssert_INT32_EQ(count_countsemtake, 2);
    UtAssert_INT32_EQ(count_sendevent, 2);
    UtAssert_INT32_EQ(FM_CHILD_EXE_ERR_EID, context_CFE_EVS_SendEvent[0].EventID);
    UtAssert_INT32_EQ(CFE_EVS_EventType_ERROR, context_CFE_EVS_SendEvent[0].EventType);
    UtAssert_INT32_EQ(FM_CHILD_TERM_SEM_ERR_EID, context_CFE_EVS_SendEvent[1].EventID);
    UtAssert_INT32_EQ(CFE_EVS_EventType_ERROR, context_CFE_EVS_SendEvent[1].EventType);
    UtAssert_INT32_EQ(childcmdqcounter_before + 1, FM_GlobalData.ChildCmdErrCounter);
}

/* ****************
 * ChildSleepStat Tests
 * ***************/

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
#ifdef FM_INCLUDE_DECOMPRESS
    UtTest_Add(Test_FM_ChildProcess_FMDecompressCC, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildProcess_FMDecompressCC");
#endif
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

    UtTest_Add(Test_FM_ChildProcess_FMDeleteIntCC, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildProcess_FMDeleteIntCC");

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
    UtTest_Add(Test_FM_ChildDeleteCmd_CommandCodeIsDeleteIntCC, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteCmd_CommandCodeNotDeleteIntCC");

    UtTest_Add(Test_FM_ChildDeleteCmd_OSRenameSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteCmd_OSRenameSuccess");

    UtTest_Add(Test_FM_ChildDeleteCmd_OSRenameNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteCmd_OSRenameNotSuccess");
}

void add_FM_ChildDeleteAllCmd_tests(void)
{
    UtTest_Add(Test_FM_ChildDeleteAllCmd_DirOpenNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteAllCmd_DirOpenNotSuccess");

    UtTest_Add(Test_FM_ChildDeleteAllCmd_DirReadNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteAllCmd_DirReadNotSuccess");

    UtTest_Add(Test_FM_ChildDeleteAllCmd_DirEntryThisDirectory, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteAllCmd_DirEntryThisDirectory");

    UtTest_Add(Test_FM_ChildDeleteAllCmd_DirEntryParentDirectory, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteAllCmd_DirEntryParentDirectory");

    UtTest_Add(Test_FM_ChildDeleteAllCmd_PathFilenameLengthGreaterMaxPthLen, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteAllCmd_PathFilenameLengthGreaterMaxPthLen");

    UtTest_Add(Test_FM_ChildDeleteAllCmd_InvalidFilenameState, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteAllCmd_InvalidFilenameState");

    UtTest_Add(Test_FM_ChildDeleteAllCmd_NotInUseFilenameState, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteAllCmd_NotInUseFilenameState");

    UtTest_Add(Test_FM_ChildDeleteAllCmd_DirectoryFilenameState, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteAllCmd_DirectoryFilenameState");

    UtTest_Add(Test_FM_ChildDeleteAllCmd_OpenFilenameState, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteAllCmd_OpenFilenameState");

    UtTest_Add(Test_FM_ChildDeleteAllCmd_ClosedFilename_OSRmNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteAllCmd_ClosedFilename_OSRmNotSuccess");

    UtTest_Add(Test_FM_ChildDeleteAllCmd_ClosedFilename_OSrmSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteAllCmd_ClosedFilename_OSrmSuccess");

    UtTest_Add(Test_FM_ChildDeleteAllCmd_FilenameStateDefaultReturn, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteAllCmd_FilenameStateDefaultReturn");
}
#ifdef FM_INCLUDE_DECOMPRESS
void add_FM_ChildDecompressCmd_tests(void)
{
    UtTest_Add(Test_FM_ChildDecompressCmd_FSDecompressNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDecompressCmd_FSDecompressNotSuccess");

    UtTest_Add(Test_FM_ChildDecompressCmd_FSDecompressSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDecompressCmd_FSDecompressSuccess");
}
#endif

void add_FM_ChildConcatCmd_tests(void)
{
    UtTest_Add(Test_FM_ChildConcatCmd_OSCpNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildConcatCmd_OSCpNotSuccess");

    UtTest_Add(Test_FM_ChildConcatCmd_OSOpenCreateSourceNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildConcatCmd_OSOpenCreateSourceNotSuccess");

    UtTest_Add(Test_FM_ChildConcatCmd_OSOpenCreateTargetNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildConcatCmd_OSOpenCreateTargetNotSuccess");

    UtTest_Add(Test_FM_ChildConcatCmd_OSReadBytesZero, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildConcatCmd_OSReadBytesZero");

    UtTest_Add(Test_FM_ChildConcatCmd_OSReadBytesLessThanZero, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildConcatCmd_OSReadBytesLessThanZero");

    UtTest_Add(Test_FM_ChildConcatCmd_BytesWrittenNotEqualBytesRead, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildConcatCmd_BytesWrittenNotEqualBytesRead");

    UtTest_Add(Test_FM_ChildConcatCmd_CopyInProgressTrueLoopCountEqualChildFileLoopCount, FM_Test_Setup,
               FM_Test_Teardown, "Test_FM_ChildConcatCmd_CopyInProgressTrueLoopCountEqualChildFileLoopCount");
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

void add_FM_ChildCreateDirCmd_tests(void)
{
    UtTest_Add(Test_FM_ChildCreateDirCmd_OSMkDirNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildCreateDirCmd_OSMkDirNotSuccess");

    UtTest_Add(Test_FM_ChildCreateDirCmd_OSMkDirSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildCreateDirCmd_OSMkDirSuccess");
}

void add_FM_ChildDeleteDirCmd_tests(void)
{

    UtTest_Add(Test_FM_ChildDeleteDirCmd_OSDirectoryOpenNoSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteDirCmd_OSDirectoryOpenNoSuccess");

    UtTest_Add(Test_FM_ChildDeleteDirCmd_OSDirectoryReadNoSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteDirCmd_OSDirectoryReadNoSuccess");

    UtTest_Add(Test_FM_ChildDeleteDirCmd_StrCmpThisDirectoryZero, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteDirCmd_StrCmpThisDirectoryZero");

    UtTest_Add(Test_FM_ChildDeleteDirCmd_StrCmpParentDirectoryZero, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteDirCmd_StrCmpParentDirectoryZero");

    UtTest_Add(Test_FM_ChildDeleteDirCmd_StrcmpNeitherDirectoryNotZero, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteDirCmd_StrcmpNeitherDirectoryNotZero");

    UtTest_Add(Test_FM_ChildDeleteDirCmd_RemoveTheDirIsTrueRmDirSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteDirCmd_RemoveTheDirIsTrueRmDirSuccess");

    UtTest_Add(Test_FM_ChildDeleteDirCmd_RemoveDirTrueOSRmDirNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDeleteDirCmd_RemoveDirTrueOSRmDirNotSuccess");
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

    UtTest_Add(Test_FM_ChildDirListPktCmd_DirListPktTotalFilesLessThanDirListPktFirstFile, FM_Test_Setup,
               FM_Test_Teardown, "Test_FM_ChildDirListPktCmd_DirListPktTotalFilesLessThanDirListPktFirstFile");

    UtTest_Add(Test_FM_ChildDirListPktCmd_DirListPktPacketFilesGreaterDIRListPktEntries, FM_Test_Setup,
               FM_Test_Teardown, "Test_FM_ChildDirListPktCmd_DirListPktPacketFilesGreaterDIRListPktEntries");

    UtTest_Add(Test_FM_ChildDirListPktCmd_EntryLengthGreaterListEntryName, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDirListPktCmd_EntryLengthGreaterListEntryName");

    UtTest_Add(Test_FM_ChildDirListPktCmd_PathAndEntryLengthGreaterMaxPathLength, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDirListPktCmd_PathAndEntryLengthGreaterMaxPathLength");

    UtTest_Add(Test_FM_ChildDirListPktCmd_EntryLengthAndTotalPathLengthSmallEnough, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDirListPktCmd_EntryLengthAndTotalPathLengthSmallEnough");
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

    UtTest_Add(Test_FM_ChildDirListFileLoop_WritelengthEqualToBytesWritten, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildDirListFileLoop_WritelengthEqualToBytesWritten");
}

void add_FM_ChildSizeTimeMode_tests(void)
{
    UtTest_Add(Test_FM_ChildSizeTimeMode_OsStatNoSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildSizeTimeMode_OsStatNoSuccess");

    UtTest_Add(Test_FM_ChildSizeTimeMode_OSFilestateTimeDefined, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildSizeTimeMode_OSFilestateTimeDefined");
}

void add_FM_ChildSleepStat_tests(void) {}

void add_FM_ChildLoop_tests(void)
{
    UtTest_Add(Test_FM_ChildLoop_CountSemTakeNotSuccess, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildLoop_CountSemTakeNotSuccess");

    UtTest_Add(Test_FM_ChildLoop_ChildQCountEqualZero, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildLoop_ChildQCountEqualZero");

    UtTest_Add(Test_FM_ChildLoop_ChildReadIndexGreaterChildQDepth, FM_Test_Setup, FM_Test_Teardown,
               "Test_FM_ChildLoop_ChildReadIndexGreaterChildQDepth");

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
    add_FM_ChildDeleteAllCmd_tests();
#ifdef FM_INCLUDE_DECOMPRESS
    add_FM_ChildDecompressCmd_tests();
#endif
    add_FM_ChildConcatCmd_tests();
    add_FM_ChildFileInfoCmd_tests();
    add_FM_ChildCreateDirCmd_tests();
    add_FM_ChildDeleteDirCmd_tests();
    add_FM_ChildDirListFileCmd_tests();
    add_FM_ChildDirListPktCmd_tests();
    add_FM_ChildSetPermissionsCmd_tests();
    add_FM_ChildDirListFileInit_tests();
    add_FM_ChildDirListFileLoop_tests();
    add_FM_ChildSizeTimeMode_tests();
    add_FM_ChildSleepStat_tests();
    add_FM_ChildLoop_tests();
}
