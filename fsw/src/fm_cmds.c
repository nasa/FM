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
 *  File Manager (FM) Application Ground Commands
 *
 *  Provides functions for the execution of the FM ground commands
 */

#include "cfe.h"
#include "fm_msg.h"
#include "fm_msgdefs.h"
#include "fm_msgids.h"
#include "fm_events.h"
#include "fm_app.h"
#include "fm_cmds.h"
#include "fm_cmd_utils.h"
#include "fm_perfids.h"
#include "fm_platform_cfg.h"
#include "fm_version.h"
#include "fm_verify.h"

#include <string.h>

/**
 * \brief Internal Macro to access the internal payload structure of a message
 *
 * This is done as a macro so it can be applied consistently to all
 * message processing functions, based on the way FM defines its messages.
 */
#define FM_GET_CMD_PAYLOAD(ptr, type) (&((const type *)(ptr))->Payload)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- NOOP                                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_NoopCmd(const CFE_SB_Buffer_t *BufPtr)
{
    const char *CmdText = "No-op";

    CFE_EVS_SendEvent(FM_NOOP_INF_EID, CFE_EVS_EventType_INFORMATION, "%s command: FM version %d.%d.%d.%d", CmdText,
                      FM_MAJOR_VERSION, FM_MINOR_VERSION, FM_REVISION, FM_MISSION_REV);

    return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Reset Counters                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_ResetCountersCmd(const CFE_SB_Buffer_t *BufPtr)
{
    const char *CmdText = "Reset Counters";

    FM_GlobalData.CommandCounter    = 0;
    FM_GlobalData.CommandErrCounter = 0;

    FM_GlobalData.ChildCmdCounter     = 0;
    FM_GlobalData.ChildCmdErrCounter  = 0;
    FM_GlobalData.ChildCmdWarnCounter = 0;

    /* Send command completion event (debug) */
    CFE_EVS_SendEvent(FM_RESET_INF_EID, CFE_EVS_EventType_INFORMATION, "%s command", CmdText);

    return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Copy File                                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_CopyFileCmd(const CFE_SB_Buffer_t *BufPtr)
{
    FM_ChildQueueEntry_t *CmdArgs = NULL;
    const char *          CmdText = "Copy File";
    bool                  CommandResult;

    const FM_OvwSourceTargetFilename_Payload_t *CmdPtr = FM_GET_CMD_PAYLOAD(BufPtr, FM_CopyFileCmd_t);

    /* Verify that overwrite argument is valid */
    CommandResult = FM_VerifyOverwrite(CmdPtr->Overwrite, FM_COPY_OVR_ERR_EID, CmdText);

    /* Verify that source file exists and is not a directory */
    if (CommandResult == true)
    {
        CommandResult = FM_VerifyFileExists(CmdPtr->Source, sizeof(CmdPtr->Source), FM_COPY_SRC_BASE_EID, CmdText);
    }

    /* Verify target filename per the overwrite argument */
    if (CommandResult == true)
    {
        if (CmdPtr->Overwrite == 0)
        {
            CommandResult = FM_VerifyFileNoExist(CmdPtr->Target, sizeof(CmdPtr->Target), FM_COPY_TGT_BASE_EID, CmdText);
        }
        else
        {
            CommandResult = FM_VerifyFileNotOpen(CmdPtr->Target, sizeof(CmdPtr->Target), FM_COPY_TGT_BASE_EID, CmdText);
        }
    }

    /* Check for lower priority child task availability */
    if (CommandResult == true)
    {
        CommandResult = FM_VerifyChildTask(FM_COPY_CHILD_BASE_EID, CmdText);
    }

    /* Prepare command for child task execution */
    if (CommandResult == true)
    {
        CmdArgs = &FM_GlobalData.ChildQueue[FM_GlobalData.ChildWriteIndex];

        /* Set handshake queue command args */
        CmdArgs->CommandCode = FM_COPY_FILE_CC;
        strncpy(CmdArgs->Source1, CmdPtr->Source, OS_MAX_PATH_LEN - 1);
        CmdArgs->Source1[OS_MAX_PATH_LEN - 1] = '\0';

        strncpy(CmdArgs->Target, CmdPtr->Target, OS_MAX_PATH_LEN - 1);
        CmdArgs->Target[OS_MAX_PATH_LEN - 1] = '\0';

        /* Invoke lower priority child task */
        FM_InvokeChildTask();
    }

    return CommandResult;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Move File                                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_MoveFileCmd(const CFE_SB_Buffer_t *BufPtr)
{
    FM_ChildQueueEntry_t *CmdArgs = NULL;
    const char *          CmdText = "Move File";
    bool                  CommandResult;

    const FM_OvwSourceTargetFilename_Payload_t *CmdPtr = FM_GET_CMD_PAYLOAD(BufPtr, FM_MoveFileCmd_t);

    /* Verify that overwrite argument is valid */
    CommandResult = FM_VerifyOverwrite(CmdPtr->Overwrite, FM_MOVE_OVR_ERR_EID, CmdText);

    /* Verify that source file exists and not a directory */
    if (CommandResult == true)
    {
        CommandResult = FM_VerifyFileExists(CmdPtr->Source, sizeof(CmdPtr->Source), FM_MOVE_SRC_BASE_EID, CmdText);
    }

    /* Verify target filename per the overwrite argument */
    if (CommandResult == true)
    {
        if (CmdPtr->Overwrite == 0)
        {
            CommandResult = FM_VerifyFileNoExist(CmdPtr->Target, sizeof(CmdPtr->Target), FM_MOVE_TGT_BASE_EID, CmdText);
        }
        else
        {
            CommandResult = FM_VerifyFileNotOpen(CmdPtr->Target, sizeof(CmdPtr->Target), FM_MOVE_TGT_BASE_EID, CmdText);
        }
    }

    /* Check for lower priority child task availability */
    if (CommandResult == true)
    {
        CommandResult = FM_VerifyChildTask(FM_MOVE_CHILD_BASE_EID, CmdText);
    }

    /* Prepare command for child task execution */
    if (CommandResult == true)
    {
        CmdArgs = &FM_GlobalData.ChildQueue[FM_GlobalData.ChildWriteIndex];

        /* Set handshake queue command args */
        CmdArgs->CommandCode = FM_MOVE_FILE_CC;

        strncpy(CmdArgs->Source1, CmdPtr->Source, OS_MAX_PATH_LEN - 1);
        CmdArgs->Source1[OS_MAX_PATH_LEN - 1] = '\0';

        strncpy(CmdArgs->Target, CmdPtr->Target, OS_MAX_PATH_LEN - 1);
        CmdArgs->Target[OS_MAX_PATH_LEN - 1] = '\0';

        /* Invoke lower priority child task */
        FM_InvokeChildTask();
    }

    return CommandResult;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Rename File                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_RenameFileCmd(const CFE_SB_Buffer_t *BufPtr)
{
    FM_ChildQueueEntry_t *CmdArgs = NULL;
    const char *          CmdText = "Rename File";
    bool                  CommandResult;

    const FM_SourceTargetFileName_Payload_t *CmdPtr = FM_GET_CMD_PAYLOAD(BufPtr, FM_RenameFileCmd_t);

    /* Verify that source file exists and is not a directory */
    CommandResult = FM_VerifyFileExists(CmdPtr->Source, sizeof(CmdPtr->Source), FM_RENAME_SRC_BASE_EID, CmdText);

    /* Verify that target file does not exist */
    if (CommandResult == true)
    {
        CommandResult = FM_VerifyFileNoExist(CmdPtr->Target, sizeof(CmdPtr->Target), FM_RENAME_TGT_BASE_EID, CmdText);
    }

    /* Check for lower priority child task availability */
    if (CommandResult == true)
    {
        CommandResult = FM_VerifyChildTask(FM_RENAME_CHILD_BASE_EID, CmdText);
    }

    /* Prepare command for child task execution */
    if (CommandResult == true)
    {
        CmdArgs = &FM_GlobalData.ChildQueue[FM_GlobalData.ChildWriteIndex];

        /* Set handshake queue command args */
        CmdArgs->CommandCode = FM_RENAME_FILE_CC;

        strncpy(CmdArgs->Source1, CmdPtr->Source, OS_MAX_PATH_LEN - 1);
        CmdArgs->Source1[OS_MAX_PATH_LEN - 1] = '\0';

        strncpy(CmdArgs->Target, CmdPtr->Target, OS_MAX_PATH_LEN - 1);
        CmdArgs->Target[OS_MAX_PATH_LEN - 1] = '\0';

        /* Invoke lower priority child task */
        FM_InvokeChildTask();
    }

    return CommandResult;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Delete File                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_DeleteFileCmd(const CFE_SB_Buffer_t *BufPtr)
{
    FM_ChildQueueEntry_t *CmdArgs = NULL;
    const char *          CmdText = "Delete File";
    bool                  CommandResult;

    const FM_SingleFilename_Payload_t *CmdPtr = FM_GET_CMD_PAYLOAD(BufPtr, FM_DeleteFileCmd_t);

    /* Verify that file exists, is not a directory and is not open */
    CommandResult = FM_VerifyFileClosed(CmdPtr->Filename, sizeof(CmdPtr->Filename), FM_DELETE_SRC_BASE_EID, CmdText);

    /* Check for lower priority child task availability */
    if (CommandResult == true)
    {
        CommandResult = FM_VerifyChildTask(FM_DELETE_CHILD_BASE_EID, CmdText);
    }

    /* Prepare command for child task execution */
    if (CommandResult == true)
    {
        CmdArgs = &FM_GlobalData.ChildQueue[FM_GlobalData.ChildWriteIndex];

        /* Set handshake queue command args - might be global or internal CC */
        CFE_MSG_GetFcnCode(&BufPtr->Msg, &CmdArgs->CommandCode);
        strncpy(CmdArgs->Source1, CmdPtr->Filename, OS_MAX_PATH_LEN - 1);
        CmdArgs->Source1[OS_MAX_PATH_LEN - 1] = '\0';

        /* Invoke lower priority child task */
        FM_InvokeChildTask();
    }

    return CommandResult;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Delete All Files                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_DeleteAllFilesCmd(const CFE_SB_Buffer_t *BufPtr)
{
    const char *          CmdText                     = "Delete All Files";
    char                  DirWithSep[OS_MAX_PATH_LEN] = "\0";
    FM_ChildQueueEntry_t *CmdArgs                     = NULL;
    bool                  CommandResult;

    const FM_DirectoryName_Payload_t *CmdPtr = FM_GET_CMD_PAYLOAD(BufPtr, FM_DeleteAllFilesCmd_t);

    /* Verify that the directory exists */
    CommandResult =
        FM_VerifyDirExists(CmdPtr->Directory, sizeof(CmdPtr->Directory), FM_DELETE_ALL_SRC_BASE_EID, CmdText);

    if (CommandResult == true)
    {
        /* Append a path separator to the end of the directory name */
        strncpy(DirWithSep, CmdPtr->Directory, OS_MAX_PATH_LEN - 1);
        DirWithSep[OS_MAX_PATH_LEN - 1] = '\0';
        FM_AppendPathSep(DirWithSep, OS_MAX_PATH_LEN);

        /* Check for lower priority child task availability */
        CommandResult = FM_VerifyChildTask(FM_DELETE_ALL_CHILD_BASE_EID, CmdText);
    }

    /* Prepare command for child task execution */
    if (CommandResult == true)
    {
        CmdArgs = &FM_GlobalData.ChildQueue[FM_GlobalData.ChildWriteIndex];

        /* Set handshake queue command args */
        CmdArgs->CommandCode = FM_DELETE_ALL_FILES_CC;
        strncpy(CmdArgs->Source1, CmdPtr->Directory, OS_MAX_PATH_LEN - 1);
        CmdArgs->Source1[OS_MAX_PATH_LEN - 1] = '\0';

        strncpy(CmdArgs->Source2, DirWithSep, OS_MAX_PATH_LEN - 1);
        CmdArgs->Source2[OS_MAX_PATH_LEN - 1] = '\0';

        /* Invoke lower priority child task */
        FM_InvokeChildTask();
    }

    return CommandResult;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Decompress File                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_DecompressFileCmd(const CFE_SB_Buffer_t *BufPtr)
{
    const char *          CmdText = "Decompress File";
    FM_ChildQueueEntry_t *CmdArgs = NULL;
    bool                  CommandResult;

    const FM_SourceTargetFileName_Payload_t *CmdPtr = FM_GET_CMD_PAYLOAD(BufPtr, FM_DecompressFileCmd_t);

    /* Verify that source file exists, is not a directory and is not open */
    CommandResult = FM_VerifyFileClosed(CmdPtr->Source, sizeof(CmdPtr->Source), FM_DECOM_SRC_BASE_EID, CmdText);

    /* Verify that target file does not exist */
    if (CommandResult == true)
    {
        CommandResult = FM_VerifyFileNoExist(CmdPtr->Target, sizeof(CmdPtr->Target), FM_DECOM_TGT_BASE_EID, CmdText);
    }

    /* Check for lower priority child task availability */
    if (CommandResult == true)
    {
        CommandResult = FM_VerifyChildTask(FM_DECOM_CHILD_BASE_EID, CmdText);
    }

    /* Prepare command for child task execution */
    if (CommandResult == true)
    {
        CmdArgs = &FM_GlobalData.ChildQueue[FM_GlobalData.ChildWriteIndex];

        /* Set handshake queue command args */
        CmdArgs->CommandCode = FM_DECOMPRESS_FILE_CC;
        snprintf(CmdArgs->Source1, OS_MAX_PATH_LEN, "%s", CmdPtr->Source);
        snprintf(CmdArgs->Target, OS_MAX_PATH_LEN, "%s", CmdPtr->Target);

        /* Invoke lower priority child task */
        FM_InvokeChildTask();
    }

    return CommandResult;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Concatenate Files                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_ConcatFilesCmd(const CFE_SB_Buffer_t *BufPtr)
{
    const char *          CmdText = "Concat Files";
    FM_ChildQueueEntry_t *CmdArgs = NULL;
    bool                  CommandResult;

    const FM_TwoSourceOneTarget_Payload_t *CmdPtr = FM_GET_CMD_PAYLOAD(BufPtr, FM_ConcatFilesCmd_t);

    /* Verify that source file #1 exists, is not a directory and is not open */
    CommandResult = FM_VerifyFileClosed(CmdPtr->Source1, sizeof(CmdPtr->Source1), FM_CONCAT_SRC1_BASE_EID, CmdText);

    /* Verify that source file #2 exists, is not a directory and is not open */
    if (CommandResult == true)
    {
        CommandResult = FM_VerifyFileClosed(CmdPtr->Source2, sizeof(CmdPtr->Source2), FM_CONCAT_SRC2_BASE_EID, CmdText);
    }

    /* Verify that target file does not exist */
    if (CommandResult == true)
    {
        CommandResult = FM_VerifyFileNoExist(CmdPtr->Target, sizeof(CmdPtr->Target), FM_CONCAT_TGT_BASE_EID, CmdText);
    }

    /* Check for lower priority child task availability */
    if (CommandResult == true)
    {
        CommandResult = FM_VerifyChildTask(FM_CONCAT_CHILD_BASE_EID, CmdText);
    }

    /* Prepare command for child task execution */
    if (CommandResult == true)
    {
        CmdArgs = &FM_GlobalData.ChildQueue[FM_GlobalData.ChildWriteIndex];

        /* Set handshake queue command args */
        CmdArgs->CommandCode = FM_CONCAT_FILES_CC;
        strncpy(CmdArgs->Source1, CmdPtr->Source1, OS_MAX_PATH_LEN - 1);
        CmdArgs->Source1[OS_MAX_PATH_LEN - 1] = '\0';
        strncpy(CmdArgs->Source2, CmdPtr->Source2, OS_MAX_PATH_LEN - 1);
        CmdArgs->Source2[OS_MAX_PATH_LEN - 1] = '\0';
        strncpy(CmdArgs->Target, CmdPtr->Target, OS_MAX_PATH_LEN - 1);
        CmdArgs->Target[OS_MAX_PATH_LEN - 1] = '\0';

        /* Invoke lower priority child task */
        FM_InvokeChildTask();
    }

    return CommandResult;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Get File Info                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_GetFileInfoCmd(const CFE_SB_Buffer_t *BufPtr)
{
    const char *          CmdText       = "Get File Info";
    FM_ChildQueueEntry_t *CmdArgs       = NULL;
    bool                  CommandResult = true;
    uint32                FilenameState = FM_NAME_IS_INVALID;

    const FM_FilenameAndCRC_Payload_t *CmdPtr = FM_GET_CMD_PAYLOAD(BufPtr, FM_GetFileInfoCmd_t);

    /* Verify that the source name is valid for a file or directory */
    FilenameState =
        FM_VerifyNameValid(CmdPtr->Filename, sizeof(CmdPtr->Filename), FM_GET_FILE_INFO_SRC_ERR_EID, CmdText);

    if (FilenameState == FM_NAME_IS_INVALID)
    {
        CommandResult = false;
    }

    /* Check for lower priority child task availability */
    if (CommandResult == true)
    {
        CommandResult = FM_VerifyChildTask(FM_FILE_INFO_CHILD_BASE_EID, CmdText);
    }

    /* Prepare command for child task execution */
    if (CommandResult == true)
    {
        CmdArgs = &FM_GlobalData.ChildQueue[FM_GlobalData.ChildWriteIndex];

        /* Set handshake queue command args */
        CmdArgs->CommandCode = FM_GET_FILE_INFO_CC;
        strncpy(CmdArgs->Source1, CmdPtr->Filename, OS_MAX_PATH_LEN - 1);
        CmdArgs->Source1[OS_MAX_PATH_LEN - 1] = '\0';

        CmdArgs->FileInfoState = FilenameState;
        CmdArgs->FileInfoCRC   = CmdPtr->FileInfoCRC;

        /* Global data set during call to FM_VerifyNameValid */
        CmdArgs->FileInfoSize = FM_GlobalData.FileStatSize;
        CmdArgs->FileInfoTime = FM_GlobalData.FileStatTime;
        CmdArgs->Mode         = FM_GlobalData.FileStatMode;

        /* Invoke lower priority child task */
        FM_InvokeChildTask();
    }

    return CommandResult;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Get List of Open Files                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_GetOpenFilesCmd(const CFE_SB_Buffer_t *BufPtr)
{
    const char *CmdText      = "Get Open Files";
    uint32      NumOpenFiles = 0;

    FM_OpenFilesPkt_Payload_t *ReportPtr = &FM_GlobalData.OpenFilesPkt.Payload;

    /* Initialize open files telemetry packet */
    CFE_MSG_Init(CFE_MSG_PTR(FM_GlobalData.OpenFilesPkt.TelemetryHeader), CFE_SB_ValueToMsgId(FM_OPEN_FILES_TLM_MID),
                 sizeof(FM_OpenFilesPkt_t));

    /* Get list of open files and count */
    NumOpenFiles = FM_GetOpenFilesData(ReportPtr->OpenFilesList);

    ReportPtr->NumOpenFiles = NumOpenFiles;

    /* Timestamp and send open files telemetry packet */
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(FM_GlobalData.OpenFilesPkt.TelemetryHeader));
    CFE_SB_TransmitMsg(CFE_MSG_PTR(FM_GlobalData.OpenFilesPkt.TelemetryHeader), true);

    /* Send command completion event (info) */
    CFE_EVS_SendEvent(FM_GET_OPEN_FILES_CMD_INF_EID, CFE_EVS_EventType_INFORMATION, "%s command", CmdText);

    return true;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Create Directory                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_CreateDirectoryCmd(const CFE_SB_Buffer_t *BufPtr)
{
    FM_ChildQueueEntry_t *CmdArgs = NULL;
    const char *          CmdText = "Create Directory";
    bool                  CommandResult;

    const FM_DirectoryName_Payload_t *CmdPtr = FM_GET_CMD_PAYLOAD(BufPtr, FM_CreateDirectoryCmd_t);

    /* Verify that the directory name is not already in use */
    CommandResult =
        FM_VerifyDirNoExist(CmdPtr->Directory, sizeof(CmdPtr->Directory), FM_CREATE_DIR_SRC_BASE_EID, CmdText);

    /* Check for lower priority child task availability */
    if (CommandResult == true)
    {
        CommandResult = FM_VerifyChildTask(FM_CREATE_DIR_CHILD_BASE_EID, CmdText);
    }

    /* Prepare command for child task execution */
    if (CommandResult == true)
    {
        CmdArgs = &FM_GlobalData.ChildQueue[FM_GlobalData.ChildWriteIndex];

        /* Set handshake queue command args */
        CmdArgs->CommandCode = FM_CREATE_DIRECTORY_CC;
        strncpy(CmdArgs->Source1, CmdPtr->Directory, OS_MAX_PATH_LEN - 1);
        CmdArgs->Source1[OS_MAX_PATH_LEN - 1] = '\0';

        /* Invoke lower priority child task */
        FM_InvokeChildTask();
    }

    return CommandResult;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Delete Directory                          */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_DeleteDirectoryCmd(const CFE_SB_Buffer_t *BufPtr)
{
    FM_ChildQueueEntry_t *CmdArgs = NULL;
    const char *          CmdText = "Delete Directory";
    bool                  CommandResult;

    const FM_DirectoryName_Payload_t *CmdPtr = FM_GET_CMD_PAYLOAD(BufPtr, FM_DeleteDirectoryCmd_t);

    /* Verify that the directory exists */
    CommandResult =
        FM_VerifyDirExists(CmdPtr->Directory, sizeof(CmdPtr->Directory), FM_DELETE_DIR_SRC_BASE_EID, CmdText);

    /* Check for lower priority child task availability */
    if (CommandResult == true)
    {
        CommandResult = FM_VerifyChildTask(FM_DELETE_DIR_CHILD_BASE_EID, CmdText);
    }

    /* Prepare command for child task execution */
    if (CommandResult == true)
    {
        CmdArgs = &FM_GlobalData.ChildQueue[FM_GlobalData.ChildWriteIndex];

        /* Set handshake queue command args */
        CmdArgs->CommandCode = FM_DELETE_DIRECTORY_CC;
        strncpy(CmdArgs->Source1, CmdPtr->Directory, OS_MAX_PATH_LEN - 1);
        CmdArgs->Source1[OS_MAX_PATH_LEN - 1] = '\0';

        /* Invoke lower priority child task */
        FM_InvokeChildTask();
    }

    return CommandResult;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Get List of Directory Entries (to file)   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_GetDirListFileCmd(const CFE_SB_Buffer_t *BufPtr)
{
    const char *          CmdText                     = "Directory List to File";
    char                  DirWithSep[OS_MAX_PATH_LEN] = "\0";
    char                  Filename[OS_MAX_PATH_LEN]   = "\0";
    FM_ChildQueueEntry_t *CmdArgs                     = NULL;
    bool                  CommandResult;

    const FM_GetDirectoryToFile_Payload_t *CmdPtr = FM_GET_CMD_PAYLOAD(BufPtr, FM_GetDirListFileCmd_t);

    /* Verify that source directory exists */
    CommandResult =
        FM_VerifyDirExists(CmdPtr->Directory, sizeof(CmdPtr->Directory), FM_GET_DIR_FILE_SRC_BASE_EID, CmdText);

    /* Verify that target file is not already open */
    if (CommandResult == true)
    {
        /* Use default filename if not specified in the command */
        if (CmdPtr->Filename[0] == '\0')
        {
            strncpy(Filename, FM_DIR_LIST_FILE_DEFNAME, sizeof(Filename) - 1);
            Filename[sizeof(Filename) - 1] = '\0';
        }
        else
        {
            memcpy(Filename, CmdPtr->Filename, sizeof(Filename));
        }

        /* Note: it is OK for this file to overwrite a previous version of the file */
        CommandResult = FM_VerifyFileNotOpen(Filename, sizeof(Filename), FM_GET_DIR_FILE_TGT_BASE_EID, CmdText);
    }

    /* Check for lower priority child task availability */
    if (CommandResult == true)
    {
        CommandResult = FM_VerifyChildTask(FM_GET_DIR_FILE_CHILD_BASE_EID, CmdText);
    }

    /* Prepare command for child task execution */
    if (CommandResult == true)
    {
        CmdArgs = &FM_GlobalData.ChildQueue[FM_GlobalData.ChildWriteIndex];

        /* Append a path separator to the end of the directory name */
        strncpy(DirWithSep, CmdPtr->Directory, OS_MAX_PATH_LEN - 1);
        DirWithSep[OS_MAX_PATH_LEN - 1] = '\0';
        FM_AppendPathSep(DirWithSep, OS_MAX_PATH_LEN);

        /* Set handshake queue command args */
        CmdArgs->CommandCode     = FM_GET_DIR_LIST_FILE_CC;
        CmdArgs->GetSizeTimeMode = CmdPtr->GetSizeTimeMode;
        strncpy(CmdArgs->Source1, CmdPtr->Directory, OS_MAX_PATH_LEN - 1);
        CmdArgs->Source1[OS_MAX_PATH_LEN - 1] = '\0';

        strncpy(CmdArgs->Source2, DirWithSep, OS_MAX_PATH_LEN - 1);
        CmdArgs->Source2[OS_MAX_PATH_LEN - 1] = '\0';

        strncpy(CmdArgs->Target, Filename, OS_MAX_PATH_LEN - 1);
        CmdArgs->Target[OS_MAX_PATH_LEN - 1] = '\0';

        /* Invoke lower priority child task */
        FM_InvokeChildTask();
    }

    return CommandResult;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Get List of Directory Entries (to pkt)    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_GetDirListPktCmd(const CFE_SB_Buffer_t *BufPtr)
{
    const char *          CmdText                     = "Directory List to Packet";
    char                  DirWithSep[OS_MAX_PATH_LEN] = "\0";
    FM_ChildQueueEntry_t *CmdArgs                     = NULL;
    bool                  CommandResult;

    const FM_GetDirectoryToPkt_Payload_t *CmdPtr = FM_GET_CMD_PAYLOAD(BufPtr, FM_GetDirListPktCmd_t);

    /* Verify that source directory exists */
    CommandResult =
        FM_VerifyDirExists(CmdPtr->Directory, sizeof(CmdPtr->Directory), FM_GET_DIR_PKT_SRC_BASE_EID, CmdText);

    /* Check for lower priority child task availability */
    if (CommandResult == true)
    {
        CommandResult = FM_VerifyChildTask(FM_GET_DIR_PKT_CHILD_BASE_EID, CmdText);
    }

    /* Prepare command for child task execution */
    if (CommandResult == true)
    {
        CmdArgs = &FM_GlobalData.ChildQueue[FM_GlobalData.ChildWriteIndex];

        /* Append a path separator to the end of the directory name */
        strncpy(DirWithSep, CmdPtr->Directory, OS_MAX_PATH_LEN - 1);
        DirWithSep[OS_MAX_PATH_LEN - 1] = '\0';
        FM_AppendPathSep(DirWithSep, OS_MAX_PATH_LEN);

        /* Set handshake queue command args */
        CmdArgs->CommandCode     = FM_GET_DIR_LIST_PKT_CC;
        CmdArgs->GetSizeTimeMode = CmdPtr->GetSizeTimeMode;
        strncpy(CmdArgs->Source1, CmdPtr->Directory, OS_MAX_PATH_LEN - 1);
        CmdArgs->Source1[OS_MAX_PATH_LEN - 1] = '\0';

        strncpy(CmdArgs->Source2, DirWithSep, OS_MAX_PATH_LEN - 1);
        CmdArgs->Source2[OS_MAX_PATH_LEN - 1] = '\0';
        CmdArgs->DirListOffset                = CmdPtr->DirListOffset;

        /* Invoke lower priority child task */
        FM_InvokeChildTask();
    }

    return CommandResult;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Get File System Free Space                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_MonitorFilesystemSpaceCmd(const CFE_SB_Buffer_t *BufPtr)
{
    const char *CmdText       = "Get Free Space";
    bool        CommandResult = true;
    uint32      i             = 0;
    int32       OpResult;

    const FM_MonitorTableEntry_t *MonitorPtr;
    FM_MonitorReportEntry_t *     ReportPtr;

    /* Verify that we have a pointer to the file system table data */
    if (FM_GlobalData.MonitorTablePtr == NULL)
    {
        CommandResult = false;

        CFE_EVS_SendEvent(FM_GET_FREE_SPACE_TBL_ERR_EID, CFE_EVS_EventType_ERROR,
                          "%s error: file system free space table is not loaded", CmdText);
    }
    else
    {
        /* Initialize the file system free space telemetry packet */
        CFE_MSG_Init(CFE_MSG_PTR(FM_GlobalData.MonitorReportPkt.TelemetryHeader),
                     CFE_SB_ValueToMsgId(FM_FREE_SPACE_TLM_MID), sizeof(FM_MonitorReportPkt_t));

        /* Process enabled file system table entries */
        MonitorPtr = FM_GlobalData.MonitorTablePtr->Entries;
        ReportPtr  = FM_GlobalData.MonitorReportPkt.Payload.FileSys;
        for (i = 0; i < FM_TABLE_ENTRY_COUNT; i++)
        {
            if (MonitorPtr->Type != FM_MonitorTableEntry_Type_UNUSED)
            {
                CFE_SB_MessageStringSet(ReportPtr->Name, MonitorPtr->Name, sizeof(ReportPtr->Name),
                                        sizeof(MonitorPtr->Name));
                ReportPtr->ReportType = MonitorPtr->Type;

                /* Pre-initialize to 0, will be overwritten with real value if successful */
                ReportPtr->Blocks = 0;
                ReportPtr->Bytes  = 0;

                if (MonitorPtr->Enabled)
                {
                    if (MonitorPtr->Type == FM_MonitorTableEntry_Type_VOLUME_FREE_SPACE)
                    {
                        OpResult = FM_GetVolumeFreeSpace(MonitorPtr->Name, &ReportPtr->Blocks, &ReportPtr->Bytes);
                    }
                    else if (MonitorPtr->Type == FM_MonitorTableEntry_Type_DIRECTORY_ESTIMATE)
                    {
                        OpResult =
                            FM_GetDirectorySpaceEstimate(MonitorPtr->Name, &ReportPtr->Blocks, &ReportPtr->Bytes);
                    }
                    else
                    {
                        OpResult = CFE_STATUS_NOT_IMPLEMENTED;
                    }

                    if (OpResult != CFE_SUCCESS)
                    {
                        CommandResult = false;
                    }
                }
            }
            else
            {
                /* Make sure this entry is all clear */
                memset(ReportPtr, 0, sizeof(*ReportPtr));
            }

            ++MonitorPtr;
            ++ReportPtr;
        }

        /* Timestamp and send file system free space telemetry packet */
        CFE_SB_TimeStampMsg(CFE_MSG_PTR(FM_GlobalData.MonitorReportPkt.TelemetryHeader));
        CFE_SB_TransmitMsg(CFE_MSG_PTR(FM_GlobalData.MonitorReportPkt.TelemetryHeader), true);

        /* Send command completion event (info) */
        CFE_EVS_SendEvent(FM_MONITOR_FILESYSTEM_SPACE_CMD_INF_EID, CFE_EVS_EventType_INFORMATION, "%s command",
                          CmdText);
    }

    return CommandResult;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Set Table Entry Enable/Disable State      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_SetTableStateCmd(const CFE_SB_Buffer_t *BufPtr)
{
    const char *CmdText       = "Set Table State";
    bool        CommandResult = true;

    const FM_TableIndexAndState_Payload_t *CmdPtr = FM_GET_CMD_PAYLOAD(BufPtr, FM_SetTableStateCmd_t);

    if (FM_GlobalData.MonitorTablePtr == NULL)
    {
        /* File system table has not been loaded */
        CommandResult = false;

        CFE_EVS_SendEvent(FM_SET_TABLE_STATE_TBL_ERR_EID, CFE_EVS_EventType_ERROR,
                          "%s error: file system free space table is not loaded", CmdText);
    }
    else if (CmdPtr->TableEntryIndex >= FM_TABLE_ENTRY_COUNT)
    {
        /* Table index argument is out of range */
        CommandResult = false;

        CFE_EVS_SendEvent(FM_SET_TABLE_STATE_ARG_IDX_ERR_EID, CFE_EVS_EventType_ERROR,
                          "%s error: invalid command argument: index = %d", CmdText, (int)CmdPtr->TableEntryIndex);
    }
    else if ((CmdPtr->TableEntryState != FM_TABLE_ENTRY_ENABLED) &&
             (CmdPtr->TableEntryState != FM_TABLE_ENTRY_DISABLED))
    {
        /* State argument must be either enabled or disabled */
        CommandResult = false;

        CFE_EVS_SendEvent(FM_SET_TABLE_STATE_ARG_STATE_ERR_EID, CFE_EVS_EventType_ERROR,
                          "%s error: invalid command argument: state = %d", CmdText, (int)CmdPtr->TableEntryState);
    }
    else if (FM_GlobalData.MonitorTablePtr->Entries[CmdPtr->TableEntryIndex].Type == FM_MonitorTableEntry_Type_UNUSED)
    {
        /* Current table entry state must not be unused */
        CommandResult = false;

        CFE_EVS_SendEvent(FM_SET_TABLE_STATE_UNUSED_ERR_EID, CFE_EVS_EventType_ERROR,
                          "%s error: cannot modify unused table entry: index = %d", CmdText,
                          (int)CmdPtr->TableEntryIndex);
    }
    else
    {
        /* Update the table entry state as commanded */
        FM_GlobalData.MonitorTablePtr->Entries[CmdPtr->TableEntryIndex].Enabled = CmdPtr->TableEntryState;

        /* Notify cFE that we have modified the table data */
        CFE_TBL_Modified(FM_GlobalData.MonitorTableHandle);

        /* Send command completion event (info) */
        CFE_EVS_SendEvent(FM_SET_TABLE_STATE_CMD_EID, CFE_EVS_EventType_INFORMATION,
                          "%s command: index = %d, state = %d", CmdText, (int)CmdPtr->TableEntryIndex,
                          (int)CmdPtr->TableEntryState);
    }

    return CommandResult;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FM command handler -- Set Permissions for a file                */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool FM_SetPermissionsCmd(const CFE_SB_Buffer_t *BufPtr)
{
    FM_ChildQueueEntry_t *CmdArgs       = NULL;
    const char *          CmdText       = "Set Permissions";
    bool                  CommandResult = true;
    bool                  FilenameState = FM_NAME_IS_INVALID;

    const FM_FilenameAndMode_Payload_t *CmdPtr = FM_GET_CMD_PAYLOAD(BufPtr, FM_SetPermissionsCmd_t);

    FilenameState = FM_VerifyNameValid(CmdPtr->FileName, sizeof(CmdPtr->FileName), 0, CmdText);

    if (FilenameState == FM_NAME_IS_INVALID)
    {
        CommandResult = false;
    }

    /* Check for lower priority child task availability */
    if (CommandResult == true)
    {
        CommandResult = FM_VerifyChildTask(FM_SET_PERM_ERR_EID, CmdText);
    }

    /* Prepare command for child task execution */
    if (CommandResult == true)
    {
        CmdArgs = &FM_GlobalData.ChildQueue[FM_GlobalData.ChildWriteIndex];
        /* Set handshake queue command args */
        CmdArgs->CommandCode = FM_SET_PERMISSIONS_CC;
        strncpy(CmdArgs->Source1, CmdPtr->FileName, OS_MAX_PATH_LEN - 1);
        CmdArgs->Source1[OS_MAX_PATH_LEN - 1] = '\0';
        CmdArgs->Mode                         = CmdPtr->Mode;

        /* Invoke lower priority child task */
        FM_InvokeChildTask();
    }

    return CommandResult;
}
